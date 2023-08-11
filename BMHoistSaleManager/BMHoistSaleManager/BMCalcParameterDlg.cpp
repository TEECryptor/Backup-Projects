//-----------------------------------------------------------------------
//	File Name:BMCalcParameterDlg.cpp
//	Introduce:implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMCalcParameterDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMCalcParameterDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMCalcParameterDlg, CDialog)
CBMCalcParameterDlg::CBMCalcParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMCalcParameterDlg::IDD, pParent)
{

}
//-----------------------------------------------------------------------
CBMCalcParameterDlg::~CBMCalcParameterDlg()
{
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_STATIC_MG_H, m_staticMGH);
	DDX_Control(pDX, IDC_STATIC_MG_V1, m_staticMGV1);
	DDX_Control(pDX, IDC_STATIC_MG_V2, m_staticMGV2);
	DDX_Control(pDX, IDC_STATIC_MG_N, m_staticMGN);
	DDX_Control(pDX, IDC_STATIC_MG_V3, m_staticMGV3);
	DDX_Control(pDX, IDC_STATIC_MG_KPX, m_staticMGKPX);
	DDX_Control(pDX, IDC_STATIC_MG_KPZ, m_staticMGKPZ);
	DDX_Control(pDX, IDC_STATIC_MG_KBZ, m_staticMGKBZ);
	DDX_Control(pDX, IDC_STATIC_MG_R, m_staticMGR);	
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMCalcParameterDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_DEFAULT, &CBMCalcParameterDlg::OnBnClickedBtnDefault)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CBMCalcParameterDlg::OnBnClickedBtnSave)
	ON_EN_CHANGE(IDC_EDIT_MG_G2, &CBMCalcParameterDlg::OnEnChangeEditMgG2)
	ON_EN_CHANGE(IDC_EDIT_MG_C2, &CBMCalcParameterDlg::OnEnChangeEditMgC2)
	ON_EN_CHANGE(IDC_EDIT_MG_DH, &CBMCalcParameterDlg::OnEnChangeEditMgDh)
	ON_CBN_SELCHANGE(IDC_COMBO_MG_T6, &CBMCalcParameterDlg::OnCbnSelchangeComboMgT6)
	ON_CBN_SELCHANGE(IDC_COMBO_MG_T7, &CBMCalcParameterDlg::OnCbnSelchangeComboMgT7)
	ON_CBN_SELCHANGE(IDC_COMBO_MG_F, &CBMCalcParameterDlg::OnCbnSelchangeComboMgF)
	ON_EN_CHANGE(IDC_EDIT_MG_MD, &CBMCalcParameterDlg::OnEnChangeEditMgMd)
	ON_EN_CHANGE(IDC_EDIT_MG_LD, &CBMCalcParameterDlg::OnEnChangeEditMgLd)
	ON_EN_CHANGE(IDC_EDIT_MG_H, &CBMCalcParameterDlg::OnEnChangeEditMgH)
	ON_EN_CHANGE(IDC_EDIT_MG_V1, &CBMCalcParameterDlg::OnEnChangeEditMgV1)
	ON_EN_CHANGE(IDC_EDIT_MG_V2, &CBMCalcParameterDlg::OnEnChangeEditMgV2)
	ON_EN_CHANGE(IDC_EDIT_MG_N, &CBMCalcParameterDlg::OnEnChangeEditMgN)
	ON_EN_CHANGE(IDC_EDIT_MG_V3, &CBMCalcParameterDlg::OnEnChangeEditMgV3)
	ON_EN_CHANGE(IDC_EDIT_MG_KPX, &CBMCalcParameterDlg::OnEnChangeEditMgKpx)
	ON_EN_CHANGE(IDC_EDIT_MG_KPZ, &CBMCalcParameterDlg::OnEnChangeEditMgKpz)
	ON_EN_CHANGE(IDC_EDIT_MG_KBZ, &CBMCalcParameterDlg::OnEnChangeEditMgKbz)
	ON_EN_CHANGE(IDC_EDIT_MG_R, &CBMCalcParameterDlg::OnEnChangeEditMgR)
	ON_CBN_SELCHANGE(IDC_COMBO_MG_F_EC, &CBMCalcParameterDlg::OnCbnSelchangeComboMgFEc)
	ON_EN_CHANGE(IDC_EDIT_MG_RMB, &CBMCalcParameterDlg::OnEnChangeEditMgRmb)
	ON_EN_CHANGE(IDC_EDIT_MG_WD, &CBMCalcParameterDlg::OnEnChangeEditMgWd)
	ON_EN_CHANGE(IDC_EDIT_MG_EUR1, &CBMCalcParameterDlg::OnEnChangeEditMgEur1)
	ON_EN_CHANGE(IDC_EDIT_MG_EUR2, &CBMCalcParameterDlg::OnEnChangeEditMgEur2)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMCalcParameterDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMCalcParameterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bIsInitFinished = FALSE;
	m_staticMGH.SetColors(RGB(0, 255, 255));
	m_staticMGV1.SetColors(RGB(0, 255, 255));
	m_staticMGV2.SetColors(RGB(0, 255, 255));
	m_staticMGN.SetColors(RGB(0, 255, 255));
	m_staticMGV3.SetColors(RGB(0, 255, 255));
	m_staticMGKPX.SetColors(RGB(0, 255, 255));
	m_staticMGKPZ.SetColors(RGB(0, 255, 255));
	m_staticMGKBZ.SetColors(RGB(0, 255, 255));
	m_staticMGR.SetColors(RGB(0, 255, 255));	

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MG_T6);
	int iIndex = pComboBox->AddString(_T("6"));
	pComboBox->SetItemData(iIndex, 6);
	iIndex = pComboBox->AddString(_T("8"));
	pComboBox->SetItemData(iIndex, 8);
	iIndex = pComboBox->AddString(_T("10"));
	pComboBox->SetItemData(iIndex, 10);
	iIndex = pComboBox->AddString(_T("12"));
	pComboBox->SetItemData(iIndex, 12);
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MG_T7);
	iIndex = pComboBox->AddString(_T("6"));
	pComboBox->SetItemData(iIndex, 6);
	iIndex = pComboBox->AddString(_T("8"));
	pComboBox->SetItemData(iIndex, 8);
	iIndex = pComboBox->AddString(_T("10"));
	pComboBox->SetItemData(iIndex, 10);
	iIndex = pComboBox->AddString(_T("12"));
	pComboBox->SetItemData(iIndex, 12);
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MG_F);
	iIndex = pComboBox->AddString(_T("1/750"));
	pComboBox->SetItemData(iIndex, 750);
	iIndex = pComboBox->AddString(_T("1/800"));
	pComboBox->SetItemData(iIndex, 800);
	iIndex = pComboBox->AddString(_T("1/1000"));
	pComboBox->SetItemData(iIndex, 1000);
	pComboBox->SetCurSel(2);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MG_F_EC);
	iIndex = pComboBox->AddString(_T("1/750"));
	pComboBox->SetItemData(iIndex, 750);
	iIndex = pComboBox->AddString(_T("1/800"));
	pComboBox->SetItemData(iIndex, 800);
	iIndex = pComboBox->AddString(_T("1/1000"));
	pComboBox->SetItemData(iIndex, 1000);
	pComboBox->SetCurSel(1);

	InitParamDataStruct();
	InitDataFromDatabase();
	m_bIsInitFinished = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnBnClickedBtnDefault()
{
	for (CalcParamDataList::iterator it = m_lstCalcParams.begin(); it != m_lstCalcParams.end(); it++)
	{		
		if (it->strCurParamValue == it->strDefaultParam)
			continue;

		it->strCurParamValue = it->strDefaultParam;
		it->bIsChanged = TRUE;
		//
		TCHAR tcWndClassName[64];
		CWnd* pWnd = GetDlgItem(it->nCurValueCtrl);
		GetClassName(pWnd->m_hWnd, tcWndClassName, 64);
		if (_tcsicmp(tcWndClassName, _T("ComboBox")) == 0)
			SelectComboBoxItem((CComboBox*)pWnd, it->strCurParamValue);
		else
			SetDlgItemText(it->nCurValueCtrl, it->strCurParamValue);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnBnClickedBtnSave()
{
	BOOL bResult = TRUE;
	CADOTable tbCalcParams;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s", g_tcTableName[eTableID_CalcParams]);
	bResult = tbCalcParams.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	for (CalcParamDataList::iterator it = m_lstCalcParams.begin(); it != m_lstCalcParams.end(); it++)
	{		
		if (FALSE == it->bIsChanged)
			continue;
		
		DWORD dwRecordCnt = tbCalcParams.GetRecordCnt();
		for (int i = 0; i < dwRecordCnt; i++)
		{
			CString strParamCode;
			tbCalcParams.GetRecordCell(i, L"ParamName", strParamCode.GetBuffer(32), 32);
			if (it->strParamName == strParamCode)
			{
				tbCalcParams.SetRecordCell(i, L"Param2", it->strCurParamValue.GetBuffer(32));
				it->bIsChanged = FALSE;
				break;
			}
		}
	}

	if (TRUE == tbCalcParams.UpdateTable())
	{
		myMessageBox(IDS_MSGDLG_EDITSUCCESS, IDS_MSGDLG_SUCCESS, MB_OK | MB_ICONINFORMATION);
	}

	tbCalcParams.CloseTable();
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgG2()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_G2);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgC2()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_G2);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgDh()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_DH);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnCbnSelchangeComboMgT6()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_COMBO_MG_T6);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnCbnSelchangeComboMgT7()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_COMBO_MG_T7);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnCbnSelchangeComboMgF()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_COMBO_MG_F);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnCbnSelchangeComboMgFEc()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_COMBO_MG_F_EC);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgRmb()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_RMB);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgMd()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_MD);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgLd()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_LD);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgH()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_H);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgV1()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_V1);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgV2()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_V2);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgN()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_N);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgV3()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_V3);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgKpx()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_KPX);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgKpz()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_KPZ);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgKbz()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_KBZ);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgR()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_R);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgWd()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_WD);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgEur1()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_EUR1);
	}
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::OnEnChangeEditMgEur2()
{
	if (TRUE == m_bIsInitFinished)
	{
		SetParamHasChanged(IDC_EDIT_MG_EUR2);
	}
}
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
void CBMCalcParameterDlg::InitParamDataStruct()
{
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_G2"), eTableDataType_DWORD, IDC_STATIC_MG_G2, IDC_EDIT_MG_G2));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_c2"), eTableDataType_DWORD, IDC_STATIC_MG_C2, IDC_EDIT_MG_C2));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_dh"), eTableDataType_FLOAT, IDC_STATIC_MG_DH, IDC_EDIT_MG_DH));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_t6"), eTableDataType_DWORD, IDC_STATIC_MG_T6, IDC_COMBO_MG_T6));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_t7"), eTableDataType_DWORD, IDC_STATIC_MG_T7, IDC_COMBO_MG_T7));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_fmax"), eTableDataType_FLOAT, IDC_STATIC_MG_F, IDC_COMBO_MG_F));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_fmax2"), eTableDataType_FLOAT, IDC_STATIC_MG_F_EC, IDC_COMBO_MG_F_EC));	
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_RMB"), eTableDataType_DWORD, IDC_STATIC_MG_RMB, IDC_EDIT_MG_RMB));	
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_h"), eTableDataType_DWORD, IDC_STATIC_MG_H, IDC_EDIT_MG_H));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_v1"), eTableDataType_FLOAT, IDC_STATIC_MG_V1, IDC_EDIT_MG_V1));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_v2"), eTableDataType_FLOAT, IDC_STATIC_MG_V2, IDC_EDIT_MG_V2));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_n"), eTableDataType_FLOAT, IDC_STATIC_MG_N, IDC_EDIT_MG_N));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_v3"), eTableDataType_FLOAT, IDC_STATIC_MG_V3, IDC_EDIT_MG_V3));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_Kpx"), eTableDataType_FLOAT, IDC_STATIC_MG_KPX, IDC_EDIT_MG_KPX));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_Kpz"), eTableDataType_FLOAT, IDC_STATIC_MG_KPZ, IDC_EDIT_MG_KPZ));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_Kbz"), eTableDataType_FLOAT, IDC_STATIC_MG_KBZ, IDC_EDIT_MG_KBZ));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_r"), eTableDataType_FLOAT, IDC_STATIC_MG_R, IDC_EDIT_MG_R));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_md"), eTableDataType_FLOAT, IDC_STATIC_MG_MD, IDC_EDIT_MG_MD));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_ld"), eTableDataType_FLOAT, IDC_STATIC_MG_LD, IDC_EDIT_MG_LD));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_Wd"), eTableDataType_DWORD, IDC_STATIC_MG_WD, IDC_EDIT_MG_WD));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_Euro1"), eTableDataType_FLOAT, IDC_STATIC_MG_EUR1, IDC_EDIT_MG_EUR1));
	m_lstCalcParams.push_back(CalcParamData(_T("Calc_MG_Euro2"), eTableDataType_FLOAT, IDC_STATIC_MG_EUR2, IDC_EDIT_MG_EUR2));
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::InitDataFromDatabase()
{	
	BOOL bResult = TRUE;
	CADOTable tbCalcParams;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s", g_tcTableName[eTableID_CalcParams]);
	bResult = tbCalcParams.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return;

	for (CalcParamDataList::iterator it = m_lstCalcParams.begin(); it != m_lstCalcParams.end(); it++)
	{		
		DWORD dwRecordCnt = tbCalcParams.GetRecordCnt();
		for (int i = 0; i < dwRecordCnt; i++)
		{
			CString strParamCode;
			tbCalcParams.GetRecordCell(i, L"ParamName", strParamCode.GetBuffer(32), 32);
			if (it->strParamName == strParamCode)
			{
				tbCalcParams.GetRecordCell(i, L"Param1", it->strDefaultParam.GetBuffer(32), 32);
				tbCalcParams.GetRecordCell(i, L"Param2", it->strCurParamValue.GetBuffer(32), 32);
				//
				TCHAR tcWndClassName[64];
				SetDlgItemText(it->nDefaultCtrl, it->strDefaultParam);
				CWnd* pWnd = GetDlgItem(it->nCurValueCtrl);
				GetClassName(pWnd->m_hWnd, tcWndClassName, 64);
				if (_tcsicmp(tcWndClassName, _T("ComboBox")) == 0)
					SelectComboBoxItem((CComboBox*)pWnd, it->strCurParamValue);
				else
					SetDlgItemText(it->nCurValueCtrl, it->strCurParamValue);
				break;
			}
		}
	}

	tbCalcParams.CloseTable();
}
//-----------------------------------------------------------------------
void CBMCalcParameterDlg::SetParamHasChanged(LONG lCtrlID)
{
	for (CalcParamDataList::iterator it = m_lstCalcParams.begin(); it != m_lstCalcParams.end(); it++)
	{		
		if (it->nCurValueCtrl == lCtrlID)
		{
			GetDlgItemText(lCtrlID, it->strCurParamValue);
			it->bIsChanged = TRUE;
			break;
		}
	}
}
//-----------------------------------------------------------------------