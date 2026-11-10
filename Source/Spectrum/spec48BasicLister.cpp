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

#include "spec48BasicLister.h"
#include "zx81config.h"
#include "spec48.h"
#include <string.h>

extern int SPECBlk[4];

// The following embedded codes are not supported:
// AT
// TAB
// OVER
// LEFT
// COMMA

spec48BasicLister::spec48BasicLister()
{
        mLineStartsFound = false;
        mLineStarts = 0;

        InitialiseColours();
        
        mColours[0] = RGB(0, 0, 0);
        mColours[1] = RGB(0, 0, 206);
        mColours[2] = RGB(206, 0, 0);
        mColours[3] = RGB(206, 0, 206);
        mColours[4] = RGB(0, 206, 0);
        mColours[5] = RGB(0, 206, 206);
        mColours[6] = RGB(206, 206, 0);
        mColours[7] = RGB(206, 206, 206);
        mColours[8] = RGB(0, 0, 0);
        mColours[9] = RGB(0, 0, 255);
        mColours[10] = RGB(255, 0, 0);
        mColours[11] = RGB(255, 0, 255);
        mColours[12] = RGB(0, 255, 0);
        mColours[13] = RGB(0, 255, 255);
        mColours[14] = RGB(255, 255, 0);
        mColours[15] = RGB(255, 255, 255);
}

std::string spec48BasicLister::GetKeywords()
{
        // Graphics encoding based upon the scheme used by ZXText2P: http://freestuff.grok.co.uk/zxtext2p/index.html
        // UDGs are prefixed with '\_' followed by the letter. Embedded control codes are prefixed by '\' followed by
        // the value of control code as a two digit hexadecimal number.
        std::string keywords = "\\00¦\\01¦\\02¦\\03¦\\04¦\\05¦\\06¦\\07¦\\08¦\\09¦\\0A¦\\0B¦\\0C¦\\0D¦\\0E¦\\0F¦\\10¦\\11¦\\12¦\\13¦\\14¦\\15¦\\16¦\\17¦\\18¦\\19¦\\1A¦\\1B¦\\1C¦\\1D¦\\1E¦\\1F¦"
                               " ¦!¦\"¦#¦$¦%¦&¦'¦(¦)¦*¦+¦,¦-¦.¦/¦0¦1¦2¦3¦4¦5¦6¦7¦8¦9¦:¦;¦<¦=¦>¦?¦"
                               "@¦A¦B¦C¦D¦E¦F¦G¦H¦I¦J¦K¦L¦M¦N¦O¦P¦Q¦R¦S¦T¦U¦V¦W¦X¦Y¦Z¦[¦\\¦]¦^¦_¦"
                               "£¦a¦b¦c¦d¦e¦f¦g¦h¦i¦j¦k¦l¦m¦n¦o¦p¦q¦r¦s¦t¦u¦v¦w¦x¦y¦z¦{¦|¦}¦~¦\xA9¦"
                               "\\  ¦\\ '¦\\' ¦\\''¦\\ .¦\\ :¦\\'.¦\\':¦\\. ¦\\.'¦\\: ¦\\:'¦\\..¦\\.:¦\\:.¦\\::¦\\_A¦\\_B¦\\_C¦\\_D¦\\_E¦\\_F¦\\_G¦\\_H¦\\_I¦\\_J¦\\_K¦\\_L¦\\_M¦\\_N¦\\_O¦\\_P¦\\_Q¦\\_R¦\\_S¦\\_T¦\\_U¦"
                               "RND¦INKEY$¦PI¦FN ¦POINT ¦SCREEN$ ¦ATTR ¦AT ¦TAB ¦VAL$ ¦CODE ¦VAL ¦LEN ¦SIN ¦COS ¦TAN ¦ASN ¦ACS ¦ATN ¦LN ¦EXP ¦INT ¦"
                               "SQR ¦SGN ¦ABS ¦PEEK ¦IN ¦USR ¦STR$ ¦CHR$ ¦NOT ¦BIN ¦ OR ¦ AND ¦<=¦>=¦<>¦ LINE ¦ THEN ¦ TO ¦ STEP ¦ DEF FN ¦ CAT ¦"
                               " FORMAT ¦ MOVE ¦ ERASE ¦ OPEN #¦ CLOSE #¦ MERGE ¦ VERIFY ¦ BEEP ¦ CIRCLE ¦ INK ¦ PAPER ¦ FLASH ¦ BRIGHT ¦ INVERSE ¦ OVER ¦ OUT ¦"
                               " LPRINT ¦ LLIST ¦ STOP ¦ READ ¦ DATA ¦ RESTORE ¦ NEW ¦ BORDER ¦ CONTINUE ¦ DIM ¦ REM ¦ FOR ¦ GO TO ¦ GO SUB ¦ INPUT ¦ LOAD ¦"
                               " LIST ¦ LET ¦ PAUSE ¦ NEXT ¦ POKE ¦ PRINT ¦ PLOT ¦ RUN ¦ SAVE ¦ RANDOMIZE ¦ IF ¦ CLS ¦ DRAW ¦ CLEAR ¦ RETURN ¦ COPY ¦";

        return keywords;
}

void spec48BasicLister::InitialiseColours()
{
        mInkValue = 0;
        mPaperValue = 7;
        mInverseValue = 0;
        mBrightValue = 0;
}

int spec48BasicLister::GetProgramStartAddress()
{
        return ReadByte(23635) + (ReadByte(23636) << 8);
}

int spec48BasicLister::GetProgramEndAddress()
{
        const int vars = 23627;
        return ReadByte(vars) + (ReadByte(vars + 1) << 8);
}

unsigned char spec48BasicLister::GetFloatingPointNumberCode()
{
        return Number;
}

unsigned char spec48BasicLister::GetLineEndingCode()
{
        return Return;
}

bool spec48BasicLister::SupportEmbeddedControlCodes()
{
        return true;
}

bool spec48BasicLister::IsEmbeddedControlCode(unsigned char code)
{
        bool controlCode = false;

        switch (code)
        {
                case Ink:
                case Paper:
                case Flash:
                case Bright:
                case Inverse:
                case Over:
                case At:
                case Tab:
                        controlCode = true;
                        break;
        }

        return controlCode;
}

int spec48BasicLister::GetEmbeddedControlCodeSize(unsigned char code)
{
        return (code == At) ? 2 : 1;
}

void spec48BasicLister::ProcessControlCode(unsigned char code, unsigned char arg1, unsigned char arg2)
{
        switch (code)
        {
                case Ink:
                        if (arg1 <= 7)
                        {
                                mInkValue = arg1;
                        }
                        break;

                case Paper:
                        if (arg1 <= 7)
                        {
                                mPaperValue = arg1;
                        }
                        break;

                case Bright:
                        mBrightValue = arg1;
                        break;

                case Inverse:
                        mInverseValue = arg1;
                        break;
        }
}

COLORREF spec48BasicLister::GetInkColour()
{
        int bright = (mBrightValue != 0) ? 8 : 0;
        COLORREF ink = mInverseValue ? mColours[mPaperValue + bright] : mColours[mInkValue + bright];
        if (mPaperValue==mInkValue) ink=mColours[(~mPaperValue)&7 + bright];
        return ink;
}

COLORREF spec48BasicLister::GetPaperColour()
{
        int bright = (mBrightValue != 0) ? 8 : 0;
        COLORREF paper = mInverseValue ? mColours[mInkValue + bright] : mColours[mPaperValue + bright];
        return paper;
}

bool spec48BasicLister::CustomColoursSupported()
{
        return true;
}

COLORREF spec48BasicLister::GetDefaultPaperColour()
{
        return mColours[7];
}

AnsiString spec48BasicLister::GetMachineName()
{
        return "Spectrum";
}

AnsiString spec48BasicLister::GetBasicFileExtension()
{
        return "b82";
}

bool spec48BasicLister::RequiresInitialSpace()
{
        return false;
}

bool spec48BasicLister::BasicVariablesSupported()
{
        return true;
}

int spec48BasicLister::GetVariablesStartAddress()
{
        const int vars = 0x5C4B;
        return ReadByte(vars) + (ReadByte(vars + 1) << 8);
}

int spec48BasicLister::TranslateVariableType(unsigned char code)
{
        if (code >= 0x61 && code <= 0x7A)
                return SingleNumber;
        else if (code >= 0xA1 && code <= 0xBA)
                return MultiNumber;
        else if (code >= 0x81 && code <= 0x9A)
                return NumberArray;
        else if (code >= 0xE1 && code <= 0xFA)
                return ForNextControl;
        else if (code >= 0x41 && code <= 0x5A)
                return SimpleString;
        else if (code >= 0xC1 && code <= 0xDA)
                return CharacterArray;
        else
                return UnsupportedType;
}

unsigned char spec48BasicLister::ConvertVariableNameCode(unsigned char code, bool first)
{
        unsigned char retVal = (unsigned char)(code & 0x7F);
        if (first) retVal |= 0x40;
        return retVal;
}

int spec48BasicLister::GetForVariableLength()
{
        return 18;
}

bool spec48BasicLister::DetectLastLetter(unsigned char letter)
{
        return (letter & 0x80);
}

double spec48BasicLister::ConvertZXNumberToDouble(int* address)
{
        unsigned char exponent = ReadByte((*address)++);
        if (exponent!=0)
        {
                unsigned char mantissa0 = ReadByte((*address)++);
                unsigned char mantissa1 = ReadByte((*address)++);
                unsigned char mantissa2 = ReadByte((*address)++);
                unsigned char mantissa3 = ReadByte((*address)++);
                if (exponent + mantissa0 + mantissa1 + mantissa2 + mantissa3 == 0) return 0;
                double signMultiplier = ((mantissa0 & 0x80) != 0) ? -1.0 : 1.0;
                double mantissaSum = (mantissa0 | 0x80)/256.0 + mantissa1/65536.0 + mantissa2/16777216.0 + mantissa3/4294967296.0;
                double absRawResult = pow(2.0,exponent-128) * mantissaSum;
                int sigfigs = 8-(1+int(log10(absRawResult)));
                return int(pow(10.0,sigfigs) * absRawResult + 0.5)/pow(10.0,sigfigs) * signMultiplier;
        }
        else
        {
                int sign = ReadByte((*address)++) ? -1 : 1;
                int word = ReadByte((*address)++) + 256*ReadByte((*address)++);
                (*address)++;
                return sign * word;
        }
}

// BASIC debugging. The interpreter starts running a line at NEXT-LINE,
// where PPC already holds its number. Each ROM with a BASIC interpreter
// (48 BASIC, and the 128 BASIC of the 128, +2 and +3 editors) has it right
// after LINE-USE, so it is found by this code in the ROM banks: every
// version and language of the ROMs works. The bank and the address of each
// are kept, as the same address in another ROM is other code.

bool spec48BasicLister::BasicDebugSupported()
{
        return true;
}

int spec48BasicLister::GetNextBasicLineNumber()
{
        const int ppc = 0x5C45;
        return ReadByte(ppc) + (ReadByte(ppc + 1) << 8);
}

// The ROM banks with the code of the machine (the 48K has one)
int spec48BasicLister::GetRomBanks()
{
        return 1;
}

void spec48BasicLister::FindLineStarts()
{
        static const unsigned char code[] =
        {
                0xFE, 0x01, 0xCE, 0x00,         // LINE-USE: CP 1, ADC A,0
                0x56, 0x23, 0x5E,               // LD D,(HL), INC HL, LD E,(HL)
                0xED, 0x53, 0x45, 0x5C,         // LD (PPC),DE
                0x23, 0x5E, 0x23, 0x56,         // INC HL, LD E,(HL), INC HL, LD D,(HL)
                0xEB, 0x19, 0x23,               // EX DE,HL, ADD HL,DE, INC HL
                0x22, 0x55, 0x5C,               // NEXT-LINE: LD (NXTLIN),HL
                0xEB, 0x22, 0x5D, 0x5C          // EX DE,HL, LD (CH_ADD),HL
        };
        const int nextLine = 18;
        int bank, address;

        mLineStarts = 0;

        for (bank = 0; bank < GetRomBanks() && mLineStarts < MaxLineStarts; bank++)
        {
                const unsigned char *rom = SpectrumMem + (bank << 14);

                for (address = 0; address + (int)sizeof(code) <= 16384; address++)
                {
                        if (!memcmp(rom + address, code, sizeof(code)))
                        {
                                mLineStartBank[mLineStarts] = bank;
                                mLineStartAddress[mLineStarts] = address + nextLine;
                                mLineStarts++;
                                break;
                        }
                }
        }

        mLineStartsFound = true;
}

bool spec48BasicLister::IsBasicLineExecuteStart(int pc)
{
        int i;

        if (pc >= 0x4000) return false;
        if (!mLineStartsFound) FindLineStarts();

        for (i = 0; i < mLineStarts; i++)
        {
                if (pc == mLineStartAddress[i] && SPECBlk[0] == mLineStartBank[i]) return true;
        }

        return false;
}

int spec48BasicLister::GetBasicLineExecuteStartAddress()
{
        if (!mLineStartsFound) FindLineStarts();
        return mLineStarts ? mLineStartAddress[0] : 65535;
}
