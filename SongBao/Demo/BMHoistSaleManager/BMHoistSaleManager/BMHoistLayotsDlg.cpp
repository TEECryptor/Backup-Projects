//-----------------------------------------------------------------------
//	File Name:BMHoistLayotsDlg.cpp
//	Introduce:Class CBMHoistLayotsDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMHoistLayotsDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMHoistLayotsDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMHoistLayotsDlg, CDialog)
CBMHoistLayotsDlg::CBMHoistLayotsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMHoistLayotsDlg::IDD, pParent)
{
	m_pCraneParam = NULL;
	for (int i = 0; i < eHoistDrawType_Cnt; i++)
	{
		for (int j = 0; j < eLayoutViewType_Cnt; j++)
		{
			m_pHoistImage[i][j] = NULL;
		}
	}
	m_eCurDrawType = eHoistDrawType_D0;
	m_eCurViewType = eLayoutViewType_Main;

}
//-----------------------------------------------------------------------
CBMHoistLayotsDlg::~CBMHoistLayotsDlg()
{
	for (int i = 0; i < eHoistDrawType_Cnt; i++)
	{
		for (int j = 0; j < eLayoutViewType_Cnt; j++)
		{
			if (NULL != m_pHoistImage[i][j])
			{
				delete m_pHoistImage[i][j];
				m_pHoistImage[i][j] = NULL;
			}
		}
	}
}
//-----------------------------------------------------------------------
void CBMHoistLayotsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HOISTPARAM, m_ctrlDimHoistLst);	
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMHoistLayotsDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMHoistLayotsDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMHoistLayotsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Initiliaze lists
	m_imgNULL.Create(1, 18, TRUE | ILC_COLOR32, 1, 0); 
	for (int i = 0; i < eHoistDrawType_Cnt; i++)
	{
		for (int j = 0; j < eLayoutViewType_Cnt; j++)
		{
			m_pHoistImage[i][j] = new CImage();
		}
	}
	m_pHoistImage[eHoistDrawType_S][eLayoutViewType_Main]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_S_MAIN);
	m_pHoistImage[eHoistDrawType_S][eLayoutViewType_Side]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_S_SIDE);
	m_pHoistImage[eHoistDrawType_S][eLayoutViewType_Top]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_S_TOP);
	m_pHoistImage[eHoistDrawType_D0][eLayoutViewType_Main]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D0_MAIN);
	m_pHoistImage[eHoistDrawType_D0][eLayoutViewType_Side]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D0_SIDE);
	m_pHoistImage[eHoistDrawType_D0][eLayoutViewType_Top]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D0_TOP);
	m_pHoistImage[eHoistDrawType_D1][eLayoutViewType_Main]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D1_MAIN);
	m_pHoistImage[eHoistDrawType_D1][eLayoutViewType_Side]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D1_SIDE);
	m_pHoistImage[eHoistDrawType_D1][eLayoutViewType_Top]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D1_TOP);
	m_pHoistImage[eHoistDrawType_D2][eLayoutViewType_Main]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D2_MAIN);
	m_pHoistImage[eHoistDrawType_D2][eLayoutViewType_Side]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D2_SIDE);
	m_pHoistImage[eHoistDrawType_D2][eLayoutViewType_Top]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D2_TOP);
	m_pHoistImage[eHoistDrawType_D3][eLayoutViewType_Main]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D3_MAIN);
	m_pHoistImage[eHoistDrawType_D3][eLayoutViewType_Side]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D3_SIDE);
	m_pHoistImage[eHoistDrawType_D3][eLayoutViewType_Top]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D3_TOP);
	m_pHoistImage[eHoistDrawType_D4][eLayoutViewType_Main]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D4_MAIN);
	m_pHoistImage[eHoistDrawType_D4][eLayoutViewType_Side]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D4_SIDE);
	m_pHoistImage[eHoistDrawType_D4][eLayoutViewType_Top]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_D4_TOP);
	m_pHoistImage[eHoistDrawType_F0][eLayoutViewType_Main]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F0_MAIN);
	m_pHoistImage[eHoistDrawType_F0][eLayoutViewType_Side]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F0_SIDE);
	m_pHoistImage[eHoistDrawType_F0][eLayoutViewType_Top]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F0_TOP);
	m_pHoistImage[eHoistDrawType_F1][eLayoutViewType_Main]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F1_MAIN);
	m_pHoistImage[eHoistDrawType_F1][eLayoutViewType_Side]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F1_SIDE);
	m_pHoistImage[eHoistDrawType_F1][eLayoutViewType_Top]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F1_TOP);
	m_pHoistImage[eHoistDrawType_F3][eLayoutViewType_Main]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F3_MAIN);
	m_pHoistImage[eHoistDrawType_F3][eLayoutViewType_Side]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F3_SIDE);
	m_pHoistImage[eHoistDrawType_F3][eLayoutViewType_Top]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F3_TOP);
	m_pHoistImage[eHoistDrawType_F4][eLayoutViewType_Main]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F4_MAIN);
	m_pHoistImage[eHoistDrawType_F4][eLayoutViewType_Side]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F4_SIDE);
	m_pHoistImage[eHoistDrawType_F4][eLayoutViewType_Top]->LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_HOST_F4_TOP);

	InitTableDataStruct();

	m_ctrlDimHoistLst.EnableWindow(FALSE);
	m_ctrlDimHoistLst.SetExtendedStyle(m_ctrlDimHoistLst.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_ctrlDimHoistLst.SetFullItemBKColor(RGB(255, 255, 255), RGB(255, 255, 255));
	m_ctrlDimHoistLst.SetImageList(&m_imgNULL, LVSIL_SMALL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMHoistLayotsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);
	if (NULL != m_ctrlDimHoistLst.m_hWnd)
	{
		CRect rcParamLst;
		rcParamLst.left = 4;
		rcParamLst.right = rcClient.right - 4;
		rcParamLst.top = 205;
		rcParamLst.bottom = rcClient.bottom - 4;
		m_ctrlDimHoistLst.MoveWindow(&rcParamLst);
	}
}
//-----------------------------------------------------------------------
void CBMHoistLayotsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(&rcClient);	
	if (NULL != m_pHoistImage[m_eCurDrawType][m_eCurViewType])
	{
		CSize sizeImage(m_pHoistImage[m_eCurDrawType][m_eCurViewType]->GetWidth(), m_pHoistImage[m_eCurDrawType][m_eCurViewType]->GetHeight());
		m_pHoistImage[m_eCurDrawType][m_eCurViewType]->StretchBlt(	dc.m_hDC,	
																	(rcClient.Width()-sizeImage.cx)/2, 
																	0, 
																	sizeImage.cx, 
																	sizeImage.cy);
	}
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMHoistLayotsDlg::SetCraneParamObject(CBMParamCalculator* pCraneParam)
{
	m_pCraneParam = pCraneParam;
}
//-----------------------------------------------------------------------
void CBMHoistLayotsDlg::SetCurrentViewType(eLayoutViewType eViewType)
{
	if (m_pCraneParam->m_strHoiDrawCode == CString(_T("BM341S")) ||
		m_pCraneParam->m_strHoiDrawCode == CString(_T("BM441S")) ||
		m_pCraneParam->m_strHoiDrawCode == CString(_T("BM541S")))
	{
		m_eCurDrawType = eHoistDrawType_S;
	}
	else if (m_pCraneParam->m_strHoiDrawCode == CString(_T("BM341D")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM441D")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM541D")))
	{
		m_eCurDrawType = eHoistDrawType_D0;
	}
	else if (m_pCraneParam->m_strHoiDrawCode == CString(_T("BM321D")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM521D")))
	{
		m_eCurDrawType = eHoistDrawType_D1;
	}
	else if (m_pCraneParam->m_strHoiDrawCode == CString(_T("BM641D")))
	{
		m_eCurDrawType = eHoistDrawType_D2;
	}
	else if (m_pCraneParam->m_strHoiDrawCode == CString(_T("BM642D")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM742D")))
	{
		m_eCurDrawType = eHoistDrawType_D3;
	}
	else if (m_pCraneParam->m_strHoiDrawCode == CString(_T("BM862D")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM942D")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM962D")))
	{
		m_eCurDrawType = eHoistDrawType_D4;
	}
	else if (m_pCraneParam->m_strHoiDrawCode == CString(_T("BM341F")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM441F")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM541F")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM641F")))
	{
		m_eCurDrawType = eHoistDrawType_F0;
	}
	else if (m_pCraneParam->m_strHoiDrawCode == CString(_T("BM321F")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM521F")))
	{
		m_eCurDrawType = eHoistDrawType_F1;
	}
	else if (m_pCraneParam->m_strHoiDrawCode == CString(_T("BM642F")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM742F")))
	{
		m_eCurDrawType = eHoistDrawType_F3;
	}
	else if (m_pCraneParam->m_strHoiDrawCode == CString(_T("BM842F")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM942F")) ||
			 m_pCraneParam->m_strHoiDrawCode == CString(_T("BM962F")))
	{
		m_eCurDrawType = eHoistDrawType_F4;
	}
	m_eCurViewType = eViewType;
	Invalidate();	

	InitDataListCtrl(&m_ctrlDimHoistLst, m_stuDimHoistTables[eViewType][m_pCraneParam->m_eCraneType]);
	if (m_pCraneParam->m_strHoistCode.IsEmpty())
		return;

	BOOL bResult = TRUE;
	CADOTable tbDimHoiSel;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE HoiDimIndCod='%s'", m_stuDimHoistTables[eViewType][m_pCraneParam->m_eCraneType].strName, m_pCraneParam->m_strHoiDimIndCode);
	bResult = tbDimHoiSel.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	TCHAR tcH2[32] = {0};
	tbDimHoiSel.GetRecordCell(0, L"WgtHoi", (DWORD&)m_pCraneParam->m_iHoist_G1);
	tbDimHoiSel.GetRecordCell(0, L"b1", (DWORD&)m_pCraneParam->m_iHoist_b1);
	tbDimHoiSel.GetRecordCell(0, L"b2", (DWORD&)m_pCraneParam->m_iHoist_b2);
	tbDimHoiSel.GetRecordCell(0, L"b", (DWORD&)m_pCraneParam->m_iHoist_b);
	tbDimHoiSel.GetRecordCell(0, L"h2", tcH2, 32);
	m_pCraneParam->m_iHoist_H2 = _ttol(tcH2);

	ShowTableRecords(&tbDimHoiSel, &m_ctrlDimHoistLst, m_stuDimHoistTables[eViewType][m_pCraneParam->m_eCraneType]);
	tbDimHoiSel.CloseTable();
}
//-----------------------------------------------------------------------
void CBMHoistLayotsDlg::InitTableDataStruct()
{
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTD].eID = eTableID_DimHoi_D;
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTD].strName = g_tcTableName[eTableID_DimHoi_D];
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"W",	IDS_DIMHOIST_W,	eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"R",	IDS_DIMHOIST_R, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 35,  L"W1",	IDS_DIMHOIST_W1,eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 40,  L"H2",	IDS_DIMHOIST_H2,eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 35,  L"H3",	IDS_DIMHOIST_H3,eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 40,  L"H4",	IDS_DIMHOIST_H4,eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 35,  L"d",	IDS_DIMHOIST_D, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 35,  L"D1",	IDS_DIMHOIST_D2,eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 60,  L"rw",	IDS_DIMHOIST_RW,eTableDataType_String));
	//
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTD].eID = eTableID_DimHoi_D;
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTD].strName = g_tcTableName[eTableID_DimHoi_D];
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"L",	IDS_DIMHOIST_L,	 eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"b1",	IDS_DIMHOIST_B1, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"b2",	IDS_DIMHOIST_B2, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"C",	IDS_DIMHOIST_C,  eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"H1",	IDS_DIMHOIST_H1, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"Rh",	IDS_DIMHOIST_RH, eTableDataType_String));
	//
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTD].eID = eTableID_DimHoi_D;
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTD].strName = g_tcTableName[eTableID_DimHoi_D];
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"b",	IDS_DIMHOIST_B,	 eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"m1",	IDS_DIMHOIST_M1, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"m2",	IDS_DIMHOIST_M2, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"m3",	IDS_DIMHOIST_M3, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"L1",	IDS_DIMHOIST_L1, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTD].lstColumns.push_back(TableColumn(FALSE, 50,  L"S4",	IDS_DIMHOIST_S4, eTableDataType_String));


	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTS].eID = eTableID_DimHoi_S;
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTS].strName = g_tcTableName[eTableID_DimHoi_S];
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 50,  L"W",	IDS_DIMHOIST_W,	eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 50,  L"S1",	IDS_DIMHOIST_S1,eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 50,  L"S2",	IDS_DIMHOIST_S2,eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 50,  L"H2",	IDS_DIMHOIST_H2,eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 50,  L"H3",	IDS_DIMHOIST_H3,eTableDataType_String));
	//
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTS].eID = eTableID_DimHoi_S;
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTS].strName = g_tcTableName[eTableID_DimHoi_S];
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 35,  L"S3",	IDS_DIMHOIST_S3, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 35,  L"S4",	IDS_DIMHOIST_S4, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 35,  L"B0",	IDS_DIMHOIST_B0, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 35,  L"H1",	IDS_DIMHOIST_H1, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 35,  L"H4",	IDS_DIMHOIST_H4, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 35,  L"C",	IDS_DIMHOIST_C,	 eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 35,  L"K1",	IDS_DIMHOIST_K1, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 35,  L"K2",	IDS_DIMHOIST_K2, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 35,  L"k3",	IDS_DIMHOIST_K3, eTableDataType_String));
	//
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTS].eID = eTableID_DimHoi_S;
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTS].strName = g_tcTableName[eTableID_DimHoi_S];
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 50,  L"b",	IDS_DIMHOIST_B,	 eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 50,  L"m2",	IDS_DIMHOIST_M2, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 50,  L"D",	IDS_DIMHOIST_D,	 eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 50,  L"S5",	IDS_DIMHOIST_S5,  eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTS].lstColumns.push_back(TableColumn(FALSE, 50,  L"S6",	IDS_DIMHOIST_S6, eTableDataType_String));


	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTF].eID = eTableID_DimHoi_F;
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTF].strName = g_tcTableName[eTableID_DimHoi_F];
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 50,  L"W",	IDS_DIMHOIST_W,	eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 50,  L"R",	IDS_DIMHOIST_R, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 35,  L"W1",	IDS_DIMHOIST_W1,eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 40,  L"H2",	IDS_DIMHOIST_H2,eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 35,  L"H3",	IDS_DIMHOIST_H3,eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Main][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 40,  L"H4",	IDS_DIMHOIST_H4,eTableDataType_String));
	//
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTF].eID = eTableID_DimHoi_F;
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTF].strName = g_tcTableName[eTableID_DimHoi_F];
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 50,  L"L",	IDS_DIMHOIST_L,	 eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 50,  L"b1",	IDS_DIMHOIST_B1, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 50,  L"b2",	IDS_DIMHOIST_B2, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 50,  L"C",	IDS_DIMHOIST_C,  eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Side][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 50,  L"H1",	IDS_DIMHOIST_H1, eTableDataType_String));
	//
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].eID = eTableID_DimHoi_F;
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].strName = g_tcTableName[eTableID_DimHoi_F];
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 35,  L"m1",	IDS_DIMHOIST_M1, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 40,  L"m2",	IDS_DIMHOIST_M2, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 35,  L"m3",	IDS_DIMHOIST_M3, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 30,  L"L1",	IDS_DIMHOIST_L1, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 30,  L"S4",	IDS_DIMHOIST_S4, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 30,  L"J1",	IDS_DIMHOIST_J1, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 30,  L"J2",	IDS_DIMHOIST_J2, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 30,  L"J3",	IDS_DIMHOIST_J3, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 30,  L"J4",	IDS_DIMHOIST_J4, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 30,  L"J5",	IDS_DIMHOIST_J5, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 30,  L"X1",	IDS_DIMHOIST_X1, eTableDataType_String));
	m_stuDimHoistTables[eLayoutViewType_Top][eCraneType_GTF].lstColumns.push_back(TableColumn(FALSE, 30,  L"X2",	IDS_DIMHOIST_X2, eTableDataType_String));

	
	m_stuDimHoistTables[eLayoutViewType_Main][3].eID = eTableID_DimHoi_Z;
	m_stuDimHoistTables[eLayoutViewType_Main][3].strName = g_tcTableName[eTableID_DimHoi_Z];
	m_stuDimHoistTables[eLayoutViewType_Main][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"W",		IDS_DIMHOIST_W,	eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Main][3].lstColumns.push_back(TableColumn(FALSE, 30,  L"W1",	IDS_DIMHOIST_W1,eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Main][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"H2",	IDS_DIMHOIST_H2,eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Main][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"H3",	IDS_DIMHOIST_H3,eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Main][3].lstColumns.push_back(TableColumn(FALSE, 30,  L"H4",	IDS_DIMHOIST_H4,eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Main][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"R",		IDS_DIMHOIST_R, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Main][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"d",		IDS_DIMHOIST_D,eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Main][3].lstColumns.push_back(TableColumn(FALSE, 30,  L"d2",	IDS_DIMHOIST_D2,eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Main][3].lstColumns.push_back(TableColumn(FALSE, 30,  L"rw",	IDS_DIMHOIST_RW,eTableDataType_DWORD));
	//
	m_stuDimHoistTables[eLayoutViewType_Side][3].eID = eTableID_DimHoi_Z;
	m_stuDimHoistTables[eLayoutViewType_Side][3].strName = g_tcTableName[eTableID_DimHoi_Z];
	m_stuDimHoistTables[eLayoutViewType_Side][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"L",		IDS_DIMHOIST_L,	 eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Side][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"b1",	IDS_DIMHOIST_B1, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Side][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"b2",	IDS_DIMHOIST_B2, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Side][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"C",		IDS_DIMHOIST_C,  eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Side][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"H1",	IDS_DIMHOIST_H1, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Side][3].lstColumns.push_back(TableColumn(FALSE, 30,  L"Rh",	IDS_DIMHOIST_RH, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Side][3].lstColumns.push_back(TableColumn(FALSE, 30,  L"Rs",	IDS_DIMHOIST_RS, eTableDataType_DWORD));
	//
	m_stuDimHoistTables[eLayoutViewType_Top][3].eID = eTableID_DimHoi_Z;
	m_stuDimHoistTables[eLayoutViewType_Top][3].strName = g_tcTableName[eTableID_DimHoi_Z];
	m_stuDimHoistTables[eLayoutViewType_Top][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"b",	IDS_DIMHOIST_B,	 eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Top][3].lstColumns.push_back(TableColumn(FALSE, 30,  L"m1",	IDS_DIMHOIST_M1, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Top][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"m2",	IDS_DIMHOIST_M2, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Top][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"m3",	IDS_DIMHOIST_M3,  eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Top][3].lstColumns.push_back(TableColumn(FALSE, 30,  L"m4",	IDS_DIMHOIST_M4, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Top][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"m5",	IDS_DIMHOIST_M5, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Top][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"m6",	IDS_DIMHOIST_M6, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Top][3].lstColumns.push_back(TableColumn(FALSE, 50,  L"L1",	IDS_DIMHOIST_L1, eTableDataType_DWORD));
	m_stuDimHoistTables[eLayoutViewType_Top][3].lstColumns.push_back(TableColumn(FALSE, 30,  L"S4",	IDS_DIMHOIST_S4, eTableDataType_DWORD));

}
//-----------------------------------------------------------------------