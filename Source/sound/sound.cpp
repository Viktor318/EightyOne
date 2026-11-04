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
        OffsetTStates = 0;
        BeeperRequested = 0;
        BeeperPendingCount = 0;
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
        if (SampleRate) m_SampleRate=SampleRate;
        if (Channels) m_Channels=Channels;

        // If anything is still missing, return

        if (!m_hWnd || !m_FPS || !m_BitsPerSample || !m_SampleRate || !m_Channels) return(0);

        // Otherwise, we're good to go, so configure the sound.

        m_BytesPerSample=m_BitsPerSample/8;
        m_SamplesPerTState=m_SampleRate/(double)machine.clockspeed;
        EnvHeld=0;
        EnvAlternating=0;
        BeeperLastSubpos=0;

        // Start by initialsing DirectSound

        int r = DXSound.Initialise(m_hWnd, m_FPS, m_BitsPerSample, m_SampleRate, m_Channels);
        if (r)
        {
                return(r);
        }

        // get a 1 frame size buffer

        FrameSize=m_SampleRate/m_FPS;

        // A frame of T-states fills exactly one buffer (the beeper has always
        // been placed like this; the AY and the SpecDrum used the clock speed,
        // which put their changes a little early by the end of the frame)
        if (machine.tperframe > 0)
                m_SamplesPerTState=FrameSize/(double)machine.tperframe;

        Buffer = new short[FrameSize*m_Channels];
        Mix = new int[FrameSize*m_Channels];

        if(Buffer==NULL || Mix==NULL)
        {
                DXSound.End();
                return(-1); // Oh dear, malloc failed
        }

        memset(Mix, 0, FrameSize*m_Channels*sizeof(int));

        OldVal=OldValOrig=0;
        OldPos=-1;
        FillPos=0;
        OffsetTStates=0;
        BeeperRequested=0;
        BeeperPendingCount=0;

        BeeperTick=0;
        BeeperTickIncr=(1<<24)/m_SampleRate;

        InitDevices();  // initialise sound generating devices

        sp0256_AL2.SetSamplingFreq(m_SampleRate);
        Digitalker.SetSamplingFreq(m_SampleRate);

        DXSound.Play();
        return(0);
}

// Reinitialise gets called if something changes... we lose the window handle,
// The user changes the sample rate - that kind of thing.

int CSound::ReInitialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels)
{
        if (!hWnd) hWnd = m_hWnd;
        if (!FPS) FPS = m_FPS;
        if (!BitsPerSample) BitsPerSample = m_BitsPerSample;
        if (!SampleRate) SampleRate = m_SampleRate;
        if (!Channels) Channels = m_Channels;

        End();
        return Initialise(hWnd,FPS,BitsPerSample,SampleRate,Channels);
}

// End() - free the sound buffer and tell DirectSould it's all over.
void CSound::End(void)
{
        DXSound.End();
        if(Buffer) delete []Buffer;
        Buffer=NULL;
        if(Mix) delete []Mix;
        Mix=NULL;
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

        AYNoiseTick=AYNoisePeriod=0;
        AYEnvTick=AYEnvPeriod=0;

        for(f=0;f<3;f++)
                AYToneTick[f]=AYTonePeriod[f]=0;

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

        AYTickIncr=(int)(65536.*clock/m_SampleRate);
        AYChangeCount=0;
        AYReset();
}

// not great having this as a macro to inline it, but it's only
// a fairly short routine, and it saves messing about.
// (XXX ummm, possibly not so true any more :-))

#define AY_GET_SUBVAL(tick,period) \
  (level*2*(tick-period)/AYTickIncr)

#define AY_OVERLAY_TONE(val,chan,level) { \
  was_high=0;								\
  if(level)								\
    {									\
    if(AYToneTick[chan]>=AYTonePeriod[chan])			\
      { (val)+=(level); was_high=1; }					\
    else								\
      (val)-=(level);						\
    }									\
  									\
  AYToneTick[chan]+=AYTickIncr;					\
  if(level && !was_high && AYToneTick[chan]>=AYTonePeriod[chan])	\
    (val)+=AY_GET_SUBVAL(AYToneTick[chan],AYTonePeriod[chan]);	\
  									\
  if(AYToneTick[chan]>=AYTonePeriod[chan]*2)			\
    {									\
    AYToneTick[chan]-=AYTonePeriod[chan]*2;				\
    /* sanity check needed to avoid making samples sound terrible */ 	\
    if(level && AYToneTick[chan]<AYTonePeriod[chan]) 		\
      (val)-=AY_GET_SUBVAL(AYToneTick[chan],0);			\
    }                                                           \
  }


void CSound::AYOverlay(void)
{       
        static int rng=1;
        static int noise_toggle=1;
        static double env_level=0;
        double tone_level[3];
        int mixer,envshape;
        int f,g,level;
        int v=0;
        struct AYChangeTag *change_ptr;
        int changes_left;
        int reg,r;
        int was_high;
        unsigned int FineValue, CoarseValue, TonePeriod;

        change_ptr=AYChange;
        changes_left=AYChangeCount;
        bool stereo = (m_Channels==2) && AYStereo!=AY_STEREO_MONO
                        && AYStereo>=0 && AYStereo<AY_STEREO_MODES;

        // If no AY chip, don't produce any AY sound (!)
        //if(!sound_ay) return;

        for(f=0;f<FrameSize;f++)
        {
                // update ay registers. All this sub-frame change stuff
                // is pretty hairy, but how else would you handle the
                // samples in Robocop? :-) It also clears up some other
                // glitches.
                //
                // Ok, maybe that's no big deal on the ZX81, but even so. :-)
                // (Though, due to tstate-changing games in z80.c, we can
                // rarely `lose' one this way - hence "f==.." bit below
                // to catch any that slip through.)

                while(changes_left && (f>=change_ptr->ofs
                        || (f==FrameSize-1 && change_ptr->ofs<FrameSize)))
                {
                        reg=change_ptr->reg;
                        AYRegisters[reg]=change_ptr->val;
                        change_ptr++;
                        changes_left--;

                        // fix things as needed for some register changes
                        switch(reg)
                        {

                        case 0: case 1: case 2: case 3: case 4: case 5:
                                FineValue   = AYRegisters[reg & ~1];
                                CoarseValue = AYRegisters[reg |  1] & 15;
                                TonePeriod = (8 * (FineValue | (CoarseValue << 8))) << 16;

                                r = reg >> 1;
                                AYTonePeriod[r] = TonePeriod;

                                // important to get this right, otherwise e.g. Ghouls 'n' Ghosts
                                // has really scratchy, horrible-sounding vibrato.

                                if(AYTonePeriod[r] && AYToneTick[r]>=AYTonePeriod[r]*2)
                                        AYToneTick[r]%=AYTonePeriod[r]*2;
                                break;
                        case 6:
                                AYNoiseTick=0;
                                AYNoisePeriod=(16*(AYRegisters[reg]&31))<<16;
                                break;
                        case 11: case 12:
                                // this one *isn't* fixed-point
                                AYEnvPeriod=AYRegisters[11]|(AYRegisters[12]<<8);
                                break;

                        case 13:
                                AYEnvTick=AYEnvSubCycles=0;
                                EnvHeld=EnvAlternating=0;
                                env_level=0;
                                break;
                        }    
                }

                // the tone level if no enveloping is being used
                for(g=0;g<3;g++)
                        tone_level[g]=AYToneLevels[AYRegisters[8+g]&15];

                // envelope
                envshape=AYRegisters[13];
                if(AYEnvPeriod)
                {
                        if(!EnvHeld)
                        {
                                v=((int)AYEnvTick*15)/AYEnvPeriod;
                                if(v<0) v=0;
                                if(v>15) v=15;
                                if((envshape&4)==0) v=15-v;
                                if(EnvAlternating) v=15-v;
                                env_level=AYToneLevels[v];
                        }
                }

                for(g=0;g<3;g++)
                        if(AYRegisters[8+g]&16)
                                tone_level[g]=env_level;

                if(AYEnvPeriod)
                {
                        // envelope gets incr'd every 256 AY cycles
                        AYEnvSubCycles+=AYTickIncr;
                        if(AYEnvSubCycles>=(256<<16))
                        {
                                AYEnvSubCycles-=(256<<16);

                                AYEnvTick++;
                                if(AYEnvTick>=AYEnvPeriod)
                                {
                                        AYEnvTick-=AYEnvPeriod;
                                        if(!EnvHeld && ((envshape&1) || (envshape&8)==0))
                                        {
                                                EnvHeld=1;
                                                if((envshape&2) || (envshape&0xc)==4)
                                                        env_level=AYToneLevels[15-v];
                                        }
                                        if(!EnvHeld && (envshape&2))
                                                EnvAlternating=!EnvAlternating;
                                }
                        }
                }

                // generate tone and noise, each channel on its own
                int chv[3]={0,0,0};
                mixer=AYRegisters[7];

                for(g=0;g<3;g++)
                {
                        if((mixer&(1<<g))==0)
                        {
                                level=(tone_level[g]*VolumeLevel[g])/VOLUME_MAX;
                                AY_OVERLAY_TONE(chv[g],g,level);
                        }
                }

                for(g=0;g<3;g++)
                {
                        if((mixer&(8<<g))==0)
                        {
                                level=noise_toggle?tone_level[g]:0;
                                level=(level*VolumeLevel[g])/VOLUME_MAX;
                                chv[g]+=level;
                        }
                }

                for(g=0;g<3;g++)
                        PEAK(g, chv[g]);

                // place them: left, centre (on both sides, but not at full
                // volume) and right, or all together in mono
                int ch1, ch2;
                if(stereo)
                {
                        const int *place = AYStereoPlaces[AYStereo];
                        int centre = chv[place[1]] * 3/4;
                        ch1 = chv[place[0]] + centre;
                        ch2 = chv[place[2]] + centre;
                }
                else
                {
                        ch1 = ch2 = chv[0] + chv[1] + chv[2];
                }

                Mix[f*m_Channels]+=ch1;
                if (m_Channels==2)
                        Mix[f*m_Channels+1]+=ch2;

                // update noise RNG/filter
                AYNoiseTick+=AYTickIncr;
                if(AYNoiseTick>=AYNoisePeriod)
                {
                        if((rng&1)^((rng&2)?1:0))
                                noise_toggle=!noise_toggle;

                        // rng is 17-bit shift reg, bit 0 is output.
                        // input is bit 0 xor bit 2.

                        rng|=((rng&1)^((rng&4)?1:0))?0x20000:0;
                        rng>>=1;

                        AYNoiseTick-=AYNoisePeriod;
                }
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
                AYChange[AYChangeCount].ofs = (unsigned short)SampleOffset(frametstates);
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
        int ofs;

        if (machine.tperframe <= 0) return 0;

        ofs = (int)(((__int64)(frametstates + OffsetTStates) * FrameSize) / machine.tperframe);
        if (ofs < 0) ofs = 0;
        if (ofs > 65535) ofs = 65535;
        return ofs;
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


#define BEEPER_FADEOUT	(((1<<24)/150)/AMPL_BEEPER)

#define BEEPER_OLDVAL_ADJUST \
  BeeperTick+=BeeperTickIncr;	\
  if(BeeperTick>=BEEPER_FADEOUT)	\
    {					\
    BeeperTick-=BEEPER_FADEOUT;	\
    if(OldVal>0)		\
      OldVal--;			\
    else				\
      if(OldVal<0)		\
        OldVal++;			\
    }

void CSound::Frame(bool pause)
{                  
        int f;

        if (!Mix || !Buffer) return;

        if (pause)
        {
                memset(Mix, 0, FrameSize*m_Channels*sizeof(int));
        }
        else
        {
        for(f=FillPos;f<FrameSize;f++)
        {
                BEEPER_OLDVAL_ADJUST;
                int tempval=(OldVal*ADJUSTTO16BIT*VolumeLevel[VOLUME_BEEPER])/VOLUME_MAX;
                PEAK(VOLUME_BEEPER, tempval);
                Mix[f*m_Channels]=tempval;

                if(m_Channels == 2)
                {
                        Mix[f*m_Channels+1]=tempval;
                }
        }

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

        DXSound.Frame((unsigned char *)Buffer, FrameSize*m_Channels*m_BytesPerSample);
        SoundOutput->UpdateImage(Buffer,m_Channels,FrameSize);

        // The next buffer
        OldPos=-1;
        FillPos=0;

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

        // The beeper edges of the frame that fell after this buffer
        for(f=0;f<BeeperPendingCount;f++)
                BeeperEdge(BeeperPending[f].on, BeeperPending[f].pos, BeeperPending[f].subpos);
        BeeperPendingCount=0;
}

void CSound::Beeper(int on, int frametstates)
{
        int newpos,subpos,val;
        __int64 pos;

        // if(!sound_enabled) return;
        if (!Mix || machine.tperframe <= 0) return;

        val=AMPL_BEEPER*(on?1:-1);

        if(val==BeeperRequested) return;
        BeeperRequested=val;

        // The position in 1/AMPL_BEEPER samples, from where the frame
        // began in its buffer
        pos=((__int64)(frametstates+OffsetTStates)*FrameSize*AMPL_BEEPER)/machine.tperframe;
        if (pos < 0) pos = 0;
        newpos=(int)(pos/AMPL_BEEPER);
        subpos=(int)(pos-(__int64)AMPL_BEEPER*newpos);

        // After the end of this buffer: keep it for the next one. (If there
        // are too many, or it is too far, the emulator is being sped up:
        // only the level counts then.)
        if(newpos>=FrameSize)
        {
                newpos-=FrameSize;
                if (newpos>=FrameSize) newpos=FrameSize-1;

                if (BeeperPendingCount<BEEPER_PENDING_MAX)
                        BeeperPendingCount++;
                BeeperPending[BeeperPendingCount-1].on=on;
                BeeperPending[BeeperPendingCount-1].pos=newpos;
                BeeperPending[BeeperPendingCount-1].subpos=subpos;
                return;
        }

        BeeperEdge(on,newpos,subpos);
}

void CSound::BeeperEdge(int on, int newpos, int subpos)
{
        int val,subval;
        int f;

        val=AMPL_BEEPER*(on?1:-1);

        if(val==OldValOrig) return;

        // if we already wrote here, adjust the level.

        if(newpos==OldPos)
        {
                // adjust it as if the rest of the sample period were all in
                // the new state. (Often it will be, but if not, we'll fix
                // it later by doing this again.)

                if(on)
                        BeeperLastSubpos+=AMPL_BEEPER-subpos;
                else
                        BeeperLastSubpos-=AMPL_BEEPER-subpos;
        }
        else
                BeeperLastSubpos=(on?AMPL_BEEPER-subpos:subpos);

        subval=0-AMPL_BEEPER+BeeperLastSubpos;

        if(newpos>=0)
        {
                //  fill gap from previous position
                for(f=FillPos;f<newpos && f<FrameSize;f++)
                {
                        BEEPER_OLDVAL_ADJUST;
                        int tempval=(OldVal*ADJUSTTO16BIT*VolumeLevel[VOLUME_BEEPER])/VOLUME_MAX;
                        PEAK(VOLUME_BEEPER, tempval);
                        Mix[f*m_Channels]=tempval;

                        if(m_Channels==2)
                        {
                                Mix[f*m_Channels+1]=tempval;
                        }
                }

                if(newpos<FrameSize)
                {
                        // newpos may be less than FillPos, so...
                        // limit subval in case of faded beeper level,
                        // to avoid slight spikes on ordinary tones.

                        if((OldVal<0 && subval<OldVal) ||
                                (OldVal>=0 && subval>OldVal))
                                        subval=OldVal;

                        // write subsample value
                        int tempval=(subval*ADJUSTTO16BIT*VolumeLevel[VOLUME_BEEPER])/VOLUME_MAX;
                        PEAK(VOLUME_BEEPER, tempval);
                        Mix[newpos*m_Channels]=tempval;
                        if(m_Channels==2)
                        {
                                Mix[newpos*m_Channels+1]=tempval;
                        }
                }
        }

        OldPos=newpos;
        FillPos=newpos+1;
        OldVal=OldValOrig=val;
}



