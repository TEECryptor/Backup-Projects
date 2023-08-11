#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CContractInfoDlg dialog

class CContractInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CContractInfoDlg)

public:
	CContractInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CContractInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTRACT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ContractTypeCombox;
	CComboBox m_CustomNameCombox;
	CComboBox m_ContactCombox;
	CComboBox m_ContractStatusCombox;
	CDateTimeCtrl m_ContractStartDateCtrl;
	CDateTimeCtrl m_ContractEndDateCtrl;
	CColorListCtrl	m_EquipmentList;
	CADOTable*		m_pEquipmentTable;
	//
	CString	m_strContractID;
	CString	m_strCustomContractID;
	CString	m_strContactTel;
	CString	m_strProjectName;
	CString	m_strEquipment;
	float	m_fTotalPrice;
	CString	m_strSaler;
	CString	m_strComment;
	CString	m_strEquipment2;
	CString	m_strModel;
	DWORD	m_dwCount2;
	float	m_fPrice;
	float	m_fTotalPrice2;
	CString	m_strComment2;
	//
	CString	m_strCurSelCustomShortName;
	CString m_strCurSelCustomContact;
public:
	CADOTable*		m_pCurTable;
	eTableOptions	m_eTableOption;
	int 			m_iSelIndex;

private:
	void	_FillCustomNameComboBox();
	void	_FillContactComboBox();
	void	_GetInitializeData();
	void	_ShowEquipmentInfo(CString strContractID);
	float	_CountEquipmentCost(CString strContractID);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboContractdlgCustom();
	afx_msg void OnCbnSelchangeComboContractdlgContact();
	afx_msg void OnEnChangeEditContractdlgCount2();
	afx_msg void OnEnChangeEditContractdlgPrice();
	afx_msg void OnBnClickedBtnContractdlgEquipmentNew();
	afx_msg void OnBnClickedBtnContractdlgEquipmentModify();
	afx_msg void OnBnClickedBtnContractdlgEquipmentDelete();
	afx_msg void OnBnClickedBtnContractdlgNew();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnContractdlgDelete();
	afx_msg void OnNMClickListContractdlgEquipment(NMHDR *pNMHDR, LRESULT *pResult);
};
