//----------------------------------------------------
//	File Name:AFUSaleRecordDlg.h
//	Introduce:Dialog class CAFUSaleRecordDlg interfaces definition
//----------------------------------------------------
#pragma once
//----------------------------------------------------
#include "DatabaseDefine.h"
#include "ADODataBase.h"
#include "ADOTable.h"
//----------------------------------------------------


//----------------------------------------------------
//	Name:CAFUSaleRecordDlg
//	Introduce:Class CAFUSaleRecordDlg
//----------------------------------------------------
class CAFUSaleRecordDlg : public CDialog
{
	DECLARE_DYNAMIC(CAFUSaleRecordDlg)
	//------------------------------------------------
public:
	CAFUSaleRecordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAFUSaleRecordDlg();
	//------------------------------------------------
// Dialog Data
	enum { IDD = IDD_DIALOG_SALERECORD };
	//------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	//------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioOldvip();
	afx_msg void OnBnClickedRadioNewvip();
	afx_msg void OnBnClickedBtnFindadd();
	afx_msg void OnCbnSelchangeComboGoodsname();
	afx_msg void OnEnChangeEditPrice();
	afx_msg void OnEnChangeEditNum();
	afx_msg void OnBnClickedOk();
	//------------------------------------------------
public:
	void	SetDatabaseAndTableName(CADODataBase* pDatabase, CString strVIPMemberTable, CString strGoodsTable, CString strEmployeeTable);
	void	SetSaleRecordInfo(DWORD dwVIPID, CTime &date, CString strSaleName, CString strGoodsName, float fPrice, LONG lCount, float fTotalCost, LONG lPoints, CString strEmployee, CString strOther);
	void	GetSaleRecordInfo(DWORD &dwVIPID, CString &strVIPName, CTime &date, CString &strSaleName, CString &strGoodsName, float &fPrice, LONG &lCount, float &fTotalCost, LONG &lPoints, CString &strEmployee, CString &strOther);
	//------------------------------------------------
private:
	void	_FillGoodsComboBox();
	void	_FillEmployeeComboBox();
	void	_InitControlData();
	BOOL	_FillVIPMemberInfomation();
	BOOL	_AddNewVIPMemberInfomation();
	//------------------------------------------------
private:
	CADODataBase*	m_pDatabase;
	CString			m_strVIPMemberTable;
	CString			m_strGoodsTable;
	CString			m_strEmployeeTable;
	BOOL			m_bIsOldVIPMember;
	//
	DWORD			m_dwVIPID;
	CString			m_strVIPName;
	CTime			m_tmDate;
	CString			m_strSaleName;
	CString			m_strGoodsName;
	float			m_fPrice;
	LONG			m_lCount;
	float			m_fTotalCost;
	LONG			m_lPoints;
	CString			m_strEmployee;
	CString			m_strOther;
	//------------------------------------------------
};
//----------------------------------------------------
