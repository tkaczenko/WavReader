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

#include <QCoreApplication>
#include <QtEndian>
#include <QDebug>
#include <QFile>

//Wav Header
struct wav_header_t
{
    char chunkId[4]; //"RIFF" = 0x46464952
    quint32 chunkSize; //28 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes] + sum(sizeof(chunk.id) + sizeof(chunk.size) + chunk.size)
    char format[4]; //"WAVE" = 0x45564157
    char subchunk1ID[4]; //"fmt " = 0x20746D66
    quint32 subchunk1Size; //16 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes]
    quint16 audioFormat;
    quint16 numChannels;
    quint32 sampleRate;
    quint32 byteRate;
    quint16 blockAlign;
    quint16 bitsPerSample;
    //[quint16 wExtraFormatBytes;]
    //[Extra format bytes]
};

wav_header_t wavHeader;

void ReadWav (const QString fileName, const QString fileToSave)
{
    if (fileName != "") {
        QFile wavFile(fileName);
        if (!wavFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Error: Could not open file!";
            return;
        }

        //Read WAV file header
        QDataStream analyzeHeader (&wavFile);
        analyzeHeader.setByteOrder(QDataStream::LittleEndian);
        analyzeHeader.readRawData(wavHeader.chunkId, 4); // "RIFF"
        analyzeHeader >> wavHeader.chunkSize; // File Size
        analyzeHeader.readRawData(wavHeader.format,4); // "WAVE"
        analyzeHeader.readRawData(wavHeader.subchunk1ID,4); // "fmt"
        analyzeHeader >> wavHeader.subchunk1Size; // Format length
        analyzeHeader >> wavHeader.audioFormat; // Format type
        analyzeHeader >> wavHeader.numChannels; // Number of channels
        analyzeHeader >> wavHeader.sampleRate; // Sample rate
        analyzeHeader >> wavHeader.byteRate; // (Sample Rate * BitsPerSample * Channels) / 8
        analyzeHeader >> wavHeader.blockAlign; // (BitsPerSample * Channels) / 8.1
        analyzeHeader >> wavHeader.bitsPerSample; // Bits per sample

        //Print WAV header
        qDebug() << "WAV File Header read:";
        qDebug() << "File Type: " << wavHeader.chunkId;
        qDebug() << "File Size: " << wavHeader.chunkSize;
        qDebug() << "WAV Marker: " << wavHeader.format;
        qDebug() << "Format Name: " << wavHeader.subchunk1ID;
        qDebug() << "Format Length: " << wavHeader.subchunk1Size;
        qDebug() << "Format Type: " << wavHeader.audioFormat;
        qDebug() << "Number of Channels: " << wavHeader.numChannels;
        qDebug() << "Sample Rate: " <<  wavHeader.sampleRate;
        qDebug() << "Sample Rate * Bits/Sample * Channels / 8: " << wavHeader.byteRate;
        qDebug() << "Bits per Sample * Channels / 8.1: " << wavHeader.blockAlign;
        qDebug() << "Bits per Sample: " << wavHeader.bitsPerSample;

        //Search data chunk
        quint32 chunkDataSize = 0;
        QByteArray temp_buff;
        char buff[0x04];
        while (true)
        {
            QByteArray tmp = wavFile.read(0x04);
            temp_buff.append(tmp);
            int idx = temp_buff.indexOf("data");
            if (idx >= 0)
            {
                int lenOfData = temp_buff.length() - (idx + 4);
                memcpy(buff, temp_buff.constData() + idx + 4, lenOfData);
                int bytesToRead = 4 - lenOfData;
                // finish readind size of chunk
                if (bytesToRead > 0)
                {
                    int read = wavFile.read(buff + lenOfData, bytesToRead);
                    if (bytesToRead != read)
                    {
                        qDebug() << "Error: Something awful happens!";
                        return;
                    }
                }
                chunkDataSize = qFromLittleEndian<quint32>((const uchar*)buff);
                break;
            }
            if (temp_buff.length() >= 8)
            {
                temp_buff.remove(0, 0x04);
            }
        }
        if (!chunkDataSize)
        {
            qDebug() << "Error: Chunk data not found!";
            return;
        }

        //Reading data from the file
        int samples = 0;
        QFile saveFile(fileToSave);
        saveFile.open(QIODevice::WriteOnly);
        if (wavHeader.numChannels == 2)
        {
            saveFile.write("L\t R\r\n");
        }
        while (wavFile.read(buff, 0x04) > 0)
        {
            chunkDataSize -= 4;
            ++samples;
            qint16 sampleChannel1 = qFromLittleEndian<qint16>((const uchar*)buff);
            qint16 sampleChannel2 = qFromLittleEndian<qint16>((const uchar*)(buff + 2));
            switch (wavHeader.numChannels) {
            case 1:
                saveFile.write(QString("%1\r\n %2\r\n").arg(sampleChannel1).arg(sampleChannel2).toUtf8());
                break;
            case 2:
                saveFile.write(QString("%1\t %2\r\n").arg(sampleChannel1).arg(sampleChannel2).toUtf8());
                break;
            }
            // check the end of the file
            if (chunkDataSize == 0 || chunkDataSize & 0x80000000)
            {
                break;
            }
        }
        qDebug() << "Readed " << samples << " samples...";
        wavFile.close();
        if (saveFile.isOpen())
        {
            saveFile.close();
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ReadWav("test.wav", "list.dat");
    return a.exec();
}
