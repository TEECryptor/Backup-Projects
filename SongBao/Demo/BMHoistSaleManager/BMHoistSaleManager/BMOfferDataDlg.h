//-----------------------------------------------------------------------
//	File Name:BMOfferDataDlg.h
//	Introduce:Class CBMOfferDataDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorListCtrl.h"
//-----------------------------------------------------------------------
typedef map<int, CString>	NameIndexMap;
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferDataDlg dialog
//-----------------------------------------------------------------------
class CBMOfferDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferDataDlg)

public:
	CBMOfferDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferDataDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OFFERDATA };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboCustom();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnYes();
	afx_msg void OnBnClickedBtnCancel();
	//-------------------------------------------------------------------
public:
	void	SetOfferID(CString strOfferID, BOOL bIsEdit = TRUE);
	//-------------------------------------------------------------------
private:
	void	FillCustomComboBox();
	void	FillContactComboBox();
	void	FillStatusComboBox();
	void	FillComponentTypeComboBox();
	void	InitComponentLstCtrl();
	void	InitDataFromDatabase();
	//-------------------------------------------------------------------
private:
	CImageList		m_imgNULL;
	CColorListCtrl	m_lstComponents;
	BOOL			m_bIsEditOpt;
	CString			m_strInitOfferID;
	NameIndexMap	m_mapCompanyNo;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
