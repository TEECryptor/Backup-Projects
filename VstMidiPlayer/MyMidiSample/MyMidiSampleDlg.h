// MyMidiSampleDlg.h : header file
//

#pragma once

#include "Effect.h"
#include "SpecWave.h" 
/***************************************/
#include "MidiFileLoader.h"
/*
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
*/
#define PM_QS_POSTMESSAGE   ((QS_POSTMESSAGE | QS_HOTKEY | QS_TIMER) << 16)
#define	PROC_MSG_START			WM_USER + 100
#define	PROC_MSG_PLAY			WM_USER + 101
#define	PROC_MSG_PAUSE			WM_USER + 102
#define	PROC_MSG_STOP			WM_USER + 103
#define	PROC_MSG_END			WM_USER + 104

const int ciMaxEventCnt = 512;

enum ePlayState
{
	ePlayState_Ready	= 0,
	ePlayState_Playing	= 1,
	ePlayState_Paused	= 2,
	ePlayState_Stopped	= 3,
	ePlayState_Cnt
};

typedef std::vector<CEffect*> EffectPtrArray;

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
	static DWORD WINAPI ProcessThread(LPVOID lpParam);
	static void	CALLBACK MidiProc(HMIDIOUT hMidi, UINT uMsg, DWORD dwInstanceData, DWORD dwParam1, DWORD dwParam2);
	//----------------------------------------------
	BOOL	LoadMidiFile(LPCTSTR lpszFileName);
	BOOL	ParseMidiData(LPBYTE lpMidiData, DWORD dwByteSize);
	BOOL	ParseMidiTracks(LPMIDITRACK lpTracks, DWORD dwTrackCount);
	BOOL	GetTrackEvent(LPMIDITRACK lpTrack, LPMIDIEVENT lpEvent);
	BOOL	GetTrackByte(LPMIDITRACK lpTrack, LPBYTE lpbyByte);
	BOOL	GetTrackVDWord(LPMIDITRACK lpTrack, LPDWORD lpdw);
	//----------------------------------------------
	void	ClearMidiData();
	LONG	GetEventsByDeltaTime(DWORD dwDeltaTime);
	DWORD	SendMidiEventsToDevice(DWORD dwEventCnt);
	DWORD	SendMidiEventsToEffect();
	DWORD	SendAudioDataToWaveDevice(float** pWavData, DWORD dwLenght);
	//----------------------------------------------
	void	UpdateMidiInfo();
	void	UpdateCurTimeInfo(DWORD dwCurTime);
	void	CreateBufferForVSTi();
	void	DeleteBufferForVSTi();
	void	ShowVSTiWindow();
	void	UpdateUIControl(DWORD dwStatus);
	CEffect*GetEffectByIndex(DWORD dwIndex);
	void	MySleep(LONGLONG llMircsecond);
	BOOL	LoadMidiEffect(LPCTSTR lpszEffectFile);
	//----------------------------------------------
public:
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
	EffectPtrArray	m_paEffect;
	CEffect*	m_pCurEffect;
	float**		m_fInputData;
	float**		m_fOutputData;
	int			m_iCurMidiEventCnt;
	MidiEvent	m_midiEvents[ciMaxEventCnt];
	//
	CSpecWaveOutDevice* m_pWaveOut;
	//
	DWORD		m_dwProcThreadID;
	HANDLE		m_hProcessThread;
	HANDLE		m_hProcessEvent;
	CRITICAL_SECTION cs;
	//
	CRect		m_rcMainDlg;
	CRect		m_rcFileGroup;
	CRect		m_rcVSTiGourp;
	CRect		m_rcMidiFileEdit;
	CRect		m_rcOpenMidiBtn;
	CRect		m_rcMidiInfoList;
	CRect		m_rcPlayMidiBtn;
	CRect		m_rcStopMidiBtn;
	CRect		m_rcVSTiComboBox;
	CRect		m_rcAddVSTiBtn;
	CRect		m_rcVSTiWnd;
	CRect		m_rcOKBtn;
	//----------------------------------------------
public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnEffect();
	afx_msg void OnCbnSelchangeComboVsti();
	afx_msg void OnBnClickedBtnStop();
};
//--------------------------------------------------
