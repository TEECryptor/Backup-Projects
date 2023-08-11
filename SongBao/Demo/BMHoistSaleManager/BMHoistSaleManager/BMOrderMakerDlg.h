//-----------------------------------------------------------------------
//	File Name:BMOrderMakerDlg.h
//	Introduce:Class CBMOrderMakerDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "OptionTabCtrl.h"
#include "BMOrderOptionDlg.h"
#include "..\\Office\\Word\\MSWordApplication.h"
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMOrderMakerDlg dialog
//-----------------------------------------------------------------------
class CBMOrderMakerDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOrderMakerDlg)

public:
	CBMOrderMakerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOrderMakerDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ORDERDLG };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnOrderdlgPrint();
	//-------------------------------------------------------------------
public:
	void	SetOfferNo(CString strOfferNo);
	//-------------------------------------------------------------------
private:
	void	InitCaptionText();
	void	InitComponentTabCtrl();
	BOOL	ExportToWordTemplate(CString strDocFile);
	BOOL	ExportEndCarriageToWordTemplate(CString strEndCarFolder);
	CString	GetOrderTemplateFileName(int iComponentCnt);
	CString GetEndCarTemplateFileName(CString strECType, TCHAR csDriverType, int iMotorCnt);
	//-------------------------------------------------------------------
private:
	CString					m_strOfferNo;
	CString					m_strDlgCaption;
	CString					m_strProjectName;
	COptionTabCtrl			m_tabOptions;
	CBMOrderOptionDlg*		m_pOrderOptionsDlg[MAX_COMPONENT_CNT];
	//-------------------------------------------------------------------
public:
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeTabOrderoptions(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnOrderdlgEndcar();
};
//-----------------------------------------------------------------------
