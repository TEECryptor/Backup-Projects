//--------------------------------------------------
//	File Name: MLVSTIMidiFileImpl.h
//	Introduce: VSTi midi file implement class
//	Date: 2010-08-18
//--------------------------------------------------
#ifndef	_MIDIFILE_LOADER_
#define	_MIDIFILE_LOADER_
//--------------------------------------------------
#include "MLVSTiPlayer.h"
#include "CommonDef.h"
//--------------------------------------------------
#include <vector>
#include <TCHAR.h>
#include <mmsystem.h> 
#define MThd		0x6468544D		// Start of file "MThd"
#define MTrk		0x6B72544D		// Start of track "MTrk"
//--------------------------------------------------

// Macros for swapping hi/lo-endian data
//
#define WORDSWAP(w)				(((w) >> 8) | \
								(((w) << 8) & 0xFF00))

#define DWORDSWAP(dw)			(((dw) >> 24) | \
								(((dw) >> 8) & 0x0000FF00) | \
								(((dw) << 8) & 0x00FF0000) | \
								(((dw) << 24) & 0xFF000000))

#define MIDI_CTRLCHANGE			((BYTE)0xB0)		// + ctrlr + value
#define MIDI_PRGMCHANGE			((BYTE)0xC0)		// + new patch
#define MIDI_CHANPRESS			((BYTE)0xD0)		// + pressure (1 byte)

#define MIDICTRL_VOLUME			((BYTE)0x07)

#define MIDIEVENT_CHANNEL(dw)	(dw & 0x0000000F)
#define MIDIEVENT_TYPE(dw)		(dw & 0x000000F0)
#define MIDIEVENT_DATA1(dw)		((dw & 0x0000FF00) >> 8)
#define MIDIEVENT_VOLUME(dw)	((dw & 0x007F0000) >> 16)

#define MIDI_SYSEX				((BYTE)0xF0)		// SysEx begin
#define MIDI_SYSEXEND			((BYTE)0xF7)		// SysEx end
#define MIDI_META				((BYTE)0xFF)		// Meta event begin
#define MIDI_META_TEMPO			((BYTE)0x51)		// Tempo change
#define MIDI_META_EOT			((BYTE)0x2F)		// End-of-track
//--------------------------------------------------
#pragma pack(1)
typedef struct tagMidiFileHeader
{
    WORD	wFormat;				// Format (hi-lo)
    WORD	wTrackCount;			// # tracks (hi-lo)
    WORD	wTimeDivision;			// Time division (hi-lo)
}MidiFileHeader, *LPMIDIFILEHEADER;
#pragma pack() // End of need for byte-aligned structures
//--------------------------------------------------
typedef struct tagMidiEvent
{
	DWORD	dwTime;					// Absolute time of event, in mircosecond
	BYTE	byShortData[4];			// Event type and parameters if channel msg
	DWORD	dwEventLength;			// Length of data which follows if meta or sysex
	LPBYTE	pLongData;				// -> Event data if applicable

	tagMidiEvent()
		: dwTime(0)
		, dwEventLength(0)
		, pLongData(NULL)
	{
		memset(byShortData, 0, 4*sizeof(BYTE));
	}
	~tagMidiEvent()
	{
		if (NULL != pLongData)
		{
			delete []pLongData;
			pLongData = NULL;
		}
	}
}MidiEvent, *LPMIDIEVENT;
//--------------------------------------------------
typedef struct tagMidiTrack
{	
	DWORD	dwTrackLength;			// Total bytes in track
	LPBYTE	pTrackStart;			// -> start of track data buffer
	LPBYTE	pTrackCurrent;			// -> next byte to read in buffer
	DWORD	dwNextEventTime;		// Absolute time of next event in track
	BYTE	byRunningStatus;		// Running status from last channel msg
	BOOL	bIsAtEnd;				// Is at the end of track?

	tagMidiTrack()
		: dwTrackLength(0)
		, pTrackStart(NULL)
		, pTrackCurrent(NULL)
		, dwNextEventTime(0)
		, byRunningStatus(0)
		, bIsAtEnd(FALSE)
	{
	}
}MidiTrack, *LPMIDITRACK;
//--------------------------------------------------
typedef std::vector<LPMIDIEVENT> PtrEventArray;
//--------------------------------------------------


//--------------------------------------------------
//	Name: CMLVSTiMidiFileImpl
//	Introduce: This class can parse midi file
//--------------------------------------------------
class CMLVSTiMidiFileImpl : public IMLVSTiMidiFile
{
public:
	CMLVSTiMidiFileImpl(void);
	~CMLVSTiMidiFileImpl(void);
	//----------------------------------------------
public:
	void	GetMidiFileInfo(MidiFileInfo &Info);
	//----------------------------------------------
public:
	BOOL	LoadMidiFile(const char* lpszFileName);
	LPCTSTR	GetFileName();
	DWORD	GetMidiFormat();
	DWORD	GetTimeDivision();
	DWORD	GetTempo();
	DWORD	GetDuration();
	DWORD	GetTrackCount();
	DWORD	GetEventCount();
	void	GetEvents(LPMIDIEVENT lpEvents, DWORD &dwCount);
	DWORD	GetMetaEventCount();
	void	GetMetaEvents(LPMIDIEVENT lpEvents, DWORD &dwCount);
	//
	long	PerpareEventsByDeltaTime(DWORD dwDeltaTime);
	long	SendCurEventsToEffect();
	//----------------------------------------------
private:	
	BOOL	ParseMidiData(LPBYTE lpMidiData, DWORD dwByteSize);
	BOOL	ParseMidiTracks(LPMIDITRACK lpTracks, DWORD dwTrackCount);
	BOOL	GetTrackEvent(LPMIDITRACK lpTrack, LPMIDIEVENT lpEvent);
	BOOL	GetTrackByte(LPMIDITRACK lpTrack, LPBYTE lpbyByte);
	BOOL	GetTrackVDWord(LPMIDITRACK lpTrack, LPDWORD lpdw);
	void	ClearMidiData();
	//----------------------------------------------
private:
	char	m_csFileName[MAX_PATH];
	DWORD	m_dwFormat;
	DWORD	m_dwTrackCount;
	DWORD	m_dwTimeDivision;
	DWORD	m_dwCurrentTempo;
	DWORD	m_dwDuration;
	PtrEventArray	m_paEvents;
	//	
	int			m_iCurMidiEventCnt;
	MidiEvent	m_midiEvents[VSTI_MAX_EVENTCNT];
	DWORD		m_dwPlayingThreadID;
	HANDLE		m_hPlayingThread;
	HANDLE		m_hPlayingEvent;
	//----------------------------------------------
};
//--------------------------------------------------
#endif	//_MIDIFILE_LOADER_
//--------------------------------------------------
