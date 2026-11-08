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
 * sound.c - sound support, based on the beeper/AY code I
 *           wrote for Fuse.
 */

/* ZX81 sound routines seem to be based on using VSYNC; in Basic
 * you might do this by alternating FAST/SLOW mode quickly,
 * for example. So that's what's emulated here. Obviously this
 * won't be perfect, not least as it won't be as noisy... :-)
 */

/* some AY details (volume levels, white noise RNG algorithm) based on
 * info from MAME's ay8910.c - MAME's licence explicitly permits free
 * use of info (even encourages it).
 */

/* NB: I know some of this stuff looks fairly CPU-hogging.
 * For example, the AY code tracks changes with sub-frame timing
 * in a rather hairy way, and there's subsampling here and there.
 * But if you measure the CPU use, it doesn't actually seem
 * very high at all. And I speak as a Cyrix owner. :-)
 *
 * (I based that on testing in Fuse, but I doubt it's that much
 * worse in z81. Though in both, the AY code does cause cache oddities
 * on my machine, so I get the bizarre situation of z81 jumping
 * between <=2% CPU use and *30*% CPU use pretty much at random...)
 */

#pragma warn -8071

#include <string.h>
#include <math.h>

#include "sound\sound.h"
#include "sound\soundop.h"
#include "sound\midi.h"
#include "zx81config.h"
#include "sp0256drv.h"
#include "Digitalkdrv.h"
#include "Joystick.h"
#include "Keypad.h"

// The T-states of the frame being run (set to 0 after Frame() is called)
extern int frametstates;

CSound Sound;

const char *AYStereoNames[AY_STEREO_MODES] =
{
        "Mono", "ABC", "ACB", "BAC", "BCA", "CAB", "CBA"
};

const char *SoundOutputNames[SOUND_OUTPUTS] =
{
        "Automatic", "WASAPI", "DirectSound"
};

// The channel (0-2 for A-C) on the left, in the centre and on the right,
// for each stereo mode but mono
const int AYStereoPlaces[AY_STEREO_MODES][3] =
{
        { 0, 1, 2 }, { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
        { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
};

// Keep the highest level of a meter
#define PEAK(meter,value) { int peak_ = (value); if (peak_ < 0) peak_ = -peak_; \
        if (peak_ > Peak[meter]) Peak[meter] = peak_; }

CSound::CSound()
{
        int i;

        for (i=0; i<VOLUME_SOURCES; i++) VolumeLevel[i] = VOLUME_DEFAULT;
        for (i=0; i<VOLUME_METERS; i++) Peak[i] = 0;
        MainVolume = VOLUME_MAX;
        AYStereo = AY_STEREO_MONO;
        Mix = NULL;
        Buffer = NULL;
        Output = NULL;
        OutputChoice = SOUND_OUTPUT_AUTO;
        AYClock = 0;
        m_hWnd = NULL;
        m_FPS = m_BitsPerSample = m_SampleRate = m_RequestedRate = m_Channels = 0;
        OffsetTStates = 0;
        BeeperOut = NULL;
        BeeperLen = 0;
        BeeperFilled = 0;
        BeeperTarget = 0;
        BeeperLevel = 0;
        BeeperLevelTime = 0;
        BeeperSlope = 0;
        AYDelta = NULL;
        AYDeltaLen = 0;
        AYOut[0] = AYOut[1] = 0;
        AYAcc[0] = AYAcc[1] = 0;
        AYCyclesPerSample = 0;
        AYRng = 1;
        AYNoiseToggle = 1;
        AYEnvStep = 0;
        AYEnvAttack = 0;
        AYEnvHold = AYEnvAlternate = AYEnvHolding = 0;
        AYEnvLevel = 0;
        AYDCIn[0] = AYDCIn[1] = AYDCOut[0] = AYDCOut[1] = 0;
        AYDCPole = 1;
        BlepInit();
}

int CSound::PeakFullScale(int meter)
{
        switch (meter)
        {
        case VOLUME_AY_A: case VOLUME_AY_B: case VOLUME_AY_C:
                return AMPL_AY_TONE;
        case VOLUME_BEEPER:
                return AMPL_BEEPER * ADJUSTTO16BIT;
        default:
                return 32767;
        }
}

// Initialise - may be called several time with different data from different
// sections of the progrm.  eg, first time it's called we provide the window
// handle, next time we're called we get the fps of the emulated machine
// third time we get the sample rate requested.
// Each time we're called we store the provided information, and only when everything
// needed has been provided do we actually do anything.

int CSound::Initialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels)
{
        // If a parameter is not NULL, store the data

        if (hWnd) m_hWnd=hWnd;
        if (FPS) m_FPS=FPS;
        if (BitsPerSample) m_BitsPerSample=BitsPerSample;
        if (SampleRate) m_RequestedRate=SampleRate;
        if (Channels) m_Channels=Channels;

        // If anything is still missing, return

        if (!m_hWnd || !m_FPS || !m_BitsPerSample || !m_RequestedRate || !m_Channels) return(0);

        // Otherwise, we're good to go, so configure the sound.

        m_BytesPerSample=m_BitsPerSample/8;

        if (Output) Output->End();
        Output=NULL;

        return StartOutput(true);
}

// Open the output, and get ready for its sample rate; the sound generating
// devices are reset if asked (otherwise they go on where they are)

int CSound::StartOutput(bool resetDevices)
{
        int oldRate=m_SampleRate;

        // Start by opening the output; it may use another sample rate (the
        // one asked for stays if it fails, the AY needs one)

        m_SampleRate=m_RequestedRate;
        int r = OpenOutput();
        if (r)
        {
                End();
                return(r);
        }

        m_SampleRate=Output->SampleRate();

        if (resetDevices || m_SampleRate!=oldRate || !Buffer || !Mix)
        {
                m_SamplesPerTState=m_SampleRate/(double)machine.clockspeed;

                // get a 1 frame size buffer

                FrameSize=m_SampleRate/m_FPS;

                // A frame of T-states fills exactly one buffer (the beeper has
                // always been placed like this; the AY and the SpecDrum used the
                // clock speed, which put their changes a little early by the end
                // of the frame)
                if (machine.tperframe > 0)
                        m_SamplesPerTState=FrameSize/(double)machine.tperframe;

                if (Buffer) delete []Buffer;
                if (Mix) delete []Mix;
                if (BeeperOut) delete []BeeperOut;
                if (AYDelta) delete []AYDelta;
                Buffer = new short[FrameSize*m_Channels];
                Mix = new int[FrameSize*m_Channels];

                // The beeper: two frames (edges may fall beyond the end of
                // one), and what the edges at the end add after them
                BeeperLen = 2*FrameSize + 2*BLEP_HALF + 2;
                BeeperOut = new float[BeeperLen];

                // The AY: a frame, and what its last steps add after it
                AYDeltaLen = FrameSize + 2*BLEP_HALF + 2;
                AYDelta = new double[2*AYDeltaLen];

                if(Buffer==NULL || Mix==NULL || BeeperOut==NULL || AYDelta==NULL)
                {
                        End();
                        return(-1); // Oh dear, malloc failed
                }

                memset(Mix, 0, FrameSize*m_Channels*sizeof(int));

                // What was in the frame being made (in samples) is lost
                memset(BeeperOut, 0, BeeperLen*sizeof(float));
                BeeperFilled=0;
                BeeperLevel=0;
                BeeperLevelTime=0;
                AYChangeCount=0;
                SpecDrumChangeCount=0;

                BeeperSlope=AMPL_BEEPER*150.0/m_SampleRate;
                if (AYClock) AYCyclesPerSample=(double)AYClock/m_SampleRate;
                memset(AYDelta, 0, 2*AYDeltaLen*sizeof(double));
                AYOut[0]=AYOut[1]=0;
                AYAcc[0]=AYAcc[1]=0;
                AYDCIn[0]=AYDCIn[1]=AYDCOut[0]=AYDCOut[1]=0;
                AYDCPole=1.0-2*3.14159265358979323846*AY_DC_CUTOFF/m_SampleRate;

                sp0256_AL2.SetSamplingFreq(m_SampleRate);
                Digitalker.SetSamplingFreq(m_SampleRate);
        }

        if (resetDevices)
        {
                BeeperLevel=0;
                BeeperTarget=0;
                OffsetTStates=0;

                InitDevices();  // initialise sound generating devices
        }

        Output->Play();
        return(0);
}

// The first output that works, of the ones the choice allows

int CSound::OpenOutput(void)
{
        bool wasapi = OutputChoice != SOUND_OUTPUT_DIRECTSOUND;
        bool dsound = OutputChoice != SOUND_OUTPUT_WASAPI;

        Output = NULL;

        if (wasapi && !WasapiSound.Initialise(m_hWnd, m_FPS, m_BitsPerSample, m_RequestedRate, m_Channels))
                Output = &WasapiSound;
        else if (dsound && !DXSound.Initialise(m_hWnd, m_FPS, m_BitsPerSample, m_RequestedRate, m_Channels))
                Output = &DXSound;
        else if (!WasapiSound.InitialiseNoDevice(m_hWnd, m_FPS, m_BitsPerSample, m_RequestedRate, m_Channels, wasapi))
                Output = &WasapiSound;

        return Output ? 0 : -1;
}

int CSound::SetOutputChoice(int choice)
{
        if (choice < 0 || choice >= SOUND_OUTPUTS) choice = SOUND_OUTPUT_AUTO;
        if (choice == OutputChoice) return 0;

        OutputChoice = choice;
        if (!Output) return 0;          // not playing: used when it starts

        Output->End();
        Output = NULL;
        return StartOutput(false);
}

int CSound::OutputChoiceOf(const char *name)
{
        int i;

        for (i=0; i<SOUND_OUTPUTS; i++)
                if (name && !stricmp(name, SoundOutputNames[i])) return i;

        return SOUND_OUTPUT_AUTO;
}

// Reinitialise gets called if something changes... we lose the window handle,
// The user changes the sample rate - that kind of thing.

int CSound::ReInitialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels)
{
        if (!hWnd) hWnd = m_hWnd;
        if (!FPS) FPS = m_FPS;
        if (!BitsPerSample) BitsPerSample = m_BitsPerSample;
        if (!SampleRate) SampleRate = m_RequestedRate;
        if (!Channels) Channels = m_Channels;

        End();
        return Initialise(hWnd,FPS,BitsPerSample,SampleRate,Channels);
}

// End() - free the sound buffer and tell DirectSould it's all over.
void CSound::End(void)
{
        if (Output) Output->End();
        Output = NULL;
        if(Buffer) delete []Buffer;
        Buffer=NULL;
        if(Mix) delete []Mix;
        Mix=NULL;
        if(BeeperOut) delete []BeeperOut;
        BeeperOut=NULL;
        if(AYDelta) delete []AYDelta;
        AYDelta=NULL;
}

void CSound::InitDevices()
{
        AYInit();
        SpecDrumInit();
        sp0256_AL2.Reset();
        Digitalker.Reset();
        Keypad.Reset();
}

// Initialise the AY Sound chip
void CSound::AYInit(void)
{
        int f,clock;
        double v;

        // logarithmic volume levels, 3dB per step
        v=AMPL_AY_TONE;
        for(f=15;f>0;f--)
        {
                AYToneLevels[f]=v;
                // 10^3/20 = 3dB
                v/=1.4125375446;
        }
        AYToneLevels[0]=0;

        AYClock=0;
        AYCyclesPerSample=0;
        AYNoiseTick=AYNoisePeriod=0;
        AYEnvPeriod=0;
        AYEnvSubCycles=0;
        AYEnvStep=0;
        AYEnvAttack=0;
        AYEnvHold=AYEnvAlternate=0;
        AYEnvHolding=1;
        AYEnvLevel=0;

        for(f=0;f<3;f++)
                AYToneTick[f]=AYToneHalf[f]=0;

        // What was still to come out of the AY is dropped
        if (AYDelta) memset(AYDelta, 0, 2*AYDeltaLen*sizeof(double));
        AYOut[0]=AYOut[1]=0;
        AYAcc[0]=AYAcc[1]=0;
        AYDCIn[0]=AYDCIn[1]=AYDCOut[0]=AYDCOut[1]=0;

        switch(machine.aytype)
        {
        case AY_TYPE_QUICKSILVA: clock=AY_CLOCK_QUICKSILVA; break;
        case AY_TYPE_ZONX:
                if (emulator.machine == MACHINESPECTRUM)
                {
                        clock=(spectrum.model >= SPECCY128 ? AY_CLOCK_ZONX_SPEC128 : AY_CLOCK_ZONX_SPEC48); break;
                }
                else
                {
                        clock=AY_CLOCK_ZONX_ZX81;
                }
                break;
        case AY_TYPE_FULLER: clock=AY_CLOCK_FULLER; break;
        case AY_TYPE_ACE_USER: clock=AY_CLOCK_ACE_USER; break;
        case AY_TYPE_SINCLAIR: clock=(spectrum.model >= SPECCY128 ? AY_CLOCK_SINCLAIR_128K : AY_CLOCK_SINCLAIR_48K); break;
        case AY_TYPE_TS2068: clock=AY_CLOCK_TS2068; break;
        case AY_TYPE_TC2068: clock=AY_CLOCK_TC2068; break;
        case AY_TYPE_BOLDFIELD: clock=AY_CLOCK_BOLDFIELD; break;
        case AY_TYPE_DKTRONICS: clock=(spectrum.model >= SPECCY128 ? AY_CLOCK_DKTRONICS_128K : AY_CLOCK_DKTRONICS_48K); break;
        default:
                //fprintf(stderr,"AY type not specified - can't happen!\n");
                //sound_ay=0;
                return;
        }

        AYClock=clock;
        AYCyclesPerSample=m_SampleRate>0 ? (double)clock/m_SampleRate : 0;
        AYChangeCount=0;
        AYReset();
}

// The AY is run in continuous time: from one event to the next - a tone
// edge, a noise change, an envelope step, a register written (at the
// fraction of a sample where it was) - and each change of what it gives out
// is a band limited step (BlepAdd()), like the beeper's edges. It used to
// be worked out sample by sample, the edges and changes falling on whole
// samples (the tone edges only roughly in between), so high notes brought
// whistling, inharmonic tones with them.
//
// What it gives out: a tone is +-its level, the noise 0 or its level, and a
// tone above half the sample rate (period 0 too) its level; with neither
// tone nor noise, the level, as the chip gives it (sample playing code uses
// these). The envelope is the chip's: 16 steps, holding at 0 or 15.

// A register written: what it changes

void CSound::AYApplyChange(int reg)
{
        unsigned int fine, coarse;
        int r;

        switch(reg)
        {
        case 0: case 1: case 2: case 3: case 4: case 5:
                fine   = AYRegisters[reg & ~1];
                coarse = AYRegisters[reg |  1] & 15;

                r = reg >> 1;
                AYToneHalf[r] = 8.0 * (fine | (coarse << 8));

                // important to get this right, otherwise e.g. Ghouls 'n'
                // Ghosts has really scratchy, horrible-sounding vibrato.
                if (AYToneHalf[r] > 0 && AYToneTick[r] >= 2*AYToneHalf[r])
                        AYToneTick[r] = fmod(AYToneTick[r], 2*AYToneHalf[r]);
                break;

        case 6:
                AYNoiseTick=0;
                AYNoisePeriod=16.0*(AYRegisters[6]&31);
                break;

        case 11: case 12:
                AYEnvPeriod=AYRegisters[11]|(AYRegisters[12]<<8);
                break;

        case 13:
                // The shape: CONTINUE, ATTACK, ALTERNATE, HOLD. One that
                // does not continue holds at 0 at the end, as the one that
                // does with ALTERNATE and HOLD set as ATTACK is.
                AYEnvAttack=(AYRegisters[13]&4) ? 15 : 0;
                if ((AYRegisters[13]&8)==0)
                {
                        AYEnvHold=1;
                        AYEnvAlternate=AYEnvAttack;
                }
                else
                {
                        AYEnvHold=AYRegisters[13]&1;
                        AYEnvAlternate=AYRegisters[13]&2;
                }
                AYEnvStep=15;
                AYEnvHolding=0;
                AYEnvSubCycles=0;
                AYEnvLevel=AYToneLevels[AYEnvStep^AYEnvAttack];
                break;
        }
}

// What the AY gives out now: if it is not what it gave out, a step at `at'
// (samples into the frame)

void CSound::AYEmit(double at)
{
        double chv[3], out[2], level;
        int g, c, mixer=AYRegisters[7];

        for(g=0;g<3;g++)
        {
                level=(AYRegisters[8+g]&16) ? AYEnvLevel : AYToneLevels[AYRegisters[8+g]&15];
                level=level*VolumeLevel[g]/VOLUME_MAX;

                chv[g]=0;
                if((mixer&(1<<g))==0)
                {
                        if (AYToneHalf[g] < AYCyclesPerSample || AYToneTick[g] >= AYToneHalf[g])
                                chv[g]+=level;
                        else
                                chv[g]-=level;
                }
                if((mixer&(8<<g))==0 && AYNoiseToggle)
                        chv[g]+=level;

                // Neither tone nor noise: the chip gives out the level
                // (sample playing code, digital drums, uses that)
                if((mixer&(9<<g))==(9<<g))
                        chv[g]=level;

                PEAK(g, (int)chv[g]);
        }

        // place them: left, centre (on both sides, but not at full
        // volume) and right, or all together in mono
        if((m_Channels==2) && AYStereo!=AY_STEREO_MONO
                && AYStereo>=0 && AYStereo<AY_STEREO_MODES)
        {
                const int *place = AYStereoPlaces[AYStereo];
                double centre = chv[place[1]] * 3/4;
                out[0] = chv[place[0]] + centre;
                out[1] = chv[place[2]] + centre;
        }
        else
        {
                out[0] = out[1] = chv[0] + chv[1] + chv[2];
        }

        for(c=0; c<m_Channels && c<2; c++)
        {
                if (out[c] != AYOut[c])
                {
                        BlepAdd(AYDelta + c*AYDeltaLen, at + BLEP_HALF, out[c]-AYOut[c]);
                        AYOut[c] = out[c];
                }
        }
}

#define AY_EVENT_END            0
#define AY_EVENT_REGISTER       1
#define AY_EVENT_ENVELOPE       2
#define AY_EVENT_TONE           3
#define AY_EVENT_NOISE          4

void CSound::AYOverlay(void)
{       
        struct AYChangeTag *change_ptr;
        int changes_left;
        double t, next, when, dt, cps, threshold, envStep;
        int f, g, c, event, which;

        if (!AYDelta || AYCyclesPerSample <= 0) return;

        change_ptr=AYChange;
        changes_left=AYChangeCount;
        cps=AYCyclesPerSample;

        // The volumes or the stereo mode may have changed since the last
        // frame
        AYEmit(0);

        t=0;
        while(1)
        {
                // The next event: the first of them
                next=FrameSize;
                event=AY_EVENT_END;
                which=0;

                // update ay registers. All this sub-frame change stuff
                // is pretty hairy, but how else would you handle the
                // samples in Robocop? :-) It also clears up some other
                // glitches. (Changes after the end of the buffer are
                // kept for the next one.)
                if (changes_left && change_ptr->ofs < FrameSize)
                {
                        when=change_ptr->ofs + change_ptr->sub/65536.0;
                        if (when < next) { next=when; event=AY_EVENT_REGISTER; }
                }

                // the envelope's next step (none while it holds)
                envStep=16.0*(AYEnvPeriod ? AYEnvPeriod : 1);
                if (!AYEnvHolding)
                {
                        when=t + (envStep-AYEnvSubCycles)/cps;
                        if (when < next) { next=when; event=AY_EVENT_ENVELOPE; }
                }

                // the tones' edges (above half the sample rate they are
                // steady)
                for(g=0;g<3;g++)
                {
                        if (AYToneHalf[g] < cps) continue;

                        threshold=AYToneTick[g] < AYToneHalf[g] ? AYToneHalf[g] : 2*AYToneHalf[g];
                        when=t + (threshold-AYToneTick[g])/cps;
                        if (when < next) { next=when; event=AY_EVENT_TONE; which=g; }
                }

                // the noise: once a sample at most, as it used to be (at
                // the end of each sample, the last one's too)
                if (AYNoisePeriod >= cps)
                        when=t + (AYNoisePeriod-AYNoiseTick)/cps;
                else
                        when=floor(t)+1;
                if (when < next || (event==AY_EVENT_END && when <= next))
                {
                        next=when;
                        event=AY_EVENT_NOISE;
                }

                // Everything moves on to then
                if (next < t) next=t;
                dt=(next-t)*cps;
                t=next;

                for(g=0;g<3;g++)
                {
                        AYToneTick[g]+=dt;

                        // steady: kept within a period
                        if (AYToneHalf[g] < cps)
                                AYToneTick[g]=AYToneHalf[g] > 0 ? fmod(AYToneTick[g], 2*AYToneHalf[g]) : 0;
                }
                if (AYNoisePeriod >= cps) AYNoiseTick+=dt;
                if (!AYEnvHolding) AYEnvSubCycles+=dt;

                if (event==AY_EVENT_END) break;

                switch(event)
                {
                case AY_EVENT_REGISTER:
                        AYRegisters[change_ptr->reg]=change_ptr->val;
                        AYApplyChange(change_ptr->reg);
                        change_ptr++;
                        changes_left--;
                        break;

                case AY_EVENT_ENVELOPE:
                        AYEnvSubCycles-=envStep;
                        if (AYEnvSubCycles < 0) AYEnvSubCycles=0;

                        AYEnvStep--;
                        if (AYEnvStep < 0)
                        {
                                if (AYEnvHold)
                                {
                                        if (AYEnvAlternate) AYEnvAttack^=15;
                                        AYEnvHolding=1;
                                        AYEnvStep=0;
                                }
                                else
                                {
                                        if (AYEnvAlternate) AYEnvAttack^=15;
                                        AYEnvStep=15;
                                }
                        }
                        AYEnvLevel=AYToneLevels[AYEnvStep^AYEnvAttack];
                        break;

                case AY_EVENT_TONE:
                        if (AYToneTick[which] < AYToneHalf[which])
                                AYToneTick[which]=AYToneHalf[which];            // up
                        else if (AYToneTick[which] >= 2*AYToneHalf[which])
                                AYToneTick[which]-=2*AYToneHalf[which];         // down
                        else if (AYToneTick[which] > 2*AYToneHalf[which]-1e-6)
                                AYToneTick[which]=0;
                        break;

                case AY_EVENT_NOISE:
                        if (AYNoisePeriod >= cps)
                        {
                                AYNoiseTick-=AYNoisePeriod;
                                if (AYNoiseTick < 0) AYNoiseTick=0;
                        }

                        // update noise RNG/filter
                        if((AYRng&1)^((AYRng&2)?1:0))
                                AYNoiseToggle=!AYNoiseToggle;

                        // rng is 17-bit shift reg, bit 0 is output.
                        // input is bit 0 xor bit 2.
                        AYRng|=((AYRng&1)^((AYRng&4)?1:0))?0x20000:0;
                        AYRng>>=1;
                        break;
                }

                AYEmit(t);
        }

        // The steps added up into the mix, without the DC (the speaker is
        // coupled through a capacitor: the AY gives out a steady level when
        // tone and noise are off, the noise is on one side of 0, so is an
        // envelope played as a waveform), and what comes after this frame
        // moved to the start
        for(c=0; c<m_Channels && c<2; c++)
        {
                double *delta = AYDelta + c*AYDeltaLen;
                double y;

                for(f=0;f<FrameSize;f++)
                {
                        AYAcc[c]+=delta[f];

                        y=AYAcc[c]-AYDCIn[c]+AYDCPole*AYDCOut[c];
                        if (y > -1e-9 && y < 1e-9) y=0;
                        AYDCIn[c]=AYAcc[c];
                        AYDCOut[c]=y;

                        Mix[f*m_Channels+c]+=(int)(y<0 ? y-0.5 : y+0.5);
                }

                memmove(delta, delta+FrameSize, (AYDeltaLen-FrameSize)*sizeof(double));
                memset(delta+AYDeltaLen-FrameSize, 0, FrameSize*sizeof(double));
        }

        // Changes after the end of this buffer go into the next one
        for(f=0;f<changes_left;f++)
        {
                int ofs = change_ptr[f].ofs - FrameSize;

                if (ofs < 0) ofs = 0;
                if (ofs >= FrameSize) ofs = FrameSize-1;
                AYChange[f] = change_ptr[f];
                AYChange[f].ofs = (unsigned short)ofs;
        }
        AYChangeCount=changes_left;
}

void CSound::AYWrite128(int reg, int val, int frametstates)
{
        reg = reg & 0x0F;

        AYWrite(reg, val, frametstates);

        if ((AYRegisterStore[7] & 0x40) == 0x40)
        {
                Midi.WriteBit(AYRegisterStore[14]);

                if (spectrum.spectrum128Keypad)
                {
                        Keypad.Write(AYRegisterStore[14]);
                }

                // This should also handle writing RS232 data
        }
}

void CSound::AYWriteTimex(int reg, int val, int frametstates)
{
        AYWrite(reg, val, frametstates);
}

// don't make the change immediately; record it for later,
// to be made by sound_frame() (via sound_ay_overlay()).

void CSound::AYWrite(int reg, int val, int frametstates)
{
        reg = reg & 0x0F;

        AYRegisterStore[reg] = (unsigned char)val;

        if (AYChangeCount < AY_CHANGE_MAX)
        {
                double pos = SamplePosition(frametstates);
                int ofs = (int)pos;

                AYChange[AYChangeCount].ofs = (unsigned short)ofs;
                AYChange[AYChangeCount].sub = (unsigned short)((pos - ofs) * 65536);
                AYChange[AYChangeCount].reg = (unsigned char)reg;
                AYChange[AYChangeCount].val = (unsigned char)val;
                AYChangeCount++;
        }
}

int CSound::AYRead128(int reg)
{
        reg = reg & 0x0F;

        int data = AYRead(reg);

        if (spectrum.spectrum128Keypad && reg == 14)
        {
                if ((AYRegisterStore[7] & 0x40) == 0x40) //Output
                {
                        data &= Keypad.Read();
                }
                else //Input
                {
                        data = Keypad.Read();
                }
        }

        return data;
}

int CSound::AYReadTimex(int reg, int joysticks = 0)
{
        reg = reg & 0x0F;

        int data = AYRead(reg);

        if (reg == 14)
        {
                if ((AYRegisterStore[7] & 0x40) == 0x40) //Output
                {
                        if ((joysticks & 0x01) && machine.joystick1Connected)
                        {
                                data &= (byte)(ReadJoystick1() & 0x8F);
                        }
                        if ((joysticks & 0x02) && machine.joystick2Connected)
                        {
                                data &= (byte)(ReadJoystick2() & 0x8F);
                        }
                }
                else //Input
                {
                        if ((joysticks & 0x01) && machine.joystick1Connected)
                        {
                                data = (byte)(ReadJoystick1() & 0x8F);
                        }
                        if ((joysticks & 0x02) && machine.joystick2Connected)
                        {
                                data = (byte)(ReadJoystick2() & 0x8F);
                        }

                        data |= 0x70;
                }
        }

        return data;
}

int CSound::AYRead(int reg)
{
        int data = AYRegisterStore[reg & 0x0F];

        switch (reg)
        {
        case 1:
        case 3:
        case 5:
        case 13:
                data &= 0x0F;
                break;

        case 6:
        case 8:
        case 9:
        case 10:
                data &= 0x1F;
                break;
        }

        return data;
}

// no need to call this initially, but should be called on reset otherwise.

void CSound::AYReset(void)
{
        int f;

        for(f=0;f<16;f++)
                AYWrite(f,0,0);
}

void CSound::SpeechOverlay(void)
{
        for(int f=0;f<FrameSize;f++)
        {
                int temp = sp0256_AL2.GetNextSample();
                temp = (temp*VolumeLevel[VOLUME_SPEECH])/VOLUME_MAX;
                PEAK(VOLUME_SPEECH, temp);
                Mix[f*m_Channels]+=temp;
                if(m_Channels == 2)
                {
                        Mix[f*m_Channels+1]+=temp;
                }
        }
}

void CSound::SpecDrumInit(void)
{
        SpecDrumChangeCount=0;
        SpecDrumLevel=0;
}

void CSound::SpecDrumWrite(BYTE data, int frametstates)
{
        if(SpecDrumChangeCount<SPECDRUM_BUFFSIZE)
        {
                SpecDrumChange[SpecDrumChangeCount].ofs=(unsigned short)SampleOffset(frametstates);
                SpecDrumChange[SpecDrumChangeCount].val=data;
                SpecDrumChangeCount++;
        }
}

void CSound::SpecDrumOverlay(void)
{
        int f;
        struct SpecDrumChangeTag *change_ptr;
        int changes_left;

        change_ptr=SpecDrumChange;
        changes_left=SpecDrumChangeCount;

        for(f=0;f<FrameSize;f++)
        {
                while(changes_left && (f>=change_ptr->ofs
                        || (f==FrameSize-1 && change_ptr->ofs<FrameSize)))
                {
                        SpecDrumLevel=change_ptr->val-128;
                        change_ptr++;
                        changes_left--;
                }

                // generate sound
                int level=(SpecDrumLevel*ADJUSTTO16BIT*VolumeLevel[VOLUME_SPECDRUM])/VOLUME_MAX;
                PEAK(VOLUME_SPECDRUM, level);

                Mix[f*m_Channels]+=level;
                if (m_Channels==2)
                        Mix[f*m_Channels+1]+=level;
        }

        // Changes after the end of this buffer go into the next one
        for(f=0;f<changes_left;f++)
        {
                int ofs = change_ptr[f].ofs - FrameSize;

                if (ofs < 0) ofs = 0;
                if (ofs >= FrameSize) ofs = FrameSize-1;
                SpecDrumChange[f] = change_ptr[f];
                SpecDrumChange[f].ofs = (unsigned short)ofs;
        }
        SpecDrumChangeCount=changes_left;
}

// The sample of the buffer at a T-state of the frame (may be beyond the
// end of the buffer)

int CSound::SampleOffset(int frametstates)
{
        return (int)SamplePosition(frametstates);
}

// The same, with the fraction of the sample

double CSound::SamplePosition(int frametstates)
{
        double pos;

        if (machine.tperframe <= 0) return 0;

        pos = (double)(frametstates + OffsetTStates) * FrameSize / machine.tperframe;
        if (pos < 0) pos = 0;
        if (pos > 65535) pos = 65535;
        return pos;
}


void CSound::DigiTalkOverlay(void)
{
        for(int f=0;f<FrameSize;f++)
        {
                int level = Digitalker.GetNextSample();
                level = (level*VolumeLevel[VOLUME_SPEECH])/VOLUME_MAX;
                PEAK(VOLUME_SPEECH, level);
                Mix[f*m_Channels] += level;
                if(m_Channels == 2)
                {
                        Mix[f*m_Channels+1] += level;
                }
        }
}

// XXX currently using speccy beeper code verbatim for VSYNC.
// Not sure how plausible this is, but for now it'll do.
// It does *sound* pretty plausible.


// it should go without saying that the beeper was hardly capable of
// generating perfect square waves. :-) What seems to have happened is
// that after the `click' in the intended direction away from the rest
// (zero) position, it faded out gradually over about 1/50th of a second
// back down to zero - the bulk of the fade being over after about
// a 1/500th.

// The true behaviour is awkward to model accurately, but a compromise
// emulation (doing a linear fadeout over 1/150th) seems to work quite
// well and IMHO sounds a little more like a real speccy than most
// emulations. It also has the considerable advantage of having a zero
// rest position, which I'm a lot happier with. :-)


// So the level jumps to +-AMPL_BEEPER at an edge, then fades to 0 at
// BeeperSlope a sample. The fade is smooth (it went in 31 steps), and the
// jumps are band limited (they were filtered only by giving the sample
// with the edge in it a level between the two): see BeeperEdge().

void CSound::Frame(bool pause)
{                  
        int f;

        if (!Mix || !Buffer || !BeeperOut) return;

        if (pause)
        {
                memset(Mix, 0, FrameSize*m_Channels*sizeof(int));

                // Nothing is heard from the beeper; it starts again from
                // its rest position
                memset(BeeperOut, 0, BeeperLen*sizeof(float));
                BeeperFilled=0;
                BeeperLevel=0;
                BeeperLevelTime=0;
        }
        else
        {
        // The beeper: the level up to the end of the frame, then this
        // frame's samples out, the rest moved to the start
        BeeperFill(FrameSize);

        double scale=(double)ADJUSTTO16BIT*VolumeLevel[VOLUME_BEEPER]/VOLUME_MAX;

        for(f=0;f<FrameSize;f++)
        {
                double v=BeeperOut[f]*scale;
                int tempval=(int)(v<0 ? v-0.5 : v+0.5);
                PEAK(VOLUME_BEEPER, tempval);
                Mix[f*m_Channels]=tempval;

                if(m_Channels == 2)
                {
                        Mix[f*m_Channels+1]=tempval;
                }
        }

        memmove(BeeperOut, BeeperOut+FrameSize, (BeeperLen-FrameSize)*sizeof(float));
        memset(BeeperOut+BeeperLen-FrameSize, 0, FrameSize*sizeof(float));
        BeeperFilled-=FrameSize;
        BeeperLevelTime-=FrameSize;

                if (machine.aytype) AYOverlay();
        if (spectrum.specdrum) SpecDrumOverlay();

        if (machine.speech)
        {
                if (machine.speech!=SPEECH_TYPE_DIGITALKER)
                        SpeechOverlay();
                else
                        DigiTalkOverlay();
        }
        }

        // The main volume, the output level, and clipping (the sources
        // together can go beyond 16 bits)
        for(f=0;f<FrameSize*m_Channels;f++)
        {
                int value = (Mix[f]*MainVolume)/VOLUME_MAX;

                PEAK(VOLUME_MAIN, value);
                if (value > 32767) value = 32767;
                else if (value < -32768) value = -32768;
                Buffer[f] = (short)value;
        }

        if (Output) Output->Frame((unsigned char *)Buffer, FrameSize*m_Channels*m_BytesPerSample);
        SoundOutput->UpdateImage(Buffer,m_Channels,FrameSize);

        // The frame just run (frametstates: it is set to 0 after this) began
        // OffsetTStates into this buffer; the next one begins where it ended.
        // It stays within a scanline or so, unless the emulator is paused or
        // sped up: then start again at the beginning of the buffer.
        if (pause)
        {
                OffsetTStates = 0;
        }
        else
        {
                OffsetTStates += frametstates - machine.tperframe;
                if (OffsetTStates < -2*machine.tperscanline
                        || OffsetTStates > 2*machine.tperscanline)
                        OffsetTStates = 0;
        }

}

void CSound::Beeper(int on, int frametstates)
{
        double pos;

        // if(!sound_enabled) return;
        if (!Mix || !BeeperOut || machine.tperframe <= 0) return;

        // The position in samples, from where the frame began in its buffer
        pos=(double)(frametstates+OffsetTStates)*FrameSize/machine.tperframe;
        if (pos < 0) pos = 0;

        BeeperEdge(on,pos);
}

// The level of the beeper `samples' after it was `level'

double CSound::BeeperFade(double level, double samples)
{
        double fade;

        if (samples <= 0) return level;

        fade=BeeperSlope*samples;
        if (level > 0) return level > fade ? level-fade : 0;
        if (level < 0) return -level > fade ? level+fade : 0;
        return 0;
}

// The level, without the edge filtering, up to sample `upto' (not included)

void CSound::BeeperFill(int upto)
{
        int f;

        if (upto > BeeperLen) upto = BeeperLen;

        for(f=BeeperFilled; f<upto; f++)
                BeeperOut[f] += (float)BeeperFade(BeeperLevel, f-BeeperLevelTime);

        if (upto > BeeperFilled) BeeperFilled = upto;
}

// An edge at `pos' samples into the frame's buffer. The beeper is BLEP_HALF
// samples late: the level jumps there, and what the band limited step
// differs from a jump by (it is not zero only BLEP_HALF samples either side)
// is added round it.

void CSound::BeeperEdge(int on, double pos)
{
        int val, f, first, last, k;
        double at, jump, x, i, step;

        val=AMPL_BEEPER*(on?1:-1);
        if(val==BeeperTarget) return;
        BeeperTarget=val;

        at=pos+BLEP_HALF;

        // Far beyond the frame (the emulator is sped up): only the level
        // counts then
        if (at > BeeperLen-BLEP_HALF-2) at = BeeperLen-BLEP_HALF-2;

        // Not before the last edge, nor before a sample already given the
        // level (if the frames start again, after a pause)
        if (at < BeeperLevelTime) at = BeeperLevelTime;
        if (BeeperFilled > 0 && at <= BeeperFilled-1) at = BeeperFilled-1+1e-6;

        // The level up to the edge, then the jump
        BeeperFill((int)ceil(at));

        jump=val-BeeperFade(BeeperLevel, at-BeeperLevelTime);
        BeeperLevel=val;
        BeeperLevelTime=at;

        first=(int)ceil(at-BLEP_HALF);
        last=(int)floor(at+BLEP_HALF);
        if (first < 0) first = 0;
        if (last > BeeperLen-1) last = BeeperLen-1;

        for(f=first; f<=last; f++)
        {
                // The step interpolated from the table, less the jump the
                // level makes (the samples from the edge on have it)
                x=f-at;
                i=(x+BLEP_HALF)*BLEP_PHASES;
                k=(int)i;
                if (k < 0) { k = 0; i = 0; }
                if (k >= BLEP_POINTS-1) step=1.0;
                else step=BlepStep[k]+(i-k)*(BlepStep[k+1]-BlepStep[k]);

                if (x >= 0) step-=1.0;
                BeeperOut[f]+=(float)(jump*step);
        }
}

// The band limited step: a windowed sinc (Kaiser window), integrated

static double BesselI0(double x)
{
        double sum=1, term=1, k;

        for(k=1; k<100; k++)
        {
                term*=(x/(2*k))*(x/(2*k));
                sum+=term;
                if (term < sum*1e-16) break;
        }
        return sum;
}

float CSound::BlepStep[BLEP_POINTS + 1];

void CSound::BlepInit(void)
{
        static bool done=false;
        static double step[BLEP_POINTS];
        const double pi=3.14159265358979323846;
        double h, last=0, sum=0, x, t, w, a;
        int j;

        if (done) return;
        done=true;

        for(j=0; j<BLEP_POINTS; j++)
        {
                x=(double)(j-BLEP_HALF*BLEP_PHASES)/BLEP_PHASES;
                t=x/BLEP_HALF;
                w=BesselI0(BLEP_KAISER_BETA*sqrt(t*t<1 ? 1-t*t : 0))/BesselI0(BLEP_KAISER_BETA);
                a=2*BLEP_CUTOFF*x;
                h=2*BLEP_CUTOFF*(a==0 ? 1.0 : sin(pi*a)/(pi*a))*w;

                // the trapezoid rule
                if (j) sum+=(h+last)/2;
                step[j]=sum;
                last=h;
        }

        for(j=0; j<BLEP_POINTS; j++)
                BlepStep[j]=(float)(step[j]/sum);
        BlepStep[BLEP_POINTS]=1.0f;
}

// The band limited step at x samples from it: 0 ... 1

double CSound::BlepStepAt(double x)
{
        double i;
        int k;

        if (x <= -BLEP_HALF) return 0;
        if (x >= BLEP_HALF) return 1;

        i=(x+BLEP_HALF)*BLEP_PHASES;
        k=(int)i;
        if (k >= BLEP_POINTS-1) return 1;
        return BlepStep[k]+(i-k)*(BlepStep[k+1]-BlepStep[k]);
}

// A band limited step of `jump' at `at' samples into `buffer', which holds
// the differences from one sample to the next (they are added up as they
// are mixed): it goes from 0 to 1 within BLEP_HALF samples either side

void CSound::BlepAdd(double *buffer, double at, double jump)
{
        int f, first, last;
        double previous=0, step;

        first=(int)ceil(at-BLEP_HALF);
        last=(int)floor(at+BLEP_HALF)+1;
        if (first < 0) first = 0;
        if (last > AYDeltaLen-1) last = AYDeltaLen-1;

        for(f=first; f<=last; f++)
        {
                step=BlepStepAt(f-at);
                buffer[f]+=jump*(step-previous);
                previous=step;
        }
}



