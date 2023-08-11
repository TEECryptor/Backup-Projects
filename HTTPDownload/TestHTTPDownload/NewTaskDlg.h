//--------------------------------------------
//	File Name:NewTaskDlg.h
//	Introduce:Class CNewTaskDlg
//	Date:2010-05-10
//--------------------------------------------
#pragma once
//--------------------------------------------
#include "NewThreadDlg.h"
#include <list>
//--------------------------------------------
using namespace std;
typedef list<StreamTaskThreadInfo> ThreadInfoList;
//--------------------------------------------
// CNewTaskDlg dialog
//--------------------------------------------
class CNewTaskDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewTaskDlg)

public:
	CNewTaskDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewTaskDlg();

// Dialog Data
	enum { IDD = IDD_DLG_NEWTASK };
	//----------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//----------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnBrowse();
	//----------------------------------------
public:
	TCHAR	m_tcNetFileURL[1024];
	TCHAR	m_tcTargetFile[MAX_PATH];
	DWORD	m_dwThreadCnt;
	BOOL	m_bIsStreamTask;
	BOOL	m_bForceDownload;
	ThreadInfoList	m_lstThreadInfo;
	//----------------------------------------
	afx_msg void OnCbnSelchangeComboThreadcnt();
	afx_msg void OnBnClickedCheckForced();
	afx_msg void OnCbnSelchangeComboTasktype();
	afx_msg void OnBnClickedBtnAddthread();
	afx_msg void OnBnClickedBtnDelthread();
};
//--------------------------------------------
