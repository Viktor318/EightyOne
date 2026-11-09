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

#include <vcl4.h>
#pragma hdrstop

#include "BasicListingFormatInfo_.h"
#include "Language.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBasicListingFormatInfoForm *BasicListingFormatInfoForm;
//---------------------------------------------------------------------------
__fastcall TBasicListingFormatInfoForm::TBasicListingFormatInfoForm(TComponent* Owner)
        : TForm(Owner)
{
        // Two paragraphs are in two labels each (a caption could only be so
        // long in the form designer): their English halves are kept, and
        // LayOut() shows each paragraph joined in its first label, in the
        // current language
        mParagraph2a = Label26->Caption;
        mParagraph2b = Label28->Caption;
        mParagraph6a = Label29->Caption;
        mParagraph6b = Label30->Caption;
        LanguageKeepText(Label26);
        LanguageKeepText(Label29);
        Label28->Visible = false;
        Label30->Visible = false;
}
//---------------------------------------------------------------------------
// The paragraphs are word wrapped labels: they are put one under the other
// from the top, so that they keep apart whatever the font and the language,
// and the window is made as high as they need.
void TBasicListingFormatInfoForm::LayOut()
{
        TLabel *paragraphs[] = { Label27, Label26, Label25, Label29, Label31,
                                 Label36, Label1, Label32, Label33, Label34, Label35 };
        int count = sizeof(paragraphs) / sizeof(paragraphs[0]);
        int top = 9;

        Label26->Caption = Tr(mParagraph2a).Trim() + " " + Tr(mParagraph2b).Trim();
        Label29->Caption = Tr(mParagraph6a).Trim() + " " + Tr(mParagraph6b).Trim();

        for (int i = 0; i < count; i++)
        {
                TLabel *label = paragraphs[i];
                label->AutoSize = false;
                label->SetBounds(9, top, ClientWidth - 18, label->Height);
                label->AutoSize = true;
                top = label->Top + label->Height + ((label == Label34) ? 3 : 7);
        }

        btnOK->Top = top + 8;
        btnOK->Left = (ClientWidth - btnOK->Width) / 2;
        ClientHeight = btnOK->Top + btnOK->Height + 12;
}
//---------------------------------------------------------------------------
void __fastcall TBasicListingFormatInfoForm::FormShow(TObject *Sender)
{
        LayOut();
}
//---------------------------------------------------------------------------

void __fastcall TBasicListingFormatInfoForm::Weblink1Click(TObject *Sender)
{
        OpenWebsite();
}
//---------------------------------------------------------------------------

void __fastcall TBasicListingFormatInfoForm::Weblink2Click(TObject *Sender)
{
        OpenWebsite();
}
//---------------------------------------------------------------------------

void __fastcall TBasicListingFormatInfoForm::Weblink3Click(TObject *Sender)
{
        OpenWebsite();
}
//---------------------------------------------------------------------------
void TBasicListingFormatInfoForm::OpenWebsite()
{
        ShellExecute(NULL, "open", "http://freestuff.grok.co.uk/zxtext2p/index.html", NULL, NULL, SW_SHOW);
}
