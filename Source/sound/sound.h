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

// The beeper is band limited: each edge is a step filtered to below half
// the sample rate (a windowed sinc, integrated), BLEP_HALF samples either
// side of it, from a table with BLEP_PHASES points a sample. So that the
// samples before an edge need not be changed after they have been mixed,
// the beeper is BLEP_HALF samples late (about 0.35 ms).
#define BLEP_HALF               16
#define BLEP_PHASES             64
#define BLEP_POINTS             (2*BLEP_HALF*BLEP_PHASES + 1)
#define BLEP_CUTOFF             0.45    /* of the sample rate, -6 dB */
#define BLEP_KAISER_BETA        9.0     /* about -90 dB */


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
        unsigned short ofs;             // the sample of the buffer...
        unsigned short sub;             // ...and where in it (1/65536ths)
        unsigned char reg,val;
};

struct SpecDrumChangeTag
{
        unsigned short ofs;
        unsigned char val;
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
        void BeeperEdge(int on, double pos);
        void BeeperFill(int upto);
        double BeeperFade(double level, double samples);
        static void BlepInit(void);
        static double BlepStepAt(double x);
        void BlepAdd(double *buffer, double at, double jump);
        int SampleOffset(int frametstates);
        double SamplePosition(int frametstates);

        void AYApplyChange(int reg);
        void AYEnvCompute(void);
        void AYEmit(double at);

        int SelectAYReg;
        int FrameSize;
        int FramesPerSecond;

        double AYToneLevels[16];
        int *Mix;                       // the sources added up
        short *Buffer;                  // the output, after the main volume

        // The beeper, BLEP_HALF samples late, in AMPL_BEEPER units: this
        // frame's samples, the next frame's (edges may fall beyond the end
        // of a frame), and what the last edges add after that. The level
        // jumps to +-AMPL_BEEPER at an edge, then fades linearly to 0 in
        // 1/150 s (see Frame()).
        float *BeeperOut;
        int BeeperLen;
        int BeeperFilled;               // samples with the level in them
        int BeeperTarget;               // the last level asked for
        double BeeperLevel;             // the level from the last edge...
        double BeeperLevelTime;         // ...which was here (in samples)
        double BeeperSlope;             // the fade, a sample

        // The band limited step, from 0 to 1, x from -BLEP_HALF to
        // BLEP_HALF (one more point, to interpolate the last)
        static float BlepStep[BLEP_POINTS + 1];

        // The emulator runs a frame until the end of a scanline, so a frame
        // is a little longer or shorter than tperframe (the next one makes up
        // for it). The sound of a frame fills exactly one buffer, so a frame
        // starts OffsetTStates into its buffer; what falls after the end of
        // the buffer goes into the next one.
        int OffsetTStates;

        // The AY, run in continuous time (see AYOverlay()). Ticks and
        // periods are in AY clock cycles; the envelope's tick and period
        // count as the chip does.
        int AYClock;                    // 0: no AY
        double AYCyclesPerSample;
        double AYToneTick[3];           // 0 ... twice the half period
        double AYToneHalf[3];           // half a period: 8 * the period set
        double AYNoiseTick, AYNoisePeriod;
        double AYEnvSubCycles;          // the envelope steps every 256
        unsigned int AYEnvTick, AYEnvPeriod;
        int EnvHeld,EnvAlternating;
        int AYEnvStep;                  // 0-15, as last worked out
        double AYEnvLevel;
        int AYRng, AYNoiseToggle;

        // What the AY gives out (left, right), and the band limited steps
        // of its changes (BLEP_HALF samples late like the beeper): the
        // differences, added up as they are mixed
        double AYOut[2];
        double AYAcc[2];
        double *AYDelta;                // two of AYDeltaLen
        int AYDeltaLen;

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
