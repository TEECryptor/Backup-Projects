//-----------------------------------------------------------------------
//	File Name:BMCustomManagerDlg.h
//	Introduce:header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMCombinDataDlg dialog
//-----------------------------------------------------------------------
class CBMCombinDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMCombinDataDlg)

public:
	CBMCombinDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMCombinDataDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMPORTDATA };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnImpdlgOpensrc1();
	afx_msg void OnBnClickedBtnImpdlgOpensrc2();
	afx_msg void OnBnClickedCheckImpdlgSameas();
	afx_msg void OnBnClickedBtnOpendest();
	afx_msg void OnBnClickedBtnImpdlgGo();
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
