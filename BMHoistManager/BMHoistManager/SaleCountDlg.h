#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include <vector>


typedef struct tagMonthAmountInfo
{
	CString strMonth;
	float	fAmountMomey;
}MonthAmountInfo;

typedef std::vector<MonthAmountInfo> lstMonthAmountInfo;


// CSaleCountDlg dialog

class CSaleCountDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaleCountDlg)

public:
	CSaleCountDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaleCountDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_COUNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CComboBox		m_EquipmentTypeCombox;
	CComboBox		m_SalerCombox;
	CComboBox		m_ShowTypeCombox;
	CDateTimeCtrl	m_StartDateCtrl;
	CDateTimeCtrl	m_EndDateCtrl;
	DWORD			m_dwShowType;
	lstMonthAmountInfo	m_lstMonthAmountInfo;

public:
	eTableID	m_eCountTableID;

private:
	void	CountDataFromTable(eTableID tableID);
	void	AddMonthAmountInfo(CString strMonth, float fMomey);
	void	DrawMonthDataGraphCurve(HDC hDC, CRect rcRect, lstMonthAmountInfo lstData);
	void	DrawMonthDataGraphPillar(HDC hDC, CRect rcRect, lstMonthAmountInfo lstData);
	void	DrawMonthDataGraphPieChart(HDC hDC, CRect rcRect, lstMonthAmountInfo lstData);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeComboCountdlgEquipment();
	afx_msg void OnCbnSelchangeComboCountdlgSaler();
	afx_msg void OnCbnSelchangeComboCountdlgShowtype();
	afx_msg void OnDtnCloseupDatetimepickerCountdlgStart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnCloseupDatetimepickerCountdlgEnd(NMHDR *pNMHDR, LRESULT *pResult);
};
