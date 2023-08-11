// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "BMHoistManager.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "HomePageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CADODataBase* g_pDatabase;

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_COMMAND(ID_MENU_SYSTEM_HOME, OnMenuSystemHome)
	ON_COMMAND(ID_MENU_SYSTEM_OPTIONS, OnMenuSystemOptions)
	ON_COMMAND(ID_MENU_SETTING_SET, OnMenuUserSettingSet)
	ON_COMMAND(ID_MENUUSER_CHANGEPSW, OnMenuUserChangePassword)
	ON_COMMAND(ID_MENUUSER_LOGOUT, OnMenuUserLogout)
	ON_COMMAND(ID_MENUTOOL_CALCULATOR, OnMenuToolCalculator)
	ON_COMMAND(ID_MENUTOOL_CALENDAR, OnMenuToolCalendar)
	ON_UPDATE_COMMAND_UI(ID_MENU_SYSTEM_HOME, &CMainFrame::OnUpdateMenuSystemHome)
	ON_UPDATE_COMMAND_UI(ID_MENU_SYSTEM_OPTIONS, &CMainFrame::OnUpdateMenuSystemOptions)
	ON_UPDATE_COMMAND_UI(ID_MENU_SETTING_SET, &CMainFrame::OnUpdateMenuSettingSet)
	ON_UPDATE_COMMAND_UI(ID_MENU_SETTING_CHANGEPSW, &CMainFrame::OnUpdateMenuSettingChangePsw)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_FIND, &CMainFrame::OnUpdateToolbtnFind)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_NEW, &CMainFrame::OnUpdateToolbtnNew)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_UPDATE, &CMainFrame::OnUpdateToolbtnModify)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_DELETE, &CMainFrame::OnUpdateToolbtnDelete)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_EXPORT, &CMainFrame::OnUpdateToolbtnExport)
	ON_UPDATE_COMMAND_UI(ID_TOOLBTN_COUNT, &CMainFrame::OnUpdateToolbtnCount)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	IDS_STATUSBAR_TXT,
	IDS_STATUSBAR_TXT,
	IDS_STATUSBAR_TXT,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_eCurTableID = eTableID_None;
	m_bMenuSystemOptionsChecked = TRUE;
}

CMainFrame::~CMainFrame()
{
	if (g_pDatabase)
	{
		g_pDatabase->CloseDatabase();
		delete g_pDatabase;
		g_pDatabase = NULL;
	}
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
	//	CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	//{
	//	TRACE0("Failed to create view window\n");
	//	return -1;
	//}
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);  
	SetIcon(m_hIcon,TRUE);
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd,GWL_STYLE)&~WS_MAXIMIZEBOX);
	
	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT|TBSTYLE_LIST, WS_CHILD | WS_VISIBLE | CBRS_TOP
	//	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	SetWindowText(_T("BM ERP System"));
	
	m_wndStatusBar.SetPaneInfo(0, IDS_STATUSBAR_TXT, SBPS_STRETCH, 200);
	m_wndStatusBar.SetPaneInfo(1, IDS_STATUSBAR_TXT, SBPS_STRETCH, 200);
	m_wndStatusBar.SetPaneInfo(2, IDS_STATUSBAR_TXT, SBPS_STRETCH, 200);
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(36);

	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	MoveWindow(0, 0, GetSystemMetrics(SM_CXFULLSCREEN), rect.bottom);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~WS_THICKFRAME;
	cs.style &= ~FWS_ADDTOTITLE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	//m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnMenuSystemHome()
{
	SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CHomepageView));
}
void CMainFrame::OnMenuSystemOptions()
{
	if (m_bMenuSystemOptionsChecked)
	{
		m_bMenuSystemOptionsChecked = FALSE;
		m_wndLeftSplitter.SetColumnInfo(0, 0, 0);
		m_wndLeftSplitter.RecalcLayout();
	}
	else
	{
		m_bMenuSystemOptionsChecked = TRUE;
		m_wndLeftSplitter.SetColumnInfo(0, 280, 280);
		m_wndLeftSplitter.RecalcLayout();
	}
}
void CMainFrame::OnMenuUserSettingSet()
{
	MessageBox(_T("此功能暂未实现!"), _T("错误"), MB_OK | MB_ICONERROR);
}
void CMainFrame::OnMenuUserChangePassword()
{
	MessageBox(_T("此功能暂未实现!"), _T("错误"), MB_OK | MB_ICONERROR);
}

void CMainFrame::OnMenuUserLogout()
{
	MessageBox(_T("此功能暂未实现!"), _T("错误"), MB_OK | MB_ICONERROR);
}

void CMainFrame::OnMenuToolCalculator()
{
	ShellExecute(m_hWnd, _T("open"), _T("calc.exe"), _T(""), _T(""), SW_SHOW);
}


void CMainFrame::OnMenuToolCalendar()
{
	MessageBox(_T("此功能暂未实现!"), _T("错误"), MB_OK | MB_ICONERROR);
}
	
void CMainFrame::OnUpdateMenuSystemHome(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CMainFrame::OnUpdateMenuSystemOptions(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CMainFrame::OnUpdateMenuSettingSet(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CMainFrame::OnUpdateMenuSettingChangePsw(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CMainFrame::OnUpdateToolbtnFind(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateToolbtnNew(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((eTableID_Contact==m_eCurTableID) ? FALSE : TRUE);
}

void CMainFrame::OnUpdateToolbtnModify(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateToolbtnDelete(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateToolbtnExport(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateToolbtnCount(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;

	if (eTableID_Offer==m_eCurTableID || 
		eTableID_Contract==m_eCurTableID)
	{
		bEnable = TRUE;
	}
	pCmdUI->Enable(bEnable);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	  //创建一个静态分栏窗口，分为一行二列 
    if(m_wndLeftSplitter.CreateStatic(this,1,2)==NULL) 
        return FALSE; 

    //将CLeftView连接到0行0列窗格上
    m_wndLeftSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(280,600), pContext); 

    m_wndLeftSplitter.CreateView(0, 1, RUNTIME_CLASS(CHomepageView), CSize(280,600), pContext); 

	return TRUE;//CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::DoLeftViewActions(ULONG uBtnID, BOOL bShowHomepage)
{
	CToolBarCtrl& bar = m_wndToolBar.GetToolBarCtrl();

	if (bShowHomepage)
	{
		//bar.PressButton(ID_TOOLBTN_HOMEPAGE, TRUE);
		//SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CHomepageView));
		////	
		//m_bTooBtnFindCanEnable = FALSE;
		//m_bTooBtnNewCanEnable = FALSE;
		//m_bTooBtnModifyCanEnable = FALSE;
		//m_bTooBtnDeleteCanEnable = FALSE;
		//m_bTooBtnExportCanEnable = FALSE;
		return;
	}
	else
	{
		//bar.PressButton(ID_TOOLBTN_HOMEPAGE, FALSE);
		////	
		//m_bTooBtnFindCanEnable = TRUE;
		//m_bTooBtnNewCanEnable = TRUE;
		//m_bTooBtnModifyCanEnable = TRUE;
		//m_bTooBtnDeleteCanEnable = TRUE;
		//m_bTooBtnExportCanEnable = TRUE;
	}

	switch(uBtnID)
	{
	case IDC_BTN_CUSTOM:
		OnLeftViewBtnCustom();
		break;
	case IDC_BTN_SALE:
		OnLeftViewBtnSale();
		break;
	case IDC_BTN_HETONG:
		OnLeftViewBtnHetong();
		break;
	case IDC_BTN_PROJECTS:
		OnLeftViewBtnProjects();
		break;
	case IDC_BTN_BUY:
		OnLeftViewBtnBuy();
		break;
	case IDC_BTN_SAVE:
		OnLeftViewBtnSave();
		break;
	case IDC_BTN_EMPLOYEE:
		OnLeftViewBtnEmployee();
		break;
	case IDC_BTN_USER:
		OnLeftViewBtnUser();
		break;
	case IDC_BTN_SYSTEM:
		OnLeftViewBtnSystemSetting();
		break;
	}
}

void CMainFrame::ToShowTable(eTableID tableId)
{	
	if (eTableID_None != tableId)
	{		
		SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CMainView));
		CMainView* pMainView = (CMainView*)m_wndLeftSplitter.GetPane(0, 1);
		if (pMainView)
		{
			pMainView->ToShowTable(tableId, g_pDatabase);
		}
	}
	else
	{
		SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CHomepageView));
	}
	m_eCurTableID = tableId;
}

void CMainFrame::SwitchViewInSplitter(CSplitterWnd* pSplitter, int row, 
									  int col, CRuntimeClass* pViewClass)
{
	ASSERT_VALID( pSplitter );
	ASSERT( pViewClass != NULL );

	ASSERT( pViewClass->IsDerivedFrom( RUNTIME_CLASS( CView ) ) );

	// 1 - Find the view to be replaced
	CWnd* pPaneWnd = pSplitter->GetPane( row, col );
	if( !pPaneWnd->IsKindOf( RUNTIME_CLASS( CView ) ) )
	{
		TRACE2(	"Unable to switch: pane (%d,%d) is not a view\n",
		row, col );
		return;
	}

	CView* pCurrentView = static_cast<CView*>( pPaneWnd );
	ASSERT_VALID( pCurrentView );
	ASSERT_KINDOF( CView, pCurrentView );

	if( pCurrentView->IsKindOf( pViewClass ) )
	{
		// No need to switch for same view class
		return;
	}

	// 2 - Store current view position and activation state
	CRect rcView;
	pCurrentView->GetWindowRect( &rcView );

	CView* pActiveView = pSplitter->GetParentFrame()->GetActiveView();
	BOOL bSaveActive = ( pActiveView == NULL )	|| ( pActiveView == pCurrentView );

	// 3 - Find the associated document
	//CDocument* pDoc = pCurrentView->GetDocument();
	//ASSERT_VALID( pDoc );

	//// 4 - Make sure the document won't self-destruct
	//// when current view is destroyed
	//BOOL bSaveAutoDelete = pDoc->m_bAutoDelete;
	//pDoc->m_bAutoDelete = FALSE;

	// 5 - Destroy the current view
	pCurrentView->DestroyWindow();
	//if (pSplitter->GetPane( row, col ))
	//{
	//	pSplitter->DeleteView(row, col);
	//}

	// 6 - Restore document to initial state
	//pDoc->m_bAutoDelete = bSaveAutoDelete;

	// 7 - Initialize creation context used by CreateView()
	CCreateContext context;
	context.m_pNewDocTemplate = NULL;
	context.m_pLastView = NULL;
	context.m_pCurrentFrame = NULL;

	context.m_pNewViewClass = pViewClass;
	//context.m_pCurrentDoc = pDoc;

	// 8 - Create the new view
	pSplitter->CreateView(	row, col, pViewClass, rcView.Size(), NULL );

	CView* pNewView = static_cast<CView*>( pSplitter->GetPane( row, col ) );
	ASSERT_VALID( pNewView );
	ASSERT_KINDOF( CView, pNewView );

	// 9 - Position the new view like the old one and
	// activate it if needed
	pSplitter->ScreenToClient( &rcView );
	pNewView->MoveWindow( &rcView, TRUE );
	//if( bSaveActive )
	{
		pSplitter->GetParentFrame()->SetActiveView( pNewView );
	}
	//pSplitter->UpdateWindow();
	// 10 - Send WM_INITIALUPDATE to the view	
	//this->InitialUpdateFrame();
	//pNewView->UpdateWindow();
	this->Invalidate();
	//pNewView->GetParentFrame()->InitialUpdateFrame( NULL, TRUE );
}


CString	CMainFrame::GetUserGroupText(DWORD dwGroup)
{
	CString strGroupText;

	switch(dwGroup)
	{
	case BMHOIST_USER_GROUP_ADMIN:
		strGroupText = _T("管理员");
		break;
	}

	return strGroupText;
}


void CMainFrame::OnLeftViewBtnCustom()
{
	SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CMainView));
	//CMainView* pMainView = (CMainView*)m_wndLeftSplitter.GetPane(0, 1);
	///pMainView->InitDataGirdCtrl(eTableID_Custom, g_pDatabase);
}

void CMainFrame::OnLeftViewBtnSale()
{
	SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CMainView));
	//CMainView* pMainView = (CMainView*)m_wndLeftSplitter.GetPane(0, 1);
	//pMainView->InitDataGirdCtrl(eTableID_Offer, g_pDatabase);
}

void CMainFrame::OnLeftViewBtnHetong()
{
	SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CMainView));
	CMainView* pMainView = (CMainView*)m_wndLeftSplitter.GetPane(0, 1);
	pMainView->ToShowTable(eTableID_Contract, g_pDatabase);
}

void CMainFrame::OnLeftViewBtnProjects()
{
	SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CMainView));
	CMainView* pMainView = (CMainView*)m_wndLeftSplitter.GetPane(0, 1);
	pMainView->ToShowTable(eTableID_Projects, g_pDatabase);
}

void CMainFrame::OnLeftViewBtnBuy()
{
	SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CMainView));
	CMainView* pMainView = (CMainView*)m_wndLeftSplitter.GetPane(0, 1);
	pMainView->ToShowTable(eTableID_Buy, g_pDatabase);
}

void CMainFrame::OnLeftViewBtnSave()
{
	SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CMainView));
	CMainView* pMainView = (CMainView*)m_wndLeftSplitter.GetPane(0, 1);
	pMainView->ToShowTable(eTableID_Save, g_pDatabase);
}

void CMainFrame::OnLeftViewBtnEmployee()
{
	SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CMainView));
	CMainView* pMainView = (CMainView*)m_wndLeftSplitter.GetPane(0, 1);
	pMainView->ToShowTable(eTableID_Employee, g_pDatabase);
}

void CMainFrame::OnLeftViewBtnUser()
{
	SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CMainView));
	CMainView* pMainView = (CMainView*)m_wndLeftSplitter.GetPane(0, 1);
	pMainView->ToShowTable(eTableID_User, g_pDatabase);
}

void CMainFrame::OnLeftViewBtnSystemSetting()
{
	SwitchViewInSplitter(&m_wndLeftSplitter, 0, 1, RUNTIME_CLASS(CMainView));
	CMainView* pMainView = (CMainView*)m_wndLeftSplitter.GetPane(0, 1);
	pMainView->ToShowTable(eTableID_SystemSetting, g_pDatabase);
}


void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	CString strUserInfo;
	strUserInfo.Format(_T("当前用户:%s      用户组:%s     姓名:%s"), m_strLoginName, m_strUserGroup, m_strUserName);
	m_wndStatusBar.GetStatusBarCtrl().SetText(strUserInfo, 0, 0);
	m_wndStatusBar.GetStatusBarCtrl().SetIcon(0, AfxGetApp()->LoadIcon(IDI_ICON_USER));
	//m_StatusBar->SetIcon(0,AfxGetApp()->LoadIcon(IDI_GREEN));

	CString strDate;
	CTime time = CTime::GetCurrentTime();
	strDate.Format(_T("今天是：%d年%d月%d日"),time.GetYear(), time.GetMonth(), time.GetDay());
	m_wndStatusBar.GetStatusBarCtrl().SetText(strDate, 1, 0);
}
