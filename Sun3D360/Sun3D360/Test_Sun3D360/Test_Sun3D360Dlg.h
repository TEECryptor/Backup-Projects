// Test_Sun3D360Dlg.h : header file
//

#if !defined(AFX_TEST_SUN3D360DLG_H__7FC83B25_AE91_4A5B_AA51_FE75208350F1__INCLUDED_)
#define AFX_TEST_SUN3D360DLG_H__7FC83B25_AE91_4A5B_AA51_FE75208350F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/Sun3D360.h"

/////////////////////////////////////////////////////////////////////////////
// CTest_Sun3D360Dlg dialog
class CTest_Sun3D360Dlg : public CDialog
{
// Construction
public:
	CTest_Sun3D360Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTest_Sun3D360Dlg)
	enum { IDD = IDD_TEST_SUN3D360_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_Sun3D360Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTest_Sun3D360Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAutoplay();
	afx_msg void OnExit();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnManulplay();
	afx_msg void OnUpdateAutoplay(CCmdUI* pCmdUI);
	afx_msg void OnUpdateManulplay(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlay(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStop(CCmdUI* pCmdUI);
	afx_msg void OnFull();
	afx_msg void OnDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL			ReadIniFile(LPCTSTR lpFile);
	//-------------------------------------------------------
private:
	CSun3D360*		m_pSun3D360;
	TCHAR			m_szSceneFile[MAX_PATH];
	int				m_iInitType;
	float			m_fCameraAng;
	float			m_fCameraNear;
	float			m_fCameraFar;			
	BOOL			m_bPlay;				//播放，暂停
	BOOL            m_bAutoPlay;			//自动播放，手动播放
	//-------------------------------------------------------
	WINDOWPLACEMENT m_OldWndPlacement;		//用来保存原窗口位置
	BOOL			m_bFullScreen;			//全屏显示标志
	CRect			m_FullScreenRect;		//表示全屏显示时的窗口位置
	//-------------------------------------------------------
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_SUN3D360DLG_H__7FC83B25_AE91_4A5B_AA51_FE75208350F1__INCLUDED_)
