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

// WASAPI output, in shared mode on the default device.
//
// The device is opened at the sample rate of the Windows mixer, and the
// emulator mixes at that rate, so the sound is not resampled; it is only
// converted to the mixer's sample format (usually 32 bit float) and channels.
//
// As with DirectSound, the audio thread is the emulator's clock: it keeps a
// little sound (WASAPI_MARGIN_MS) waiting for the audio engine, running one
// emulated frame for each frame of sound it writes. If the engine runs out
// (the emulator was held up), it plays silence by itself; old sound is
// never repeated.
//
// If the device stops working (unplugged, or the audio service stopped), or
// another device becomes the default, the emulator is kept running by a
// timer, and the default device is opened again - at once after a change,
// every WASAPI_RETRY_MS otherwise. If its mixer runs at another rate,
// Windows converts the sound.
//
// The audio thread owns all the WASAPI objects. Nothing here is linked to
// WASAPI: on Windows XP creating the device enumerator just fails.

#include <windows.h>
#include <objbase.h>
#include <mmreg.h>
#include "sound\SoundWASAPI.h"
#include "Language.h"

#ifndef WAVE_FORMAT_EXTENSIBLE
#define WAVE_FORMAT_EXTENSIBLE  0xFFFE
#endif

#define WAS_eRender             0
#define WAS_eConsole            0
#define WAS_SHAREMODE_SHARED    0

#define WAS_STREAMFLAGS_AUTOCONVERTPCM          0x80000000
#define WAS_STREAMFLAGS_SRC_DEFAULT_QUALITY     0x08000000

//---------------------------------------------------------------------------
// The WASAPI interfaces (from mmdeviceapi.h and audioclient.h, which the
// compiler does not have)

static const GUID WAS_CLSID_MMDeviceEnumerator =
        { 0xBCDE0395, 0xE52F, 0x467C, { 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E } };
static const GUID WAS_IID_IMMDeviceEnumerator =
        { 0xA95664D2, 0x9614, 0x4F35, { 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6 } };
static const GUID WAS_IID_IAudioClient =
        { 0x1CB9AD4C, 0xDBFA, 0x4C32, { 0xB1, 0x78, 0xC2, 0xF5, 0x68, 0xA7, 0x03, 0xB2 } };
static const GUID WAS_IID_IAudioRenderClient =
        { 0xF294ACFC, 0x3146, 0x4483, { 0xA7, 0xBF, 0xAD, 0xDC, 0xA7, 0xC2, 0x60, 0xE2 } };
static const GUID WAS_IID_IMMNotificationClient =
        { 0x7991EEC9, 0x7E89, 0x4D85, { 0x83, 0x90, 0x6C, 0x70, 0x3C, 0xEC, 0x60, 0xC0 } };
static const GUID WAS_IID_IUnknown =
        { 0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

// KSDATAFORMAT_SUBTYPE_PCM and _IEEE_FLOAT are this with 1 and 3 first
static const GUID WAS_SubTypeBase =
        { 0x00000000, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };

struct IMMDevice : public IUnknown
{
        virtual HRESULT STDMETHODCALLTYPE Activate(REFIID iid, DWORD context, void *params, void **object) = 0;
        virtual HRESULT STDMETHODCALLTYPE OpenPropertyStore(DWORD access, void **store) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetId(LPWSTR *id) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetState(DWORD *state) = 0;
};

struct IMMDeviceEnumerator : public IUnknown
{
        virtual HRESULT STDMETHODCALLTYPE EnumAudioEndpoints(int flow, DWORD states, void **devices) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetDefaultAudioEndpoint(int flow, int role, IMMDevice **device) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetDevice(LPCWSTR id, IMMDevice **device) = 0;
        virtual HRESULT STDMETHODCALLTYPE RegisterEndpointNotificationCallback(IMMNotificationClient *client) = 0;
        virtual HRESULT STDMETHODCALLTYPE UnregisterEndpointNotificationCallback(IMMNotificationClient *client) = 0;
};

struct IAudioClient : public IUnknown
{
        virtual HRESULT STDMETHODCALLTYPE Initialize(int shareMode, DWORD flags, __int64 bufferDuration,
                        __int64 periodicity, const WAVEFORMATEX *format, const GUID *session) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetBufferSize(UINT32 *frames) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetStreamLatency(__int64 *latency) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetCurrentPadding(UINT32 *frames) = 0;
        virtual HRESULT STDMETHODCALLTYPE IsFormatSupported(int shareMode, const WAVEFORMATEX *format,
                        WAVEFORMATEX **closest) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetMixFormat(WAVEFORMATEX **format) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetDevicePeriod(__int64 *defaultPeriod, __int64 *minimumPeriod) = 0;
        virtual HRESULT STDMETHODCALLTYPE Start(void) = 0;
        virtual HRESULT STDMETHODCALLTYPE Stop(void) = 0;
        virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetEventHandle(HANDLE event) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetService(REFIID iid, void **service) = 0;
};

struct IAudioRenderClient : public IUnknown
{
        virtual HRESULT STDMETHODCALLTYPE GetBuffer(UINT32 frames, BYTE **data) = 0;
        virtual HRESULT STDMETHODCALLTYPE ReleaseBuffer(UINT32 frames, DWORD flags) = 0;
};

static bool SameGUID(const GUID &a, const GUID &b)
{
        return memcmp(&a, &b, sizeof(GUID)) == 0;
}

//---------------------------------------------------------------------------
// CWasapiNotify: called by Windows on a thread of its own, so it only sets
// a flag for the audio thread

HRESULT STDMETHODCALLTYPE CWasapiNotify::QueryInterface(REFIID riid, void **object)
{
        if (!object) return E_POINTER;

        if (SameGUID(riid, WAS_IID_IUnknown) || SameGUID(riid, WAS_IID_IMMNotificationClient))
        {
                *object = (IMMNotificationClient *)this;
                return S_OK;
        }

        *object = NULL;
        return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE CWasapiNotify::OnDefaultDeviceChanged(int flow, int role, LPCWSTR id)
{
        if (flow == WAS_eRender && role == WAS_eConsole)
                InterlockedExchange((LONG *)&Changed, 1);
        return S_OK;
}

//---------------------------------------------------------------------------
// CWasapiSnd

CWasapiSnd::CWasapiSnd(void)
{
        m_hWnd=NULL;
        m_FPS=0;
        m_SampleRate=0;
        m_Channels=0;
        m_NeedDevice=true;
        m_OpenLater=true;

        m_ThreadID=0;
        m_ThreadHandle=NULL;
        m_StopEvent=NULL;
        m_PlayEvent=NULL;
        m_ReadyEvent=NULL;
        m_Stopping=false;
        m_OpenOK=false;
        m_DeviceOpen=false;
        m_FrameEvent=NULL;
        m_FrameDone=CreateEvent(NULL, FALSE, FALSE, NULL);

        m_Enumerator=NULL;
        m_Client=NULL;
        m_Render=NULL;
        m_NotifyRegistered=false;
        m_BufferFrames=0;
        m_TargetFrames=0;
        m_DevChannels=0;
        m_DevBytes=0;
        m_DevFloat=false;

        m_ClockFreq.QuadPart=1;
        m_ClockStart.QuadPart=0;
        m_ClockFrames=0;
        m_RetryTime=0;

        m_FrameBytes=0;
        m_FrameFrames=0;
        m_LastSample[0]=m_LastSample[1]=0;

        m_QueueLockInit=false;
        m_lpAudioQueue=NULL;
        m_QueueLen=0;
        m_QueueSize=0;
        m_QueueStart=0;
        m_lpFrameBuf=NULL;
}

int CWasapiSnd::Initialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels)
{
        return Start(hWnd, FPS, BitsPerSample, SampleRate, Channels, true, true);
}

int CWasapiSnd::InitialiseNoDevice(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels,
                                   bool openLater)
{
        return Start(hWnd, FPS, BitsPerSample, SampleRate, Channels, false, openLater);
}

// Start the audio thread, which opens the device (and waits for Play()).
// The sound from the emulator is 16 bit, mono or stereo.

int CWasapiSnd::Start(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels,
                      bool needDevice, bool openLater)
{
        if (m_ThreadHandle) return 0;
        if (!hWnd || FPS <= 0 || BitsPerSample != 16 || SampleRate <= 0
                || (Channels != 1 && Channels != 2)) return -1;

        m_hWnd=hWnd;
        m_FPS=FPS;
        m_SampleRate=SampleRate;
        m_Channels=Channels;
        m_NeedDevice=needDevice;
        m_OpenLater=openLater;

        if (!m_QueueLockInit)
        {
                InitializeCriticalSection(&m_QueueLock);
                m_QueueLockInit=true;
        }

        m_StopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        m_PlayEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        m_ReadyEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (!m_StopEvent || !m_PlayEvent || !m_ReadyEvent)
        {
                Release();
                return -1;
        }

        m_Stopping=false;
        m_OpenOK=false;
        m_DeviceOpen=false;

        m_ThreadHandle = CreateThread(0, 0, &CallThread, this, 0, &m_ThreadID);
        if (!m_ThreadHandle)
        {
                Release();
                return -1;
        }

        // The thread opens the device: did it work?
        if (WaitForSingleObject(m_ReadyEvent, WASAPI_START_MS) != WAIT_OBJECT_0 || !m_OpenOK)
        {
                End();
                return -1;
        }

        // The rate is now the device's
        m_FrameFrames = m_SampleRate / m_FPS;
        m_FrameBytes = m_FrameFrames * m_Channels * sizeof(short);

        EnterCriticalSection(&m_QueueLock);
        m_QueueLen = m_FrameBytes * WASAPI_QUEUE_FRAMES;
        m_lpAudioQueue = new BYTE[m_QueueLen];
        m_lpFrameBuf = new BYTE[m_QueueLen];
        m_QueueStart = 0;
        m_QueueSize = 0;
        LeaveCriticalSection(&m_QueueLock);

        return 0;
}

int CWasapiSnd::Play()
{
        if (m_ThreadHandle && m_PlayEvent) SetEvent(m_PlayEvent);
        return 0;
}

int CWasapiSnd::End()
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

// Free everything Start() made (the thread must not be running)

void CWasapiSnd::Release()
{
        if (m_StopEvent) { CloseHandle(m_StopEvent); m_StopEvent = NULL; }
        if (m_PlayEvent) { CloseHandle(m_PlayEvent); m_PlayEvent = NULL; }
        if (m_ReadyEvent) { CloseHandle(m_ReadyEvent); m_ReadyEvent = NULL; }

        m_DeviceOpen = false;

        if (m_QueueLockInit) EnterCriticalSection(&m_QueueLock);

        if (m_lpAudioQueue) { delete []m_lpAudioQueue; m_lpAudioQueue = NULL; }
        if (m_lpFrameBuf) { delete []m_lpFrameBuf; m_lpFrameBuf = NULL; }
        m_QueueLen=0;
        m_QueueStart=0;
        m_QueueSize=0;

        if (m_QueueLockInit) LeaveCriticalSection(&m_QueueLock);
}

DWORD WINAPI CWasapiSnd::CallThread(LPVOID Param)
{
        CWasapiSnd *instance = (CWasapiSnd *)Param;

        if (!instance) return (DWORD)-1;

        instance->ThreadFN();
        return 0;
}

//---------------------------------------------------------------------------
// The device

// Open the default device, at `rate' (0: at the rate of its mixer).
// Not started yet.

bool CWasapiSnd::OpenDevice(int rate)
{
        IMMDevice *device = NULL;
        WAVEFORMATEX *mix = NULL;
        WAVEFORMATEX own;
        const WAVEFORMATEX *format;
        DWORD flags = 0;
        __int64 period = 0, minimum = 0;
        UINT32 periodFrames, frameFrames, most;
        HRESULT hr;

        CloseDevice();

        if (!m_Enumerator)
        {
                hr = CoCreateInstance(WAS_CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL,
                                WAS_IID_IMMDeviceEnumerator, (void **)&m_Enumerator);
                if (FAILED(hr) || !m_Enumerator)
                {
                        m_Enumerator = NULL;    // no WASAPI (Windows XP)
                        return false;
                }

                m_NotifyRegistered = SUCCEEDED(
                        m_Enumerator->RegisterEndpointNotificationCallback(&m_Notify));
        }

        hr = m_Enumerator->GetDefaultAudioEndpoint(WAS_eRender, WAS_eConsole, &device);
        if (FAILED(hr) || !device) return false;

        hr = device->Activate(WAS_IID_IAudioClient, CLSCTX_ALL, NULL, (void **)&m_Client);
        device->Release();
        if (FAILED(hr) || !m_Client)
        {
                m_Client = NULL;
                return false;
        }

        hr = m_Client->GetMixFormat(&mix);
        if (FAILED(hr) || !mix)
        {
                CloseDevice();
                return false;
        }

        if (rate <= 0) rate = mix->nSamplesPerSec;

        // The mixer's own format if the rate is the same and the samples
        // are ones written here (16, 24 or 32 bit integers, 32 bit float)

        format = NULL;
        if ((int)mix->nSamplesPerSec == rate && mix->nChannels > 0)
        {
                WORD tag = mix->wFormatTag;
                int bits = mix->wBitsPerSample;

                if (tag == WAVE_FORMAT_EXTENSIBLE && mix->cbSize >= 22)
                {
                        GUID sub;

                        memcpy(&sub, (BYTE *)mix + sizeof(WAVEFORMATEX) + 6, sizeof(GUID));
                        tag = 0;
                        if (memcmp((BYTE *)&sub + 4, (BYTE *)&WAS_SubTypeBase + 4, sizeof(GUID) - 4) == 0)
                                tag = (WORD)sub.Data1;
                }

                if (tag == WAVE_FORMAT_IEEE_FLOAT && bits == 32)
                {
                        m_DevFloat = true;
                        format = mix;
                }
                else if (tag == WAVE_FORMAT_PCM && (bits == 16 || bits == 24 || bits == 32))
                {
                        m_DevFloat = false;
                        format = mix;
                }

                if (format)
                {
                        m_DevChannels = mix->nChannels;
                        m_DevBytes = bits / 8;
                        if (mix->nBlockAlign != m_DevChannels * m_DevBytes) format = NULL;
                }
        }

        // Otherwise the emulator's format, converted by Windows
        if (!format)
        {
                ZeroMemory(&own, sizeof(own));
                own.wFormatTag = WAVE_FORMAT_PCM;
                own.nChannels = 2;
                own.nSamplesPerSec = rate;
                own.wBitsPerSample = 16;
                own.nBlockAlign = 4;
                own.nAvgBytesPerSec = rate * 4;

                m_DevFloat = false;
                m_DevChannels = 2;
                m_DevBytes = 2;
                format = &own;
                flags = WAS_STREAMFLAGS_AUTOCONVERTPCM | WAS_STREAMFLAGS_SRC_DEFAULT_QUALITY;
        }

        hr = m_Client->Initialize(WAS_SHAREMODE_SHARED, flags,
                        (__int64)WASAPI_BUFFER_MS * 10000, 0, format, NULL);
        CoTaskMemFree(mix);

        if (FAILED(hr)
                || FAILED(m_Client->GetBufferSize(&m_BufferFrames))
                || FAILED(m_Client->GetService(WAS_IID_IAudioRenderClient, (void **)&m_Render))
                || !m_Render)
        {
                m_Render = NULL;
                CloseDevice();
                return false;
        }

        // How much to keep waiting: enough for a few device periods, but
        // room must be left for a frame more
        if (FAILED(m_Client->GetDevicePeriod(&period, &minimum)) || period <= 0)
                period = 100000;                        // 10 ms
        periodFrames = (UINT32)(period * rate / 10000000);
        frameFrames = rate / m_FPS;

        m_TargetFrames = rate * WASAPI_MARGIN_MS / 1000;
        if (m_TargetFrames < periodFrames * WASAPI_MARGIN_PERIODS)
                m_TargetFrames = periodFrames * WASAPI_MARGIN_PERIODS;

        most = m_BufferFrames > frameFrames * 2 ? m_BufferFrames - frameFrames * 2 : m_BufferFrames / 2;
        if (m_TargetFrames > most) m_TargetFrames = most;

        m_SampleRate = rate;
        m_LastSample[0] = m_LastSample[1] = 0;
        m_DeviceOpen = true;
        return true;
}

void CWasapiSnd::CloseDevice(void)
{
        m_DeviceOpen = false;

        if (m_Client) m_Client->Stop();

        if (m_Render)
        {
                m_Render->Release();
                m_Render = NULL;
        }

        if (m_Client)
        {
                m_Client->Release();
                m_Client = NULL;
        }

        m_BufferFrames = 0;
}

// Write `frames' frames of the emulator's sound, converted to the device's
// format; if `data' is NULL, the last sample fading to zero (about 2 ms)

bool CWasapiSnd::WriteDevice(const short *data, UINT32 frames)
{
        BYTE *out;
        UINT32 i;
        int c, l, r, v;

        if (!frames) return true;
        if (FAILED(m_Render->GetBuffer(frames, &out)) || !out) return false;

        for (i=0; i<frames; i++)
        {
                if (data)
                {
                        l = data[i * m_Channels];
                        r = (m_Channels == 2) ? data[i * m_Channels + 1] : l;
                        m_LastSample[0] = (short)l;
                        m_LastSample[1] = (short)r;
                }
                else
                {
                        l = m_LastSample[0];
                        r = m_LastSample[1];
                        m_LastSample[0] = (short)(l * 15 / 16);
                        m_LastSample[1] = (short)(r * 15 / 16);
                }

                for (c=0; c<m_DevChannels; c++)
                {
                        // Left and right on the first two channels, nothing
                        // on the others; both on a single one
                        if (m_DevChannels == 1) v = (l + r) / 2;
                        else if (c == 0) v = l;
                        else if (c == 1) v = r;
                        else v = 0;

                        switch (m_DevBytes)
                        {
                        case 4:
                                if (m_DevFloat) *(float *)out = (float)v / 32768.0f;
                                else *(long *)out = (long)v * 65536L;
                                break;
                        case 3:
                                out[0] = 0;
                                out[1] = (BYTE)(v & 0xFF);
                                out[2] = (BYTE)((v >> 8) & 0xFF);
                                break;
                        default:
                                *(short *)out = (short)v;
                                break;
                        }
                        out += m_DevBytes;
                }
        }

        return SUCCEEDED(m_Render->ReleaseBuffer(frames, 0));
}

//---------------------------------------------------------------------------
// The audio thread

// Ask the emulator's frame thread to run one frame, and wait until it has
// put its sound in the queue (Frame()). False if it did not come: the
// emulator is not running (the hardware is being changed), or we stop.

bool CWasapiSnd::RunEmulatorFrame(void)
{
        HANDLE events[2];

        if (!m_FrameEvent || !m_FrameDone) return false;

        ResetEvent(m_FrameDone);
        SetEvent(m_FrameEvent);

        events[0] = m_FrameDone;
        events[1] = m_StopEvent;
        return WaitForMultipleObjects(2, events, FALSE, SOUND_FRAME_WAIT_MS) == WAIT_OBJECT_0;
}

// Keep a little sound waiting for the audio engine. False if the device
// does not work any more.

bool CWasapiSnd::FillDevice(void)
{
        UINT32 padding, frames;
        DWORD n;
        int count = 0;

        while(1)
        {
                if (FAILED(m_Client->GetCurrentPadding(&padding))) return false;

                // Enough waiting (or enough frames in one go)
                if (padding >= m_TargetFrames || count >= 8) return true;

                bool ran = RunEmulatorFrame();
                if (m_Stopping) return true;

                n = TakeFromQueue(m_lpFrameBuf, m_QueueLen);

                // No sound from the emulator (it is not running): the last
                // sample fading to silence keeps the time
                frames = n ? n / (m_Channels * sizeof(short)) : m_FrameFrames;

                if (FAILED(m_Client->GetCurrentPadding(&padding))) return false;
                if (padding >= m_BufferFrames) frames = 0;
                else if (frames > m_BufferFrames - padding) frames = m_BufferFrames - padding;

                if (!WriteDevice(n ? (const short *)m_lpFrameBuf : NULL, frames)) return false;
                count++;

                // The emulator is not running: no more for now
                if (!ran) return true;
        }
}

void CWasapiSnd::StartClock(void)
{
        QueryPerformanceCounter(&m_ClockStart);
        m_ClockFrames = 0;
}

// Without a device: run the frames that are due by the clock; their sound
// is thrown away

void CWasapiSnd::RunByClock(void)
{
        LARGE_INTEGER now;
        __int64 due;

        QueryPerformanceCounter(&now);
        due = (now.QuadPart - m_ClockStart.QuadPart) * m_FPS / m_ClockFreq.QuadPart;

        // Held up for long (a dialog was opened): no catching up
        if (due - m_ClockFrames > 8) m_ClockFrames = due - 1;

        while (m_ClockFrames < due)
        {
                if (!RunEmulatorFrame())
                {
                        // The emulator is not running: no catching up
                        m_ClockFrames = due;
                        return;
                }
                if (m_Stopping) return;

                TakeFromQueue(m_lpFrameBuf, m_QueueLen);
                m_ClockFrames++;
        }
}

void CWasapiSnd::ThreadFN()
{
        HANDLE events[2];
        bool com = SUCCEEDED(CoInitialize(NULL));

        if (!QueryPerformanceFrequency(&m_ClockFreq) || m_ClockFreq.QuadPart <= 0)
                m_ClockFreq.QuadPart = 1000;

        // Open the device; without one, carry on only if asked to

        if (m_NeedDevice)
        {
                m_OpenOK = OpenDevice(0);
        }
        else
        {
                if (m_OpenLater) OpenDevice(m_SampleRate);
                m_OpenOK = true;
        }
        SetEvent(m_ReadyEvent);

        events[0] = m_StopEvent;
        events[1] = m_PlayEvent;

        if (m_OpenOK && WaitForMultipleObjects(2, events, FALSE, INFINITE) == WAIT_OBJECT_0 + 1)
        {
                if (m_DeviceOpen && FAILED(m_Client->Start())) CloseDevice();
                StartClock();
                m_RetryTime = GetTickCount() + WASAPI_RETRY_MS;
                InterlockedExchange((LONG *)&m_Notify.Changed, 0);

                while (WaitForSingleObject(m_StopEvent, WASAPI_POLL_MS) != WAIT_OBJECT_0)
                {
                        // Another default device: open it at once
                        if (InterlockedExchange((LONG *)&m_Notify.Changed, 0))
                        {
                                if (m_DeviceOpen) StartClock();
                                CloseDevice();
                                m_RetryTime = GetTickCount();
                        }

                        if (!m_DeviceOpen && m_OpenLater
                                && (LONG)(GetTickCount() - m_RetryTime) >= 0)
                        {
                                m_RetryTime = GetTickCount() + WASAPI_RETRY_MS;
                                if (OpenDevice(m_SampleRate) && FAILED(m_Client->Start()))
                                        CloseDevice();
                        }

                        if (m_DeviceOpen)
                        {
                                if (!FillDevice())
                                {
                                        // Unplugged, or the audio service
                                        // stopped: the clock takes over
                                        CloseDevice();
                                        StartClock();
                                        m_RetryTime = GetTickCount() + WASAPI_RETRY_MS;
                                }
                        }
                        else
                        {
                                RunByClock();
                        }

                        if (m_Stopping) break;
                }
        }

        CloseDevice();

        if (m_Enumerator)
        {
                if (m_NotifyRegistered)
                        m_Enumerator->UnregisterEndpointNotificationCallback(&m_Notify);
                m_NotifyRegistered = false;
                m_Enumerator->Release();
                m_Enumerator = NULL;
        }

        if (com) CoUninitialize();
}

//---------------------------------------------------------------------------
// The queue between the emulator and the audio thread

DWORD CWasapiSnd::TakeFromQueue(BYTE *dest, DWORD max)
{
        DWORD n, first;

        EnterCriticalSection(&m_QueueLock);

        if (!m_lpAudioQueue || !m_QueueLen)
        {
                LeaveCriticalSection(&m_QueueLock);
                return 0;
        }

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

// Copy a sound frame from the emulator into the queue. If the emulator gets
// too far ahead, the oldest sound is dropped.

void CWasapiSnd::Frame(unsigned char *data, unsigned int len)
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
