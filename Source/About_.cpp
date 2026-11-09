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

#include "About_.h"
#include "zx81config.h"
#include "Language.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAbout *About;

#define MaxWidth(label,width) if((label->Width + label->Left) > width) width=label->Width + label->Left

// The text below the link: paragraphs made of the English lines of the old
// labels (each translated), wrapped to the width of the window and justified
// (the last line of a paragraph left aligned). The old labels had their own
// line breaks, which the longer Hungarian lines did not fit.
static const char *const Paragraphs[][5] =
{
        { "Releases up to 1.0a were created by Michael D Wynne, with all",
          "subsequent releases created by Charlie Robson and Paul Farrow.", NULL },
        { "This program is released under the GNU GPL. Please see the",
          "attached readme.txt file that should be included with this distribution.", NULL },
        { "Z80 core and DirectSound code taken from fuse, the free UNIX",
          "Spectrum emulator \xA9 2003 Philip Kendall.", NULL },
        { "AY8910 Code taken from Z81 \xA9 1995-2001 Russell Marks.", NULL },
        { "Many thanks to Wilf Rigter for providing essential technical",
          "information on the internal workings of the ZX81 itself and also ",
          "various add-ons including the Memotech and G007 Hi-res cards.",
          "G007 information also provided by Kryton.", NULL },
        { "dk'tronics graphics ROM dump and several bug reports provided",
          "by Simon Holdsworth.  ", NULL },
        { "lib765 and libdsk \xA9 2000-2005 John Elliot.", NULL },
        { "Speech synthesis code from SP0256_CTS256A-AL2.", NULL },
        { "Icon designed by Paul E Collins.", NULL },
};
#define PARAGRAPHS (sizeof(Paragraphs) / sizeof(Paragraphs[0]))
#define PARAGRAPH_GAP   8       // pixels between paragraphs
#define LINE_GAP        3       // and between lines

static AnsiString ParagraphText(int i)
{
        AnsiString text;
        int j;

        for (j=0; Paragraphs[i][j]; j++)
        {
                if (j) text += " ";
                text += Tr(Paragraphs[i][j]).Trim();
        }

        return text;
}

//---------------------------------------------------------------------------

static AnsiString ExtractPart(AnsiString versionNumber, int& s, int&e)
{
        AnsiString part;

        while (e <= versionNumber.Length())
        {
                if (versionNumber[e] == '.')
                {
                        part = versionNumber.SubString(s, e - s);
                        s = e + 1;
                        e += 2;
                        break;
                }

                e++;
        }

        return part;
}

bool GetVersionNumber(int& versionNumberMajor, int& versionNumberMinor, int& versionNumberPart3, int& versionNumberPart4)
{
        versionNumberMajor = 0;
        versionNumberMinor = 0;
        versionNumberPart3 = 0;
        versionNumberPart4 = 0;
        
        AnsiString versionNumber = "";

        AnsiString fileName = Application->ExeName;

	// Get the size of the version information buffer
	DWORD dwHandle = 0;
	DWORD dwLength = GetFileVersionInfoSize(fileName.c_str(), &dwHandle);
	if (dwLength < 1)
        {
		return false;
	}

	// Allocate space for the version information buffer
	LPVOID lpvMem;
	HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, dwLength);
	if (hMem == NULL)
        {
		return false;
	}
        
	lpvMem = GlobalLock(hMem);
	if (lpvMem == NULL)
        {
		GlobalUnlock(hMem);
		GlobalFree(hMem);
		return false;
	}

	// Get the version information block to the buffer
	BOOL fRet = GetFileVersionInfo(	fileName.c_str(), dwHandle, dwLength, (LPVOID)lpvMem);
	if (!fRet)
        {
		GlobalUnlock(hMem);
		GlobalFree(hMem);
		return false;
	}

        // Get the root block
        VS_FIXEDFILEINFO fileInfo;
	LPVOID lpInfo;
	UINT cch;
	if (VerQueryValue(lpvMem, "\\", &lpInfo, &cch))
        {
                ZeroMemory((void*)(&fileInfo), sizeof(fileInfo));
                CopyMemory((void*)&fileInfo, (const void*)lpInfo, sizeof(fileInfo));
        }
	else
        {
		GlobalUnlock(hMem);
		GlobalFree(hMem);
		return false;
	}

	// Get translation information
        AnsiString langID;
        AnsiString charset;
        if (VerQueryValue(lpvMem, TEXT("\\VarFileInfo\\Translation"), &lpInfo, &cch))
        {
    	        WORD wLangID = ((WORD*)lpInfo)[0];
                WORD wCharsetID = ((WORD*)lpInfo)[1];
                langID = langID.IntToHex((int)wLangID, 4);
                charset = charset.IntToHex((int)wCharsetID, 4);
   	}
  	else
        {
  		GlobalUnlock(hMem);
  		GlobalFree(hMem);
  		return false;
  	}

	// Fetch the version information
	TCHAR key[80];

	lstrcpy(key, TEXT("\\StringFileInfo\\"));
   	AnsiString versionInfoLangID(langID + charset);
        lstrcat(key, versionInfoLangID.c_str());
	lstrcat(key, "\\");
	lstrcat(key, TEXT("FileVersion"));

	if (VerQueryValue(lpvMem, key, &lpInfo, &cch))
        {
                versionNumber = (char*)lpInfo;
                versionNumber += ".";
        }
        else
        {
  		GlobalUnlock(hMem);
  		GlobalFree(hMem);
  		return false;
  	}
        
	GlobalUnlock(hMem);
	GlobalFree(hMem);

        // Extract the version number parts

        int s = 1;
        int e = 2;
        versionNumberMajor = StrToInt(ExtractPart(versionNumber, s, e));
        versionNumberMinor = StrToInt(ExtractPart(versionNumber, s, e));
        versionNumberPart3 = StrToInt(ExtractPart(versionNumber, s, e));
        versionNumberPart4 = StrToInt(ExtractPart(versionNumber, s, e));

        return true;
}
//---------------------------------------------------------------------------
__fastcall TAbout::TAbout(TComponent* Owner)
        : TForm(Owner)
{
        int w=0;

        Left = (Screen->Width - Width) /2;
        Top = (Screen->Height - Height) /2;

        int versionNumberMajor;
        int versionNumberMinor;
        int versionNumberPart3;
        int versionNumberPart4;
        bool versionFound = GetVersionNumber(versionNumberMajor, versionNumberMinor, versionNumberPart3, versionNumberPart4);
        if (versionFound)
        {
                AnsiString version;

                version = AnsiString(versionNumberMajor) + "." + AnsiString(versionNumberMinor);
                if (versionNumberPart3 != 0)
                {
                        version = version + "." + AnsiString(versionNumberPart3);
                        if (versionNumberPart4 != 0)
                        {
                                version = version + "." + AnsiString(versionNumberPart4);
                        }
                }
                VersionNumber = version;
        }

        // The labels of the text are kept (their texts are in the
        // translations) but hidden: the text is drawn by Body
        {
                TLabel *labels[] = { Label19, Label20, Label2, Label3, Label5,
                        Label6, Label10, Label9, Label11, Label4, Label12, Label16,
                        Label13, Label14, Label7, Label22, Label17 };
                unsigned int i;

                for (i=0; i<sizeof(labels)/sizeof(labels[0]); i++)
                        labels[i]->Visible = false;
        }

        // As wide as the head of the window needs, but not narrower than
        // the form was designed
        MaxWidth(Label1,w); MaxWidth(Label21,w);
        MaxWidth(Label8,w); MaxWidth(Label15,w);
        ClientWidth = w + 8 > 415 ? w + 8 : 415;

        Body = new TPaintBox(this);
        Body->Parent = this;
        Body->OnPaint = BodyPaint;

        LayOut();
}

// Shown: laid out again, as the language may have changed since
void __fastcall TAbout::FormShow(TObject *Sender)
{
        // Put together with the language now (the language switch cannot
        // turn a text made by TrF back into English)
        Version->Caption = VersionNumber.IsEmpty() ? Tr("Unknown")
                                : TrF("Version {0}", VersionNumber);
        LayOut();
}

// A paragraph wrapped to the width of Body, from y; returns the y below it.
// Drawn only if draw is set (else just measured)
int TAbout::Paragraph(TCanvas *canvas, const AnsiString &text, int y, bool draw)
{
        AnsiString words[64];
        int widths[64];
        int count = 0, i, start, width = Body->Width;
        int space = canvas->TextWidth(" ");
        int lineHeight = canvas->TextHeight("Ag") + LINE_GAP;
        AnsiString rest = text.Trim();

        // The words
        while (rest.Length() && count < 64)
        {
                int position = rest.Pos(" ");

                if (position)
                {
                        words[count] = rest.SubString(1, position - 1);
                        rest = rest.SubString(position + 1, rest.Length()).TrimLeft();
                }
                else
                {
                        words[count] = rest;
                        rest = "";
                }
                widths[count] = canvas->TextWidth(words[count]);
                count++;
        }

        // The lines: as many words as fit
        start = 0;
        while (start < count)
        {
                int end = start + 1, used = widths[start], x;
                bool last;

                while (end < count && used + space + widths[end] <= width)
                {
                        used += space + widths[end];
                        end++;
                }
                last = (end == count);

                if (draw)
                {
                        int gaps = end - start - 1;
                        int extra = (last || !gaps) ? 0 : width - used;

                        x = 0;
                        for (i=start; i<end; i++)
                        {
                                canvas->TextOut(x, y, words[i]);
                                if (i < end - 1)
                                {
                                        // The extra room shared by the gaps
                                        int k = i - start;
                                        x += widths[i] + space
                                                + (extra * (k + 1)) / gaps - (extra * k) / gaps;
                                }
                        }
                }

                y += lineHeight;
                start = end;
        }

        return y;
}

// Body below the link, then the dedication and OK below it
void TAbout::LayOut(void)
{
        int y = 0;
        unsigned int i;

        Canvas->Font->Assign(Font);

        Body->SetBounds(8, Label15->Top + Label15->Height + 12,
                        ClientWidth - 16, 10);

        for (i=0; i<PARAGRAPHS; i++)
        {
                if (i) y += PARAGRAPH_GAP;
                y = Paragraph(Canvas, ParagraphText(i), y, false);
        }

        Body->Height = y;

        Button1->Top = Body->Top + Body->Height + 10;
        Button1->Left = ClientWidth - 8 - Button1->Width;
        Label18->Top = Button1->Top + (Button1->Height - Label18->Height) / 2;
        Image2->Top = Label18->Top + (Label18->Height - Image2->Height) / 2;
        ClientHeight = Button1->Top + Button1->Height + 8;

        Body->Invalidate();
}

void __fastcall TAbout::BodyPaint(TObject *Sender)
{
        int y = 0;
        unsigned int i;

        Body->Canvas->Font->Assign(Font);
        Body->Canvas->Brush->Style = bsClear;

        for (i=0; i<PARAGRAPHS; i++)
        {
                if (i) y += PARAGRAPH_GAP;
                y = Paragraph(Body->Canvas, ParagraphText(i), y, true);
        }
}

//---------------------------------------------------------------------------
void __fastcall TAbout::Button1Click(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------

void __fastcall TAbout::Label15Click(TObject *Sender)
{
        // Original = http://www.chuntey.com/eightyone/
        
        ShellExecute(NULL, NULL, "https://sourceforge.net/projects/eightyone-sinclair-emulator/", NULL, NULL, SW_NORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TAbout::Label15MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        Label15->Font->Color=clRed;
}
//---------------------------------------------------------------------------

void __fastcall TAbout::Label15MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        Label15->Font->Color=clHighlight;
}
//---------------------------------------------------------------------------



