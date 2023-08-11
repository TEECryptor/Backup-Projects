#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// COfferInfoDlg dialog

class COfferInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(COfferInfoDlg)

public:
	COfferInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COfferInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OFFER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
private:
	CComboBox m_OfferTypeCombox;
	CComboBox m_CustomNameCombox;
	CComboBox m_ContactCombox;
	CComboBox m_OfferStatusCombox;
	CDateTimeCtrl m_OfferDateCtrl;
	CColorListCtrl	m_EquipmentList;
	CADOTable*		m_pEquipmentTable;
	//
	CString	m_strOfferID;
	CString	m_strOfferInputer;
	CString	m_strContactTel;
	CString	m_strProjectName;
	CString	m_strEquipment;
	DWORD	m_dwCount;
	float	m_fTotalPrice;
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
	//
private:
	void	_FillCustomNameComboBox();
	void	_FillContactComboBox();
	void	_GetInitializeData();
	void	_ShowEquipmentInfo(CString strOfferID);
	float	_CountEquipmentCost(CString strOfferID);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboOfferdlgCustom();
	afx_msg void OnCbnSelchangeComboOfferdlgContact();
	afx_msg void OnEnChangeEditOfferdlgCount2();
	afx_msg void OnEnChangeEditOfferdlgPrice();
	afx_msg void OnBnClickedBtnContractdlgEquimentNew();
	afx_msg void OnBnClickedBtnOfferdlgEquipmentModify();
	afx_msg void OnBnClickedBtnOfferdlgEquipmentDelete();
	afx_msg void OnBnClickedBtnOfferdlgNew();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnOfferdlgDelete();
	afx_msg void OnNMClickListOfferdlgEquipment(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditOfferdlgCount();
};
