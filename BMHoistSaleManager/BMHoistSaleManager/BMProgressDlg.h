//-----------------------------------------------------------------------
//	File Name:BMProgressDlg.h
//	Introduce:Class CBMProgressDlg header file
//-----------------------------------------------------------------------
#pragma once
#include "afxcmn.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMProgressDlg dialog
//-----------------------------------------------------------------------
class CBMProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMProgressDlg)

public:
	CBMProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMProgressDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROGRESS };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	//-------------------------------------------------------------------
public:
	void	SetProcessCnt(int iCnt);
	void	SetCurrentProcessName(int iIndex, CString strProcessName);
	void	SetCurrentProgress(int iPos);
	//-------------------------------------------------------------------
private:
	int				m_iTotalProcessCnt;
	int				m_iCurProcessIndex;
	CString			m_strCurProcessName;
	CProgressCtrl	m_curProgress;
	CProgressCtrl	m_totalProgress;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
