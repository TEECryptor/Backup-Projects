//-----------------------------------------------------------------------
//	File Name:BMOrderOptionDlg.cpp
//	Introduce:Class CBMOrderOptionDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOrderOptionDlg.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOrderOptionDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOrderOptionDlg, CDialog)
CBMOrderOptionDlg::CBMOrderOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOrderOptionDlg::IDD, pParent)
{
	m_pCraneCalculator = NULL;
}
//-----------------------------------------------------------------------
CBMOrderOptionDlg::~CBMOrderOptionDlg()
{
	if (NULL != m_pCraneCalculator)
	{
		delete m_pCraneCalculator;
		m_pCraneCalculator = NULL;
	}
}
//-----------------------------------------------------------------------
void CBMOrderOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOrderOptionDlg, CDialog)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOrderOptionDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOrderOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitDataControls();

	SetDlgItemText(IDC_EDIT_ORDERDLG_COMNAME, m_strComName);
	SetDlgItemText(IDC_EDIT_ORDERDLG_COMTYPE, m_pCraneCalculator->m_bOnlyHoist ? STR_COMTYPE_HOIST : STR_COMTYPE_PACKAGE);
	SetDlgItemText(IDC_EDIT_ORDERDLG_COMQTY, _T("1"));
	//
	GetDlgItem(IDC_EDIT_ORDERDLG_COMNOTE)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
BOOL CBMOrderOptionDlg::SetOfferNoAndComName(CString strOfferNo, CString strComName)
{
	m_strOfferNo = strOfferNo;
	m_strComName = strComName;

	// Use calculator object to load offer data from database
	if (NULL == m_pCraneCalculator)
	{
		m_pCraneCalculator = new CBMParamCalculator();
	}
	m_pCraneCalculator->LoadDataFromDatabase(m_strOfferNo, m_strComName);
	m_pCraneCalculator->LoadOtherParamsFromDatabase();
	if (!m_pCraneCalculator->m_bOnlyHoist)
	{
		m_pCraneCalculator->CalcMainGirder();
		m_pCraneCalculator->CalcEndCar();
	}

	return TRUE;
}
//-----------------------------------------------------------------------
void CBMOrderOptionDlg::GetGeneralInfo(CString &strComName, 
									   CString &strComType, 
									   int &iComCount, 
									   CString &strComNote, 
									   CString &strComAdditional)
{
	CString strCount;
	GetDlgItemText(IDC_EDIT_ORDERDLG_COMNAME, strComName);
	GetDlgItemText(IDC_EDIT_ORDERDLG_COMTYPE, strComType);
	GetDlgItemText(IDC_EDIT_ORDERDLG_COMQTY, strCount);
	GetDlgItemText(IDC_EDIT_ORDERDLG_COMNOTE, strComNote);
	iComCount = _ttol(strCount);
	//
	TechDataList lstStdHoistAdditions;
	TechDataList lstSelHoistAdditions;
	TechDataList lstSelCraneAdditions;
	m_pCraneCalculator->GetHoistStdAddTechData(lstStdHoistAdditions);
	m_pCraneCalculator->GetHoistSelAddTechData(lstSelHoistAdditions);
	m_pCraneCalculator->GetCraneSelAddTechData(lstSelCraneAdditions);
	for (TechDataList::iterator it = lstStdHoistAdditions.begin();
		 it != lstStdHoistAdditions.end();
		 it++)
	{

		TechData data = *it;
		strComAdditional += data.strPrintText_CN;
		strComAdditional += _T("\r\n");
	}	
	for (TechDataList::iterator it = lstSelHoistAdditions.begin();
		 it != lstSelHoistAdditions.end();
		 it++)
	{

		TechData data = *it;
		strComAdditional += data.strPrintText_CN;
		strComAdditional += _T("\r\n");
	}	
	for (TechDataList::iterator it = lstSelCraneAdditions.begin();
		 it != lstSelCraneAdditions.end();
		 it++)
	{

		TechData data = *it;
		strComAdditional += data.strPrintText_CN;
		strComAdditional += _T("\r\n");
	}	
}
//-----------------------------------------------------------------------
BOOL CBMOrderOptionDlg::GetComponentInfo(eComponentType eType, 
										 CString &strCode, 
										 int &iCount, 
										 CString &strFreeText)
{
	if (eComponentType_MainGirder == eType)
		return FALSE;

	if (m_pCraneCalculator->m_bOnlyHoist && eComponentType_Hoist != eType)
		return FALSE;
	
	if (BST_UNCHECKED == ((CButton*)GetDlgItem(m_DetailOrderCtrls[eType].nCheckBox))->GetCheck())
		return FALSE;

	CString strCount;
	GetDlgItemText(m_DetailOrderCtrls[eType].nCodeEdit, strCode);
	GetDlgItemText(m_DetailOrderCtrls[eType].nQtyEdit, strCount);
	GetDlgItemText(m_DetailOrderCtrls[eType].nFreeTextEdit, strFreeText);
	iCount = _ttol(strCount);
	return TRUE;
}
//-----------------------------------------------------------------------
CBMParamCalculator*	CBMOrderOptionDlg::GetCalculator()
{
	return m_pCraneCalculator;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOrderOptionDlg::InitDataControls()
{
	// Initliaze controls arrary
	m_DetailOrderCtrls[eComponentType_Hoist].SetDialog(m_hWnd);
	m_DetailOrderCtrls[eComponentType_Hoist].nCheckBox = IDC_CHECK_ORDERDLG_HOIST;
	m_DetailOrderCtrls[eComponentType_Hoist].nQtyEdit = IDC_EDIT_ORDERDLG_HOIST_CNT;
	m_DetailOrderCtrls[eComponentType_Hoist].nCodeEdit = IDC_EDIT_ORDERDLG_HOIST;
	m_DetailOrderCtrls[eComponentType_Hoist].nFreeTextEdit = IDC_EDIT_ORDERDLG_HOIST_FREETEXT;
	//
	m_DetailOrderCtrls[eComponentType_EndCar].SetDialog(m_hWnd);
	m_DetailOrderCtrls[eComponentType_EndCar].nCheckBox = IDC_CHECK_ORDERDLG_ENDCAR;
	m_DetailOrderCtrls[eComponentType_EndCar].nQtyEdit = IDC_EDIT_ORDERDLG_ENDCAR_CNT;
	m_DetailOrderCtrls[eComponentType_EndCar].nCodeEdit = IDC_EDIT_ORDERDLG_ENDCAR;
	m_DetailOrderCtrls[eComponentType_EndCar].nFreeTextEdit = IDC_EDIT_ORDERDLG_ENDCAR_FREETEXT;
	//
	m_DetailOrderCtrls[eComponentType_BridgeDri].SetDialog(m_hWnd);
	m_DetailOrderCtrls[eComponentType_BridgeDri].nCheckBox = IDC_CHECK_ORDERDLG_BRIDGEDRI;
	m_DetailOrderCtrls[eComponentType_BridgeDri].nQtyEdit = IDC_EDIT_ORDERDLG_BRIDGEDRI_CNT;
	m_DetailOrderCtrls[eComponentType_BridgeDri].nCodeEdit = IDC_EDIT_ORDERDLG_BRIDGEDRI;
	m_DetailOrderCtrls[eComponentType_BridgeDri].nFreeTextEdit = IDC_EDIT_ORDERDLG_BRIDGEDRI_FREETEXT;
	//
	m_DetailOrderCtrls[eComponentType_BridgePannel].SetDialog(m_hWnd);
	m_DetailOrderCtrls[eComponentType_BridgePannel].nCheckBox = IDC_CHECK_ORDERDLG_BRIDGEPANNEL;
	m_DetailOrderCtrls[eComponentType_BridgePannel].nQtyEdit = IDC_EDIT_ORDERDLG_BRIDGEPANNEL_CNT;
	m_DetailOrderCtrls[eComponentType_BridgePannel].nCodeEdit = IDC_EDIT_ORDERDLG_BRIDGEPANNEL;
	m_DetailOrderCtrls[eComponentType_BridgePannel].nFreeTextEdit = IDC_EDIT_ORDERDLG_BRIDGEPANNEL_FREETEXT;
	//
	m_DetailOrderCtrls[eComponentType_Fastoon].SetDialog(m_hWnd);
	m_DetailOrderCtrls[eComponentType_Fastoon].nCheckBox = IDC_CHECK_ORDERDLG_FASTOON;
	m_DetailOrderCtrls[eComponentType_Fastoon].nQtyEdit = IDC_EDIT_ORDERDLG_FASTOON_CNT;
	m_DetailOrderCtrls[eComponentType_Fastoon].nCodeEdit = IDC_EDIT_ORDERDLG_FASTOON;
	m_DetailOrderCtrls[eComponentType_Fastoon].nFreeTextEdit = IDC_EDIT_ORDERDLG_FASTOON_FREETEXT;
	//
	m_DetailOrderCtrls[eComponentType_OperateUnit].SetDialog(m_hWnd);
	m_DetailOrderCtrls[eComponentType_OperateUnit].nCheckBox = IDC_CHECK_ORDERDLG_OPERATEUNIT;
	m_DetailOrderCtrls[eComponentType_OperateUnit].nQtyEdit = IDC_EDIT_ORDERDLG_OPERATEUNIT_CNT;
	m_DetailOrderCtrls[eComponentType_OperateUnit].nCodeEdit = IDC_EDIT_ORDERDLG_OPERATEUNIT;
	m_DetailOrderCtrls[eComponentType_OperateUnit].nFreeTextEdit = IDC_EDIT_ORDERDLG_OPERATEUNIT_FREETEXT;
	//
	m_DetailOrderCtrls[eComponentType_CranePower].SetDialog(m_hWnd);
	m_DetailOrderCtrls[eComponentType_CranePower].nCheckBox = IDC_CHECK_ORDERDLG_CRANEPOWER;
	m_DetailOrderCtrls[eComponentType_CranePower].nQtyEdit = IDC_EDIT_ORDERDLG_CRANEPOWER_CNT;
	m_DetailOrderCtrls[eComponentType_CranePower].nCodeEdit = IDC_EDIT_ORDERDLG_CRANEPOWER;
	m_DetailOrderCtrls[eComponentType_CranePower].nFreeTextEdit = IDC_EDIT_ORDERDLG_CRANEPOWER_FREETEXT;	
	//

	CString strHoistFreeText;
	if (eCraneType_GTS == m_pCraneCalculator->m_eCraneType)
		strHoistFreeText.Format(_T("%d"), m_pCraneCalculator->m_iGirder_w1);
	else
		strHoistFreeText.Format(_T("%s"), m_pCraneCalculator->m_strRaiGauge);
	m_DetailOrderCtrls[eComponentType_Hoist].SetInitData(m_pCraneCalculator->m_iNumOfHoist, m_pCraneCalculator->m_strHoistCode, strHoistFreeText);
	if (!m_pCraneCalculator->m_bOnlyHoist)
	{
		CString strEndCarFreeText;
		strEndCarFreeText.Format(_T("%d/%s/Q%d"), m_pCraneCalculator->GetWheelBase(), m_pCraneCalculator->m_strECStructure, m_pCraneCalculator->m_iEndCar_Matrial);
		m_DetailOrderCtrls[eComponentType_EndCar].SetInitData(2, m_pCraneCalculator->GetEndCarDataText(), strEndCarFreeText);
		m_DetailOrderCtrls[eComponentType_BridgeDri].SetInitData(m_pCraneCalculator->m_iNumOfMotor, m_pCraneCalculator->m_strDriverCode);
		m_DetailOrderCtrls[eComponentType_BridgePannel].SetInitData(1, m_pCraneCalculator->m_strCraPannelCode);
		m_DetailOrderCtrls[eComponentType_Fastoon].SetInitData(1, m_pCraneCalculator->m_strFastoonCode);
		m_DetailOrderCtrls[eComponentType_OperateUnit].SetInitData(1, m_pCraneCalculator->m_strOperterUnitCode);
		m_DetailOrderCtrls[eComponentType_CranePower].SetInitData(1, m_pCraneCalculator->m_strCraPowSupplyCode);
	}
		
	for (int i = 0; i <= eComponentType_CranePower; i++)
	{
		if (m_pCraneCalculator->m_bOnlyHoist && i > 0)
		{
			GetDlgItem(m_DetailOrderCtrls[i].nCheckBox)->EnableWindow(FALSE);
			((CButton*)GetDlgItem(m_DetailOrderCtrls[i].nCheckBox))->SetCheck(BST_UNCHECKED);
		}
		else
		{
			((CButton*)GetDlgItem(m_DetailOrderCtrls[i].nCheckBox))->SetCheck(BST_CHECKED);
		}
	}
}
//-----------------------------------------------------------------------
