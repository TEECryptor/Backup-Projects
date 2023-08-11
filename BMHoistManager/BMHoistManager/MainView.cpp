// MainView.cpp : implementation file
//

#include "stdafx.h"
#include "BMHoistManager.h"
#include "MainView.h"
#include "CustomInfoDlg.h"
#include "OfferInfoDlg.h"
#include "ContractInfoDlg.h"
#include "SaleCountDlg.h"
#include ".\Database\ExcelExporter.h"


// CMainView

IMPLEMENT_DYNCREATE(CMainView, CFormView)

CMainView::CMainView()
	: CFormView(CMainView::IDD)
{
	m_eCurTableID = eTableID_None;
	m_bShowFindOption = FALSE;
	m_bIsMultiPage = TRUE;
	m_lCountPerPage = 100;
	m_lTotalPage = 0;
	m_lCurPage = 0;
}

CMainView::~CMainView()
{
}

void CMainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAINVIEW_MAIN, m_MainListCtrl);
	DDX_Control(pDX, IDC_STATIC_MAINVIEW_BOTTOM, m_StaticBottom);
	DDX_Control(pDX, IDC_STATIC_MAINVIEW_PAGENUM, m_StaticPageNum);
	DDX_Control(pDX, IDC_SYSLINK_MAINVIEW_FIRST, m_LinkCtrlFirst);
	DDX_Control(pDX, IDC_SYSLINK_MAINVIEW_UP, m_LinkCtrlUp);
	DDX_Control(pDX, IDC_SYSLINK_MAINVIEW_DWON, m_LinkCtrlDown);
	DDX_Control(pDX, IDC_SYSLINK_MAINVIEW_LAST, m_LinkCtrlLast);
	DDX_Control(pDX, IDC_COMBO_MAINVIEW_FINDOPTION, m_FindOptionCombox);
	DDX_Control(pDX, IDC_STATIC_MAINVIEW_FIND, m_FindOptionStatic);
}

BEGIN_MESSAGE_MAP(CMainView, CFormView)
	ON_COMMAND(ID_TOOLBTN_FIND, OnToolBarFind)
	ON_COMMAND(ID_TOOLBTN_NEW, OnToolBarNew)
	ON_COMMAND(ID_TOOLBTN_UPDATE, OnToolBarUpdate)
	ON_COMMAND(ID_TOOLBTN_DELETE, OnToolBarDelete)
	ON_COMMAND(ID_TOOLBTN_EXPORT, OnToolBarExport)
	ON_COMMAND(ID_TOOLBTN_COUNT, OnToolBarCount)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_MAINVIEW_FIND, &CMainView::OnBnClickedBtnMainviewFind)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MAINVIEW_MAIN, &CMainView::OnNMDblclkListMainviewMain)
END_MESSAGE_MAP()


// CMainView diagnostics

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainView message handlers

void CMainView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

}

void CMainView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();		
	CFont *pDlgFont = GetFont();

	CreateToolBar();
	InitUIControls();
	InitTableStruct();
	ShowFindOptions(FALSE);
	
	m_MainListCtrl.SetExtendedStyle(m_MainListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

}

void CMainView::OnNMDblclkListMainviewMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	OnToolBarUpdate();

	*pResult = 0;
}

void CMainView::CreateToolBar()
{
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT|TBSTYLE_LIST, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Can't create toolbar\n");
		return;
	}
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

	CToolBarCtrl& bar = m_wndToolBar.GetToolBarCtrl();

	VERIFY(m_wndToolBar.SetButtonText(0, _T("����")));
	VERIFY(m_wndToolBar.SetButtonText(2, _T("�½�")));
	VERIFY(m_wndToolBar.SetButtonText(3, _T("�޸�")));
	VERIFY(m_wndToolBar.SetButtonText(4, _T("ɾ��")));
	VERIFY(m_wndToolBar.SetButtonText(6, _T("����")));
	VERIFY(m_wndToolBar.SetButtonText(7, _T("ͳ��")));

	CBitmap bp;
	BOOL bRes = bp.LoadBitmap(IDB_BITMAP_TOOLBAR_ENABLE);
	bRes = m_ToolBarEnableImages.Create(32, 32, TRUE | ILC_COLOR24, 6, 0 );
	int iRes = m_ToolBarEnableImages.Add(&bp, RGB(0,0,0));
	bar.SetImageList(&m_ToolBarEnableImages);
	bp.Detach();
	//
	bRes = bp.LoadBitmap(IDB_BITMAP_TOOLBAR_DISABLE);
	bRes = m_ToolBarDisableImages.Create(32, 32, TRUE | ILC_COLOR24, 6, 0 );
	iRes = m_ToolBarDisableImages.Add(&bp, RGB(0,0,0));
	bar.SetDisabledImageList(&m_ToolBarDisableImages);
	bp.Detach();
	//
	//bar.EnableButton(ID_TOOLBTN_FIND, TRUE);
	//bar.EnableButton(ID_TOOLBTN_NEW, TRUE);
	//bar.EnableButton(ID_TOOLBTN_UPDATE, TRUE);
	//bar.EnableButton(ID_TOOLBTN_DELETE, TRUE);
}

void CMainView::InitUIControls()
{	
	CRect rcClient;
	GetWindowRect(&rcClient);

	CRect rcBottomStatic;
	rcBottomStatic.left = 0;
	rcBottomStatic.right = rcClient.Width();
	rcBottomStatic.top = rcClient.Height() - 48;
	rcBottomStatic.bottom = rcClient.Height();
	m_StaticBottom.MoveWindow(rcBottomStatic);

	CRect rcToolBar;
	m_wndToolBar.GetWindowRect(&rcToolBar);
	ScreenToClient(&rcToolBar);

	CRect rcFindOption = SetFindOptionsCtrlPos(rcToolBar.bottom);

	CRect rcMainGird;
	rcMainGird.left = 0;
	rcMainGird.right = rcClient.Width();
	rcMainGird.top = m_bShowFindOption ? (rcFindOption.bottom + 5) : (rcToolBar.bottom + 5);
	rcMainGird.bottom = rcClient.Height() - rcBottomStatic.Height() - 5;
	m_MainListCtrl.MoveWindow(rcMainGird);

	CRect rcPageNum;
	m_StaticPageNum.GetWindowRect(&rcPageNum);
	int iWidth = rcPageNum.Width();
	int iHeight = rcPageNum.Height();
	rcPageNum.left = 10;
	rcPageNum.right = rcPageNum.left + iWidth;
	rcPageNum.top = rcBottomStatic.top + (rcBottomStatic.Height() - iHeight) / 2;
	rcPageNum.bottom = rcPageNum.top + iHeight;
	m_StaticPageNum.MoveWindow(rcPageNum);

	CRect rcFirstPageLink;
	m_LinkCtrlFirst.GetWindowRect(&rcFirstPageLink);
	iWidth = rcFirstPageLink.Width();
	iHeight = rcFirstPageLink.Height();
	rcFirstPageLink.left = (rcBottomStatic.Width() - 4*iWidth - 3*10) / 2;
	rcFirstPageLink.right = rcFirstPageLink.left + iWidth;
	rcFirstPageLink.top = rcBottomStatic.top + (rcBottomStatic.Height() - iHeight) / 2;
	rcFirstPageLink.bottom = rcFirstPageLink.top + iHeight;
	m_LinkCtrlFirst.MoveWindow(rcFirstPageLink);
	
	CRect rcUpPageLink;
	m_LinkCtrlUp.GetWindowRect(&rcUpPageLink);
	iWidth = rcUpPageLink.Width();
	iHeight = rcUpPageLink.Height();
	rcUpPageLink.left = rcFirstPageLink.right + 10;
	rcUpPageLink.right = rcUpPageLink.left + iWidth;
	rcUpPageLink.top = rcBottomStatic.top + (rcBottomStatic.Height() - iHeight) / 2;
	rcUpPageLink.bottom = rcUpPageLink.top + iHeight;
	m_LinkCtrlUp.MoveWindow(rcUpPageLink);
	
	CRect rcDownPageLink;
	m_LinkCtrlDown.GetWindowRect(&rcDownPageLink);
	iWidth = rcDownPageLink.Width();
	iHeight = rcDownPageLink.Height();
	rcDownPageLink.left = rcUpPageLink.right + 10;
	rcDownPageLink.right = rcDownPageLink.left + iWidth;
	rcDownPageLink.top = rcBottomStatic.top + (rcBottomStatic.Height() - iHeight) / 2;
	rcDownPageLink.bottom = rcDownPageLink.top + iHeight;
	m_LinkCtrlDown.MoveWindow(rcDownPageLink);

	CRect rcLastPageLink;
	m_LinkCtrlLast.GetWindowRect(&rcLastPageLink);
	iWidth = rcLastPageLink.Width();
	iHeight = rcLastPageLink.Height();
	rcLastPageLink.left = rcDownPageLink.right + 10;
	rcLastPageLink.right = rcLastPageLink.left + iWidth;
	rcLastPageLink.top = rcBottomStatic.top + (rcBottomStatic.Height() - iHeight) / 2;
	rcLastPageLink.bottom = rcLastPageLink.top + iHeight;
	m_LinkCtrlLast.MoveWindow(rcLastPageLink);

	InvalidateRect(&rcBottomStatic);
}

CRect CMainView::SetFindOptionsCtrlPos(int iTop)
{
	CRect rcCtrl;
	CRect rcClient;
	CRect rcFindOption;
	
	GetClientRect(&rcClient);

	m_FindOptionStatic.GetWindowRect(&rcFindOption);
	int iHeight = rcFindOption.Height();
	ScreenToClient(&rcFindOption);
	rcFindOption.top = iTop + 5;
	rcFindOption.bottom = rcFindOption.top + iHeight;
	rcFindOption.right = rcClient.Width() - 10;
	m_FindOptionStatic.MoveWindow(rcFindOption);
	//
	CWnd* pWnd = GetDlgItem(IDC_STATIC_MAINVIEW_FINDOPTION);
	pWnd->GetWindowRect(&rcCtrl);
	ScreenToClient(&rcCtrl);
	iHeight = rcCtrl.Height();
	rcCtrl.top = rcFindOption.top + (rcFindOption.Height()-rcCtrl.Height())/2;
	rcCtrl.bottom = rcCtrl.top + iHeight;
	pWnd->MoveWindow(rcCtrl);
	//
	pWnd = GetDlgItem(IDC_COMBO_MAINVIEW_FINDOPTION);
	pWnd->GetWindowRect(&rcCtrl);
	ScreenToClient(&rcCtrl);
	iHeight = rcCtrl.Height();
	rcCtrl.top = rcFindOption.top + (rcFindOption.Height()-rcCtrl.Height())/2;
	rcCtrl.bottom = rcCtrl.top + iHeight;
	pWnd->MoveWindow(rcCtrl);
	//
	pWnd = GetDlgItem(IDC_STATIC_MAINVIEW_FINDVALUE);
	pWnd->GetWindowRect(&rcCtrl);
	ScreenToClient(&rcCtrl);
	iHeight = rcCtrl.Height();
	rcCtrl.top = rcFindOption.top + (rcFindOption.Height()-rcCtrl.Height())/2;
	rcCtrl.bottom = rcCtrl.top + iHeight;
	pWnd->MoveWindow(rcCtrl);
	//
	pWnd = GetDlgItem(IDC_EDIT_MAINVIEW_FINDVALUE);
	pWnd->GetWindowRect(&rcCtrl);
	ScreenToClient(&rcCtrl);
	iHeight = rcCtrl.Height();
	rcCtrl.top = rcFindOption.top + (rcFindOption.Height()-rcCtrl.Height())/2;
	rcCtrl.bottom = rcCtrl.top + iHeight;
	pWnd->MoveWindow(rcCtrl);
	//
	pWnd = GetDlgItem(IDC_BTN_MAINVIEW_FIND);
	pWnd->GetWindowRect(&rcCtrl);
	ScreenToClient(&rcCtrl);
	iHeight = rcCtrl.Height();
	rcCtrl.top = rcFindOption.top + (rcFindOption.Height()-rcCtrl.Height())/2;
	rcCtrl.bottom = rcCtrl.top + iHeight;
	pWnd->MoveWindow(rcCtrl);

	return rcFindOption;	
}

void CMainView::OnToolBarFind()
{
	CRect rcToolBar;
	CToolBarCtrl& bar = m_wndToolBar.GetToolBarCtrl();
	m_wndToolBar.GetWindowRect(&rcToolBar);
	ScreenToClient(&rcToolBar);

	CRect rcMainGrid;
	m_MainListCtrl.GetWindowRect(&rcMainGrid);
	ScreenToClient(&rcMainGrid);

	if (m_bShowFindOption)
	{
		rcMainGrid.top = rcToolBar.bottom + 5;
		//	
		ShowFindOptions(FALSE);
		m_bShowFindOption = FALSE;
		bar.PressButton(ID_TOOLBTN_FIND, FALSE);
	}
	else
	{
		CRect rcFindOption = SetFindOptionsCtrlPos(rcToolBar.bottom);
		rcMainGrid.top = rcFindOption.bottom + 5;
		//		
		ShowFindOptions(TRUE);
		m_bShowFindOption = TRUE;
		bar.PressButton(ID_TOOLBTN_FIND, TRUE);
	}
	m_MainListCtrl.MoveWindow(rcMainGrid);
}

void CMainView::OnToolBarNew()
{
	BOOL bNeedUpdateTable = FALSE;
	switch(m_eCurTableID)
	{
	case eTableID_Custom:
		{
			CCustomInfoDlg dlg;
			dlg.m_eTableOption = eTableOptions_New;
			dlg.m_pCurTable = &m_TableObj;
			bNeedUpdateTable = (dlg.DoModal() == IDOK) ? TRUE : FALSE;
			break;
		}
	case eTableID_Offer:
		{
			COfferInfoDlg dlg;
			dlg.m_eTableOption = eTableOptions_New;
			dlg.m_pCurTable = &m_TableObj;
			bNeedUpdateTable = (dlg.DoModal() == IDOK) ? TRUE : FALSE;
			break;
		}
	case eTableID_Contract:
		{
			CContractInfoDlg dlg;
			dlg.m_eTableOption = eTableOptions_New;
			dlg.m_pCurTable = &m_TableObj;
			bNeedUpdateTable = (dlg.DoModal() == IDOK) ? TRUE : FALSE;
			break;
		}
	case eTableID_None:
	default:
		MessageBox(_T("�˹�����δʵ��!"), _T("����"), MB_OK | MB_ICONERROR);
		break;
	}
	if (bNeedUpdateTable)
	{
		OpenTable(m_eCurTableID, _T(""));
	}
}
	
void CMainView::OnToolBarUpdate()
{
	BOOL bNeedUpdateTable = FALSE;
	POSITION pos = m_MainListCtrl.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("����ѡ����Ҫ�޸ĵļ�¼!"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}
	int iSelItem = m_MainListCtrl.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		MessageBox(_T("����ѡ����Ҫ�޸ĵļ�¼!"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}

	switch(m_eCurTableID)
	{
	case eTableID_Custom:
		{
			CCustomInfoDlg dlg;
			dlg.m_eTableOption = eTableOptions_Modify;
			dlg.m_pCurTable = &m_TableObj;
			dlg.m_iSelIndex = iSelItem;
			dlg.m_bIsModifyContact = FALSE;
			bNeedUpdateTable = (dlg.DoModal() == IDOK) ? TRUE : FALSE;
			break;
		}
	case eTableID_Contact:
		{
			CCustomInfoDlg dlg;
			dlg.m_eTableOption = eTableOptions_Modify;
			dlg.m_pCurTable = &m_TableObj;
			dlg.m_iSelIndex = iSelItem;
			dlg.m_bIsModifyContact = TRUE;
			bNeedUpdateTable = TRUE;
			break;
		}
	case eTableID_Offer:
		{
			COfferInfoDlg dlg;
			dlg.m_eTableOption = eTableOptions_Modify;
			dlg.m_pCurTable = &m_TableObj;
			dlg.m_iSelIndex = iSelItem;
			bNeedUpdateTable = (dlg.DoModal() == IDOK) ? TRUE : FALSE;
			break;
		}
	case eTableID_Contract:
		{
			CContractInfoDlg dlg;
			dlg.m_eTableOption = eTableOptions_Modify;
			dlg.m_pCurTable = &m_TableObj;
			dlg.m_iSelIndex = iSelItem;
			bNeedUpdateTable = (dlg.DoModal() == IDOK) ? TRUE : FALSE;
			break;
		}
	case eTableID_None:
	default:
		MessageBox(_T("�˹�����δʵ��!"), _T("����"), MB_OK | MB_ICONERROR);
		break;
	}

	if (bNeedUpdateTable)
	{
		OpenTable(m_eCurTableID, _T(""));
	}
}
	
void CMainView::OnToolBarDelete()
{
	POSITION pos = m_MainListCtrl.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("����ѡ����Ҫ�޸ĵļ�¼!"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}
	int iSelItem = m_MainListCtrl.GetNextSelectedItem(pos);
	if (iSelItem < 0)
	{
		MessageBox(_T("����ѡ����Ҫ�޸ĵļ�¼!"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (IDYES == MessageBox(_T("��ȷ��Ҫɾ��ѡ�еļ�¼��"), _T("ȷ��"), MB_YESNO | MB_ICONQUESTION))
	{
		m_TableObj.DeleteItem(iSelItem);
		if (m_TableObj.UpdateTable())
		{
			OpenTable(m_eCurTableID, _T(""));
			MessageBox(_T("����ɾ���ɹ���"), _T("�ɹ�"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(_T("����ɾ��ʧ��!"), _T("ʧ��"), MB_OK | MB_ICONERROR);
		}
	}
}

void CMainView::OnToolBarExport()
{	
	CString strExcelFile;
	CFileDialog dlgSave(FALSE, _T(".xlsx"), NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, _T("Excel Workbook(*.xls)|*.xls||"));
	if (IDOK == dlgSave.DoModal())
		strExcelFile = dlgSave.GetPathName();
	else
		return;

	BOOL bResult = CExcelExporter::SaveDataFromListCtrl(strExcelFile, g_Tables[m_eCurTableID].strName, g_Tables[m_eCurTableID], (CListCtrl*)&m_MainListCtrl);
	if (TRUE == bResult)
		MessageBox(_T("�����ɹ���"), _T("�ɹ�"), MB_OK | MB_ICONINFORMATION);
	else
		MessageBox(_T("����ʧ�ܣ�"), _T("ʧ��"), MB_OK | MB_ICONERROR);
}

void CMainView::OnToolBarCount()
{
	CSaleCountDlg dlg;
	dlg.m_eCountTableID = m_eCurTableID;
	dlg.DoModal();
}

void CMainView::ToShowTable(eTableID curTable, CADODataBase* pDatabase)
{/*
	// �����ǰ�������
	int iColumnCount = m_MainListCtrl.GetHeaderCtrl()->GetItemCount();
	m_eCurTableID = eTableID_None;	
	m_MainListCtrl.DeleteAllItems();
	for (int i = 0;i < iColumnCount; i++)
	{
	   m_MainListCtrl.DeleteColumn(0);
	}

	// ���ݵ�ǰҪ�򿪵ı�,�����µı��ؼ�
	int iIndex = 0;
	for(TableColumnsList::iterator it = g_Tables[curTable].lstColumns.begin();
		it != g_Tables[curTable].lstColumns.end();
		it++, iIndex++)
	{
		TableColumn cl = *it;
		if (cl.bIsVisible)
		{
			m_MainListCtrl.InsertColumn(iIndex, cl.strDisplayText, 0, cl.lColumnWidth);
		}
	}*/

	// �򿪱�;
	if (!OpenTable(curTable, _T("")))
	{
		return;
	}

	// ��Ӹñ�Ĳ���ѡ��
	m_FindOptionCombox.ResetContent();
	for (DWORD dwIndex = 0; dwIndex < g_Tables[curTable].lstFinds.size(); dwIndex++)
	{
		m_FindOptionCombox.AddString(g_Tables[curTable].lstFinds[dwIndex].strDisplayText);	
	}
	m_FindOptionCombox.SetCurSel(0);
}

void CMainView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (!m_MainListCtrl.m_hWnd)
		return;

	InitUIControls();	
}

BOOL CMainView::OnEraseBkgnd(CDC* pDC)
{
	//����brushΪϣ���ı�����ɫ
	//CBrush backBrush(RGB(0x96,0xff, 0xff));
	////����ɵ�brush
	//CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	//CRect rect;
	//pDC->GetClipBox(&rect);
	////����Ҫ������
	//pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	//pDC->SelectObject(pOldBrush);
	//return TRUE;

	return CFormView::OnEraseBkgnd(pDC);
}



void CMainView::InitTableStruct()
{	
	// "ϵͳ�û���Ϣ��"
	g_Tables[eTableID_User].eID = eTableID_User;
	g_Tables[eTableID_User].strName = _T("S001");
	g_Tables[eTableID_User].lstColumns.clear();
	g_Tables[eTableID_User].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 80, L"ID", L"Ա�����", eTableDataType_String));
	g_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 80, L"LoginName", L"�û���", eTableDataType_String));
	//g_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 100, L"Password", L"����", eTableDataType_String));
	g_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 100, L"Group", L"�û���", eTableDataType_String));
	g_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 100, L"UserName", L"����", eTableDataType_String));
	g_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 80, L"Sex", L"�Ա�", eTableDataType_String));
	g_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 50, L"Birth", L"����", eTableDataType_Date));
	g_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 80,  L"Position", L"ְλ", eTableDataType_String));
	g_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 100, L"Tel", L"�绰", eTableDataType_String));
	g_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 100, L"Email", L"Email", eTableDataType_String));
	g_Tables[eTableID_User].lstColumns.push_back(TableColumn(FALSE, 250, L"FromDate", L"��ְ����", eTableDataType_Date));
	g_Tables[eTableID_User].lstFinds.clear();
	g_Tables[eTableID_User].lstFinds.push_back(TableFindOption(L"ID", L"Ա�����"));
	g_Tables[eTableID_User].lstFinds.push_back(TableFindOption(L"LoginName", L"�û���"));
	g_Tables[eTableID_User].lstFinds.push_back(TableFindOption(L"Group", L"�û���"));
	g_Tables[eTableID_User].lstFinds.push_back(TableFindOption(L"UserName", L"����"));

	// "��˾Ա����Ϣ��"
	g_Tables[eTableID_Employee].eID = eTableID_Employee;
	g_Tables[eTableID_Employee].strName = _T("S002");
	g_Tables[eTableID_Employee].lstColumns.clear();
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 100, L"ID", L"Ա�����", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 100, L"UserName", L"����", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 50, L"Sex", L"�Ա�", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 100, L"Birth", L"����", eTableDataType_Date));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 100, L"Hail",L"����", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 100, L"Married", L"���", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 150, L"Nation", L"����", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"Health", L"����״̬", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"IDNumber", L"���֤����", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"CellTel", L"�ֻ�����", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"Tel", L"�̶��绰", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"Email", L"Email", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"Address", L"��ͥסַ", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"Emergency", L"������ϵ��", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"EmergencyTel", L"������ϵ�˵绰", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"Eductaion", L"ѧ��", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"College", L"��ҵԺУ", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"CollegeDate", L"ʱ�䷶Χ", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"Position", L"ְλ", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"FromDate", L"��ְʱ��", eTableDataType_Date));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"UntilDate", L"��ְʱ��", eTableDataType_Date));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"LastCompany", L"��һ��˾", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"CompanyDate", L"ʱ�䷶Χ", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"Holidays", L"�������", eTableDataType_FLOAT));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"Photo", L"��Ƭ", eTableDataType_DWORD));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"BaseInfo", L"������Ϣ", eTableDataType_String));
	g_Tables[eTableID_Employee].lstColumns.push_back(TableColumn(FALSE, 200, L"Comment", L"��ע", eTableDataType_String));
	g_Tables[eTableID_Employee].lstFinds.clear();
	g_Tables[eTableID_Employee].lstFinds.push_back(TableFindOption(L"ID", L"Ա�����"));
	g_Tables[eTableID_Employee].lstFinds.push_back(TableFindOption(L"UserName", L"����"));
	g_Tables[eTableID_Employee].lstFinds.push_back(TableFindOption(L"IDNumber", L"���֤����"));
	g_Tables[eTableID_Employee].lstFinds.push_back(TableFindOption(L"CellTel", L"�ֻ�����"));

	// ���ͻ��嵥��
	g_Tables[eTableID_Custom].eID = eTableID_Custom;
	g_Tables[eTableID_Custom].strName = _T("S003");
	g_Tables[eTableID_Custom].lstColumns.clear();
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"CustomID", L"�ͻ���", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"Type", L"��λ����", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 150, L"FullName", L"��˾����", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"ShortName", L"��˾���", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"Code",L"��ѯ����", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 150, L"Address", L"��ַ", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 80, L"PostCode", L"�ʱ�", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"Websit", L"��ַ", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"Tel", L"�绰", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"Fox", L"����", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 150, L"Comment", L"��ע", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"Contact", L"��ϵ��", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"Position", L"ְλ", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"ContactTel", L"��ϵ�˵绰", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"ContactFox", L"��ϵ�˴���", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"Email", L"Email", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 150, L"Bank", L"������", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 150, L"Account", L"�˺�", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 150, L"TaxNumber", L"˰��", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 150, L"BankNumber", L"�����к�", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 150, L"Others", L"����", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"Inputer", L"¼��Ա", eTableDataType_String));
	g_Tables[eTableID_Custom].lstColumns.push_back(TableColumn(FALSE, 100, L"ShareOrNot", L"�Ƿ���", eTableDataType_DWORD));
	g_Tables[eTableID_Custom].lstFinds.clear();
	g_Tables[eTableID_Custom].lstFinds.push_back(TableFindOption(L"CustomID", L"�ͻ���"));
	g_Tables[eTableID_Custom].lstFinds.push_back(TableFindOption(L"Type", L"��λ����"));
	g_Tables[eTableID_Custom].lstFinds.push_back(TableFindOption(L"FullName", L"��˾����"));
	g_Tables[eTableID_Custom].lstFinds.push_back(TableFindOption(L"ShortName", L"��˾���"));
	g_Tables[eTableID_Custom].lstFinds.push_back(TableFindOption(L"Code", L"��ѯ����"));
	g_Tables[eTableID_Custom].lstFinds.push_back(TableFindOption(L"Address", L"��ַ"));
	g_Tables[eTableID_Custom].lstFinds.push_back(TableFindOption(L"Contact", L"��ϵ��"));

	
	// ����ϵ���嵥��
	g_Tables[eTableID_Contact].eID = eTableID_Contact;
	g_Tables[eTableID_Contact].strName = _T("S004");
	g_Tables[eTableID_Contact].lstColumns.clear();
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 100, L"Type", L"��λ����", eTableDataType_String));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 80, L"ContactName", L"����", eTableDataType_String));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 100, L"Code", L"��ѯ����", eTableDataType_String));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 100, L"CompanyName", L"��˾���", eTableDataType_String));
	//g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 80, L"IsDefault",L"ȱʡ", eTableDataType_DWORD));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 80, L"Position",L"ְλ", eTableDataType_String));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 50, L"Sex", L"�Ա�", eTableDataType_String));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 80, L"Tel", L"�绰", eTableDataType_String));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 80, L"Fox", L"����", eTableDataType_String));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 80, L"CellTel", L"�ֻ�", eTableDataType_String));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 100, L"Email", L"����", eTableDataType_String));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 100, L"Address", L"��ַ", eTableDataType_String));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 50, L"PostCode", L"�ʱ�", eTableDataType_String));
	g_Tables[eTableID_Contact].lstColumns.push_back(TableColumn(FALSE, 100, L"Comment", L"��ע", eTableDataType_String));
	g_Tables[eTableID_Contact].lstFinds.clear();
	g_Tables[eTableID_Contact].lstFinds.push_back(TableFindOption(L"Type", L"��λ����"));
	g_Tables[eTableID_Contact].lstFinds.push_back(TableFindOption(L"ContactName", L"����"));
	g_Tables[eTableID_Contact].lstFinds.push_back(TableFindOption(L"Code", L"��ѯ����"));
	g_Tables[eTableID_Contact].lstFinds.push_back(TableFindOption(L"CompanyName", L"��˾���"));
	g_Tables[eTableID_Contact].lstFinds.push_back(TableFindOption(L"CellTel", L"�ֻ�"));
	
	// �������嵥��
	g_Tables[eTableID_Offer].eID = eTableID_Offer;
	g_Tables[eTableID_Offer].strName = _T("S005");
	g_Tables[eTableID_Offer].lstColumns.clear();
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 120, L"OfferID", L"���۵���", eTableDataType_String));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 120, L"OfferType", L"��������", eTableDataType_String));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 80, L"OfferPeople", L"������", eTableDataType_String));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 100, L"Custom", L"�ͻ�", eTableDataType_String));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 100, L"Contact",L"��ϵ��", eTableDataType_String));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 100, L"Tel", L"�绰", eTableDataType_String));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 150, L"ProjectName", L"��Ŀ", eTableDataType_String));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 150, L"Equipment", L"�豸", eTableDataType_String));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 80, L"Number", L"����", eTableDataType_DWORD));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 120, L"OfferDate", L"��������", eTableDataType_Date));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 100, L"Cost", L"�ܽ��", eTableDataType_FLOAT));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 80, L"Status", L"״̬", eTableDataType_String));
	g_Tables[eTableID_Offer].lstColumns.push_back(TableColumn(FALSE, 100, L"Comment", L"��ע", eTableDataType_String));
	g_Tables[eTableID_Offer].lstFinds.clear();
	g_Tables[eTableID_Offer].lstFinds.push_back(TableFindOption(L"OfferID", L"���۵���"));
	g_Tables[eTableID_Offer].lstFinds.push_back(TableFindOption(L"OfferType", L"��������"));
	g_Tables[eTableID_Offer].lstFinds.push_back(TableFindOption(L"OfferPeople", L"������"));
	g_Tables[eTableID_Offer].lstFinds.push_back(TableFindOption(L"Custom", L"�ͻ�"));
	g_Tables[eTableID_Offer].lstFinds.push_back(TableFindOption(L"Contact", L"��ϵ��"));
	g_Tables[eTableID_Offer].lstFinds.push_back(TableFindOption(L"ProjectName", L"��Ŀ"));
	g_Tables[eTableID_Offer].lstFinds.push_back(TableFindOption(L"Status", L"״̬"));
	
	// �������굥��
	g_Tables[eTableID_OfferInfo].eID = eTableID_OfferInfo;
	g_Tables[eTableID_OfferInfo].strName = _T("S006");
	g_Tables[eTableID_OfferInfo].lstColumns.clear();
	g_Tables[eTableID_OfferInfo].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_OfferInfo].lstColumns.push_back(TableColumn(FALSE, 120, L"OfferID", L"���۵���", eTableDataType_String));
	//g_Tables[eTableID_OfferInfo].lstColumns.push_back(TableColumn(FALSE, 100, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_OfferInfo].lstColumns.push_back(TableColumn(FALSE, 150, L"Equipment", L"�豸", eTableDataType_String));
	g_Tables[eTableID_OfferInfo].lstColumns.push_back(TableColumn(FALSE, 150, L"Model", L"�ͺ�", eTableDataType_String));
	g_Tables[eTableID_OfferInfo].lstColumns.push_back(TableColumn(FALSE, 100, L"Number", L"����", eTableDataType_DWORD));
	g_Tables[eTableID_OfferInfo].lstColumns.push_back(TableColumn(FALSE, 100, L"Price", L"����", eTableDataType_FLOAT));
	g_Tables[eTableID_OfferInfo].lstColumns.push_back(TableColumn(FALSE, 100, L"Cost", L"�ܼ�", eTableDataType_FLOAT));
	g_Tables[eTableID_OfferInfo].lstColumns.push_back(TableColumn(FALSE, 150, L"Comment", L"��ע", eTableDataType_String));
	g_Tables[eTableID_OfferInfo].lstFinds.clear();
	g_Tables[eTableID_OfferInfo].lstFinds.push_back(TableFindOption(L"OfferID", L"���۵���"));
	g_Tables[eTableID_OfferInfo].lstFinds.push_back(TableFindOption(L"Equipment", L"�豸"));
	g_Tables[eTableID_OfferInfo].lstFinds.push_back(TableFindOption(L"Model", L"�ͺ�"));

	// ����ͬ�嵥��
	g_Tables[eTableID_Contract].eID = eTableID_Contract;
	g_Tables[eTableID_Contract].strName = _T("S007");
	g_Tables[eTableID_Contract].lstColumns.clear();
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 120, L"ContractID", L"��ͬ��", eTableDataType_String));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 120, L"CustomContractID", L"�ͻ���ͬ��", eTableDataType_String));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 120, L"Type", L"��ͬ����", eTableDataType_String));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 100, L"Saler", L"����Ա", eTableDataType_String));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 120, L"Custom",L"�ͻ�", eTableDataType_String));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 100, L"Contact", L"��ϵ��", eTableDataType_String));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 100, L"Tel", L"�绰", eTableDataType_String));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 100, L"ProjectName", L"��Ŀ", eTableDataType_String));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 150, L"Equipment", L"�豸", eTableDataType_String));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 100, L"Cost", L"�ܽ��", eTableDataType_FLOAT));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 100, L"ContractDate", L"��ͬ����", eTableDataType_Date));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 100, L"FinishDate", L"��������", eTableDataType_Date));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 100, L"Status", L"״̬", eTableDataType_String));
	g_Tables[eTableID_Contract].lstColumns.push_back(TableColumn(FALSE, 150, L"Comment", L"��ע", eTableDataType_String));
	g_Tables[eTableID_Contract].lstFinds.clear();
	g_Tables[eTableID_Contract].lstFinds.push_back(TableFindOption(L"ContractID", L"��ͬ��"));
	g_Tables[eTableID_Contract].lstFinds.push_back(TableFindOption(L"CustomContractID", L"�ͻ���ͬ��"));
	g_Tables[eTableID_Contract].lstFinds.push_back(TableFindOption(L"Type", L"��ͬ����"));
	g_Tables[eTableID_Contract].lstFinds.push_back(TableFindOption(L"Saler", L"����Ա"));
	g_Tables[eTableID_Contract].lstFinds.push_back(TableFindOption(L"Custom", L"�ͻ�"));
	g_Tables[eTableID_Contract].lstFinds.push_back(TableFindOption(L"Contact", L"��ϵ��"));
	g_Tables[eTableID_Contract].lstFinds.push_back(TableFindOption(L"ProjectName", L"��Ŀ"));
	g_Tables[eTableID_Contract].lstFinds.push_back(TableFindOption(L"Equipment", L"�豸"));
	g_Tables[eTableID_Contract].lstFinds.push_back(TableFindOption(L"Status", L"״̬"));
	
	// ����ͬ�굥��
	g_Tables[eTableID_ContractInfo].eID = eTableID_ContractInfo;
	g_Tables[eTableID_ContractInfo].strName = _T("S008");
	g_Tables[eTableID_ContractInfo].lstColumns.clear();
	g_Tables[eTableID_ContractInfo].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_ContractInfo].lstColumns.push_back(TableColumn(FALSE, 100, L"ContractID", L"��ͬ����", eTableDataType_String));
	g_Tables[eTableID_ContractInfo].lstColumns.push_back(TableColumn(FALSE, 100, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_ContractInfo].lstColumns.push_back(TableColumn(FALSE, 150, L"Equipment", L"�豸", eTableDataType_String));
	g_Tables[eTableID_ContractInfo].lstColumns.push_back(TableColumn(FALSE, 100, L"Model", L"�ͺ�", eTableDataType_String));
	g_Tables[eTableID_ContractInfo].lstColumns.push_back(TableColumn(FALSE, 100, L"Number", L"����", eTableDataType_DWORD));
	g_Tables[eTableID_ContractInfo].lstColumns.push_back(TableColumn(FALSE, 100, L"Price", L"����", eTableDataType_FLOAT));
	g_Tables[eTableID_ContractInfo].lstColumns.push_back(TableColumn(FALSE, 100, L"Cost", L"�ܼ�", eTableDataType_FLOAT));
	g_Tables[eTableID_ContractInfo].lstColumns.push_back(TableColumn(FALSE, 150, L"Comment", L"��ע", eTableDataType_String));
	g_Tables[eTableID_ContractInfo].lstFinds.clear();
	g_Tables[eTableID_ContractInfo].lstFinds.push_back(TableFindOption(L"ContractID", L"��ͬ����"));
	g_Tables[eTableID_ContractInfo].lstFinds.push_back(TableFindOption(L"Equipment", L"�豸"));
	g_Tables[eTableID_ContractInfo].lstFinds.push_back(TableFindOption(L"Model", L"�ͺ�"));


	// ����Ŀ�����
	g_Tables[eTableID_Projects].eID = eTableID_Projects;
	g_Tables[eTableID_Projects].strName = _T("BMMemberTable");
	g_Tables[eTableID_Projects].lstColumns.clear();
	g_Tables[eTableID_Projects].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_Projects].lstColumns.push_back(TableColumn(FALSE, 100, L"ID", L"��Ŀ������1", eTableDataType_String));
	g_Tables[eTableID_Projects].lstColumns.push_back(TableColumn(FALSE, 100, L"UserName", L"��Ŀ������2", eTableDataType_String));
	g_Tables[eTableID_Projects].lstColumns.push_back(TableColumn(FALSE, 50, L"Sex", L"��Ŀ������3", eTableDataType_String));
	g_Tables[eTableID_Projects].lstColumns.push_back(TableColumn(FALSE, 100, L"Birth", L"��Ŀ������4", eTableDataType_String));
	g_Tables[eTableID_Projects].lstColumns.push_back(TableColumn(FALSE, 100, L"Hail",L"��Ŀ������5", eTableDataType_String));
	g_Tables[eTableID_Projects].lstColumns.push_back(TableColumn(FALSE, 100, L"Married", L"��Ŀ������6", eTableDataType_String));
	g_Tables[eTableID_Projects].lstColumns.push_back(TableColumn(FALSE, 150, L"Nation", L"��Ŀ������7", eTableDataType_String));


	// ���ɹ������
	g_Tables[eTableID_Buy].eID = eTableID_Buy;
	g_Tables[eTableID_Buy].strName = _T("BMMemberTable");
	g_Tables[eTableID_Buy].lstColumns.clear();
	g_Tables[eTableID_Buy].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_Buy].lstColumns.push_back(TableColumn(FALSE, 100, L"ID", L"�ɹ�������1", eTableDataType_String));
	g_Tables[eTableID_Buy].lstColumns.push_back(TableColumn(FALSE, 100, L"UserName", L"�ɹ�������2", eTableDataType_String));
	g_Tables[eTableID_Buy].lstColumns.push_back(TableColumn(FALSE, 50, L"Sex", L"�ɹ�������3", eTableDataType_String));
	g_Tables[eTableID_Buy].lstColumns.push_back(TableColumn(FALSE, 100, L"Birth", L"�ɹ�������4", eTableDataType_String));
	g_Tables[eTableID_Buy].lstColumns.push_back(TableColumn(FALSE, 100, L"Hail",L"�ɹ�������5", eTableDataType_String));
	g_Tables[eTableID_Buy].lstColumns.push_back(TableColumn(FALSE, 100, L"Married", L"�ɹ�������6", eTableDataType_String));
	g_Tables[eTableID_Buy].lstColumns.push_back(TableColumn(FALSE, 150, L"Nation", L"�ɹ�������7", eTableDataType_String));


	// ���������
	g_Tables[eTableID_Save].eID = eTableID_Save;
	g_Tables[eTableID_Save].strName = _T("BMMemberTable");
	g_Tables[eTableID_Save].lstColumns.clear();
	g_Tables[eTableID_Save].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_Save].lstColumns.push_back(TableColumn(FALSE, 100, L"ID", L"��������1", eTableDataType_String));
	g_Tables[eTableID_Save].lstColumns.push_back(TableColumn(FALSE, 100, L"UserName", L"��������2", eTableDataType_String));
	g_Tables[eTableID_Save].lstColumns.push_back(TableColumn(FALSE, 50, L"Sex", L"��������3", eTableDataType_String));
	g_Tables[eTableID_Save].lstColumns.push_back(TableColumn(FALSE, 100, L"Birth", L"��������4", eTableDataType_String));
	g_Tables[eTableID_Save].lstColumns.push_back(TableColumn(FALSE, 100, L"Hail",L"��������5", eTableDataType_String));
	g_Tables[eTableID_Save].lstColumns.push_back(TableColumn(FALSE, 100, L"Married", L"��������6", eTableDataType_String));
	g_Tables[eTableID_Save].lstColumns.push_back(TableColumn(FALSE, 150, L"Nation", L"��������7", eTableDataType_String));


	// ��ϵͳ�����
	g_Tables[eTableID_SystemSetting].eID = eTableID_SystemSetting;
	g_Tables[eTableID_SystemSetting].strName = _T("S002");
	g_Tables[eTableID_SystemSetting].lstColumns.clear();
	g_Tables[eTableID_SystemSetting].lstColumns.push_back(TableColumn(TRUE, 50, L"Index", L"���", eTableDataType_DWORD));
	g_Tables[eTableID_SystemSetting].lstColumns.push_back(TableColumn(FALSE, 100, L"ID", L"ϵͳ������1", eTableDataType_String));
	g_Tables[eTableID_SystemSetting].lstColumns.push_back(TableColumn(FALSE, 100, L"UserName", L"ϵͳ������2", eTableDataType_String));
	g_Tables[eTableID_SystemSetting].lstColumns.push_back(TableColumn(FALSE, 50, L"Sex", L"ϵͳ������3", eTableDataType_String));
	g_Tables[eTableID_SystemSetting].lstColumns.push_back(TableColumn(FALSE, 100, L"Birth", L"ϵͳ������4", eTableDataType_String));
	g_Tables[eTableID_SystemSetting].lstColumns.push_back(TableColumn(FALSE, 100, L"Hail",L"ϵͳ������5", eTableDataType_String));
	g_Tables[eTableID_SystemSetting].lstColumns.push_back(TableColumn(FALSE, 100, L"Married", L"ϵͳ������6", eTableDataType_String));
	g_Tables[eTableID_SystemSetting].lstColumns.push_back(TableColumn(FALSE, 150, L"Nation", L"ϵͳ������7", eTableDataType_String));

}

void CMainView::ShowFindOptions(BOOL bShow)
{
	m_FindOptionStatic.ShowWindow(bShow);
	m_FindOptionCombox.ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_MAINVIEW_FINDOPTION)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_MAINVIEW_FINDVALUE)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_MAINVIEW_FINDVALUE)->ShowWindow(bShow);
	GetDlgItem(IDC_BTN_MAINVIEW_FIND)->ShowWindow(bShow);	
}

BOOL CMainView::OpenTable(eTableID tableID, CString strOperation)
{	
	if (eTableID_None == tableID)
		return FALSE;

	CStringW strOpenOperation;
	if (strOperation.IsEmpty())
		strOpenOperation.Format(L"SELECT * FROM %s", g_Tables[tableID].strName);
	else
		strOpenOperation = strOperation;

	// Open table from database
	BOOL bResult = TRUE;
	m_TableObj.CloseTable();
	bResult = m_TableObj.OpenTable(g_pDatabase->GetConnectionPtr(), strOpenOperation);
	if (!bResult)
	{
		return bResult;
	}
	
	// Show records in UI
	m_eCurTableID = tableID;
	ShowCurrentTable(m_bIsMultiPage, m_lCountPerPage, 0);

	return bResult;
}

void CMainView::ShowCurrentTable(BOOL bIsMultiPage, LONG lCountPerPage, LONG lCurPage)
{
	int iColumnCount = m_MainListCtrl.GetHeaderCtrl()->GetItemCount();
	m_MainListCtrl.DeleteAllItems();
	for (int i = 0;i < iColumnCount; i++)
	{
	   m_MainListCtrl.DeleteColumn(0);
	}
	
	InitListHeader(&m_MainListCtrl, &g_Tables[m_eCurTableID]);

	DWORD dwRecordCnt = m_TableObj.GetRecordCnt();

	DWORD dwStart = 0;
	DWORD dwEnd = dwRecordCnt;
	LONG lTotalPage = 1;
	GetDlgItem(IDC_SYSLINK_MAINVIEW_FIRST)->EnableWindow(bIsMultiPage);
	GetDlgItem(IDC_SYSLINK_MAINVIEW_UP)->EnableWindow(bIsMultiPage);
	GetDlgItem(IDC_SYSLINK_MAINVIEW_DWON)->EnableWindow(bIsMultiPage);
	GetDlgItem(IDC_SYSLINK_MAINVIEW_LAST)->EnableWindow(bIsMultiPage);
	if (TRUE == bIsMultiPage)
	{
		lTotalPage = (dwRecordCnt / lCountPerPage) + 1;
		if (lCurPage <= 0)
		{
			lCurPage = 0;
			GetDlgItem(IDC_SYSLINK_MAINVIEW_FIRST)->EnableWindow(FALSE);
			GetDlgItem(IDC_SYSLINK_MAINVIEW_UP)->EnableWindow(FALSE);
		}
		else if(lCurPage >= (lTotalPage-1))
		{
			lCurPage = lTotalPage-1;
			GetDlgItem(IDC_SYSLINK_MAINVIEW_DWON)->EnableWindow(FALSE);
			GetDlgItem(IDC_SYSLINK_MAINVIEW_LAST)->EnableWindow(FALSE);
		}
		//
		dwStart = lCurPage * lCountPerPage;
		dwEnd = min(dwStart + lCountPerPage, dwRecordCnt);
	}
	CString strPageInfo;
	strPageInfo.Format(_T("��ǰ:��%dҳ ��%dҳ"), lCurPage+1, lTotalPage);
	SetDlgItemText(IDC_STATIC_MAINVIEW_PAGENUM, strPageInfo);

	FillListFromTable(&m_MainListCtrl, &m_TableObj, &g_Tables[m_eCurTableID], dwStart, dwEnd);
}

void CMainView::OnBnClickedBtnMainviewFind()
{
	CString strFindName;
	CString strFindValue;
	CString strFindSetting;

	// ��ȡ���ҵ�����
	int iSelIndex = m_FindOptionCombox.GetCurSel();
	if (iSelIndex < 0)
	{
		MessageBox(_T("�޲���ѡ��!"), _T("����"), MB_OK | MB_ICONWARNING);
	}
	strFindName = g_Tables[m_eCurTableID].lstFinds[iSelIndex].strName;

	// ��ȡ���ҵ�ֵ
	GetDlgItemText(IDC_EDIT_MAINVIEW_FINDVALUE, strFindValue);

	// �������м�¼
	if (strFindValue.IsEmpty())
	{
		strFindSetting.Format(_T("SELECT * FROM %s"), g_Tables[m_eCurTableID].strName);
	}
	// ��ʽ��SQL�������
	else
	{
		strFindSetting.Format(_T("SELECT * FROM %s WHERE %s LIKE '%s%s%s'"), 
			g_Tables[m_eCurTableID].strName, strFindName, _T("%"), strFindValue, _T("%"));
	}

	// ���´򿪱�
	OpenTable(m_eCurTableID, strFindSetting);
}
