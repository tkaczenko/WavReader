/*
 * WavReaderQt.cpp
 *
 * Copyright 2015 Andrei Tkachenko(@tkacz-) include.free@gmail.com
 *
 * This program open and read wav-file: reading wav header and data from the files (.wav)
 *
 *
 * The header for a WAV file looks like this:
 *
 * The "RIFF" chunk descriptor:
 * 1 - 4	"RIFF". Marks the file as a riff file. Characters are each 1 byte long.
 * 5 - 8	File size (integer)	Size of the overall file - 8 bytes, in bytes (32-bit integer).
 * 9 -12	"WAVE"	File Type Header. For our purposes, it always equals "WAVE".
 *
 * The "fmt" sub-chunk:
 * 13-16	"fmt "	Format chunk marker. Includes trailing null
 * 17-20	16	Length of format data as listed above
 * 21-22	1	Type of format (1 is PCM) - 2 byte integer
 * 23-24	2	Number of Channels - 2 byte integer
 * 25-28	44100	Sample Rate - 32 byte integer. CSample Rate = Number of Samples per second, or Hertz.
 * 29-32	176400	(Sample Rate * BitsPerSample * Channels) / 8. Byte rate
 * 33-34	4	(BitsPerSample * Channels) / 8.1 - 8 bit mono2 - 8 bit stereo/16 bit mono4 - 16 bit stereo
 * 35-36	16	Bits per sample
 *
 * The "data" sub-chunk:
 * 37-40	"data" Marks the beginning of the data section.
 * 41-44	File size (data). Size of the data section.
 * 44-..    Data
 *
 * The file could have another sub-chunks.
 *
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include "string.h"
#include "mem.h"

//Wav Header
struct wav_header_t
{
    char chunkID[4]; //"RIFF" = 0x46464952
    unsigned long chunkSize; //28 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes] + sum(sizeof(chunk.id) + sizeof(chunk.size) + chunk.size)
    char format[4]; //"WAVE" = 0x45564157
    char subchunk1ID[4]; //"fmt " = 0x20746D66
    unsigned long subchunk1Size; //16 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes]
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned long sampleRate;
    unsigned long byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    //[WORD wExtraFormatBytes;]
    //[Extra format bytes]
};

//Chunks
struct chunk_t
{
    char ID[4]; //"data" = 0x61746164
    unsigned long size;  //Chunk data bytes
};

void WavReader (const char* fileName, const char* fileToSave)
{
    FILE *fin = fopen(fileName, "rb");

    //Read WAV header
    wav_header_t header;
    fread(&header, sizeof(header), 1, fin);

    //Print WAV header
    printf("WAV File Header read:\n");
    printf("File Type: %s\n", header.chunkID);
    printf("File Size: %ld\n", header.chunkSize);
    printf("WAV Marker: %s\n", header.format);
    printf("Format Name: %s\n", header.subchunk1ID);
    printf("Format Length: %ld\n", header.subchunk1Size );
    printf("Format Type: %hd\n", header.audioFormat);
    printf("Number of Channels: %hd\n", header.numChannels);
    printf("Sample Rate: %ld\n", header.sampleRate);
    printf("Sample Rate * Bits/Sample * Channels / 8: %ld\n", header.byteRate);
    printf("Bits per Sample * Channels / 8.1: %hd\n", header.blockAlign);
    printf("Bits per Sample: %hd\n", header.bitsPerSample);

    //skip wExtraFormatBytes & extra format bytes
    //fseek(f, header.chunkSize - 16, SEEK_CUR);

    //Reading file
    chunk_t chunk;
    printf("id\t" "size\n");
    //go to data chunk
    while (true)
    {
        fread(&chunk, sizeof(chunk), 1, fin);
        printf("%c%c%c%c\t" "%li\n", chunk.ID[0], chunk.ID[1], chunk.ID[2], chunk.ID[3], chunk.size);
        if (*(unsigned int *)&chunk.ID == 0x61746164)
            break;
        //skip chunk data bytes
        fseek(fin, chunk.size, SEEK_CUR);
    }

    //Number of samples
    int sample_size = header.bitsPerSample / 8;
    int samples_count = chunk.size * 8 / header.bitsPerSample;
    printf("Samples count = %i\n", samples_count);

    short int *value = new short int[samples_count];
    memset(value, 0, sizeof(short int) * samples_count);

    //Reading data
    for (int i = 0; i < samples_count; i++)
    {
        fread(&value[i], sample_size, 1, fin);
    }

    //Write data into the file
    FILE *fout = fopen(fileToSave, "w");
    for (int i = 0; i < samples_count; i++)
    {
        fprintf(fout, "%d\n", value[i]);
    }
    fclose(fin);
    fclose(fout);
}

int main()
{
    WavReader("E:/TEMP/20_21_converted.wav", "list.dat");
    return 0;
}
