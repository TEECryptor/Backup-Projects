//----------------------------------------------------
//	File Name:AFUManagerDlg.h
//	Introduce:Class CAFUManagerDlg intefaces definition file
//
//----------------------------------------------------
#pragma once
//----------------------------------------------------
#include "DatabaseDefine.h"
#include "ADODataBase.h"
#include "ADOTable.h"
#include "ColorListCtrl.h"
//----------------------------------------------------
static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
//----------------------------------------------------
// CAFUManagerDlg class
//----------------------------------------------------
class CAFUManagerDlg : public CDialog
{
public:
	CAFUManagerDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_AFUMANAGER_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//------------------------------------------------
protected:
	HICON m_hIcon;
	CColorListCtrl m_RecordListCtrl;
	//------------------------------------------------
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnToolBarFind();
	afx_msg void OnToolBarAdd();
	afx_msg void OnToolBarEdit();
	afx_msg void OnToolBarDelete();
	afx_msg void OnToolBarSetting();
	afx_msg void OnToolBarClose();
	DECLARE_MESSAGE_MAP()
	//------------------------------------------------
public:
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTvnSelchangedTreeLeft(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslinkFirstpage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslinkUppage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslinkDownpage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslinkEndpage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSystemExit();
	afx_msg void OnUserChangepassword();
	afx_msg void OnUserSwitchuser();
	afx_msg void OnCbnSelchangeComboBirthyear();
	afx_msg void OnCbnSelchangeComboBirthmonth();
	afx_msg void OnCbnSelchangeComboToyear();
	afx_msg void OnCbnSelchangeComboTomonth();
	afx_msg void OnBnClickedButtonFind();
	//------------------------------------------------
private:
	void		CreateToolBar();
	void		CreateStatusBar();
	void		InitUIControls();
	void		FillTreeListItems();
	void		InitFindSetControls();
	void		FillBirthDayComboBox();
	void		FillToDayComboBox();
	void		InitTableStruct();
	void		UpdateUIControlsStatus();
	void		UpdateStatusBarInfo();
	void		ReadSettingFromRegistry();
	void		SaveSettingToRegistry();
	//------------------------------------------------
	BOOL		OpenDatabase();
	BOOL		EmployeeLogin();
	void		GetTodaySaleInfo(LONG &lSaleRecordCnt, float &fSaleCost);
	void		OpenTableByID(eTableID eSelTable, CString strOperation = _T(""));
	void		ShowCurrentTable(BOOL bIsMultiPage, LONG lCountPerPage, LONG lCurPage);
	void		UpdateVIPMemberTable(BOOL bIsAddNewItem);
	void		UpdateSaleRecordTable(BOOL bIsAddNewItem);
	void		UpdateAFUEmployeeTable(BOOL bIsAddNewItem);
	void		UpdateAFUGoodsTable(BOOL bIsAddNewItem);
	BOOL		UpdatePointsForVIPMember(DWORD dwVIPID, LONG lChangedPoints);
	BOOL		UpdateProductCountsForGoodsTable(CString strProductName, LONG lSaledCnt);
	LPWSTR		GetEmployeeGroupInfo(eEmployeeGroup eGroup);
	LPWSTR		EncodePasswordString(LPCWSTR lpwzPassword);
	LPWSTR		DecodePasswordString(LPCWSTR lpwzEncodedPassword);
	//------------------------------------------------
private:	
	CMenu			m_MainMenu;
	CToolBar		m_wndToolBar;
	//CStatusBar		m_wndStatusBar;
	CStatusBarCtrl	m_StatusBar;
	CImageList		m_ToolBarImages;
	CImageList		m_ToolBarDisableImages;
	CImageList		m_TreeItemImages;
	//
	HTREEITEM		m_hVIPMemberItem;
	HTREEITEM		m_hSaleRecordItem;
	HTREEITEM		m_hAdminItem;
	HTREEITEM		m_hEmployeeItem;
	HTREEITEM		m_hProductItem;
	//
	BOOL			m_bFindBtnIsPressed;
	DWORD			m_dwLoginUserID;
	CString			m_strLoginUserName;
	eEmployeeGroup	m_eLoginGroup;
	LONG			m_lTodaySaledCnt;
	float			m_fTodaySaleCost;
	//	
	CString			m_strDatabaseFile;
	TableData		m_Tables[eTableID_Cnt];
	CADODataBase*	m_pDatabase;
	CADOTable		m_TableObj;
	eTableID		m_eCurSelTable;
	BOOL			m_bIsMultiPage;
	LONG			m_lCountPerPage;
	LONG			m_lTotalPage;
	LONG			m_lCurPage;
	//------------------------------------------------
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkListMain(NMHDR *pNMHDR, LRESULT *pResult);
};
//----------------------------------------------------
