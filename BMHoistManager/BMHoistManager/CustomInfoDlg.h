#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ColorListCtrl.h"


// CCustomInfoDlg dialog

class CCustomInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CCustomInfoDlg)

public:
	CCustomInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCustomInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CUSTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CComboBox		m_CustomTypeCombox;
	CComboBox		m_SharedOrNotCombox;
	CColorListCtrl	m_ContactListCtrl;
	CComboBox		m_ContactSexCombox;
	CADOTable*		m_pContactTable;
private:
	void	_GetInitializeData();
	void	_GetInitializeDataByContact();
	DWORD	_GetNewCompanyIndex(CString strTypeShortName);
	void	_ShowContactInfo(CString strType, CString strCompanyShotrName);
public:
	CADOTable*		m_pCurTable;
	eTableOptions	m_eTableOption;
	int 			m_iSelIndex;
	BOOL			m_bIsModifyContact;
	//
private:
	CString m_strCustomID;
	CString m_strCustomInputer;
	CString m_strCustomName;
	CString m_strCustomShortName;
	CString m_strCustomFindCode;
	CString m_strCustomTel;
	CString m_strCustomFox;
	CString m_strCustomHttp;
	CString m_strCustomContact;
	CString m_strCustomContactTel;
	CString m_strCustomEmail;
	CString m_strCustomAddress;
	CString m_strCustomPostCode;
	CString m_strCustomBank;
	CString m_strCustomBankID;
	CString m_strCustomAccount;
	CString m_strCustomTaxID;
	CString m_strCustomComment;
	CString m_strContactName;
	CString m_strContactFindCode;
	CString m_strContactPosition;
	CString m_strContactTel;
	CString m_strContactFox;
	CString m_strContactCellTel;
	CString m_strContactEmail;
	CString m_strContactAddress;
	CString m_strContactPostCode;
	CString m_strContactComment;
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboCustomdlgType();
	afx_msg void OnNMClickListCustomdlgContacts(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListCustomdlgContacts(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuSetAsDefaultContact();
	afx_msg void OnBnClickedBtnCustomdlgContactNew();
	afx_msg void OnBnClickedBtnCustomdlgContactModify();
	afx_msg void OnBnClickedBtnCustomdlgContactDelete();
	afx_msg void OnBnClickedBtnCustomdlgNew();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnCustomdlgDelete();
};
