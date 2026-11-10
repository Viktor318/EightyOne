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

#include "BasicLister_.h"
#include "zx81config.h"
#include "main_.h"
#include <iostream>
#include <fstream>
#include "BasicListerOptions_.h"
#include "BasicListingFormatInfo_.h"
#include "Language.h"
#include "Debug.h"
#include "BasicVariables_.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBasicLister *BasicLister;
extern void RefreshCallback();


enum StatusBarIndex
{
        PanelLines = 0,
        PanelLineInfo
};

//---------------------------------------------------------------------------
__fastcall TBasicLister::TBasicLister(TComponent* Owner)
        : TForm(Owner),
        mBitmap(NULL),
        mHWND(this->Handle),
        mLastHighlightedEntryIndex(-1),
        mLastFilterIndex(1),
        mBasicLister(NULL),
        mLastBreakPointIndex(-1),
        mLastBreakPointMenuIndex(-1),
        mHasDebug(false),
        mRelativePos(0)
{
        mRefreshLock = new TCriticalSection();
        mLines = new std::vector<LineInfo>();
        mProgramDisplayRows = 0;
        Dbg->SetBPListChangedCB(RefreshCallback);
        mToolbarHeight = ToolBar->Height;
}

void TBasicLister::SizeWindow()
{
        int displayAreaWidth = ListingAreaWidth(DisplayableColumns * PixelsPerCharacterHeight * mScaling);
        int displayAreaHeight = (DisplayableRows * PixelsPerCharacterHeight * mScaling);

        ClientWidth = displayAreaWidth + ScrollBar->Width + 1;
        ClientHeight = mToolbarHeight + StatusBar->Height + displayAreaHeight;
}

// The tool bar's buttons can need more room than the listing (a bigger
// font, a longer translation): the listing area is then made wider, so that
// the window shows all of them.
int TBasicLister::ListingAreaWidth(int listingWidth)
{
        int toolBarWidth = 0;
        for (int i = 0; i < ToolBar->ButtonCount; i++)
        {
                TToolButton *button = ToolBar->Buttons[i];
                if (button->Visible && button->Left + button->Width > toolBarWidth)
                {
                        toolBarWidth = button->Left + button->Width;
                }
        }

        int needed = toolBarWidth + 4 - ScrollBar->Width - 1;
        return (listingWidth > needed) ? listingWidth : needed;
}

__fastcall TBasicLister::~TBasicLister()
{
        ::DeleteObject(mBitmap);
        SetBasicLister(NULL, true);
        delete mLines;
        delete mRefreshLock;
}

void TBasicLister::SetBasicLister(IBasicLister* basicLister, bool exiting)
{
        IBasicLister::StopRefresh();
        mRefreshLock->Acquire();
        mLines->clear();

        if (mBasicLister != NULL)
        {
                delete mBasicLister;
        }

        mBasicLister = basicLister;

        if (mBasicLister != NULL)
        {
                mBasicLister->CopyCsetImage();
                mHasDebug = mBasicLister->BasicDebugSupported();
                bool hasVariables = mBasicLister->BasicVariablesSupported();
                bool hasExtras = mHasDebug || hasVariables;

                DebugControls->Visible = false;
                DebugControls->Enabled = false;
                ToolButtonRunStop->Enabled = false;
                Variables->Enabled = false;
                BasicVariables->Close();

                if (hasExtras)
                {
                        DebugControls->Visible = true;
                        DebugControls->Enabled = true;
                }

                if (mHasDebug)
                {
                        ToolButtonRunStop->Enabled = true;
                }

                if (hasVariables)
                {
                        Variables->Enabled = true;
                        if (Form1->BasicListerOption->Checked && Variables->Down) BasicVariables->ShowScale(mScaling);
                }


                mBasicLister->PopulateKeywords();
                mBasicLister->SetLines(mLines);
                mBasicLister->SetBpEnabledBitmap(BpEnabledImg->Picture->Bitmap);
                mBasicLister->SetBpDisabledBitmap(BpDisabledImg->Picture->Bitmap);
                mToolbarHeight = ToolBar->Height + (hasExtras ? DebugControls->Height: 0);
                SizeWindow();
        }
        else
        {
                mHasDebug = false;
                mToolbarHeight = ToolBar->Height;
                DebugControls->Visible = false;
                DebugControls->Enabled = false;
                ToolButtonRunStop->Enabled = false;
                Variables->Enabled = false;
        }

        StepBasic->Enabled = false;

        if (!exiting) BasicVariables->SetLister(mBasicLister);

        mRefreshLock->Release();
        IBasicLister::GoRefresh();
}

bool TBasicLister::ListerAvailable()
{
        return (mBasicLister != NULL);
}

void TBasicLister::ClearBitmap()
{
        if (mBitmap)
        {
            ::DeleteObject(mBitmap);
        }

        HDC hdc = GetDC(mHWND);
        HDC chdc = CreateCompatibleDC(hdc);

        mBitmap = ::CreateCompatibleBitmap(hdc, mBMWidth, mBMHeight);

        HGDIOBJ oldbm = SelectObject(chdc, mBitmap);

        RECT rect;
        rect.left = 0;
        rect.top = 0;
        rect.right = mBMWidth;
        rect.bottom = mBMHeight;

        if (mBasicLister) mBasicLister->ClearRenderedListing(chdc, rect, ToolButtonLineEnds->Down);

        SelectObject(chdc, oldbm);
        DeleteDC(chdc);
        ReleaseDC(mHWND,hdc);
}

void TBasicLister::ExtractProgramDetails()
{
        mBasicLister->ExtractProgramDetails();
}

void TBasicLister::ConstructBitmap()
{
        if (mBitmap)
        {
            ::DeleteObject(mBitmap);
        }

        int displayRows = mBasicLister->GetProgramRows();
        int displayColumns = 1 + mBasicLister->GetDisplayColumns();

        mBMWidth = ListingAreaWidth(displayColumns * PixelsPerCharacterWidth * mScaling);
        mBMHeight = displayRows * PixelsPerCharacterHeight * mScaling;

        ClientWidth = mBMWidth + ScrollBar->Width + 1;

        HDC hdc = GetDC(mHWND);
        HDC chdc = CreateCompatibleDC(hdc);

        mBitmap = ::CreateCompatibleBitmap(hdc, mBMWidth, mBMHeight);

        HGDIOBJ oldbm = SelectObject(chdc, mBitmap);

        RECT rect;
        rect.left = 0;
        rect.top = 0;
        rect.right = mBMWidth;
        rect.bottom = mBMHeight;

        mBasicLister->RenderListing(chdc, rect, ToolButtonLineEnds->Down, mScaling);

        SelectObject(chdc, oldbm);
        DeleteDC(chdc);
        ReleaseDC(mHWND,hdc);
}

//---------------------------------------------------------------------------

void TBasicLister::UnhighlightRows(int startRow, int endRow)
{
        const bool unhighlight = false;
        ColourRows(startRow, endRow, HIGHLIGHT, unhighlight);
}

void TBasicLister::HighlightRows(int startRow, int endRow)
{
        const bool highlight = true;
        ColourRows(startRow, endRow, HIGHLIGHT, highlight);
}

void TBasicLister::UnBreakPointRows(int startRow, int endRow)
{
        const bool unbreakpoint = false;
        ColourRows(startRow, endRow, BREAKPOINT, unbreakpoint);
        if (mLastHighlightedEntryIndex != -1)
        {
                int startRow = (*mLines)[mLastHighlightedEntryIndex].startDisplayRow;
                int endRow = startRow + (*mLines)[mLastHighlightedEntryIndex].displayRows - 1;
                HighlightRows(startRow, endRow);
        }
}

void TBasicLister::BreakPointRows(int startRow, int endRow)
{
        UnhighlightRows(startRow, endRow);
        const bool breakpoint = true;
        ColourRows(startRow, endRow, BREAKPOINT, breakpoint);
}

void TBasicLister::ColourRows(int startRow, int endRow, LineMode mode, bool setornot)
{
        HDC hdc = GetDC(mHWND);
        HDC chdc = CreateCompatibleDC(hdc);
        HGDIOBJ oldbm = SelectObject(chdc, mBitmap);

        COLORREF paperColour = mBasicLister->GetPaperColour();
        COLORREF highlightColour = GetHighlightColour();
        COLORREF breakpointColour = GetBreakPointColour();
        COLORREF findColour;
        COLORREF replaceColour;

        switch (mode)
        {
        case HIGHLIGHT:
                findColour = setornot ? paperColour : highlightColour;
                replaceColour = setornot ? highlightColour : paperColour;
                break;

        case BREAKPOINT:
                findColour = setornot ? paperColour : breakpointColour;
                replaceColour = setornot ? breakpointColour : paperColour;
                break;
        }

        int startX = PixelsPerCharacterWidth * mScaling;
        int startY = startRow * PixelsPerCharacterHeight * mScaling;
        int endY = (endRow + 1) * PixelsPerCharacterHeight * mScaling;

        bool doubleSize = (mScaling == 2);

        for (int y = startY; y < endY; y += mScaling)
        {
                int xWidth = mBMWidth * mScaling;

                for (int x = startX; x < xWidth; x += mScaling)
                {
                        COLORREF pixelColor = GetPixel(chdc, x, y);
                        if (pixelColor == findColour)
                        {
                                SetPixelV(chdc, x, y, replaceColour);
                                if (doubleSize)
                                {
                                        SetPixelV(chdc, x+1, y, replaceColour);
                                        SetPixelV(chdc, x, y+1, replaceColour);
                                        SetPixelV(chdc, x+1, y+1, replaceColour);
                                }
                        }
                }
        }

        SelectObject(chdc, oldbm);
        DeleteDC(chdc);
        ReleaseDC(mHWND,hdc);
}

void TBasicLister::HighlightLine(int lineNumber)
{
        int index = FindLineIndex(lineNumber);
        HighlightEntry(index);
}

void TBasicLister::BreakPointLine(int lineNumber)
{
        int index = FindLineIndex(lineNumber);
        BreakPointEntry(index);
        ScrollToIndex(index);
}

void RefreshCallback()
{
        const bool keepScrollbarPosition = true;
        BasicLister->Refresh(keepScrollbarPosition);
}

void TBasicLister::UnhighlightEntry(int index)
{
        if (index == -1)
        {
                index = mLastHighlightedEntryIndex;
        }

        if (index != -1)
        {
                int startRow = (*mLines)[index].startDisplayRow;
                int endRow = startRow + (*mLines)[index].displayRows - 1;
                UnhighlightRows(startRow, endRow);

                StatusBar->Panels->Items[PanelLineInfo]->Text = "";

                Invalidate();
        }

        mLastHighlightedEntryIndex = -1;
}

void TBasicLister::HighlightEntry(int index)
{
        if (index != -1)
        {
                UnhighlightEntry(mLastHighlightedEntryIndex);

                int startRow = (*mLines)[index].startDisplayRow;
                int endRow = startRow + (*mLines)[index].displayRows - 1;
                HighlightRows(startRow, endRow);
        }
        Invalidate();

        mLastHighlightedEntryIndex = index;

        AnsiString lineDetails = "";

        if (index != -1)
        {
                int lineNumber = (*mLines)[index].lineNumber;
                lineDetails += TrF("Line {0}: ", AnsiString(lineNumber));

                int startAddress = (*mLines)[index].address;
                lineDetails += "$" + AnsiString::IntToHex(startAddress, 4);

                int endAddress = startAddress + (*mLines)[index].lineLength - 1;
                lineDetails += "-$" + AnsiString::IntToHex(endAddress, 4);
        }

        StatusBar->Panels->Items[PanelLineInfo]->Text = lineDetails;
}

void TBasicLister::UnBreakPointLastEntry()
{
        if (mLastBreakPointIndex == -1)
        {
                return;
        }

        int startRow = (*mLines)[mLastBreakPointIndex].startDisplayRow;
        int endRow = startRow + (*mLines)[mLastBreakPointIndex].displayRows - 1;
        UnBreakPointRows(startRow, endRow);

        Invalidate();

        mLastBreakPointIndex = -1;
}

void TBasicLister::BreakPointEntry(int index)
{
        if (index != -1)
        {
                UnBreakPointLastEntry();

                int startRow = (*mLines)[index].startDisplayRow;
                int endRow = startRow + (*mLines)[index].displayRows - 1;
                BreakPointRows(startRow, endRow);
        }

        Invalidate();

        mLastBreakPointIndex = index;
}

int TBasicLister::FindLineIndex(int lineNumber)
{
        int index = mLines->size() - 1;

        while (index >= 0)
        {
                if ((*mLines)[index].lineNumber == lineNumber)
                {
                        break;
                }

                index--;
        }

        return index;
}

COLORREF TBasicLister::GetHighlightColour()
{
        return RGB(255, 255, 132);
}

COLORREF TBasicLister::GetBreakPointColour()
{
        return RGB(255, 134, 134);
}

//---------------------------------------------------------------------------

// The first row of the listing shown. A listing that fits the window is
// shown from its start whatever the scroll bar says: its position can be
// read while it is being reset (Refresh() is also called from the
// emulation's thread), and then it moved a short listing up by a row,
// under the tool bar.
int TBasicLister::ListingTopRow()
{
        return (mProgramDisplayRows > DisplayableRows) ? ScrollBar->Position : 0;
}

void __fastcall TBasicLister::FormPaint(TObject *Sender)
{
        HDC hdc = (HDC)Canvas->Handle;
        HDC chdc = CreateCompatibleDC(hdc);
        HGDIOBJ oldbm = SelectObject(chdc, mBitmap);

        if (mBasicLister != NULL)
        {
                int programDisplayRows = mBasicLister->GetProgramRows();
                int programDisplayPixels = programDisplayRows * PixelsPerCharacterHeight * mScaling;
                int copyHeight = (programDisplayPixels > mBMHeight) ? programDisplayPixels : mBMHeight;

                ::BitBlt(hdc, 0, mToolbarHeight - ListingTopRow() * PixelsPerCharacterHeight * mScaling, mBMWidth, copyHeight, chdc, 0, 0, SRCCOPY);
        }
        else
        {
                RECT rect;
                rect.left = 0;
                rect.top = mToolbarHeight;
                rect.right = ClientWidth;
                rect.bottom = ClientHeight - mToolbarHeight - StatusBar->Height;
                FillRect(chdc, &rect, (HBRUSH)(COLOR_BTNFACE+1));
        }

        SelectObject(chdc, oldbm);
        DeleteDC(chdc);
}

void TBasicLister::CallShow()
{
        Show();

        // Opened from the menu: the listing of the program in memory at
        // once, not only after the next line is entered
        Refresh(false);
        if (Form1->BasicListerOption->Checked && Variables->Down) BasicVariables->ShowScale(mScaling);
}

//---------------------------------------------------------------------------

void __fastcall TBasicLister::FormShow(TObject *Sender)
{
        GetSaveOptions();
        SizeWindow();
        //Refresh(false);
}
//---------------------------------------------------------------------------

void __fastcall TBasicLister::ScrollBarChange(TObject *Sender)
{
        Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TBasicLister::ToolButtonRefreshClick(TObject *Sender)
{
        const bool keepScrollbarPosition = true;
        Refresh(keepScrollbarPosition);
}
//---------------------------------------------------------------------------

int TBasicLister::HandleRefreshThreadProc(void *param)
{
        TBasicLister* self = static_cast<TBasicLister*>(param);
        self->mRefreshLock->Acquire();
        try {

                self->HandleRefresh();
        } __finally {
                self->mRefreshLock->Release();
        }
        return 0;
}

void TBasicLister::HandleRefresh(void)
{
        LoadProgram(false);
        BreakPointEntry(mLastBreakPointIndex);

        PostMessage(mHWND, WM_SCROLLBAR, mRelativePos, 0);
}

void TBasicLister::Refresh(bool keepScrollbarPosition)
{
        double relativePos = ScrollBar->Max > 0 ? (double)ScrollBar->Position / ScrollBar->Max : 0;

        mRelativePos = (keepScrollbarPosition ? relativePos : 0)*1000;

        DisableButtons();
        Form1->ThreadPool.EnqueueTask(new TTask(HandleRefreshThreadProc, (void *)this));
}
//---------------------------------------------------------------------------

int TBasicLister::HandleClearThreadProc(void *param)
{
        TBasicLister* self = static_cast<TBasicLister*>(param);

        self->HandleClear();
        return 0;
}

void TBasicLister::HandleClear(void)
{
        IBasicLister::StopRefresh();
        mRefreshLock->Acquire();
        mLines->clear();

        ClearBitmap();
        PostMessage(mHWND, WM_STATUSBAR, 0, 0);
        PostMessage(mHWND, WM_SCROLLBAR, 0, 0);

        mRefreshLock->Release();
        IBasicLister::GoRefresh();
}

void TBasicLister::Clear()
{
        // Only clear if there are lines to clear
        if (mLines->size()>0)
        {
                DisableButtons();
                Form1->ThreadPool.EnqueueTask(new TTask(HandleClearThreadProc, (void *)this));
        }

        BasicVariables->Clear();
}
//---------------------------------------------------------------------------

void TBasicLister::LoadProgram(bool keepEntries)
{
        if (!keepEntries) mLastHighlightedEntryIndex = -1;
        
        if (mBasicLister != NULL)
        {
                ExtractProgramDetails();
                ConstructBitmap();
        }

        PostMessage(mHWND, WM_STATUSBAR, 0, 0);
}

void __fastcall TBasicLister::WMUpdateScrollBar(TMessage &Message)
{
        if (Message.WParam==0)
        {
                ScrollBar->Min = 0;
                mProgramDisplayRows = mBasicLister != NULL ? mBasicLister->GetProgramRows() : 0;
                bool scrollable = (mProgramDisplayRows > DisplayableRows);
                if (scrollable)
                        ScrollBar->Max = mProgramDisplayRows - DisplayableRows;
                else
                        ScrollBar->Max = mProgramDisplayRows;

                ScrollBar->SmallChange = 1;
                ScrollBar->LargeChange = DisplayableRows;
                ScrollBar->Position = 1;
                ScrollBar->Position = 0;     // This forces the scroll bar to be disabled
                ScrollBar->Enabled = scrollable;
        }
        else
                ScrollBar->Position = (int)(ceil((Message.WParam/1000.0) * ScrollBar->Max));

        Invalidate();
        EnableButtons();
}

// After a change of language: the program's details in the status bar, and
// the listing again, as wide as the tool bar's new captions need

void TBasicLister::LanguageChanged()
{
        if (Visible) Refresh(true);
        else PostMessage(mHWND, WM_STATUSBAR, 0, 0);
}

void __fastcall TBasicLister::WMUpdateStatusBar(TMessage &Message)
{
        AnsiString programDetails;

        if (mBasicLister != NULL)
        {
                programDetails += TrF("Lines {0}", AnsiString((int)mLines->size()));

                int programSize = ProgramSize();
                if (programSize > 0)
                {
                        int basicStart = mBasicLister->GetProgramStartAddress();
                        programDetails += ": $";
                        programDetails += AnsiString::IntToHex(basicStart, 4);
                        programDetails += "-$";
                        programDetails += AnsiString::IntToHex(basicStart + programSize - 1, 4);
                }
        }

        StatusBar->Panels->Items[PanelLines]->Text = programDetails;
        StatusBar->Panels->Items[PanelLineInfo]->Text = "";

        Invalidate();
        EnableButtons();
}

void TBasicLister::DisableButtons()
{
        ToolButtonRefresh->Enabled = false;
        ToolButtonSettings->Enabled = false;
        ToolButtonSave->Enabled = false;
        ToolButtonLineEnds->Enabled = false;
        ToolButtonInfo->Enabled = false;

        ScrollBar->Enabled = false;
}

void TBasicLister::EnableButtons()
{
        ToolButtonRefresh->Enabled = true;
        ToolButtonSettings->Enabled = true;
        bool programLoaded = (ProgramSize() > 0);
        ToolButtonSave->Enabled = programLoaded;
        ToolButtonLineEnds->Enabled = programLoaded;
        ToolButtonInfo->Enabled = true;

        ScrollBar->Enabled = (mProgramDisplayRows > DisplayableRows);
}

int TBasicLister::ProgramSize()
{
        int programSize = 0;

        for (std::vector<LineInfo>::iterator it = mLines->begin(); it != mLines->end(); it++)
        {
                programSize += (*it).lineLength;
        }

        return programSize;
}

//---------------------------------------------------------------------------

int TBasicLister::FindLineDisplayedOnRow(int row)
{
        int index = mLines->size() - 1;

        while (index >= 0)
        {
                int startDisplayRow = (*mLines)[index].startDisplayRow;
                int displayRows = (*mLines)[index].displayRows;
                int endDisplayRow = startDisplayRow + displayRows - 1;
                
                if ((row >= startDisplayRow) && (row <= endDisplayRow))
                {
                        break;
                }

                index--;
        }

        return index;
}

void TBasicLister::ScrollToIndex(int index)
{
        if (mProgramDisplayRows > DisplayableRows)
        {
                double relativePos = mProgramDisplayRows > 0 ? (double)(*mLines)[index].startDisplayRow / mProgramDisplayRows : 0;

                ScrollBar->Position = (int)(ceil(relativePos * ScrollBar->Max));
        }
}

int TBasicLister::HandleMouseDownThreadProc(void *param)
{
        TBasicLister* self = static_cast<TBasicLister*>(param);

        self->HandleMouseDown();
        return 0;
}

void TBasicLister::HandleMouseDown(void)
{
        if (mLastRowIndex != mLastHighlightedEntryIndex)
        {
                HighlightEntry(mLastRowIndex);
        }
        else
        {
                UnhighlightEntry(mLastRowIndex);
        }
}

//---------------------------------------------------------------------------

void __fastcall TBasicLister::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        int rowWithinClientArea = (Y - mToolbarHeight) / (PixelsPerCharacterHeight * mScaling);
        int row = rowWithinClientArea + ListingTopRow();

        mLastRowIndex = FindLineDisplayedOnRow(row);

        if (Button == mbLeft)
        {
                Form1->ThreadPool.EnqueueTask(new TTask(HandleMouseDownThreadProc, (void *)this));
        }
        else if (Button == mbRight && mHasDebug)
        {
                if (mLastRowIndex != -1) PopupMenu1->Popup(Mouse->CursorPos.x, Mouse->CursorPos.y);
        }
}

//---------------------------------------------------------------------------

void __fastcall TBasicLister::FormMouseWheel(TObject *Sender,
      TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{
        if (mProgramDisplayRows > DisplayableRows)
        {
                int currentPos = ScrollBar->Position;
                int newPos = currentPos - (WheelDelta / 120);

                if (newPos < ScrollBar->Min)
                {
                        newPos = ScrollBar->Min;
                }

                if (newPos > ScrollBar->Max)
                {
                        newPos = ScrollBar->Max;
                }

                ScrollBar->Position = newPos;
        }

        Handled = true;
}

//---------------------------------------------------------------------------
       
void __fastcall TBasicLister::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        Form1->BasicListerOption->Checked = false;        
        BasicVariables->Close();
}
//---------------------------------------------------------------------------

void __fastcall TBasicLister::ToolButtonSaveClick(TObject *Sender)
{
        DisableButtons();
        Form1->ThreadPool.EnqueueTask(new TTask(HandleSaveListingToFileThreadProc, (void *)this));
}
//---------------------------------------------------------------------------

int TBasicLister::HandleSaveListingToFileThreadProc(void *param)
{
        TBasicLister* self = static_cast<TBasicLister*>(param);

        self->SaveListingToFile();
        PostMessage(self->mHWND, WM_STATUSBAR, 0, 0);
        return 0;
}

void TBasicLister::SaveListingToFile()
{
        if (mBasicLister == NULL)
        {
                return;
        }

        LoadProgram(true);

        bool programLoaded = (ProgramSize() > 0);
        if (!programLoaded)
        {
                Application->MessageBox(Tr("There is no BASIC program loaded.").c_str(), Tr("Save BASIC Listing").c_str(), MB_OK | MB_ICONERROR);
                return;
        }

        AnsiString machineName = mBasicLister->GetMachineName();
        AnsiString basicFileExtension = mBasicLister->GetBasicFileExtension();
        bool zxTokenFormatSupported = mBasicLister->ZxTokenSupported();

        AnsiString filter = TrF("{0} ZxText2P BASIC File (*.{1})", machineName, basicFileExtension) + "|*." + basicFileExtension;
        if (zxTokenFormatSupported)
        {
                filter += "|" + TrF("{0} ZxToken BASIC File (*.{1})", machineName, basicFileExtension) + "|*." + basicFileExtension;
        }
        filter += "|" + Tr("Bitmap File (*.bmp)") + "|*.bmp";

        SaveDialog->Filter = filter;
        SaveDialog->DefaultExt = basicFileExtension;
        SaveDialog->FilterIndex = mLastFilterIndex;
        SaveDialog->FileName = "";
        SaveDialog->Options << ofOverwritePrompt;
        SaveDialog->Title = Tr("Save BASIC Listing");

        if (!SaveDialog->Execute())
        {
                return;
        }

        mLastFilterIndex = SaveDialog->FilterIndex;

        bool outputInZxTokenFormat = (zxTokenFormatSupported && SaveDialog->FilterIndex == 2);

        if (SaveDialog->FilterIndex == 1 || outputInZxTokenFormat)
        {
                GetSaveOptions();

                std::ofstream ofs;
                ofs.open(SaveDialog->FileName.c_str());
                for (std::vector<LineInfo>::iterator it = mLines->begin(); it != mLines->end(); it++)
                {
                        AnsiString lineText = mBasicLister->RenderLineAsText(*it, mOutputRemTokensAsCharacterCodes, mOutputStringTokensAsCharacterCodes, mOutputNonAsciiAsCharacterCodes, mOutputVariableNamesInLowercase, outputInZxTokenFormat, mLimitLineLengths, mOutputFullWidthLineNumbers);
                        ofs << lineText.c_str() << '\n';
                }

                ofs.close();
        }
        else
        {
                int highlightIndex = mLastHighlightedEntryIndex;
                UnhighlightEntry(highlightIndex);

                Graphics::TBitmap* tempBitmap = new Graphics::TBitmap();
                tempBitmap->Handle = mBitmap;
                tempBitmap->SaveToFile(SaveDialog->FileName);
                tempBitmap->ReleaseHandle();
                delete tempBitmap;

                HighlightEntry(highlightIndex);
        }
}

void TBasicLister::SaveSettings(TIniFile *ini)
{
        ini->WriteInteger("BASICLISTER", "Top", Top);
        ini->WriteInteger("BASICLISTER", "Left", Left);
        ini->WriteBool("BASICLISTER", "ShowLineEnds", ToolButtonLineEnds->Down);
        ini->WriteBool("BASICLISTER", "Variables", Variables->Down);
}

void TBasicLister::LoadSettings(TIniFile *ini)
{
        Top = ini->ReadInteger("BASICLISTER", "Top", Top);
        Left = ini->ReadInteger("BASICLISTER", "Left", Left);
        ToolButtonLineEnds->Down = ini->ReadBool("BASICLISTER", "ShowLineEnds", ToolButtonLineEnds->Down);
        Variables->Down = ini->ReadBool("BASICLISTER", "Variables", Variables->Down);

        if (Form1->BasicListerOption->Checked) Show();
}

int TBasicLister::BasicLineExecuteStartAddress()
{
        return mBasicLister != NULL ? mBasicLister->GetBasicLineExecuteStartAddress() : 65535;
}

bool TBasicLister::BasicLineExecuteStart(int pc)
{
        return mBasicLister != NULL && mBasicLister->IsBasicLineExecuteStart(pc);
}

int TBasicLister::NextBasicLineNumberToExecute()
{
        return mBasicLister != NULL ? mBasicLister->GetNextBasicLineNumber() : 65535;
}

void TBasicLister::BreakAtNextBasicLine()
{
        if (mBasicLister == NULL) return;

        ToolButtonRunStop->Caption = Tr("Run");
        StepBasic->Enabled = true;

        BreakPointLine(mBasicLister->GetNextBasicLineNumber());
}

int TBasicLister::HandleLineEndsThreadProc(void *param)
{
        TBasicLister* self = static_cast<TBasicLister*>(param);

        self->HandleLineEnds();
        return 0;
}

void TBasicLister::HandleLineEnds(void)
{
        LoadProgram(true);
        
        HighlightEntry(mLastHighlightedEntryIndex);
        BreakPointEntry(mLastBreakPointIndex);

        PostMessage(mHWND, WM_STATUSBAR, 0, 0);
}

void __fastcall TBasicLister::ToolButtonLineEndsClick(TObject *Sender)
{
        ToolButtonLineEnds->Down = !ToolButtonLineEnds->Down;

        DisableButtons();
        Form1->ThreadPool.EnqueueTask(new TTask(HandleLineEndsThreadProc, (void *)this));
}
//---------------------------------------------------------------------------

void __fastcall TBasicLister::ToolButtonSettingsClick(TObject *Sender)
{
        DisableButtons();

        SaveBasicListingOptionsForm->Left = BasicLister->Left + (BasicLister->Width - SaveBasicListingOptionsForm->Width) / 2;
        SaveBasicListingOptionsForm->Top = BasicLister->Top + (BasicLister->Height - SaveBasicListingOptionsForm->Height) / 2;

        SaveBasicListingOptionsForm->ShowModal();

        GetSaveOptions();
        SizeWindow();

        EnableButtons();

        if (Form1->BasicListerOption->Checked && Variables->Down) BasicVariables->ShowScale(mScaling);
        
        const bool keepScrollbarPosition = false;
        Refresh(keepScrollbarPosition);
}

void TBasicLister::GetSaveOptions()
{
        mOutputRemTokensAsCharacterCodes = SaveBasicListingOptionsForm->GetOutputRemTokensAsCharacterCodes();
        mOutputStringTokensAsCharacterCodes = SaveBasicListingOptionsForm->GetOutputStringTokensAsCharacterCodes();
        mOutputNonAsciiAsCharacterCodes = SaveBasicListingOptionsForm->GetOutputNonAsciiAsCharacterCodes();
        mOutputVariableNamesInLowercase = SaveBasicListingOptionsForm->GetOutputVariableNamesInLowercase();
        mLimitLineLengths = SaveBasicListingOptionsForm->GetLimitLineLengths();
        mOutputFullWidthLineNumbers = SaveBasicListingOptionsForm->GetOutputFullWidthLineNumbers();
        mScaling = SaveBasicListingOptionsForm->GetScalingFator();
}

void __fastcall TBasicLister::ToolButtonInfoClick(TObject *Sender)
{
        BasicListingFormatInfoForm->LayOut();
        BasicListingFormatInfoForm->Left = BasicLister->Left + (BasicLister->Width - BasicListingFormatInfoForm->Width) / 2;
        BasicListingFormatInfoForm->Top = BasicLister->Top + (BasicLister->Height - BasicListingFormatInfoForm->Height) / 2;

        DisableButtons();

        BasicListingFormatInfoForm->ShowModal();

        EnableButtons();
}
//---------------------------------------------------------------------------

void __fastcall TBasicLister::PopupMenu1Popup(TObject *Sender)
{
        breakpoint bp((*mLines)[mLastRowIndex].lineNumber, BP_BASIC);
        mLastBreakPointMenuIndex = Dbg->FindBreakPointEntry(0, bp, false);
        if (mLastBreakPointMenuIndex < 0)
        {
                PopupMenu1->Items->Items[Add_Delete_BP]->Caption = Tr("Add Breakpoint");
                PopupMenu1->Items->Items[Enable_BP]->Enabled = false;
                PopupMenu1->Items->Items[Enable_BP]->Checked = false;
        }
        else
        {
                PopupMenu1->Items->Items[Add_Delete_BP]->Caption = Tr("Delete Breakpoint");
                PopupMenu1->Items->Items[Enable_BP]->Enabled = true;
                PopupMenu1->Items->Items[Enable_BP]->Checked = Dbg->BreakpointIsEnabled(mLastBreakPointMenuIndex);
        }
}
//---------------------------------------------------------------------------


void __fastcall TBasicLister::AddBreakPointClick(TObject *Sender)
{
        if (mLastBreakPointMenuIndex < 0)
        {
                breakpoint bp((*mLines)[mLastRowIndex].lineNumber, BP_BASIC);
                Dbg->AddBreakPoint(bp);
        }
        else
        {
                Dbg->DelBreakPoint(mLastBreakPointMenuIndex);
        }
}
//---------------------------------------------------------------------------

void __fastcall TBasicLister::Enabled1Click(TObject *Sender)
{
        int newstate = !Dbg->BreakpointIsEnabled(mLastBreakPointMenuIndex);
        Dbg->SetBreakpointEnabledState(mLastBreakPointMenuIndex, newstate);
}
//---------------------------------------------------------------------------

void __fastcall TBasicLister::DisableAll1Click(TObject *Sender)
{
        Dbg->SetEnabledStateAllOfType(BP_BASIC, false);
}
//---------------------------------------------------------------------------


void __fastcall TBasicLister::ToolButtonRunStopClick(TObject *Sender)
{
        if (LanguageSame(ToolButtonRunStop->Caption, "Run"))
        {
                ToolButtonRunStop->Caption = Tr("Stop");
                Dbg->BasicStartStop(false);
        }
        else
        {
                ToolButtonRunStop->Caption = Tr("Run");
                Dbg->BasicStartStop(true);
        }

        StepBasic->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TBasicLister::StepBasicClick(TObject *Sender)
{
        ToolButtonRunStop->Caption = Tr("Run");
        StepBasic->Enabled = false;
        Dbg->BasicStartStop(true);
}
//---------------------------------------------------------------------------

void __fastcall TBasicLister::VariablesClick(TObject *Sender)
{
        Variables->Down = !Variables->Down;
        if (BasicVariables->Visible)
        {
                BasicVariables->Close();
                return;
        }

        BasicVariables->ShowScale(mScaling);
}
//---------------------------------------------------------------------------

void __fastcall TBasicLister::DeletAll1Click(TObject *Sender)
{
        Dbg->RemoveAllBasicBPs();
}
//---------------------------------------------------------------------------

void __fastcall TBasicLister::RuntoLine1Click(TObject *Sender)
{
        if (mLastBreakPointMenuIndex < 0)
        {
                breakpoint bp((*mLines)[mLastRowIndex].lineNumber, BP_BASIC);
                bp.Permanent = false;
                Dbg->AddBreakPoint(bp);
                ToolButtonRunStop->Caption = Tr("Stop");
                Dbg->BasicStartStop(false);
                StepBasic->Enabled = false;
        }
        else
        {
                Dbg->DelBreakPoint(mLastBreakPointMenuIndex);
        }
}
//---------------------------------------------------------------------------

