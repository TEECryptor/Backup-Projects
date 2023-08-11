//-----------------------------------------------------------------------
//	File Name:BMOfferOptionsDlg.h
//	Introduce:Class CBMOfferOptionsDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "BMParamCalculator.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
struct DetailPriceCtrls
{
	DetailPriceCtrls()
	{
		iQtyCount = 0;
		eCurrency = eCurrencyType_RMB;
		fCurrencyRate = 1.0f;
		fVATRate = 0.17f;
		iTotalOfferPrice = 0;
		hDialogWnd = NULL;
		bIsEndCarCtrls = FALSE;
	}
	//-------------------------------------------------------------------
	void SetDialog(HWND hDlgWnd)
	{
		hDialogWnd = hDlgWnd;
	}
	//-------------------------------------------------------------------
	void SetCurrency(eCurrencyType eCurrencyType, float fRate)
	{
		eCurrency = eCurrencyType;
		fCurrencyRate = fRate;
		switch(eCurrencyType)
		{
		case eCurrencyType_RMB:
			SetDlgItemText(hDialogWnd, nTotalUnitStatic, _T("RMB"));
			break;
		case eCurrencyType_EUR:
			SetDlgItemText(hDialogWnd, nTotalUnitStatic, _T("EUR"));
			break;
		case eCurrencyType_USD:
			SetDlgItemText(hDialogWnd, nTotalUnitStatic, _T("USD"));
			break;
		}
		UpdateOfferPrice();
	}
	//-------------------------------------------------------------------
	void SetVATRate(float fVATRate)
	{
		fVATRate = fVATRate;
		UpdateOfferPrice();
	}
	//-------------------------------------------------------------------
	void UpdateOfferPrice()
	{
		int iCount;
		int iUnitPrice;
		TCHAR tcCount[32] = {_T("\0")};
		TCHAR tcDiscount[32] = {_T("\0")};
		TCHAR tcUnitPrice[32] = {_T("\0")};
		GetDlgItemText(hDialogWnd, nQtyEdit, tcCount, 32);
		GetDlgItemText(hDialogWnd, nDiscountEdit, tcDiscount, 32);
		GetDlgItemText(hDialogWnd, nUnitPriceEdit, tcUnitPrice, 32);
		//		
		iCount = _ttol(tcCount);
		iUnitPrice = _ttol(tcUnitPrice);
		float fDiscount = _ttol(tcDiscount) / 100.0f;
		if (eCurrency != eCurrencyType_RMB)
		{
			iUnitPrice = int(iUnitPrice / fCurrencyRate);
			/*if (TRUE == bIsEndCarCtrls)
				iTotalOfferPrice = int((iCount / 2) * (iUnitPrice / (1+fVATRate) * (1-fDiscount)));
			else*/
				iTotalOfferPrice = int(/*iCount **/ (iUnitPrice / (1+fVATRate) * (1-fDiscount)));
		}
		else
		{
			/*if (TRUE == bIsEndCarCtrls)
				iTotalOfferPrice = int((iCount / 2) * (iUnitPrice  * (1-fDiscount)));
			else*/
				iTotalOfferPrice = int(/*iCount **/ (iUnitPrice  * (1-fDiscount)));
		}
		//
		CString strTotalPrice;
		strTotalPrice.Format(_T("%d"), iTotalOfferPrice);
		SetDlgItemText(hDialogWnd, nTotalPriceEdit, strTotalPrice);
	}
	//-------------------------------------------------------------------
	void SetInitData(int iCount, int iUnitPrice, int iDiscount, CString strCode, BOOL bIsEndCar = FALSE)
	{
		CString strCount;
		CString strUnitPrice;
		CString strDiscount;
		iQtyCount = iCount;
		strCount.Format(_T("%d"), iCount);
		strUnitPrice.Format(_T("%d"), iUnitPrice);
		strDiscount.Format(_T("%d"), iDiscount);
		SetDlgItemText(hDialogWnd, nQtyEdit, strCount);
		SetDlgItemText(hDialogWnd, nCodeEdit, strCode);
		SetDlgItemText(hDialogWnd, nDiscountEdit, strDiscount);
		SetDlgItemText(hDialogWnd, nUnitPriceEdit, strUnitPrice);
		bIsEndCarCtrls = bIsEndCar;
	}
	//-------------------------------------------------------------------
	void UpdateCheck()
	{
		HWND hBtn = GetDlgItem(hDialogWnd, nCheckBox);
		if (NULL != hBtn)
		{
			int iCheck = (int)::SendMessage(hBtn, BM_GETCHECK, 0, 0);

			CString strCount;
			strCount.Format(_T("%d"), (BST_CHECKED == iCheck) ? iQtyCount : 0);
			SetDlgItemText(hDialogWnd, nQtyEdit, strCount);
			EnableWindow(GetDlgItem(hDialogWnd, nQtyEdit), (BST_CHECKED == iCheck) ? TRUE : FALSE);
			EnableWindow(GetDlgItem(hDialogWnd, nDiscountEdit), (BST_CHECKED == iCheck) ? TRUE : FALSE);
		}
	}
	//-------------------------------------------------------------------
	UINT	nCheckBox;
	UINT	nQtyEdit;
	UINT	nCodeEdit;
	UINT	nUnitPriceEdit;
	UINT	nUnitUnitStatic;
	UINT	nDiscountEdit;
	UINT	nTotalPriceEdit;
	UINT	nTotalUnitStatic;
	//
	HWND			hDialogWnd;
	eCurrencyType	eCurrency;
	int				iQtyCount;
	float			fCurrencyRate;
	float			fVATRate;
	int				iTotalOfferPrice;
	BOOL			bIsEndCarCtrls;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
// CBMOfferOptionsDlg dialog
//-----------------------------------------------------------------------
class CBMOfferOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferOptionsDlg)

public:
	CBMOfferOptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferOptionsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OFFEROPTIONS };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//-------------------------------------------------------------------
public:
	BOOL	SetOfferNoAndComName(CString strOfferNo, CString strComName);
	void	SetCurrentCurrency(eCurrencyType eType, float fRate);
	void	SetVATRate(float fVAT);
	void	GetDetialPriceInfo(DetailPriceList &lstPrices);
	//-------------------------------------------------------------------
private:
	void	UpdateTotalPrice();
	//-------------------------------------------------------------------
public:
	HWND	m_hMsgParent;
	CString	m_strOfferNo;
	CString	m_strComName;
	int		m_iComCount;
	int		m_iComToatlPrice;
	CBMParamCalculator*	m_pCraneCalculator;
	//-------------------------------------------------------------------
private:
	DetailPriceCtrls	m_DetailPricesCtrls[eComponentType_Cnt];
	//-------------------------------------------------------------------
public:
	afx_msg void OnEnChangeEditOfferdlgHoistCnt();
	afx_msg void OnEnChangeEditOfferdlgEndcarCnt();
	afx_msg void OnEnChangeEditOfferdlgBridgedriCnt();
	afx_msg void OnEnChangeEditOfferdlgBridgepannelCnt();
	afx_msg void OnEnChangeEditOfferdlgFastoonCnt();
	afx_msg void OnEnChangeEditOfferdlgOperateunitCnt();
	afx_msg void OnEnChangeEditOfferdlgCranepowerCnt();
	afx_msg void OnEnChangeEditOfferdlgMaingirderCnt();
	afx_msg void OnBnClickedCheckHoist();
	afx_msg void OnBnClickedCheckEndcar();
	afx_msg void OnBnClickedCheckBridgedri();
	afx_msg void OnBnClickedCheckBridgepannel();
	afx_msg void OnBnClickedCheckFastoon();
	afx_msg void OnBnClickedCheckOperateunit();
	afx_msg void OnBnClickedCheckCranepower();
	afx_msg void OnBnClickedCheckMaingirder();
	afx_msg void OnEnChangeEditOfferdlgHoistDiscount();
	afx_msg void OnEnChangeEditOfferdlgEndcarDiscount();
	afx_msg void OnEnChangeEditOfferdlgBridgedriDiscount();
	afx_msg void OnEnChangeEditOfferdlgBridgepannelDiscount();
	afx_msg void OnEnChangeEditOfferdlgFastoonDiscount();
	afx_msg void OnEnChangeEditOfferdlgOperateunitDiscount();
	afx_msg void OnEnChangeEditOfferdlgCranepowerDiscount();
	afx_msg void OnEnChangeEditOfferdlgMaingirderDiscount();
	afx_msg void OnEnChangeEditOfferdlgCnt();
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
