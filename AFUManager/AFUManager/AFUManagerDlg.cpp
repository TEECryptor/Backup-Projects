//----------------------------------------------------
//	File Name: AFUManagerDlg.cpp
//	Introduce: Class CAFUManagerDlg implement file
//
//----------------------------------------------------
#include "stdafx.h"
#include "AFUManager.h"
#include "AFUManagerDlg.h"
#include "AFULoginDlg.h"
#include "AFUMemberDlg.h"
#include "AFUSaleRecordDlg.h"
#include "AFUEmployeeDlg.h"
#include "AFUProductDlg.h"
#include "AFUChangePasswordDlg.h"
#include "AFUSettingDlg.h"
//----------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//----------------------------------------------------



//----------------------------------------------------
//	Name:CAFUManagerDlg
//	Introduce:Construction
//----------------------------------------------------
CAFUManagerDlg::CAFUManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAFUManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bFindBtnIsPressed = FALSE;
	m_lTodaySaledCnt = 0;
	m_fTodaySaleCost = 0.0f;
	m_bIsMultiPage = FALSE;
	m_lCountPerPage = 100;
	m_lCurPage = 0;
	m_lTotalPage = 1;
}
//----------------------------------------------------
void CAFUManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAIN, m_RecordListCtrl);
}
//----------------------------------------------------
BEGIN_MESSAGE_MAP(CAFUManagerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_COMMAND(ID_TBBUTTON_FIND, OnToolBarFind)
	ON_COMMAND(ID_TBBUTTON_ADD, OnToolBarAdd)
	ON_COMMAND(ID_TBBUTTON_EDIT, OnToolBarEdit)
	ON_COMMAND(ID_TBBUTTON_DELETE, OnToolBarDelete)
	ON_COMMAND(ID_TBBUTTON_SET, OnToolBarSetting)
	ON_COMMAND(ID_TBBUTTON_CLOSE, OnToolBarClose)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_LEFT, &CAFUManagerDlg::OnTvnSelchangedTreeLeft)
	ON_COMMAND(ID_SYSTEM_EXIT, &CAFUManagerDlg::OnSystemExit)
	ON_COMMAND(ID_USER_CHANGEPASSWORD, &CAFUManagerDlg::OnUserChangepassword)
	ON_COMMAND(ID_USER_SWITCHUSER, &CAFUManagerDlg::OnUserSwitchuser)
	ON_CBN_SELCHANGE(IDC_COMBO_BIRTHYEAR, &CAFUManagerDlg::OnCbnSelchangeComboBirthyear)
	ON_CBN_SELCHANGE(IDC_COMBO_BIRTHMONTH, &CAFUManagerDlg::OnCbnSelchangeComboBirthmonth)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CAFUManagerDlg::OnBnClickedButtonFind)
	ON_CBN_SELCHANGE(IDC_COMBO_TOYEAR, &CAFUManagerDlg::OnCbnSelchangeComboToyear)
	ON_CBN_SELCHANGE(IDC_COMBO_TOMONTH, &CAFUManagerDlg::OnCbnSelchangeComboTomonth)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_FIRSTPAGE, &CAFUManagerDlg::OnNMClickSyslinkFirstpage)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_UPPAGE, &CAFUManagerDlg::OnNMClickSyslinkUppage)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_DOWNPAGE, &CAFUManagerDlg::OnNMClickSyslinkDownpage)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_ENDPAGE, &CAFUManagerDlg::OnNMClickSyslinkEndpage)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MAIN, &CAFUManagerDlg::OnNMDblclkListMain)
END_MESSAGE_MAP()
//----------------------------------------------------



//----------------------------------------------------
//	Name:OnInitDialog
//	Introduce:Initialize dialog
//----------------------------------------------------
BOOL CAFUManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	const int APPSIZE_X = 1000;
	const int APPSIZE_Y = 680;
	SetWindowPos(NULL, 0, 0, APPSIZE_X, APPSIZE_Y, SWP_SHOWWINDOW);
	CenterWindow();

	//Create main menu
	m_MainMenu.LoadMenu(IDR_MENU_MAIN);
	SetMenu(&m_MainMenu);

	//Create ToolBar and buttons
	CreateToolBar();

	//Create status bar
	CreateStatusBar();

	//Set controls position
	InitUIControls();

	//Set tree list items;
	FillTreeListItems();

	//Set find setting controls
	InitFindSetControls();

	//Set tables struct
	InitTableStruct();

	//Set list view attributes
	m_RecordListCtrl.SetExtendedStyle(m_RecordListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	OnToolBarFind();

	//Open Database and login
	if (FALSE == OpenDatabase() || FALSE == EmployeeLogin())
	{
		::PostQuitMessage(0);
		return FALSE;
	}
	
	GetTodaySaleInfo(m_lTodaySaledCnt, m_fTodaySaleCost);

	UpdateStatusBarInfo();
	//
	CTreeCtrl* pTreeCtrl = (CTreeCtrl*)GetDlgItem(IDC_TREE_LEFT);
	pTreeCtrl->SelectItem(m_hVIPMemberItem);

	::UpdateWindow(m_hWnd);

	return TRUE;
}
//----------------------------------------------------
void CAFUManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
//----------------------------------------------------
HCURSOR CAFUManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//----------------------------------------------------
void CAFUManagerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_TableObj.CloseTable();
	if (NULL != m_pDatabase)
	{
		delete m_pDatabase;
		m_pDatabase = NULL;
	}
}
//----------------------------------------------------
BOOL CAFUManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return FALSE;
		case VK_F4:
			{
				int nVirtKey = GetKeyState(VK_CONTROL); 
				if (nVirtKey & 0x8000) 
				{
					return FALSE;
				}
			}
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
//----------------------------------------------------
void CAFUManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	this->InitUIControls();
}
//----------------------------------------------------
void CAFUManagerDlg::OnNMDblclkListMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	// Get current selected item
	POSITION pos = m_RecordListCtrl.GetFirstSelectedItemPosition();
	if (NULL == pos)
		return;
	int iSelItem = m_RecordListCtrl.GetNextSelectedItem(pos);
	CString strVIPID = m_RecordListCtrl.GetItemText(iSelItem, 1);
	
	if (eTableID_Member == m_eCurSelTable)
	{
		m_eCurSelTable = eTableID_Record;
		SetDlgItemText(IDC_EDIT_VIPID, strVIPID);
		OnBnClickedButtonFind();
		//
		CTreeCtrl* pTreeCtrl = (CTreeCtrl*)GetDlgItem(IDC_TREE_LEFT);
		pTreeCtrl->SelectItem(m_hSaleRecordItem);
	}
	else if (eTableID_Record == m_eCurSelTable)
	{
		m_eCurSelTable = eTableID_Member;
		SetDlgItemText(IDC_EDIT_VIPID, strVIPID);
		OnBnClickedButtonFind();
		//		
		CTreeCtrl* pTreeCtrl = (CTreeCtrl*)GetDlgItem(IDC_TREE_LEFT);
		pTreeCtrl->SelectItem(m_hVIPMemberItem);
	}

	*pResult = 0;
}
//----------------------------------------------------
void CAFUManagerDlg::OnTvnSelchangedTreeLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CRect rcTreeCtrl;
	CTreeCtrl* pTreeCtrl = (CTreeCtrl*)GetDlgItem(IDC_TREE_LEFT);
	HTREEITEM hSelItem = pTreeCtrl->GetSelectedItem();
	eTableID  tbID = (eTableID)pTreeCtrl->GetItemData(hSelItem);
	if (tbID != m_eCurSelTable)
	{
		m_eCurSelTable = tbID;
		OpenTableByID(m_eCurSelTable);
		UpdateUIControlsStatus();
	}

	*pResult = 0;
}
//----------------------------------------------------
void CAFUManagerDlg::OnNMClickSyslinkFirstpage(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_lCurPage = 0;
	ShowCurrentTable(m_bIsMultiPage, m_lCountPerPage, m_lCurPage);
	*pResult = 0;
}
//----------------------------------------------------
void CAFUManagerDlg::OnNMClickSyslinkUppage(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_lCurPage--;
	ShowCurrentTable(m_bIsMultiPage, m_lCountPerPage, m_lCurPage);
	*pResult = 0;
}
//----------------------------------------------------
void CAFUManagerDlg::OnNMClickSyslinkDownpage(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_lCurPage++;
	ShowCurrentTable(m_bIsMultiPage, m_lCountPerPage, m_lCurPage);
	*pResult = 0;
}
//----------------------------------------------------
void CAFUManagerDlg::OnNMClickSyslinkEndpage(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_lCurPage = MAXLONG;
	ShowCurrentTable(m_bIsMultiPage, m_lCountPerPage, m_lCurPage);
	*pResult = 0;
}
//----------------------------------------------------
void CAFUManagerDlg::OnToolBarFind()
{
	CToolBarCtrl& bar = m_wndToolBar.GetToolBarCtrl();
	if (TRUE  == m_bFindBtnIsPressed)
	{
		m_bFindBtnIsPressed = FALSE;
		bar.PressButton(ID_TBBUTTON_FIND, FALSE);
		//
		GetDlgItem(IDC_STATIC_FINDSET)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_STATIC_VIPID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VIPID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_VIPNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VIPNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BIRTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BIRTHYEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BIRTHMONTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BIRTHDAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_TOYEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_TOMONTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_TODAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_POINTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_POINTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_FIND)->ShowWindow(SW_HIDE);		
		((CEdit*)GetDlgItem(IDC_STATIC_VIPID))->SetFocus();
		//
		CRect rcListCtrl;
		m_RecordListCtrl.GetWindowRect(&rcListCtrl);
		ScreenToClient(&rcListCtrl);
		rcListCtrl.top -= 50;
		m_RecordListCtrl.MoveWindow(&rcListCtrl);
	}
	else
	{
		m_bFindBtnIsPressed = TRUE;
		bar.PressButton(ID_TBBUTTON_FIND, TRUE);
		//
		GetDlgItem(IDC_STATIC_FINDSET)->ShowWindow(SW_SHOW);	
		GetDlgItem(IDC_STATIC_VIPID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_VIPID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_VIPNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_VIPNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BIRTH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_BIRTHYEAR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_BIRTHMONTH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_BIRTHDAY)->ShowWindow(SW_SHOW);	
		if (eTableID_Record == m_eCurSelTable)
		{
			GetDlgItem(IDC_STATIC_TO)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_TOYEAR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_TOMONTH)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_TODAY)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_STATIC_POINTS)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_POINTS)->ShowWindow(SW_SHOW);
		}
		GetDlgItem(IDC_BUTTON_FIND)->ShowWindow(SW_SHOW);
		//
		CRect rcListCtrl;
		m_RecordListCtrl.GetWindowRect(&rcListCtrl);
		ScreenToClient(&rcListCtrl);
		rcListCtrl.top += 50;
		m_RecordListCtrl.MoveWindow(&rcListCtrl);
	}
}
//----------------------------------------------------
void CAFUManagerDlg::OnToolBarAdd()
{
	switch(m_eCurSelTable)
	{
	case eTableID_Member:
		UpdateVIPMemberTable(TRUE);
		break;
	case eTableID_Record:
		UpdateSaleRecordTable(TRUE);
		break;	
	case eTableID_User:
		UpdateAFUEmployeeTable(TRUE);
		break;	
	case eTableID_Goods:
		UpdateAFUGoodsTable(TRUE);
		break;
	}
}
//----------------------------------------------------
void CAFUManagerDlg::OnToolBarEdit()
{
	switch(m_eCurSelTable)
	{
	case eTableID_Member:
		UpdateVIPMemberTable(FALSE);
		break;
	case eTableID_Record:
		UpdateSaleRecordTable(FALSE);
		break;	
	case eTableID_User:
		UpdateAFUEmployeeTable(FALSE);
		break;	
	case eTableID_Goods:
		UpdateAFUGoodsTable(FALSE);
		break;
	}
}
//----------------------------------------------------
void CAFUManagerDlg::OnToolBarDelete()
{
	POSITION pos = m_RecordListCtrl.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("请先选择需要删除的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	int iSelItem = m_RecordListCtrl.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		MessageBox(_T("请先选择需要删除的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	CString strRecordIndex = m_RecordListCtrl.GetItemText(iSelItem, 0);
	int iSelRecordIndex = _ttol(strRecordIndex) - 1;

	if (IDYES == MessageBox(_T("您确定要删除选中的记录吗？"), _T("确认"), MB_YESNO | MB_ICONQUESTION))
	{
		if (m_TableObj.DeleteItem(iSelRecordIndex) && m_TableObj.UpdateTable())
		{
			OpenTableByID(m_eCurSelTable);
			MessageBox(_T("数据删除成功！"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		}
	}
}
//----------------------------------------------------
void CAFUManagerDlg::OnToolBarSetting()
{
	CAFUSettingDlg dlg;
	dlg.SetInitializeData(m_strDatabaseFile, m_bIsMultiPage, m_lCountPerPage);
	if (IDOK == dlg.DoModal())
	{
		CString strDBFile;
		BOOL bIsMultiPage;
		LONG lCountPerPage;
		dlg.GetSettingData(strDBFile, bIsMultiPage, lCountPerPage);
		if (strDBFile != m_strDatabaseFile)
		{
			m_strDatabaseFile = strDBFile;
			m_bIsMultiPage = bIsMultiPage;
			m_lCountPerPage = lCountPerPage;
			
			//TBD:Reconnect database and reopen table
		}
		else if (bIsMultiPage != m_bIsMultiPage || lCountPerPage != m_lCountPerPage)
		{
			m_bIsMultiPage = bIsMultiPage;
			m_lCountPerPage = lCountPerPage;
			m_lCurPage = 0;
			ShowCurrentTable(m_bIsMultiPage, m_lCountPerPage, m_lCurPage);
		}
	}
}
//----------------------------------------------------
void CAFUManagerDlg::OnToolBarClose()
{
	::PostQuitMessage(0);
}
//----------------------------------------------------
void CAFUManagerDlg::OnSystemExit()
{
	::PostQuitMessage(0);
}
//----------------------------------------------------
void CAFUManagerDlg::OnUserChangepassword()
{
	CADOTable tmEmployee;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE ID=%d", m_Tables[eTableID_User].strName, m_dwLoginUserID);
	if (tmEmployee.OpenTable(m_pDatabase->GetConnectionPtr(), strOpenOperation))
	{
		DWORD dwCount = tmEmployee.GetRecordCnt();
		if (1 == dwCount)
		{
			CString strPassword;
			tmEmployee.GetRecordCell(0, L"Password", strPassword.GetBuffer(64), 64);

			CAFUChangePasswordDlg dlg;
			dlg.SetOldPassword(DecodePasswordString(strPassword));
			if (IDOK == dlg.DoModal())
			{
				CString strNewPassword;
				dlg.GetNewPassword(strNewPassword);
				strPassword = EncodePasswordString(strNewPassword);
				tmEmployee.SetRecordCell(0, L"Password", strPassword.GetBuffer(strPassword.GetLength()));
				if (TRUE == tmEmployee.UpdateTable())
				{
					MessageBox(_T("密码修改成功，请保存好您设置的新密码!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
				}
			}
		}
	}
}
//----------------------------------------------------
void CAFUManagerDlg::OnUserSwitchuser()
{
	if (TRUE == EmployeeLogin())
	{
		UpdateUIControlsStatus();
		UpdateStatusBarInfo();
	}
}
//----------------------------------------------------
void CAFUManagerDlg::OnCbnSelchangeComboBirthyear()
{
	FillBirthDayComboBox();
}
//----------------------------------------------------
void CAFUManagerDlg::OnCbnSelchangeComboBirthmonth()
{
	FillBirthDayComboBox();
}
//----------------------------------------------------
void CAFUManagerDlg::OnCbnSelchangeComboToyear()
{
	FillToDayComboBox();
}
//----------------------------------------------------
void CAFUManagerDlg::OnCbnSelchangeComboTomonth()
{
	FillToDayComboBox();
}
//----------------------------------------------------
void CAFUManagerDlg::OnBnClickedButtonFind()
{
	if (eTableID_Member != m_eCurSelTable && eTableID_Record != m_eCurSelTable)
		return;

	BOOL bHasWhereString = FALSE;
	CString strFindSetting;
	strFindSetting.Format(_T("SELECT * FROM %s"), m_Tables[m_eCurSelTable].strName);
	//
	DWORD dwVIPID = 0;
	CString strVIPID;
	GetDlgItemText(IDC_EDIT_VIPID, strVIPID);
	dwVIPID = _ttol(strVIPID);
	if (0 != dwVIPID)
	{
		CString strTemp;
		strTemp.Format(_T(" WHERE VIPID LIKE '%d%s'"), dwVIPID, _T("%"));
		strFindSetting += strTemp;
		bHasWhereString = TRUE;
	}
	//
	CString strVIPName;
	GetDlgItemText(IDC_EDIT_VIPNAME, strVIPName);
	if (!strVIPName.IsEmpty())
	{
		CString strTemp;
		if (FALSE == bHasWhereString)
			strTemp.Format(_T(" WHERE NAME LIKE '%s%s'"), strVIPName, _T("%"));
		else
			strTemp.Format(_T(" AND NAME LIKE '%s%s'"), strVIPName, _T("%"));
		strFindSetting += strTemp;
		bHasWhereString = TRUE;
	}
	//
	CComboBox* pYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BIRTHYEAR);
	CComboBox* pMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BIRTHMONTH);
	CComboBox* pDayComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BIRTHDAY);
	CComboBox* pToYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOYEAR);
	CComboBox* pToMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOMONTH);
	CComboBox* pToDayComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TODAY);
	//
	CString strBirth;
	int iCurSel = pYearComboBox->GetCurSel();
	long lItemData = pYearComboBox->GetItemData(iCurSel);
	if (lItemData > 0)
	{
		CString strTemp;
		strTemp.Format(_T("%d"), lItemData);
		strBirth += strTemp;
		//
		iCurSel = pMonthComboBox->GetCurSel();
		lItemData = pMonthComboBox->GetItemData(iCurSel);
		if (lItemData > 0)
		{
			strTemp.Format(_T("-%d"), lItemData);
			strBirth += strTemp;
			//
			iCurSel = pDayComboBox->GetCurSel();
			lItemData = pDayComboBox->GetItemData(iCurSel);
			if (lItemData > 0)
			{
				strTemp.Format(_T("-%d"), lItemData);
				strBirth += strTemp;
			}
		}
	}
	//
	if (eTableID_Member == m_eCurSelTable)
	{
		CString strTemp;
		if (!strBirth.IsEmpty())
		{
			if (FALSE == bHasWhereString)
				strTemp.Format(_T(" WHERE BIRTH LIKE '%s%s'"), strBirth, _T("%"));
			else
				strTemp.Format(_T(" AND BIRTH LIKE '%s%s'"), strBirth, _T("%"));
			strFindSetting += strTemp;
			bHasWhereString = TRUE;
		}
		strTemp.Empty();

		LONG lPointsFrom = 0;
		LONG lPointsTo = 0;
		CComboBox* pPointsComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_POINTS);
		iCurSel = pPointsComboBox->GetCurSel();
		lItemData = pPointsComboBox->GetItemData(iCurSel);
		lPointsFrom = LOWORD(lItemData);
		lPointsTo = HIWORD(lItemData);
		if (0 == lPointsFrom && 0 != lPointsTo)
		{
			if (FALSE == bHasWhereString)
				strTemp.Format(_T(" WHERE POINTS < %d"), lPointsTo);
			else
				strTemp.Format(_T(" AND POINTS < %d"), lPointsTo);
		}
		else if (0 != lPointsFrom && 0 != lPointsTo)
		{
			if (FALSE == bHasWhereString)
				strTemp.Format(_T(" WHERE POINTS >= %d AND POINTS < %d"), lPointsFrom, lPointsTo);
			else
				strTemp.Format(_T(" AND POINTS < %d AND POINTS < %d"), lPointsFrom, lPointsTo);
		}
		else if (0 != lPointsFrom && 0 == lPointsTo)
		{
			if (FALSE == bHasWhereString)
				strTemp.Format(_T(" WHERE POINTS >= %d"), lPointsFrom);
			else
				strTemp.Format(_T(" AND POINTS >= %d"), lPointsFrom);
		}
		if (!strTemp.IsEmpty())
		{
			strFindSetting += strTemp;
			bHasWhereString = TRUE;
		}
	}
	else
	{
		CString strToDate;
		int iCurSel = pToYearComboBox->GetCurSel();
		long lItemData = pToYearComboBox->GetItemData(iCurSel);
		if (lItemData > 0)
		{
			CString strTemp;
			strTemp.Format(_T("%d"), lItemData);
			strToDate += strTemp;
			//
			iCurSel = pToMonthComboBox->GetCurSel();
			lItemData = pToMonthComboBox->GetItemData(iCurSel);
			if (lItemData > 0)
			{
				strTemp.Format(_T("-%d"), lItemData);
				strToDate += strTemp;
				//
				iCurSel = pToDayComboBox->GetCurSel();
				lItemData = pToDayComboBox->GetItemData(iCurSel);
				if (lItemData > 0)
				{
					strTemp.Format(_T("-%d"), lItemData);
					strToDate += strTemp;
				}
			}
		}

		CString strTempDate;
		if (!strBirth.IsEmpty() && strToDate.IsEmpty())
		{
			if (FALSE == bHasWhereString)
				strTempDate.Format(_T(" WHERE DATE >= #%s#"), strBirth);
			else
				strTempDate.Format(_T(" AND DATE >= #%s#"), strBirth);
		}
		else if (!strBirth.IsEmpty() && !strToDate.IsEmpty())
		{
			if (FALSE == bHasWhereString)
				strTempDate.Format(_T(" WHERE DATE >= #%s# AND DATE <= #%s#"), strBirth, strToDate);
			else
				strTempDate.Format(_T(" AND DATE >= #%s# AND DATE <= #%s#"), strBirth, strToDate);
		}
		else if(strBirth.IsEmpty() && !strToDate.IsEmpty())
		{			
			if (FALSE == bHasWhereString)
				strTempDate.Format(_T(" WHERE DATE <= #%s#"), strToDate);
			else
				strTempDate.Format(_T(" AND DATE <= #%s#"), strToDate);
		}
		if (!strTempDate.IsEmpty())
		{
			strFindSetting += strTempDate;
			bHasWhereString = TRUE;
		}
	}
	
	OpenTableByID(m_eCurSelTable, strFindSetting);

}
//----------------------------------------------------





//----------------------------------------------------
void CAFUManagerDlg::CreateToolBar()
{
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT|TBSTYLE_LIST, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_MAIN))
	{
		TRACE0("Can't create toolbar\n");
		return;
	}
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

	VERIFY(m_wndToolBar.SetButtonText(0, _T("查找")));
	VERIFY(m_wndToolBar.SetButtonText(1, _T("添加")));
	VERIFY(m_wndToolBar.SetButtonText(2, _T("修改")));
	VERIFY(m_wndToolBar.SetButtonText(3, _T("删除")));
	VERIFY(m_wndToolBar.SetButtonText(5, _T("设置")));
	VERIFY(m_wndToolBar.SetButtonText(7, _T("退出")));

	CRect rc(0, 0, 0, 0);
	CToolBarCtrl& bar = m_wndToolBar.GetToolBarCtrl();
	bar.GetItemRect(0, rc);

	CBitmap bp;
	BOOL bRes = bp.LoadBitmap(IDB_BITMAP_TOOLBAR);
	bRes = m_ToolBarImages.Create(32, 32, TRUE | ILC_COLOR24, 6, 0 );
	int iRes = m_ToolBarImages.Add(&bp, RGB(0,0,0));
	bar.SetImageList(&m_ToolBarImages);
	bp.Detach();
	//
	bRes = bp.LoadBitmap(IDB_BITMAP_TOOLBAR_DISABLE);
	bRes = m_ToolBarDisableImages.Create(32, 32, TRUE | ILC_COLOR24, 6, 0 );
	iRes = m_ToolBarDisableImages.Add(&bp, RGB(0,0,0));
	bar.SetDisabledImageList(&m_ToolBarDisableImages);
	bp.Detach();
}
//----------------------------------------------------
void CAFUManagerDlg::CreateStatusBar()
{
	int strPartDim[3]= {300, 800, -1};
	m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW|CCS_BOTTOM, CRect(0,0,0,0), this, 0); 
	m_StatusBar.SetParts(3, strPartDim);	
	m_StatusBar.SetText(_T("当前用户:未知  用户组:未知"), 0, 0);
}
//----------------------------------------------------
void CAFUManagerDlg::InitUIControls()
{
	if (NULL == m_StatusBar.m_hWnd)
		return;

	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcStatusBar;
	m_StatusBar.GetWindowRect(&rcStatusBar);
	ScreenToClient(&rcStatusBar);
	int iStatusBarHeight = rcStatusBar.Height();
	rcStatusBar.left = 5;
	rcStatusBar.right = rcClient.right - 5;
	rcStatusBar.bottom = rcClient.bottom;
	rcStatusBar.top = rcStatusBar.bottom - iStatusBarHeight;
	m_StatusBar.MoveWindow(&rcStatusBar);

	//For tree control
	CRect rcTreeCtrl;
	CTreeCtrl* pTreeCtrl = (CTreeCtrl*)GetDlgItem(IDC_TREE_LEFT);
	pTreeCtrl->GetWindowRect(&rcTreeCtrl);
	ScreenToClient(&rcTreeCtrl);
	rcTreeCtrl.left = 10;
	rcTreeCtrl.bottom = rcStatusBar.top - 5;
	rcTreeCtrl.right = rcTreeCtrl.left + 160;
	pTreeCtrl->MoveWindow(&rcTreeCtrl);

	//For bottom page controls
	CRect rcStaticBottom;
	CStatic* pStaticBottom = (CStatic*)GetDlgItem(IDC_STATIC_BOTTOM);
	rcStaticBottom.left = rcTreeCtrl.right + 10;
	rcStaticBottom.right = rcStatusBar.right - 5;
	rcStaticBottom.bottom = rcTreeCtrl.bottom;
	rcStaticBottom.top = rcStaticBottom.bottom - 24;
	pStaticBottom->MoveWindow(&rcStaticBottom);
	//
	int iWidth, iHeight;
	CRect rcStaticPageInfo;
	CStatic* pStaticPageInfo = (CStatic*)GetDlgItem(IDC_STATIC_PAGEINFO);
	pStaticPageInfo->GetWindowRect(&rcStaticPageInfo);
	ScreenToClient(&rcStaticPageInfo);
	iWidth = rcStaticPageInfo.Width();
	iHeight = rcStaticPageInfo.Height();
	rcStaticPageInfo.left = rcStaticBottom.left + 5;
	rcStaticPageInfo.right = rcStaticPageInfo.left + iWidth;
	rcStaticPageInfo.top = rcStaticBottom.top + 5;
	rcStaticPageInfo.bottom = rcStaticPageInfo.top + iHeight;
	pStaticPageInfo->MoveWindow(&rcStaticPageInfo);
	//
	CRect rcFirstPage;
	CWnd* pFirstPageLink = GetDlgItem(IDC_SYSLINK_FIRSTPAGE);
	pFirstPageLink->GetWindowRect(&rcFirstPage);
	ScreenToClient(&rcFirstPage);
	iWidth = rcFirstPage.Width();
	iHeight = rcFirstPage.Height();
	rcFirstPage.left = rcStaticBottom.left + (rcStaticBottom.Width() - 4*iWidth - 3*10) / 2;
	rcFirstPage.right = rcFirstPage.left + iWidth;
	rcFirstPage.top = rcStaticPageInfo.top;
	rcFirstPage.bottom = rcStaticPageInfo.bottom;
	pFirstPageLink->MoveWindow(&rcFirstPage);
	//
	CRect rcUpPage;
	CWnd* pUpPageLink = GetDlgItem(IDC_SYSLINK_UPPAGE);
	pUpPageLink->GetWindowRect(&rcUpPage);
	ScreenToClient(&rcUpPage);
	iWidth = rcUpPage.Width();
	iHeight = rcUpPage.Height();
	rcUpPage.left = rcFirstPage.right + 10;
	rcUpPage.right = rcUpPage.left + iWidth;
	rcUpPage.top = rcStaticPageInfo.top;
	rcUpPage.bottom = rcStaticPageInfo.bottom;
	pUpPageLink->MoveWindow(&rcUpPage);
	//
	CRect rcDownPage;
	CWnd* pDownPageLink = GetDlgItem(IDC_SYSLINK_DOWNPAGE);
	pDownPageLink->GetWindowRect(&rcDownPage);
	ScreenToClient(&rcDownPage);
	iWidth = rcDownPage.Width();
	iHeight = rcDownPage.Height();
	rcDownPage.left = rcUpPage.right + 10;
	rcDownPage.right = rcDownPage.left + iWidth;
	rcDownPage.top = rcStaticPageInfo.top;
	rcDownPage.bottom = rcStaticPageInfo.bottom;
	pDownPageLink->MoveWindow(&rcDownPage);
	//
	CRect rcEndPage;
	CWnd* pEndPageLink = GetDlgItem(IDC_SYSLINK_ENDPAGE);
	pEndPageLink->GetWindowRect(&rcEndPage);
	ScreenToClient(&rcEndPage);
	iWidth = rcEndPage.Width();
	iHeight = rcEndPage.Height();
	rcEndPage.left = rcDownPage.right + 10;
	rcEndPage.right = rcEndPage.left + iWidth;
	rcEndPage.top = rcStaticPageInfo.top;
	rcEndPage.bottom = rcStaticPageInfo.bottom;
	pEndPageLink->MoveWindow(&rcEndPage);

	//For find set group
	CRect rcFindSetStatic;
	CStatic* pFindSetStatic = (CStatic*)GetDlgItem(IDC_STATIC_FINDSET);
	pFindSetStatic->GetWindowRect(&rcFindSetStatic);
	ScreenToClient(&rcFindSetStatic);
	rcFindSetStatic.left = rcStaticBottom.left;
	rcFindSetStatic.right = rcStaticBottom.right;
	rcFindSetStatic.top = rcTreeCtrl.top;
	rcFindSetStatic.bottom = rcFindSetStatic.top + 44;
	pFindSetStatic->MoveWindow(&rcFindSetStatic);

	//For List control
	CRect rcListCtrl;
	m_RecordListCtrl.GetWindowRect(&rcListCtrl);
	ScreenToClient(&rcListCtrl);
	rcListCtrl.left = rcStaticBottom.left;
	rcListCtrl.right = rcStaticBottom.right;
	rcListCtrl.top = m_bFindBtnIsPressed ?  rcFindSetStatic.bottom + 5 : rcTreeCtrl.top;
	rcListCtrl.bottom = rcStaticBottom.top - 5;
	m_RecordListCtrl.MoveWindow(&rcListCtrl);
	
}
//----------------------------------------------------
void CAFUManagerDlg::FillTreeListItems()
{	
	CBitmap bp;
	BOOL bRes = bp.LoadBitmap(IDB_BITMAP_TREEITEM);
	bRes = m_TreeItemImages.Create(16, 16, TRUE | ILC_COLOR24, 5, 0 );
	int iRes = m_TreeItemImages.Add(&bp, RGB(0,0,0));
	bp.Detach();

	//For tree control
	CRect rcTreeCtrl;
	CTreeCtrl* pTreeCtrl = (CTreeCtrl*)GetDlgItem(IDC_TREE_LEFT);
	pTreeCtrl->SetImageList(&m_TreeItemImages, LVSIL_NORMAL);

	//"会员管理"
	m_hVIPMemberItem = pTreeCtrl->InsertItem(_T("会员管理"), 0, 0);
	pTreeCtrl->SetItemData(m_hVIPMemberItem, eTableID_Member);

	//"销售管理"
	m_hSaleRecordItem = pTreeCtrl->InsertItem(_T("销售管理"), 1, 1);
	pTreeCtrl->SetItemData(m_hSaleRecordItem, eTableID_Record);

	//"系统管理"
	m_hAdminItem = pTreeCtrl->InsertItem(_T("系统管理"), 2, 2);
	pTreeCtrl->SetItemData(m_hAdminItem, eTableID_None);

	//"商品管理"
	m_hProductItem = pTreeCtrl->InsertItem(_T("商品管理"), 4, 4, m_hAdminItem);
	pTreeCtrl->SetItemData(m_hProductItem, eTableID_Goods);

	//"员工管理"
	m_hEmployeeItem = pTreeCtrl->InsertItem(_T("员工管理"), 3, 3, m_hAdminItem);
	pTreeCtrl->SetItemData(m_hEmployeeItem, eTableID_User);

	pTreeCtrl->Expand(m_hAdminItem, TVE_EXPAND);
}
//----------------------------------------------------
void CAFUManagerDlg::InitFindSetControls()
{
	CComboBox* pYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BIRTHYEAR);
	CComboBox* pMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BIRTHMONTH);
	pYearComboBox->ResetContent();
	pMonthComboBox->ResetContent();
	int iIndex = pYearComboBox->AddString(_T("年"));
	pYearComboBox->SetItemData(iIndex, 0);
	iIndex = pMonthComboBox->AddString(_T("月"));
	pMonthComboBox->SetItemData(iIndex, 0);
	//
	CTime tmNow;
	tmNow = CTime::GetCurrentTime();
	for (int iYear = 1950; iYear <= tmNow.GetYear(); iYear++)
	{
		CString strYear;
		strYear.Format(_T("%d"), iYear);
		int iIndex = pYearComboBox->AddString(strYear);
		pYearComboBox->SetItemData(iIndex, iYear);
	}
	for (int iMonth = 1; iMonth <= 12; iMonth++)
	{
		CString strMonth;
		strMonth.Format(_T("%02d"), iMonth);
		int iIndex = pMonthComboBox->AddString(strMonth);
		pMonthComboBox->SetItemData(iIndex, iMonth);
	}
	pYearComboBox->SetCurSel(0);
	pMonthComboBox->SetCurSel(0);
	FillBirthDayComboBox();

	CRect rcStaticBirth;
	CRect rcComboBoxBirth;
	GetDlgItem(IDC_STATIC_BIRTH)->GetWindowRect(&rcStaticBirth);
	pYearComboBox->GetWindowRect(&rcComboBoxBirth);
	ScreenToClient(&rcStaticBirth);
	ScreenToClient(&rcComboBoxBirth);

	CRect rcStaticTo;
	CWnd* pStaticTo = GetDlgItem(IDC_STATIC_TO);
	pStaticTo->GetWindowRect(&rcStaticTo);
	ScreenToClient(&rcStaticTo);
	rcStaticTo.top = rcStaticBirth.top;
	rcStaticTo.bottom = rcStaticBirth.bottom;
	pStaticTo->MoveWindow(&rcStaticTo);
	pStaticTo->ShowWindow(SW_HIDE);
	//
	CRect rcComboBoxToYear;
	CRect rcComboBoxToMonth;
	CRect rcComboBoxToDay;
	CComboBox* pComboBoxToYear = (CComboBox*)GetDlgItem(IDC_COMBO_TOYEAR);
	CComboBox* pComboBoxToMonth = (CComboBox*)GetDlgItem(IDC_COMBO_TOMONTH);
	CComboBox* pComboBoxToDay = (CComboBox*)GetDlgItem(IDC_COMBO_TODAY);
	pComboBoxToYear->GetWindowRect(&rcComboBoxToYear);
	pComboBoxToMonth->GetWindowRect(&rcComboBoxToMonth);
	pComboBoxToDay->GetWindowRect(&rcComboBoxToDay);
	ScreenToClient(&rcComboBoxToYear);
	ScreenToClient(&rcComboBoxToMonth);
	ScreenToClient(&rcComboBoxToDay);
	rcComboBoxToYear.top = rcComboBoxBirth.top;
	rcComboBoxToYear.bottom = rcComboBoxBirth.bottom;
	rcComboBoxToMonth.top = rcComboBoxBirth.top;
	rcComboBoxToMonth.bottom = rcComboBoxBirth.bottom;
	rcComboBoxToDay.top = rcComboBoxBirth.top;
	rcComboBoxToDay.bottom = rcComboBoxBirth.bottom;
	pComboBoxToYear->MoveWindow(&rcComboBoxToYear);
	pComboBoxToMonth->MoveWindow(&rcComboBoxToMonth);
	pComboBoxToDay->MoveWindow(&rcComboBoxToDay);
	pComboBoxToYear->ShowWindow(SW_HIDE);
	pComboBoxToMonth->ShowWindow(SW_HIDE);
	pComboBoxToDay->ShowWindow(SW_HIDE);
	//
	pComboBoxToYear->ResetContent();
	pComboBoxToMonth->ResetContent();
	iIndex = pComboBoxToYear->AddString(_T("年"));
	pComboBoxToYear->SetItemData(iIndex, 0);
	iIndex = pComboBoxToMonth->AddString(_T("月"));
	pComboBoxToMonth->SetItemData(iIndex, 0);
	for (int iYear = 2011; iYear <= tmNow.GetYear(); iYear++)
	{
		CString strYear;
		strYear.Format(_T("%d"), iYear);
		int iIndex = pComboBoxToYear->AddString(strYear);
		pComboBoxToYear->SetItemData(iIndex, iYear);
	}
	for (int iMonth = 1; iMonth <= 12; iMonth++)
	{
		CString strMonth;
		strMonth.Format(_T("%02d"), iMonth);
		int iIndex = pComboBoxToMonth->AddString(strMonth);
		pComboBoxToMonth->SetItemData(iIndex, iMonth);
	}
	pComboBoxToYear->SetCurSel(0);
	pComboBoxToMonth->SetCurSel(0);
	FillToDayComboBox();

	CComboBox* pPointsComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_POINTS);
	pPointsComboBox->ResetContent();
	iIndex = pPointsComboBox->AddString(_T("不限"));
	pPointsComboBox->SetItemData(iIndex, MAKELONG(0, 0));
	iIndex = pPointsComboBox->AddString(_T("2000以下"));
	pPointsComboBox->SetItemData(iIndex, MAKELONG(0, 2000));
	iIndex = pPointsComboBox->AddString(_T("2000-5000"));
	pPointsComboBox->SetItemData(iIndex, MAKELONG(2000, 5000));
	iIndex = pPointsComboBox->AddString(_T("5000-8000"));
	pPointsComboBox->SetItemData(iIndex, MAKELONG(5000, 8000));
	iIndex = pPointsComboBox->AddString(_T("8000-10000"));
	pPointsComboBox->SetItemData(iIndex, MAKELONG(8000, 10000));
	iIndex = pPointsComboBox->AddString(_T("10000-15000"));
	pPointsComboBox->SetItemData(iIndex, MAKELONG(10000, 15000));
	iIndex = pPointsComboBox->AddString(_T("15000-20000"));
	pPointsComboBox->SetItemData(iIndex, MAKELONG(15000, 20000));
	iIndex = pPointsComboBox->AddString(_T("20000以上"));
	pPointsComboBox->SetItemData(iIndex, MAKELONG(20000, 0));
	pPointsComboBox->SetCurSel(0);

}
//----------------------------------------------------
void CAFUManagerDlg::FillBirthDayComboBox()
{
	CComboBox* pDayComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BIRTHDAY);
	pDayComboBox->ResetContent();
	int iIndex = pDayComboBox->AddString(_T("日"));
	pDayComboBox->SetItemData(iIndex, 0);
	//
	CComboBox* pYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BIRTHYEAR);
	CComboBox* pMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BIRTHMONTH);
	int iCurSel = pYearComboBox->GetCurSel();
	LONG lYear = pYearComboBox->GetItemData(iCurSel);
	iCurSel = pMonthComboBox->GetCurSel();
	LONG lMonth = pMonthComboBox->GetItemData(iCurSel);

	int iDayCount = 0;
	if (1 == lMonth || 3 == lMonth || 5 == lMonth || 7 == lMonth || 8 == lMonth || 10 == lMonth || 12 == lMonth)
		iDayCount = 31;
	else if (4 == lMonth || 6 == lMonth || 9 == lMonth || 11 == lMonth)
		iDayCount = 30;
	else if (2 == lMonth)
		iDayCount = (lYear % 4 == 0) ? 29 : 28;
	else
		iDayCount = 31;

	for (int iDay = 1; iDay <= iDayCount; iDay++)
	{
		CString strDay;
		strDay.Format(_T("%02d"), iDay);
		int iIndex = pDayComboBox->AddString(strDay);
		pDayComboBox->SetItemData(iIndex, iDay);
	}
	pDayComboBox->SetCurSel(0);
}
//----------------------------------------------------
void CAFUManagerDlg::FillToDayComboBox()
{
	CComboBox* pDayComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TODAY);
	pDayComboBox->ResetContent();
	int iIndex = pDayComboBox->AddString(_T("日"));
	pDayComboBox->SetItemData(iIndex, 0);
	//
	CComboBox* pYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOYEAR);
	CComboBox* pMonthComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TOMONTH);
	int iCurSel = pYearComboBox->GetCurSel();
	LONG lYear = pYearComboBox->GetItemData(iCurSel);
	iCurSel = pMonthComboBox->GetCurSel();
	LONG lMonth = pMonthComboBox->GetItemData(iCurSel);

	int iDayCount = 0;
	if (1 == lMonth || 3 == lMonth || 5 == lMonth || 7 == lMonth || 8 == lMonth || 10 == lMonth || 12 == lMonth)
		iDayCount = 31;
	else if (4 == lMonth || 6 == lMonth || 9 == lMonth || 11 == lMonth)
		iDayCount = 30;
	else if (2 == lMonth)
		iDayCount = (lYear % 4 == 0) ? 29 : 28;
	else
		iDayCount = 31;

	for (int iDay = 1; iDay <= iDayCount; iDay++)
	{
		CString strDay;
		strDay.Format(_T("%02d"), iDay);
		int iIndex = pDayComboBox->AddString(strDay);
		pDayComboBox->SetItemData(iIndex, iDay);
	}
	pDayComboBox->SetCurSel(0);
}
//----------------------------------------------------
void CAFUManagerDlg::InitTableStruct()
{
	// "会员管理表"
	m_Tables[eTableID_Member].eID = eTableID_Member;
	m_Tables[eTableID_Member].strName = _T("AFUMemberTable");
	m_Tables[eTableID_Member].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"序号", eTableDataType_DWORD));
	m_Tables[eTableID_Member].lstColumns.push_back(TableColumn(FALSE, 100, L"VIPID", L"会员卡号", eTableDataType_DWORD));
	m_Tables[eTableID_Member].lstColumns.push_back(TableColumn(FALSE, 100, L"Name", L"姓名", eTableDataType_String));
	m_Tables[eTableID_Member].lstColumns.push_back(TableColumn(FALSE, 50, L"Sex", L"性别", eTableDataType_String));
	m_Tables[eTableID_Member].lstColumns.push_back(TableColumn(FALSE, 100, L"Tel", L"电话", eTableDataType_String));
	m_Tables[eTableID_Member].lstColumns.push_back(TableColumn(FALSE, 100, L"Birth",L"生日", eTableDataType_Date));
	m_Tables[eTableID_Member].lstColumns.push_back(TableColumn(FALSE, 100, L"Points", L"总积分", eTableDataType_DWORD));
	m_Tables[eTableID_Member].lstColumns.push_back(TableColumn(FALSE, 150, L"Email", L"电子信箱", eTableDataType_String));
	m_Tables[eTableID_Member].lstColumns.push_back(TableColumn(FALSE, 200, L"Address", L"住址", eTableDataType_String));
	m_Tables[eTableID_Member].lstColumns.push_back(TableColumn(FALSE, 200, L"Comment", L"备注", eTableDataType_String));

	// "销售记录表"
	m_Tables[eTableID_Record].eID = eTableID_Record;
	m_Tables[eTableID_Record].strName = _T("AFUSaleTable");
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"序号", eTableDataType_DWORD));
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(FALSE, 80, L"VIPID", L"会员卡号", eTableDataType_DWORD));
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(FALSE, 80, L"Name", L"会员姓名", eTableDataType_String));
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(FALSE, 100, L"Date", L"日期", eTableDataType_Date));
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(FALSE, 100, L"Product", L"商品名称", eTableDataType_String));
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(FALSE, 100, L"SaleName", L"活动名称", eTableDataType_String));
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(FALSE, 80, L"Price", L"单价(元)", eTableDataType_FLOAT));
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(FALSE, 50, L"Count", L"数量", eTableDataType_DWORD));
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(FALSE, 80, L"Cost", L"总金额(元)", eTableDataType_FLOAT));
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(FALSE, 100, L"TotalPoints", L"新增积分", eTableDataType_DWORD));
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(FALSE, 100, L"Employee", L"操作员工", eTableDataType_String));
	m_Tables[eTableID_Record].lstColumns.push_back(TableColumn(FALSE, 250, L"Comment", L"备注", eTableDataType_String));

	// "员工管理表"
	m_Tables[eTableID_User].eID = eTableID_User;
	m_Tables[eTableID_User].strName = _T("AFUEmployeeTable");
	m_Tables[eTableID_User].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"序号", eTableDataType_DWORD));
	m_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 80, L"ID", L"工号", eTableDataType_DWORD));
	m_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 100, L"UserName", L"用户名", eTableDataType_String));
	m_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 100, L"Password", L"密码", eTableDataType_String, FALSE));
	m_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 100, L"Name", L"姓名", eTableDataType_String));
	m_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 100, L"Tel", L"联系电话", eTableDataType_String));
	m_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 150, L"Address", L"家庭住址", eTableDataType_String));
	m_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 80, L"Group", L"用户组", eTableDataType_DWORD));
	m_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 300, L"Comment", L"权限说明", eTableDataType_String));

	// "商品管理表"
	m_Tables[eTableID_Goods].eID = eTableID_Goods;
	m_Tables[eTableID_Goods].strName = _T("AFUGoodsTable");
	m_Tables[eTableID_Goods].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"序号", eTableDataType_DWORD));
	m_Tables[eTableID_Goods].lstColumns.push_back(TableColumn(FALSE, 100, L"ID", L"商品号", eTableDataType_String));
	m_Tables[eTableID_Goods].lstColumns.push_back(TableColumn(FALSE, 150, L"Name", L"商品名称", eTableDataType_String));
	m_Tables[eTableID_Goods].lstColumns.push_back(TableColumn(FALSE, 100, L"Price", L"价格(元)", eTableDataType_FLOAT));
	m_Tables[eTableID_Goods].lstColumns.push_back(TableColumn(FALSE, 100, L"Count", L"数量", eTableDataType_DWORD));
	m_Tables[eTableID_Goods].lstColumns.push_back(TableColumn(FALSE, 250, L"Comment", L"备注", eTableDataType_String));
}
//----------------------------------------------------
void CAFUManagerDlg::UpdateUIControlsStatus()
{
	// Update toolbar buttons status
	CToolBarCtrl& bar = m_wndToolBar.GetToolBarCtrl();
	switch(m_eLoginGroup)
	{
	case eEmployeeGroup_Admin:
		bar.EnableButton(ID_TBBUTTON_ADD, TRUE);
		bar.EnableButton(ID_TBBUTTON_EDIT, TRUE);
		bar.EnableButton(ID_TBBUTTON_DELETE, TRUE);
		bar.EnableButton(ID_TBBUTTON_SET, TRUE);
		break;
	case eEmployeeGroup_Normal:
		bar.EnableButton(ID_TBBUTTON_ADD, (eTableID_Member == m_eCurSelTable || eTableID_Record == m_eCurSelTable) ? TRUE : FALSE);
		bar.EnableButton(ID_TBBUTTON_EDIT, (eTableID_Member == m_eCurSelTable || eTableID_Record == m_eCurSelTable) ? TRUE : FALSE);
		bar.EnableButton(ID_TBBUTTON_DELETE, (eTableID_Member == m_eCurSelTable || eTableID_Record == m_eCurSelTable) ? TRUE : FALSE);
		bar.EnableButton(ID_TBBUTTON_SET, FALSE);
		break;
	case eEmployeeGroup_Viewer:
		bar.EnableButton(ID_TBBUTTON_ADD, FALSE);
		bar.EnableButton(ID_TBBUTTON_EDIT, FALSE);
		bar.EnableButton(ID_TBBUTTON_DELETE, FALSE);
		bar.EnableButton(ID_TBBUTTON_SET, FALSE);
		break;
	}

	// Update find set controls status
	if (eTableID_Member == m_eCurSelTable || eTableID_Record == m_eCurSelTable)
	{
		GetDlgItem(IDC_EDIT_VIPID)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VIPNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_BIRTHYEAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_BIRTHMONTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_BIRTHDAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TOYEAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TOMONTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TODAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_POINTS)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_VIPID))->SetFocus();
		//
		CComboBox* pYearComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BIRTHYEAR);
		pYearComboBox->ResetContent();
		int iIndex = pYearComboBox->AddString(_T("年"));
		pYearComboBox->SetItemData(iIndex, 0);
		if (eTableID_Member == m_eCurSelTable)
		{
			SetDlgItemText(IDC_STATIC_BIRTH, _T("会员生日:"));
			if (TRUE == m_bFindBtnIsPressed)
			{
				GetDlgItem(IDC_STATIC_TO)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_COMBO_TOYEAR)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_COMBO_TOMONTH)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_COMBO_TODAY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_POINTS)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_COMBO_POINTS)->ShowWindow(SW_SHOW);
			}
			//
			CTime tmNow;
			tmNow = CTime::GetCurrentTime();
			for (int iYear = 1950; iYear <= tmNow.GetYear(); iYear++)
			{
				CString strYear;
				strYear.Format(_T("%d"), iYear);
				int iIndex = pYearComboBox->AddString(strYear);
				pYearComboBox->SetItemData(iIndex, iYear);
			}
		}
		else
		{
			SetDlgItemText(IDC_STATIC_BIRTH, _T("销售日期:"));
			if (TRUE == m_bFindBtnIsPressed)
			{
				GetDlgItem(IDC_STATIC_TO)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_COMBO_TOYEAR)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_COMBO_TOMONTH)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_COMBO_TODAY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_POINTS)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_COMBO_POINTS)->ShowWindow(SW_HIDE);
			}
			//
			CTime tmNow;
			tmNow = CTime::GetCurrentTime();
			for (int iYear = 2011; iYear <= tmNow.GetYear(); iYear++)
			{
				CString strYear;
				strYear.Format(_T("%d"), iYear);
				int iIndex = pYearComboBox->AddString(strYear);
				pYearComboBox->SetItemData(iIndex, iYear);
			}
		}
		pYearComboBox->SetCurSel(0);
		FillBirthDayComboBox();
	}
	else
	{
		GetDlgItem(IDC_EDIT_VIPID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VIPNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_BIRTHYEAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_BIRTHMONTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_BIRTHDAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TOYEAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TOMONTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TODAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_POINTS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(FALSE);
	}

	// Update treeview control
	CTreeCtrl* pTreeCtrl = (CTreeCtrl*)GetDlgItem(IDC_TREE_LEFT);
	switch(m_eLoginGroup)
	{
	case eEmployeeGroup_Admin:
		{
			if (NULL == m_hAdminItem)
			{
				m_hAdminItem = pTreeCtrl->InsertItem(_T("系统管理"), 2, 2);
				m_hProductItem = pTreeCtrl->InsertItem(_T("商品管理"), 4, 4, m_hAdminItem);
				m_hEmployeeItem = pTreeCtrl->InsertItem(_T("员工管理"), 3, 3, m_hAdminItem);
				pTreeCtrl->SetItemData(m_hAdminItem, eTableID_None);
				pTreeCtrl->SetItemData(m_hProductItem, eTableID_Goods);
				pTreeCtrl->SetItemData(m_hEmployeeItem, eTableID_User);
			}
			else
			{
				if (NULL == m_hEmployeeItem)
				{
					m_hEmployeeItem = pTreeCtrl->InsertItem(_T("员工管理"), 3, 3, m_hAdminItem);
					pTreeCtrl->SetItemData(m_hEmployeeItem, eTableID_User);
				}
			}
		}
		break;
	case eEmployeeGroup_Normal:
		{
			if (NULL == m_hAdminItem)
			{
				m_hAdminItem = pTreeCtrl->InsertItem(_T("系统管理"), 2, 2);
				m_hProductItem = pTreeCtrl->InsertItem(_T("商品管理"), 4, 4, m_hAdminItem);
				pTreeCtrl->SetItemData(m_hAdminItem, eTableID_None);
				pTreeCtrl->SetItemData(m_hProductItem, eTableID_Goods);
			}
			else
			{
				if (NULL != m_hEmployeeItem)
				{
					pTreeCtrl->DeleteItem(m_hEmployeeItem);
					m_hEmployeeItem = NULL;
				}
			}
		}
		break;
	case eEmployeeGroup_Viewer:
		{
			if (NULL != m_hAdminItem)
			{
				pTreeCtrl->DeleteItem(m_hEmployeeItem);
				pTreeCtrl->DeleteItem(m_hProductItem);
				pTreeCtrl->DeleteItem(m_hAdminItem);
				m_hEmployeeItem = NULL;
				m_hProductItem = NULL;
				m_hAdminItem = NULL;
			}
		}
		break;
	}
}
//----------------------------------------------------
void CAFUManagerDlg::UpdateStatusBarInfo()
{
	CString strEmployeeInfo;
	switch(m_eLoginGroup)
	{
	case eEmployeeGroup_Admin:
		strEmployeeInfo.Format(_T("当前用户:%s  用户组:系统管理员"), m_strLoginUserName);
		break;
	case eEmployeeGroup_Normal:
		strEmployeeInfo.Format(_T("当前用户:%s  用户组:普通用户"), m_strLoginUserName);
		break;
	case eEmployeeGroup_Viewer:
		strEmployeeInfo.Format(_T("当前用户:%s  用户组:浏览用户"), m_strLoginUserName);
		break;
	}
	m_StatusBar.SetText(strEmployeeInfo, 0, 0);
	//
	CString strTodaySaleInfo;
	strTodaySaleInfo.Format(_T("今天销售情况:共 %d 条销售记录,销售金额共 %.2f 元"), m_lTodaySaledCnt, m_fTodaySaleCost);
	m_StatusBar.SetText(strTodaySaleInfo, 1, 0);
}
//----------------------------------------------------
void CAFUManagerDlg::ReadSettingFromRegistry()
{
}
//----------------------------------------------------
void CAFUManagerDlg::SaveSettingToRegistry()
{
}
//----------------------------------------------------
BOOL CAFUManagerDlg::OpenDatabase()
{
	TCHAR tcExePath[MAX_PATH];
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	m_strDatabaseFile = tcExePath;
	int iPos = m_strDatabaseFile.ReverseFind(_T('\\'));
	m_strDatabaseFile = m_strDatabaseFile.Left(iPos + 1);
	m_strDatabaseFile += _T("AFUDatabase.mdb");

	m_pDatabase = new CADODataBase();
	if (!m_pDatabase->OpenDatabase(m_strDatabaseFile))
	{
		delete m_pDatabase;
		m_pDatabase = NULL;
		MessageBox(_T("打开数据库文件错误，请确认数据库文件是否存在!"), _T("错误"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}
//----------------------------------------------------
BOOL CAFUManagerDlg::EmployeeLogin()
{
	CADOTable tmEmployee;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s", m_Tables[eTableID_User].strName);
	if (tmEmployee.OpenTable(m_pDatabase->GetConnectionPtr(), strOpenOperation))
	{
		CString strUserName;
		CString strPassword;
		CString strInputUserName;
		CString strInputPassword;
		BOOL bIsLogin = FALSE;
		DWORD dwCount = tmEmployee.GetRecordCnt();

		do
		{
			CAFULoginDlg dlg;
			if (IDOK != dlg.DoModal())
				return FALSE;
			
			BOOL bHasThisUser = FALSE;
			dlg.GetUserNameAndPassword(strInputUserName, strInputPassword);
			if (!strInputUserName.IsEmpty())
			{
				for (DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
				{
					tmEmployee.GetRecordCell(dwIndex, L"UserName", strUserName.GetBuffer(256), 256);
					tmEmployee.GetRecordCell(dwIndex, L"Password", strPassword.GetBuffer(256), 256);
					if (_tcsicmp(strUserName, strInputUserName) == 0)
					{
						bHasThisUser = TRUE;
						if (strInputPassword == DecodePasswordString(strPassword))
						{
							bIsLogin = TRUE;
							m_strLoginUserName = strUserName;
							tmEmployee.GetRecordCell(dwIndex, L"ID", m_dwLoginUserID);
							tmEmployee.GetRecordCell(dwIndex, L"Group", (DWORD&)m_eLoginGroup);
						}
						else
							MessageBox(_T("输入的用户名、密码不一致，请重新输入！"), _T("错误"), MB_OK | MB_ICONERROR);
						break;
					}
				}
			}
			if (FALSE == bHasThisUser)
			{
				MessageBox(_T("输入的用户名不存在，请重新输入！"), _T("错误"), MB_OK | MB_ICONERROR);
			}
		}while(!bIsLogin);

		tmEmployee.CloseTable();
	}

	return TRUE;
}
//----------------------------------------------------
void CAFUManagerDlg::GetTodaySaleInfo(LONG &lSaleRecordCnt, float &fSaleCost)
{
	lSaleRecordCnt = 0;
	fSaleCost = 0.0f;

	CADOTable tmSaleRecord;
	CString strOpenSQL;
	CString strDate;
	CTime tmNow = CTime::GetCurrentTime();
	strDate.Format(_T("%d-%d-%d"), tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay());
	strOpenSQL.Format(_T("SELECT * FROM %s WHERE DATE = #%s#"), m_Tables[eTableID_Record].strName, strDate);
	if (tmSaleRecord.OpenTable(m_pDatabase->GetConnectionPtr(), strOpenSQL))
	{
		lSaleRecordCnt = tmSaleRecord.GetRecordCnt();
		for (int iIndex = 0; iIndex < lSaleRecordCnt; iIndex++)
		{
			float fCost = 0.0f;
			tmSaleRecord.GetRecordCell(iIndex, L"Cost", fCost);
			fSaleCost += fCost;
		}
		tmSaleRecord.CloseTable();
	}
}
//----------------------------------------------------
void CAFUManagerDlg::OpenTableByID(eTableID eSelTable, CString strOperation)
{
	if (eTableID_None == eSelTable)
		return;

	CStringW strOpenOperation;
	if (strOperation.IsEmpty())
		strOpenOperation.Format(L"SELECT * FROM %s", m_Tables[eSelTable].strName);
	else
		strOpenOperation = strOperation;

	// Open table from database
	BOOL bResult = TRUE;
	m_TableObj.CloseTable();
	bResult = m_TableObj.OpenTable(m_pDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	// Show records in UI
	ShowCurrentTable(m_bIsMultiPage, m_lCountPerPage, m_lCurPage);
}
//----------------------------------------------------
void CAFUManagerDlg::ShowCurrentTable(BOOL bIsMultiPage, LONG lCountPerPage, LONG lCurPage)
{
	// Clear table list
	int iColumnCount = m_RecordListCtrl.GetHeaderCtrl()->GetItemCount();
	m_RecordListCtrl.DeleteAllItems();
	for (int i = 0;i < iColumnCount; i++)
	{
	   m_RecordListCtrl.DeleteColumn(0);
	}

	// Create new list based on selected table struct
	int iIndex = 0;
	for(TableColumnsList::iterator it = m_Tables[m_eCurSelTable].lstColumns.begin();
		it != m_Tables[m_eCurSelTable].lstColumns.end();
		it++, iIndex++)
	{
		TableColumn cl = *it;
		if (cl.bIsVisible)
		{
			m_RecordListCtrl.InsertColumn(iIndex, cl.strDisplayText, 0, cl.lColumnWidth);
		}
	}

	DWORD dwRecordCnt = m_TableObj.GetRecordCnt();

	DWORD dwStart = 0;
	DWORD dwEnd = dwRecordCnt;
	LONG lTotalPage = 1;
	GetDlgItem(IDC_SYSLINK_FIRSTPAGE)->EnableWindow(bIsMultiPage);
	GetDlgItem(IDC_SYSLINK_UPPAGE)->EnableWindow(bIsMultiPage);
	GetDlgItem(IDC_SYSLINK_DOWNPAGE)->EnableWindow(bIsMultiPage);
	GetDlgItem(IDC_SYSLINK_ENDPAGE)->EnableWindow(bIsMultiPage);
	if (TRUE == bIsMultiPage)
	{
		lTotalPage = (dwRecordCnt / lCountPerPage) + 1;
		if (lCurPage <= 0)
		{
			lCurPage = 0;
			GetDlgItem(IDC_SYSLINK_FIRSTPAGE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SYSLINK_UPPAGE)->EnableWindow(FALSE);
		}
		else if(lCurPage >= (lTotalPage-1))
		{
			lCurPage = lTotalPage-1;
			GetDlgItem(IDC_SYSLINK_DOWNPAGE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SYSLINK_ENDPAGE)->EnableWindow(FALSE);
		}
		//
		dwStart = lCurPage * lCountPerPage;
		dwEnd = min(dwStart + lCountPerPage, dwRecordCnt);
	}
	CString strPageInfo;
	strPageInfo.Format(_T("当前:第%d页 共%d页"), lCurPage+1, lTotalPage);
	SetDlgItemText(IDC_STATIC_PAGEINFO, strPageInfo);
	
	// Add table items to UI
	for (DWORD dwIndex = dwStart; dwIndex < dwEnd; dwIndex++)
	{
		DWORD dwColumn = 0;
		for(TableColumnsList::iterator it = m_Tables[m_eCurSelTable].lstColumns.begin();
			it != m_Tables[m_eCurSelTable].lstColumns.end();
			it++, dwColumn++)
		{
			TableColumn cl = *it;
			if (!cl.bIsVisible)
			{
				dwColumn--;
				continue;
			}

			CString strItemText;
			switch(cl.eDataType)
			{
			case eTableDataType_DWORD:
				{
					DWORD dwCellValue = 0;
					if (TRUE == cl.bIsIndex)
						dwCellValue = dwIndex + 1;
					else
						m_TableObj.GetRecordCell(dwIndex, cl.strName, dwCellValue);
					strItemText.Format(_T("%d"), dwCellValue);
				}
				break;
			case eTableDataType_FLOAT:
				{
					float fCellValue = 0;
					m_TableObj.GetRecordCell(dwIndex, cl.strName, fCellValue);
					strItemText.Format(_T("%.2f"), fCellValue);
				}
				break;
			case eTableDataType_Date:
				{
					CTime tm;
					m_TableObj.GetRecordCell(dwIndex, cl.strName, tm);
					strItemText.Format(_T("%02d/%02d/%d"), tm.GetMonth(), tm.GetDay(), tm.GetYear());
				}
				break;
			case eTableDataType_String:
			default:
				{
					WCHAR wcCellValue[256] = {0};
					m_TableObj.GetRecordCell(dwIndex, cl.strName, wcCellValue, 256);
					strItemText = wcCellValue;
				}
				break;
			}
			if (0 == dwColumn)
				m_RecordListCtrl.InsertItem(dwIndex-dwStart, strItemText);
			else
				m_RecordListCtrl.SetItemText(dwIndex-dwStart, dwColumn, strItemText);
		}
		m_RecordListCtrl.SetFullItemBKColor(dwIndex-dwStart, (dwIndex-dwStart)%2==0? RGB(180,180,90):RGB(90,180,180));
	}
}
//----------------------------------------------------
void CAFUManagerDlg::UpdateVIPMemberTable(BOOL bIsAddNewItem)
{
	CString strName(_T(""));
	CString strTel(_T(""));
	CString strEmail(_T(""));
	CString strAddress(_T(""));
	CString strOther(_T(""));
	CTime dateBirth = CTime();
	DWORD dwVIPID = 100000;
	BOOL bSexIsFemale = TRUE;

	int iSelRecordIndex = 0;
	if (FALSE == bIsAddNewItem)
	{		
		int iSelItem = 0;
		POSITION pos = m_RecordListCtrl.GetFirstSelectedItemPosition();
		if (NULL == pos)
		{
			MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}
		iSelItem = m_RecordListCtrl.GetNextSelectedItem(pos);
		if (iSelItem < 0)
		{
			MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}
		CString strRecordIndex = m_RecordListCtrl.GetItemText(iSelItem, 0);
		iSelRecordIndex = _ttol(strRecordIndex) - 1;

		WCHAR wcSex[32] = {0};
		m_TableObj.GetRecordCell(iSelRecordIndex, L"VIPID", dwVIPID);
		m_TableObj.GetRecordCell(-1, L"Name", strName.GetBuffer(256), 256);
		m_TableObj.GetRecordCell(-1, L"Sex", wcSex, 32);
		m_TableObj.GetRecordCell(-1, L"Tel", strTel.GetBuffer(64), 64);
		m_TableObj.GetRecordCell(-1, L"Birth", dateBirth);
		m_TableObj.GetRecordCell(-1, L"Email", strEmail.GetBuffer(128), 128);
		m_TableObj.GetRecordCell(-1, L"Address", strAddress.GetBuffer(128), 128);
		m_TableObj.GetRecordCell(-1, L"Comment", strOther.GetBuffer(256), 256);
		bSexIsFemale = (wcscmp(wcSex, STR_FEMALE) == 0) ? TRUE : FALSE;
	}

	CAFUMemberDlg dlg;
	dlg.SetInitializeData(strName, dwVIPID, bSexIsFemale, strTel, dateBirth, strEmail, strAddress, strOther);
	if (IDOK == dlg.DoModal())
	{
		dlg.GetUpdatedData(strName, dwVIPID, bSexIsFemale, strTel, dateBirth, strEmail, strAddress, strOther);

		if (TRUE == bIsAddNewItem)
		{
			m_TableObj.AddNewItem();
			m_TableObj.SetRecordCell(-1, L"VIPID", dwVIPID);
		}
		else
		{
			m_TableObj.SetRecordCell(iSelRecordIndex, L"VIPID", dwVIPID);
		}
		m_TableObj.SetRecordCell(-1, L"Name", strName.GetBuffer(strName.GetLength()));
		m_TableObj.SetRecordCell(-1, L"Sex", bSexIsFemale ? STR_FEMALE : STR_MALE);
		m_TableObj.SetRecordCell(-1, L"Tel", strTel.GetBuffer(strTel.GetLength()));
		m_TableObj.SetRecordCell(-1, L"Birth", dateBirth);
		m_TableObj.SetRecordCell(-1, L"Email", strEmail.GetBuffer(strEmail.GetLength()));
		m_TableObj.SetRecordCell(-1, L"Address", strAddress.GetBuffer(strAddress.GetLength()));
		m_TableObj.SetRecordCell(-1, L"Comment", strOther.GetBuffer(strOther.GetLength()));
		if (TRUE == m_TableObj.UpdateTable())
		{
			OpenTableByID(eTableID_Member);
			if (TRUE == bIsAddNewItem) MessageBox(_T("数据添加成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
			else  MessageBox(_T("数据修改成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			if (TRUE == bIsAddNewItem) MessageBox(_T("数据添加失败!"), _T("失败"), MB_OK | MB_ICONERROR);
			else  MessageBox(_T("数据修改失败!"), _T("失败"), MB_OK | MB_ICONERROR);
		}
	}
}
//----------------------------------------------------
void CAFUManagerDlg::UpdateSaleRecordTable(BOOL bIsAddNewItem)
{
	DWORD dwVIPID = 0;
	CString strVIPName = _T("");
	CString strSaleName = _T("");
	CString strGoodsName = _T("");
	CString	strEmployee = _T("");
	CString strOther = _T("");
	CTime tmDate = CTime::GetCurrentTime();
	float fPrice = 0.0f;
	LONG lCount = 1;
	float fTotalCost = 0.0f;
	DWORD dwPoints = 0;

	int iSelRecordIndex = 0;
	if (FALSE == bIsAddNewItem)
	{		
		int iSelItem = 0;
		POSITION pos = m_RecordListCtrl.GetFirstSelectedItemPosition();
		if (NULL == pos)
		{
			MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}
		iSelItem = m_RecordListCtrl.GetNextSelectedItem(pos);
		if (iSelItem < 0)
		{
			MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}
		CString strRecordIndex = m_RecordListCtrl.GetItemText(iSelItem, 0);
		iSelRecordIndex = _ttol(strRecordIndex) - 1;

		WCHAR wcSex[32] = {0};
		m_TableObj.GetRecordCell(iSelRecordIndex, L"VIPID", dwVIPID);
		m_TableObj.GetRecordCell(-1, L"Name", strVIPName.GetBuffer(256), 256);
		m_TableObj.GetRecordCell(-1, L"Date", tmDate);		
		m_TableObj.GetRecordCell(-1, L"Product", strGoodsName.GetBuffer(256), 256);
		m_TableObj.GetRecordCell(-1, L"SaleName", strSaleName.GetBuffer(256), 256);
		m_TableObj.GetRecordCell(-1, L"Price", fPrice);
		m_TableObj.GetRecordCell(-1, L"Count", (DWORD&)lCount);
		m_TableObj.GetRecordCell(-1, L"Cost", fTotalCost);
		m_TableObj.GetRecordCell(-1, L"TotalPoints", dwPoints);
		m_TableObj.GetRecordCell(-1, L"Employee", strEmployee.GetBuffer(128), 128);
		m_TableObj.GetRecordCell(-1, L"Comment", strOther.GetBuffer(256), 256);
	}
	LONG lOldCount = lCount;
	float fOldCost = fTotalCost;
	DWORD dwOldPoints = dwPoints;

	CAFUSaleRecordDlg dlg;
	dlg.SetDatabaseAndTableName(m_pDatabase, m_Tables[eTableID_Member].strName, m_Tables[eTableID_Goods].strName, m_Tables[eTableID_User].strName);
	dlg.SetSaleRecordInfo(dwVIPID, tmDate, strSaleName, strGoodsName, fPrice, lCount, fTotalCost, dwPoints, strEmployee, strOther);
	if (IDOK == dlg.DoModal())
	{
		dlg.GetSaleRecordInfo(dwVIPID, strVIPName, tmDate, strSaleName, strGoodsName, fPrice, lCount, fTotalCost, (LONG&)dwPoints, strEmployee, strOther);
		if (TRUE == bIsAddNewItem)
		{
			m_lTodaySaledCnt++;
			m_TableObj.AddNewItem();
			m_TableObj.SetRecordCell(-1, L"VIPID", dwVIPID);
		}
		else
		{
			m_TableObj.SetRecordCell(iSelRecordIndex, L"VIPID", dwVIPID);
		}
		m_TableObj.SetRecordCell(-1, L"Name", strVIPName.GetBuffer(strVIPName.GetLength()));
		m_TableObj.SetRecordCell(-1, L"Date", tmDate);		
		m_TableObj.SetRecordCell(-1, L"Product", strGoodsName.GetBuffer(strGoodsName.GetLength()));
		m_TableObj.SetRecordCell(-1, L"SaleName", strSaleName.GetBuffer(strSaleName.GetLength()));
		m_TableObj.SetRecordCell(-1, L"Price", fPrice);
		m_TableObj.SetRecordCell(-1, L"Count", (DWORD)lCount);
		m_TableObj.SetRecordCell(-1, L"Cost", fTotalCost);
		m_TableObj.SetRecordCell(-1, L"TotalPoints", dwPoints);
		m_TableObj.SetRecordCell(-1, L"Employee", strEmployee.GetBuffer(strEmployee.GetLength()));
		m_TableObj.SetRecordCell(-1, L"Comment", strOther.GetBuffer(strOther.GetLength()));
		if (TRUE == m_TableObj.UpdateTable())
		{
			OpenTableByID(eTableID_Record);

			//Update status bar
			m_fTodaySaleCost += (fTotalCost - fOldCost);
			UpdateStatusBarInfo();

			//Update points in VIP member table
			UpdatePointsForVIPMember(dwVIPID, dwPoints-dwOldPoints);

			//Upadte product new counts in Goods table
			LONG lSaledCnt = (lCount-lOldCount > 0) ? lCount-lOldCount : 0;
			UpdateProductCountsForGoodsTable(strGoodsName, lSaledCnt);

			if (TRUE == bIsAddNewItem) MessageBox(_T("数据添加成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
			else  MessageBox(_T("数据修改成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			if (TRUE == bIsAddNewItem) MessageBox(_T("数据添加失败!"), _T("失败"), MB_OK | MB_ICONERROR);
			else  MessageBox(_T("数据修改失败!"), _T("失败"), MB_OK | MB_ICONERROR);
		}
	}
}
//----------------------------------------------------
void CAFUManagerDlg::UpdateAFUEmployeeTable(BOOL bIsAddNewItem)
{
	DWORD dwEmployeeID = 0;
	CString strUserName = _T("");
	CString strName = _T("");
	CString strTel = _T("");
	CString strAddress = _T("");
	eEmployeeGroup eGroup = eEmployeeGroup_Normal;

	int iSelRecordIndex = 0;
	if (FALSE == bIsAddNewItem)
	{		
		int iSelItem = 0;
		POSITION pos = m_RecordListCtrl.GetFirstSelectedItemPosition();
		if (NULL == pos)
		{
			MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}
		iSelItem = m_RecordListCtrl.GetNextSelectedItem(pos);
		if (iSelItem < 0)
		{
			MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}
		CString strRecordIndex = m_RecordListCtrl.GetItemText(iSelItem, 0);
		iSelRecordIndex = _ttol(strRecordIndex) - 1;

		m_TableObj.GetRecordCell(iSelRecordIndex, L"ID", dwEmployeeID);
		m_TableObj.GetRecordCell(-1, L"UserName", strUserName.GetBuffer(256), 256);
		m_TableObj.GetRecordCell(-1, L"Name", strName.GetBuffer(256), 256);
		m_TableObj.GetRecordCell(-1, L"Tel", strTel.GetBuffer(256), 256);	
		m_TableObj.GetRecordCell(-1, L"Address", strAddress.GetBuffer(256), 256);
		m_TableObj.GetRecordCell(-1, L"Group", (DWORD&)eGroup);
	}

	CAFUEmployeeDlg dlg;
	dlg.SetInitializeData(dwEmployeeID, strUserName, strName, eGroup, strTel, strAddress);
	if (IDOK == dlg.DoModal())
	{
		dlg.GetUpdatedData(dwEmployeeID, strUserName, strName, eGroup, strTel, strAddress);				
		if (TRUE == bIsAddNewItem)
		{
			m_TableObj.AddNewItem();
			m_TableObj.SetRecordCell(-1, L"ID", dwEmployeeID);
			m_TableObj.SetRecordCell(-1, L"Comment", GetEmployeeGroupInfo(eGroup));
			m_TableObj.SetRecordCell(-1, L"Password", EncodePasswordString(L"123456"));
		}
		else
		{
			m_TableObj.SetRecordCell(iSelRecordIndex, L"ID", dwEmployeeID);
		}
		m_TableObj.SetRecordCell(-1, L"UserName", strUserName.GetBuffer(strUserName.GetLength()));
		m_TableObj.SetRecordCell(-1, L"Name", strName.GetBuffer(strName.GetLength()));
		m_TableObj.SetRecordCell(-1, L"Tel", strTel.GetBuffer(strTel.GetLength()));	
		m_TableObj.SetRecordCell(-1, L"Address", strAddress.GetBuffer(strAddress.GetLength()));
		m_TableObj.SetRecordCell(-1, L"Group", (DWORD&)eGroup);
		m_TableObj.SetRecordCell(-1, L"Comment", GetEmployeeGroupInfo(eGroup));
		if (TRUE == m_TableObj.UpdateTable())
		{
			OpenTableByID(eTableID_User);
			if (TRUE == bIsAddNewItem) MessageBox(_T("数据添加成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
			else  MessageBox(_T("数据修改成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			if (TRUE == bIsAddNewItem) MessageBox(_T("数据添加失败!"), _T("失败"), MB_OK | MB_ICONERROR);
			else  MessageBox(_T("数据修改失败!"), _T("失败"), MB_OK | MB_ICONERROR);
		}
	}
}
//----------------------------------------------------
void CAFUManagerDlg::UpdateAFUGoodsTable(BOOL bIsAddNewItem)
{
	CString strProductID = _T("");
	CString strProductName = _T("");
	CString strOtherInfo = _T("");
	float fPrice = 0.0f;
	DWORD dwCount = 0;

	int iSelRecordIndex = 0;
	if (FALSE == bIsAddNewItem)
	{		
		int iSelItem = 0;
		POSITION pos = m_RecordListCtrl.GetFirstSelectedItemPosition();
		if (NULL == pos)
		{
			MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}
		iSelItem = m_RecordListCtrl.GetNextSelectedItem(pos);
		if (iSelItem < 0)
		{
			MessageBox(_T("请先选择需要修改的纪录!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}
		CString strRecordIndex = m_RecordListCtrl.GetItemText(iSelItem, 0);
		iSelRecordIndex = _ttol(strRecordIndex) - 1;

		m_TableObj.GetRecordCell(iSelRecordIndex, L"ID", strProductID.GetBuffer(256), 256);
		m_TableObj.GetRecordCell(-1, L"Name", strProductName.GetBuffer(256), 256);
		m_TableObj.GetRecordCell(-1, L"Price", fPrice);	
		m_TableObj.GetRecordCell(-1, L"Count", dwCount);
		m_TableObj.GetRecordCell(-1, L"Comment", strOtherInfo.GetBuffer(256), 256);
	}

	CAFUProductDlg dlg;
	dlg.SetInitializeData(strProductID, strProductName, fPrice, dwCount, strOtherInfo);
	if (IDOK == dlg.DoModal())
	{
		dlg.GetUpdatedData(strProductID, strProductName, fPrice, (long&)dwCount, strOtherInfo);				
		if (TRUE == bIsAddNewItem)
		{
			m_TableObj.AddNewItem();
			m_TableObj.SetRecordCell(-1, L"ID", strProductID.GetBuffer(strProductID.GetLength()));
		}
		else
		{
			m_TableObj.SetRecordCell(iSelRecordIndex, L"ID", strProductID.GetBuffer(strProductID.GetLength()));
		}
		m_TableObj.SetRecordCell(-1, L"Name", strProductName.GetBuffer(strProductName.GetLength()));
		m_TableObj.SetRecordCell(-1, L"Price", fPrice);	
		m_TableObj.SetRecordCell(-1, L"Count", dwCount);
		m_TableObj.SetRecordCell(-1, L"Comment", strOtherInfo.GetBuffer(strOtherInfo.GetLength()));
		if (TRUE == m_TableObj.UpdateTable())
		{
			OpenTableByID(eTableID_Goods);
			if (TRUE == bIsAddNewItem) MessageBox(_T("数据添加成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
			else  MessageBox(_T("数据修改成功!"), _T("成功"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			if (TRUE == bIsAddNewItem) MessageBox(_T("数据添加失败!"), _T("失败"), MB_OK | MB_ICONERROR);
			else  MessageBox(_T("数据修改失败!"), _T("失败"), MB_OK | MB_ICONERROR);
		}
	}
}
//----------------------------------------------------
BOOL CAFUManagerDlg::UpdatePointsForVIPMember(DWORD dwVIPID, LONG lChangedPoints)
{
	CADOTable tmVIPMember;
	CString strOpenSQL;
	strOpenSQL.Format(_T("SELECT * FROM %s WHERE VIPID = %d"), m_Tables[eTableID_Member].strName, dwVIPID);
	if (tmVIPMember.OpenTable(m_pDatabase->GetConnectionPtr(), strOpenSQL))
	{
		if (1 == tmVIPMember.GetRecordCnt())
		{
			DWORD dwCurPoints;
			tmVIPMember.GetRecordCell(0, L"Points", dwCurPoints);
			//
			LONG lNewPoints = (dwCurPoints + lChangedPoints);
			if (lNewPoints < 0) lNewPoints = 0;
			dwCurPoints = (DWORD)lNewPoints;
			tmVIPMember.SetRecordCell(0, L"Points", dwCurPoints);
			if (TRUE == tmVIPMember.UpdateTable())
				return TRUE;
		}
	}

	return FALSE;
}
//----------------------------------------------------
BOOL CAFUManagerDlg::UpdateProductCountsForGoodsTable(CString strProductName, 
													  LONG lSaledCnt)
{
	CADOTable tmProducts;
	CString strOpenSQL;
	strOpenSQL.Format(_T("SELECT * FROM %s WHERE NAME = '%s'"), m_Tables[eTableID_Goods].strName, strProductName);
	if (tmProducts.OpenTable(m_pDatabase->GetConnectionPtr(), strOpenSQL))
	{
		if (1 == tmProducts.GetRecordCnt())
		{
			DWORD dwCurCount;
			tmProducts.GetRecordCell(0, L"Count", dwCurCount);
			//
			LONG lNewCount = (dwCurCount - lSaledCnt);
			if (lNewCount < 0) lNewCount = 0;
			dwCurCount = (DWORD)lNewCount;
			tmProducts.SetRecordCell(0, L"Count", dwCurCount);
			if (TRUE == tmProducts.UpdateTable())
				return TRUE;
		}
	}

	return FALSE;
}
//----------------------------------------------------
LPWSTR CAFUManagerDlg::GetEmployeeGroupInfo(eEmployeeGroup eGroup)
{
	static WCHAR wcGroupInfo[512] = {0};
	memset(wcGroupInfo, 0, sizeof(WCHAR) * 512);
	switch(eGroup)
	{
	case eEmployeeGroup_Admin:
		wcscpy_s(wcGroupInfo, 512, STR_GROUP_ADMIN);
		break;
	case eEmployeeGroup_Normal:
		wcscpy_s(wcGroupInfo, 512, STR_GROUP_NORMAL);
		break;
	case eEmployeeGroup_Viewer:
		wcscpy_s(wcGroupInfo, 512, STR_GROUP_VIEWER);
		break;
	}

	return wcGroupInfo;
}
//----------------------------------------------------
LPWSTR CAFUManagerDlg::EncodePasswordString(LPCWSTR lpwzPassword)
{
	static WCHAR wcEncodedString[32] = {0};
	memset(wcEncodedString, 0, sizeof(WCHAR) * 32);
	if (NULL != lpwzPassword && wcslen(lpwzPassword) > 0)
	{
		int iIndex = 0;
		LPCWSTR p = lpwzPassword;
		while(*p != L'\0')
		{
			wcEncodedString[iIndex++] = lpwzPassword[iIndex] - L'C';
			p++;
		}
	}

	return wcEncodedString;
}
//----------------------------------------------------
LPWSTR CAFUManagerDlg::DecodePasswordString(LPCWSTR lpwzEncodedPassword)
{
	static WCHAR wcDecodedString[32] = {0};
	memset(wcDecodedString, 0, sizeof(WCHAR) * 32);
	if (NULL != lpwzEncodedPassword && wcslen(lpwzEncodedPassword) > 0)
	{
		int iIndex = 0;
		LPCWSTR p = lpwzEncodedPassword;
		while(*p != L'\0')
		{
			wcDecodedString[iIndex++] = lpwzEncodedPassword[iIndex] + L'C';
			p++;
		}
	}

	return wcDecodedString;
}
//----------------------------------------------------