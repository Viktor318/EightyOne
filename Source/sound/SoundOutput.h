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

// What CSound needs of a sound output (DirectSound, WASAPI).
//
// The output's audio thread is also the emulator's clock: for each frame of
// sound it needs, it asks the main window (WM_USER) to run one frame, which
// hands its sound back through Frame().

#ifndef _SOUNDOUTPUT_H
#define _SOUNDOUTPUT_H

#include <windows.h>

class CSoundOutput
{
public:
        virtual ~CSoundOutput() {}

        // 0 on success. The sound handed to Frame() is in this format, at
        // the rate SampleRate() gives (which may not be the one asked for).
        virtual int Initialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels) = 0;
        virtual int Play() = 0;
        virtual int End() = 0;
        virtual void Frame(unsigned char *data, unsigned int len) = 0;

        // True while End() is waiting for the audio thread to finish: a
        // frame the thread asked for just before must not be run then
        virtual bool Stopping() = 0;

        virtual int SampleRate() = 0;

        // What is playing the sound now ("WASAPI", "DirectSound"), or NULL
        // if nothing is (the emulator is kept running by a timer)
        virtual const char *Name() = 0;
};

#endif
