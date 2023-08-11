//--------------------------------------------------
// MusicLegendAppDlg.h : header file
//
//--------------------------------------------------
#pragma once
//--------------------------------------------------
#include "MLVSTiPlayer.h"
//--------------------------------------------------
#define	MAX_TRACK_NUM	8
//--------------------------------------------------
enum eMLTrackUIState
{
	eMLTrackUIState_NoVSTiAndNoMidi	= 0,
	eMLTrackUIState_NoMidi			= 1,
	eMLTrackUIState_NoVSTi			= 2,
	eMLTrackUIState_Ready			= 3,
	eMLTrackUIState_Playing			= 4,
	eMLTrackUIState_Cnt
};
//--------------------------------------------------
enum eMLFileType
{
	eMLFileType_VSTi				= 0,
	eMLFileType_Midi				= 1,
	eMLFileType_Audio				= 2,
	eMLFileType_Cnt
};	
//--------------------------------------------------
// CMusicLegendAppDlg dialog
//--------------------------------------------------
class CMusicLegendAppDlg : public CDialog
{
// Construction
public:
	CMusicLegendAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MUSICLEGENDAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//----------------------------------------------
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	//----------------------------------------------
private:
	void	UpdateVSTiMidiUI(int iTrack, eMLTrackUIState eState);
	CString	GetOpenFileName(eMLFileType eFileType);
	BOOL	LoadVSTiEffect(int iTrack, const char* lpEffectFile);
	BOOL	LoadMidiFile(int iTrack, const char* lpMidiFile);
	void	UpdateVSTiName(int iTrack, CStatic* pStaticVSTiName);
	void	UpdateMidiName(int iTrack, CStatic* pStaticMidiName);
	void	PlayStopMidiFile(int iTrack);
	void	ShowVSTiEffectWindow(int iTrack);
	//----------------------------------------------
private:
	IMLVSTiPlayer*		m_pVSTiPlayer;
	IMLVSTiEffect*		m_pVSTiEffects[MAX_TRACK_NUM];
	IMLVSTiMidiFile*	m_pVSTiMidiFiles[MAX_TRACK_NUM];
	eMLTrackUIState		m_eTrackUIStates[MAX_TRACK_NUM];
	//----------------------------------------------
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnVsti1();
	afx_msg void OnBnClickedBtnMidi1();
	afx_msg void OnBnClickedBtnPlaystop1();
	afx_msg void OnBnClickedBtnVstiinfo1();
	afx_msg void OnBnClickedBtnVsti2();
	afx_msg void OnBnClickedBtnMidi2();
	afx_msg void OnBnClickedBtnPlaystop2();
	afx_msg void OnBnClickedBtnVstiinfo2();
	afx_msg void OnBnClickedBtnVsti3();
	afx_msg void OnBnClickedBtnMidi3();
	afx_msg void OnBnClickedBtnPlaystop3();
	afx_msg void OnBnClickedBtnVstiinfo3();
	afx_msg void OnBnClickedBtnVsti4();
	afx_msg void OnBnClickedBtnMidi4();
	afx_msg void OnBnClickedBtnPlaystop4();
	afx_msg void OnBnClickedBtnVstiinfo4();
	afx_msg void OnBnClickedBtnVsti5();
	afx_msg void OnBnClickedBtnMidi5();
	afx_msg void OnBnClickedBtnPlaystop5();
	afx_msg void OnBnClickedBtnVstiinfo5();
	afx_msg void OnBnClickedBtnVsti6();
	afx_msg void OnBnClickedBtnMidi6();
	afx_msg void OnBnClickedBtnPlaystop6();
	afx_msg void OnBnClickedBtnVstiinfo6();
	afx_msg void OnBnClickedBtnVsti7();
	afx_msg void OnBnClickedBtnMidi7();
	afx_msg void OnBnClickedBtnPlaystop7();
	afx_msg void OnBnClickedBtnVstiinfo7();
	afx_msg void OnBnClickedBtnVsti8();
	afx_msg void OnBnClickedBtnMidi8();
	afx_msg void OnBnClickedBtnPlaystop8();
	afx_msg void OnBnClickedBtnVstiinfo8();
	afx_msg void OnBnClickedBtnPlaystopAll();
};
//--------------------------------------------------
