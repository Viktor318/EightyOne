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

#include <windows.h>
#include "utils.h"
#include "zx81config.h"
#include "keypad.h"
#include "kbstatus.h"

CKeypad Keypad;

const int CKeypad::KeypadSocketOutLineLevelLow  = 0x00;
const int CKeypad::KeypadSocketOutLineLevelHigh = 0x01;
const int CKeypad::KeypadSocketInLineLevelLow  = 0x00;
const int CKeypad::KeypadSocketInLineLevelHigh = 0x20;

// The ROM may be interrupted between two steps of a bit (the frame
// interrupt takes about 1900 T-states), so the keypad waits longer than
// that before it gives up (as in Fuse)
static const int BitTimeoutTCycles = 5000;

// The keypad keys: bit numbers in hostKeys, rows 5, 4, 1, 2, 3 from the
// lowest bits up (bit 3 of a row is its leftmost key, as in
// FetchRowKeys_Keyboard()). The editing functions of the 128 BASIC editor
// in brackets
enum KeypadKey
{
        KeypadPeriod   = 1,     // (last line of the program)
        Keypad0        = 3,     // (SHIFT)
        KeypadEnter    = 4,
        Keypad3        = 5,     // (first line of the program)
        Keypad2        = 6,     // (end of the line)
        Keypad1        = 7,     // (start of the line)
        KeypadRParen   = 8,     // (TOGGLE)
        KeypadLParen   = 9,     // (DEL right)
        KeypadMultiply = 10,    // (cursor up)
        KeypadDivide   = 11,    // (DEL left)
        KeypadMinus    = 12,    // (CMND)
        Keypad9        = 13,    // (cursor right)
        Keypad8        = 14,    // (cursor down)
        Keypad7        = 15,    // (cursor left)
        KeypadPlus     = 16,    // (10 lines down)
        Keypad6        = 17,    // (10 lines up)
        Keypad5        = 18,    // (next word)
        Keypad4        = 19     // (previous word)
};

#define KEYPADKEY(k) (1 << (k))

CKeypad::CKeypad()
{
        hostKeyCount = 0;
        hostKeys = 0;
        hostShiftSent = false;
        Reset();
}

// The machine starts again
void CKeypad::Reset()
{
        // The line to the keypad is high when idle. The ROM asks for a poll
        // by taking it low; starting from low, its first write (0xff) would
        // look like the end of a poll the keypad never answered, and the
        // ROM would only try again some 3 seconds later
        keypadSocketOut = KeypadSocketOutLineLevelHigh;
        ResetProtocol();
}

// The keypad gives up and waits for the next poll
void CKeypad::ResetProtocol()
{
        state = AwaitingPollRequest;
        substate = AwaitingReadBitSync;
        keypadSocketIn = KeypadSocketInLineLevelHigh;
        stateTimeoutTCycles = 0;
        hostShiftSent = false;

        for (int i = 0; i < 5; i++)
        {
                keyRowBits[i] = 0xF;
        }
}

void CKeypad::Write(int registerByte)
{
        if (keypadSocketOut == (registerByte & 0x01))
        {
                return;
        }

        keypadSocketOut = (registerByte & 0x01);

        switch (state)
        {
        case AwaitingPollRequest:
                keypadSocketIn = KeypadSocketInLineLevelLow;
                state = AwaitingPollRequestEnd;
                stateTimeoutTCycles = 5000;
                break;

        case AwaitingPollRequestEnd:
                keypadSocketIn = KeypadSocketInLineLevelHigh;
                state = AwaitingScanRequest;
                stateTimeoutTCycles = 5000;
                break;

        case AwaitingScanRequest:
                state = AwaitingReadKeypadID;
                substate = AwaitingReadBitSync;
                stateTimeoutTCycles = 5500;
                keyBitsToSend = 0x4;
                bitsToSend = 4;
                break;

        case AwaitingReadKeypadID:
                HandleRowRead();
                break;

        case AwaitingReadRowFlag:
                HandleRowRead();
                break;

        case AwaitingReadRowBits:
                HandleRowRead();
                break;
        }
}

void CKeypad::HandleRowRead()
{
        switch (substate)
        {
        case AwaitingReadBitSync:
                keypadSocketIn = KeypadSocketInLineLevelLow;
                stateTimeoutTCycles = 19000;
                substate = AwaitingReadBitSyncEnd;

                if (state == AwaitingReadRowFlag)
                {
                        int keyBits = FetchRowKeys();

                        if (keyBits != keyRowBits[row])
                        {
                                keyRowBits[row] = keyBits;
                                keyBitsToSend = (keyRowBits[row] << 1) | 1;
                                bitsToSend = 5;
                        }
                        else
                        {
                                keyBitsToSend = 0;
                                bitsToSend = 1;
                        }
                }
                break;

        case AwaitingReadBitSyncEnd:
                keypadSocketIn = (keyBitsToSend & 0x01) ? KeypadSocketInLineLevelHigh: KeypadSocketInLineLevelLow;
                keyBitsToSend >>= 1;

                substate = AwaitingReadBit;
                stateTimeoutTCycles = BitTimeoutTCycles;
                break;

        case AwaitingReadBit:
                keypadSocketIn = KeypadSocketInLineLevelHigh;
                substate = AwaitingReadBitEnd;
                stateTimeoutTCycles = BitTimeoutTCycles;
                break;

        case AwaitingReadBitEnd:
                bitsToSend--;

                if (bitsToSend > 0)
                {
                        if (state == AwaitingReadRowFlag)
                        {
                                state = AwaitingReadRowBits;
                        }

                        substate = AwaitingReadBitSync;
                        stateTimeoutTCycles = BitTimeoutTCycles;
                }
                else
                {
                        if (state == AwaitingReadKeypadID)
                        {
                                state = AwaitingReadRowFlag;
                                substate = AwaitingReadBitSync;
                                stateTimeoutTCycles = BitTimeoutTCycles;
                                row = 0;
                        }
                        else
                        {
                                row++;
                                if (row == 5)
                                {
                                        row = 0;
                                        stateTimeoutTCycles = 0;
                                }
                                else
                                {
                                        stateTimeoutTCycles = 5000;
                                }
                                state = AwaitingReadRowFlag;
                                substate = AwaitingReadBitSync;
                        }
                }
                break;
        }
}

int CKeypad::Read()
{
        return keypadSocketIn;
}

int CKeypad::FetchRowKeys()
{
        // The editing keys of the PC held as keypad keys. A key given with
        // the keypad SHIFT (the deletions) waits until the SHIFT has been
        // sent: the row of SHIFT goes first in every scan, so a press in the
        // middle of a scan would send the other key first, and the editor
        // would move the cursor instead of deleting
        int keys = hostKeys;

        if (row == 0)
        {
                hostShiftSent = ((keys & KEYPADKEY(Keypad0)) != 0);
        }
        else if ((keys & KEYPADKEY(Keypad0)) && !hostShiftSent)
        {
                keys = 0;
        }

        int keyData = (keys >> (row * 4)) & 0x0F;

        // The right Ctrl with the keys at the places of the keypad's keys.
        // The numeric keypad of the PC is not the keypad: it gives the
        // digits of the keyboard
        if (IsAsyncKeyPressed(VK_RCONTROL))
        {
                keyData |= FetchRowKeys_Keyboard();
        }

        return keyData;
}

//---------------------------------------------------------------------------
// The editing keys of a PC keyboard give the editing functions of the 128
// BASIC editor, which only the keypad has (as in Fuse):
//
//   Home / End                  start / end of the line
//   Ctrl + Home / End           first / last line of the program
//   Ctrl + Left / Right         previous / next word
//   Page Up / Page Down         10 lines up / down
//   Delete                      delete the character under the cursor
//   Ctrl + Backspace / Delete   delete to the start / end of the word
//   Shift + Backspace / Delete  delete to the start / end of the line
//   Insert                      CMND (the editor menu)
//   Ctrl + Insert               TOGGLE (between the upper and lower screen)
//
// The deletions of more than one character need the keypad SHIFT too.
// Ctrl wins over Shift when both are held. With Alt (or AltGr) the keys
// are not taken

// Only the 128 and the +2 have the KEYPAD socket and a ROM which polls
// the keypad
bool CKeypad::HostKeysActive()
{
        return spectrum.spectrum128Keypad
                && emulator.machine == MACHINESPECTRUM
                && (spectrum.model == SPECCY128 || spectrum.model == SPECCYPLUS2);
}

int CKeypad::HostKeyKeypadKeys(unsigned short key)
{
        bool ctrl = IsKeyPressed(VK_CONTROL);
        bool shift = IsKeyPressed(VK_SHIFT);

        switch (key)
        {
        case VK_HOME:
                return ctrl ? KEYPADKEY(Keypad3) : KEYPADKEY(Keypad1);

        case VK_END:
                return ctrl ? KEYPADKEY(KeypadPeriod) : KEYPADKEY(Keypad2);

        case VK_PRIOR:
                return KEYPADKEY(Keypad6);

        case VK_NEXT:
                return KEYPADKEY(KeypadPlus);

        case VK_INSERT:
                return ctrl ? KEYPADKEY(KeypadRParen) : KEYPADKEY(KeypadMinus);

        case VK_DELETE:
                if (ctrl) return KEYPADKEY(Keypad0) | KEYPADKEY(Keypad5);
                if (shift) return KEYPADKEY(Keypad0) | KEYPADKEY(Keypad2);
                return KEYPADKEY(KeypadLParen);

        case VK_BACK:
                if (ctrl) return KEYPADKEY(Keypad0) | KEYPADKEY(Keypad4);
                if (shift) return KEYPADKEY(Keypad0) | KEYPADKEY(Keypad1);
                return 0;

        case VK_LEFT:
                return ctrl ? KEYPADKEY(Keypad4) : 0;

        case VK_RIGHT:
                return ctrl ? KEYPADKEY(Keypad5) : 0;

        default:
                return 0;
        }
}

void CKeypad::UpdateHostKeys()
{
        int keys = 0;

        for (int i = 0; i < hostKeyCount; i++)
        {
                keys |= hostKeysHeld[i].keys;
        }

        hostKeys = keys;
}

bool CKeypad::HostKey(unsigned short key, bool press)
{
        int i;

        for (i = 0; i < hostKeyCount; i++)
        {
                if (hostKeysHeld[i].key == key) break;
        }

        if (!press)
        {
                // Released as the keypad keys it was pressed as, whatever
                // Ctrl and Shift are now
                if (i == hostKeyCount) return false;

                hostKeysHeld[i] = hostKeysHeld[--hostKeyCount];
                UpdateHostKeys();
                return true;
        }

        // The autorepeat of a key held as keypad keys
        if (i < hostKeyCount) return true;

        if (!HostKeysActive() || IsKeyPressed(VK_MENU)) return false;

        int keys = HostKeyKeypadKeys(key);
        if (!keys || hostKeyCount == MaxHostKeys) return false;

        hostKeysHeld[hostKeyCount].key = key;
        hostKeysHeld[hostKeyCount].keys = keys;
        hostKeyCount++;
        UpdateHostKeys();
        return true;
}

void CKeypad::ReleaseHostKeys()
{
        hostKeyCount = 0;
        hostKeys = 0;
}
//---------------------------------------------------------------------------

inline int CKeypad::FetchRowKeys_Keyboard()
{
        int keyData = 0x00;

        switch (row)
        {
        default:
        case 0: // Row 5
                if (IsAsyncKeyPressed('0')) keyData |= 0x08;
                if (IsAsyncKeyPressed(VK_OEM_PERIOD)) keyData |= 0x02;
                break;

        case 1: // Row 4
                if (IsAsyncKeyPressed('1')) keyData |= 0x08;
                if (IsAsyncKeyPressed('2')) keyData |= 0x04;
                if (IsAsyncKeyPressed('3')) keyData |= 0x02;
                if (IsAsyncKeyPressed(VK_RETURN)) keyData |= 0x01;
                break;

        case 2: // Row 1
                if (IsAsyncKeyPressed(VK_OEM_2)) keyData |= 0x08;
                if (IsAsyncKeyPressed(VK_OEM_7)) keyData |= 0x04;
                if (IsAsyncKeyPressed(VK_OEM_4)) keyData |= 0x02;
                if (IsAsyncKeyPressed(VK_OEM_6)) keyData |= 0x01;
                break;

        case 3: // Row 2
                if (IsAsyncKeyPressed('7')) keyData |= 0x08;
                if (IsAsyncKeyPressed('8')) keyData |= 0x04;
                if (IsAsyncKeyPressed('9')) keyData |= 0x02;
                if (IsAsyncKeyPressed(VK_OEM_MINUS)) keyData |= 0x01;
                break;

        case 4: // Row 3
                if (IsAsyncKeyPressed('4')) keyData |= 0x08;
                if (IsAsyncKeyPressed('5')) keyData |= 0x04;
                if (IsAsyncKeyPressed('6')) keyData |= 0x02;
                if (IsAsyncKeyPressed(VK_OEM_PLUS)) keyData |= 0x01;
                break;
        }

        return keyData;
}
/*
inline int CKeypad::FetchRowKeys_NumericPad()
{
        int keyData = 0x00;

        switch (row)
        {
        default:
        case 0: // Row 5
                if (IsAsyncKeyPressed(VK_NUMPAD0)) keyData |= 0x08;
                if (IsAsyncKeyPressed(VK_DECIMAL)) keyData |= 0x02;
                break;

        case 1: // Row 4
                if (IsAsyncKeyPressed(VK_NUMPAD1)) keyData |= 0x08;
                if (IsAsyncKeyPressed(VK_NUMPAD2)) keyData |= 0x04;
                if (IsAsyncKeyPressed(VK_NUMPAD3)) keyData |= 0x02;
                if (IsAsyncKeyPressed(VK_RETURN) ) keyData |= 0x01;
                break;

        case 2: // Row 1
                if (IsAsyncKeyPressed(VK_DIVIDE)  ) keyData |= 0x08;
                if (IsAsyncKeyPressed(VK_MULTIPLY)) keyData |= 0x04;
                if (IsAsyncKeyPressed(VK_NUMLOCK) ) keyData |= 0x02;
                if (IsAsyncKeyPressed(VK_LAUNCH_APP1)) keyData |= 0x01;
                break;

        case 3: // Row 2
                if (IsAsyncKeyPressed(VK_NUMPAD7) ) keyData |= 0x08;
                if (IsAsyncKeyPressed(VK_NUMPAD8) ) keyData |= 0x04;
                if (IsAsyncKeyPressed(VK_NUMPAD9) ) keyData |= 0x02;
                if (IsAsyncKeyPressed(VK_SUBTRACT)) keyData |= 0x01;
                break;

        case 4: // Row 3
                if (IsAsyncKeyPressed(VK_NUMPAD4)) keyData |= 0x08;
                if (IsAsyncKeyPressed(VK_NUMPAD5)) keyData |= 0x04;
                if (IsAsyncKeyPressed(VK_NUMPAD6)) keyData |= 0x02;
                if (IsAsyncKeyPressed(VK_ADD)    ) keyData |= 0x01;
                break;
        }

        return keyData;
}
*/
void CKeypad::ClockTick(int ts)
{
        if (stateTimeoutTCycles > 0)
        {
                stateTimeoutTCycles -= ts;
                
                if (stateTimeoutTCycles <= 0)
                {
                        ResetProtocol();
                }
        }
}

