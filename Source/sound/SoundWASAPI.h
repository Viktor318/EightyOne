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

// WASAPI output (Windows Vista and later), in shared mode on the default
// device. See SoundWASAPI.cpp.

#ifndef _SOUNDWASAPI_H
#define _SOUNDWASAPI_H

#include <windows.h>
#include <objbase.h>
#include "sound\SoundOutput.h"

// How much sound is kept waiting for the audio engine. The emulator runs one
// frame for each frame of sound written, so this is also about how far the
// emulation is ahead of what is heard.

#define WASAPI_MARGIN_MS        30      // at least this much...
#define WASAPI_MARGIN_PERIODS   3       // ...and this many device periods
#define WASAPI_BUFFER_MS        200     // the buffer asked for
#define WASAPI_POLL_MS          5       // how often the audio thread looks
#define WASAPI_QUEUE_FRAMES     4       // frames the emulator may get ahead
#define WASAPI_RETRY_MS         1000    // trying again to open a device
#define WASAPI_START_MS         5000    // the longest wait for the device

struct IMMDeviceEnumerator;
struct IAudioClient;
struct IAudioRenderClient;

// Told by Windows when the default device changes (on another thread)
struct IMMNotificationClient : public IUnknown
{
        virtual HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR id, DWORD state) = 0;
        virtual HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR id) = 0;
        virtual HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR id) = 0;
        virtual HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(int flow, int role, LPCWSTR id) = 0;
        // The PROPERTYKEY (a GUID and a DWORD) is passed by value: as five
        // DWORDs, so that the stack is the same whatever the compiler does
        // with structures
        virtual HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR id,
                DWORD key0, DWORD key1, DWORD key2, DWORD key3, DWORD key4) = 0;
};

class CWasapiNotify : public IMMNotificationClient
{
public:
        volatile LONG Changed;          // set when the default device changes

        CWasapiNotify() { Changed = 0; }

        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **object);
        ULONG STDMETHODCALLTYPE AddRef(void) { return 1; }      // not on the heap
        ULONG STDMETHODCALLTYPE Release(void) { return 1; }

        HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR id, DWORD state) { return S_OK; }
        HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR id) { return S_OK; }
        HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR id) { return S_OK; }
        HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(int flow, int role, LPCWSTR id);
        HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR id,
                DWORD key0, DWORD key1, DWORD key2, DWORD key3, DWORD key4) { return S_OK; }
};

class CWasapiSnd : public CSoundOutput
{
public:
        CWasapiSnd();

        // Opens the default device, at its own sample rate (the one asked
        // for is not used). Fails if there is no WASAPI or no device.
        int Initialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels);

        // Without a device (none could be opened): keeps the emulator
        // running at the right speed, at the sample rate asked for, and if
        // `openLater', plays the sound when a device can be opened
        int InitialiseNoDevice(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels,
                               bool openLater);

        int Play();
        int End();
        void Frame(unsigned char *data, unsigned int len);
        bool Stopping() { return m_Stopping; }
        void SetFrameEvent(HANDLE event) { m_FrameEvent = event; }
        int SampleRate() { return m_SampleRate; }
        const char *Name() { return m_DeviceOpen ? "WASAPI" : NULL; }

private:
        int Start(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels,
                  bool needDevice, bool openLater);
        void Release(void);

        void ThreadFN(void);
        static DWORD WINAPI CallThread(LPVOID Param);

        bool OpenDevice(int rate);
        void CloseDevice(void);
        bool FillDevice(void);
        void RunByClock(void);
        void StartClock(void);
        bool WriteDevice(const short *data, UINT32 frames);
        DWORD TakeFromQueue(BYTE *dest, DWORD max);

        HWND m_hWnd;
        int m_FPS;
        int m_SampleRate;
        int m_Channels;
        bool m_NeedDevice;
        bool m_OpenLater;               // look for a device when there is none

        DWORD m_ThreadID;
        HANDLE m_ThreadHandle;
        HANDLE m_StopEvent;
        HANDLE m_PlayEvent;
        HANDLE m_ReadyEvent;
        volatile bool m_Stopping;
        volatile bool m_OpenOK;
        volatile bool m_DeviceOpen;
        HANDLE m_FrameEvent;            // the emulator's (not ours)
        HANDLE m_FrameDone;             // set by Frame()
        bool RunEmulatorFrame(void);

        // WASAPI (only used on the audio thread)
        IMMDeviceEnumerator *m_Enumerator;
        IAudioClient *m_Client;
        IAudioRenderClient *m_Render;
        CWasapiNotify m_Notify;
        bool m_NotifyRegistered;
        UINT32 m_BufferFrames;
        UINT32 m_TargetFrames;          // kept waiting for the audio engine
        int m_DevChannels;
        int m_DevBytes;                 // bytes of one sample of one channel
        bool m_DevFloat;

        // Keeping time without a device
        LARGE_INTEGER m_ClockFreq;
        LARGE_INTEGER m_ClockStart;
        __int64 m_ClockFrames;
        DWORD m_RetryTime;

        // One emulated frame of sound, and the last sample written (to fade
        // from when there is no sound)
        DWORD m_FrameBytes;
        UINT32 m_FrameFrames;
        short m_LastSample[2];

        // Frames from the emulator waiting to be written (a circular queue)
        CRITICAL_SECTION m_QueueLock;
        bool m_QueueLockInit;
        LPBYTE m_lpAudioQueue;
        DWORD m_QueueLen;
        DWORD m_QueueSize;
        DWORD m_QueueStart;
        LPBYTE m_lpFrameBuf;            // taken from the queue
};

#endif
