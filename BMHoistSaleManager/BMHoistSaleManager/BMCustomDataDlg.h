//-----------------------------------------------------------------------
//	File Name:BMCustomManagerDlg.h
//	Introduce:header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorListCtrl.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMCustomDataDlg dialog
//-----------------------------------------------------------------------
class CBMCustomDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMCustomDataDlg)

public:
	CBMCustomDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMCustomDataDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CUSTOMDATA };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnModify();
	afx_msg void OnBnClickedBtnRemove();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnClear();
	//-------------------------------------------------------------------
public:
	void	SetCustomerID(CString strCompanyID, BOOL bIsEdit = TRUE);
	//-------------------------------------------------------------------
private:
	void	InitLevelComboBox();
	void	InitDataListCtrls();
	void	InitTablesDataStruct();
	void	InitDataFromDatabase();
	void	UpdateContactsInfoList(int iEditItem = -1);
	//-------------------------------------------------------------------
private:
	CImageList		m_imgNULL;
	CColorListCtrl	m_lstCtlContacts;
	TableData		m_stuContactsTable;
	BOOL			m_bIsEditOpt;
	CString			m_strInitCompanyID;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
