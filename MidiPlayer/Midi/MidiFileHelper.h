//--------------------------------------------------
//File Name:MidiFileHelper.h
//--------------------------------------------------
#ifndef	_MIDI_FILEHELPER_H_
#define	_MIDI_FILEHELPER_H_
//--------------------------------------------------
#include <string>
#include <fstream>
#include <iostream>
//--------------------------------------------------
using namespace std;
//--------------------------------------------------
int midiBytesToInt(const std::string& strBytes)
{
    if (strBytes.length() != 2)
		return 0;

    int intRet = ((int)(((MidiByte)strBytes[0]) << 8)) |
                 ((int)(((MidiByte)strBytes[1])));
    return intRet;
}
//--------------------------------------------------
long midiBytesToLong(const std::string& strBytes)
{
    if (strBytes.length() != 4) 
		return 0;

    long longRet = ((long)(((MidiByte)strBytes[0]) << 24)) |
                   ((long)(((MidiByte)strBytes[1]) << 16)) |
                   ((long)(((MidiByte)strBytes[2]) << 8))  |
                   ((long)((MidiByte)(strBytes[3])));

    return longRet;
}
//--------------------------------------------------
long getNumberFromMidiBytes(std::ifstream* midiFile, int firstByte = -1)
{
	return 0;
}
//--------------------------------------------------
MidiByte getMidiByte(std::ifstream* midiFile)
{
	return 0;
}
//--------------------------------------------------
std::string getMidiBytes(std::ifstream* midiFile, unsigned long numberOfBytes)
{
    string stringRet;
    char fileMidiByte;
	static int bytesGot = 0; // purely for progress reporting purposes

    if (midiFile->eof())
		return "";

    while (stringRet.length() < numberOfBytes &&
            midiFile->read(&fileMidiByte, 1))
	{
        stringRet += fileMidiByte;
    }

    // if we've reached the end of file without fulfilling the
    // quota then panic as our parsing has performed incorrectly
    //
    if (stringRet.length() < numberOfBytes) 
        stringRet = "";

    return stringRet;
}
//--------------------------------------------------
int skipToNextTrack(ifstream *midiFile)
{
	if (!midiFile || !midiFile->is_open())
		return -1;

	int iTrackByteCnt = -1;
    while (!midiFile->eof()) 
	{
        string strTrackHeader = getMidiBytes(midiFile, 4);

        if (strTrackHeader.compare(0, 4, MIDI_TRACK_HEADER) == 0) 
		{
            iTrackByteCnt = midiBytesToLong(getMidiBytes(midiFile, 4));
			break;
        }
    }
	
	return iTrackByteCnt;
}
//--------------------------------------------------
#endif	//_MIDI_FILEHELPER_H_
//--------------------------------------------------