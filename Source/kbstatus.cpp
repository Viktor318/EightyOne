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

//---------------------------------------------------------------------------

#include <vcl4.h>
#pragma hdrstop

#include <stdio.h>
#include "utils.h"
#include "kbstatus.h"
#include "zx81.h"
#include "zx81config.h"

#define kbD0 1
#define kbD1 2
#define kbD2 4
#define kbD3 8
#define kbD4 16
#define kbD5 32

#define kbA8  0
#define kbA9  1
#define kbA10 2
#define kbA11 3
#define kbA12 4
#define kbA13 5
#define kbA14 6
#define kbA15 7

int PCShift=1;
unsigned int PCALT=0;
AnsiString keypadMultiplyKey = "#";

struct kb
{
        BYTE Shift;
        WORD WinKey;
        BYTE Addr1, Data1, Addr2, Data2;
};

void SetKeyMap(const kb *source);
void AdjustLocalKeyboard();

// "Use the Windows keyboard layout": typing follows the characters the
// Windows keyboard layout gives, not the positions of the keys
bool WindowsLayout = true;

// The base map of the emulated machine, for the positions of the keys
static const kb *BaseMap = NULL;

struct kb KeyMap[100];

const struct kb KBZX81[]=
{
        { 0, VK_SHIFT, kbA8, kbD0, 255, 255 },
        { 0, VK_RETURN, kbA14, kbD0, 255, 255 },
        { 0, VK_SPACE, kbA15, kbD0, 255, 255 },

        { 0, 'A', kbA9, kbD0, 255, 255 },
        { 0, 'B', kbA15, kbD4, 255, 255 },
        { 0, 'C', kbA8, kbD3, 255, 255 },
        { 0, 'D', kbA9, kbD2, 255, 255 },
        { 0, 'E', kbA10, kbD2, 255, 255 },
        { 0, 'F', kbA9, kbD3, 255, 255 },
        { 0, 'G', kbA9, kbD4, 255, 255 },
        { 0, 'H', kbA14, kbD4, 255, 255 },
        { 0, 'I', kbA13, kbD2, 255, 255 },
        { 0, 'J', kbA14, kbD3, 255, 255 },
        { 0, 'K', kbA14, kbD2, 255, 255 },
        { 0, 'L', kbA14, kbD1, 255, 255 },
        { 0, 'M', kbA15, kbD2, 255, 255 },
        { 0, 'N', kbA15, kbD3, 255, 255 },
        { 0, 'O', kbA13, kbD1, 255, 255 },
        { 0, 'P', kbA13, kbD0, 255, 255 },
        { 0, 'Q', kbA10, kbD0, 255, 255 },
        { 0, 'R', kbA10, kbD3, 255, 255 },
        { 0, 'S', kbA9, kbD1, 255, 255 },
        { 0, 'T', kbA10, kbD4, 255, 255 },
        { 0, 'U', kbA13, kbD3, 255, 255 },
        { 0, 'V', kbA8, kbD4, 255, 255 },
        { 0, 'W', kbA10, kbD1, 255, 255 },
        { 0, 'X', kbA8, kbD2, 255, 255 },
        { 0, 'Y', kbA13, kbD4, 255, 255 },
        { 0, 'Z', kbA8, kbD1, 255, 255 },

        { 0, '1', kbA11, kbD0, 255, 255 },
        { 0, '2', kbA11, kbD1, 255, 255 },
        { 0, '3', kbA11, kbD2, 255, 255 },
        { 0, '4', kbA11, kbD3, 255, 255 },
        { 0, '5', kbA11, kbD4, 255, 255 },
        { 0, '6', kbA12, kbD4, 255, 255 },
        { 0, '7', kbA12, kbD3, 255, 255 },
        { 0, '8', kbA12, kbD2, 255, 255 },
        { 0, '9', kbA12, kbD1, 255, 255 },
        { 0, '0', kbA12, kbD0, 255, 255 },

        { 0, VK_NUMPAD1, kbA11, kbD0, 255, 255 },
        { 0, VK_NUMPAD2, kbA11, kbD1, 255, 255 },
        { 0, VK_NUMPAD3, kbA11, kbD2, 255, 255 },
        { 0, VK_NUMPAD4, kbA11, kbD3, 255, 255 },
        { 0, VK_NUMPAD5, kbA11, kbD4, 255, 255 },
        { 0, VK_NUMPAD6, kbA12, kbD4, 255, 255 },
        { 0, VK_NUMPAD7, kbA12, kbD3, 255, 255 },
        { 0, VK_NUMPAD8, kbA12, kbD2, 255, 255 },
        { 0, VK_NUMPAD9, kbA12, kbD1, 255, 255 },
        { 0, VK_NUMPAD0, kbA12, kbD0, 255, 255 },

        { 0, VK_MULTIPLY, kbA15, kbD4, kbA8, kbD0 },
        { 0, VK_DIVIDE, kbA8, kbD4, kbA8, kbD0 },
        { 0, VK_SUBTRACT, kbA14, kbD3, kbA8, kbD0 },
        { 0, VK_ADD, kbA14, kbD2, kbA8, kbD0 },
        { 0, VK_DECIMAL, kbA15, kbD1, 255, 255 },

        { 1, VK_OEM_1, kbA8, kbD2, kbA8, kbD0 },
        { 2, VK_OEM_1, kbA8, kbD1, kbA8, kbD0 },
        { 1, VK_OEM_MINUS, kbA14, kbD3, kbA8, kbD0 },

        { 1, VK_OEM_PLUS, kbA14, kbD1, kbA8, kbD0 },
        { 2, VK_OEM_PLUS, kbA14, kbD2, kbA8, kbD0 },

        { 1, VK_OEM_COMMA, kbA15, kbD1, kbA8, kbD0 },
        { 2, VK_OEM_COMMA, kbA15, kbD3, kbA8, kbD0 },

        { 1, VK_OEM_PERIOD, kbA15, kbD1, 255, 255 },
        { 2, VK_OEM_PERIOD, kbA15, kbD2, kbA8, kbD0 },
        { 1, VK_OEM_2, kbA8, kbD4, kbA8, kbD0 },
        { 2, VK_OEM_2, kbA8, kbD3, kbA8, kbD0 },
        { 0, VK_OEM_4, kbA13, kbD2, kbA8, kbD0 },
        { 0, VK_OEM_6, kbA13, kbD1, kbA8, kbD0 },
        { 2, VK_OEM_7, kbA13, kbD0, kbA8, kbD0 },
        { 0, VK_OEM_3, kbA13, kbD0, kbA8, kbD0 },

        //{ 2, VK_OEM_COMMA, kbA15, kbD3, kbA8, kbD0 },
        //{ 2, VK_OEM_PERIOD, kbA15, kbD2, kbA8, kbD0 },

        { 0, VK_BACK, kbA12, kbD0, kbA8, kbD0 },
        { 0, VK_LEFT, kbA11, kbD4, kbA8, kbD0 },
        { 0, VK_DOWN, kbA12, kbD4, kbA8, kbD0 },
        { 0, VK_UP, kbA12, kbD3, kbA8, kbD0 },
        { 0, VK_RIGHT, kbA12, kbD2, kbA8, kbD0 },

        { 0, VK_CONTROL, kbA14, kbD0, kbA8, kbD0 },

        {0, 0, 0, 0, 0, 0 }
};

const struct kb KBZX80[]=
{
        { 0, VK_SHIFT, kbA8, kbD0, 255, 255 },
        { 0, VK_RETURN, kbA14, kbD0, 255, 255 },
        { 0, VK_SPACE, kbA15, kbD0, 255, 255 },

        { 0, 'A', kbA9, kbD0, 255, 255 },
        { 0, 'B', kbA15, kbD4, 255, 255 },
        { 0, 'C', kbA8, kbD3, 255, 255 },
        { 0, 'D', kbA9, kbD2, 255, 255 },
        { 0, 'E', kbA10, kbD2, 255, 255 },
        { 0, 'F', kbA9, kbD3, 255, 255 },
        { 0, 'G', kbA9, kbD4, 255, 255 },
        { 0, 'H', kbA14, kbD4, 255, 255 },
        { 0, 'I', kbA13, kbD2, 255, 255 },
        { 0, 'J', kbA14, kbD3, 255, 255 },
        { 0, 'K', kbA14, kbD2, 255, 255 },
        { 0, 'L', kbA14, kbD1, 255, 255 },
        { 0, 'M', kbA15, kbD2, 255, 255 },
        { 0, 'N', kbA15, kbD3, 255, 255 },
        { 0, 'O', kbA13, kbD1, 255, 255 },
        { 0, 'P', kbA13, kbD0, 255, 255 },
        { 0, 'Q', kbA10, kbD0, 255, 255 },
        { 0, 'R', kbA10, kbD3, 255, 255 },
        { 0, 'S', kbA9, kbD1, 255, 255 },
        { 0, 'T', kbA10, kbD4, 255, 255 },
        { 0, 'U', kbA13, kbD3, 255, 255 },
        { 0, 'V', kbA8, kbD4, 255, 255 },
        { 0, 'W', kbA10, kbD1, 255, 255 },
        { 0, 'X', kbA8, kbD2, 255, 255 },
        { 0, 'Y', kbA13, kbD4, 255, 255 },
        { 0, 'Z', kbA8, kbD1, 255, 255 },

        { 0, '1', kbA11, kbD0, 255, 255 },
        { 0, '2', kbA11, kbD1, 255, 255 },
        { 0, '3', kbA11, kbD2, 255, 255 },
        { 0, '4', kbA11, kbD3, 255, 255 },
        { 0, '5', kbA11, kbD4, 255, 255 },
        { 0, '6', kbA12, kbD4, 255, 255 },
        { 0, '7', kbA12, kbD3, 255, 255 },
        { 0, '8', kbA12, kbD2, 255, 255 },
        { 0, '9', kbA12, kbD1, 255, 255 },
        { 0, '0', kbA12, kbD0, 255, 255 },

        { 0, VK_NUMPAD1, kbA11, kbD0, 255, 255 },
        { 0, VK_NUMPAD2, kbA11, kbD1, 255, 255 },
        { 0, VK_NUMPAD3, kbA11, kbD2, 255, 255 },
        { 0, VK_NUMPAD4, kbA11, kbD3, 255, 255 },
        { 0, VK_NUMPAD5, kbA11, kbD4, 255, 255 },
        { 0, VK_NUMPAD6, kbA12, kbD4, 255, 255 },
        { 0, VK_NUMPAD7, kbA12, kbD3, 255, 255 },
        { 0, VK_NUMPAD8, kbA12, kbD2, 255, 255 },
        { 0, VK_NUMPAD9, kbA12, kbD1, 255, 255 },
        { 0, VK_NUMPAD0, kbA12, kbD0, 255, 255 },

        { 0, VK_MULTIPLY, kbA13, kbD0, kbA8, kbD0 },
        { 0, VK_DIVIDE, kbA8, kbD4, kbA8, kbD0 },
        { 0, VK_SUBTRACT, kbA14, kbD3, kbA8, kbD0 },
        { 0, VK_ADD, kbA14, kbD2, kbA8, kbD0 },
        { 1, VK_DECIMAL, kbA15, kbD1, 255, 255 },

        { 1, VK_OEM_1, kbA8, kbD2, kbA8, kbD0 },
        { 2, VK_OEM_1, kbA8, kbD1, kbA8, kbD0 },
        { 1, VK_OEM_MINUS, kbA14, kbD2, kbA8, kbD0 },

        { 1, VK_OEM_PLUS, kbA14, kbD1, kbA8, kbD0 },
        { 2, VK_OEM_PLUS, kbA14, kbD2, kbA8, kbD0 },

        { 1, VK_OEM_COMMA, kbA15, kbD1, kbA8, kbD0 },
        { 2, VK_OEM_COMMA, kbA15, kbD3, kbA8, kbD0 },

        { 1, VK_OEM_PERIOD, kbA15, kbD1, 255, 255 },
        { 2, VK_OEM_PERIOD, kbA15, kbD2, kbA8, kbD0 },
        { 1, VK_OEM_2, kbA8, kbD4, kbA8, kbD0 },
        { 2, VK_OEM_2, kbA8, kbD3, kbA8, kbD0 },
        { 0, VK_OEM_4, kbA13, kbD2, kbA8, kbD0 },
        { 0, VK_OEM_6, kbA13, kbD1, kbA8, kbD0 },
        { 2, VK_OEM_7, kbA13, kbD4, kbA8, kbD0 },

        //{ 2, VK_OEM_COMMA, kbA15, kbD3, kbA8, kbD0 },
        //{ 2, VK_OEM_PERIOD, kbA15, kbD2, kbA8, kbD0 },

        { 0, VK_BACK, kbA12, kbD0, kbA8, kbD0 },
        { 0, VK_LEFT, kbA11, kbD4, kbA8, kbD0 },
        { 0, VK_DOWN, kbA12, kbD4, kbA8, kbD0 },
        { 0, VK_UP, kbA12, kbD3, kbA8, kbD0 },
        { 0, VK_RIGHT, kbA12, kbD2, kbA8, kbD0 },

        { 0, VK_CONTROL, kbA14, kbD0, kbA8, kbD0 },

        {0, 0, 0, 0, 0, 0 }
};

const struct kb KBACE[]=
{
        { 0, VK_SHIFT, kbA8, kbD0, 255, 255 },
        { 0, VK_CONTROL, kbA8, kbD1, 255, 255 },

        { 0, VK_RETURN, kbA14, kbD0, 255, 255 },
        { 0, VK_SPACE, kbA15, kbD0, 255, 255 },

        { 0, 'A', kbA9, kbD0, 255, 255 },
        { 0, 'B', kbA15, kbD3, 255, 255 },
        { 0, 'C', kbA8, kbD4, 255, 255 },
        { 0, 'D', kbA9, kbD2, 255, 255 },
        { 0, 'E', kbA10, kbD2, 255, 255 },
        { 0, 'F', kbA9, kbD3, 255, 255 },
        { 0, 'G', kbA9, kbD4, 255, 255 },
        { 0, 'H', kbA14, kbD4, 255, 255 },
        { 0, 'I', kbA13, kbD2, 255, 255 },
        { 0, 'J', kbA14, kbD3, 255, 255 },
        { 0, 'K', kbA14, kbD2, 255, 255 },
        { 0, 'L', kbA14, kbD1, 255, 255 },
        { 0, 'M', kbA15, kbD1, 255, 255 },
        { 0, 'N', kbA15, kbD2, 255, 255 },
        { 0, 'O', kbA13, kbD1, 255, 255 },
        { 0, 'P', kbA13, kbD0, 255, 255 },
        { 0, 'Q', kbA10, kbD0, 255, 255 },
        { 0, 'R', kbA10, kbD3, 255, 255 },
        { 0, 'S', kbA9, kbD1, 255, 255 },
        { 0, 'T', kbA10, kbD4, 255, 255 },
        { 0, 'U', kbA13, kbD3, 255, 255 },
        { 0, 'V', kbA15, kbD4, 255, 255 },
        { 0, 'W', kbA10, kbD1, 255, 255 },
        { 0, 'X', kbA8, kbD3, 255, 255 },
        { 0, 'Y', kbA13, kbD4, 255, 255 },
        { 0, 'Z', kbA8, kbD2, 255, 255 },

        { 0, '1', kbA11, kbD0, 255, 255 },
        { 0, '2', kbA11, kbD1, 255, 255 },
        { 0, '3', kbA11, kbD2, 255, 255 },
        { 0, '4', kbA11, kbD3, 255, 255 },
        { 0, '5', kbA11, kbD4, 255, 255 },
        { 0, '6', kbA12, kbD4, 255, 255 },
        { 0, '7', kbA12, kbD3, 255, 255 },
        { 0, '8', kbA12, kbD2, 255, 255 },
        { 0, '9', kbA12, kbD1, 255, 255 },
        { 0, '0', kbA12, kbD0, 255, 255 },

        { 0, VK_NUMPAD1, kbA11, kbD0, 255, 255 },
        { 0, VK_NUMPAD2, kbA11, kbD1, 255, 255 },
        { 0, VK_NUMPAD3, kbA11, kbD2, 255, 255 },
        { 0, VK_NUMPAD4, kbA11, kbD3, 255, 255 },
        { 0, VK_NUMPAD5, kbA11, kbD4, 255, 255 },
        { 0, VK_NUMPAD6, kbA12, kbD4, 255, 255 },
        { 0, VK_NUMPAD7, kbA12, kbD3, 255, 255 },
        { 0, VK_NUMPAD8, kbA12, kbD2, 255, 255 },
        { 0, VK_NUMPAD9, kbA12, kbD1, 255, 255 },
        { 0, VK_NUMPAD0, kbA12, kbD0, 255, 255 },

        { 0, VK_MULTIPLY, kbA15, kbD3, kbA8, kbD1 },
        { 0, VK_DIVIDE, kbA15, kbD4, kbA8, kbD1 },
        { 0, VK_SUBTRACT, kbA14, kbD3, kbA8, kbD1 },
        { 0, VK_ADD, kbA14, kbD2, kbA8, kbD1 },
        { 1, VK_DECIMAL, kbA15, kbD1, kbA8, kbD1 },

        { 1, VK_OEM_1, kbA13, kbD1, kbA8, kbD1 },
        { 2, VK_OEM_1, kbA8, kbD2, kbA8, kbD1 },
        { 1, VK_OEM_3, kbA12, kbD3, kbA8, kbD1 },
        { 2, VK_OEM_3, kbA11, kbD1, kbA8, kbD1 },
        { 1, VK_OEM_7, kbA11, kbD2, kbA8, kbD1 },
        { 2, VK_OEM_7, kbA9, kbD0, kbA8, kbD1 },
        { 1, VK_OEM_4, kbA13, kbD4, kbA8, kbD1 },
        { 1, VK_OEM_6, kbA13, kbD3, kbA8, kbD1 },
        { 2, VK_OEM_4, kbA9, kbD3, kbA8, kbD1 },
        { 2, VK_OEM_6, kbA9, kbD4, kbA8, kbD1 },
        { 1, VK_OEM_MINUS, kbA14, kbD3, kbA8, kbD1 },
        { 2, VK_OEM_MINUS, kbA12, kbD0, kbA8, kbD1 },
        { 1, VK_OEM_PLUS, kbA14, kbD1, kbA8, kbD1 },
        { 2, VK_OEM_PLUS, kbA14, kbD2, kbA8, kbD1 },
        { 1, VK_OEM_MINUS, kbA14, kbD3, kbA8, kbD1 },
        { 2, VK_OEM_MINUS, kbA12, kbD0, kbA8, kbD1 },
        { 1, VK_OEM_COMMA, kbA15, kbD2, kbA8, kbD1 },
        { 1, VK_OEM_PERIOD, kbA15, kbD1, kbA8, kbD1 },
        { 2, VK_OEM_COMMA, kbA10, kbD3, kbA8, kbD1 },
        { 2, VK_OEM_PERIOD, kbA10, kbD4, kbA8, kbD1 },
        { 1, VK_OEM_2, kbA15, kbD4, kbA8, kbD1 },
        { 2, VK_OEM_2, kbA8, kbD4, kbA8, kbD1 },
        { 1, VK_OEM_5, kbA9, kbD2, kbA8, kbD1 },
        { 2, VK_OEM_5, kbA9, kbD1, kbA8, kbD1 },

        { 0, VK_BACK, kbA12, kbD0, kbA8, kbD0 },
        { 0, VK_LEFT, kbA11, kbD4, kbA8, kbD0 },
        { 0, VK_UP, kbA12, kbD4, kbA8, kbD0 },
        { 0, VK_DOWN, kbA12, kbD3, kbA8, kbD0 },
        { 0, VK_RIGHT, kbA12, kbD2, kbA8, kbD0 },

        { 0, VK_CAPITAL, kbA11, kbD1, kbA8, kbD0 },
        {0, 0, 0, 0, 0, 0 }
};

const struct kb KBLAMBDA[]=
{
        { 0, VK_SHIFT, kbA8, kbD0, 255, 255 },
        { 0, VK_RETURN, kbA14, kbD0, 255, 255 },
        { 0, VK_SPACE, kbA15, kbD0, 255, 255 },

        { 0, 'A', kbA9, kbD0, 255, 255 },
        { 0, 'B', kbA15, kbD4, 255, 255 },
        { 0, 'C', kbA8, kbD3, 255, 255 },
        { 0, 'D', kbA9, kbD2, 255, 255 },
        { 0, 'E', kbA10, kbD2, 255, 255 },
        { 0, 'F', kbA9, kbD3, 255, 255 },
        { 0, 'G', kbA9, kbD4, 255, 255 },
        { 0, 'H', kbA14, kbD4, 255, 255 },
        { 0, 'I', kbA13, kbD2, 255, 255 },
        { 0, 'J', kbA14, kbD3, 255, 255 },
        { 0, 'K', kbA14, kbD2, 255, 255 },
        { 0, 'L', kbA14, kbD1, 255, 255 },
        { 0, 'M', kbA15, kbD2, 255, 255 },
        { 0, 'N', kbA15, kbD3, 255, 255 },
        { 0, 'O', kbA13, kbD1, 255, 255 },
        { 0, 'P', kbA13, kbD0, 255, 255 },
        { 0, 'Q', kbA10, kbD0, 255, 255 },
        { 0, 'R', kbA10, kbD3, 255, 255 },
        { 0, 'S', kbA9, kbD1, 255, 255 },
        { 0, 'T', kbA10, kbD4, 255, 255 },
        { 0, 'U', kbA13, kbD3, 255, 255 },
        { 0, 'V', kbA8, kbD4, 255, 255 },
        { 0, 'W', kbA10, kbD1, 255, 255 },
        { 0, 'X', kbA8, kbD2, 255, 255 },
        { 0, 'Y', kbA13, kbD4, 255, 255 },
        { 0, 'Z', kbA8, kbD1, 255, 255 },

        { 0, '1', kbA11, kbD0, 255, 255 },
        { 0, '2', kbA11, kbD1, 255, 255 },
        { 0, '3', kbA11, kbD2, 255, 255 },
        { 0, '4', kbA11, kbD3, 255, 255 },
        { 0, '5', kbA11, kbD4, 255, 255 },
        { 0, '6', kbA12, kbD4, 255, 255 },
        { 0, '7', kbA12, kbD3, 255, 255 },
        { 0, '8', kbA12, kbD2, 255, 255 },
        { 0, '9', kbA12, kbD1, 255, 255 },
        { 0, '0', kbA12, kbD0, 255, 255 },

        { 0, VK_NUMPAD1, kbA11, kbD0, 255, 255 },
        { 0, VK_NUMPAD2, kbA11, kbD1, 255, 255 },
        { 0, VK_NUMPAD3, kbA11, kbD2, 255, 255 },
        { 0, VK_NUMPAD4, kbA11, kbD3, 255, 255 },
        { 0, VK_NUMPAD5, kbA11, kbD4, 255, 255 },
        { 0, VK_NUMPAD6, kbA12, kbD4, 255, 255 },
        { 0, VK_NUMPAD7, kbA12, kbD3, 255, 255 },
        { 0, VK_NUMPAD8, kbA12, kbD2, 255, 255 },
        { 0, VK_NUMPAD9, kbA12, kbD1, 255, 255 },
        { 0, VK_NUMPAD0, kbA12, kbD0, 255, 255 },

        { 0, VK_MULTIPLY, kbA14, kbD3, kbA8, kbD0 },
        { 0, VK_DIVIDE, kbA14, kbD4, kbA8, kbD0 },
        { 0, VK_SUBTRACT, kbA14, kbD2, kbA8, kbD0 },
        { 0, VK_ADD, kbA14, kbD1, kbA8, kbD0 },
        { 1, VK_DECIMAL, kbA15, kbD1, 255, 255 },

        { 1, VK_OEM_1, kbA12, kbD4, kbA8, kbD0 },
        //{ 2, VK_OEM_1, kbA8, kbD1, kbA8, kbD0 }, // No : on the lambda
        { 1, VK_OEM_MINUS, kbA14, kbD2, kbA8, kbD0 },

        { 1, VK_OEM_PLUS, kbA12, kbD0, kbA8, kbD0 },
        { 2, VK_OEM_PLUS, kbA14, kbD1, kbA8, kbD0 },

        { 1, VK_OEM_COMMA, kbA12, kbD3, kbA8, kbD0 },
        { 2, VK_OEM_COMMA, kbA13, kbD1, kbA8, kbD0 },

        { 1, VK_OEM_PERIOD, kbA15, kbD1, 255, 255 },
        { 2, VK_OEM_PERIOD, kbA13, kbD0, kbA8, kbD0 },
        { 1, VK_OEM_2, kbA14, kbD4, kbA8, kbD0 },

        { 0, VK_OEM_4, kbA12, kbD2, kbA8, kbD0 },
        { 0, VK_OEM_6, kbA12, kbD1, kbA8, kbD0 },

        // { 2, VK_OEM_2, kbA8, kbD3, kbA8, kbD0 }, // Lambda has no ? symbol!
        { 2, VK_OEM_7, kbA11, kbD4, kbA8, kbD0 },

        //{ 2, VK_OEM_COMMA, kbA15, kbD3, kbA8, kbD0 },
        //{ 2, VK_OEM_PERIOD, kbA15, kbD2, kbA8, kbD0 },

        { 0, VK_BACK, kbA15, kbD1, kbA8, kbD0 },
        { 0, VK_LEFT, kbA11, kbD4, kbA8, kbD0 },
        { 0, VK_DOWN, kbA12, kbD4, kbA8, kbD0 },
        { 0, VK_UP, kbA12, kbD3, kbA8, kbD0 },
        { 0, VK_RIGHT, kbA12, kbD2, kbA8, kbD0 },

        { 0, VK_CONTROL, kbA14, kbD0, kbA8, kbD0 },

        {0, 0, 0, 0, 0, 0 }
};

const struct kb KBSPEC[]=
{
        { 0, VK_SHIFT, kbA8, kbD0, 255, 255 },
        { 0, VK_RETURN, kbA14, kbD0, 255, 255 },
        { 0, VK_SPACE, kbA15, kbD0, 255, 255 },

        { 0, 'A', kbA9, kbD0, 255, 255 },
        { 0, 'B', kbA15, kbD4, 255, 255 },
        { 0, 'C', kbA8, kbD3, 255, 255 },
        { 0, 'D', kbA9, kbD2, 255, 255 },
        { 0, 'E', kbA10, kbD2, 255, 255 },
        { 0, 'F', kbA9, kbD3, 255, 255 },
        { 0, 'G', kbA9, kbD4, 255, 255 },
        { 0, 'H', kbA14, kbD4, 255, 255 },
        { 0, 'I', kbA13, kbD2, 255, 255 },
        { 0, 'J', kbA14, kbD3, 255, 255 },
        { 0, 'K', kbA14, kbD2, 255, 255 },
        { 0, 'L', kbA14, kbD1, 255, 255 },
        { 0, 'M', kbA15, kbD2, 255, 255 },
        { 0, 'N', kbA15, kbD3, 255, 255 },
        { 0, 'O', kbA13, kbD1, 255, 255 },
        { 0, 'P', kbA13, kbD0, 255, 255 },
        { 0, 'Q', kbA10, kbD0, 255, 255 },
        { 0, 'R', kbA10, kbD3, 255, 255 },
        { 0, 'S', kbA9, kbD1, 255, 255 },
        { 0, 'T', kbA10, kbD4, 255, 255 },
        { 0, 'U', kbA13, kbD3, 255, 255 },
        { 0, 'V', kbA8, kbD4, 255, 255 },
        { 0, 'W', kbA10, kbD1, 255, 255 },
        { 0, 'X', kbA8, kbD2, 255, 255 },
        { 0, 'Y', kbA13, kbD4, 255, 255 },
        { 0, 'Z', kbA8, kbD1, 255, 255 },

        { 0, '1', kbA11, kbD0, 255, 255 },
        { 0, '2', kbA11, kbD1, 255, 255 },
        { 0, '3', kbA11, kbD2, 255, 255 },
        { 0, '4', kbA11, kbD3, 255, 255 },
        { 0, '5', kbA11, kbD4, 255, 255 },
        { 0, '6', kbA12, kbD4, 255, 255 },
        { 0, '7', kbA12, kbD3, 255, 255 },
        { 0, '8', kbA12, kbD2, 255, 255 },
        { 0, '9', kbA12, kbD1, 255, 255 },
        { 0, '0', kbA12, kbD0, 255, 255 },

        { 0, VK_NUMPAD1, kbA11, kbD0, 255, 255 },
        { 0, VK_NUMPAD2, kbA11, kbD1, 255, 255 },
        { 0, VK_NUMPAD3, kbA11, kbD2, 255, 255 },
        { 0, VK_NUMPAD4, kbA11, kbD3, 255, 255 },
        { 0, VK_NUMPAD5, kbA11, kbD4, 255, 255 },
        { 0, VK_NUMPAD6, kbA12, kbD4, 255, 255 },
        { 0, VK_NUMPAD7, kbA12, kbD3, 255, 255 },
        { 0, VK_NUMPAD8, kbA12, kbD2, 255, 255 },
        { 0, VK_NUMPAD9, kbA12, kbD1, 255, 255 },
        { 0, VK_NUMPAD0, kbA12, kbD0, 255, 255 },

        { 0, VK_MULTIPLY, kbA15, kbD4, kbA15, kbD1 },
        { 0, VK_DIVIDE, kbA8, kbD4, kbA15, kbD1 },
        { 0, VK_SUBTRACT, kbA14, kbD3, kbA15, kbD1 },
        { 0, VK_ADD, kbA14, kbD2, kbA15, kbD1 },
        { 1, VK_DECIMAL, kbA15, kbD2, kbA15, kbD1 },

        { 1, VK_OEM_1, kbA13, kbD1, kbA15, kbD1 },
        { 2, VK_OEM_1, kbA8, kbD1, kbA15, kbD1 },
        { 1, VK_OEM_MINUS, kbA14, kbD3, kbA15, kbD1 },
        { 2, VK_OEM_MINUS, kbA12, kbD0, kbA15, kbD1 },

        { 1, VK_OEM_PLUS, kbA14, kbD1, kbA15, kbD1 },
        { 2, VK_OEM_PLUS, kbA14, kbD2, kbA15, kbD1 },

        { 1, VK_OEM_COMMA, kbA15, kbD3, kbA15, kbD1 },
        { 2, VK_OEM_COMMA, kbA10, kbD3, kbA15, kbD1 },

        { 1, VK_OEM_PERIOD, kbA15, kbD2, kbA15, kbD1 },
        { 2, VK_OEM_PERIOD, kbA10, kbD4, kbA15, kbD1 },
        { 1, VK_OEM_2, kbA8, kbD4, kbA15, kbD1 },
        { 2, VK_OEM_2, kbA8, kbD3, kbA15, kbD1 },
        { 1, VK_OEM_3, kbA12, kbD3, kbA15, kbD1 },
        { 2, VK_OEM_3, kbA11, kbD1, kbA15, kbD1 },
        { 0, VK_OEM_4, kbA12, kbD2, kbA15, kbD1 },
        { 0, VK_OEM_6, kbA12, kbD1, kbA15, kbD1 },
        { 1, VK_OEM_7, kbA11, kbD2, kbA15, kbD1 },
        { 2, VK_OEM_7, 0, 0, kbA15, kbD1 },

        //{ 2, VK_OEM_COMMA, kbA15, kbD3, kbA8, kbD0 },
        //{ 2, VK_OEM_PERIOD, kbA15, kbD2, kbA8, kbD0 },

        { 0, VK_BACK, kbA12, kbD0, kbA8, kbD0 },
        { 0, VK_LEFT, kbA11, kbD4, kbA8, kbD0 },
        { 0, VK_DOWN, kbA12, kbD4, kbA8, kbD0 },
        { 0, VK_UP, kbA12, kbD3, kbA8, kbD0 },
        { 0, VK_RIGHT, kbA12, kbD2, kbA8, kbD0 },

        { 0, VK_CONTROL, kbA15, kbD1, 255, 255 },
        { 0, VK_CAPITAL, kbA11, kbD1, kbA8, kbD0 },

        {0, 0, 0, 0, 0, 0 }
};
//---------------------------------------------------------------------------

#pragma package(smart_init)

void PCKbInit(void)
{
        PCAllKeysUp();

        switch(emulator.machine)
        {
        case MACHINEZX80:
                BaseMap = KBZX80;
                break;

        case MACHINESPECTRUM:
                BaseMap = KBSPEC;
                break;

        case MACHINEACE:
                BaseMap = KBACE;
                break;

        case MACHINELAMBDA:
                BaseMap = KBLAMBDA;
                break;

        default:
                BaseMap = KBZX81;
                break;
        }

        SetKeyMap(BaseMap);

        AdjustLocalKeyboard();
}

void SetKeyMap(const kb *source)
{
        int i=0;

        while (source[i].WinKey)
        {
                KeyMap[i]=source[i];
                i++;
        }
        KeyMap[i]=source[i];
}

int PCFindKey(Word key, bool shift)
{
        int i=0;

        while (KeyMap[i].WinKey)
        {
                if (KeyMap[i].WinKey == key)
                {
                        if ((KeyMap[i].Shift==2 && shift) || (KeyMap[i].Shift<2 && !shift))
                                return(i);
                }
                i++;
        }

        return(-1);
}

int PCFindKey(Word key)
{
        return PCFindKey(key,false);
}

void PCSetKey(WORD dest, int source, int shift)
{
        int d;

        d=PCFindKey(dest);

        if (d!=-1)
        {
                KeyMap[d].Addr1 = KeyMap[source].Addr1;
                KeyMap[d].Data1 = KeyMap[source].Data1;

                if (shift)
                {
                        KeyMap[d].Addr2 = kbA8;
                        KeyMap[d].Data2 = kbD0;
                }
                else
                {
                        KeyMap[d].Addr2 = 255;
                        KeyMap[d].Data2 = 255;
                }
        }
}

void PCKeySetCTRL(char key)
{
        int Kctrl;

        if (emulator.machine==MACHINESPECTRUM
                || emulator.machine==MACHINEACE) return;

        if (!key)
        {
                Kctrl=PCFindKey(VK_RETURN);
                PCSetKey(VK_CONTROL,Kctrl,1);
        }
        else
        {
                Kctrl=PCFindKey(key);
                PCSetKey(VK_CONTROL,Kctrl,0);
        }
}

void PCKeySetCursor(char left, char down, char up, char right, int shift)
{
        int Kleft,Kdown, Kright, Kup;
 
        Kleft=PCFindKey(left);
        Kdown=PCFindKey(down);
        Kup=PCFindKey(up);
        Kright=PCFindKey(right);

        PCSetKey(VK_LEFT,Kleft,shift);
        PCSetKey(VK_DOWN,Kdown,shift);
        PCSetKey(VK_UP,Kup,shift);
        PCSetKey(VK_RIGHT,Kright,shift);
}

void PCKeyDown(WORD key)
{
        if (spectrum.spectrum128Keypad && IsKeyPressed(VK_RCONTROL))
        {
                return;
        }

        int i = 0;
        if (key == VK_SHIFT) PCShift = 2;
        PCALT=IsKeyPressed(VK_MENU);

        if (PCALT) return;

        while (KeyMap[i].WinKey)
        {
                if ((KeyMap[i].WinKey == key) && ((KeyMap[i].Shift == PCShift) || (KeyMap[i].Shift == 0)))
                {
                        bool numpadKey = (key >= VK_NUMPAD0 && key <= VK_DIVIDE);
                        bool numLockOn = ((GetKeyState(VK_NUMLOCK) & 0x0001) == 0x0001);

                        if (!numpadKey || !emulator.UseNumericPadForJoystick1 || !emulator.UseNumericPadForJoystick2 || !numLockOn)
                        {
                                if (key >= VK_OEM_1 && key <= VK_OEM_7 && emulator.machine == MACHINESPECTRUM)
                                {
                                        ZXKeyboard[kbA8] &= ~kbD0;
                                }

                                ZXKeyboard[KeyMap[i].Addr1] |= KeyMap[i].Data1;

                                if (KeyMap[i].Addr2 != 255)
                                {
                                        ZXKeyboard[KeyMap[i].Addr2] |= KeyMap[i].Data2;
                                }

                                return;
                        }
                }
                i++;
        }
}

void PCKeyUp(WORD key)
{
        if (spectrum.spectrum128Keypad && IsKeyPressed(VK_RSHIFT))
        {
                return;
        }

        int i = 0;

        if (key == VK_SHIFT) PCShift=1;

        while (KeyMap[i].WinKey)
        {
                if (KeyMap[i].WinKey == key)
                {
                        bool numpadKey = (key >= VK_NUMPAD0 && key <= VK_DIVIDE);
                        bool numLockOn = ((GetKeyState(VK_NUMLOCK) & 0x0001) == 0x0001);

                        if (!numpadKey || !emulator.UseNumericPadForJoystick1 || !emulator.UseNumericPadForJoystick2 || !numLockOn)
                        {
                                ZXKeyboard[KeyMap[i].Addr1] &= (BYTE)(~KeyMap[i].Data1);
                                if (KeyMap[i].Addr2 != 255)
                                {
                                        ZXKeyboard[KeyMap[i].Addr2] &= (BYTE)(~KeyMap[i].Data2);
                                }
                        }
                }
                i++;
        }

        if (PCShift == 2) ZXKeyboard[kbA8] |= kbD0;
}

static void ForgetCharKeys();

void PCAllKeysUp()
{
        int i;
        for(i=0; i<8; i++) ZXKeyboard[i]=0;
        ForgetCharKeys();
}

void AdjustLocalKeyboard()
{
        char keyboardName[KL_NAMELENGTH];
        if (GetKeyboardLayoutName(keyboardName))
        {
                unsigned int lcid=0;
                sscanf(keyboardName,"%x",&lcid);

                switch (lcid)
                {
                case 0x00000404: // Chinese (Traditional) - US
                case 0x00000409: // US
                case 0x00000804: // Chinese (Simplified) - US
                case 0x00000c04: // Chinese (Traditional, Hong Kong S.A.R.) - US
                case 0x00001004: // Chinese (Simplified, Singapore) - US
                case 0x00001404: // Chinese (Traditional, Macao S.A.R.) - US
                case 0x00010402: // Bulgarian (Latin)
                case 0x00020409: // United States-International
                        if (emulator.machine==MACHINESPECTRUM || emulator.machine==MACHINEACE)
                        {
                                int source=PCFindKey(VK_OEM_3,false);
                                int dest=PCFindKey(VK_OEM_7,false);
                                KeyMap[dest].Addr1 = KeyMap[source].Addr1;
                                KeyMap[dest].Data1 = KeyMap[source].Data1;

                                source=PCFindKey('P');
                                dest=PCFindKey(VK_OEM_7,true);
                                BYTE tempAddr1=KeyMap[dest].Addr1;
                                BYTE tempData1=KeyMap[dest].Data1;
                                KeyMap[dest].Addr1 = KeyMap[source].Addr1;
                                KeyMap[dest].Data1 = KeyMap[source].Data1;

                                dest=PCFindKey(VK_OEM_3,true);
                                KeyMap[dest].Addr1 = tempAddr1;
                                KeyMap[dest].Data1 = tempData1;

                                keypadMultiplyKey = "\\";
                        }
                        break;

                default:
                        break;
                }
        }
}

AnsiString GetKeypadMultiplyKey()
{
         return keypadMultiplyKey;
}

void DetermineKeyMatrixPosition(int machine, KeyInfo& keyInfo)
{
        const kb* mapEntry;

        switch (machine)
        {
        case MACHINEZX80:
                mapEntry = KBZX80;
                break;

        case MACHINESPECTRUM:
                mapEntry = KBSPEC;
                break;

        case MACHINEACE:
                mapEntry = KBACE;
                break;

        case MACHINELAMBDA:
                mapEntry = KBLAMBDA;
                break;

        default:
                mapEntry = KBZX81;
                break;
        }
        
        char c;

        if (keyInfo.Character == '^')
        {
                c = VK_SHIFT;
        }
        else if (keyInfo.Character == '.')
        {
                c = VK_CONTROL;
        }
        else if (keyInfo.Character == '#')
        {
                c = VK_RETURN;
        }
        else
        {
                c = keyInfo.Character;
        }

        while (mapEntry->WinKey != 0)
        {
                if (mapEntry->Shift == 0 && mapEntry->WinKey == c)
                {
                        keyInfo.AddressMask = (WORD)(1 << (mapEntry->Addr1 + 8));
                        keyInfo.Data = (BYTE)~(mapEntry->Data1);
                        return;
                }

                mapEntry++;
        }
}

//---------------------------------------------------------------------------
// Typing with the Windows keyboard layout
//
// A key that gives a character is typed as that character: the key
// combination that gives the character on the emulated machine is pressed,
// whatever Windows key it came from (on the Hungarian layout, Shift+2 gives
// " which is SHIFT+P on the ZX81). The machine's own shift is taken off
// for it, or put on if the character needs it.
//
// - Letters stay keys: Shift+letter is the machine's SHIFT+letter, as
//   before (keywords and symbols on the ZX80, ZX81 and Lambda, capitals on
//   the Spectrum and the Ace).
// - Accented letters are typed without the accent.
// - Characters the machine has no key for are ignored.
// - Keys without a character (cursors, Enter, Backspace, Ctrl, the function
//   keys) and the numeric keypad work as before.
// - The left Alt and a number key give SHIFT + that number (EDIT, AND,
//   THEN, cursors, GRAPHICS, RUBOUT...), which Shift+number now can't.
// - AltGr is sent by Windows as Ctrl + Alt; its Ctrl is not passed on, so
//   the characters typed with AltGr don't press what Ctrl is mapped to.
//---------------------------------------------------------------------------

// The modifier a character needs
#define KEYMOD_NONE   0
#define KEYMOD_SHIFT  1    // SHIFT (CAPS SHIFT on the Spectrum)
#define KEYMOD_SYMBOL 2    // SYMBOL SHIFT (Spectrum and Ace)

// The period key of the ZX80, ZX81 and Lambda
#define KEY_PERIOD '.'

struct kbchar
{
        unsigned short Char;    // UTF-16
        char Key;               // 'A'-'Z', '0'-'9', ' ' or KEY_PERIOD
        BYTE Mod;
};

static const struct kbchar CharsZX81[]=
{
        { '"', 'P', KEYMOD_SHIFT },  { '$', 'U', KEYMOD_SHIFT },
        { '(', 'I', KEYMOD_SHIFT },  { ')', 'O', KEYMOD_SHIFT },
        { '-', 'J', KEYMOD_SHIFT },  { '+', 'K', KEYMOD_SHIFT },
        { '=', 'L', KEYMOD_SHIFT },  { ':', 'Z', KEYMOD_SHIFT },
        { ';', 'X', KEYMOD_SHIFT },  { '?', 'C', KEYMOD_SHIFT },
        { '/', 'V', KEYMOD_SHIFT },  { '*', 'B', KEYMOD_SHIFT },
        { '<', 'N', KEYMOD_SHIFT },  { '>', 'M', KEYMOD_SHIFT },
        { ',', KEY_PERIOD, KEYMOD_SHIFT }, { '.', KEY_PERIOD, KEYMOD_NONE },
        { 0x00a3, ' ', KEYMOD_SHIFT },     // pound sign
        { 0x00d7, 'B', KEYMOD_SHIFT },    // multiplication sign
        { 0x00f7, 'V', KEYMOD_SHIFT },    // division sign
        { 0, 0, 0 }
};

static const struct kbchar CharsZX80[]=
{
        { '"', 'Y', KEYMOD_SHIFT },  { '$', 'U', KEYMOD_SHIFT },
        { '(', 'I', KEYMOD_SHIFT },  { ')', 'O', KEYMOD_SHIFT },
        { '*', 'P', KEYMOD_SHIFT },  { '-', 'J', KEYMOD_SHIFT },
        { '+', 'K', KEYMOD_SHIFT },  { '=', 'L', KEYMOD_SHIFT },
        { ':', 'Z', KEYMOD_SHIFT },  { ';', 'X', KEYMOD_SHIFT },
        { '?', 'C', KEYMOD_SHIFT },  { '/', 'V', KEYMOD_SHIFT },
        { '<', 'N', KEYMOD_SHIFT },  { '>', 'M', KEYMOD_SHIFT },
        { ',', KEY_PERIOD, KEYMOD_SHIFT }, { '.', KEY_PERIOD, KEYMOD_NONE },
        { 0x00a3, ' ', KEYMOD_SHIFT },     // pound sign
        { 0x00d7, 'P', KEYMOD_SHIFT },    // multiplication sign
        { 0x00f7, 'V', KEYMOD_SHIFT },    // division sign
        { 0, 0, 0 }
};

static const struct kbchar CharsLambda[]=
{
        { '$', '4', KEYMOD_SHIFT },  { '"', '5', KEYMOD_SHIFT },
        { ';', '6', KEYMOD_SHIFT },  { ',', '7', KEYMOD_SHIFT },
        { '(', '8', KEYMOD_SHIFT },  { ')', '9', KEYMOD_SHIFT },
        { '=', '0', KEYMOD_SHIFT },  { '<', 'O', KEYMOD_SHIFT },
        { '>', 'P', KEYMOD_SHIFT },  { '/', 'H', KEYMOD_SHIFT },
        { '*', 'J', KEYMOD_SHIFT },  { '-', 'K', KEYMOD_SHIFT },
        { '+', 'L', KEYMOD_SHIFT },  { '.', KEY_PERIOD, KEYMOD_NONE },
        { 0x00d7, 'J', KEYMOD_SHIFT },    // multiplication sign
        { 0x00f7, 'H', KEYMOD_SHIFT },    // division sign
        { 0, 0, 0 }
};

static const struct kbchar CharsSpectrum[]=
{
        { '!', '1', KEYMOD_SYMBOL }, { '@', '2', KEYMOD_SYMBOL },
        { '#', '3', KEYMOD_SYMBOL }, { '$', '4', KEYMOD_SYMBOL },
        { '%', '5', KEYMOD_SYMBOL }, { '&', '6', KEYMOD_SYMBOL },
        { '\'', '7', KEYMOD_SYMBOL }, { '(', '8', KEYMOD_SYMBOL },
        { ')', '9', KEYMOD_SYMBOL }, { '_', '0', KEYMOD_SYMBOL },
        { '<', 'R', KEYMOD_SYMBOL }, { '>', 'T', KEYMOD_SYMBOL },
        { ';', 'O', KEYMOD_SYMBOL }, { '"', 'P', KEYMOD_SYMBOL },
        { '^', 'H', KEYMOD_SYMBOL }, { '-', 'J', KEYMOD_SYMBOL },
        { '+', 'K', KEYMOD_SYMBOL }, { '=', 'L', KEYMOD_SYMBOL },
        { ':', 'Z', KEYMOD_SYMBOL }, { 0x00a3, 'X', KEYMOD_SYMBOL },
        { '?', 'C', KEYMOD_SYMBOL }, { '/', 'V', KEYMOD_SYMBOL },
        { '*', 'B', KEYMOD_SYMBOL }, { ',', 'N', KEYMOD_SYMBOL },
        { '.', 'M', KEYMOD_SYMBOL },
        { 0x00d7, 'B', KEYMOD_SYMBOL },    // multiplication sign
        { 0x00f7, 'V', KEYMOD_SYMBOL },    // division sign
        { 0, 0, 0 }
};

static const struct kbchar CharsAce[]=
{
        { '!', '1', KEYMOD_SYMBOL }, { '@', '2', KEYMOD_SYMBOL },
        { '#', '3', KEYMOD_SYMBOL }, { '$', '4', KEYMOD_SYMBOL },
        { '%', '5', KEYMOD_SYMBOL }, { '&', '6', KEYMOD_SYMBOL },
        { '\'', '7', KEYMOD_SYMBOL }, { '(', '8', KEYMOD_SYMBOL },
        { ')', '9', KEYMOD_SYMBOL }, { '_', '0', KEYMOD_SYMBOL },
        { '<', 'R', KEYMOD_SYMBOL }, { '>', 'T', KEYMOD_SYMBOL },
        { '[', 'Y', KEYMOD_SYMBOL }, { ']', 'U', KEYMOD_SYMBOL },
        { 0x00a9, 'I', KEYMOD_SYMBOL },    // copyright sign
        { ';', 'O', KEYMOD_SYMBOL }, { '"', 'P', KEYMOD_SYMBOL },
        { '~', 'A', KEYMOD_SYMBOL }, { '|', 'S', KEYMOD_SYMBOL },
        { '\\', 'D', KEYMOD_SYMBOL }, { '{', 'F', KEYMOD_SYMBOL },
        { '}', 'G', KEYMOD_SYMBOL }, { '^', 'H', KEYMOD_SYMBOL },
        { '-', 'J', KEYMOD_SYMBOL }, { '+', 'K', KEYMOD_SYMBOL },
        { '=', 'L', KEYMOD_SYMBOL }, { ':', 'Z', KEYMOD_SYMBOL },
        { 0x00a3, 'X', KEYMOD_SYMBOL },    // pound sign
        { '?', 'C', KEYMOD_SYMBOL }, { '/', 'V', KEYMOD_SYMBOL },
        { '*', 'B', KEYMOD_SYMBOL }, { ',', 'N', KEYMOD_SYMBOL },
        { '.', 'M', KEYMOD_SYMBOL },
        { 0x00d7, 'B', KEYMOD_SYMBOL },    // multiplication sign
        { 0x00f7, 'V', KEYMOD_SYMBOL },    // division sign
        { 0, 0, 0 }
};

// Spectrum symbols of the extended mode: typed as a timed sequence of
// CAPS SHIFT + SYMBOL SHIFT (E mode), then SYMBOL SHIFT + key. It works in
// the 48K and the 128K editors alike
static const struct kbchar CharsSpectrumExtended[]=
{
        { '[', 'Y', KEYMOD_SYMBOL }, { ']', 'U', KEYMOD_SYMBOL },
        { '{', 'F', KEYMOD_SYMBOL }, { '}', 'G', KEYMOD_SYMBOL },
        { '\\', 'D', KEYMOD_SYMBOL }, { '|', 'S', KEYMOD_SYMBOL },
        { '~', 'A', KEYMOD_SYMBOL },
        { 0x00a9, 'P', KEYMOD_SYMBOL },    // copyright sign
        { 0, 0, 0 }
};

// Accented letters, typed without the accent
static const struct
{
        unsigned short Char;
        char Letter;
} AccentedLetters[]=
{
        { 0x00e1, 'a' }, { 0x00e9, 'e' }, { 0x00ed, 'i' }, { 0x00f3, 'o' },
        { 0x00f6, 'o' }, { 0x0151, 'o' }, { 0x00fa, 'u' }, { 0x00fc, 'u' },
        { 0x0171, 'u' }, { 0x00e4, 'a' },
        { 0x00c1, 'A' }, { 0x00c9, 'E' }, { 0x00cd, 'I' }, { 0x00d3, 'O' },
        { 0x00d6, 'O' }, { 0x0150, 'O' }, { 0x00da, 'U' }, { 0x00dc, 'U' },
        { 0x0170, 'U' }, { 0x00c4, 'A' },
        { 0, 0 }
};

// Keys held down that were typed as characters, by their Windows key.
// Addr1 is NO_KEY if nothing is held for it on the emulated machine (a
// character it has no key for, or one waiting in the sequence queue)
struct charkey
{
        WORD WinKey;
        BYTE Addr1, Data1, Addr2, Data2;
};

#define NO_KEY 255

#define MAX_CHAR_KEYS 16
static struct charkey CharKeys[MAX_CHAR_KEYS];
static int CharKeyCount = 0;

// The timed sequences: characters typed while one runs wait in the queue
// too, to keep the order of typing
struct queuedchar
{
        struct charkey Keys;
        bool Extended;
};

#define QUEUE_SIZE 32
#define STEP_FRAMES 3   // frames each step of a sequence lasts

static struct queuedchar Queue[QUEUE_SIZE];
static int QueueStart = 0, QueueCount = 0;

// The step of the sequence at the head of the queue: 0: press CAPS SHIFT +
// SYMBOL SHIFT (E mode), 1: release them, 2: press the key, 3: release it
static int QueueStep = 0, QueueStepFrames = 0;

static void ForgetCharKeys()
{
        CharKeyCount = 0;
        QueueStart = QueueCount = 0;
        QueueStep = QueueStepFrames = 0;
}

static int FindCharKey(WORD winKey)
{
        int i;

        for (i=0; i<CharKeyCount; i++)
                if (CharKeys[i].WinKey == winKey) return i;

        return -1;
}

static void ReleaseBits(const struct charkey &k)
{
        if (k.Addr1 == NO_KEY) return;
        ZXKeyboard[k.Addr1] &= (BYTE)(~k.Data1);
        if (k.Addr2 != NO_KEY) ZXKeyboard[k.Addr2] &= (BYTE)(~k.Data2);
}

static void PressBits(const struct charkey &k)
{
        if (k.Addr1 == NO_KEY) return;
        ZXKeyboard[k.Addr1] |= k.Data1;
        if (k.Addr2 != NO_KEY) ZXKeyboard[k.Addr2] |= k.Data2;
}

// Keys still held keep their bits, if another key shared them
static void PressCharKeys()
{
        int i;

        if (QueueCount) return;         // the sequence owns the keyboard

        for (i=0; i<CharKeyCount; i++) PressBits(CharKeys[i]);
}

// Remember a Windows key as held, with what it holds on the machine
static void HoldCharKey(const struct charkey &k)
{
        int i = FindCharKey(k.WinKey);

        if (i != -1)
        {
                // A key repeating may give another character now
                ReleaseBits(CharKeys[i]);
                CharKeys[i] = k;
        }
        else if (CharKeyCount < MAX_CHAR_KEYS)
        {
                CharKeys[CharKeyCount++] = k;
        }
}

static void HoldNothing(WORD winKey)
{
        struct charkey k;

        k.WinKey = winKey;
        k.Addr1 = k.Data1 = k.Addr2 = k.Data2 = NO_KEY;
        HoldCharKey(k);
}

static bool IsZX81Family()
{
        return emulator.machine != MACHINESPECTRUM
                && emulator.machine != MACHINEACE;
}

// The place of a key of the emulated machine in its keyboard matrix
static bool FindKeyPosition(char key, BYTE &addr, BYTE &data)
{
        int i;

        if (key == KEY_PERIOD)
        {
                if (!IsZX81Family()) return false;
                addr = kbA15;
                data = kbD1;
                return true;
        }

        if (!BaseMap) return false;

        for (i=0; BaseMap[i].WinKey; i++)
        {
                if (BaseMap[i].Shift == 0 && BaseMap[i].WinKey == (WORD)(BYTE)key
                        && BaseMap[i].Addr2 == 255)
                {
                        addr = BaseMap[i].Addr1;
                        data = BaseMap[i].Data1;
                        return true;
                }
        }

        return false;
}

// The bits of a key of the emulated machine with a modifier
static bool MakeCharKey(WORD winKey, char key, BYTE mod, struct charkey &k)
{
        if (!FindKeyPosition(key, k.Addr1, k.Data1)) return false;

        k.WinKey = winKey;
        k.Addr2 = NO_KEY;
        k.Data2 = NO_KEY;

        if (mod == KEYMOD_SHIFT)
        {
                k.Addr2 = kbA8;
                k.Data2 = kbD0;
        }
        else if (mod == KEYMOD_SYMBOL)
        {
                if (emulator.machine == MACHINESPECTRUM)
                {
                        k.Addr2 = kbA15;
                        k.Data2 = kbD1;
                }
                else if (emulator.machine == MACHINEACE)
                {
                        k.Addr2 = kbA8;
                        k.Data2 = kbD1;
                }
                else return false;
        }

        return true;
}

// Press the key of the emulated machine for a Windows key, with the
// modifier given; the shift of the machine is taken off otherwise
static void PressCharKey(WORD winKey, char key, BYTE mod)
{
        struct charkey k;

        if (!MakeCharKey(winKey, key, mod, k))
        {
                HoldNothing(winKey);
                return;
        }

        HoldCharKey(k);

        // The SHIFT key is on the same place on every machine
        ZXKeyboard[kbA8] &= (BYTE)(~kbD0);
        PressBits(k);
}

static void QueueCharKey(const struct charkey &k, bool extended)
{
        int i;

        if (QueueCount >= QUEUE_SIZE) return;

        Queue[(QueueStart + QueueCount) % QUEUE_SIZE].Keys = k;
        Queue[(QueueStart + QueueCount) % QUEUE_SIZE].Extended = extended;

        if (QueueCount == 0)
        {
                // A sequence starts: the keys typed as characters stop
                // holding their keys on the machine (they are still held
                // on the PC)
                for (i=0; i<CharKeyCount; i++)
                {
                        ReleaseBits(CharKeys[i]);
                        CharKeys[i].Addr1 = NO_KEY;
                }

                QueueStep = extended ? 0 : 2;
                QueueStepFrames = 0;
        }

        QueueCount++;
}

static const struct kbchar *CharMap()
{
        switch (emulator.machine)
        {
        case MACHINEZX80:     return CharsZX80;
        case MACHINESPECTRUM: return CharsSpectrum;
        case MACHINEACE:      return CharsAce;
        case MACHINELAMBDA:   return CharsLambda;
        default:              return CharsZX81;
        }
}

static bool FindChar(const struct kbchar *map, unsigned short c,
                     char &key, BYTE &mod)
{
        int i;

        for (i=0; map[i].Char; i++)
        {
                if (map[i].Char == c)
                {
                        key = map[i].Key;
                        mod = map[i].Mod;
                        return true;
                }
        }

        return false;
}

// The key combination of the emulated machine for a character. Returns
// false if it has none
static bool MapChar(unsigned short c, char &key, BYTE &mod, bool &extended)
{
        int i;

        extended = false;
        mod = KEYMOD_NONE;

        for (i=0; AccentedLetters[i].Char; i++)
        {
                if (AccentedLetters[i].Char == c)
                {
                        c = AccentedLetters[i].Letter;
                        break;
                }
        }

        if ((c >= '0' && c <= '9') || c == ' ' || c == '\r')
        {
                key = (char)c;          // VK_SPACE and VK_RETURN too
                return true;
        }

        // Capitals with SHIFT on the machines that have lower case letters
        if (c >= 'a' && c <= 'z')
        {
                key = (char)(c - 'a' + 'A');
                return true;
        }

        if (c >= 'A' && c <= 'Z')
        {
                key = (char)c;
                if (!IsZX81Family()) mod = KEYMOD_SHIFT;
                return true;
        }

        if (FindChar(CharMap(), c, key, mod)) return true;

        if (emulator.machine == MACHINESPECTRUM
                && FindChar(CharsSpectrumExtended, c, key, mod))
        {
                extended = true;
                return true;
        }

        return false;
}

// Type a character with the key combination of the emulated machine
static void TypeChar(WORD winKey, unsigned short c)
{
        struct charkey k;
        char key;
        BYTE mod;
        bool extended;

        if (!MapChar(c, key, mod, extended)
                || !MakeCharKey(winKey, key, mod, k))
        {
                HoldNothing(winKey);    // ignored
                return;
        }

        // Extended mode symbols, and anything typed while a sequence runs,
        // go through the queue
        if (extended || QueueCount)
        {
                QueueCharKey(k, extended);
                HoldNothing(winKey);
                return;
        }

        PressCharKey(winKey, key, mod);
}

// Called every frame: the steps of the timed sequences
void PCKeyFrame()
{
        struct queuedchar *q;

        if (!QueueCount) return;

        q = &Queue[QueueStart];

        // The state of the step, every frame in case something else
        // changed the shifts meanwhile
        switch (QueueStep)
        {
        case 0:
                ZXKeyboard[kbA8] |= kbD0;
                ZXKeyboard[kbA15] |= kbD1;
                break;

        case 2:
                ZXKeyboard[kbA8] &= (BYTE)(~kbD0);
                if (emulator.machine == MACHINESPECTRUM)
                        ZXKeyboard[kbA15] &= (BYTE)(~kbD1);
                PressBits(q->Keys);
                break;

        default:
                if (QueueStep == 3 && QueueStepFrames == 0)
                        ReleaseBits(q->Keys);
                ZXKeyboard[kbA8] &= (BYTE)(~kbD0);
                if (emulator.machine == MACHINESPECTRUM)
                        ZXKeyboard[kbA15] &= (BYTE)(~kbD1);
                break;
        }

        if (++QueueStepFrames < STEP_FRAMES) return;

        QueueStepFrames = 0;
        if (++QueueStep <= 3) return;

        // This character is done
        QueueStart = (QueueStart + 1) % QUEUE_SIZE;
        QueueCount--;

        if (QueueCount)
        {
                QueueStep = Queue[QueueStart].Extended ? 0 : 2;
        }
        else if (PCShift == 2)
        {
                ZXKeyboard[kbA8] |= kbD0;
        }
}

// The character of this key press. TranslateMessage() has already posted
// the WM_CHAR message for it, right behind the WM_KEYDOWN being handled.
// The W variant gives Unicode even for an ANSI window. Returns 0 if none
// (keys like cursors or dead keys, which give no WM_CHAR). The message is
// left in the queue: the main form's OnKeyPress still needs some (space,
// Esc); TakeChar() removes it when it is typed here
static unsigned short PeekChar()
{
        MSG msg;
        unsigned short c;

        if (!PeekMessageW(&msg, NULL, WM_CHAR, WM_CHAR, PM_NOREMOVE)) return 0;

        c = (unsigned short)msg.wParam;

        // A character outside the Basic Multilingual Plane comes as a
        // surrogate pair; none of them is on these machines
        if (c >= 0xd800 && c <= 0xdfff) return 0;

        return c;
}

static void TakeChar()
{
        MSG msg;

        PeekMessageW(&msg, NULL, WM_CHAR, WM_CHAR, PM_REMOVE);
}

// Is this Ctrl press the one Windows sends with AltGr? The right Alt
// press follows it in the queue, with the same time
static bool IsAltGrCtrl()
{
        MSG msg;

        if (!PeekMessage(&msg, NULL, WM_KEYDOWN, WM_SYSKEYDOWN, PM_NOREMOVE))
                return false;

        return (msg.message == WM_KEYDOWN || msg.message == WM_SYSKEYDOWN)
                && msg.wParam == VK_MENU
                && (msg.lParam & (1 << 24))
                && msg.time == (DWORD)GetMessageTime();
}

// The digit of a number key of the main block, or -1. The digit may be on
// another key than VK_0-VK_9 (like 0 on the Hungarian layout), so it is
// the character the key gives without Shift
static int NumberKeyDigit(WORD key)
{
        unsigned int c;

        if (key >= VK_NUMPAD0 && key <= VK_DIVIDE) return -1;

        c = LOWORD(MapVirtualKey(key, 2));      // MAPVK_VK_TO_CHAR
        if (c >= '0' && c <= '9') return (int)(c - '0');

        return -1;
}

void PCKeyDownWin(WORD key)
{
        unsigned short c;
        int digit, i;

        if (!WindowsLayout || (key >= VK_NUMPAD0 && key <= VK_DIVIDE)
                || key == VK_SHIFT || key == VK_MENU)
        {
                PCKeyDown(key);
                return;
        }

        if (key == VK_CONTROL)
        {
                if (!IsAltGrCtrl()) PCKeyDown(key);
                return;
        }

        // The left Alt and a number key: SHIFT + number
        if (IsKeyPressed(VK_LMENU) && !IsKeyPressed(VK_RMENU))
        {
                digit = NumberKeyDigit(key);
                if (digit != -1)
                {
                        MSG msg;

                        // No beep for a menu letter not found
                        PeekMessageW(&msg, NULL, WM_SYSCHAR, WM_SYSCHAR, PM_REMOVE);
                        PressCharKey(key, (char)('0' + digit), KEYMOD_SHIFT);
                }
                return;
        }

        c = PeekChar();

        // Repeats of a key that holds nothing (queued or ignored)
        i = FindCharKey(key);
        if (c && i != -1 && CharKeys[i].Addr1 == NO_KEY)
        {
                TakeChar();
                return;
        }

        // While a sequence runs, everything typed waits for it
        if (QueueCount && (c >= 0x20 || c == '\r') && c != 0x7f)
        {
                TakeChar();
                TypeChar(key, c);
                return;
        }

        // Keys without a character, control characters, space and the
        // letters of their own keys go as before
        if (c < 0x20 || c == ' ' || c == 0x7f
                || (key >= 'A' && key <= 'Z'
                    && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))))
        {
                PCKeyDown(key);
                return;
        }

        TakeChar();
        TypeChar(key, c);
}

void PCKeyUpWin(WORD key)
{
        int i;

        if (WindowsLayout)
        {
                i = FindCharKey(key);
                if (i != -1)
                {
                        ReleaseBits(CharKeys[i]);
                        CharKeys[i] = CharKeys[--CharKeyCount];
                        if (!QueueCount && PCShift == 2)
                                ZXKeyboard[kbA8] |= kbD0;
                        PressCharKeys();
                        return;
                }
        }

        PCKeyUp(key);
        PressCharKeys();
}
