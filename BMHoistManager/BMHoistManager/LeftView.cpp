// LeftView.cpp : implementation file
//

#include "stdafx.h"
#include "BMHoistManager.h"
#include "LeftView.h"
#include "MainFrm.h"


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CFormView)

CLeftView::CLeftView()
	: CFormView(CLeftView::IDD)
{
	AfxInitRichEdit();
}

CLeftView::~CLeftView()
{
}

void CLeftView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_OPTIONS, m_OptionsTree);
}

BEGIN_MESSAGE_MAP(CLeftView, CFormView)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CUSTOM, &CLeftView::OnBnClickedBtnCustom)
	ON_BN_CLICKED(IDC_BTN_SALE, &CLeftView::OnBnClickedBtnSale)
	ON_BN_CLICKED(IDC_BTN_HETONG, &CLeftView::OnBnClickedBtnHetong)
	ON_BN_CLICKED(IDC_BTN_PROJECTS, &CLeftView::OnBnClickedBtnProjects)
	ON_BN_CLICKED(IDC_BTN_BUY, &CLeftView::OnBnClickedBtnBuy)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CLeftView::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_FINANCE, &CLeftView::OnBnClickedBtnFinance)
	ON_BN_CLICKED(IDC_BTN_HR, &CLeftView::OnBnClickedBtnHr)
	ON_BN_CLICKED(IDC_BTN_ADMIN, &CLeftView::OnBnClickedBtnAdmin)
	ON_BN_CLICKED(IDC_BTN_STATISTICS, &CLeftView::OnBnClickedBtnStatistics)
	//ON_BN_CLICKED(IDC_BTN_USER, &CLeftView::OnBnClickedBtnUser)
	//ON_BN_CLICKED(IDC_BTN_SYSTEM, &CLeftView::OnBnClickedBtnSystem)
	//ON_BN_CLICKED(IDC_BTN_EMPLOYEE, &CLeftView::OnBnClickedBtnEmployee)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_OPTIONS, &CLeftView::OnTvnSelchangedTreeOptions)
END_MESSAGE_MAP()


// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLeftView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLeftView message handlers

//BOOL CLeftView::OnEraseBkgnd(CDC* pDC)
//{
//	//CBrush backBrush(RGB(0xA0,0xA0, 0xA0));
//	//CBrush* pOldBrush = pDC->SelectObject(&backBrush);
//
//	//CRect rect;
//	//pDC->GetClipBox(&rect);
//	//pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
//	//pDC->SelectObject(pOldBrush);
//
//	return TRUE;
//}

//HBRUSH CLeftView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	//if (nCtlColor==CTLCOLOR_STATIC || nCtlColor==CTLCOLOR_EDIT)
//	//{
//	//	pDC->SetBkMode(TRANSPARENT);
//	//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
//	//}
//
//	return hbr;
//}

void CLeftView::OnBnClickedBtnCustom()
{
	HTREEITEM hRootItem = NULL;
	HTREEITEM hCustomItem = NULL;
	HTREEITEM hContactItem = NULL;

	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_CUSTOM);
	m_OptionsTree.DeleteAllItems();

	// "�ͻ�����"
	hRootItem = m_OptionsTree.InsertItem(_T("�ͻ�����"));
	m_OptionsTree.SetItemData(hCustomItem, eTableID_None);

	// "�����ͻ�"
	hCustomItem = m_OptionsTree.InsertItem(_T("�����ͻ�"), hRootItem);
	m_OptionsTree.SetItemData(hCustomItem, eTableID_Custom);

	// "��ϵ�˹���"
	hContactItem = m_OptionsTree.InsertItem(_T("��ϵ�˹���"), hRootItem);
	m_OptionsTree.SetItemData(hContactItem, eTableID_Contact);

	m_OptionsTree.SelectItem(hRootItem);
	m_OptionsTree.Expand(hRootItem, TVE_EXPAND);
}

void CLeftView::OnBnClickedBtnSale()
{
	HTREEITEM hRootItem = NULL;
	HTREEITEM hOfferItem = NULL;
	HTREEITEM hContractItem = NULL;
	HTREEITEM hStatisticsItem = NULL;

	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_SALE);
	m_OptionsTree.DeleteAllItems();

	// "���۹���"
	hRootItem = m_OptionsTree.InsertItem(_T("���۹���"));
	m_OptionsTree.SetItemData(hRootItem, eTableID_None);

	// "���ۼ�¼"
	hOfferItem = m_OptionsTree.InsertItem(_T("���ۼ�¼"), hRootItem);
	m_OptionsTree.SetItemData(hOfferItem, eTableID_Offer);

	// "��ͬ��¼"
	hContractItem = m_OptionsTree.InsertItem(_T("��ͬ��¼"), hRootItem);
	m_OptionsTree.SetItemData(hContractItem, eTableID_Contract);

	// "����ͳ��"
	hStatisticsItem = m_OptionsTree.InsertItem(_T("����ͳ��"), hRootItem);
	m_OptionsTree.SetItemData(hStatisticsItem, eTableID_None);

	m_OptionsTree.SelectItem(hRootItem);
	m_OptionsTree.Expand(hRootItem, TVE_EXPAND);
}

void CLeftView::OnBnClickedBtnHetong()
{
	HTREEITEM hRootItem = NULL;
	HTREEITEM hContractItem = NULL;

	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_HETONG);
	m_OptionsTree.DeleteAllItems();

	// "��ͬ����"
	hRootItem = m_OptionsTree.InsertItem(_T("��ͬ����"));
	m_OptionsTree.SetItemData(hRootItem, eTableID_None);

	// "��ͬ��¼"
	hContractItem = m_OptionsTree.InsertItem(_T("��ͬ��¼"), hRootItem);
	m_OptionsTree.SetItemData(hContractItem, eTableID_Contract);
	
	m_OptionsTree.Expand(hRootItem, TVE_EXPAND);
	m_OptionsTree.SelectItem(hRootItem);
}

void CLeftView::OnBnClickedBtnProjects()
{
	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_PROJECTS, TRUE);
	m_OptionsTree.DeleteAllItems();
}

void CLeftView::OnBnClickedBtnBuy()
{
	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_BUY, TRUE);
	m_OptionsTree.DeleteAllItems();
}

void CLeftView::OnBnClickedBtnSave()
{
	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_SAVE, TRUE);
	m_OptionsTree.DeleteAllItems();
}

void CLeftView::OnBnClickedBtnFinance()
{
	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_FINANCE, TRUE);
	m_OptionsTree.DeleteAllItems();
}

void CLeftView::OnBnClickedBtnHr()
{	
	HTREEITEM hRootItem = NULL;
	HTREEITEM hUserItem = NULL;
	HTREEITEM hEmployeeItem = NULL;

	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_HR);
	m_OptionsTree.DeleteAllItems();

	// "���¹���"
	hRootItem = m_OptionsTree.InsertItem(_T("���¹���"));
	m_OptionsTree.SetItemData(hRootItem, eTableID_None);

	// "�û�����"
	hUserItem = m_OptionsTree.InsertItem(_T("�û�����"), hRootItem);
	m_OptionsTree.SetItemData(hUserItem, eTableID_User);

	// "Ա������"
	hEmployeeItem = m_OptionsTree.InsertItem(_T("Ա������"), hRootItem);
	m_OptionsTree.SetItemData(hEmployeeItem, eTableID_Employee);
	
	m_OptionsTree.SelectItem(hRootItem);
	m_OptionsTree.Expand(hRootItem, TVE_EXPAND);
}

void CLeftView::OnBnClickedBtnAdmin()
{
	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_ADMIN, TRUE);
	m_OptionsTree.DeleteAllItems();
}

void CLeftView::OnBnClickedBtnStatistics()
{
	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_STATISTICS, TRUE);
	m_OptionsTree.DeleteAllItems();
}


//void CLeftView::OnBnClickedBtnEmployee()
//{
//	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_EMPLOYEE);
//}
//
//void CLeftView::OnBnClickedBtnUser()
//{
//	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_USER);
//}
//
//void CLeftView::OnBnClickedBtnSystem()
//{
//	((CMainFrame*)AfxGetMainWnd())->DoLeftViewActions(IDC_BTN_SYSTEM);
//}


void CLeftView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	SetDlgItemText(IDC_RICHEDIT_NOTICE, _T("���޹���"));
}

void CLeftView::OnTvnSelchangedTreeOptions(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hSelItem = m_OptionsTree.GetSelectedItem();
	eTableID  tbID = (eTableID)m_OptionsTree.GetItemData(hSelItem);
	((CMainFrame*)AfxGetMainWnd())->ToShowTable(tbID);

	*pResult = 0;
}
