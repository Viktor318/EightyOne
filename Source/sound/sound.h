/* z81/xz81, Linux console and X ZX81/ZX80 emulators.
 * Copyright (C) 1994 Ian Collier. z81 changes (C) 1995-2001 Russell Marks.
 * Modifications/Changes (C) 2021 Michael D Wynne, EightyOne 
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
 *
 *
 * sound.h - sound support, based on the beeper/AY code I
 *           wrote for Fuse.
 */

#ifndef _SOUND_H
#define _SOUND_H

#include "sound\SoundDX.h"
#include "sound\SoundWASAPI.h"

#define AY_TYPE_NONE                    0
#define AY_TYPE_QUICKSILVA              1
#define AY_TYPE_ZONX                    2
#define AY_TYPE_FULLER                  3
#define AY_TYPE_ACE_USER                4
#define AY_TYPE_SINCLAIR                5
#define AY_TYPE_TS2068                  6
#define AY_TYPE_TC2068                  7
#define AY_TYPE_BOLDFIELD               8
#define AY_TYPE_DKTRONICS               9

#define AY_CLOCK_QUICKSILVA	        (3250000/4)
#define AY_CLOCK_ZONX_ZX81              1625000
#define AY_CLOCK_ZONX_SPEC48            1750000
#define AY_CLOCK_ZONX_SPEC128           1773400
#define AY_CLOCK_FULLER                 1638190
#define AY_CLOCK_ACE_USER               1625000
#define AY_CLOCK_SINCLAIR_128K          1773400
#define AY_CLOCK_SINCLAIR_48K           1750000
#define AY_CLOCK_TS2068                 1764000
#define AY_CLOCK_TC2068                 1750000
#define AY_CLOCK_BOLDFIELD              1773400
#define AY_CLOCK_DKTRONICS_128K         1773400
#define AY_CLOCK_DKTRONICS_48K          1750000

#define SPEECH_TYPE_NONE                0
#define SPEECH_TYPE_USPEECH             1
#define SPEECH_TYPE_PARROT              2
#define SPEECH_TYPE_SWEETTALKER         3
#define SPEECH_TYPE_MAGECO              4
#define SPEECH_TYPE_TALKBACK            5
#define SPEECH_TYPE_DKTRONICS           6
#define SPEECH_TYPE_ORATOR              7
#define SPEECH_TYPE_DIGITALKER          8

// assume all three tone channels together match the beeper volume.

// Volumes are percentages: at VOLUME_MAX a source is as loud as it can be,
// as it was with the old 0-31 sliders at 31. The sources are mixed with
// more headroom than 16 bits, and the result is clipped after the main
// volume.

#define VOLUME_MAX              100
#define VOLUME_DEFAULT          50
#define AMPL_BEEPER             31
#define AMPL_AY_TONE            (31*256) /* three of these */
#define ADJUSTTO16BIT           256

// max. number of sub-frame AY port writes allowed;
// given the number of port writes theoretically possible in a
// 50th I think this should be plenty.

#define AY_CHANGE_MAX		8000

// Measured an approximate sampling rate from the SpecDrum software of 22kHz.
// At 50fps this is about 440 samples, which I doubled and rounded up to get this. 
#define SPECDRUM_BUFFSIZE       1000

// Beeper edges that fall after the end of the frame's buffer, kept for the
// next one
#define BEEPER_PENDING_MAX      64


// The volumes, mutes and level meters of the sound settings window
#define VOLUME_AY_A             0
#define VOLUME_AY_B             1
#define VOLUME_AY_C             2
#define VOLUME_BEEPER           3
#define VOLUME_SPEECH           4
#define VOLUME_SPECDRUM         5
#define VOLUME_SOURCES          6
#define VOLUME_MAIN             6       // level meter only
#define VOLUME_METERS           7

// AY stereo modes: the channels on the left, in the centre and on the
// right (the centre one is heard on both sides, a little quieter)
#define AY_STEREO_MONO          0
#define AY_STEREO_ABC           1
#define AY_STEREO_ACB           2
#define AY_STEREO_BAC           3
#define AY_STEREO_BCA           4
#define AY_STEREO_CAB           5
#define AY_STEREO_CBA           6
#define AY_STEREO_MODES         7

// The names of the modes ("Mono", "ABC"...), also kept in the settings
extern const char *AYStereoNames[AY_STEREO_MODES];

// The output chosen: Automatic tries WASAPI, then DirectSound. Whichever is
// chosen, if it cannot be opened the emulator is kept running by a timer
// (with WASAPI or Automatic a device is looked for again every second).
#define SOUND_OUTPUT_AUTO               0
#define SOUND_OUTPUT_WASAPI             1
#define SOUND_OUTPUT_DIRECTSOUND        2
#define SOUND_OUTPUTS                   3

// Their names ("Automatic", "WASAPI", "DirectSound"), also kept in the
// settings
extern const char *SoundOutputNames[SOUND_OUTPUTS];

// The channel (0-2 for A-C) on the left, in the centre and on the right
extern const int AYStereoPlaces[AY_STEREO_MODES][3];

struct AYChangeTag
{
        unsigned short ofs;
        unsigned char reg,val;
};

struct SpecDrumChangeTag
{
        unsigned short ofs;
        unsigned char val;
};

struct BeeperEdgeTag
{
        int on;
        int pos, subpos;
};

class CSound
{
public:
        CSound();
	void InitDevices(void);
        int Initialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels);
        int ReInitialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels);

        void End(void);
        bool Stopping(void) { return Output ? Output->Stopping() : false; }

        // What is playing the sound ("WASAPI", "DirectSound"), or NULL if
        // nothing is; and at what rate
        const char *OutputName(void) { return Output ? Output->Name() : NULL; }
        int OutputRate(void) { return Output ? m_SampleRate : 0; }

        // SOUND_OUTPUT_...: used from the next Initialise(), or at once if
        // the sound is playing (the sound devices are not reset)
        int OutputChoice;
        int SetOutputChoice(int choice);
        static int OutputChoiceOf(const char *name);
        void AYWrite(int reg,int val, int frametstates);
        void AYWrite128(int reg, int val, int frametstates);
        void AYWriteTimex(int reg, int val, int frametstates);
        int AYRead(int reg);
        int AYRead128(int reg);
        int AYReadTimex(int reg, int joysticks);
        void AYReset(void);
        void SpecDrumWrite(BYTE data, int frametstates);
        void Frame(bool pause);
        void Beeper(int on, int frametstates);

        // Percentages (0 when muted): VOLUME_AY_A ... VOLUME_SPECDRUM
        int VolumeLevel[VOLUME_SOURCES];
        int MainVolume;
        int AYStereo;                   // AY_STEREO_...

        // The highest level of each source after its volume, and of the
        // output after the main volume, since the window last looked
        // (it sets them to 0); and what is full scale for each
        int Peak[VOLUME_METERS];
        static int PeakFullScale(int meter);
        unsigned char AYRegisterStore[16];

private:
        // WASAPI if it works, otherwise DirectSound, otherwise WASAPI
        // without a device (a timer keeps the emulator running)
        CWasapiSnd WasapiSound;
        CDSnd DXSound;
        CSoundOutput *Output;
        int OpenOutput(void);
        int StartOutput(bool resetDevices);

        HWND m_hWnd;
        int m_BitsPerSample;
        int m_BytesPerSample;
        int m_SampleRate;               // the output's
        int m_RequestedRate;            // the one asked for
        int m_Channels;
        int m_FPS;
        double m_SamplesPerTState;

        void AYInit(void);
        void AYOverlay(void);
        void SpeechOverlay(void);
        void SpecDrumInit(void);
        void SpecDrumOverlay(void);
        void DigiTalkOverlay(void);
        void BeeperEdge(int on, int newpos, int subpos);
        int SampleOffset(int frametstates);

        int SelectAYReg;
        int FrameSize;
        int FramesPerSecond;

        double AYToneLevels[16];
        int *Mix;                       // the sources added up
        short *Buffer;                  // the output, after the main volume
        int OldPos,FillPos,OldVal,OldValOrig;

        // The emulator runs a frame until the end of a scanline, so a frame
        // is a little longer or shorter than tperframe (the next one makes up
        // for it). The sound of a frame fills exactly one buffer, so a frame
        // starts OffsetTStates into its buffer; what falls after the end of
        // the buffer goes into the next one.
        int OffsetTStates;
        int BeeperRequested;            // the last level asked for
        struct BeeperEdgeTag BeeperPending[BEEPER_PENDING_MAX];
        int BeeperPendingCount;

	// timer used for fadeout after beeper-toggle;
	// fixed-point with low 24 bits as fractional part.

        unsigned int BeeperTick,BeeperTickIncr;

	// tick/incr/periods are all fixed-point with low 16 bits as
	// fractional part, except ay_env_{tick,period} which count as the chip does.

        unsigned int AYToneTick[3],AYNoiseTick;
        unsigned int AYEnvTick,AYEnvSubCycles;
        unsigned int AYTickIncr;
        int AYClock;                    // 0: no AY
        unsigned int AYTonePeriod[3],AYNoisePeriod,AYEnvPeriod;

        int EnvHeld,EnvAlternating;
        int BeeperLastSubpos;

	// AY registers
	// we have 16 so we can fake an 8910 if needed

        unsigned char AYRegisters[16];
        struct AYChangeTag AYChange[AY_CHANGE_MAX];
        int AYChangeCount;

        // SpecDrum buffer
        struct SpecDrumChangeTag SpecDrumChange[SPECDRUM_BUFFSIZE];
        int SpecDrumChangeCount;
        int SpecDrumLevel;

};

extern CSound Sound;

#endif
