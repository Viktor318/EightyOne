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
#include <windows.h>
#include <mmsystem.h>
#include <math.h>
#pragma hdrstop

#include "sound\sound.h"
#include "sound\midi.h"
#include "sound\SoundForm.h"
#include "zx81config.h"
#include "main_.h"
#include "Language.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMidiForm *MidiForm;

// The fader
#define FADER_HEIGHT    200
#define FADER_PAD       7       // half the thumb, above and below the track
#define FADER_SCALE_W   22
#define FADER_TRACK_W   22
#define FADER_GAP       3
#define FADER_METER_W   8
#define METER_SEGMENTS  24
#define METER_RANGE_DB  40.0    // the meter shows -40 dB ... 0 dB
#define METER_FALL      0.025   // how much it falls in a tick (1 dB)
#define METER_TICK_MS   40

// The mixer: sections (group boxes) of columns
#define COLUMN_W        70
#define SECTION_PAD     8
#define SECTION_GAP     6
#define SECTIONS_LEFT   12
#define SECTIONS_TOP    50
#define ROW_CHANNEL     18
#define ROW_PLACE       34
#define ROW_FADER       52
#define ROW_PERCENT     (ROW_FADER + FADER_HEIGHT + 4)
#define ROW_MUTE        (ROW_PERCENT + 18)
#define SECTION_H       (ROW_MUTE + 24)

// The settings (in the MIDI section, as before), and the old ones: 0-31
// sliders upside down, one mute for the whole AY, and ACB or mono
static const char *VolumeKeys[VOLUME_METERS] =
        { "AYA", "AYB", "AYC", "Beeper", "Speech", "SpecDrum", "Main" };
static const char *OldVolumeKeys[VOLUME_SOURCES] =
        { "ChAVol", "ChBVol", "ChCVol", "BeeperVol", "SpeechVol", "SpecDrumVol" };
static const char *OldMuteKeys[VOLUME_SOURCES] =
        { "AYMute", "AYMute", "AYMute", "BeeperMute", "SpeechMute", "SpecDrumMute" };
#define OLD_VOLUME_MAX  31

//---------------------------------------------------------------------------
// TFader

__fastcall TFader::TFader(Classes::TComponent *Owner, bool scale)
        : TCustomControl(Owner)
{
        ControlStyle = ControlStyle << csCaptureMouse << csClickEvents
                                    << csDoubleClicks << csOpaque;
        FPosition = VOLUME_DEFAULT;
        FDefault = VOLUME_DEFAULT;
        FScale = scale;
        FMuted = false;
        FDragging = false;
        FDragOffset = 0;
        FLevel = 0;
        FLit = 0;
        OnChange = NULL;

        ParentColor = true;
        TabStop = true;
        Width = WidthFor(scale);
        Height = FADER_HEIGHT;

        FBubble = new THintWindow(this);
        FBubble->Color = clInfoBk;
}

int TFader::WidthFor(bool scale)
{
        return (scale ? FADER_SCALE_W : 0) + FADER_TRACK_W + FADER_GAP
                + FADER_METER_W + 1;
}

int TFader::Top_(void)
{
        return FADER_PAD;
}

int TFader::Bottom_(void)
{
        return ClientHeight - 1 - FADER_PAD;
}

int TFader::TrackX(void)
{
        return (FScale ? FADER_SCALE_W : 0) + FADER_TRACK_W / 2;
}

int TFader::YOf(int value)
{
        int range = Bottom_() - Top_();

        return Top_() + ((VOLUME_MAX - value) * range + VOLUME_MAX / 2) / VOLUME_MAX;
}

int TFader::ValueAt(int y)
{
        int range = Bottom_() - Top_();

        if (y <= Top_()) return VOLUME_MAX;
        if (y >= Bottom_()) return 0;

        return VOLUME_MAX - ((y - Top_()) * VOLUME_MAX + range / 2) / range;
}

void TFader::SetPosition(int value)
{
        if (value < 0) value = 0;
        if (value > VOLUME_MAX) value = VOLUME_MAX;
        if (value == FPosition) return;

        FPosition = value;
        Invalidate();
        if (FDragging) ShowBubble();
        if (OnChange) OnChange(this);
}

void TFader::SetMuted(bool muted)
{
        if (muted == FMuted) return;

        FMuted = muted;
        Invalidate();
}

void TFader::SetLevel(double level)
{
        int lit;

        FLevel = level;
        lit = FMuted ? 0 : (int)(level * METER_SEGMENTS + 0.5);
        if (lit == FLit) return;

        FLit = lit;
        Invalidate();
}

void TFader::Wheel(int steps, bool big)
{
        SetPosition(FPosition + steps * (big ? 10 : 1));
}

// The percentage in a small window above the thumb, while dragging

void TFader::ShowBubble(void)
{
        AnsiString text = IntToStr(FPosition) + "%";
        TRect size = FBubble->CalcHintRect(200, "100%", NULL);
        int w = size.Right - size.Left;
        int h = size.Bottom - size.Top;
        TPoint p = ClientToScreen(Point(TrackX(), YOf(FPosition) - FADER_PAD - 2));
        TRect r = Rect(p.x - w / 2, p.y - h, p.x - w / 2 + w, p.y);

        if (FBubble->HandleAllocated() && IsWindowVisible(FBubble->Handle))
        {
                FBubble->Caption = text;
                SetWindowPos(FBubble->Handle, HWND_TOPMOST, r.Left, r.Top,
                                w, h, SWP_NOACTIVATE);
                FBubble->Invalidate();
        }
        else
        {
                FBubble->ActivateHint(r, text);
        }
}

void TFader::HideBubble(void)
{
        if (FBubble->HandleAllocated()) FBubble->ReleaseHandle();
}

void __fastcall TFader::Paint(void)
{
        static const COLORREF on[3] =
                { RGB(54, 194, 74), RGB(240, 196, 25), RGB(226, 59, 46) };
        static const COLORREF off[3] =
                { RGB(36, 64, 42), RGB(74, 66, 32), RGB(74, 36, 32) };

        Graphics::TBitmap *bmp = new Graphics::TBitmap();

        try
        {
                int w = ClientWidth, h = ClientHeight;
                int top = Top_(), bottom = Bottom_();
                int cx = TrackX();
                int ty = YOf(FPosition);
                int mx = (FScale ? FADER_SCALE_W : 0) + FADER_TRACK_W + FADER_GAP;
                int i, p;
                TCanvas *c;
                TRect r;

                bmp->Width = w;
                bmp->Height = h;
                c = bmp->Canvas;

                c->Brush->Color = Color;
                c->FillRect(Rect(0, 0, w, h));

                // The ticks, and the scale

                c->Font->Assign(Font);
                c->Font->Size = 7;
                c->Font->Color = (TColor)RGB(110, 110, 110);
                c->Pen->Color = (TColor)RGB(190, 190, 190);

                for (p=0; p<=VOLUME_MAX; p+=VOLUME_MAX/4)
                {
                        int y = YOf(p);

                        c->MoveTo(cx - 8, y);
                        c->LineTo(cx + 9, y);

                        if (FScale)
                        {
                                AnsiString text = IntToStr(p);

                                c->Brush->Style = bsClear;
                                c->TextOut(FADER_SCALE_W - 3 - c->TextWidth(text),
                                           y - c->TextHeight(text) / 2, text);
                                c->Brush->Style = bsSolid;
                        }
                }

                // The track, filled up to the thumb

                c->Brush->Color = (TColor)RGB(185, 185, 185);
                c->FillRect(Rect(cx - 2, top, cx + 2, bottom + 1));
                c->Brush->Color = FMuted ? (TColor)RGB(150, 150, 150)
                                         : (TColor)RGB(47, 127, 211);
                c->FillRect(Rect(cx - 2, ty, cx + 2, bottom + 1));

                // The level meter: green, yellow, red

                for (i=0; i<METER_SEGMENTS; i++)
                {
                        int y1 = bottom + 1 - (i * (bottom + 1 - top)) / METER_SEGMENTS;
                        int y0 = bottom + 1 - ((i + 1) * (bottom + 1 - top)) / METER_SEGMENTS + 1;
                        int zone = i < METER_SEGMENTS * 2 / 3 ? 0
                                 : i < METER_SEGMENTS * 7 / 8 ? 1 : 2;

                        c->Brush->Color = (TColor)(i < FLit ? on[zone] : off[zone]);
                        c->FillRect(Rect(mx, y0, mx + FADER_METER_W, y1));
                }

                // The thumb (blue frame when focused)

                r = Rect(cx - 10, ty - 6, cx + 11, ty + 7);
                c->Brush->Color = (TColor)RGB(232, 232, 232);
                c->FillRect(r);
                c->Brush->Color = (TColor)RGB(250, 250, 250);
                c->FillRect(Rect(r.Left + 1, r.Top + 1, r.Right - 1, ty));

                c->Brush->Color = Focused() ? (TColor)RGB(0, 120, 215)
                                            : (TColor)RGB(100, 100, 100);
                c->FrameRect(r);
                if (Focused())
                        c->FrameRect(Rect(r.Left + 1, r.Top + 1, r.Right - 1, r.Bottom - 1));

                c->Pen->Color = (TColor)RGB(60, 60, 60);
                c->MoveTo(r.Left + 4, ty);
                c->LineTo(r.Right - 4, ty);

                Canvas->Draw(0, 0, bmp);
        }
        __finally
        {
                delete bmp;
        }
}

void __fastcall TFader::MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y)
{
        int ty;

        TCustomControl::MouseDown(Button, Shift, X, Y);

        // The second press of a double click: DblClick() has done it
        if (Button != mbLeft || Shift.Contains(ssDouble)) return;

        if (CanFocus()) SetFocus();

        // On the thumb: drag it from where it was taken; elsewhere on the
        // track: jump there, and drag from there
        ty = YOf(FPosition);
        if (Y >= ty - FADER_PAD && Y <= ty + FADER_PAD)
        {
                FDragOffset = Y - ty;
        }
        else
        {
                FDragOffset = 0;
                SetPosition(ValueAt(Y));
        }

        FDragging = true;
        ShowBubble();
}

void __fastcall TFader::MouseMove(Classes::TShiftState Shift, int X, int Y)
{
        TCustomControl::MouseMove(Shift, X, Y);

        if (FDragging) SetPosition(ValueAt(Y - FDragOffset));
}

void __fastcall TFader::MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y)
{
        TCustomControl::MouseUp(Button, Shift, X, Y);

        if (Button == mbLeft && FDragging)
        {
                FDragging = false;
                HideBubble();
        }
}

void __fastcall TFader::DblClick(void)
{
        TCustomControl::DblClick();

        FDragging = false;
        HideBubble();
        Reset();
}

void __fastcall TFader::KeyDown(Word &Key, Classes::TShiftState Shift)
{
        int step = Shift.Contains(ssShift) ? 10 : 1;

        TCustomControl::KeyDown(Key, Shift);

        switch (Key)
        {
        case VK_UP: case VK_RIGHT:      SetPosition(FPosition + step); break;
        case VK_DOWN: case VK_LEFT:     SetPosition(FPosition - step); break;
        case VK_PRIOR:                  SetPosition(FPosition + 10); break;
        case VK_NEXT:                   SetPosition(FPosition - 10); break;
        case VK_HOME:                   SetPosition(VOLUME_MAX); break;
        case VK_END:                    SetPosition(0); break;
        default:                        return;
        }

        Key = 0;
}

void __fastcall TFader::DoEnter(void)
{
        TCustomControl::DoEnter();
        Invalidate();
}

void __fastcall TFader::DoExit(void)
{
        TCustomControl::DoExit();
        FDragging = false;
        HideBubble();
        Invalidate();
}

// Arrow keys are for the fader, not for moving to the next control

void __fastcall TFader::WMGetDlgCode(TMessage &Message)
{
        Message.Result = DLGC_WANTARROWS;
}

// Paint() covers everything: no erasing first, so no flicker

void __fastcall TFader::WMEraseBkgnd(TWMEraseBkgnd &Message)
{
        Message.Result = 1;
}

//---------------------------------------------------------------------------
// TMidiForm

__fastcall TMidiForm::TMidiForm(TComponent* Owner)
        : TForm(Owner)
{
        int i,Devices;
        MIDIOUTCAPS moc;
        TIniFile *ini;

        Loading = true;

        Devices=midiOutGetNumDevs();

        while(MidiBox->Items->Count) MidiBox->Items->Delete(0);

        MidiBox->Items->Add(Tr("Nothing"));
        MidiBox->Items->Add("Midi Mapper");

        for(i=0;i<Devices;i++)
        {
                if (!midiOutGetDevCaps(i, &moc, sizeof(MIDIOUTCAPS)))
                        MidiBox->Items->Add(moc.szPname);
        }

        MidiBox->ItemIndex=1;
        Midi.Start();

        StereoBox->Items->Clear();
        for (i=0; i<AY_STEREO_MODES; i++)
                StereoBox->Items->Add(AYStereoNames[i]);
        StereoBox->ItemIndex = AY_STEREO_MONO;

        CreateMixer();
        PlaceAYChannels();

        Loading = false;

        ini = new TIniFile(emulator.inipath);
        LoadSettings(ini);
        delete ini;
}

// The mixer: main volume, AY (3 channels), beeper, speech, SpecDrum

void TMidiForm::CreateMixer(void)
{
        static const char *placeNames[3] = { "left", "centre", "right" };
        int i, x = SECTIONS_LEFT;

        for (i=0; i<VOLUME_METERS; i++)
        {
                Level[i] = 0;
                Fader[i] = NULL;
                Percent[i] = NULL;
                Mute[i] = NULL;
        }
        for (i=0; i<3; i++) ChannelLabel[i] = PlaceLabel[i] = NULL;
        WheelFader = NULL;
        WheelRest = 0;

        MainGroup = CreateSection(x, 1, "Main volume");
        CreateColumn(MainGroup, 0, VOLUME_MAIN, true);
        x += MainGroup->Width + SECTION_GAP;

        AYGroup = CreateSection(x, 3, "AY");
        for (i=0; i<3; i++)
        {
                CreateColumn(AYGroup, i, VOLUME_AY_A + i, false);

                ChannelLabel[i] = new TLabel(this);
                ChannelLabel[i]->Parent = AYGroup;
                ChannelLabel[i]->AutoSize = false;
                ChannelLabel[i]->Alignment = taCenter;
                ChannelLabel[i]->Font->Style = TFontStyles() << fsBold;
                ChannelLabel[i]->SetBounds(SECTION_PAD + i * COLUMN_W, ROW_CHANNEL, COLUMN_W, 16);
                ChannelLabel[i]->Caption = AnsiString((char)('A' + i));

                PlaceLabel[i] = new TLabel(this);
                PlaceLabel[i]->Parent = AYGroup;
                PlaceLabel[i]->AutoSize = false;
                PlaceLabel[i]->Alignment = taCenter;
                PlaceLabel[i]->Font->Color = clGrayText;
                PlaceLabel[i]->SetBounds(SECTION_PAD + i * COLUMN_W, ROW_PLACE, COLUMN_W, 16);
                PlaceLabel[i]->Caption = Tr(placeNames[i]);
        }
        x += AYGroup->Width + SECTION_GAP;

        BeeperGroup = CreateSection(x, 1, "Beeper");
        CreateColumn(BeeperGroup, 0, VOLUME_BEEPER, false);
        x += BeeperGroup->Width + SECTION_GAP;

        SpeechGroup = CreateSection(x, 1, "Speech");
        CreateColumn(SpeechGroup, 0, VOLUME_SPEECH, false);
        x += SpeechGroup->Width + SECTION_GAP;

        SpecDrumGroup = CreateSection(x, 1, "SpecDrum");
        CreateColumn(SpecDrumGroup, 0, VOLUME_SPECDRUM, false);

        // Beside the OK button: the output (it may change while playing)
        OutputLabel = new TLabel(this);
        OutputLabel->Parent = this;
        OutputLabel->AutoSize = false;
        OutputLabel->SetBounds(GroupBox1->Left + 4, OK->Top + 5, OK->Left - GroupBox1->Left - 12, 16);
        LanguageKeepText(OutputLabel);
        ShowOutput();

        MeterTimer = new TTimer(this);
        MeterTimer->Interval = METER_TICK_MS;
        MeterTimer->OnTimer = MeterTimerTimer;
        MeterTimer->Enabled = true;
}

void TMidiForm::ShowOutput(void)
{
        const char *name = Sound.OutputName();
        AnsiString text;

        if (name)
                text = TrF("Output: {0}, {1} Hz", name, IntToStr(Sound.OutputRate()));
        else
                text = Tr("Output: no sound device");

        if (OutputLabel->Caption != text) OutputLabel->Caption = text;
}

TGroupBox *TMidiForm::CreateSection(int left, int columns, const char *caption)
{
        TGroupBox *section = new TGroupBox(this);

        section->Parent = GroupBox2;
        section->SetBounds(left, SECTIONS_TOP, SECTION_PAD * 2 + columns * COLUMN_W, SECTION_H);
        section->Caption = Tr(caption);
        return section;
}

void TMidiForm::CreateColumn(TGroupBox *section, int column, int index, bool scale)
{
        int x = SECTION_PAD + column * COLUMN_W;
        TFader *fader = new TFader(this, scale);
        TLabel *percent = new TLabel(this);
        TCheckBox *mute = new TCheckBox(this);

        fader->Parent = section;
        fader->Tag = index;
        fader->SetBounds(x + (COLUMN_W - fader->Width) / 2, ROW_FADER,
                         fader->Width, FADER_HEIGHT);
        fader->SetDefault(index == VOLUME_MAIN ? VOLUME_MAX : VOLUME_DEFAULT);
        fader->Reset();
        fader->OnChange = FaderChange;

        percent->Parent = section;
        percent->Tag = index;
        percent->AutoSize = false;
        percent->Alignment = taCenter;
        percent->SetBounds(x, ROW_PERCENT, COLUMN_W, 16);

        mute->Parent = section;
        mute->Tag = index;
        mute->SetBounds(x + 8, ROW_MUTE, COLUMN_W - 10, 18);
        mute->Caption = Tr("Mute");
        mute->OnClick = MuteClick;

        Fader[index] = fader;
        Percent[index] = percent;
        Mute[index] = mute;

        ShowPercent(index);
}

// The AY channels in the order of the stereo mode: left, centre, right

void TMidiForm::PlaceAYChannels(void)
{
        static const char *placeNames[3] = { "left", "centre", "right" };
        int mode = StereoBox->ItemIndex;
        int column, channel, x;

        if (mode < 0 || mode >= AY_STEREO_MODES) mode = AY_STEREO_MONO;

        for (column=0; column<3; column++)
        {
                channel = mode == AY_STEREO_MONO ? column : AYStereoPlaces[mode][column];
                x = SECTION_PAD + column * COLUMN_W;

                Fader[channel]->Left = x + (COLUMN_W - Fader[channel]->Width) / 2;
                Percent[channel]->Left = x;
                Mute[channel]->Left = x + 8;
                ChannelLabel[channel]->Left = x;

                Fader[channel]->TabOrder = (TTabOrder)(column * 2);
                Mute[channel]->TabOrder = (TTabOrder)(column * 2 + 1);

                PlaceLabel[column]->Caption = mode == AY_STEREO_MONO ?
                        AnsiString("") : Tr(placeNames[column]);
        }
}

void TMidiForm::ShowPercent(int index)
{
        bool muted = Mute[index]->Checked;
        TColor colour = muted ? clGrayText : clWindowText;

        Percent[index]->Caption = IntToStr(Fader[index]->Position()) + "%";
        Percent[index]->Font->Color = colour;
        Fader[index]->SetMuted(muted);

        if (index < 3 && ChannelLabel[index])
                ChannelLabel[index]->Font->Color = colour;
}

void TMidiForm::ApplySettings(void)
{
        int i, mode;

        for (i=0; i<VOLUME_SOURCES; i++)
                Sound.VolumeLevel[i] = Mute[i]->Checked ? 0 : Fader[i]->Position();

        Sound.MainVolume = Mute[VOLUME_MAIN]->Checked ? 0 : Fader[VOLUME_MAIN]->Position();

        mode = StereoBox->ItemIndex;
        Sound.AYStereo = (mode >= 0 && mode < AY_STEREO_MODES) ? mode : AY_STEREO_MONO;
}

void __fastcall TMidiForm::FaderChange(TObject *Sender)
{
        TComponent *component = dynamic_cast<TComponent *>(Sender);

        if (!component) return;

        ShowPercent(component->Tag);
        if (!Loading) ApplySettings();
}

void __fastcall TMidiForm::MuteClick(TObject *Sender)
{
        TComponent *component = dynamic_cast<TComponent *>(Sender);

        if (!component) return;

        ShowPercent(component->Tag);
        if (!Loading) ApplySettings();
}

void __fastcall TMidiForm::StereoBoxChange(TObject *Sender)
{
        PlaceAYChannels();
        ApplySettings();
}

// Every volume back to its default, nothing muted (the stereo mode stays)

void __fastcall TMidiForm::DefaultsButtonClick(TObject *Sender)
{
        int i;

        for (i=0; i<VOLUME_METERS; i++)
        {
                Mute[i]->Checked = false;
                Fader[i]->Reset();
                ShowPercent(i);
        }

        ApplySettings();
}

// The level meters: the highest level since the last tick, in dB, falling
// slowly

void __fastcall TMidiForm::MeterTimerTimer(TObject *Sender)
{
        int i, peak;
        double level;

        if (!Visible) return;

        ShowOutput();

        for (i=0; i<VOLUME_METERS; i++)
        {
                peak = Sound.Peak[i];
                Sound.Peak[i] = 0;

                level = 0;
                if (peak > 0)
                {
                        level = 1.0 + 20.0 * log10((double)peak / CSound::PeakFullScale(i))
                                        / METER_RANGE_DB;
                        if (level < 0) level = 0;
                        if (level > 1) level = 1;
                }

                if (level < Level[i] - METER_FALL) level = Level[i] - METER_FALL;
                if (level < 0) level = 0;

                Level[i] = level;
                Fader[i]->SetLevel(level);
        }
}

// The mouse wheel moves the fader under the mouse, focused or not: a step
// for each notch (WHEEL_DELTA); smooth wheels and touchpads send smaller
// amounts, which are added up

void __fastcall TMidiForm::MouseWheelHandler(Messages::TMessage &Message)
{
        TFader *fader = dynamic_cast<TFader *>(FindVCLWindow(Mouse->CursorPos));

        if (fader && fader->Owner == this)
        {
                int steps;

                if (fader != WheelFader)
                {
                        WheelFader = fader;
                        WheelRest = 0;
                }

                WheelRest += (short)HIWORD(Message.WParam);
                steps = WheelRest / WHEEL_DELTA;
                WheelRest -= steps * WHEEL_DELTA;

                if (steps)
                        fader->Wheel(steps, (LOWORD(Message.WParam) & MK_SHIFT) != 0);
                Message.Result = 1;
                return;
        }

        TForm::MouseWheelHandler(Message);
}

//---------------------------------------------------------------------------
void __fastcall TMidiForm::OKClick(TObject *Sender)
{
        Form1->Midi1->Checked=false;
        MidiChange(NULL);
        Close();
}
//---------------------------------------------------------------------------
void __fastcall TMidiForm::FormClose(TObject *Sender, TCloseAction &Action)
{
        Form1->Midi1->Checked=false;
}
//---------------------------------------------------------------------------
void __fastcall TMidiForm::FormDestroy(TObject *Sender)
{
        if (MeterTimer) MeterTimer->Enabled = false;
        Midi.Stop();
}
//---------------------------------------------------------------------------
void __fastcall TMidiForm::MidiChange(TObject *Sender)
{
        static int LastMidiItem=0;

        ApplySettings();

        if (LastMidiItem!=MidiBox->ItemIndex)
        {
                Midi.Stop();
                Midi.Device = MidiBox->ItemIndex -2;
                Midi.Start();
                LastMidiItem=MidiBox->ItemIndex;
        }
}
//---------------------------------------------------------------------------
void TMidiForm::SaveSettings(TIniFile *ini)
{
        int i, mode;

        ini->WriteInteger("MIDI","Top",Top);
        ini->WriteInteger("MIDI","Left",Left);

        for (i=0; i<VOLUME_METERS; i++)
        {
                ini->WriteInteger("MIDI", AnsiString("Volume") + VolumeKeys[i], Fader[i]->Position());
                ini->WriteBool("MIDI", AnsiString("Mute") + VolumeKeys[i], Mute[i]->Checked);
        }

        mode = StereoBox->ItemIndex;
        if (mode < 0 || mode >= AY_STEREO_MODES) mode = AY_STEREO_MONO;
        ini->WriteString("MIDI","AYStereo",AYStereoNames[mode]);

        ini->WriteInteger("MIDI","MidiDev",MidiBox->ItemIndex);
        ini->WriteBool("MIDI","BeeperExcludeHSyncs",BeeperExcludeHSyncs->Checked);
}

void TMidiForm::LoadSettings(TIniFile *ini)
{
        int i, value, mode;
        bool muted;
        AnsiString key, name;

        Loading = true;

        Top=ini->ReadInteger("MIDI","Top",Top);
        Left=ini->ReadInteger("MIDI","Left",Left);

        for (i=0; i<VOLUME_METERS; i++)
        {
                // The volume: new setting, or the old slider made a percentage
                key = AnsiString("Volume") + VolumeKeys[i];
                value = i == VOLUME_MAIN ? VOLUME_MAX : VOLUME_DEFAULT;

                if (ini->ValueExists("MIDI", key))
                {
                        value = ini->ReadInteger("MIDI", key, value);
                }
                else if (i < VOLUME_SOURCES && ini->ValueExists("MIDI", OldVolumeKeys[i]))
                {
                        int old = ini->ReadInteger("MIDI", OldVolumeKeys[i], 16);

                        if (old < 0) old = 0;
                        if (old > OLD_VOLUME_MAX) old = OLD_VOLUME_MAX;
                        value = ((OLD_VOLUME_MAX - old) * VOLUME_MAX + OLD_VOLUME_MAX / 2)
                                / OLD_VOLUME_MAX;
                }

                Fader[i]->SetPosition(value);

                // Muted: likewise
                key = AnsiString("Mute") + VolumeKeys[i];
                muted = false;

                if (ini->ValueExists("MIDI", key))
                        muted = ini->ReadBool("MIDI", key, false);
                else if (i < VOLUME_SOURCES)
                        muted = ini->ReadBool("MIDI", OldMuteKeys[i], false);

                Mute[i]->Checked = muted;
                ShowPercent(i);
        }

        // The stereo mode: by name, or the old ACB / mono setting
        name = ini->ReadString("MIDI","AYStereo","");
        mode = -1;
        for (i=0; i<AY_STEREO_MODES; i++)
                if (name == AYStereoNames[i]) mode = i;
        if (mode < 0)
                mode = ini->ReadBool("MIDI","AYMixACB",false) ? AY_STEREO_ACB : AY_STEREO_MONO;
        StereoBox->ItemIndex = mode;
        PlaceAYChannels();

        MidiBox->ItemIndex=ini->ReadInteger("MIDI","MidiDev",MidiBox->ItemIndex);
        BeeperExcludeHSyncs->Checked=ini->ReadBool("MIDI","BeeperExcludeHSyncs",BeeperExcludeHSyncs->Checked);

        zx81.beeperExcludeHSyncs = BeeperExcludeHSyncs->Checked;

        Loading = false;

        OKClick(NULL);
}

void __fastcall TMidiForm::BeeperExcludeHSyncsClick(TObject *Sender)
{
        zx81.beeperExcludeHSyncs = BeeperExcludeHSyncs->Checked;
}
//---------------------------------------------------------------------------

void TMidiForm::SetComputer(bool zx81)
{
        if (zx81)
        {
                BeeperGroup->Caption = Tr("Video");
                VideoSoundGroupBox->Enabled = true;
                BeeperExcludeHSyncs->Enabled = true;
        }
        else
        {
                BeeperGroup->Caption = "Beeper";
                VideoSoundGroupBox->Enabled = false;
                BeeperExcludeHSyncs->Enabled = false;
        }
}
