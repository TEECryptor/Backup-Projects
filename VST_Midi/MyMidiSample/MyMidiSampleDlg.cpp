// MyMidiSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyMidiSample.h"
#include "MyMidiSampleDlg.h"
#include ".\mymidisampledlg.h"
#include "EffectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI PlayThreadFunc(LPVOID lpParam) 
{ 
	CMyMidiSampleDlg* pDlg = (CMyMidiSampleDlg*)lpParam;
	if (NULL != pDlg)
		return pDlg->OnPlay();

    return 0; 
} 

const int g_iBuffSize = 4410;
CMyMidiSampleDlg* g_pMidiDlg = NULL;
long VSTCALLBACK AudioMasterCallback(AEffect *effect, long opcode, long index, long value, void *ptr, float opt)
{
	if (NULL != g_pMidiDlg)
		return g_pMidiDlg->MidiMasterCallback(effect, opcode, index, value, ptr, opt);

	return 0;
}	

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMyMidiSampleDlg dialog



CMyMidiSampleDlg::CMyMidiSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyMidiSampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dwFormat = 0;
	m_dwTrackCount = 0;
	m_dwTimeDivision = 0;
	m_dwCurrentTempo = 800000;
	m_dwDuration = 0;
	m_hPlayThread = NULL;
	m_hMidiOut = NULL;
	m_pEffect = NULL;
	m_fInputData = NULL;
	m_fOutputData = NULL;
	m_pEffectWnd = NULL;
	m_pWaveOut = NULL;

	g_pMidiDlg = this;
}

void CMyMidiSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyMidiSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBnClickedBtnOpen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PLAY, OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_EFFECT, OnBnClickedBtnEffect)
	ON_BN_CLICKED(IDC_BTN_SHOWVSTI, OnBnClickedBtnShowvsti)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMyMidiSampleDlg message handlers

BOOL CMyMidiSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CListCtrl* pListCtr = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
	pListCtr->SetExtendedStyle(pListCtr->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	pListCtr->InsertColumn(0, _T("Name"), LVCFMT_CENTER, 100);
	pListCtr->InsertColumn(1, _T("Value"), LVCFMT_CENTER, 100);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyMidiSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyMidiSampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyMidiSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CMyMidiSampleDlg::OnDestroy()
{
	CDialog::OnDestroy();

	ClearMidiData();

	if (m_pWaveOut)
	{
		m_pWaveOut->Close();
		delete m_pWaveOut;
		m_pWaveOut = NULL;
	}

	if (m_pEffectWnd)
	{
		m_pEffectWnd->DestroyWindow();
		delete m_pEffectWnd;
		m_pEffectWnd = NULL;
	}

	if (m_pEffect)
	{
		int iInputNum = m_pEffect->GetInputNum();
		int iOutputNum = m_pEffect->GetOutputNum();

		if (iInputNum > 0)
		{
			for(int iNum = 0; iNum < iInputNum; iNum++)
				delete []m_fInputData[iNum];
			delete []m_fInputData;
		}
		if (iOutputNum > 0)
		{
			for(int iNum = 0; iNum < iOutputNum; iNum++)
				delete []m_fOutputData[iNum];
			delete []m_fOutputData;
		}

		m_pEffect->EffStopProcess();
		delete m_pEffect;
		m_pEffect = NULL;
	}
}

//--------------------------------------------------





//--------------------------------------------------
void CMyMidiSampleDlg::MidiProc(HMIDIOUT hMidi, UINT uMsg, DWORD dwInstanceData, DWORD dwParam1, DWORD dwParam2)
{	
	CMyMidiSampleDlg *pMidiDlg = (CMyMidiSampleDlg *)dwInstanceData;
	ASSERT(pMidiDlg != 0);
	MIDIHDR *pHdr = (MIDIHDR*)dwParam1;

	switch(uMsg) 
	{
		case MOM_OPEN:
			//pMidi->OnMidiOutOpen();
			break;

		case MOM_CLOSE:
			//pMidi->OnMidiOutClose();
			break;

		case MOM_DONE:
			ASSERT(pHdr != 0);
			pMidiDlg->OnMidiOutDone(pHdr);
			break;

		case MOM_POSITIONCB:
			ASSERT(pHdr != 0);
			//pMidi->OnMidiOutPositionCB(*pHdr, *((MIDIEVENT*)(pHdr->lpData + pHdr->dwOffset)));
			break;

		default:
			break;
	}
}
//--------------------------------------------------
BOOL CMyMidiSampleDlg::LoadMidiFile(LPCTSTR lpszFileName)
{
	OFSTRUCT os;
	memset(&os, 0, sizeof(OFSTRUCT));
	os.cBytes = sizeof(OFSTRUCT);
	HANDLE hMidiFile = CreateFile(	lpszFileName, 
									GENERIC_READ, 
									FILE_SHARE_READ, 
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
									NULL);
	if (NULL == hMidiFile)
		return FALSE;

	DWORD dwFileSize = 0;
	dwFileSize = GetFileSize(hMidiFile, NULL);
	if (dwFileSize == 0)
		return FALSE;

	LPBYTE lpMidiFileData = new BYTE[dwFileSize];
	
	DWORD dwReadedBytes = 0;
	if (!ReadFile(hMidiFile, lpMidiFileData, dwFileSize, &dwReadedBytes, NULL) ||
		dwReadedBytes != dwFileSize)
	{
		CloseHandle(hMidiFile);
		delete []lpMidiFileData;
		return FALSE;
	}
	CloseHandle(hMidiFile);

	// Parse all data in midi file
	BOOL bRes = ParseMidiData(lpMidiFileData, dwFileSize);
	delete []lpMidiFileData;
	lpMidiFileData = NULL;

	return bRes;
}
//--------------------------------------------------
BOOL CMyMidiSampleDlg::ParseMidiData(LPBYTE lpMidiData, DWORD dwByteSize)
{
	if (NULL == lpMidiData || dwByteSize <= 0)
		return FALSE;

	register LPBYTE p = LPBYTE(lpMidiData);

	ClearMidiData();

	// Check header of MIDI
	if(*(DWORD*)p != MThd) {
		ASSERT(FALSE);
		return FALSE;
	}
	p += sizeof(DWORD);

	// Check header size
	DWORD dwHeaderSize = DWORDSWAP(*(DWORD*)p);
	if( dwHeaderSize != sizeof(MidiFileHeader) ) {
		ASSERT(FALSE);
		return FALSE;
	}
	p += sizeof(DWORD);

	// Get header
	MidiFileHeader stuMidiHeader;
	::CopyMemory(&stuMidiHeader, p, dwHeaderSize);
	m_dwFormat = DWORD(WORDSWAP(stuMidiHeader.wFormat));
	m_dwTrackCount = DWORD(WORDSWAP(stuMidiHeader.wTrackCount));
	m_dwTimeDivision = DWORD(WORDSWAP(stuMidiHeader.wTimeDivision));
	p += dwHeaderSize;

	// Create the array of tracks
	LPMIDITRACK lpTracks = new MidiTrack[m_dwTrackCount];
	for(register DWORD i = 0; i < m_dwTrackCount; ++i) 
	{
		// Check header of track
		if(*(DWORD*)p != MTrk) 
		{
			ASSERT(FALSE);
			return FALSE;
		}
		p += sizeof(DWORD);

		// Track bytes length
		lpTracks[i].dwTrackLength = DWORDSWAP(*(DWORD*)p);
		p += sizeof(DWORD);

		// Track start and current buffer pointers
		lpTracks[i].pTrackStart = lpTracks[i].pTrackCurrent = p;
		p += lpTracks[i].dwTrackLength;

        // Handle bozo MIDI files which contain empty track chunks
		if( !lpTracks[i].dwTrackLength ) {
			//m_Tracks[i].fdwTrack |= ITS_F_ENDOFTRK;
			continue;
		}

		// We always preread the time from each track so the mixer code can
		// determine which track has the next event with a minimum of work
		if( !GetTrackVDWord( &lpTracks[i], &lpTracks[i].dwNextEventTime )) 
		{
			TRACE0("Error in MIDI data\n");
			ASSERT(FALSE);
			return FALSE;
		}
	}

	// Parse tracks data to events
	BOOL bRes = ParseMidiTracks(lpTracks, m_dwTrackCount);
	delete []lpTracks;
	lpTracks = NULL;

	return bRes;
}
//--------------------------------------------------
BOOL CMyMidiSampleDlg::ParseMidiTracks(LPMIDITRACK lpTracks, DWORD dwTrackCount)
{
	if (NULL == lpTracks || 0 == dwTrackCount)
		return FALSE;

	int iEventCnt = 0;
	while(1)
	{
		LPMIDITRACK lpFoundTrack = NULL;
		DWORD dwNextTrackTime = 0xFFFFFFFFL;

		// Find nearest event due
		for (register DWORD dwIndex = 0; dwIndex < dwTrackCount; ++dwIndex) 
		{
			if (FALSE == lpTracks[dwIndex].bIsAtEnd && 
				lpTracks[dwIndex].dwNextEventTime < dwNextTrackTime) 
			{
				dwNextTrackTime = lpTracks[dwIndex].dwNextEventTime;
				lpFoundTrack = &lpTracks[dwIndex];
			}
		}
		
		// None found?  We must be done, so return to the caller with a smile.
		if (NULL == lpFoundTrack) 
		{
			return TRUE; //CONVERTERR_NOERROR;
		}

		// Update the total duration (in mircosecnd)
		m_dwDuration = lpFoundTrack->dwNextEventTime * (m_dwCurrentTempo/m_dwTimeDivision);

		LPMIDIEVENT lpNewEvent = new MidiEvent;

		// Ok, get the event header from that track
		if (!GetTrackEvent(lpFoundTrack, lpNewEvent)) 
		{
			// Warn future calls that this converter is stuck at a corrupt spot
			// and can't continue
			//m_dwStatus |= CONVERTF_STATUS_STUCK;
			delete lpNewEvent;
			return FALSE; //CONVERTERR_CORRUPT;
		}

		iEventCnt++;

		// Don't add end of track event 'til we're done
		if (lpNewEvent->byShortData[0] == MIDI_META && 
			lpNewEvent->byShortData[1] == MIDI_META_EOT) 
		{
			delete lpNewEvent;
			continue;
		}

		// Channel message.
		if (lpNewEvent->byShortData[0] < MIDI_SYSEX) 
		{
		}
		// SysEx event, ignore it.
		else if (	lpNewEvent->byShortData[0] == MIDI_SYSEX || 
					lpNewEvent->byShortData[0] == MIDI_SYSEXEND)
		{
			//delete lpNewEvent;
			//continue;
		}
		// Meta event, the only meta-event we care about is change tempo
		else
		{
			if (lpNewEvent->byShortData[1] == MIDI_META_TEMPO ) 
			{
				m_dwCurrentTempo =	lpNewEvent->pLongData[2] | 
									((DWORD)lpNewEvent->pLongData[1] << 8) |
									((DWORD)lpNewEvent->pLongData[0] << 16 );
			}
			delete lpNewEvent;
			continue;
		}

		// Add to event list
		m_paEvents.push_back(lpNewEvent);

	};

	return TRUE;
}
//--------------------------------------------------
// Fills in the event struct with the next event from the track
//--------------------------------------------------
BOOL CMyMidiSampleDlg::GetTrackEvent(LPMIDITRACK lpTrack, LPMIDIEVENT lpEvent)
{
	if (NULL == lpTrack || NULL == lpEvent)
		return FALSE;

	DWORD   idx;
	UINT    dwEventLength;

	// Clear out the temporary event structure to get rid of old data...
	memset(lpEvent, 0, sizeof(MidiEvent));

	// Already at end of track? There's nothing to read.
	if (lpTrack->bIsAtEnd)
		return FALSE;

	// Get the first byte, which determines the type of event.
	BYTE byByte;
	if (!GetTrackByte(lpTrack, &byByte))
		return FALSE;

	// If the high bit is not set, then this is a channel message
	// which uses the status byte from the last channel message
	// we saw. NOTE: We do not clear running status across SysEx or
	// meta events even though the spec says to because there are
	// actually files out there which contain that sequence of data.
	if (!(byByte & 0x80)) 
	{
		// No previous status byte? We're hosed.
		if (!lpTrack->byRunningStatus) 
		{
			//TrackError(lpTrack, gteBadRunStat);
			return FALSE;
		}

		lpEvent->byShortData[0] = lpTrack->byRunningStatus;
		lpEvent->byShortData[1] = byByte;

		byByte = lpEvent->byShortData[0] & 0xF0;
		lpEvent->dwEventLength = 2;

		// Only program change and channel pressure events are 2 bytes long;
		// the rest are 3 and need another byte
		if ((byByte != MIDI_PRGMCHANGE) && (byByte != MIDI_CHANPRESS)) 
		{
			if (!GetTrackByte(lpTrack, &lpEvent->byShortData[2]))
				return FALSE;
			++lpEvent->dwEventLength;
		}
	} 
	else if ((byByte & 0xF0) != MIDI_SYSEX) 
	{
		// Not running status, not in SysEx range - must be
		// normal channel message (0x80-0xEF)
		lpEvent->byShortData[0] = byByte;
		lpTrack->byRunningStatus = byByte;

		// Strip off channel and just keep message type
		byByte &= 0xF0;

		dwEventLength = (byByte == MIDI_PRGMCHANGE || byByte == MIDI_CHANPRESS) ? 1 : 2;
		lpEvent->dwEventLength = dwEventLength + 1;

		if (!GetTrackByte(lpTrack, &lpEvent->byShortData[1]))
			return FALSE;

		if (dwEventLength == 2)
		{
			if (!GetTrackByte(lpTrack, &lpEvent->byShortData[2]))
				return FALSE;
		}
	} 
	else if ((byByte == MIDI_SYSEX) || (byByte == MIDI_SYSEXEND)) 
	{
		// One of the SysEx types. (They are the same as far as we're concerned;
		// there is only a semantic difference in how the data would actually
		// get sent when the file is played. We must take care to put the proper
		// event type back on the output track, however.)
		//
		// Parse the general format of:
		//  BYTE 	bEvent (MIDI_SYSEX or MIDI_SYSEXEND)
		//  VDWORD 	cbParms
		//  BYTE   	abParms[cbParms]
		lpEvent->byShortData[0] = byByte;
		if (!GetTrackVDWord(lpTrack, &lpEvent->dwEventLength)) 
		{
			//TrackError(lpTrack, gteSysExLenTrunc);
			return FALSE;
		}

		// Malloc a temporary memory block to hold the parameter data
		lpEvent->pLongData = new BYTE[lpEvent->dwEventLength];
		if (lpEvent->pLongData == 0) 
		{
			//TrackError(lpTrack, gteNoMem);
			return FALSE;
		}

		// Copy from the input buffer to the parameter data buffer
		for (idx = 0; idx < lpEvent->dwEventLength; idx++)
		{
			if (!GetTrackByte(lpTrack, lpEvent->pLongData + idx))
			{
				//TrackError(lpTrack, gteSysExTrunc);
				return FALSE;
			}
		}
	} 
	else if( byByte == MIDI_META ) 
	{
		// It's a meta event. Parse the general form:
		//  BYTE	bEvent	(MIDI_META)
		//  BYTE	bClass
		//  VDWORD	cbParms
		//  BYTE	abParms[cbParms]
		lpEvent->byShortData[0] = byByte;

		if (!GetTrackByte( lpTrack, &lpEvent->byShortData[1]))
			return FALSE;

		if (!GetTrackVDWord( lpTrack, &lpEvent->dwEventLength)) 
		{	
			//TrackError(lpTrack, gteMetaLenTrunc);
			return FALSE;
		}

		// NOTE: It's perfectly valid to have a meta with no data
		// In this case, dwEventLength == 0 and pLongData == NULL
		if (lpEvent->dwEventLength) 
		{		
			// Malloc a temporary memory block to hold the parameter data
			lpEvent->pLongData = new BYTE[lpEvent->dwEventLength];
			if (lpEvent->pLongData == 0) 
			{
				//TrackError(lpTrack, gteNoMem);
				return FALSE;
			}

			// Copy from the input buffer to the parameter data buffer
			for (idx = 0; idx < lpEvent->dwEventLength; idx++)
			{
				if (!GetTrackByte(lpTrack, lpEvent->pLongData + idx))
				{
					//TrackError(lpTrack, gteMetaTrunc);
					return FALSE;
				}
			}
		}

		if (lpEvent->byShortData[1] == MIDI_META_EOT)
			lpTrack->bIsAtEnd = TRUE;
	} 
	else 
	{
		// Messages in this range are system messages and aren't supposed to
		// be in a normal MIDI file. If they are, we've either misparsed or the
		// authoring software is stupid.
		return FALSE;
	}

	// Event time was already stored as the current track time (in mircosecond)
	lpEvent->dwTime = lpTrack->dwNextEventTime * (m_dwCurrentTempo/m_dwTimeDivision);

	// Now update to the next event time. The code above MUST properly
	// maintain the end of track flag in case the end of track meta is
	// missing.  NOTE: This code is a continuation of the track event
	// time pre-read which is done at the end of track initialization.
	if (FALSE == lpTrack->bIsAtEnd) 
	{
		DWORD	tkDelta;
		if (!GetTrackVDWord(lpTrack, &tkDelta))
			return FALSE;

		lpTrack->dwNextEventTime += tkDelta;
	}

	return TRUE;
}
//--------------------------------------------------
// Retrieve the next byte from the track buffer, refilling the buffer from
// disk if necessary.
//--------------------------------------------------
BOOL CMyMidiSampleDlg::GetTrackByte(LPMIDITRACK lpTrack, LPBYTE lpbyByte) 
{
	if (DWORD(lpTrack->pTrackCurrent - lpTrack->pTrackStart) == lpTrack->dwTrackLength)
		return FALSE;

	*lpbyByte = *lpTrack->pTrackCurrent++;

	return TRUE;
}
//--------------------------------------------------
// Attempts to parse a variable length DWORD from the given track.
//--------------------------------------------------
BOOL CMyMidiSampleDlg::GetTrackVDWord(LPMIDITRACK lpTrack, LPDWORD lpdw)
{
	if (NULL == lpTrack || NULL == lpdw)
		return FALSE;

	if (lpTrack->dwTrackLength <= 0)
		return FALSE;

	BYTE	byByte;
	DWORD	dw = 0;

	do {
		if (!GetTrackByte(lpTrack, &byByte))
			return FALSE;

		dw = ( dw << 7 ) | ( byByte & 0x7F );
	} while (byByte & 0x80);

	*lpdw = dw;

	return TRUE;
}
//--------------------------------------------------
void CMyMidiSampleDlg::OnMidiOutDone(MIDIHDR* pHdr)
{
	static int iCnt = 0;
	iCnt++;
}
//--------------------------------------------------
BOOL CMyMidiSampleDlg::PlayMidiFile()
{ 
	KillTimer(1);
	if (NULL == m_pEffect)
		//midiOutOpen(&m_hMidiOut,(UINT)-1, (DWORD)MidiProc, (DWORD)this, CALLBACK_FUNCTION);  
		midiOutOpen(&m_hMidiOut,(UINT)-1, 0, 0, CALLBACK_NULL);      

	int iMidiEventCnt = 0;
	const int ciMaxEventCnt = 512;
	MidiEvent midiEvents[ciMaxEventCnt];
	memset(&midiEvents, 0, ciMaxEventCnt*sizeof(MidiEvent));

	DWORD dwCurrentTime = 0;
	LPMIDIEVENT lpEvent = NULL;
	for (PtrEventArray::iterator it = m_paEvents.begin();
		it != m_paEvents.end();
		it++)
	{
		lpEvent = *it;
		if (lpEvent->dwTime <= dwCurrentTime)
		{			
			if (NULL == m_pEffect){
				SendMidiEventsToDevice(m_hMidiOut, lpEvent, 1);				
			}
			else{
				memcpy(&midiEvents[iMidiEventCnt], lpEvent, sizeof(MidiEvent));
				iMidiEventCnt++;
			}
		}
		else
		{
			if (m_pEffect && iMidiEventCnt)
			{
				SendMidiEventsToEffect(m_pEffect, midiEvents, iMidiEventCnt);
				iMidiEventCnt = 0;
			}

			// Waiting for delat-time
			//MySleep(lpEvent->dwTime - dwCurrentTime);
			//dwCurrentTime = lpEvent->dwTime;
			Sleep(20);
			dwCurrentTime += (20*1000);
			it--;
		}
	}
 
	if (m_hMidiOut)
		midiOutClose(m_hMidiOut); 
	SetTimer(1, 50, NULL);   

	return TRUE;
}
//--------------------------------------------------
void CMyMidiSampleDlg::ClearMidiData()
{
	LPMIDIEVENT lpEvent = NULL;
	for (PtrEventArray::iterator it = m_paEvents.begin();
		it != m_paEvents.end();
		it++)
	{
		lpEvent = *it;
		if (NULL != lpEvent)
		{
			delete lpEvent;
			lpEvent = NULL;
		}		
	}
	m_paEvents.clear();
}
//--------------------------------------------------
LONG CMyMidiSampleDlg::GetEventsByDeltaTime(DWORD dwDeltaTime, LPDWORD lpEvents)
{
	if (NULL == lpEvents)
		return 0;

	static PtrEventArray::iterator it = m_paEvents.begin();

	LONG lEventCnt = 0;
	while (it != m_paEvents.end())
	{		
		LPMIDIEVENT lpEvent = *it;
		if (lpEvent->dwTime <= dwDeltaTime)
		{
			lpEvents[lEventCnt++] = lpEvent->byShortData[0] | 
									((DWORD)lpEvent->byShortData[1] << 8) |
									((DWORD)lpEvent->byShortData[2] << 16) |
									MEVT_F_SHORT |
									MEVT_F_CALLBACK;
		}
		else
		{
			break;
		}
		it++;
	}

	if (it == m_paEvents.end())
	{
		lEventCnt = -1;
		it = m_paEvents.begin();
	}

	return lEventCnt;
}
//--------------------------------------------------
DWORD CMyMidiSampleDlg::SendMidiEventsToDevice(HMIDIOUT hMidiOut, LPMIDIEVENT lpEvent, DWORD dwEventCnt)
{
	if (NULL == hMidiOut || NULL == lpEvent || 0 == dwEventCnt)
		return 0;

	for (DWORD dwIndex = 0; dwIndex < dwEventCnt; dwIndex++)
	{
		DWORD dwShortMsg = 0;
		dwShortMsg = lpEvent->byShortData[0] | 
					((DWORD)lpEvent->byShortData[1] << 8) |
					((DWORD)lpEvent->byShortData[2] << 16) |
					MEVT_F_SHORT;

		if((lpEvent->byShortData[0] & 0xF0) == MIDI_CTRLCHANGE && 
			lpEvent->byShortData[1] == MIDICTRL_VOLUME) 
		{
			// If this is a volume change, generate a callback so we can grab
			// the new volume for our cache
			dwShortMsg |= MEVT_F_CALLBACK;
		}

		midiOutShortMsg(hMidiOut, dwShortMsg);  
	}

	return 0;
}
//--------------------------------------------------
DWORD CMyMidiSampleDlg::SendMidiEventsToEffect(CEffect* pEffect, LPMIDIEVENT lpEvent, DWORD dwEventCnt)
{
	if (NULL == pEffect)
		return 0;

	if (NULL == lpEvent || 0 == dwEventCnt)
	{
		VstEvents ves;
		ves.numEvents = 0;
		m_pEffect->EffProcessEvents(&ves);

		m_pEffect->EffProcessReplacing(m_fInputData, m_fOutputData, g_iBuffSize);

		SendAudioDataToWaveDevice(m_fOutputData, g_iBuffSize);
	}
	else
	{
		DWORD dwBufferHeaderSize = sizeof(VstEvents) + (dwEventCnt - 1)*sizeof(VstMidiEvent *);
		DWORD dwBufferTotalSize = dwBufferHeaderSize + dwEventCnt*sizeof(VstMidiEvent);
		LPBYTE lpEventData = new BYTE[dwBufferTotalSize];
		VstEvents* lpves = (VstEvents*)lpEventData;
		memset(lpEventData, 0, dwBufferTotalSize);
		lpves->numEvents = dwEventCnt;
		for (int i = 0; i < dwEventCnt; i++)
		{
			VstMidiEvent* pEv = ((VstMidiEvent *)(lpEventData + dwBufferHeaderSize)) + i;
			lpves->events[i] = (VstEvent *)pEv;
			pEv->type = kVstMidiType;
			pEv->byteSize = sizeof(VstMidiEvent);
			pEv->flags = kVstMidiEventIsRealtime;
			pEv->deltaFrames = 0;//lpEvent[i].dwTime;
			pEv->midiData[0] = lpEvent[i].byShortData[0];
			pEv->midiData[1] = lpEvent[i].byShortData[1];
			pEv->midiData[2] = lpEvent[i].byShortData[2];
			pEv->noteOffVelocity = 127;
		}

		m_pEffect->EffProcessEvents(lpves);

		m_pEffect->EffProcessReplacing(m_fInputData, m_fOutputData, g_iBuffSize);

		SendAudioDataToWaveDevice(m_fOutputData, g_iBuffSize);

		delete []lpEventData;
		lpEventData = NULL;
	}

	return 0;
}
//--------------------------------------------------
DWORD CMyMidiSampleDlg::SendAudioDataToWaveDevice(float** pWavData, DWORD dwLenght)
{
	if (m_pWaveOut && m_pWaveOut->IsOpen())
	{
		m_pWaveOut->SendData(pWavData, dwLenght);
	}

	return 0;
}
//--------------------------------------------------



//--------------------------------------------------
void CMyMidiSampleDlg::OnBnClickedBtnOpen()
{
	CFileDialog dlgOpen(TRUE, 
						NULL, 
						NULL, 
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
						_T("Midi Files (*.mid)|*.mid||"));
	if (dlgOpen.DoModal() == IDOK)
	{
		CListCtrl* pListCtr = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
		pListCtr->DeleteAllItems();

		CString strMidiFile = dlgOpen.GetPathName();
		SetDlgItemText(IDC_EDIT_MIDIFILE, strMidiFile);

		if (FALSE == LoadMidiFile(strMidiFile))
		{
			MessageBox(_T("Open this MIDI file failed!"));
		}
		else
		{
			UpdateMidiInfo();
		}
	}
}
//--------------------------------------------------
void CMyMidiSampleDlg::OnBnClickedBtnPlay()
{
	if (m_hPlayThread)
		CloseHandle(m_hPlayThread);

	DWORD dwThreadID = 0;
	m_hPlayThread = CreateThread(	NULL,                        // default security attributes 
									0,                           // use default stack size  
									PlayThreadFunc,              // thread function 
									this,						 // argument to thread function 
									0,                           // use default creation flags 
									&dwThreadID);                // returns the thread identifier

	if (NULL == m_hPlayThread)
	{
		CloseHandle(m_hPlayThread);
		m_hPlayThread = NULL;
	}
}
//--------------------------------------------------
void CMyMidiSampleDlg::OnBnClickedBtnEffect()
{
	CFileDialog dlgOpen(TRUE, 
						NULL, 
						NULL, 
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
						_T("Midi Effect Files (*.dll)|*.dll||"));
	if (dlgOpen.DoModal() == IDOK)
	{
		CString strMidiEffect = dlgOpen.GetPathName();
		SetDlgItemText(IDC_EDIT_MIDIEFFECT, strMidiEffect);

		if (FALSE == LoadMidiEffect(strMidiEffect))
		{
			MessageBox(_T("Load this MIDI Effect failed!"));
		}
		else
		{
			m_pWaveOut = new CSpecWaveOutDevice();
			if (m_pWaveOut->Open(0, NULL, WAVEOPENDEFAULT, WAVEOPENDEFAULT, CALLBACK_EVENT))
			{
				m_pWaveOut->AllocateBuffers(44100 / 4410);
				SetTimer(1, 50, NULL);
			}
			else
			{
				MessageBox(_T("Open wave device failed!"));
			}
		}
	}
}
//--------------------------------------------------
void CMyMidiSampleDlg::OnBnClickedBtnShowvsti()
{
	if (NULL == m_pEffect)
		return;

	ERect *prc = NULL;
	long lResult = m_pEffect->EffEditGetRect(&prc);

	if (NULL == m_pEffectWnd)
		m_pEffectWnd = new CWnd();
	BOOL bRes = m_pEffectWnd->CreateEx(	WS_EX_CLIENTEDGE, 
										_T("STATIC"), 
										_T(""), 
										WS_OVERLAPPEDWINDOW, 
										CRect(0, 0, prc->right-prc->left, prc->bottom-prc->top+80), 
										NULL, 
										0);
   
	m_pEffect->EffEditOpen(m_pEffectWnd->GetSafeHwnd());
	m_pEffectWnd->ShowWindow(SW_SHOW);
	m_pEffectWnd->UpdateWindow();
}
//--------------------------------------------------
void CMyMidiSampleDlg::UpdateMidiInfo()
{
	CString strTracksNum;
	CString strTicksNumPerNote;
	CString strTempo;
	CString strDuration;
	strTracksNum.Format(_T("%d"), m_dwTrackCount);
	strTicksNumPerNote.Format(_T("%d"), m_dwTimeDivision);
	strTempo.Format(_T("%d"), m_dwCurrentTempo);
	strDuration.Format(_T("%0.2f"), m_dwDuration/1000.0f/1000.0f);

	CListCtrl* pListCtr = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
	pListCtr->InsertItem(0, _T("Tracks Num:"));
	pListCtr->InsertItem(1, _T("Ticks Num:"));
	pListCtr->InsertItem(2, _T("Tempo:"));
	pListCtr->InsertItem(3, _T("Duration:"));
	pListCtr->SetItemText(0, 1, strTracksNum);
	pListCtr->SetItemText(1, 1, strTicksNumPerNote);
	pListCtr->SetItemText(2, 1, strTempo);
	pListCtr->SetItemText(3, 1, strDuration);
}
//--------------------------------------------------
void CMyMidiSampleDlg::MySleep(LONGLONG llMircsecond)
{
    static  bool  bFirst = true;
    static  LARGE_INTEGER  liFreq;
    static  double  fMircsecondsPerTick;

    if (bFirst)
    {
		// Get CPU Performance Tick count Per second
        QueryPerformanceFrequency(&liFreq);

		// The mircsecond Per CPU tick
		fMircsecondsPerTick = (1000000.0 / liFreq.QuadPart);
        bFirst = false;
    }

    LARGE_INTEGER  liStartCounter;
    LARGE_INTEGER  liCurrentCounter;
    QueryPerformanceCounter(&liStartCounter);

	double fDeltaTime = 0;
	do
	{
		QueryPerformanceCounter(&liCurrentCounter);
		fDeltaTime = (liCurrentCounter.QuadPart-liStartCounter.QuadPart) * fMircsecondsPerTick;
	}while (fDeltaTime < llMircsecond);	
}
//--------------------------------------------------
DWORD CMyMidiSampleDlg::OnPlay()
{
	DWORD dwEventSize = m_paEvents.size();
	if (dwEventSize > 0)
	{
		PlayMidiFile();
	}

	return 0;
}
//--------------------------------------------------
long CMyMidiSampleDlg::MidiMasterCallback(AEffect *effect, long opcode, long index, long value, void *ptr, float opt)
{
	switch (opcode)
	{		
	case audioMasterProcessEvents:
		{
			MessageBox(_T("audioMasterProcessEvents"));
		}
		break;
	case audioMasterAutomate:
		return 0;//OnSetParameterAutomated(nEffect, index, opt);
	case audioMasterVersion:
		#if defined(VST_2_4_EXTENSIONS)
			return 2400L;
		#elif defined(VST_2_3_EXTENSIONS)
			return 2300L;
		#elif defined(VST_2_2_EXTENSIONS)
			return 2200L;
		#elif defined(VST_2_1_EXTENSIONS)
			return 2100L;
		#else 
			return 2L;
		#endif
			break;
	case audioMasterWantMidi:
		if (m_pEffect)
		{
			m_pEffect->bWantMidi = true;
			return 1;
		}
		break;
	case audioMasterNeedIdle:
		if (m_pEffect)
		{
			m_pEffect->bNeedIdle = true;
			return 1;
		}
		break;
	case audioMasterCanDo :
		return OnCanDo((const char *)ptr);
	case audioMasterIdle :
		m_pEffect->EffEditIdle();
		break;
	}

	return 0;
}
//--------------------------------------------------
BOOL CMyMidiSampleDlg::LoadMidiEffect(LPCTSTR lpszEffectFile)
{	
	if (m_pEffect)
	{
		m_pEffect->EffStopProcess();
		m_pEffect->EffClose();
		delete m_pEffect;
	}

	m_pEffect = new CEffect(AudioMasterCallback);

	if (!m_pEffect->Load(lpszEffectFile))
	{
		delete m_pEffect;
		m_pEffect = NULL;
		return FALSE;
	}

	m_pEffect->EffOpen();                     /* open the effect                   */
	m_pEffect->EffSetSampleRate(44100); /* adjust its sample rate            */

	//long lRes = m_pEffect->EffCanDo("receiveVstEvents");
	//lRes = m_pEffect->EffGetPlugCategory();
	// this is a safety measure against some plugins that only set their buffers
	// ONCE - this should ensure that they allocate a buffer that's large enough
	//m_pEffect->EffSetBlockSize(11025);
	//m_pEffect->EffResume();                   /* then force resume.                */
	//m_pEffect->EffSuspend();                  /* suspend again...                  */
	m_pEffect->EffSetBlockSize(g_iBuffSize);   /* and block size                    */
	m_pEffect->EffResume();                   /* then force resume.                */

	m_pEffect->EffSetProgram(0);
	//MidiProgramName mn;
	//lRes = m_pEffect->EffGetMidiProgramName(1, &mn);
	m_pEffect->EffStartProcess();
	
	int iInputNum = m_pEffect->GetInputNum();
	int iOutputNum = m_pEffect->GetOutputNum();

	if (iInputNum > 0)
	{
		m_fInputData = new float *[iInputNum];
		for(int iNum = 0; iNum < iInputNum; iNum++)
		{
			m_fInputData[iNum] = new float[g_iBuffSize];
			memset(m_fInputData[iNum], 0, sizeof(float)*g_iBuffSize);
		}
	}
	if (iOutputNum > 0)
	{
		m_fOutputData = new float *[iOutputNum];
		for(int iNum = 0; iNum < iOutputNum; iNum++)
		{
			m_fOutputData[iNum] = new float[g_iBuffSize];
			memset(m_fOutputData[iNum], 0, sizeof(float)*g_iBuffSize);
		}
	}
    
	return TRUE;
}
//--------------------------------------------------
bool CMyMidiSampleDlg::OnCanDo(const char *ptr)
{
	if ((!strcmp(ptr, "sendVstEvents")) ||
		(!strcmp(ptr, "sendVstMidiEvent")) ||
		(!strcmp(ptr, "receiveVstEvents")) ||
		(!strcmp(ptr, "receiveVstMidiEvent")) ||
		(!strcmp(ptr, "sizeWindow")) ||
		(!strcmp(ptr, "sendVstMidiEventFlagIsRealtime")) ||
		0)
		return true;
	return false;
}
//--------------------------------------------------
void CMyMidiSampleDlg::OnTimer(UINT nIDEvent)
{
	SendMidiEventsToEffect(m_pEffect, NULL, 0);

	CDialog::OnTimer(nIDEvent);
}
