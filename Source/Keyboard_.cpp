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
#include <string.h>
#pragma hdrstop

#include "Keyboard_.h"
#include "KeyboardFunctions_.h"
#include "Keypad_.h"
#include "main_.h"
#include "zx81config.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TKeyboard *Keyboard;

//---------------------------------------------------------------------------
__fastcall TKeyboard::TKeyboard(TComponent* Owner)
        : TForm(Owner)
{
        TIniFile *ini;

        // The size of the picture is changed by double clicking it: 1x,
        // 1.25x, 1.5x, 1.75x, 2x and again 1x; it is scaled smoothly. The
        // pictures of the machines stay as they are, under the one shown
        Size = 0;
        AutoSize = false;
        ClientWidth = PictureWidth;
        ClientHeight = PictureHeight;

        Display = new TImage(this);
        Display->Parent = this;
        Display->Align = alClient;
        Display->OnDblClick = KeyboardDblClick;
        Display->BringToFront();

        ini = new TIniFile(emulator.inipath);
        LoadSettings(ini);
        delete ini;
}

// The sizes, in quarters of the normal size
static const int Sizes[] = { 4, 5, 6, 7, 8 };
#define SIZES (sizeof(Sizes) / sizeof(Sizes[0]))

// The picture of the machine, scaled to the window with halftone
// stretching (much smoother than the stretching of TImage)
void TKeyboard::UpdateDisplay(void)
{
        TImage *source = NULL;
        Graphics::TBitmap *picture;
        int i;

        if (!Display) return;

        for (i=0; i<ComponentCount; i++)
        {
                TImage *image = dynamic_cast<TImage *>(Components[i]);
                if (image && image != Display && image->Visible)
                {
                        source = image;
                        break;
                }
        }

        if (!source || ClientWidth <= 0 || ClientHeight <= 0) return;

        picture = new Graphics::TBitmap();
        picture->PixelFormat = pf24bit;
        picture->Width = ClientWidth;
        picture->Height = ClientHeight;

        SetStretchBltMode(picture->Canvas->Handle, HALFTONE);
        SetBrushOrgEx(picture->Canvas->Handle, 0, 0, NULL);
        StretchBlt(picture->Canvas->Handle, 0, 0, ClientWidth, ClientHeight,
                   source->Picture->Bitmap->Canvas->Handle, 0, 0,
                   source->Picture->Bitmap->Width,
                   source->Picture->Bitmap->Height, SRCCOPY);

        Display->Picture->Bitmap = picture;
        delete picture;

        Display->BringToFront();
}

// A picture of the windows beside the keyboard at a size; its normal size
// is kept in its Tag
static void ScaleImage(TImage *image, int quarters)
{
        if (!image) return;

        if (!image->Tag) image->Tag = (image->Width << 16) | image->Height;

        image->Width = ((image->Tag >> 16) * quarters + 2) / 4;
        image->Height = ((image->Tag & 0xffff) * quarters + 2) / 4;
}

void TKeyboard::SetSize(int size)
{
        if (size < 0 || size >= (int)SIZES) size = 0;
        Size = size;

        ClientWidth = (PictureWidth * Sizes[size] + 2) / 4;
        ClientHeight = (PictureHeight * Sizes[size] + 2) / 4;
        UpdateDisplay();

        // The windows beside the keyboard (ZX80 functions, Spectrum 128
        // keypad) with it, following its right edge
        if (KeyboardFunctions)
        {
                ScaleImage(KeyboardFunctions->zx80IntegralFunctions, Sizes[size]);
                KeyboardFunctions->Left = Left + Width + FunctionsOffset;
                KeyboardFunctions->Top = Top;
        }
        if (Keypad128)
        {
                ScaleImage(Keypad128->KeypadUk, Sizes[size]);
                ScaleImage(Keypad128->KeypadSpanish, Sizes[size]);
                Keypad128->Left = Left + Width + KeypadOffset;
                Keypad128->Top = Top;
        }
}

void __fastcall TKeyboard::FormClose(TObject *Sender, TCloseAction &Action)
{
        Form1->KeyboardMap1->Checked=false;

        KeyboardFunctions->Visible = false;
        KeyboardFunctions->EnableTimer(false);

        Keypad128->Visible = false;
        Keypad128->EnableTimer(false);
}
//---------------------------------------------------------------------------

void TKeyboard::LoadSettings(TIniFile *ini)
{
        int width;

        Top = ini->ReadInteger("KBMAP","Top",Top);
        Left = ini->ReadInteger("KBMAP","Left",Left);

        // The side windows don't exist yet: their size follows in KbChange
        width = ini->ReadInteger("KBMAP","Size",0);
        if (width < 0 || width >= (int)SIZES) width = 0;
        Size = width;
        ClientWidth = (PictureWidth * Sizes[Size] + 2) / 4;
        ClientHeight = (PictureHeight * Sizes[Size] + 2) / 4;
}

void TKeyboard::SaveSettings(TIniFile *ini)
{
        ini->WriteInteger("KBMAP","Top",Top);
        ini->WriteInteger("KBMAP","Left",Left);
        ini->WriteInteger("KBMAP","Size",Size);
}

void __fastcall TKeyboard::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        Form1->FormKeyDown(Sender, Key, Shift);
}
//---------------------------------------------------------------------------

void __fastcall TKeyboard::FormKeyPress(TObject *Sender, char &Key)
{
        Form1->FormKeyPress(Sender, Key);
}
//---------------------------------------------------------------------------

void __fastcall TKeyboard::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        Form1->FormKeyUp(Sender, Key, Shift);
}
//---------------------------------------------------------------------------
void TKeyboard::KbChange(void)
{
        Keyboard->zx80kb->Visible=false;
        Keyboard->zx80zxpandkb->Visible=false;
        Keyboard->zx81kb->Visible=false;
        Keyboard->zx81zxpandkb->Visible=false;
        Keyboard->ts1000kb->Visible=false;
        Keyboard->ts1000zxpandkb->Visible=false;
        Keyboard->ts1500kb->Visible=false;
        Keyboard->ts1500zxpandkb->Visible=false;
        Keyboard->lambdakb->Visible=false;
        Keyboard->r470kb->Visible=false;
        Keyboard->tk85kb->Visible=false;
        Keyboard->acekb->Visible=false;
        Keyboard->zx97kb->Visible=false;
        Keyboard->spec16kb->Visible=false;
        Keyboard->spec48kb->Visible=false;
        Keyboard->spec48kbNordic->Visible=false;
        Keyboard->spec128kb->Visible=false;
        Keyboard->spec128kbSpanish->Visible=false;
        Keyboard->spec128kbArabic->Visible=false;
        Keyboard->tc2048kb->Visible=false;
        Keyboard->tc2068kbTimeWord->Visible=false;
        Keyboard->ts2068kb->Visible=false;
        Keyboard->specPlus2kb->Visible=false;
        Keyboard->specPlus2kbSpanish->Visible=false;
        Keyboard->specPlus2kbArabic->Visible=false;
        Keyboard->specPlus3kb->Visible=false;
        Keyboard->specPlus3kbSpanish->Visible=false;
        Keyboard->specPlus3kbArabic->Visible=false;

        KeyboardFunctions->Visible=false;
        KeyboardFunctions->EnableTimer(false);

        Keypad128->Visible=false;
        Keypad128->EnableTimer(false);

        if (Form1->KeyboardMap1->Checked) Show();

        switch(emulator.romcrc)
        {
        case CRCACE:
                Keyboard->acekb->Visible=true;
                break;

        case CRCTS1500:
                if (zx81.zxpand)
                        Keyboard->ts1500zxpandkb->Visible=true;
                else
                        Keyboard->ts1500kb->Visible=true;
                break;

        case CRCZX80:
                if (zx81.zxpand) Keyboard->zx80zxpandkb->Visible=true;
                else Keyboard->zx80kb->Visible=true;
                KeyboardFunctions->Visible = Keyboard->Visible;
                KeyboardFunctions->EnableTimer(KeyboardFunctions->Visible);
                break;

        case CRCLAMBDA:
                Keyboard->lambdakb->Visible=true;
                break;

        case CRCTK85:
                Keyboard->tk85kb->Visible=true;
                break;

        case CRCZX97LE:
                Keyboard->zx97kb->Visible=true;
                break;

        case CRCR470:
                Keyboard->r470kb->Visible=true;
                break;

        case CRCZX81_ED1:
        case CRCZX81_ED2:
        case CRCZX81_ED3:
                if (emulator.machine == MACHINETS1000)
                        if (zx81.zxpand)
                                Keyboard->ts1000zxpandkb->Visible=true;
                        else
                                Keyboard->ts1000kb->Visible=true;
                else if (emulator.machine == MACHINETS1500)
                        if (zx81.zxpand)
                                Keyboard->ts1500zxpandkb->Visible=true;
                        else
                                Keyboard->ts1500kb->Visible=true;
                else
                        if (zx81.zxpand)
                                Keyboard->zx81zxpandkb->Visible=true;
                        else
                                Keyboard->zx81kb->Visible=true;
                break;

        case CRCH4TH:
        case CRCTREE4TH:
                Keyboard->zx81kb->Visible=true;
                break;

        default:
                switch(emulator.machine)
                {
                case MACHINEZX80:
                        if (zx81.zxpand) Keyboard->zx80zxpandkb->Visible=true;
                        else Keyboard->zx80kb->Visible=true;
                        KeyboardFunctions->Visible = Keyboard->Visible;
                        KeyboardFunctions->EnableTimer(KeyboardFunctions->Visible);
                        break;
                case MACHINEZX81:
                        if (machine.NTSC && zx81.zxpand) Keyboard->ts1000zxpandkb->Visible=true;
                        else if (machine.NTSC) Keyboard->ts1000kb->Visible=true;
                        else if (zx81.zxpand) Keyboard->zx81zxpandkb->Visible=true;
                        else Keyboard->zx81kb->Visible=true;
                        break;
                case MACHINEACE:
                        Keyboard->acekb->Visible=true;
                        break;
                case MACHINETS1000:
                        Keyboard->ts1000kb->Visible=true;
                        break;
                case MACHINETS1500:
                        Keyboard->ts1500kb->Visible=true;
                        break;
                case MACHINER470:
                        Keyboard->r470kb->Visible=true;
                        break;
                case MACHINETK85:
                        Keyboard->tk85kb->Visible=true;
                        break;
                case MACHINELAMBDA:
                        Keyboard->lambdakb->Visible=true;
                        break;
                case MACHINESPECTRUM:
                        switch (spectrum.model)
                        {
                        case SPECCY128:
                                if (emulator.romcrc == CRCARABIC48V1 || emulator.romcrc == CRCARABIC48V2 || emulator.romcrc == CRCARABIC48V31)
                                {
                                        Keyboard->spec128kbArabic->Visible=true;
                                }
                                else if (emulator.romcrc == CRCSPANISH128)
                                {
                                        Keyboard->spec128kbSpanish->Visible=true;
                                        Keypad128->SelectSpanishLayout();
                                        Keypad128->Visible = Keyboard->Visible && spectrum.spectrum128Keypad;
                                        Keypad128->EnableTimer(Keypad128->Visible);
                                }
                                else
                                {
                                        Keyboard->spec128kb->Visible=true;
                                        Keypad128->SelectUKLayout();
                                        Keypad128->Visible = Keyboard->Visible && spectrum.spectrum128Keypad;
                                        Keypad128->EnableTimer(Keypad128->Visible);
                                }
                                break;
                        case SPECCYTC2048:
                                Keyboard->tc2048kb->Visible=true;
                                break;
                        case SPECCYTC2068:
                        case SPECCYTS2068:
                                if (strstr(emulator.romcartridgefilepath, "TimeWord"))
                                {
                                        Keyboard->tc2068kbTimeWord->Visible=true;
                                }
                                else
                                {
                                        Keyboard->ts2068kb->Visible=true;
                                }

                                break;
                        case SPECCYPLUS:
                                if (emulator.romcrc == CRCARABIC48V1 || emulator.romcrc == CRCARABIC48V2 || emulator.romcrc == CRCARABIC48V31)
                                        Keyboard->spec128kbArabic->Visible=true;
                                else if (emulator.romcrc == CRCSPANISH48)
                                        Keyboard->spec128kbSpanish->Visible=true;
                                else
                                        Keyboard->spec128kb->Visible=true;
                                break;
                        case SPECCY48:
                                if (emulator.romcrc == CRCNORDIC48)
                                        Keyboard->spec48kbNordic->Visible=true;
                                else
                                        Keyboard->spec48kb->Visible=true;
                                break;
                        case SPECCY16:
                                Keyboard->spec16kb->Visible=true;
                                break;
                        case SPECCYPLUS2:
                                if (emulator.romcrc == CRCARABIC48V1 || emulator.romcrc == CRCARABIC48V2 || emulator.romcrc == CRCARABIC48V31)
                                {
                                        Keyboard->specPlus2kbArabic->Visible=true;
                                }
                                else if (emulator.romcrc == CRCSPANISHPLUS2)
                                {
                                        Keyboard->specPlus2kbSpanish->Visible=true;
                                        Keypad128->SelectSpanishLayout();
                                        Keypad128->Visible = Keyboard->Visible && spectrum.spectrum128Keypad;
                                        Keypad128->EnableTimer(Keypad128->Visible);
                                }
                                else
                                {
                                        Keyboard->specPlus2kb->Visible=true;
                                        Keypad128->SelectUKLayout();
                                        Keypad128->Visible = Keyboard->Visible && spectrum.spectrum128Keypad;
                                        Keypad128->EnableTimer(Keypad128->Visible);
                                }
                                break;
                        case SPECCYPLUS2A:
                        case SPECCYPLUS3:
                                if (emulator.romcrc == CRCARABIC48V1 || emulator.romcrc == CRCARABIC48V2 || emulator.romcrc == CRCARABIC48V31 || emulator.romcrc == CRCARABICPLUS3)
                                        Keyboard->specPlus3kbArabic->Visible=true;
                                else if (emulator.romcrc == CRCSPANISHPLUS3V40 || emulator.romcrc == CRCSPANISHPLUS3V41)
                                        Keyboard->specPlus3kbSpanish->Visible=true;
                                else
                                        Keyboard->specPlus3kb->Visible=true;
                                break;
                        default:
                                Keyboard->spec48kb->Visible=true;
                                break;
                        }
                        break;
                default:
                        Keyboard->zx81kb->Visible=true;
                        break;
                }
        }

        KeyboardFunctions->Left = Keyboard->Left + Keyboard->Width + Keyboard->FunctionsOffset;
        KeyboardFunctions->Top = Keyboard->Top;

        Keypad128->Left = Keyboard->Left + Keyboard->Width + Keyboard->KeypadOffset;
        Keypad128->Top = Keyboard->Top;

        SetSize(Size);
}

// Double click: the next size
void __fastcall TKeyboard::KeyboardDblClick(TObject *Sender)
{
        SetSize((Size + 1) % SIZES);
}

