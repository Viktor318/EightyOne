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

// sound support based on the beeper/AY code I (Mike) wrote for Fuse.

#ifndef _DSND_H
#define _DSND_H

#include <windows.h>
#include <mmreg.h>
#include <dsound.h>

// How far ahead of DirectSound's write cursor the audio thread keeps the
// buffer filled. The emulator runs one frame for each frame of sound
// written, so this is also how far the emulation is ahead of what is heard.

#define DSND_MARGIN_MS          40      // at least this much...
#define DSND_MARGIN_FRAMES      2       // ...and at least this many frames
#define DSND_POLL_MS            5       // how often the audio thread looks
#define DSND_QUEUE_FRAMES       4       // frames the emulator may get ahead

class CDSnd
{
public:
        CDSnd();
	int Initialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels);
        int Play();
	int End();
        void Frame(unsigned char *data, unsigned int len);

        // True while End() is waiting for the audio thread to finish: a
        // frame the thread asked for just before must not be run then
        bool Stopping() { return m_Stopping; }

private:
        void ThreadFN(void);
        static DWORD WINAPI CallThread(LPVOID Param);

        void Release(void);
        void UpdatePlayed(DWORD *play, DWORD *write);
        int WriteBuffer(DWORD position, const BYTE *data, DWORD bytes);
        DWORD TakeFromQueue(BYTE *dest, DWORD max);

        HWND m_hWnd;
        int m_BitsPerSample;
        int m_SampleRate;
        int m_Channels;
        int m_FPS;

        DWORD m_ThreadID;
        HANDLE m_ThreadHandle;
        HANDLE m_StopEvent;
        volatile bool m_Stopping;
        bool m_SendErrorShown;

        // DirectSound
	WAVEFORMATEX m_WFE;
	LPDIRECTSOUND m_lpDS;
	LPDIRECTSOUNDBUFFER m_lpDSB;
        DWORD m_DXBufLen;               // the circular buffer, about 1 second
        DWORD m_DXBufMs;
        DWORD m_BytesPerMs;
        DWORD m_FrameBytes;             // one emulated frame of sound

        // Position in the circular buffer. The bytes written and played are
        // also counted, so an underrun (playing overtaking writing) can be
        // told apart from being far ahead.
        DWORD m_NextPos;
        DWORD m_LastPlay;
        DWORD m_LastFrameTime;
        unsigned __int64 m_WrittenTotal;
        unsigned __int64 m_PlayedTotal;
        short m_LastSample[2];

        // Frames from the emulator waiting to be written (a circular queue)
        CRITICAL_SECTION m_QueueLock;
        bool m_QueueLockInit;
	LPBYTE m_lpAudioQueue;
        DWORD m_QueueLen;
        DWORD m_QueueSize;
        DWORD m_QueueStart;
        LPBYTE m_lpFrameBuf;            // one frame, taken from the queue
};

#endif

