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
#include <map>
#include <vector>
#include <string>
#pragma hdrstop

#include "Keyboard_.h"
#include "KeyboardFunctions_.h"
#include "Keypad_.h"
#include "Language.h"
#include "main_.h"
#include "zx81config.h"
#include "zlib.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TKeyboard *Keyboard;

//---------------------------------------------------------------------------
__fastcall TKeyboard::TKeyboard(TComponent* Owner)
        : TForm(Owner)
{
        // The size of the picture is changed by double clicking it: 100%
        // (the first size), 125%, 150%, 175%, 200%, 75% and again 100%; it
        // is scaled smoothly. The window can be resized too, but only to
        // these sizes, and only to those that fit the screen. The pictures
        // of the machines stay as they are, under the one shown
        Size = DefaultSize;
        AutoSize = false;
        BorderStyle = bsSizeToolWin;
        ClientWidth = PictureWidthAt(DefaultSize);
        ClientHeight = PictureHeightAt(DefaultSize);

        Display = new TImage(this);
        Display->Parent = this;
        Display->Align = alClient;
        Display->OnDblClick = KeyboardDblClick;
        Display->BringToFront();

        OnCanResize = CanResizeWindow;
        OnResize = ResizeWindow;

        // The title shows the size: put together here, not translated with
        // the form
        LanguageKeepText(this);
        ShowSize();
}

// The sizes, in percent. 100% is 1.75 times the size of the original
// pictures (505 x 223): 884 x 390, the first size. The pictures drawn again
// are held at 200%, the largest size (100% is half of it, so it is scaled
// down cleanly); the others at their original size
static const int Sizes[] = { 75, 100, 125, 150, 175, 200 };
#define SIZES (sizeof(Sizes) / sizeof(Sizes[0]))
const int TKeyboard::DefaultSize = 1;

// The width or height of a picture at a size, from its original one
static int ScaledSize(int original, int size)
{
        int full = (original * 7 + 2) / 4;

        return (full * Sizes[size] + 50) / 100;
}

int TKeyboard::PictureWidthAt(int size)
{
        return ScaledSize(PictureWidth, size);
}

int TKeyboard::PictureHeightAt(int size)
{
        return ScaledSize(PictureHeight, size);
}

void TKeyboard::ShowSize(void)
{
        Caption = TrF("Keyboard \226 {0}%", AnsiString(Sizes[Size]));
}

// After a change of language: the title is put together again
void TKeyboard::LanguageChanged(void)
{
        ShowSize();
}

//---------------------------------------------------------------------------
// The pictures drawn again are in the resources of the program (KBPics.rc,
// made by Source/KBPics/svg/make.py), as PNG pictures named after the
// TImages they are shown for. A PNG picture is read here: RGB or RGBA (the
// alpha is left out) of 8 bits a sample, or palette colours of 1 - 8 bits,
// not interlaced

static unsigned long BigEndian(const unsigned char *p)
{
        return ((unsigned long)p[0] << 24) | ((unsigned long)p[1] << 16)
                | ((unsigned long)p[2] << 8) | p[3];
}

static int Paeth(int a, int b, int c)
{
        int p = a + b - c;
        int pa = abs(p - a), pb = abs(p - b), pc = abs(p - c);

        if (pa <= pb && pa <= pc) return a;
        return pb <= pc ? b : c;
}

// The picture of a PNG file in memory; NULL if it can't be read
static Graphics::TBitmap *ReadPng(const unsigned char *data, unsigned long size)
{
        static const unsigned char signature[8] = { 137, 'P', 'N', 'G', 13, 10, 26, 10 };
        unsigned char palette[256][3];
        unsigned long width = 0, height = 0, length, pos = 8, stride, x, y;
        int depth = 0, type = -1, interlace = 0, channels, step;
        std::string compressed;
        std::vector<unsigned char> raw;
        uLongf rawSize;
        Graphics::TBitmap *picture;

        if (!data || size < 8 || memcmp(data, signature, 8)) return NULL;
        memset(palette, 0, sizeof(palette));

        // The chunks: the size and kind of the picture, its palette and its
        // compressed data (in one or more parts)
        while (pos + 12 <= size)
        {
                const unsigned char *kind = data + pos + 4, *chunk = data + pos + 8;

                length = BigEndian(data + pos);
                if (length > size - pos - 12) return NULL;

                if (!memcmp(kind, "IHDR", 4) && length >= 13)
                {
                        width = BigEndian(chunk);
                        height = BigEndian(chunk + 4);
                        depth = chunk[8];
                        type = chunk[9];
                        interlace = chunk[12];
                }
                else if (!memcmp(kind, "PLTE", 4))
                        memcpy(palette, chunk, length < sizeof(palette) ? length : sizeof(palette));
                else if (!memcmp(kind, "IDAT", 4))
                        compressed.append((const char *)chunk, length);
                else if (!memcmp(kind, "IEND", 4))
                        break;

                pos += length + 12;
        }

        if (interlace || !width || !height || width > 16384 || height > 16384) return NULL;
        if (type == 2 && depth == 8) channels = 3;
        else if (type == 6 && depth == 8) channels = 4;
        else if (type == 3 && (depth == 1 || depth == 2 || depth == 4 || depth == 8)) channels = 1;
        else return NULL;

        // The lines, each with the kind of its filter before it; the
        // filters work on whole pixels, or on bytes when they are smaller
        stride = (width * channels * depth + 7) / 8;
        step = depth == 8 ? channels : 1;
        rawSize = (stride + 1) * height;
        raw.resize(rawSize);
        if (uncompress(&raw[0], &rawSize, (const Bytef *)compressed.data(), compressed.size()) != Z_OK
                || rawSize != (stride + 1) * height)
                return NULL;

        for (y=0; y<height; y++)
        {
                unsigned char *line = &raw[y * (stride + 1)] + 1;
                unsigned char *above = y ? line - (stride + 1) : NULL;
                int filter = line[-1];

                for (x=0; x<stride; x++)
                {
                        int a = x >= (unsigned long)step ? line[x - step] : 0;
                        int b = above ? above[x] : 0;
                        int c = above && x >= (unsigned long)step ? above[x - step] : 0;

                        switch (filter)
                        {
                        case 0: break;
                        case 1: line[x] = (unsigned char)(line[x] + a); break;
                        case 2: line[x] = (unsigned char)(line[x] + b); break;
                        case 3: line[x] = (unsigned char)(line[x] + (a + b) / 2); break;
                        case 4: line[x] = (unsigned char)(line[x] + Paeth(a, b, c)); break;
                        default: return NULL;
                        }
                }
        }

        picture = new Graphics::TBitmap();
        picture->PixelFormat = pf24bit;
        picture->Width = width;
        picture->Height = height;

        for (y=0; y<height; y++)
        {
                const unsigned char *line = &raw[y * (stride + 1)] + 1;
                unsigned char *out = (unsigned char *)picture->ScanLine[y];

                for (x=0; x<width; x++, out+=3)
                {
                        const unsigned char *rgb;

                        if (type == 3)
                        {
                                int bit = x * depth;
                                int index = (line[bit / 8] >> (8 - depth - bit % 8)) & ((1 << depth) - 1);
                                rgb = palette[index];
                        }
                        else
                                rgb = line + x * channels;

                        out[0] = rgb[2];
                        out[1] = rgb[1];
                        out[2] = rgb[0];
                }
        }

        return picture;
}

// The picture drawn again of a TImage, from the resources; NULL if it has
// none
static Graphics::TBitmap *ResourcePicture(TImage *image)
{
        HRSRC found = FindResource(HInstance, image->Name.c_str(), RT_RCDATA);
        HGLOBAL loaded;

        if (!found || (loaded = LoadResource(HInstance, found)) == NULL) return NULL;

        return ReadPng((const unsigned char *)LockResource(loaded), SizeofResource(HInstance, found));
}

// The picture held for a TImage: drawn again (at 200%), or its own picture
// in the form (at the original size). A new bitmap, for the caller to free
static Graphics::TBitmap *PictureOf(TImage *image)
{
        Graphics::TBitmap *picture = ResourcePicture(image);

        if (!picture)
        {
                picture = new Graphics::TBitmap();
                picture->Assign(image->Picture->Bitmap);
        }

        return picture;
}

// A picture scaled smoothly to a size: with halftone stretching (much
// smoother than the stretching of TImage), or, to exactly half its size,
// each pixel the average of four, done here: the pictures drawn again are
// held at 200%, so 100% is as sharp as drawn, whatever the halftone
// stretching of Windows does. A new bitmap, for the caller to free
static Graphics::TBitmap *ScaledPicture(Graphics::TBitmap *source, int width, int height)
{
        Graphics::TBitmap *picture = new Graphics::TBitmap();
        int x, y;

        picture->PixelFormat = pf24bit;
        picture->Width = width;
        picture->Height = height;

        if (source->PixelFormat == pf24bit && source->Width == width * 2 && source->Height == height * 2)
        {
                for (y=0; y<height; y++)
                {
                        const unsigned char *a = (const unsigned char *)source->ScanLine[2 * y];
                        const unsigned char *b = (const unsigned char *)source->ScanLine[2 * y + 1];
                        unsigned char *out = (unsigned char *)picture->ScanLine[y];

                        for (x=0; x<width * 3; x++)
                        {
                                int i = x / 3 * 6 + x % 3;
                                out[x] = (unsigned char)((a[i] + a[i + 3] + b[i] + b[i + 3] + 2) / 4);
                        }
                }
        }
        else
        {
                SetStretchBltMode(picture->Canvas->Handle, HALFTONE);
                SetBrushOrgEx(picture->Canvas->Handle, 0, 0, NULL);
                StretchBlt(picture->Canvas->Handle, 0, 0, width, height,
                           source->Canvas->Handle, 0, 0, source->Width, source->Height, SRCCOPY);
        }

        return picture;
}

//---------------------------------------------------------------------------

// The picture of the machine, scaled to the window. Only the picture shown
// is held: it is read again when another machine's is shown
void TKeyboard::UpdateDisplay(void)
{
        static TImage *shown = NULL;
        static Graphics::TBitmap *held = NULL;
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

        if (source != shown)
        {
                delete held;
                held = PictureOf(source);
                shown = source;
        }

        picture = ScaledPicture(held, ClientWidth, ClientHeight);
        Display->Picture->Bitmap = picture;
        delete picture;

        Display->BringToFront();
}

// A picture of the windows beside the keyboard at a size, scaled smoothly
// from its own picture, which may be larger (the keypad drawn again is held
// at 200%). Its normal size (as in the form) is kept in its Tag, its own
// picture in Originals
static std::map<TImage *, Graphics::TBitmap *> Originals;

static void ScaleImage(TImage *image, int size)
{
        Graphics::TBitmap *original, *picture;
        int width, height;

        if (!image) return;

        if (!image->Tag) image->Tag = (image->Width << 16) | image->Height;

        if (Originals.find(image) == Originals.end())
                Originals[image] = PictureOf(image);
        original = Originals[image];

        width = ScaledSize(image->Tag >> 16, size);
        height = ScaledSize(image->Tag & 0xffff, size);

        picture = ScaledPicture(original, width, height);

        image->Stretch = false;
        image->Picture->Bitmap = picture;
        delete picture;

        image->Width = width;
        image->Height = height;
}

// The frame of a window as drawn: from Windows Vista, with the desktop
// composition (DWM), windows have invisible borders (the parts of the
// resizing borders outside the frame seen); false if not known
typedef HRESULT (WINAPI *DwmGetWindowAttributeFunc)(HWND, DWORD, PVOID, DWORD);

static bool VisibleFrame(HWND window, RECT &whole, RECT &seen)
{
        static DwmGetWindowAttributeFunc get = NULL;
        static bool looked = false;
        const DWORD DWMWA_EXTENDED_FRAME_BOUNDS = 9;

        if (!looked)
        {
                HMODULE dwm = LoadLibrary("dwmapi.dll");
                if (dwm) get = (DwmGetWindowAttributeFunc)GetProcAddress(dwm, "DwmGetWindowAttribute");
                looked = true;
        }

        if (!get || !GetWindowRect(window, &whole)) return false;
        if (get(window, DWMWA_EXTENDED_FRAME_BOUNDS, &seen, sizeof(seen)) != S_OK) return false;

        // In other units (the program is scaled by Windows at a larger
        // text size): not usable
        if (seen.left < whole.left || seen.right > whole.right
                || seen.left - whole.left > 20 || whole.right - seen.right > 20)
                return false;

        return true;
}

// Where a window beside the keyboard goes: SideGap from it, as seen
int TKeyboard::SideOffset(TForm *side)
{
        RECT whole, seen;
        int offset = SideGap;

        if (VisibleFrame(Handle, whole, seen)) offset -= whole.right - seen.right;
        if (side && VisibleFrame(side->Handle, whole, seen)) offset -= seen.left - whole.left;

        return offset;
}

// The largest size, not larger than this one, at which the window and the
// one beside it (the ZX80 functions or the keypad) fit the work area of its
// monitor; the smallest if none does
int TKeyboard::FittingSize(int size)
{
        MONITORINFO info;
        HMONITOR monitor;
        RECT area;
        TForm *side = NULL;
        TImage *sideImage = NULL;
        int offset = 0, normal, width, height, sideHeight;
        int frameWidth = Width - ClientWidth, frameHeight = Height - ClientHeight;

        monitor = MonitorFromWindow(Handle, MONITOR_DEFAULTTONEAREST);
        info.cbSize = sizeof(info);
        if (!monitor || !GetMonitorInfo(monitor, &info))
                SystemParametersInfo(SPI_GETWORKAREA, 0, &info.rcWork, 0);
        area = info.rcWork;

        if (KeyboardFunctions && KeyboardFunctions->Visible)
        {
                side = KeyboardFunctions;
                sideImage = KeyboardFunctions->zx80IntegralFunctions;
                offset = SideOffset(KeyboardFunctions);
        }
        else if (Keypad128 && Keypad128->Visible)
        {
                side = Keypad128;
                sideImage = Keypad128->KeypadUk->Visible ? Keypad128->KeypadUk : Keypad128->KeypadSpanish;
                offset = SideOffset(Keypad128);
        }

        for (; size > 0; size--)
        {
                width = PictureWidthAt(size) + frameWidth;
                height = PictureHeightAt(size) + frameHeight;

                if (side)
                {
                        normal = sideImage->Tag ? sideImage->Tag : (sideImage->Width << 16) | sideImage->Height;
                        width += offset + ScaledSize(normal >> 16, size) + side->Width - side->ClientWidth;
                        sideHeight = ScaledSize(normal & 0xffff, size) + side->Height - side->ClientHeight;
                        if (sideHeight > height) height = sideHeight;
                }

                if (width <= area.right - area.left && height <= area.bottom - area.top) break;
        }

        return size;
}

void TKeyboard::SetSize(int size)
{
        if (size < 0 || size >= (int)SIZES) size = DefaultSize;
        size = FittingSize(size);
        Size = size;

        ClientWidth = PictureWidthAt(size);
        ClientHeight = PictureHeightAt(size);
        UpdateDisplay();
        ShowSize();

        // The windows beside the keyboard (ZX80 functions, Spectrum 128
        // keypad) with it, following its right edge
        if (KeyboardFunctions)
        {
                ScaleImage(KeyboardFunctions->zx80IntegralFunctions, size);
                KeyboardFunctions->Left = Left + Width + SideOffset(KeyboardFunctions);
                KeyboardFunctions->Top = Top;
        }
        if (Keypad128)
        {
                ScaleImage(Keypad128->KeypadUk, size);
                ScaleImage(Keypad128->KeypadSpanish, size);
                Keypad128->Left = Left + Width + SideOffset(Keypad128);
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
        int percent, i;

        Top = ini->ReadInteger("KBMAP","Top",Top);
        Left = ini->ReadInteger("KBMAP","Left",Left);

        // The size, in percent; a size saved before in other units (Size,
        // from 1x) is not taken over: 100%. The side windows don't exist
        // yet: their size follows in KbChange
        percent = ini->ReadInteger("KBMAP", "Percent", Sizes[DefaultSize]);
        Size = DefaultSize;
        for (i=0; i<(int)SIZES; i++)
        {
                if (Sizes[i] == percent) Size = i;
        }
        ClientWidth = PictureWidthAt(Size);
        ClientHeight = PictureHeightAt(Size);
        ShowSize();
}

void TKeyboard::SaveSettings(TIniFile *ini)
{
        ini->WriteInteger("KBMAP","Top",Top);
        ini->WriteInteger("KBMAP","Left",Left);
        ini->WriteInteger("KBMAP","Percent",Sizes[Size]);
        ini->DeleteKey("KBMAP","Size");
        ini->DeleteKey("KBMAP","Zoom");
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

        KeyboardFunctions->Left = Keyboard->Left + Keyboard->Width + Keyboard->SideOffset(KeyboardFunctions);
        KeyboardFunctions->Top = Keyboard->Top;

        Keypad128->Left = Keyboard->Left + Keyboard->Width + Keyboard->SideOffset(Keypad128);
        Keypad128->Top = Keyboard->Top;

        SetSize(Size);
}

// Resizing the window: it jumps to the size nearest to the one dragged to
// (the side dragged decides)
void __fastcall TKeyboard::CanResizeWindow(TObject *Sender, int &NewWidth,
                                           int &NewHeight, bool &Resize)
{
        SnapSize(NewWidth, NewHeight, NewWidth == Width && NewHeight != Height);
}

// Dragging the frame: the size jumps here, where the side dragged is known,
// so the opposite side stays where it is (OnCanResize can only change the
// size, so dragging the left or top side moved the right or bottom one)
void __fastcall TKeyboard::WndProc(Messages::TMessage &Message)
{
        if (Message.Msg == WM_SIZING)
        {
                RECT *r = (RECT *)Message.LParam;
                int edge = Message.WParam;
                int w = r->right - r->left, h = r->bottom - r->top;

                SnapSize(w, h, edge == WMSZ_TOP || edge == WMSZ_BOTTOM);

                if (edge == WMSZ_LEFT || edge == WMSZ_TOPLEFT || edge == WMSZ_BOTTOMLEFT)
                        r->left = r->right - w;
                else
                        r->right = r->left + w;

                if (edge == WMSZ_TOP || edge == WMSZ_TOPLEFT || edge == WMSZ_TOPRIGHT)
                        r->top = r->bottom - h;
                else
                        r->bottom = r->top + h;
        }

        TForm::WndProc(Message);
}

// A window size (frame included) made the nearest picture size, judged by
// the width or the height
void TKeyboard::SnapSize(int &NewWidth, int &NewHeight, bool byHeight)
{
        int frameWidth = Width - ClientWidth;
        int frameHeight = Height - ClientHeight;
        int wanted, size, best = 0, bestDistance = -1, distance, i;
        int largest = FittingSize(SIZES - 1);

        for (i=0; i<=largest; i++)
        {
                if (byHeight)
                {
                        wanted = NewHeight - frameHeight;
                        size = PictureHeightAt(i);
                }
                else
                {
                        wanted = NewWidth - frameWidth;
                        size = PictureWidthAt(i);
                }

                distance = wanted > size ? wanted - size : size - wanted;
                if (bestDistance < 0 || distance < bestDistance)
                {
                        best = i;
                        bestDistance = distance;
                }
        }

        NewWidth = PictureWidthAt(best) + frameWidth;
        NewHeight = PictureHeightAt(best) + frameHeight;
}

// The picture is drawn again only when the size has changed
void __fastcall TKeyboard::ResizeWindow(TObject *Sender)
{
        int i;

        for (i=0; i<(int)SIZES; i++)
        {
                if (ClientWidth == PictureWidthAt(i))
                {
                        if (i != Size) SetSize(i);
                        return;
                }
        }
}

// Double click: the next size; after the largest that fits the screen,
// the smallest
void __fastcall TKeyboard::KeyboardDblClick(TObject *Sender)
{
        int next = Size + 1;

        if (next >= (int)SIZES || FittingSize(next) < next) next = 0;
        SetSize(next);
}

