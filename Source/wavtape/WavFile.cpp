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
#include <stdio.h>
#pragma hdrstop

#include "WavFile.h"
#include "utils.h"

//---------------------------------------------------------------------------

TWavFile::TWavFile()
{
        FileName="";
        SampleRate=0;
        NoSamples=0;
        Bits=8;
        Data.Data=NULL;

        Volume=0;
        Bias=0;

        NewFile();
}

TWavFile::~TWavFile()
{
        FileName="";
        SampleRate=0;
        NoSamples=0;
        Bits=0;
        Stereo=false;

        /*if (Data.Data)*/ delete[] Data.Data;

}

bool TWavFile::SaveCSW(AnsiString FName)
{
        return true;
}

// The length of a file

static long FileLength(FILE *f)
{
        long pos, end;

        pos=ftell(f);
        fseek(f, 0, SEEK_END);
        end=ftell(f);
        fseek(f, pos, SEEK_SET);
        return end;
}

// The next pulse of a CSW file (how many samples): 0 at the end

static unsigned int CSWPulse(FILE *f)
{
        int c;
        unsigned char b[4];

        c=fgetc(f);
        if (c==EOF) return 0;
        if (c) return (unsigned int)c;

        // 0: the length follows in 4 bytes
        if (fread(b, 1, 4, f) != 4) return 0;
        return b[0] | (b[1]<<8) | (b[2]<<16) | ((unsigned int)b[3]<<24);
}

// A CSW (version 1.01) file: each pulse is turned into its samples. The
// file is only taken if it is all right; otherwise what was loaded stays.

bool TWavFile::LoadCSW(AnsiString FName)
{
        FILE *f;
        struct CSW csw;
        unsigned int size, pulse, i, rate;
        unsigned char *buffer;
        int current;

        f=fopen(FName.c_str(), "rb");
        if (!f) return(false);

        memset(&csw, 0, sizeof(csw));
        if (fread(&csw,0x20,1,f) != 1
                || strncmp(csw.Sig, "Compressed Square Wave", 22)
                || csw.MajVer!=1 || csw.MinVer!=1)
        {
                fclose(f);
                return(false);
        }

        rate=(unsigned char)csw.SampleRateHi * 256 + (unsigned char)csw.SampleRateLo;
        if (rate==0)
        {
                fclose(f);
                return(false);
        }

        // How many samples: no more than a sample a byte can be (2 GB)
        size=0;
        while((pulse=CSWPulse(f)) != 0)
        {
                if (pulse > 0x7FFFFFFF - size) pulse = 0x7FFFFFFF - size;
                size+=pulse;
                if (size==0x7FFFFFFF) break;
        }

        try
        {
                buffer=new unsigned char[size ? size : 1];
        }
        catch(...)
        {
                buffer=NULL;
        }
        if (!buffer)
        {
                fclose(f);
                return(false);
        }

        // The samples
        fseek(f, 0x20, SEEK_SET);
        current=csw.Flags&1;
        i=0;
        while(i<size && (pulse=CSWPulse(f)) != 0)
        {
                if (pulse > size-i) pulse=size-i;
                memset(buffer+i, current ? 255 : 0, pulse);
                i+=pulse;
                current=1-current;
        }
        fclose(f);

        SampleRate = rate;
        Stereo=false;
        Signed=false;
        NoSamples=size;
        Bits=8;
        Format.BytesSample=1;
        Format.NoChannels=1;
        Format.SampleRate=rate;
        Format.BitsSample=8;
        Data.DataLen=NoSamples;
        if (Data.Data!=NULL) delete[] Data.Data;
        Data.Data=buffer;
        MemAllocated=size ? size : 1;
        FileName=FName;

        return(true);
}

// A WAV file. Its chunks are looked through for "fmt " and "data"; the file
// is only taken if they make sense, otherwise what was loaded stays. Data
// cut short is taken as far as it goes.

bool TWavFile::LoadFile(AnsiString FName)
{
        FILE *f;
        struct RIFFHeader head;
        struct RIFFFormat format;
        char blkname[4];
        unsigned int blklen, datalen;
        long length, left;
        bool haveHead, haveFormat, haveData;
        unsigned char *buffer;

        if (FileNameGetExt(FName)==".CSW") return(LoadCSW(FName));

        f=fopen(FName.c_str(), "rb");
        if (!f) return(false);
        length=FileLength(f);

        memset(&head, 0, sizeof(head));
        memset(&format, 0, sizeof(format));
        haveHead=haveFormat=haveData=false;
        datalen=0;

        // Each chunk read moves on, or ends the search
        while(!haveData)
        {
                long pos=ftell(f);

                if (fread(blkname, 4,1,f) != 1 || fread(&blklen, 4,1,f) != 1) break;

                if (!strncmp(blkname,"RIFF",4))
                {
                        fseek(f, pos, SEEK_SET);
                        if (fread(&head,sizeof(struct RIFFHeader),1,f) != 1) break;
                        haveHead=true;
                }
                else if (!strncmp(blkname,"fmt",3))
                {
                        fseek(f, pos, SEEK_SET);
                        if (fread(&format,sizeof(struct RIFFFormat),1,f) != 1) break;
                        haveFormat=true;
                        if (format.BlkLen>16)
                        {
                                if (format.BlkLen-16 > (unsigned long)(length-ftell(f))) break;
                                fseek(f, format.BlkLen-16, SEEK_CUR);
                        }
                }
                else if (!strncmp(blkname,"data",4))
                {
                        datalen=blklen;
                        haveData=true;
                }
                else
                {
                        if (blklen > (unsigned long)(length-ftell(f))) break;
                        fseek(f, blklen, SEEK_CUR);
                }
        }

        if (!haveHead || !haveFormat || !haveData
                || format.NoChannels < 1 || format.BytesSample < 1
                || format.SampleRate == 0)
        {
                fclose(f);
                return false;
        }

        // A recording cut short: as far as it goes
        left=length-ftell(f);
        if (left < 0) left=0;
        if (datalen > (unsigned long)left) datalen=left;

        try
        {
                buffer=new unsigned char[datalen ? datalen : 1];
        }
        catch(...)
        {
                buffer=NULL;
        }
        if (!buffer)
        {
                fclose(f);
                return false;
        }

        fread(buffer, 1, datalen, f);
        fclose(f);

        Head=head;
        Format=format;
        strncpy(Data.Head, "data", 4);
        Data.DataLen=datalen;
        if (Data.Data!=NULL) delete[] Data.Data;
        Data.Data=buffer;
        MemAllocated=datalen ? datalen : 1;

        SampleRate = Format.SampleRate;
        NoSamples = Data.DataLen /  Format.BytesSample;
        Bits = Format.BytesSample * 8;
        Signed=false;

        if (Format.NoChannels == 1) Stereo=false;
        else Stereo=true;

        FileName=FName;
        if (Format.BitsSample>8) Signed=true;

        //Signed = !Signed;

        return(true);
}

bool TWavFile::SaveFile(AnsiString FName)
{
        FILE *f;

        if (FileNameGetExt(FName)==".CSW") return(SaveCSW(FName));

        f=fopen(FName.c_str(), "wb");

        fwrite(&Head, 1, sizeof(Head), f);
        fwrite(&Format, 1, sizeof(Format), f);
        fwrite(&Data, 1, 8, f);
        fwrite(Data.Data, Data.DataLen, 1, f);

        fclose(f);
           
        return(true);
}

unsigned char TWavFile::Sample(unsigned int SampleNo, int Channel)
{
        int Pos, SampleSize;
        unsigned char data;
        int val;

        if (SampleNo>=NoSamples || Format.NoChannels<1) return(128);

        SampleSize = Format.BytesSample / Format.NoChannels;

        Pos = SampleNo * Format.BytesSample + Channel * SampleSize;


        switch( SampleSize )
        {
        case 1:
                data = Data.Data[Pos];
                break;

        case 2:
                data = (unsigned char)((* (( unsigned short *) (Data.Data + Pos))) / 256);
                break;

        default:
                data=128;
        }

        if (Signed==true) data = (unsigned char)(128 + ((signed char) data));
        data=(unsigned char)(255-data);

        val=data-128;

        if (Volume>0) val=val * (1+Volume/10.0);
        if (Volume<0) val=val / (1-Volume/10.0);
        val += Bias;

        if (val>127) val=127;
        if (val<-127) val=-127;

        data = (unsigned char)(val + 128);
        return(data);

}

bool TWavFile::NewFile(void)
{
        if (Data.Data)
                delete[] Data.Data;

        FileName = "";
        Signed = false;

        strncpy(Head.Head,"RIFF",4);
        Head.TotalLen=36;
        strncpy(Head.Type,"WAVE",4);

        strncpy(Format.Head,"fmt ",4);
        Format.BlkLen=16;
        Format.fill=1;
        Format.NoChannels=1;
        Format.SampleRate=44100;
        Format.BytesSec=44100;
        Format.BytesSample=1;
        Format.BitsSample=8;

        strncpy(Data.Head,"data",4);
        Data.DataLen=0;
        Data.Data=NULL;

        SampleRate=44100;
        NoSamples=0;
        Bits=8;
        Stereo=false;

        MemAllocated = (16384*8*140);  // 16k * average of 70 Samples per bit
        Data.Data = new unsigned char[MemAllocated];
        Data.Data[0]=128;


        return true;
}
bool TWavFile::SetSample(unsigned int Pos, int Value)
{
        char c;

        if (!MemAllocated) return(false);

        if (Pos>=MemAllocated)
        {

                int NewMemAllocated;
                unsigned char *NewData;

                NewMemAllocated = MemAllocated + (16384*8*70);
                NewData = new unsigned char[NewMemAllocated];

                if (Data.Data)
                {
                        memcpy(NewData, Data.Data, MemAllocated);
                        delete[] Data.Data;
                }
                Data.Data = NewData;
                MemAllocated = NewMemAllocated;

        }

        if (Pos>NoSamples) NoSamples=Pos;
        if (Pos>Data.DataLen)
        {
                Data.DataLen=Pos;
                Head.TotalLen=36+Data.DataLen;
        }

        c=(char) Value;

        Data.Data[Pos] = c;
        return(true);
}

bool TWavFile::CanRecord(void)
{
        if (Format.SampleRate!=44100) return(false);
        if (Format.BytesSample!=1) return(false);
        if (Format.BitsSample!=8) return(false);
        if (Stereo) return(false);
        if (Signed) return(false);
        return(true);
}

