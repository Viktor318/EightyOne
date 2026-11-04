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

#ifndef midifrmH
#define midifrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <IniFiles.hpp>

#include "sound\sound.h"
//---------------------------------------------------------------------------

// A mixer fader: a vertical slider (0-100%) with a level meter beside it,
// and a scale on its left if asked for. Drag it (the percentage is shown
// above it meanwhile), click on the track to jump there, double click for
// the default, arrows / Page Up / Page Down / Home / End, or the mouse wheel
// (through the form).

class TFader : public TCustomControl
{
private:
        int FPosition;
        int FDefault;
        bool FScale;
        bool FMuted;
        bool FDragging;
        int FDragOffset;
        double FLevel;                  // the meter, 0-1
        int FLit;                       // meter segments lit
        THintWindow *FBubble;

        int Top_(void);
        int Bottom_(void);
        int TrackX(void);
        int YOf(int value);
        int ValueAt(int y);
        void ShowBubble(void);
        void HideBubble(void);

        void __fastcall WMGetDlgCode(TMessage &Message);
        void __fastcall WMEraseBkgnd(TWMEraseBkgnd &Message);

protected:
        virtual void __fastcall Paint(void);
        DYNAMIC void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
        DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
        DYNAMIC void __fastcall MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
        DYNAMIC void __fastcall DblClick(void);
        DYNAMIC void __fastcall KeyDown(Word &Key, Classes::TShiftState Shift);
        DYNAMIC void __fastcall DoEnter(void);
        DYNAMIC void __fastcall DoExit(void);

public:
        __fastcall TFader(Classes::TComponent *Owner, bool scale);

        int Position(void) { return FPosition; }
        void SetPosition(int value);    // also calls OnChange if it changed
        void Reset(void) { SetPosition(FDefault); }
        void SetDefault(int value) { FDefault = value; }
        void SetMuted(bool muted);
        void SetLevel(double level);
        void Wheel(int steps, bool big);

        static int WidthFor(bool scale);

        TNotifyEvent OnChange;

BEGIN_MESSAGE_MAP
        VCL_MESSAGE_HANDLER(WM_GETDLGCODE, TMessage, WMGetDlgCode)
        VCL_MESSAGE_HANDLER(WM_ERASEBKGND, TWMEraseBkgnd, WMEraseBkgnd)
END_MESSAGE_MAP(TCustomControl)
};

//---------------------------------------------------------------------------
class TMidiForm : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox2;
        TLabel *StereoLabel;
        TComboBox *StereoBox;
        TButton *DefaultsButton;
        TLabel *HelpLabel;
        TGroupBox *GroupBox1;
        TComboBox *MidiBox;
        TGroupBox *VideoSoundGroupBox;
        TCheckBox *BeeperExcludeHSyncs;
        TButton *OK;
        void __fastcall OKClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall MidiChange(TObject *Sender);
        void __fastcall BeeperExcludeHSyncsClick(TObject *Sender);
        void __fastcall StereoBoxChange(TObject *Sender);
        void __fastcall DefaultsButtonClick(TObject *Sender);
private:	// User declarations
        // Indexed by VOLUME_AY_A ... VOLUME_SPECDRUM, VOLUME_MAIN
        TFader *Fader[VOLUME_METERS];
        TLabel *Percent[VOLUME_METERS];
        TCheckBox *Mute[VOLUME_METERS];
        double Level[VOLUME_METERS];

        TGroupBox *MainGroup, *AYGroup, *BeeperGroup, *SpeechGroup, *SpecDrumGroup;
        TLabel *ChannelLabel[3];        // A, B, C: move with their faders
        TLabel *PlaceLabel[3];          // left, centre, right: stay
        TTimer *MeterTimer;
        bool Loading;
        TFader *WheelFader;             // the fader the wheel last moved
        int WheelRest;                  // and what is left of the turns

        void CreateMixer(void);
        TGroupBox *CreateSection(int left, int columns, const char *caption);
        void CreateColumn(TGroupBox *section, int column, int index, bool scale);
        void PlaceAYChannels(void);
        void ShowPercent(int index);
        void ApplySettings(void);
        void __fastcall FaderChange(TObject *Sender);
        void __fastcall MuteClick(TObject *Sender);
        void __fastcall MeterTimerTimer(TObject *Sender);

protected:
        DYNAMIC void __fastcall MouseWheelHandler(Messages::TMessage &Message);

public:		// User declarations
        __fastcall TMidiForm(TComponent* Owner);
        void SaveSettings(TIniFile *ini);
        void LoadSettings(TIniFile *ini);
        void SetComputer(bool zx81);
};
//---------------------------------------------------------------------------
extern PACKAGE TMidiForm *MidiForm;
//---------------------------------------------------------------------------
#endif
