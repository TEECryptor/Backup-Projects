// MyMidiSampleDlg.h : header file
//

#pragma once

#include "Effect.h"
#include "SpecWave.h" 
/***************************************/
#include <vector>
#include <mmsystem.h> 

#define MThd		0x6468544D		// Start of file "MThd"
#define MTrk		0x6B72544D		// Start of track "MTrk"

// Macros for swapping hi/lo-endian data
//
#define WORDSWAP(w)		(((w) >> 8) | \
						(((w) << 8) & 0xFF00))

#define DWORDSWAP(dw)	(((dw) >> 24) | \
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


// These structures are stored in MIDI files; they need to be byte aligned.
//
#pragma pack(1)
typedef struct tagMidiFileHeader
{
    WORD	wFormat;				// Format (hi-lo)
    WORD	wTrackCount;			// # tracks (hi-lo)
    WORD	wTimeDivision;			// Time division (hi-lo)
}MidiFileHeader, *LPMIDIFILEHEADER;
#pragma pack() // End of need for byte-aligned structures

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

typedef std::vector<LPMIDIEVENT> PtrEventArray;
/***************************************/


// CMyMidiSampleDlg dialog
class CMyMidiSampleDlg : public CDialog
{
// Construction
public:
	CMyMidiSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MYMIDISAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	static void	CALLBACK MidiProc(HMIDIOUT hMidi, UINT uMsg, DWORD dwInstanceData, DWORD dwParam1, DWORD dwParam2);
	//----------------------------------------------
	BOOL	LoadMidiFile(LPCTSTR lpszFileName);
	BOOL	ParseMidiData(LPBYTE lpMidiData, DWORD dwByteSize);
	BOOL	ParseMidiTracks(LPMIDITRACK lpTracks, DWORD dwTrackCount);
	BOOL	GetTrackEvent(LPMIDITRACK lpTrack, LPMIDIEVENT lpEvent);
	BOOL	GetTrackByte(LPMIDITRACK lpTrack, LPBYTE lpbyByte);
	BOOL	GetTrackVDWord(LPMIDITRACK lpTrack, LPDWORD lpdw);
	//----------------------------------------------
	BOOL	PlayMidiFile();
	void	ClearMidiData();
	LONG	GetEventsByDeltaTime(DWORD dwDeltaTime, LPDWORD lpEvents);
	DWORD	SendMidiEventsToDevice(HMIDIOUT hMidiOut, LPMIDIEVENT lpEvent, DWORD dwEventCnt);
	DWORD	SendMidiEventsToEffect(CEffect* pEffect, LPMIDIEVENT lpEvent, DWORD dwEventCnt);
	DWORD	SendAudioDataToWaveDevice(float** pWavData, DWORD dwLenght);
	//----------------------------------------------
	void	UpdateMidiInfo();
	void	MySleep(LONGLONG llMircsecond);
	BOOL	LoadMidiEffect(LPCTSTR lpszEffectFile);
	//----------------------------------------------
public:
	DWORD	OnPlay();
	long	MidiMasterCallback(AEffect *effect, long opcode, long index, long value, void *ptr, float opt);
	void	OnMidiOutDone(MIDIHDR* pHdr);
	bool	OnCanDo(const char *ptr);
	//----------------------------------------------
private:
	DWORD	m_dwFormat;
	DWORD	m_dwTrackCount;
	DWORD	m_dwTimeDivision;
	DWORD	m_dwCurrentTempo;
	//
	DWORD	m_dwDuration;
	PtrEventArray	m_paEvents;
	//
	HANDLE		m_hPlayThread;
	HMIDIOUT	m_hMidiOut;
	//		
	CWnd*		m_pEffectWnd;
	CEffect*	m_pEffect;
	float**		m_fInputData;
	float**		m_fOutputData;
	//
	CSpecWaveOutDevice* m_pWaveOut;
	//----------------------------------------------
public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnOpen3();
	afx_msg void OnBnClickedBtnEffect();
	afx_msg void OnBnClickedBtnShowvsti();
	afx_msg void OnTimer(UINT nIDEvent);
};
//--------------------------------------------------
