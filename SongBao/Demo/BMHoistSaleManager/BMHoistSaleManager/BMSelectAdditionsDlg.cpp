//-----------------------------------------------------------------------
//	File Name:BMSelectAdditionsDlg.cpp
//	Introduce:Class CBMSelectAdditionsDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMSelectAdditionsDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMSelectAdditionsDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMSelectAdditionsDlg, CDialog)
CBMSelectAdditionsDlg::CBMSelectAdditionsDlg(eAdditionType eType, CWnd* pParent /*=NULL*/)
	: CDialog(CBMSelectAdditionsDlg::IDD, pParent)
{
	m_eAdditionType = eType;
}
//-----------------------------------------------------------------------
CBMSelectAdditionsDlg::~CBMSelectAdditionsDlg()
{
}
//-----------------------------------------------------------------------
void CBMSelectAdditionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADDITION, m_lstCtlAdditions);
	DDX_Control(pDX, IDC_LIST_SELECTEDADD, m_lstCtlSelected);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMSelectAdditionsDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CBMSelectAdditionsDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CBMSelectAdditionsDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDOK, &CBMSelectAdditionsDlg::OnBnClickedOk)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMSelectAdditionsDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMSelectAdditionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Initiliaze lists
	m_imgNULL.Create(1, 18, TRUE | ILC_COLOR32, 1, 0); 

	InitTablesDataStruct();
	InitDataLstControls();
	FillAdditionLists();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMSelectAdditionsDlg::OnBnClickedBtnAdd()
{
	POSITION pos = m_lstCtlAdditions.GetFirstSelectedItemPosition();
	if (NULL == pos)
		return;

	while (pos)
	{
		int iSelItem = m_lstCtlAdditions.GetNextSelectedItem(pos);

		CString strCode;
		CString strPrice;
		CString strComment;
		strCode = m_lstCtlAdditions.GetItemText(iSelItem, 1);
		strPrice = m_lstCtlAdditions.GetItemText(iSelItem, 2);
		strComment = m_lstCtlAdditions.GetItemText(iSelItem, 3);
		//
		BOOL bHasAdded = FALSE;
		int iAddedCnt = m_lstCtlSelected.GetItemCount();
		for (int iIndex = 0; iIndex < iAddedCnt; iIndex++)
		{
			CString strAddedCode = m_lstCtlSelected.GetItemText(iIndex, 1);
			if (strAddedCode == strCode)
			{
				bHasAdded = TRUE;
				break;
			}
		}
		if (TRUE == bHasAdded) continue;
		//
		CString strAddedIndex;
		strAddedIndex.Format(_T("%d"), iAddedCnt+1);
		m_lstCtlSelected.InsertItem(iAddedCnt, strAddedIndex);
		m_lstCtlSelected.SetItemText(iAddedCnt, 1, strCode);
		m_lstCtlSelected.SetItemText(iAddedCnt, 2, strPrice);
		m_lstCtlSelected.SetItemText(iAddedCnt, 3, strComment);
	}
}
//-----------------------------------------------------------------------
void CBMSelectAdditionsDlg::OnBnClickedBtnDelete()
{
	POSITION pos = m_lstCtlSelected.GetFirstSelectedItemPosition();
	if (NULL == pos)
		return;

	while (pos)
	{
		int iSelItem = m_lstCtlSelected.GetNextSelectedItem(pos);
		m_lstCtlSelected.DeleteItem(iSelItem);
		pos = m_lstCtlSelected.GetFirstSelectedItemPosition();
	}
}
//-----------------------------------------------------------------------
void CBMSelectAdditionsDlg::OnBnClickedOk()
{
	m_lstSelAdditions.clear();
	int iSelAddCnt = m_lstCtlSelected.GetItemCount();
	for (int iIndex = 0; iIndex < iSelAddCnt; iIndex++)
	{
		CString strAddCode;
		strAddCode = m_lstCtlSelected.GetItemText(iIndex, 1);
		m_lstSelAdditions.push_back(strAddCode);
	}

	OnOK();
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMSelectAdditionsDlg::SetSelAdditions(StringList lstSelNames)
{
	m_lstSelAdditions = lstSelNames;
}
//-----------------------------------------------------------------------
void CBMSelectAdditionsDlg::GetSelAdditions(StringList &lstSelNames)
{
	lstSelNames = m_lstSelAdditions;
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CBMSelectAdditionsDlg::InitTablesDataStruct()
{	
	// "AdditionInfo" table
	m_stuAdditionTable.eID = eTableID_Additions;
	m_stuAdditionTable.strName = g_tcTableName[eTableID_Additions];
	m_stuAdditionTable.lstColumns.clear();
	m_stuAdditionTable.lstColumns.push_back(TableColumn(TRUE,  50,  L"Index",		IDS_RECORD_INDEX,			eTableDataType_DWORD));
	m_stuAdditionTable.lstColumns.push_back(TableColumn(FALSE, 120, L"Code",		IDS_ADDITIONLIST_NAME,		eTableDataType_String));
	m_stuAdditionTable.lstColumns.push_back(TableColumn(FALSE, 100,  L"Price",		IDS_ADDITIONLIST_PRICE,		eTableDataType_DWORD));
	m_stuAdditionTable.lstColumns.push_back(TableColumn(FALSE, 150, L"Comment",		IDS_ADDITIONLIST_COMMENT,	eTableDataType_String));
}
//-----------------------------------------------------------------------
void CBMSelectAdditionsDlg::InitDataLstControls()
{
	m_lstCtlAdditions.SetExtendedStyle(m_lstCtlAdditions.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_lstCtlAdditions.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstCtlAdditions.SetImageList(&m_imgNULL, LVSIL_SMALL);
	m_lstCtlAdditions.DeleteAllItems();
	InitDataListCtrl(&m_lstCtlAdditions, m_stuAdditionTable);
	//
	m_lstCtlSelected.SetExtendedStyle(m_lstCtlSelected.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_lstCtlSelected.SetFullItemBKColor(RGB(214, 231, 255), RGB(94, 231, 221));
	m_lstCtlSelected.SetImageList(&m_imgNULL, LVSIL_SMALL);
	m_lstCtlSelected.DeleteAllItems();
	InitDataListCtrl(&m_lstCtlSelected, m_stuAdditionTable);
}
//-----------------------------------------------------------------------
void CBMSelectAdditionsDlg::FillAdditionLists()
{
	// Fill all addition list
	BOOL bResult = TRUE;
	CADOTable tbAdditions;	
	CStringW strOpenOperation;
	switch(m_eAdditionType)
	{
	case eAdditionType_Hoist:
		strOpenOperation.Format(L"SELECT * FROM %s WHERE Code LIKE '%s%s'", m_stuAdditionTable.strName, _T("Hoist"), _T("%"));
		break;
	case eAdditionType_EndCar:
		strOpenOperation.Format(L"SELECT * FROM %s WHERE Code LIKE '%s%s'", m_stuAdditionTable.strName, _T("EC"), _T("%"));
		break;
	case eAdditionType_Elec:
		strOpenOperation.Format(L"SELECT * FROM %s WHERE Code LIKE '%s%s'", m_stuAdditionTable.strName, _T("EL"), _T("%"));
		break;
	default:
		strOpenOperation.Format(L"SELECT * FROM %s", m_stuAdditionTable.strName);
		break;
	}
	bResult = tbAdditions.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	m_lstCtlAdditions.DeleteAllItems();
	m_lstCtlSelected.DeleteAllItems();

	DWORD dwSelIndex = 0;
	DWORD dwAdditionCnt = tbAdditions.GetRecordCnt();
	for (DWORD dwIndex = 0; dwIndex < dwAdditionCnt; dwIndex++)
	{
		CString strAddCode;
		CString strAddComment;
		DWORD dwAddPrice = 0;
		tbAdditions.GetRecordCell(dwIndex, _T("Code"), strAddCode.GetBuffer(32), 32);
		tbAdditions.GetRecordCell(dwIndex, _T("Price"), dwAddPrice);
		tbAdditions.GetRecordCell(dwIndex, _T("Comment"), strAddComment.GetBuffer(64), 64);
		//
		CString strIndex;
		CString strPrice;
		strIndex.Format(_T("%d"), dwIndex+1);
		strPrice.Format(_T("%d"), dwAddPrice);
		m_lstCtlAdditions.InsertItem(dwIndex, strIndex);
		m_lstCtlAdditions.SetItemText(dwIndex, 1, strAddCode);
		m_lstCtlAdditions.SetItemText(dwIndex, 2, strPrice);
		m_lstCtlAdditions.SetItemText(dwIndex, 3, strAddComment);
		//
		for (StringList::iterator it = m_lstSelAdditions.begin();
			 it != m_lstSelAdditions.end();
			 it++)
		{
			if (strAddCode == *it)
			{
				strIndex.Format(_T("%d"), dwSelIndex+1);
				m_lstCtlSelected.InsertItem(dwSelIndex, strIndex);
				m_lstCtlSelected.SetItemText(dwSelIndex, 1, strAddCode);
				m_lstCtlSelected.SetItemText(dwSelIndex, 2, strPrice);
				m_lstCtlSelected.SetItemText(dwSelIndex, 3, strAddComment);
				dwSelIndex++;
				break;
			}
		}
	}

	tbAdditions.CloseTable();
}
//-----------------------------------------------------------------------
