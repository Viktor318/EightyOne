/* EightyOne - A Windows emulator of the Sinclair ZX range of computers.
 * Copyright (C) 2003-2025 Michael D Wynne
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// DirectSound output.
//
// The audio thread is also the emulator's clock: for each frame of sound
// it needs, it has the emulator's frame thread run one frame, which hands
// its sound back through Frame().
//
// The circular DirectSound buffer is about a second long, but it is only
// kept filled a little beyond DirectSound's write cursor (DSND_MARGIN_MS).
// What is before the write cursor may already have been mixed - on Windows
// Vista and later DirectSound runs on top of WASAPI, and the write cursor
// can be tens of milliseconds ahead of the play cursor - so it is never
// written. The bytes written and played are counted, so an underrun is
// noticed and writing continues a little after the write cursor. After
// each write, the rest of the buffer is filled with the last sample fading
// to zero: if the emulator is held up (the hardware being changed), the
// buffer plays silence instead of old sound again.

#include <dsound.h>
#include "sound\SoundDX.h"
#include "Language.h"

CDSnd::CDSnd(void)
{
        m_hWnd=NULL;
        m_BitsPerSample=0;
        m_SampleRate=0;
        m_Channels=0;
        m_FPS=0;

        m_ThreadID=0;
        m_ThreadHandle=NULL;
        m_StopEvent=NULL;
        m_Stopping=false;
        m_FrameEvent=NULL;
        m_FrameDone=CreateEvent(NULL, FALSE, FALSE, NULL);

        ZeroMemory(&m_WFE, sizeof(m_WFE));
        m_lpDS=NULL;
        m_lpDSB=NULL;
        m_DXBufLen=0;
        m_DXBufMs=0;
        m_BytesPerMs=0;
        m_FrameBytes=0;

        m_NextPos=0;
        m_LastPlay=0;
        m_LastFrameTime=0;
        m_WrittenTotal=0;
        m_PlayedTotal=0;
        m_LastSample[0]=m_LastSample[1]=0;

        m_QueueLockInit=false;
        m_lpAudioQueue=NULL;
        m_QueueLen=0;
        m_QueueSize=0;
        m_QueueStart=0;
        m_lpFrameBuf=NULL;
}

int CDSnd::Initialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels)
{
        m_BitsPerSample=BitsPerSample;
        m_SampleRate=SampleRate;
        m_Channels=Channels;
        m_FPS=FPS;
        m_hWnd=hWnd;

        // If any essentials haven't been initialised,
        // or DirectSound is already set up

        if (!m_hWnd || !m_FPS || !m_BitsPerSample || !m_SampleRate || !m_Channels) return(0);
        if (m_lpDS) return(0);

        if (!m_QueueLockInit)
        {
                InitializeCriticalSection(&m_QueueLock);
                m_QueueLockInit=true;
        }

        // Set Sound Output Format

	ZeroMemory(&m_WFE, sizeof(m_WFE));
        m_WFE.wFormatTag =  WAVE_FORMAT_PCM;
        m_WFE.nChannels = (WORD)m_Channels;
        m_WFE.nSamplesPerSec = m_SampleRate;
        m_WFE.wBitsPerSample = (WORD)m_BitsPerSample;
        m_WFE.nBlockAlign = (WORD)(m_WFE.nChannels * m_BitsPerSample / 8);
        m_WFE.nAvgBytesPerSec = m_WFE.nSamplesPerSec * m_WFE.nBlockAlign;

        // Buffer lengths: one frame is what CSound::Frame() hands over,
        // the DirectSound buffer is a second long

        m_FrameBytes = (m_SampleRate / m_FPS) * m_WFE.nBlockAlign;
        m_DXBufLen = m_WFE.nAvgBytesPerSec;
        m_DXBufMs = 1000;
        m_BytesPerMs = m_WFE.nAvgBytesPerSec / 1000;

        m_StopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (!m_StopEvent)
        {
                Release();
                return -1;
        }

	//Create DirectSound
        int r = DirectSoundCreate(NULL, &m_lpDS, NULL);
        if (r)
        {
                m_lpDS=NULL;
                Release();
                return(r); // Create DirectSound Failed
        }

	//Set Cooperative Level
        if (m_lpDS->SetCooperativeLevel(m_hWnd, DSSCL_PRIORITY))
        {
                Release();
                return(-1); // SetCooperativeLevel Failed
        }

	//Set the format of the primary buffer. Not needed for the sound to
        //play (DirectSound converts), so a failure here is not an error.
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	LPDIRECTSOUNDBUFFER lpPrimary = NULL;
        if (m_lpDS->CreateSoundBuffer(&dsbd, &lpPrimary, NULL) == DS_OK)
        {
                lpPrimary->SetFormat(&m_WFE);
                lpPrimary->Release();
        }

	//Create Secondary Buffer
        ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS;
	dsbd.dwBufferBytes = m_DXBufLen;
	dsbd.lpwfxFormat = &m_WFE;

        if (m_lpDS->CreateSoundBuffer(&dsbd, &m_lpDSB, NULL))
        {
                m_lpDSB=NULL;
                Release();
                return(-1);// Create Second Sound Buffer Failed
        }

	//New audio Queue

        m_QueueLen = m_FrameBytes * DSND_QUEUE_FRAMES;
	m_lpAudioQueue = new BYTE[m_QueueLen];
        m_lpFrameBuf = new BYTE[m_QueueLen];
        m_QueueStart = 0;
        m_QueueSize = 0;

        // Start with silence

        m_LastSample[0]=m_LastSample[1]=0;
        WriteBuffer(0, NULL, m_DXBufLen);

        return 0;
}

int CDSnd::Play()
{
        // Ensure Sound is initialised before attempting to play sound

        if (!m_lpDSB) return(0);

        if (m_lpDSB->Play(0, 0, DSBPLAY_LOOPING) == DSERR_BUFFERLOST)
        {
                m_lpDSB->Restore();
                m_lpDSB->Play(0, 0, DSBPLAY_LOOPING);
        }

        if (!m_ThreadHandle)
        {
                // Nothing written yet: the thread starts by writing a little
                // after the write cursor
                DWORD play=0, write=0;
                m_lpDSB->GetCurrentPosition(&play, &write);
                m_LastPlay = play;
                m_NextPos = play;
                m_WrittenTotal = 0;
                m_PlayedTotal = 0;
                m_LastFrameTime = GetTickCount();

                m_Stopping = false;
                ResetEvent(m_StopEvent);

                m_ThreadHandle = CreateThread(0, 0, &CallThread, this, 0, &m_ThreadID);
                if(!m_ThreadHandle) return -1; // Cannot create Audio thread.
        }

        return 0;
}

int CDSnd::End()
{
        if (m_ThreadHandle)
        {
                // Ask the audio thread to stop (the stop event also ends its
                // waiting for a frame), letting messages sent to this
                // thread through while waiting

                m_Stopping = true;
                SetEvent(m_StopEvent);

                DWORD start = GetTickCount();
                while(1)
                {
                        DWORD r = MsgWaitForMultipleObjects(1, &m_ThreadHandle,
                                        FALSE, 100, QS_SENDMESSAGE);

                        if (r == WAIT_OBJECT_0) break;

                        if (r == WAIT_OBJECT_0 + 1)
                        {
                                MSG msg;
                                PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
                        }

                        if (GetTickCount() - start > 2000)
                        {
                                TerminateThread(m_ThreadHandle, 0);
                                break;
                        }
                }

                CloseHandle(m_ThreadHandle);
                m_ThreadHandle = NULL;
                m_Stopping = false;
        }

        Release();
        return 0;
}

// Free everything Initialise() made (the thread must not be running)

void CDSnd::Release()
{
        if (m_lpDSB)
        {
                m_lpDSB->Stop();
                m_lpDSB->Release();
                m_lpDSB = NULL;
        }

        if (m_lpDS)
        {
                m_lpDS->Release();
                m_lpDS = NULL;
        }

        if (m_StopEvent)
        {
                CloseHandle(m_StopEvent);
                m_StopEvent = NULL;
        }

        if (m_QueueLockInit) EnterCriticalSection(&m_QueueLock);

	if (m_lpAudioQueue != NULL)
        {
		delete []m_lpAudioQueue;
		m_lpAudioQueue = NULL;
	}

        if (m_lpFrameBuf != NULL)
        {
                delete []m_lpFrameBuf;
                m_lpFrameBuf = NULL;
        }

        m_QueueLen=0;
        m_QueueStart=0;
        m_QueueSize=0;

        if (m_QueueLockInit) LeaveCriticalSection(&m_QueueLock);
}

// Helper function to get around not being able to
// use class functions as windows callback directly
DWORD WINAPI CDSnd::CallThread(LPVOID Param)
{
        CDSnd *instance = (CDSnd *)Param;

        if(!instance) return -1;

        instance->ThreadFN();
        return(0);
}

// Read the cursors, and count how much has been played since last time

void CDSnd::UpdatePlayed(DWORD *play, DWORD *write)
{
        if (m_lpDSB->GetCurrentPosition(play, write) != DS_OK)
        {
                *play = m_LastPlay;
                *write = m_LastPlay;
                return;
        }

        *play %= m_DXBufLen;
        *write %= m_DXBufLen;

        m_PlayedTotal += (*play + m_DXBufLen - m_LastPlay) % m_DXBufLen;
        m_LastPlay = *play;
}

// Write `bytes' bytes at `position': from `data', or if NULL, the last
// sample fading to zero (silence without a click). Returns 0 on success

int CDSnd::WriteBuffer(DWORD position, const BYTE *data, DWORD bytes)
{
        LPVOID part[2];
        DWORD length[2];
        int i;

        if (!bytes) return 0;
        if (bytes > m_DXBufLen) bytes = m_DXBufLen;

        HRESULT hr = m_lpDSB->Lock(position, bytes, &part[0], &length[0],
                                        &part[1], &length[1], 0);
        if (hr == DSERR_BUFFERLOST)
        {
                m_lpDSB->Restore();
                hr = m_lpDSB->Lock(position, bytes, &part[0], &length[0],
                                        &part[1], &length[1], 0);
        }
        if (hr != DS_OK) return 1;

        for(i=0; i<2; i++)
        {
                if (!part[i]) continue;

                if (data)
                {
                        memcpy(part[i], data, length[i]);
                        data += length[i];
                }
                else if (m_BitsPerSample != 16)
                {
                        memset(part[i], 0x80, length[i]);
                }
                else
                {
                        // Fade from the last sample to zero in about 2 ms
                        short *out = (short *)part[i];
                        DWORD j, samples = length[i] / sizeof(short);

                        for(j=0; j<samples; j++)
                        {
                                int c = (m_Channels == 2) ? (j & 1) : 0;

                                if (!m_LastSample[0] && !m_LastSample[1])
                                {
                                        memset(out + j, 0, (samples - j) * sizeof(short));
                                        break;
                                }

                                out[j] = m_LastSample[c];
                                m_LastSample[c] = (short)(m_LastSample[c] * 15 / 16);
                        }
                }
        }

        m_lpDSB->Unlock(part[0], length[0], part[1], length[1]);
        return 0;
}

// Take at most `max' bytes of sound from the queue

DWORD CDSnd::TakeFromQueue(BYTE *dest, DWORD max)
{
        DWORD n, first;

        EnterCriticalSection(&m_QueueLock);

        n = m_QueueSize;
        if (n > max) n = max;

        first = m_QueueLen - m_QueueStart;
        if (first > n) first = n;

        memcpy(dest, m_lpAudioQueue + m_QueueStart, first);
        memcpy(dest + first, m_lpAudioQueue, n - first);

        m_QueueStart = (m_QueueStart + n) % m_QueueLen;
        m_QueueSize -= n;

        LeaveCriticalSection(&m_QueueLock);
        return n;
}

// Ask the emulator's frame thread to run one frame, and wait until it has
// put its sound in the queue (Frame()). False if it did not come: the
// emulator is not running (the hardware is being changed), or we stop.

bool CDSnd::RunEmulatorFrame(void)
{
        HANDLE events[2];

        if (!m_FrameEvent || !m_FrameDone) return false;

        ResetEvent(m_FrameDone);
        SetEvent(m_FrameEvent);

        events[0] = m_FrameDone;
        events[1] = m_StopEvent;
        return WaitForMultipleObjects(2, events, FALSE, SOUND_FRAME_WAIT_MS) == WAIT_OBJECT_0;
}

// Audio thread: keep the DirectSound buffer filled a little beyond the
// write cursor, running one emulated frame for each frame of sound

void CDSnd::ThreadFN()
{
        DWORD play, write, lead, margin, align, n;
        __int64 ahead;
        int frames;
        bool wrote;

        align = m_WFE.nBlockAlign;

        margin = DSND_MARGIN_MS * m_BytesPerMs;
        if (margin < m_FrameBytes * DSND_MARGIN_FRAMES)
                margin = m_FrameBytes * DSND_MARGIN_FRAMES;
        margin = margin / align * align;

        while(1)
        {
                if (WaitForSingleObject(m_StopEvent, DSND_POLL_MS) == WAIT_OBJECT_0)
                        return;

                wrote = false;
                frames = 0;

                while(1)
                {
                        UpdatePlayed(&play, &write);
                        lead = (write + m_DXBufLen - play) % m_DXBufLen;

                        // Underrun: we are not beyond the write cursor any
                        // more, or have not written for so long that playing
                        // may have gone round the buffer. Continue writing a
                        // little after the write cursor.

                        ahead = (__int64)(m_WrittenTotal - m_PlayedTotal);
                        if (ahead < (__int64)lead
                                || GetTickCount() - m_LastFrameTime > m_DXBufMs * 3 / 4)
                        {
                                m_NextPos = write + 5 * m_BytesPerMs;
                                m_NextPos = m_NextPos / align * align % m_DXBufLen;
                                m_WrittenTotal = m_PlayedTotal
                                        + (m_NextPos + m_DXBufLen - play) % m_DXBufLen;
                                ahead = (__int64)(m_WrittenTotal - m_PlayedTotal);
                                m_LastFrameTime = GetTickCount();
                        }

                        // Far enough ahead (or enough frames in one go)

                        if (ahead >= (__int64)(lead + margin) || frames >= 8) break;

                        // Run one frame of the emulator. It puts its sound
                        // in the queue (Frame()); if it does not come, the
                        // emulator is not running: silence, and no more now
                        if (!RunEmulatorFrame()) frames = 8;

                        if (m_Stopping) return;

                        // The frame may have taken long (it waited for us):
                        // look at the cursors again before writing

                        UpdatePlayed(&play, &write);
                        lead = (write + m_DXBufLen - play) % m_DXBufLen;
                        ahead = (__int64)(m_WrittenTotal - m_PlayedTotal);
                        if (ahead < (__int64)lead
                                || GetTickCount() - m_LastFrameTime > m_DXBufMs * 3 / 4)
                        {
                                m_NextPos = write + 5 * m_BytesPerMs;
                                m_NextPos = m_NextPos / align * align % m_DXBufLen;
                                m_WrittenTotal = m_PlayedTotal
                                        + (m_NextPos + m_DXBufLen - play) % m_DXBufLen;
                        }

                        n = TakeFromQueue(m_lpFrameBuf, m_QueueLen);

                        if (n)
                        {
                                WriteBuffer(m_NextPos, m_lpFrameBuf, n);

                                if (m_BitsPerSample == 16 && n >= align)
                                {
                                        short *last = (short *)(m_lpFrameBuf + n - align);
                                        m_LastSample[0] = last[0];
                                        m_LastSample[1] = (m_Channels == 2) ? last[1] : last[0];
                                }
                        }
                        else
                        {
                                // No sound from the emulator (it is not
                                // running): silence keeps the time
                                n = m_FrameBytes;
                                WriteBuffer(m_NextPos, NULL, n);
                        }

                        m_NextPos = (m_NextPos + n) % m_DXBufLen;
                        m_WrittenTotal += n;
                        m_LastFrameTime = GetTickCount();
                        wrote = true;
                        frames++;
                }

                // Fill the rest of the buffer, up to the play cursor, with the
                // last sample fading to zero, so an underrun plays silence and
                // not old sound. WriteBuffer() fades m_LastSample, keep a copy.

                if (wrote)
                {
                        DWORD fill = 0;
                        short saved[2];

                        ahead = (__int64)(m_WrittenTotal - m_PlayedTotal);
                        if (ahead >= 0 && ahead + align < (__int64)m_DXBufLen)
                                fill = (m_DXBufLen - (DWORD)ahead - align) / align * align;

                        if (fill)
                        {
                                saved[0] = m_LastSample[0];
                                saved[1] = m_LastSample[1];
                                WriteBuffer(m_NextPos, NULL, fill);
                                m_LastSample[0] = saved[0];
                                m_LastSample[1] = saved[1];
                        }
                }
        }
}

// Copy a sound frame from the emulator into the Sound Queue. If the
// emulator gets too far ahead, the oldest sound is dropped.

void CDSnd::Frame( unsigned char *data, unsigned int len )
{
        DWORD pos, first, drop;

        if (!m_QueueLockInit) return;

        EnterCriticalSection(&m_QueueLock);

        if (!m_lpAudioQueue || !m_QueueLen)
        {
                LeaveCriticalSection(&m_QueueLock);
                return;
        }

        if (len > m_QueueLen)
        {
                data += len - m_QueueLen;
                len = m_QueueLen;
        }

        if (m_QueueSize + len > m_QueueLen)
        {
                drop = m_QueueSize + len - m_QueueLen;
                m_QueueStart = (m_QueueStart + drop) % m_QueueLen;
                m_QueueSize -= drop;
        }

        pos = (m_QueueStart + m_QueueSize) % m_QueueLen;

        first = m_QueueLen - pos;
        if (first > len) first = len;

        memcpy(m_lpAudioQueue + pos, data, first);
        memcpy(m_lpAudioQueue, data + first, len - first);

        m_QueueSize += len;

        LeaveCriticalSection(&m_QueueLock);

        if (m_FrameDone) SetEvent(m_FrameDone);
}
