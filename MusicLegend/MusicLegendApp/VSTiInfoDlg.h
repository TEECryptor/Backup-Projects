//--------------------------------------------------
#pragma once
//--------------------------------------------------
#include "MLVSTiPlayer.h"
//--------------------------------------------------
// CVSTiInfoDlg dialog
//--------------------------------------------------
class CVSTiInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CVSTiInfoDlg)
public:
	CVSTiInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVSTiInfoDlg();

// Dialog Data
	enum { IDD = IDD_DLG_VSTIINFO };
	//----------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//----------------------------------------------
public:
	IMLVSTiEffect*	m_pVSTiEffect;
	//----------------------------------------------
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
//--------------------------------------------------
