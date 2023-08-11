// TesAudioDlg.h : header file
//

#if !defined(AFX_TESAUDIODLG_H__38B8C565_3684_4B2E_8DF0_FAD9BD3A2C2B__INCLUDED_)
#define AFX_TESAUDIODLG_H__38B8C565_3684_4B2E_8DF0_FAD9BD3A2C2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//----------------------------------------------------
#include <dshow.h>
//----------------------------------------------------
enum 
{
	MS_Stopped,
	MS_Playing,
	MS_Paused
};
//----------------------------------------------------
class CTesAudioDlg : public CDialog
{
// Construction
public:
	CTesAudioDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTesAudioDlg)
	enum { IDD = IDD_TESAUDIO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTesAudioDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTesAudioDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnOpen();
	afx_msg void OnBtnPlay();
	afx_msg void OnBtnPause();
	afx_msg void OnBtnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//------------------------------------------------
private:
	HRESULT		InitDirectShow();
	//------------------------------------------------
private:	
    IGraphBuilder*		m_pGraphBuilder;
	IMediaControl*		m_pMediaControl;
	//
	CString				m_strMusicFile;
	UINT				m_uMusicState;
	//------------------------------------------------
};
//----------------------------------------------------
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESAUDIODLG_H__38B8C565_3684_4B2E_8DF0_FAD9BD3A2C2B__INCLUDED_)
//----------------------------------------------------
