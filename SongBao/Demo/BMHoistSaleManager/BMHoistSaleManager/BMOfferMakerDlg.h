//-----------------------------------------------------------------------
//	File Name:BMOfferMakerDlg.h
//	Introduce:Class CBMOfferMakerDlg header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "BMOfferContactsInfoDlg.h"
#include "BMOfferComponentsInfoDlg.h"
#include "BMParamCalculator.h"
#include "..\\Office\\Word\\MSWordApplication.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferMakerDlg dialog
//-----------------------------------------------------------------------
class CBMOfferMakerDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferMakerDlg)

public:
	CBMOfferMakerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferMakerDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OFFERMAKER };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
//	afx_msg void OnBnClickedBtnEdit();
//	afx_msg void OnBnClickedBtnSave();
//	afx_msg void OnBnClickedBtnPreview();
	afx_msg void OnCbnSelchangeComboOffermakerdlgSales();
	afx_msg void OnBnClickedBtnPrint();
	afx_msg void OnBnClickedBtnPrintdraw();
	afx_msg void OnBnClickedBtnEndcar();
	HRESULT OnUpdateTotalPrice(WPARAM wParam, LPARAM lParam);
	//-------------------------------------------------------------------
public:
	void	SetComponentPrices(CString strDlgCaption, CString strOfferNo, CString strProjectName, ComponentPriceList lstPriceInfo, CString strCurrency);
	//-------------------------------------------------------------------
private:
	void	FillSalesComoboBox();
	BOOL	ExportToWordTemplate(CString strDocFile);
	BOOL	ExportComponentDataToWordTemplate(CMSWordApplication* pWordApp, int iComponentIndex, ComPriceInfo component);
	BOOL	ExportTechDataToWordTemplate(CMSWordApplication* pWordApp, int iComponentIndex, ComPriceInfo component, CBMParamCalculator* pCalculator);
	BOOL	ExportDrawingToWordTemplate(ComponentPriceList lstSelPrices, CString strDrawingFolder);
	BOOL	ExportComponentDrawingToWordTemplate(CMSWordApplication* pWordApp, CBMParamCalculator* pCalculator);
	BOOL	ExportEndCarriageToWordTemplate(ComponentPriceList lstSelPrices, CString strEndCarFolder);
	CString	GetOfferTemplateFileName(int iComponentCnt);
	CString	GetDrawingTemplateFileName(CString strECType, eCraneCtrlType eCtrlType);
	CString GetEndCarTemplateFileName(CString strECType, TCHAR csDriverType, int iMotorCnt);
	//-------------------------------------------------------------------
private:
	CString						m_strDlgCaption;
	CString						m_strOfferNo;
	CString						m_strProjectName;
	CString						m_strCurrency;
	ComponentPriceList			m_lstPricesList;
	CBMOfferContactsInfoDlg*	m_pContactsInfoDlg;
	CBMOfferComponentsInfoDlg*	m_pComponentsInfoDlg;
	//-------------------------------------------------------------------
	CString		m_strSpecialNote;
	CString		m_strDownPlayment;
	CString		m_strLastPlayment;
	CString		m_strDeliveryTime;
	CString		m_strDeliveryType;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
