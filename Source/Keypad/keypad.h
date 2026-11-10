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

#ifndef KEYPAD_H
#define KEYPAD_H

enum State
{
        AwaitingPollRequest,
        AwaitingPollRequestEnd,
        AwaitingScanRequest,
        AwaitingReadKeypadID,
        AwaitingReadRowFlag,
        AwaitingReadRowBits
};

enum Substate
{
        AwaitingReadBitSync,
        AwaitingReadBitSyncEnd,
        AwaitingReadBit,
        AwaitingReadBitEnd
};

class CKeypad
{
public:
        CKeypad();
        void Write(int registerByte);
        int Read();
        void ClockTick(int ts);
        void Reset();

        // Keys of the PC as keypad keys (Home, End, Page Up...): true if
        // the key was taken by the keypad, so it must not act on the
        // keyboard too
        bool HostKey(unsigned short key, bool press);

        // All the keys taken by the keypad up (e.g. on losing the focus)
        void ReleaseHostKeys();

private:
        static const int KeypadSocketOutLineLevelLow;
        static const int KeypadSocketOutLineLevelHigh;
        static const int KeypadSocketInLineLevelLow;
        static const int KeypadSocketInLineLevelHigh;

        State state;
        Substate substate;
        int row;
        int keypadSocketOut;
        int keypadSocketIn;
        int stateTimeoutTCycles;
        int bitsToSend;
        int keyRowBits[5];
        int keyBitsToSend;

        // The PC keys held as keypad keys, and the keypad keys they give:
        // 4 bits a row, rows 5, 4, 1, 2, 3 from the lowest bits up (the
        // order the rows are sent in)
        static const int MaxHostKeys = 8;
        struct HostKeyHeld
        {
                unsigned short key;
                int keys;
        } hostKeysHeld[MaxHostKeys];
        int hostKeyCount;
        volatile int hostKeys;
        bool hostShiftSent;

        void ResetProtocol();
        void HandleRowRead();
        int FetchRowKeys();
        int FetchRowKeys_Keyboard();
        bool HostKeysActive();
        int HostKeyKeypadKeys(unsigned short key);
        void UpdateHostKeys();
};

extern CKeypad Keypad;

#endif
