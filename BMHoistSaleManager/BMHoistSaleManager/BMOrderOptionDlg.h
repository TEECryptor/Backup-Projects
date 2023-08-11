//-----------------------------------------------------------------------
//	File Name:BMOrderOptionDlg.h
//	Introduce:Class CBMOrderOptionDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "BMParamCalculator.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
struct DetailOrderCtrls
{
	DetailOrderCtrls()
	{
		hDialogWnd = NULL;
	}
	//-------------------------------------------------------------------
	void SetDialog(HWND hDlgWnd)
	{
		hDialogWnd = hDlgWnd;
	}
	//-------------------------------------------------------------------
	void SetInitData(int iCount, CString strCode, CString strFreeText = _T(""))
	{
		CString strCount;
		strCount.Format(_T("%d"), iCount);
		SetDlgItemText(hDialogWnd, nQtyEdit, strCount);
		SetDlgItemText(hDialogWnd, nCodeEdit, strCode);
		SetDlgItemText(hDialogWnd, nFreeTextEdit, strFreeText);
	}
	//-------------------------------------------------------------------
	HWND	hDialogWnd;
	UINT	nCheckBox;
	UINT	nQtyEdit;
	UINT	nCodeEdit;
	UINT	nFreeTextEdit;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
// CBMOrderOptionDlg dialog
//-----------------------------------------------------------------------
class CBMOrderOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOrderOptionDlg)

public:
	CBMOrderOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOrderOptionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ORDEROPTIONS };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	//-------------------------------------------------------------------
public:
	BOOL	SetOfferNoAndComName(CString strOfferNo, CString strComName);
	void	GetGeneralInfo(CString &strComName, CString &strComType, int &iComCount, CString &strComNote, CString &strComAdditional);
	BOOL	GetComponentInfo(eComponentType eType, CString &strCode, int &iCount, CString &strFreeText);
	CBMParamCalculator*	GetCalculator();
	//-------------------------------------------------------------------
private:
	void	InitDataControls();
	//-------------------------------------------------------------------
private:
	CString	m_strOfferNo;
	CString	m_strComName;
	CBMParamCalculator*	m_pCraneCalculator;
	DetailOrderCtrls	m_DetailOrderCtrls[eComponentType_Cnt];
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
