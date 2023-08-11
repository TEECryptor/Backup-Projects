// MusicLegendAppDlg.cpp : implementation file
//
//--------------------------------------------------
#include "stdafx.h"
#include "MusicLegendApp.h"
#include "MusicLegendAppDlg.h"
#include "VSTiInfoDlg.h"
//--------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//--------------------------------------------------
// CAboutDlg dialog used for App About
//--------------------------------------------------
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
//--------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
//--------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//--------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
//--------------------------------------------------



//--------------------------------------------------
// CMusicLegendAppDlg dialog
//--------------------------------------------------
CMusicLegendAppDlg::CMusicLegendAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMusicLegendAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pVSTiPlayer = NULL;
}
//--------------------------------------------------
void CMusicLegendAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//--------------------------------------------------
BEGIN_MESSAGE_MAP(CMusicLegendAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_VSTI_1, OnBnClickedBtnVsti1)
	ON_BN_CLICKED(IDC_BTN_MIDI_1, OnBnClickedBtnMidi1)
	ON_BN_CLICKED(IDC_BTN_PLAYSTOP_1, OnBnClickedBtnPlaystop1)
	ON_BN_CLICKED(IDC_BTN_VSTIINFO_1, OnBnClickedBtnVstiinfo1)
	ON_BN_CLICKED(IDC_BTN_VSTI_2, OnBnClickedBtnVsti2)
	ON_BN_CLICKED(IDC_BTN_MIDI_2, OnBnClickedBtnMidi2)
	ON_BN_CLICKED(IDC_BTN_PLAYSTOP_2, OnBnClickedBtnPlaystop2)
	ON_BN_CLICKED(IDC_BTN_VSTIINFO_2, OnBnClickedBtnVstiinfo2)
	ON_BN_CLICKED(IDC_BTN_VSTI_3, OnBnClickedBtnVsti3)
	ON_BN_CLICKED(IDC_BTN_MIDI_3, OnBnClickedBtnMidi3)
	ON_BN_CLICKED(IDC_BTN_PLAYSTOP_3, OnBnClickedBtnPlaystop3)
	ON_BN_CLICKED(IDC_BTN_VSTIINFO_3, OnBnClickedBtnVstiinfo3)
	ON_BN_CLICKED(IDC_BTN_VSTI_4, OnBnClickedBtnVsti4)
	ON_BN_CLICKED(IDC_BTN_MIDI_4, OnBnClickedBtnMidi4)
	ON_BN_CLICKED(IDC_BTN_PLAYSTOP_4, OnBnClickedBtnPlaystop4)
	ON_BN_CLICKED(IDC_BTN_VSTIINFO_4, OnBnClickedBtnVstiinfo4)
	ON_BN_CLICKED(IDC_BTN_VSTI_5, OnBnClickedBtnVsti5)
	ON_BN_CLICKED(IDC_BTN_MIDI_5, OnBnClickedBtnMidi5)
	ON_BN_CLICKED(IDC_BTN_PLAYSTOP_5, OnBnClickedBtnPlaystop5)
	ON_BN_CLICKED(IDC_BTN_VSTIINFO_5, OnBnClickedBtnVstiinfo5)
	ON_BN_CLICKED(IDC_BTN_VSTI_6, OnBnClickedBtnVsti6)
	ON_BN_CLICKED(IDC_BTN_MIDI_6, OnBnClickedBtnMidi6)
	ON_BN_CLICKED(IDC_BTN_PLAYSTOP_6, OnBnClickedBtnPlaystop6)
	ON_BN_CLICKED(IDC_BTN_VSTIINFO_6, OnBnClickedBtnVstiinfo6)
	ON_BN_CLICKED(IDC_BTN_VSTI_7, OnBnClickedBtnVsti7)
	ON_BN_CLICKED(IDC_BTN_MIDI_7, OnBnClickedBtnMidi7)
	ON_BN_CLICKED(IDC_BTN_PLAYSTOP_7, OnBnClickedBtnPlaystop7)
	ON_BN_CLICKED(IDC_BTN_VSTIINFO_7, OnBnClickedBtnVstiinfo7)
	ON_BN_CLICKED(IDC_BTN_VSTI_8, OnBnClickedBtnVsti8)
	ON_BN_CLICKED(IDC_BTN_MIDI_8, OnBnClickedBtnMidi8)
	ON_BN_CLICKED(IDC_BTN_PLAYSTOP_8, OnBnClickedBtnPlaystop8)
	ON_BN_CLICKED(IDC_BTN_VSTIINFO_8, OnBnClickedBtnVstiinfo8)
	ON_BN_CLICKED(IDC_BTN_PLAYSTOP_ALL, OnBnClickedBtnPlaystopAll)
END_MESSAGE_MAP()
//--------------------------------------------------



//--------------------------------------------------
// CMusicLegendAppDlg message handlers
//--------------------------------------------------
BOOL CMusicLegendAppDlg::OnInitDialog()
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

	for (int iIndex = 0; iIndex < MAX_TRACK_NUM; iIndex++)
	{
		m_pVSTiEffects[iIndex] = NULL;
		m_pVSTiMidiFiles[iIndex] = NULL;
	}
	for (int iIndex = 1; iIndex <= MAX_TRACK_NUM; iIndex++)
	{
		UpdateVSTiMidiUI(iIndex, eMLTrackUIState_NoVSTiAndNoMidi);
	}

	BOOL bInitVSTiPlayer = FALSE;
	m_pVSTiPlayer = MLVSTiPlayer_Create();
	if (NULL != m_pVSTiPlayer)
		bInitVSTiPlayer = m_pVSTiPlayer->Initialize();
	if (FALSE == bInitVSTiPlayer)
	{
		MessageBox(_T("Initliaze VSTi Player failed!"), _T("Error"), MB_OK);
		PostQuitMessage(0);
	}

	CComboBox* pComboBoxDevice = (CComboBox*)GetDlgItem(IDC_COMBO_DEVICE);

	char* pszDevName = NULL;
	DWORD dwOutDevCnt = m_pVSTiPlayer->GetOutDeviceCnt();
	for (DWORD dwIndex = 0; dwIndex < dwOutDevCnt; dwIndex++)
	{
		pszDevName = m_pVSTiPlayer->GetDeviceName(dwIndex);
		pComboBoxDevice->AddString(pszDevName);
	}
	pComboBoxDevice->SetCurSel(0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//--------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//--------------------------------------------------
void CMusicLegendAppDlg::OnPaint() 
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
//--------------------------------------------------
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
//--------------------------------------------------
HCURSOR CMusicLegendAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnDestroy()
{
	CDialog::OnDestroy();

	MLVSTiPlayer_Destroy(m_pVSTiPlayer);
	m_pVSTiPlayer = NULL;
}
//--------------------------------------------------




//--------------------------------------------------
void CMusicLegendAppDlg::UpdateVSTiMidiUI(int iTrack, eMLTrackUIState eState)
{
	CButton* pCheckMute = NULL;
	CButton* pBtnVSTi = NULL;
	CButton* pBtnMidi = NULL;
	CButton* pBtnPlayStop = NULL;
	CButton* pBtnVSTiInfo = NULL;
	CSliderCtrl* pSlider = NULL;
	CStatic* pStaticMidi = NULL;
	CStatic* pStaticVSTi = NULL;
	CStatic* pStaticTimeInfo = NULL;
	CButton* pBtnPlayStopAll = (CButton*)GetDlgItem(IDC_BTN_PLAYSTOP_ALL);

	switch(iTrack)
	{
	case 1:
		pCheckMute = (CButton*)GetDlgItem(IDC_CHECK_MUTE_1);
		pBtnVSTi = (CButton*)GetDlgItem(IDC_BTN_VSTI_1);
		pBtnMidi = (CButton*)GetDlgItem(IDC_BTN_MIDI_1);
		pBtnPlayStop = (CButton*)GetDlgItem(IDC_BTN_PLAYSTOP_1);
		pBtnVSTiInfo = (CButton*)GetDlgItem(IDC_BTN_VSTIINFO_1);
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_1);
		pStaticMidi = (CStatic*)GetDlgItem(IDC_STATIC_FILEINFO_1);
		pStaticVSTi = (CStatic*)GetDlgItem(IDC_STATIC_VSTINAME_1);
		pStaticTimeInfo = (CStatic*)GetDlgItem(IDC_STATIC_TIME_1);
		break;
	case 2:
		pCheckMute = (CButton*)GetDlgItem(IDC_CHECK_MUTE_2);
		pBtnVSTi = (CButton*)GetDlgItem(IDC_BTN_VSTI_2);
		pBtnMidi = (CButton*)GetDlgItem(IDC_BTN_MIDI_2);
		pBtnPlayStop = (CButton*)GetDlgItem(IDC_BTN_PLAYSTOP_2);
		pBtnVSTiInfo = (CButton*)GetDlgItem(IDC_BTN_VSTIINFO_2);
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_2);
		pStaticMidi = (CStatic*)GetDlgItem(IDC_STATIC_FILEINFO_2);
		pStaticVSTi = (CStatic*)GetDlgItem(IDC_STATIC_VSTINAME_2);
		pStaticTimeInfo = (CStatic*)GetDlgItem(IDC_STATIC_TIME_2);
		break;
	case 3:
		pCheckMute = (CButton*)GetDlgItem(IDC_CHECK_MUTE_3);
		pBtnVSTi = (CButton*)GetDlgItem(IDC_BTN_VSTI_3);
		pBtnMidi = (CButton*)GetDlgItem(IDC_BTN_MIDI_3);
		pBtnPlayStop = (CButton*)GetDlgItem(IDC_BTN_PLAYSTOP_3);
		pBtnVSTiInfo = (CButton*)GetDlgItem(IDC_BTN_VSTIINFO_3);
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_3);
		pStaticMidi = (CStatic*)GetDlgItem(IDC_STATIC_FILEINFO_3);
		pStaticVSTi = (CStatic*)GetDlgItem(IDC_STATIC_VSTINAME_3);
		pStaticTimeInfo = (CStatic*)GetDlgItem(IDC_STATIC_TIME_3);
		break;
	case 4:
		pCheckMute = (CButton*)GetDlgItem(IDC_CHECK_MUTE_4);
		pBtnVSTi = (CButton*)GetDlgItem(IDC_BTN_VSTI_4);
		pBtnMidi = (CButton*)GetDlgItem(IDC_BTN_MIDI_4);
		pBtnPlayStop = (CButton*)GetDlgItem(IDC_BTN_PLAYSTOP_4);
		pBtnVSTiInfo = (CButton*)GetDlgItem(IDC_BTN_VSTIINFO_4);
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_4);
		pStaticMidi = (CStatic*)GetDlgItem(IDC_STATIC_FILEINFO_4);
		pStaticVSTi = (CStatic*)GetDlgItem(IDC_STATIC_VSTINAME_4);
		pStaticTimeInfo = (CStatic*)GetDlgItem(IDC_STATIC_TIME_4);
		break;
	case 5:
		pCheckMute = (CButton*)GetDlgItem(IDC_CHECK_MUTE_5);
		pBtnVSTi = (CButton*)GetDlgItem(IDC_BTN_VSTI_5);
		pBtnMidi = (CButton*)GetDlgItem(IDC_BTN_MIDI_5);
		pBtnPlayStop = (CButton*)GetDlgItem(IDC_BTN_PLAYSTOP_5);
		pBtnVSTiInfo = (CButton*)GetDlgItem(IDC_BTN_VSTIINFO_5);
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_5);
		pStaticMidi = (CStatic*)GetDlgItem(IDC_STATIC_FILEINFO_5);
		pStaticVSTi = (CStatic*)GetDlgItem(IDC_STATIC_VSTINAME_5);
		pStaticTimeInfo = (CStatic*)GetDlgItem(IDC_STATIC_TIME_5);
		break;
	case 6:
		pCheckMute = (CButton*)GetDlgItem(IDC_CHECK_MUTE_6);
		pBtnVSTi = (CButton*)GetDlgItem(IDC_BTN_VSTI_6);
		pBtnMidi = (CButton*)GetDlgItem(IDC_BTN_MIDI_6);
		pBtnPlayStop = (CButton*)GetDlgItem(IDC_BTN_PLAYSTOP_6);
		pBtnVSTiInfo = (CButton*)GetDlgItem(IDC_BTN_VSTIINFO_6);
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_6);
		pStaticMidi = (CStatic*)GetDlgItem(IDC_STATIC_FILEINFO_6);
		pStaticVSTi = (CStatic*)GetDlgItem(IDC_STATIC_VSTINAME_6);
		pStaticTimeInfo = (CStatic*)GetDlgItem(IDC_STATIC_TIME_6);
		break;
	case 7:
		pCheckMute = (CButton*)GetDlgItem(IDC_CHECK_MUTE_7);
		pBtnVSTi = (CButton*)GetDlgItem(IDC_BTN_VSTI_7);
		pBtnMidi = (CButton*)GetDlgItem(IDC_BTN_MIDI_7);
		pBtnPlayStop = (CButton*)GetDlgItem(IDC_BTN_PLAYSTOP_7);
		pBtnVSTiInfo = (CButton*)GetDlgItem(IDC_BTN_VSTIINFO_7);
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_7);
		pStaticMidi = (CStatic*)GetDlgItem(IDC_STATIC_FILEINFO_7);
		pStaticVSTi = (CStatic*)GetDlgItem(IDC_STATIC_VSTINAME_7);
		pStaticTimeInfo = (CStatic*)GetDlgItem(IDC_STATIC_TIME_7);
		break;
	case 8:
		pCheckMute = (CButton*)GetDlgItem(IDC_CHECK_MUTE_8);
		pBtnVSTi = (CButton*)GetDlgItem(IDC_BTN_VSTI_8);
		pBtnMidi = (CButton*)GetDlgItem(IDC_BTN_MIDI_8);
		pBtnPlayStop = (CButton*)GetDlgItem(IDC_BTN_PLAYSTOP_8);
		pBtnVSTiInfo = (CButton*)GetDlgItem(IDC_BTN_VSTIINFO_8);
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_8);
		pStaticMidi = (CStatic*)GetDlgItem(IDC_STATIC_FILEINFO_8);
		pStaticVSTi = (CStatic*)GetDlgItem(IDC_STATIC_VSTINAME_8);
		pStaticTimeInfo = (CStatic*)GetDlgItem(IDC_STATIC_TIME_8);
		break;
	default:
		return;
	}

	switch(eState)
	{
	case eMLTrackUIState_NoVSTiAndNoMidi:
		pCheckMute->EnableWindow(FALSE);
		pBtnVSTi->EnableWindow(TRUE);
		pBtnMidi->EnableWindow(TRUE);
		pBtnPlayStop->EnableWindow(FALSE);
		pBtnVSTiInfo->EnableWindow(FALSE);
		pSlider->EnableWindow(FALSE);
		pStaticVSTi->EnableWindow(FALSE);
		pStaticMidi->EnableWindow(FALSE);
		pStaticTimeInfo->EnableWindow(FALSE);
		//
		pStaticVSTi->SetWindowText(_T("VSTi:"));
		pStaticMidi->SetWindowText(_T("Midi File:"));
		pStaticTimeInfo->SetWindowText(_T("0/0 s"));
		pBtnPlayStop->SetWindowText(_T("Play"));
		pBtnPlayStopAll->SetWindowText(_T("Play All"));
		break;
	case eMLTrackUIState_NoMidi:
		pCheckMute->EnableWindow(FALSE);
		pBtnVSTi->EnableWindow(TRUE);
		pBtnMidi->EnableWindow(TRUE);
		pBtnPlayStop->EnableWindow(FALSE);
		pBtnVSTiInfo->EnableWindow(TRUE);
		pSlider->EnableWindow(FALSE);
		pStaticVSTi->EnableWindow(TRUE);
		pStaticMidi->EnableWindow(FALSE);
		pStaticTimeInfo->EnableWindow(FALSE);
		//
		pStaticVSTi->SetWindowText(_T("VSTi:"));
		pStaticMidi->SetWindowText(_T("Midi File:"));
		pStaticTimeInfo->SetWindowText(_T("0/0 s"));
		pBtnPlayStop->SetWindowText(_T("Play"));
		pBtnPlayStopAll->SetWindowText(_T("Play All"));
		//
		UpdateVSTiName(iTrack, pStaticVSTi);
		break;
	case eMLTrackUIState_NoVSTi:
		pCheckMute->EnableWindow(TRUE);
		pBtnVSTi->EnableWindow(TRUE);
		pBtnMidi->EnableWindow(TRUE);
		pBtnPlayStop->EnableWindow(TRUE);
		pBtnVSTiInfo->EnableWindow(FALSE);
		pSlider->EnableWindow(TRUE);
		pStaticVSTi->EnableWindow(FALSE);
		pStaticMidi->EnableWindow(TRUE);
		pStaticTimeInfo->EnableWindow(TRUE);
		//
		pStaticVSTi->SetWindowText(_T("VSTi:"));
		pStaticMidi->SetWindowText(_T("Midi File:"));
		pStaticTimeInfo->SetWindowText(_T("0/0 s"));
		pBtnPlayStop->SetWindowText(_T("Play"));
		pBtnPlayStopAll->SetWindowText(_T("Play All"));
		//
		UpdateMidiName(iTrack, pStaticMidi);
		break;
	case eMLTrackUIState_Ready:
		pCheckMute->EnableWindow(TRUE);
		pBtnVSTi->EnableWindow(TRUE);
		pBtnMidi->EnableWindow(TRUE);
		pBtnPlayStop->EnableWindow(TRUE);
		pBtnVSTiInfo->EnableWindow(TRUE);
		pSlider->EnableWindow(TRUE);
		pStaticVSTi->EnableWindow(TRUE);
		pStaticMidi->EnableWindow(TRUE);
		pStaticTimeInfo->EnableWindow(TRUE);
		//
		pStaticVSTi->SetWindowText(_T("VSTi:"));
		pStaticMidi->SetWindowText(_T("Midi File:"));
		pStaticTimeInfo->SetWindowText(_T("0/0 s"));
		pBtnPlayStop->SetWindowText(_T("Play"));
		pBtnPlayStopAll->SetWindowText(_T("Play All"));
		//
		UpdateVSTiName(iTrack, pStaticVSTi);
		UpdateMidiName(iTrack, pStaticMidi);
		break;
	case eMLTrackUIState_Playing:
		pCheckMute->EnableWindow(TRUE);
		pBtnVSTi->EnableWindow(FALSE);
		pBtnMidi->EnableWindow(FALSE);
		pBtnPlayStop->EnableWindow(TRUE);
		pBtnVSTiInfo->EnableWindow(TRUE);
		pSlider->EnableWindow(TRUE);
		pStaticVSTi->EnableWindow(TRUE);
		pStaticMidi->EnableWindow(TRUE);
		pStaticTimeInfo->EnableWindow(TRUE);
		//
		pBtnPlayStop->SetWindowText(_T("Stop"));
		pBtnPlayStopAll->SetWindowText(_T("Stop All"));
		break;
	}
}
//--------------------------------------------------
CString	CMusicLegendAppDlg::GetOpenFileName(eMLFileType eFileType)
{
	CString strExt;
	switch(eFileType)
	{
	case eMLFileType_VSTi:
		strExt = _T("VSTi Files (*.dll)|*.dll||");
		break;
	case eMLFileType_Midi:
		strExt = _T("Midi Files (*.mid)|*.mid||");
		break;
	case eMLFileType_Audio:
		strExt = _T("Audio Files (*.wav)|*.wav||");
		break;
	default:
		return _T("");
	}

	CString strFileName;
	CFileDialog dlgOpen(TRUE, 
						NULL, 
						NULL, 
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
						strExt);
	if (dlgOpen.DoModal() == IDOK)
	{
		strFileName = dlgOpen.GetPathName();
	}

	return strFileName;
}
//--------------------------------------------------
BOOL CMusicLegendAppDlg::LoadVSTiEffect(int iTrack, const char* lpEffectFile)
{
	if (iTrack < 0 || iTrack >= MAX_TRACK_NUM)
		return FALSE;

	// Remove old effect in this track
	if (NULL != m_pVSTiEffects[iTrack])
	{
		int iOldEffectIndex = m_pVSTiPlayer->GetVSTiEffectIndex(m_pVSTiEffects[iTrack]);
		if (iOldEffectIndex >= 0)
		{
			m_pVSTiPlayer->UnLoadVSTiEffect(iOldEffectIndex);
			m_pVSTiEffects[iTrack] = NULL;
		}
	}

	// Load new effect DLL file
	IMLVSTiEffect* pNewEffect = m_pVSTiPlayer->LoadVSTiEffect(lpEffectFile);
	if (NULL != pNewEffect)
	{
		m_pVSTiEffects[iTrack] = pNewEffect;
		eMLTrackUIState eNewState = (NULL != m_pVSTiMidiFiles[iTrack]) ? eMLTrackUIState_Ready : eMLTrackUIState_NoMidi;
		UpdateVSTiMidiUI(iTrack, eNewState);
	}

	return TRUE;
}
//--------------------------------------------------
BOOL CMusicLegendAppDlg::LoadMidiFile(int iTrack, const char* lpMidiFile)
{
	if (iTrack < 0 || iTrack >= MAX_TRACK_NUM)
		return FALSE;

	// Remove old midi file in this track
	if (NULL != m_pVSTiMidiFiles[iTrack])
	{
		int iOldEffectIndex = m_pVSTiPlayer->GetMidiFileIndex(m_pVSTiMidiFiles[iTrack]);
		if (iOldEffectIndex >= 0)
		{
			m_pVSTiPlayer->UnLoadMidiFile(iOldEffectIndex);
			m_pVSTiMidiFiles[iTrack] = NULL;
		}
	}

	// Load new midi file
	IMLVSTiMidiFile* pNewMidi = m_pVSTiPlayer->LoadMidiFile(lpMidiFile);
	if (NULL != pNewMidi)
	{
		m_pVSTiMidiFiles[iTrack] = pNewMidi;
		eMLTrackUIState eNewState = (NULL != m_pVSTiEffects[iTrack]) ? eMLTrackUIState_Ready : eMLTrackUIState_NoVSTi;
		UpdateVSTiMidiUI(iTrack, eNewState);
	}

	return TRUE;
}
//--------------------------------------------------
void CMusicLegendAppDlg::UpdateVSTiName(int iTrack, CStatic* pStaticVSTiName)
{
	if (iTrack < 0 || iTrack >= MAX_TRACK_NUM)
		return;

	if (NULL == pStaticVSTiName)
		return;

	CString strVSTiEffectName = _T("VSTi:");
	if (NULL != m_pVSTiEffects[iTrack])
	{
		VSTiEffectInfo Info;
		m_pVSTiEffects[iTrack]->GetEffectInfo(Info);
		CString strEffectName = Info.csEffectName;
		strVSTiEffectName += strEffectName;
	}
	pStaticVSTiName->SetWindowText((LPCTSTR)strVSTiEffectName);
}
//--------------------------------------------------
void CMusicLegendAppDlg::UpdateMidiName(int iTrack, CStatic* pStaticMidiName)
{
	if (iTrack < 0 || iTrack >= MAX_TRACK_NUM)
		return;

	if (NULL == pStaticMidiName)
		return;

	CString strMidiFile = _T("Midi File:");
	if (NULL != m_pVSTiMidiFiles[iTrack])
	{
		MidiFileInfo Info;
		m_pVSTiMidiFiles[iTrack]->GetMidiFileInfo(Info);
		CString strFileName = Info.csFileName;
		strMidiFile += strFileName;
	}
	pStaticMidiName->SetWindowText((LPCTSTR)strMidiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::PlayStopMidiFile(int iTrack)
{
	if (iTrack < 0 || iTrack >= MAX_TRACK_NUM)
		return;

	if (eMLTrackUIState_Ready != m_eTrackUIStates[iTrack] &&
		eMLTrackUIState_Playing != m_eTrackUIStates[iTrack])
		return;

	int iEffectIndex = m_pVSTiPlayer->GetVSTiEffectIndex(m_pVSTiEffects[iTrack]);
	int iMidiFileIndex = m_pVSTiPlayer->GetMidiFileIndex(m_pVSTiMidiFiles[iTrack]);
	if (-1 != iMidiFileIndex)
	{
		if (eMLTrackUIState_Ready == m_eTrackUIStates[iTrack])
			m_pVSTiPlayer->PlayMidiFile(iMidiFileIndex, iEffectIndex);
		else
			m_pVSTiPlayer->StopMidiFile(iMidiFileIndex);
	}
}
//--------------------------------------------------
void CMusicLegendAppDlg::ShowVSTiEffectWindow(int iTrack)
{
	if (iTrack < 0 || iTrack >= MAX_TRACK_NUM)
		return;

	if (NULL == m_pVSTiEffects[iTrack])
		return;

	CVSTiInfoDlg dlg;
	dlg.m_pVSTiEffect = m_pVSTiEffects[iTrack];
	dlg.DoModal();
}
//--------------------------------------------------



//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVsti1()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_VSTi);
	LoadVSTiEffect(1, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnMidi1()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_Midi);
	LoadMidiFile(1, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnPlaystop1()
{
	PlayStopMidiFile(1);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVstiinfo1()
{
	ShowVSTiEffectWindow(1);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVsti2()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_VSTi);
	LoadVSTiEffect(2, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnMidi2()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_Midi);
	LoadMidiFile(2, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnPlaystop2()
{
	PlayStopMidiFile(2);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVstiinfo2()
{
	ShowVSTiEffectWindow(2);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVsti3()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_VSTi);
	LoadVSTiEffect(3, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnMidi3()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_Midi);
	LoadMidiFile(3, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnPlaystop3()
{
	PlayStopMidiFile(3);
}

void CMusicLegendAppDlg::OnBnClickedBtnVstiinfo3()
{
	ShowVSTiEffectWindow(3);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVsti4()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_VSTi);
	LoadVSTiEffect(4, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnMidi4()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_Midi);
	LoadMidiFile(4, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnPlaystop4()
{
	PlayStopMidiFile(4);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVstiinfo4()
{
	ShowVSTiEffectWindow(4);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVsti5()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_VSTi);
	LoadVSTiEffect(5, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnMidi5()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_Midi);
	LoadMidiFile(5, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnPlaystop5()
{
	PlayStopMidiFile(5);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVstiinfo5()
{
	ShowVSTiEffectWindow(5);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVsti6()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_VSTi);
	LoadVSTiEffect(6, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnMidi6()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_Midi);
	LoadMidiFile(6, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnPlaystop6()
{
	PlayStopMidiFile(6);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVstiinfo6()
{
	ShowVSTiEffectWindow(6);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVsti7()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_VSTi);
	LoadVSTiEffect(7, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnMidi7()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_Midi);
	LoadMidiFile(7, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnPlaystop7()
{
	PlayStopMidiFile(7);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVstiinfo7()
{
	ShowVSTiEffectWindow(7);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVsti8()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_VSTi);
	LoadVSTiEffect(8, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnMidi8()
{
	CString strVStiFile = GetOpenFileName(eMLFileType_Midi);
	LoadMidiFile(8, strVStiFile);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnPlaystop8()
{
	PlayStopMidiFile(8);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnVstiinfo8()
{
	ShowVSTiEffectWindow(8);
}
//--------------------------------------------------
void CMusicLegendAppDlg::OnBnClickedBtnPlaystopAll()
{
	// TODO: Add your control notification handler code here
}
//--------------------------------------------------
