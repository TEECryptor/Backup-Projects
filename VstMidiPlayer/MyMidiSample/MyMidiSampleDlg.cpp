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

BOOL WaitFor(HANDLE hEvent)
{
	DWORD nRet = WAIT_TIMEOUT;

	Sleep(100);

	while(WAIT_OBJECT_0 != nRet)
	{
		nRet = WaitForSingleObject(hEvent, 100);

		Sleep(100);

		MSG msg;
		while (PeekMessage (&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage  (&msg);
		}
	}
	return TRUE;
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
	m_pCurEffect = NULL;
	m_fInputData = NULL;
	m_fOutputData = NULL;
	m_pWaveOut = NULL;
	m_iCurMidiEventCnt = 0;
	m_dwProcThreadID = 0;
	m_hProcessThread = NULL;
	m_hProcessEvent = NULL;	

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
//	ON_WM_TIMER()
ON_CBN_SELCHANGE(IDC_COMBO_VSTI, OnCbnSelchangeComboVsti)
ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
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


	CWnd* pControl = NULL;
	GetWindowRect(&m_rcMainDlg);
	pControl = GetDlgItem(IDC_STATIC_GROUPFILE);
	pControl->GetWindowRect(&m_rcFileGroup);
	pControl = GetDlgItem(IDC_STATIC_GROUPVSTI);
	pControl->GetWindowRect(&m_rcVSTiGourp);
	pControl = GetDlgItem(IDC_EDIT_MIDIFILE);
	pControl->GetWindowRect(&m_rcMidiFileEdit);
	pControl = GetDlgItem(IDC_BTN_OPEN);
	pControl->GetWindowRect(&m_rcOpenMidiBtn);
	pControl = GetDlgItem(IDC_BTN_PLAY);
	pControl->GetWindowRect(&m_rcPlayMidiBtn);
	pControl = GetDlgItem(IDC_BTN_STOP);
	pControl->GetWindowRect(&m_rcStopMidiBtn);	
	pControl = GetDlgItem(IDC_LIST_INFO);
	pControl->GetWindowRect(&m_rcMidiInfoList);
	pControl = GetDlgItem(IDC_COMBO_VSTI);
	pControl->GetWindowRect(&m_rcVSTiComboBox);
	pControl = GetDlgItem(IDC_BTN_EFFECT);
	pControl->GetWindowRect(&m_rcAddVSTiBtn);
	pControl = GetDlgItem(IDC_LIST_VSTIWND);
	pControl->GetWindowRect(&m_rcVSTiWnd);	
	pControl = GetDlgItem(IDOK);
	pControl->GetWindowRect(&m_rcOKBtn);
	//
	ScreenToClient(&m_rcFileGroup);
	ScreenToClient(&m_rcVSTiGourp);
	ScreenToClient(&m_rcMidiFileEdit);
	ScreenToClient(&m_rcOpenMidiBtn);
	ScreenToClient(&m_rcMidiInfoList);
	ScreenToClient(&m_rcPlayMidiBtn);
	ScreenToClient(&m_rcStopMidiBtn);	
	ScreenToClient(&m_rcVSTiComboBox);
	ScreenToClient(&m_rcAddVSTiBtn);
	ScreenToClient(&m_rcVSTiWnd);
	ScreenToClient(&m_rcOKBtn);

	CListCtrl* pListCtr = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
	pListCtr->SetExtendedStyle(pListCtr->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	pListCtr->InsertColumn(0, _T("Name"), LVCFMT_CENTER, 100);
	pListCtr->InsertColumn(1, _T("Value"), LVCFMT_CENTER, 100);

	CComboBox* pVSTiComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_VSTI);
	pVSTiComboBox->AddString(_T("Don't use any VSTi"));
	pVSTiComboBox->SetCurSel(0);

	//Create process thread and event	
	m_hProcessEvent = CreateEvent(	NULL,				// no security attributes
									FALSE,				// manual-reset event
									FALSE,				// initial state is signaled
									_T("ProgressEvent") // object name
									);
	m_hProcessThread = CreateThread(0, 
									 0, 
									 ProcessThread, 
									 (LPVOID)this, 
									 0, 
									 &m_dwProcThreadID);
	//SetThreadPriority(m_hProcessThread, THREAD_PRIORITY_HIGHEST);
	WaitFor(m_hProcessEvent);
	PostThreadMessage(m_dwProcThreadID, PROC_MSG_START, 0, 0);
	
	//Open the wave device
	m_pWaveOut = new CSpecWaveOutDevice();
	if (m_pWaveOut->Open(0, NULL, WAVEOPENDEFAULT, WAVEOPENDEFAULT, CALLBACK_EVENT))
	{
		m_pWaveOut->AllocateBuffers(44100 / g_iBuffSize);
		m_pWaveOut->BoostPriority();
	}

	UpdateUIControl(ePlayState_Ready);
	InitializeCriticalSection(&cs);
	
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

	PostThreadMessage(m_dwProcThreadID, PROC_MSG_END, 0, 0);
	WaitFor(m_hProcessThread);

	if (m_pWaveOut)
	{
		m_pWaveOut->Close();
		delete m_pWaveOut;
		m_pWaveOut = NULL;
	}
	
	DeleteBufferForVSTi();

	if (NULL != m_pCurEffect)
	{
		m_pCurEffect->EffEditClose();
	}

	CEffect* pEffect = NULL;
	for (EffectPtrArray::iterator it = m_paEffect.begin();
		it != m_paEffect.end();
		it++)
	{
		pEffect = *it;
		if (NULL != pEffect)
		{
			delete pEffect;
			pEffect = NULL;
			break;
		}
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
LONG CMyMidiSampleDlg::GetEventsByDeltaTime(DWORD dwDeltaTime)
{
	static PtrEventArray::iterator it = m_paEvents.begin();

	if (-1 == dwDeltaTime)
	{
		it = m_paEvents.begin();
		return 0;
	}

	m_iCurMidiEventCnt = 0;
	while (it != m_paEvents.end())
	{		
		LPMIDIEVENT lpEvent = *it;
		if (lpEvent->dwTime <= dwDeltaTime)
		{
			memcpy(&m_midiEvents[m_iCurMidiEventCnt], lpEvent, sizeof(MidiEvent)); 
			m_iCurMidiEventCnt++;
		}
		else
		{
			break;
		}
		it++;
	}

	BOOL bIsEnd = FALSE;
	if (it == m_paEvents.end())
	{
		bIsEnd = TRUE;
		it = m_paEvents.begin();
	}

	return bIsEnd ? -1 : m_iCurMidiEventCnt;
}
//--------------------------------------------------
DWORD CMyMidiSampleDlg::SendMidiEventsToDevice(DWORD dwEventCnt)
{
	if (NULL == m_hMidiOut || 0 == dwEventCnt)
		return 0;

	for (DWORD dwIndex = 0; dwIndex < dwEventCnt; dwIndex++)
	{
		DWORD dwShortMsg = 0;
		dwShortMsg = m_midiEvents[dwIndex].byShortData[0] | 
					((DWORD)m_midiEvents[dwIndex].byShortData[1] << 8) |
					((DWORD)m_midiEvents[dwIndex].byShortData[2] << 16) |
					MEVT_F_SHORT;

		if((m_midiEvents[dwIndex].byShortData[0] & 0xF0) == MIDI_CTRLCHANGE && 
			m_midiEvents[dwIndex].byShortData[1] == MIDICTRL_VOLUME) 
		{
			// If this is a volume change, generate a callback so we can grab
			// the new volume for our cache
			dwShortMsg |= MEVT_F_CALLBACK;
		}

		midiOutShortMsg(m_hMidiOut, dwShortMsg);  
	}

	return 0;
}
//--------------------------------------------------
DWORD CMyMidiSampleDlg::SendMidiEventsToEffect()
{
	if (NULL == m_pCurEffect)
		return 0;

	EnterCriticalSection(&cs);  
	
	if (0 == m_iCurMidiEventCnt)
	{
		VstEvents ves;
		memset(&ves, 0, sizeof(VstEvents));
		m_pCurEffect->EffProcessEvents(&ves);
	}
	else
	{
		static DWORD dwLastDelta = 0;
		DWORD dwBufferHeaderSize = sizeof(VstEvents) + (m_iCurMidiEventCnt - 1)*sizeof(VstMidiEvent *);
		DWORD dwBufferTotalSize = dwBufferHeaderSize + m_iCurMidiEventCnt*sizeof(VstMidiEvent);
		LPBYTE lpEventData = new BYTE[dwBufferTotalSize];
		VstEvents* lpves = (VstEvents*)lpEventData;
		memset(lpEventData, 0, dwBufferTotalSize);
		lpves->numEvents = m_iCurMidiEventCnt;
		for (int i = 0; i < m_iCurMidiEventCnt; i++)
		{
			VstMidiEvent* pEv = ((VstMidiEvent *)(lpEventData + dwBufferHeaderSize)) + i;
			lpves->events[i] = (VstEvent *)pEv;
			pEv->type = kVstMidiType;
			pEv->byteSize = sizeof(VstMidiEvent);
			pEv->flags = kVstMidiEventIsRealtime;
			pEv->deltaFrames = DWORD((m_midiEvents[i].dwTime-dwLastDelta) *(g_iBuffSize/1000000.0f));
			pEv->midiData[0] = m_midiEvents[i].byShortData[0];
			pEv->midiData[1] = m_midiEvents[i].byShortData[1];
			pEv->midiData[2] = m_midiEvents[i].byShortData[2];
			dwLastDelta = m_midiEvents[i].dwTime;

			TCHAR tcDeltaTime[32] = {_T("\0")};
			_stprintf(tcDeltaTime, _T("deltaFrames: %d\n"), pEv->deltaFrames);
			TRACE(tcDeltaTime);
		}
		m_iCurMidiEventCnt = 0;

		m_pCurEffect->EffProcessEvents(lpves);

		delete []lpEventData;
		lpEventData = NULL;
	}

	m_pCurEffect->EffProcessReplacing(m_fInputData, m_fOutputData, g_iBuffSize);

	SendAudioDataToWaveDevice(m_fOutputData, g_iBuffSize);
	
	LeaveCriticalSection(&cs);

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
	PostThreadMessage(m_dwProcThreadID, PROC_MSG_PLAY, 0, 0);
	UpdateUIControl(ePlayState_Playing);
}
//--------------------------------------------------
void CMyMidiSampleDlg::OnBnClickedBtnStop()
{
	PostThreadMessage(m_dwProcThreadID, PROC_MSG_STOP, 0, 0);
	UpdateUIControl(ePlayState_Stopped);
}
//--------------------------------------------------
void CMyMidiSampleDlg::OnBnClickedBtnEffect()
{
	CComboBox* pVSTiComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_VSTI);
	CFileDialog dlgOpen(TRUE, 
						NULL, 
						NULL, 
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
						_T("Midi Effect Files (*.dll)|*.dll||"));
	if (dlgOpen.DoModal() == IDOK)
	{
		CString strMidiEffect = dlgOpen.GetPathName();
		if (FALSE == LoadMidiEffect(strMidiEffect))
		{
			MessageBox(_T("Load this MIDI Effect failed!"));
		}
		else
		{
			pVSTiComboBox->AddString(strMidiEffect);
			pVSTiComboBox->SetCurSel(m_paEffect.size() > 0 ? m_paEffect.size() : 0);
			OnCbnSelchangeComboVsti();
		}
	}
}
//--------------------------------------------------
void CMyMidiSampleDlg::OnCbnSelchangeComboVsti()
{
	DeleteBufferForVSTi();

	if (NULL != m_pCurEffect)
		m_pCurEffect->EffEditClose();
	
	CComboBox* pVSTiComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_VSTI);
	DWORD dwCurSel = pVSTiComboBox->GetCurSel();
	if (0 == dwCurSel)
	{
		m_pCurEffect = NULL;
	}
	else
	{
		m_pCurEffect = GetEffectByIndex(dwCurSel-1);	
		CreateBufferForVSTi();
	}

	ShowVSTiWindow();
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
void CMyMidiSampleDlg::UpdateCurTimeInfo(DWORD dwCurTime)
{
	dwCurTime = (dwCurTime > m_dwDuration/1000) ? m_dwDuration/1000 : dwCurTime;

	TCHAR tcTimeInfo[32] = {_T("\0")};
	_stprintf(tcTimeInfo, _T("%0.2f / %0.2f"), dwCurTime/1000.0f, m_dwDuration/1000000.0f);
	SetDlgItemText(IDC_STATIC_TIME, tcTimeInfo);
}
//--------------------------------------------------
void CMyMidiSampleDlg::CreateBufferForVSTi()
{
	if (NULL == m_pCurEffect)
		return;

	int iInputNum = m_pCurEffect->GetInputNum();
	int iOutputNum = m_pCurEffect->GetOutputNum();

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
}
//--------------------------------------------------
void CMyMidiSampleDlg::DeleteBufferForVSTi()
{
	if (NULL == m_pCurEffect)
		return;

	m_pCurEffect->EffStopProcess();
	int iInputNum = m_pCurEffect->GetInputNum();
	int iOutputNum = m_pCurEffect->GetOutputNum();

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
}
//--------------------------------------------------
void CMyMidiSampleDlg::ShowVSTiWindow()
{
	if (NULL == m_pCurEffect)
	{
		MoveWindow(&m_rcMainDlg);
		CenterWindow();
		//
		CWnd* pControl = NULL;
		pControl = GetDlgItem(IDC_STATIC_GROUPFILE);
		pControl->MoveWindow(&m_rcFileGroup);
		pControl = GetDlgItem(IDC_STATIC_GROUPVSTI);
		pControl->MoveWindow(&m_rcVSTiGourp);
		pControl = GetDlgItem(IDC_EDIT_MIDIFILE);
		pControl->MoveWindow(&m_rcMidiFileEdit);
		pControl = GetDlgItem(IDC_BTN_OPEN);
		pControl->MoveWindow(&m_rcOpenMidiBtn);
		pControl = GetDlgItem(IDC_BTN_PLAY);
		pControl->MoveWindow(&m_rcPlayMidiBtn);
		pControl = GetDlgItem(IDC_BTN_STOP);
		pControl->MoveWindow(&m_rcStopMidiBtn);
		pControl = GetDlgItem(IDC_LIST_INFO);
		pControl->MoveWindow(&m_rcMidiInfoList);
		pControl = GetDlgItem(IDC_COMBO_VSTI);
		pControl->MoveWindow(&m_rcVSTiComboBox);
		pControl = GetDlgItem(IDC_BTN_EFFECT);
		pControl->MoveWindow(&m_rcAddVSTiBtn);
		pControl = GetDlgItem(IDOK);
		pControl->MoveWindow(&m_rcOKBtn);
		//
		pControl = GetDlgItem(IDC_LIST_VSTIWND);
		pControl->MoveWindow(&m_rcVSTiWnd);
		pControl->ShowWindow(SW_HIDE);

		return;
	}

	CRect rcMainDlg;
	CRect rcNewMainDlg;
	GetWindowRect(&rcMainDlg);

	CRect rcVSTiWnd;
	CWnd* pVSTiParentWnd = GetDlgItem(IDC_LIST_VSTIWND);
	pVSTiParentWnd->ShowWindow(SW_SHOW);
	pVSTiParentWnd->GetWindowRect(&rcVSTiWnd);
	ScreenToClient(&rcVSTiWnd);
	
	CRect rcFileGroup;
	CRect rcVSTiGourp;
	CRect rcMidiFileEdit;
	CRect rcOpenMidiBtn;
	CRect rcPlayMidiBtn;
	CRect rcStopMidiBtn;
	CRect rcMidiInfoList;
	CRect rcVSTiComboBox;
	CRect rcAddVSTiBtn;
	CRect rcOKBtn;
	CWnd* pCrlFileGroup = GetDlgItem(IDC_STATIC_GROUPFILE);
	CWnd* pCrlVSTiGroup = GetDlgItem(IDC_STATIC_GROUPVSTI);
	CWnd* pCrlFileEdit = GetDlgItem(IDC_EDIT_MIDIFILE);
	CWnd* pCrlOpenFileBtn = GetDlgItem(IDC_BTN_OPEN);
	CWnd* pCrlPlayFileBtn = GetDlgItem(IDC_BTN_PLAY);
	CWnd* pCrlStopFileBtn = GetDlgItem(IDC_BTN_STOP);
	CWnd* pCrlMidiInfoList = GetDlgItem(IDC_LIST_INFO);
	CWnd* pCrlVSTiComboBox = GetDlgItem(IDC_COMBO_VSTI);
	CWnd* pCrlAddVSTiBtn = GetDlgItem(IDC_BTN_EFFECT);
	CWnd* pCrlOKBtn = GetDlgItem(IDOK);
	pCrlFileGroup->GetWindowRect(&rcFileGroup);
	pCrlVSTiGroup->GetWindowRect(&rcVSTiGourp);
	pCrlFileEdit->GetWindowRect(&rcMidiFileEdit);
	pCrlOpenFileBtn->GetWindowRect(&rcOpenMidiBtn);
	pCrlPlayFileBtn->GetWindowRect(&rcPlayMidiBtn);
	pCrlStopFileBtn->GetWindowRect(&rcStopMidiBtn);
	pCrlMidiInfoList->GetWindowRect(&rcMidiInfoList);
	pCrlVSTiComboBox->GetWindowRect(&rcVSTiComboBox);
	pCrlAddVSTiBtn->GetWindowRect(&rcAddVSTiBtn);
	pCrlOKBtn->GetWindowRect(&rcOKBtn);
	ScreenToClient(&rcFileGroup);
	ScreenToClient(&rcVSTiGourp);
	ScreenToClient(&rcMidiFileEdit);
	ScreenToClient(&rcOpenMidiBtn);
	ScreenToClient(&rcPlayMidiBtn);
	ScreenToClient(&rcStopMidiBtn);
	ScreenToClient(&rcMidiInfoList);
	ScreenToClient(&rcVSTiComboBox);
	ScreenToClient(&rcAddVSTiBtn);
	ScreenToClient(&rcOKBtn);

	ERect *prc = NULL;
	long lResult = m_pCurEffect->EffEditGetRect(&prc);

	int iDeltaX = (prc->right-prc->left) - rcVSTiWnd.Width();
	int iDeltaY = (prc->bottom-prc->top) - rcVSTiWnd.Height();

	// Adjust maid dialog size
	rcMainDlg.right += iDeltaX;
	rcMainDlg.bottom += iDeltaY;
	MoveWindow(&rcMainDlg);
	CenterWindow();

	// Adjust VSTi parent window size
	rcVSTiWnd.right += iDeltaX;
	rcVSTiWnd.bottom += iDeltaY;
	pVSTiParentWnd->MoveWindow(&rcVSTiWnd);

	// Adjust other related controls
	rcFileGroup.right += iDeltaX;
	pCrlFileGroup->MoveWindow(&rcFileGroup);
	//
	rcVSTiGourp.right += iDeltaX;
	rcVSTiGourp.bottom += iDeltaY;
	pCrlVSTiGroup->MoveWindow(&rcVSTiGourp);
	//
	rcMidiFileEdit.right += iDeltaX;
	pCrlFileEdit->MoveWindow(&rcMidiFileEdit);
	//
	rcOpenMidiBtn.OffsetRect(iDeltaX, 0); 
	pCrlOpenFileBtn->MoveWindow(&rcOpenMidiBtn);
	//
	rcPlayMidiBtn.OffsetRect(iDeltaX, 0); 
	pCrlPlayFileBtn->MoveWindow(&rcPlayMidiBtn);
	//
	rcStopMidiBtn.OffsetRect(iDeltaX, 0); 
	pCrlStopFileBtn->MoveWindow(&rcStopMidiBtn);
	//
	rcMidiInfoList.right += iDeltaX;
	pCrlMidiInfoList->MoveWindow(&rcMidiInfoList);
	//
	rcVSTiComboBox.right += iDeltaX;
	pCrlVSTiComboBox->MoveWindow(&rcVSTiComboBox);
	//
	rcAddVSTiBtn.OffsetRect(iDeltaX, 0); 
	pCrlAddVSTiBtn->MoveWindow(&rcAddVSTiBtn);
	//
	CRect rcNewOKBtn;
	rcNewOKBtn.left = (rcMainDlg.Width() - rcOKBtn.Width()) / 2;
	rcNewOKBtn.right = rcNewOKBtn.left + rcOKBtn.Width();
	rcNewOKBtn.bottom = rcMainDlg.Height() - 45;
	rcNewOKBtn.top = rcNewOKBtn.bottom - rcOKBtn.Height();
	pCrlOKBtn->MoveWindow(&rcNewOKBtn);

	m_pCurEffect->EffEditOpen(pVSTiParentWnd->GetSafeHwnd());
	pVSTiParentWnd->ShowWindow(SW_SHOW);
	pVSTiParentWnd->UpdateWindow();
}
//--------------------------------------------------
void CMyMidiSampleDlg::UpdateUIControl(DWORD dwStatus)
{
	CWnd* pOpenBtn = GetDlgItem(IDC_BTN_OPEN);
	CWnd* pPlayBtn = GetDlgItem(IDC_BTN_PLAY);
	CWnd* pStopBtn = GetDlgItem(IDC_BTN_STOP);
	CWnd* pVSTiComboBox = GetDlgItem(IDC_COMBO_VSTI);
	CWnd* pAddVSTiBtn = GetDlgItem(IDC_BTN_EFFECT);
	if (ePlayState_Playing == dwStatus)
	{
		pOpenBtn->EnableWindow(FALSE);
		pPlayBtn->EnableWindow(FALSE);
		pStopBtn->EnableWindow(TRUE);
		pVSTiComboBox->EnableWindow(FALSE);
		pAddVSTiBtn->EnableWindow(FALSE);
	}
	else
	{
		pOpenBtn->EnableWindow(TRUE);
		pPlayBtn->EnableWindow(TRUE);
		pStopBtn->EnableWindow(FALSE);
		pVSTiComboBox->EnableWindow(TRUE);
		pAddVSTiBtn->EnableWindow(TRUE);
	}
}
//--------------------------------------------------
CEffect* CMyMidiSampleDlg::GetEffectByIndex(DWORD dwIndex)
{
	CEffect* pFoundEffect = NULL;

	DWORD dwCurIndex = 0;
	for (EffectPtrArray::iterator it = m_paEffect.begin();
		it != m_paEffect.end();
		it++, dwCurIndex++)
	{
		if (dwCurIndex == dwIndex)
		{
			pFoundEffect = *it;
			break;
		}
	}

	return pFoundEffect;
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
		if (m_pCurEffect)
		{
			m_pCurEffect->bWantMidi = true;
			return 1;
		}
		break;
	case audioMasterNeedIdle:
		if (m_pCurEffect)
		{
			m_pCurEffect->bNeedIdle = true;
			return 1;
		}
		break;
	case audioMasterCanDo :
		return OnCanDo((const char *)ptr);
	case audioMasterIdle :
		m_pCurEffect->EffEditIdle();
		break;
	}

	return 0;
}
//--------------------------------------------------
DWORD WINAPI CMyMidiSampleDlg::ProcessThread(LPVOID lpParam)
{
	CMyMidiSampleDlg *pMidiDlg = (CMyMidiSampleDlg *)lpParam;

	MSG msg;
	BOOL bCallCoUninitialzeOnExit = FALSE;
	HRESULT hrCoInit = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(SUCCEEDED(hrCoInit))
	{
		bCallCoUninitialzeOnExit = TRUE;
	}

	SetEvent(pMidiDlg->m_hProcessEvent);

	const int c_iTimeInterval = 85;
	const int c_iTimeInterval2 = 70;

	BOOL bIsStarted = FALSE;
	BOOL bExitThread = FALSE;
	DWORD dwCurrentTime = 0;
	ePlayState eCurPlayState = ePlayState_Ready;
	do
	{
		while(PeekMessage(&msg, 0, 0, 0,PM_REMOVE|PM_QS_POSTMESSAGE))
		{
			switch(msg.message)
			{
			case PROC_MSG_START:
				bIsStarted = TRUE;   
				break;	
			case PROC_MSG_PLAY:
				eCurPlayState = ePlayState_Playing;
				midiOutOpen(&pMidiDlg->m_hMidiOut,(UINT)-1, 0, 0, CALLBACK_NULL);  
				dwCurrentTime = ::GetTickCount(); 
				break;	
			case PROC_MSG_PAUSE:
				eCurPlayState = ePlayState_Paused;
				break;	
			case PROC_MSG_STOP:
				eCurPlayState = ePlayState_Stopped;
				pMidiDlg->GetEventsByDeltaTime(-1);
				if (pMidiDlg->m_hMidiOut)
					midiOutClose(pMidiDlg->m_hMidiOut); 
				break;	
			case PROC_MSG_END:
				eCurPlayState = ePlayState_Stopped;
				bExitThread = TRUE;
				break;	
			default:
				{
					::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
					break;
				}
			}
		}

		if (ePlayState_Playing == eCurPlayState)
		{
			DWORD dwDeltaTime = ::GetTickCount() - dwCurrentTime;
			int iEventCntThisTime = pMidiDlg->GetEventsByDeltaTime(dwDeltaTime*1000/*dwCurrentTime*/);

			if (NULL == pMidiDlg->m_pCurEffect &&
				iEventCntThisTime > 0)
			{
				pMidiDlg->SendMidiEventsToDevice(iEventCntThisTime);
			}
			//dwCurrentTime += (c_iTimeInterval*1000);

			// Is at the end of list
			if (-1 == iEventCntThisTime)
			{
				pMidiDlg->UpdateUIControl(ePlayState_Stopped);
				eCurPlayState = ePlayState_Stopped;
				dwCurrentTime = 0;
			}

			// Update current time information
			pMidiDlg->UpdateCurTimeInfo(dwDeltaTime);
		}
				
		if (bIsStarted && pMidiDlg->m_pCurEffect)
		{
			pMidiDlg->SendMidiEventsToEffect();
		}

		Sleep(c_iTimeInterval);

	}while (!bExitThread);

	if(bCallCoUninitialzeOnExit)
	{
		::CoUninitialize();
	}

	return 0;
}
//--------------------------------------------------
BOOL CMyMidiSampleDlg::LoadMidiEffect(LPCTSTR lpszEffectFile)
{	
	CEffect* pEffect = new CEffect(AudioMasterCallback);
	if (!pEffect->Load(lpszEffectFile))
	{
		delete pEffect;
		pEffect = NULL;
		return FALSE;
	}

	pEffect->EffOpen();                     /* open the effect                   */
	pEffect->EffSetSampleRate(44100); /* adjust its sample rate            */
	pEffect->EffSetBlockSize(g_iBuffSize);   /* and block size                    */
	pEffect->EffResume();                   /* then force resume.                */
	pEffect->EffSetProgram(0);
	pEffect->EffStartProcess();

	m_paEffect.push_back(pEffect);

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