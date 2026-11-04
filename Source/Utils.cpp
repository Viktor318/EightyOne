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
#include <ctype.h>
#include <stdio.h>
#pragma hdrstop

#include "Utils.h"
#include "zlib.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

char ZXCharTable[]= " ----------\'£$:?()><=+-*/;,.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
ULONG crc32_table[256];  // Lookup table array

ULONG CRC32Reflect(ULONG ref, BYTE ch)
{
        ULONG value=0;

        for(int i = 1; i < (ch + 1); i++)
        {
                if (ref & 1) value |= 1 << (ch - i);
                ref >>= 1;
        }
        return value;
}

void CRC32Init()
{
	ULONG ulPolynomial = 0x04c11db7;

        for(int i = 0; i <= 0xFF; i++)
        {
                crc32_table[i]=CRC32Reflect(i, 8) << 24;
                for (int j = 0; j < 8; j++)
                        crc32_table[i] = (crc32_table[i] << 1)
                                                ^ (crc32_table[i] & (1 << 31)
                                                        ? ulPolynomial : 0);
                crc32_table[i] = CRC32Reflect(crc32_table[i], 32);
        }
}

int CRC32Block(BYTE *block, int len)
{
        static bool initialised=0;
        ULONG  ulCRC=0xffffffff;
        BYTE* buffer;

        if (!initialised)
        {
                CRC32Init();
                initialised=true;
        }

        buffer = block;
        while(len--)
                ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF) ^ *buffer++];
        return ulCRC & 0xffff;
}

AnsiString FileNameGetPath(AnsiString Fname)
{
        int i,Pos;
        AnsiString Result;

        Pos=0;
        for(i=1;i<=Fname.Length();i++)
                if (Fname[i]=='\\') Pos=i;

        if (!Pos) return("");

        Result="";

        for(i=1;i<=Pos;i++) Result += Fname[i];
        return(Result);
}

AnsiString FileNameGetFname(AnsiString Fname)
{
        int i,Pos1,Pos2;
        AnsiString Result;

        Pos1=Pos2=0;
        for(i=1;i<=Fname.Length();i++)
        {
                if (Fname[i]=='\\') Pos1=i;
                if (Fname[i]=='.') Pos2=i;
        }

        Pos1++;
        if (Pos2<Pos1) Pos2=Fname.Length() + 1;

        Result="";

        for(i=Pos1;i<Pos2;i++) Result += Fname[i];
        return(Result);
}

AnsiString FileNameGetExt(AnsiString Fname)
{
        int i,Pos1,Pos2;
        AnsiString Result;

        Pos1=Pos2=0;
        for(i=1;i<=Fname.Length();i++)
        {
                if (Fname[i]=='\\') Pos1=i;
                if (Fname[i]=='.') Pos2=i;
        }

        if (!Pos1) Pos1=1;
        if (Pos2<Pos1) Pos2=Fname.Length() + 1;

        Result="";

        for(i=Pos2;i<=Fname.Length();i++) Result += Fname[i];
        Result = Result.UpperCase();
        return(Result);
}

AnsiString ConvertZX81ASCII(BYTE *str)
{
        AnsiString Output;
        BYTE c;
        int len=0;

        Output="";

        do
        {
                c=*(str++);

                if (c<64) Output += ZXCharTable[c];
                else if (c>=128 && c<192)
                {
                        Output += ZXCharTable[c-128];
                        return(Output);
                }
                else Output += '-';

                len++;
        } while (len<16);
        return(Output);
}



void ConvertASCIIZX81(AnsiString Str, BYTE *Output)
{
        char c, *p;
        int i;

        if (!Str.Length()) return;

        i=1;
        p=ZXCharTable;

        while(i<=Str.Length() && p)
        {
                c = (char)toupper(Str[i]);
                if (c=='_') c=' ';

                p = strchr(ZXCharTable, c);
                if (p) *(Output++) = (BYTE)(p - ZXCharTable);
                i++;
        }


        Output--;
        *Output |= 128;
}

void ASCIIZX81(BYTE *in, BYTE *Out)
{
        char c, *p;
        unsigned int i;

        if (!strlen((const char*)in)) return;

        i=0;
        p=ZXCharTable;

        while(i<strlen((const char*)in) && p)
        {
                c = (char)toupper(in[i]);
                if (c==' ') c='_';

                p = strchr(ZXCharTable, c);
                if (p) *(Out++) = (BYTE)(p - ZXCharTable);
                i++;
        }


        Out--;
        *Out |= 128;
}


int ZX81Strlen(BYTE *str)
{
        int len=1;

        while ( !((*str)&128) )
        {
                len++;
                str++;
        }
        return(len);
}

//---------------------------------------------------------------------------
AnsiString RemoveExt(AnsiString Fname)
{
        AnsiString Ext;
        int len,pos;

        len=Fname.Length();

        if (len<3) return(Fname);

        pos=len;
        while(pos)
        {
                if (Fname[pos]=='.') break;
                pos--;
        }

        Ext = Fname.SubString(pos, 1+len-pos);

        if ( (Ext==".p" || Ext==".P"
                || Ext==".T81" || Ext==".t81"
                || Ext==".P81" || Ext==".p81"
                || Ext==".A83" || Ext==".a83"
                || Ext==".o" || Ext==".O"
                || Ext==".81" || Ext==".80"
                || Ext==".TZX" || Ext==".tzx"
                || Ext==".TAP" || Ext==".tap"
                || Ext==".WAV" || Ext==".wav"
                || Ext==".sna" || Ext=="SNA"
                || Ext==".z80" || Ext==".Z80"
                || Ext==".ini" || Ext==".INI"
                || Ext==".mdr" || Ext==".MDR"
                || Ext==".mdv" || Ext==".MDV"
                || Ext==".dsk" || Ext==".DSK"
                || Ext==".img" || Ext==".IMG"
                || Ext==".opd" || Ext==".OPD"
                || Ext==".opu" || Ext==".OPU"
                || Ext==".mgt" || Ext==".MGT"
                || Ext==".trd" || Ext==".TRD"
                || Ext==".hdf" || Ext==".HDF"
                || Ext==".b81" || Ext==".B81"
                || Ext==".b82" || Ext==".B82"
                || Ext==".txt" || Ext==".TXT"
                || Ext==".bas" || Ext==".BAS"
                || Ext==".web" || Ext==".WEB"
                || Ext==".pdf" || Ext==".PDF"
                || Ext==".ref" || Ext==".REF"
                ) && pos>1)
                Fname = Fname.SubString(1,pos-1);

        return(Fname);
}

AnsiString GetExt(AnsiString Fname)
{
        AnsiString Ext;
        int len,pos;

        len=Fname.Length();

        if (len<3) return(Fname);

        pos=len;
        while(pos)
        {
                if (Fname[pos]=='.') break;
                pos--;
        }

        Ext = Fname.SubString(pos, 1+len-pos);

        for(pos=1; pos<=Ext.Length(); pos++) Ext[pos]=(char)toupper(Ext[pos]);
        return(Ext);
}
//---------------------------------------------------------------------------

AnsiString RemovePath(AnsiString Str)
{
        int p;

        if (Str[Str.Length()] == '\\') Str = "";

        do
        {
                p=Str.AnsiPos("\\");

                if (p)
                {
                        p++;
                        Str = Str.SubString(p, 1+Str.Length()-p);
                }
        } while(p);

        return(Str);
}

//---------------------------------------------------------------------------
// Enumerate the Serial Ports
//---------------------------------------------------------------------------

int EnumeratePorts(TStrings *List, AnsiString Type)
{
        int i;
        AnsiString PortName;
        bool Success;
        HANDLE Port;

        for (i=1; i<256; i++)
        {
                PortName="\\\\.\\"+Type; //COM";
                PortName+=i;

                Success = FALSE;
                Port = CreateFile(PortName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
                if (Port == INVALID_HANDLE_VALUE)
                {
                        DWORD dwError = GetLastError();
                        if (dwError == ERROR_ACCESS_DENIED || dwError == ERROR_GEN_FAILURE || dwError == ERROR_SHARING_VIOLATION)
                                Success = TRUE;
                }
                else
                {
                        Success = TRUE;
                        CloseHandle(Port);
                }

                if (Success)
                {
                        PortName=Type;
                        PortName+=i;
                        List->Add(PortName);
                }
        }
        return true;
}

//---------------------------------------------------------------------------

void AccessIniFileInteger(TIniFile* ini, IniFileAccessType accessType, AnsiString section, AnsiString entryName, int& entryValue)
{
        if (accessType == Write)
        {
                ini->WriteInteger(section, entryName, entryValue);
        }
        else
        {
                entryValue = ini->ReadInteger(section, entryName, entryValue);
        }
}

void AccessIniFileBoolean(TIniFile* ini, IniFileAccessType accessType, AnsiString section, AnsiString entryName, bool& entryValue)
{
        if (accessType == Write)
        {
                ini->WriteBool(section, entryName, entryValue);
        }
        else
        {
                entryValue = ini->ReadBool(section, entryName, entryValue);
        }
}

void AccessIniFileString(TIniFile* ini, IniFileAccessType accessType, AnsiString section, AnsiString entryName, AnsiString& entryValue)
{
        if (accessType == Write)
        {
                ini->WriteString(section, entryName, entryValue);
        }
        else
        {
                entryValue = ini->ReadString(section, entryName, entryValue);
        }
}

void AccessIniFileString(TIniFile* ini, IniFileAccessType accessType, AnsiString section, AnsiString entryName, char* entryValue)
{
        if (accessType == Write)
        {
                ini->WriteString(section, entryName, entryValue);
        }
        else
        {
                AnsiString value = ini->ReadString(section, entryName, entryValue);
                strcpy(entryValue, value.c_str());
        }
}

//---------------------------------------------------------------------------
// PNG

static void PNGPut32(unsigned char *p, unsigned long value)
{
        p[0] = (unsigned char)(value >> 24);
        p[1] = (unsigned char)(value >> 16);
        p[2] = (unsigned char)(value >> 8);
        p[3] = (unsigned char)value;
}

// A chunk: length, type, data, and the CRC of the type and the data
static bool PNGWriteChunk(FILE *f, const char *type, const unsigned char *data, unsigned long length)
{
        unsigned char word[4];
        unsigned long crc;

        PNGPut32(word, length);
        if (fwrite(word, 1, 4, f) != 4) return false;
        if (fwrite(type, 1, 4, f) != 4) return false;
        if (length && fwrite(data, 1, length, f) != length) return false;

        crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, (const Bytef *)type, 4);
        if (length) crc = crc32(crc, (const Bytef *)data, length);
        PNGPut32(word, crc);
        return fwrite(word, 1, 4, f) == 4;
}

bool SavePNG(const AnsiString &filename, int width, int height, const unsigned char *rgb)
{
        static const unsigned char signature[8] =
                { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
        unsigned long palette[256];
        unsigned char *index = NULL, *raw = NULL, *packed = NULL;
        unsigned char header[13], plte[256 * 3];
        int colours = 0, depth, rowBytes, x, y, i, last = -1;
        unsigned long rawLength;
        uLongf packedLength;
        bool ok = false;
        FILE *f = NULL;

        if (width <= 0 || height <= 0 || !rgb) return false;

        // The colours: a palette if there are no more than 256
        index = new unsigned char[width * height];
        for (i=0; i<width*height; i++)
        {
                unsigned long colour = ((unsigned long)rgb[i*3] << 16)
                                     | ((unsigned long)rgb[i*3+1] << 8) | rgb[i*3+2];
                int c;

                if (last >= 0 && palette[last] == colour)
                {
                        c = last;
                }
                else
                {
                        for (c=0; c<colours && palette[c]!=colour; c++)
                                ;
                        if (c == colours)
                        {
                                if (colours == 256) { colours = 257; break; }
                                palette[colours++] = colour;
                        }
                        last = c;
                }
                index[i] = (unsigned char)c;
        }

        if (colours <= 256)
        {
                depth = colours <= 2 ? 1 : colours <= 4 ? 2 : colours <= 16 ? 4 : 8;
                rowBytes = (width * depth + 7) / 8;
        }
        else
        {
                depth = 8;
                rowBytes = width * 3;
        }

        // The lines, each with filter type 0 (none) before it
        rawLength = (unsigned long)(rowBytes + 1) * height;
        raw = new unsigned char[rawLength];
        memset(raw, 0, rawLength);

        for (y=0; y<height; y++)
        {
                unsigned char *line = raw + (unsigned long)y * (rowBytes + 1) + 1;

                if (colours <= 256)
                {
                        for (x=0; x<width; x++)
                        {
                                int bit = x * depth;
                                line[bit >> 3] |= (unsigned char)
                                        (index[y * width + x] << (8 - depth - (bit & 7)));
                        }
                }
                else
                {
                        memcpy(line, rgb + (unsigned long)y * width * 3, width * 3);
                }
        }

        packedLength = compressBound(rawLength);
        packed = new unsigned char[packedLength];
        if (compress2(packed, &packedLength, raw, rawLength, 9) != Z_OK) goto done;

        f = fopen(filename.c_str(), "wb");
        if (!f) goto done;

        PNGPut32(header, width);
        PNGPut32(header + 4, height);
        header[8] = (unsigned char)depth;
        header[9] = (unsigned char)(colours <= 256 ? 3 : 2);   // palette / RGB
        header[10] = 0;                                         // deflate
        header[11] = 0;                                         // filters
        header[12] = 0;                                         // no interlace

        if (fwrite(signature, 1, 8, f) != 8) goto done;
        if (!PNGWriteChunk(f, "IHDR", header, 13)) goto done;

        if (colours <= 256)
        {
                for (i=0; i<colours; i++)
                {
                        plte[i*3]   = (unsigned char)(palette[i] >> 16);
                        plte[i*3+1] = (unsigned char)(palette[i] >> 8);
                        plte[i*3+2] = (unsigned char)palette[i];
                }
                if (!PNGWriteChunk(f, "PLTE", plte, colours * 3)) goto done;
        }

        if (!PNGWriteChunk(f, "IDAT", packed, packedLength)) goto done;
        if (!PNGWriteChunk(f, "IEND", NULL, 0)) goto done;

        ok = true;

done:
        if (f && fclose(f) != 0) ok = false;
        if (f && !ok) remove(filename.c_str());
        delete [] index;
        delete [] raw;
        delete [] packed;
        return ok;
}

bool SaveBitmapAsPNG(Graphics::TBitmap *bitmap, const AnsiString &filename)
{
        Graphics::TBitmap *copy;
        unsigned char *rgb;
        int x, y, w, h;
        bool ok;

        if (!bitmap) return false;

        // As 24 bits a pixel, which ScanLine gives as B, G, R
        copy = new Graphics::TBitmap();
        copy->Assign(bitmap);
        copy->PixelFormat = pf24bit;

        w = copy->Width;
        h = copy->Height;
        rgb = new unsigned char[(w > 0 && h > 0) ? w * h * 3 : 1];

        for (y=0; y<h; y++)
        {
                unsigned char *line = (unsigned char *)copy->ScanLine[y];

                for (x=0; x<w; x++)
                {
                        rgb[(y * w + x) * 3]     = line[x * 3 + 2];
                        rgb[(y * w + x) * 3 + 1] = line[x * 3 + 1];
                        rgb[(y * w + x) * 3 + 2] = line[x * 3];
                }
        }

        ok = SavePNG(filename, w, h, rgb);

        delete [] rgb;
        delete copy;
        return ok;
}
