//-----------------------------------------------------------------------
//	File Name:BMOfferMakerDlg.cpp
//	Introduce:Class CBMOfferMakerDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOfferMakerDlg.h"
#include <shlwapi.h>
#include <shlobj.h>
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferMakerDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOfferMakerDlg, CDialog)
CBMOfferMakerDlg::CBMOfferMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOfferMakerDlg::IDD, pParent)
	, m_strSpecialNote(_T(""))
	, m_strDownPlayment(_T(""))
	, m_strLastPlayment(_T(""))
	, m_strDeliveryTime(_T(""))
	, m_strDeliveryType(_T(""))
{
	m_pContactsInfoDlg = NULL;
	m_pComponentsInfoDlg = NULL;
}
//-----------------------------------------------------------------------
CBMOfferMakerDlg::~CBMOfferMakerDlg()
{
}
//-----------------------------------------------------------------------
void CBMOfferMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPECICALNOTE, m_strSpecialNote);
	DDX_Text(pDX, IDC_EDIT_OFFERMAKERDLG_DOWNPLAY, m_strDownPlayment);
	DDX_Text(pDX, IDC_EDIT_OFFERMAKERDLG_LASTPLAY, m_strLastPlayment);
	DDX_Text(pDX, IDC_EDIT_OFFERMAKERDLG_DELIVERYTIME, m_strDeliveryTime);
	DDX_CBString(pDX, IDC_COMBO_OFFERMAKERDLG_DELIVERYTYPE, m_strDeliveryType);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOfferMakerDlg, CDialog)
	//ON_BN_CLICKED(IDC_BTN_EDIT, &CBMOfferMakerDlg::OnBnClickedBtnEdit)
	//ON_BN_CLICKED(IDC_BTN_SAVE, &CBMOfferMakerDlg::OnBnClickedBtnSave)
	//ON_BN_CLICKED(IDC_BTN_PREVIEW, &CBMOfferMakerDlg::OnBnClickedBtnPreview)
	ON_BN_CLICKED(IDC_BTN_PRINT, &CBMOfferMakerDlg::OnBnClickedBtnPrint)
	ON_MESSAGE(WM_USER_UPDATETOTALPRICE, OnUpdateTotalPrice)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PRINTDRAW, &CBMOfferMakerDlg::OnBnClickedBtnPrintdraw)
	ON_BN_CLICKED(IDC_BTN_ENDCAR, &CBMOfferMakerDlg::OnBnClickedBtnEndcar)
	ON_CBN_SELCHANGE(IDC_COMBO_OFFERMAKERDLG_SALES, &CBMOfferMakerDlg::OnCbnSelchangeComboOffermakerdlgSales)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOfferMakerDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_strDlgCaption);
	//
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_OFFERMAKERDLG_COMPANY);
	/* commented by Demo
	pComboBox->AddString(_T("BM"));
	pComboBox->AddString(_T("GT"));
	*/
	pComboBox->AddString(_T("Baosong"));
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_OFFERMAKERDLG_LANGUAGE);
	pComboBox->AddString(_T("CN"));
	pComboBox->AddString(_T("EN"));
	pComboBox->SetCurSel(0);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_OFFERMAKERDLG_DELIVERYTYPE);
	pComboBox->AddString(_T("A"));
	pComboBox->AddString(_T("B"));
	pComboBox->SetCurSel(0);
	//
	SetDlgItemText(IDC_STATIC_OFFERMAKERDLG_PRICENUIT, m_strCurrency);
	SetDlgItemText(IDC_EDIT_OFFERMAKERDLG_DOWNPLAY, _T("30"));
	SetDlgItemText(IDC_EDIT_OFFERMAKERDLG_LASTPLAY, _T("70"));
	SetDlgItemText(IDC_EDIT_OFFERMAKERDLG_DELIVERYTIME, _T("12"));	
	//
	CRect rcContacts;
	CWnd* pWndContactGrp = GetDlgItem(IDC_STATIC_CONTACTS);
	pWndContactGrp->GetWindowRect(&rcContacts);
	ScreenToClient(&rcContacts);
	rcContacts.left += 5;	rcContacts.right -= 5;
	rcContacts.top += 12;	rcContacts.bottom -= 4;
	m_pContactsInfoDlg = new CBMOfferContactsInfoDlg(this);
	m_pContactsInfoDlg->SetOfferNo(m_strOfferNo);
	m_pContactsInfoDlg->Create(IDD_DIALOG_OFFERCONTACTS, this);
	m_pContactsInfoDlg->MoveWindow(&rcContacts);
	m_pContactsInfoDlg->ShowWindow(SW_SHOW);
	m_pContactsInfoDlg->UpdateWindow();
	//
	CRect rcComponents;
	CWnd* pWndComponentGrp = GetDlgItem(IDC_STATIC_PACKAGE);
	pWndComponentGrp->GetWindowRect(&rcComponents);
	ScreenToClient(&rcComponents);
	rcComponents.left += 5;	rcComponents.right -= 5;
	rcComponents.top += 12;	rcComponents.bottom -= 30;
	m_pComponentsInfoDlg = new CBMOfferComponentsInfoDlg(this);
	m_pComponentsInfoDlg->SetComponentList(m_lstPricesList, m_strCurrency);
	m_pComponentsInfoDlg->Create(IDD_DIALOG_OFFERCOMPONENTS, this);
	m_pComponentsInfoDlg->MoveWindow(&rcComponents);
	m_pComponentsInfoDlg->ShowWindow(SW_SHOW);
	m_pComponentsInfoDlg->UpdateWindow();

	FillSalesComoboBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOfferMakerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (NULL != m_pContactsInfoDlg)
	{
		delete m_pContactsInfoDlg;
		m_pContactsInfoDlg = NULL;
	}
	if (NULL != m_pComponentsInfoDlg)
	{
		delete m_pComponentsInfoDlg;
		m_pComponentsInfoDlg = NULL;
	}
}
//-----------------------------------------------------------------------
/*
void CBMOfferMakerDlg::OnBnClickedBtnEdit()
{
	// TODO: Add your control notification handler code here
}
//-----------------------------------------------------------------------
void CBMOfferMakerDlg::OnBnClickedBtnSave()
{
	// TODO: Add your control notification handler code here
}
//-----------------------------------------------------------------------
void CBMOfferMakerDlg::OnBnClickedBtnPreview()
{
	// TODO: Add your control notification handler code here
}*/
//-----------------------------------------------------------------------
void CBMOfferMakerDlg::OnCbnSelchangeComboOffermakerdlgSales()
{
	if (NULL != m_pComponentsInfoDlg)
	{
		TCHAR tcSales[128] = {_T("\0")};
		GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_SALES, tcSales, 128);
		m_pContactsInfoDlg->UpdateSales(tcSales);
	}
}
//-----------------------------------------------------------------------
void CBMOfferMakerDlg::OnBnClickedBtnPrint()
{
	UpdateData(TRUE);

	CFileDialog dlg(FALSE, _T("doc"), (LPCTSTR)m_strProjectName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Word Document (*.doc)|*.doc|Word 2007 Document (*.docx)|*.docx||"));
	if (dlg.DoModal() == IDOK)
	{
		CString strDocFile = dlg.GetPathName();
		if (ExportToWordTemplate(strDocFile))
		{			
			if (IDYES == myMessageBox(IDS_OFFERMAKERDLG_PRINTSUCCESS, IDS_MSGDLG_SUCCESS, MB_YESNO | MB_ICONINFORMATION))
				ShellExecute(NULL, _T("open"), strDocFile, NULL, NULL, SW_MAXIMIZE);
		}
	}
}
//-----------------------------------------------------------------------
void CBMOfferMakerDlg::OnBnClickedBtnPrintdraw()
{
	ComponentPriceList lstSelectedPrices;
	m_pComponentsInfoDlg->GetSelectedComponents(lstSelectedPrices);
	int iSelectedCnt = lstSelectedPrices.size();
	if (0 == iSelectedCnt)
	{
		myMessageBox(IDS_OFFERMAKERDLG_NOSELECTEDPRICE, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return;
	}

	BROWSEINFO   bi;   
	memset(&bi, 0, sizeof(bi));
	bi.hwndOwner		= m_hWnd;   
	bi.pidlRoot			= NULL;   
	bi.pszDisplayName   = NULL;    
	bi.lpszTitle		= _T("Select Folder");    
	bi.ulFlags			= BIF_BROWSEFORCOMPUTER & BIF_RETURNONLYFSDIRS;   
	bi.lpfn				= NULL;   
	bi.lParam			= NULL;   

	LPITEMIDLIST   pidl;   
	if(pidl = SHBrowseForFolder(&bi))
	{   
		TCHAR tcPath[MAX_PATH] = {0};
		SHGetPathFromIDList(pidl, tcPath); 
		//
		if (ExportDrawingToWordTemplate(lstSelectedPrices, tcPath))
		{			
			if (IDYES == myMessageBox(IDS_OFFERMAKERDLG_PRINTDRAWINGSUCCESS, IDS_MSGDLG_SUCCESS, MB_YESNO | MB_ICONINFORMATION))
				ShellExecute(NULL, _T("open"), tcPath, NULL, NULL, SW_MAXIMIZE);
		}
	} 
}
//-----------------------------------------------------------------------
void CBMOfferMakerDlg::OnBnClickedBtnEndcar()
{
	ComponentPriceList lstSelectedPrices;
	m_pComponentsInfoDlg->GetSelectedComponents(lstSelectedPrices);
	int iSelectedCnt = lstSelectedPrices.size();
	if (0 == iSelectedCnt)
	{
		myMessageBox(IDS_OFFERMAKERDLG_NOSELECTEDPRICE, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return;
	}

	BROWSEINFO   bi;   
	memset(&bi, 0, sizeof(bi));
	bi.hwndOwner		= m_hWnd;   
	bi.pidlRoot			= NULL;   
	bi.pszDisplayName   = NULL;    
	bi.lpszTitle		= _T("Select Folder");    
	bi.ulFlags			= BIF_BROWSEFORCOMPUTER & BIF_RETURNONLYFSDIRS;   
	bi.lpfn				= NULL;   
	bi.lParam			= NULL;   

	LPITEMIDLIST   pidl;   
	if(pidl = SHBrowseForFolder(&bi))
	{   
		TCHAR tcPath[MAX_PATH] = {0};
		SHGetPathFromIDList(pidl, tcPath); 
		//
		if (ExportEndCarriageToWordTemplate(lstSelectedPrices, tcPath))
		{			
			if (IDYES == myMessageBox(IDS_OFFERMAKERDLG_PRINTENDCARSUCCESS, IDS_MSGDLG_SUCCESS, MB_YESNO | MB_ICONINFORMATION))
				ShellExecute(NULL, _T("open"), tcPath, NULL, NULL, SW_MAXIMIZE);
		}
	} 
}
//-----------------------------------------------------------------------
HRESULT CBMOfferMakerDlg::OnUpdateTotalPrice(WPARAM wParam, LPARAM lParam)
{
	int iTotalPrice = wParam;

	CString strTotaPrice;
	strTotaPrice.Format(_T("%d"), iTotalPrice);
	SetDlgItemText(IDC_EDIT_OFFERMAKERDLG_TOTALPRICE, strTotaPrice);
	return 0;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferMakerDlg::SetComponentPrices(CString strDlgCaption, 
										  CString strOfferNo, 
										  CString strProjectName,
										  ComponentPriceList lstPriceInfo, 
										  CString strCurrency)
{
	m_strDlgCaption = strDlgCaption;
	m_strOfferNo = strOfferNo;
	m_strProjectName = strProjectName;
	m_strCurrency = strCurrency;
	m_lstPricesList = lstPriceInfo;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOfferMakerDlg::FillSalesComoboBox()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_OFFERMAKERDLG_SALES);
	if (NULL == pComboBox)
		return;

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE OfferNo='%s'"), g_tcTableName[eTableID_OffersInfo], m_strOfferNo);
	CString strDefalutSales = GetStringValueFromTable(strSQL, _T("Employee"), g_pOfferDatabase);

	CString strOperations;
	CADOTable tbEmployeeInfo;
	strOperations.Format(_T("SELECT * FROM %s"), g_tcTableName[eTableID_EmployeeInfo]);
	if (!tbEmployeeInfo.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOperations) || tbEmployeeInfo.GetRecordCnt() == 0)
		return;
	
	DWORD dwDefaultIndex = 0;
	pComboBox->ResetContent();
	DWORD dwEmployeeCnt = tbEmployeeInfo.GetRecordCnt();
	for (DWORD dwIndex = 0; dwIndex < dwEmployeeCnt; dwIndex++)
	{
		TCHAR tcTemp[128] = {0};
		tbEmployeeInfo.GetRecordCell(dwIndex, L"UserName", tcTemp, 128);
		pComboBox->AddString(tcTemp);
		//
		if (_tcsicmp(tcTemp, strDefalutSales) == 0)
		{
			dwDefaultIndex = dwIndex;
		}
	}
	tbEmployeeInfo.CloseTable();
	pComboBox->SetCurSel(dwDefaultIndex);
	OnCbnSelchangeComboOffermakerdlgSales();
}
//-----------------------------------------------------------------------
CString CBMOfferMakerDlg::GetOfferTemplateFileName(int iComponentCnt)
{
	const TCHAR cTemplateFile[] = {_T("%s-hoist-%s-template%d.doc")};

	CString strCompany;
	CString strLanguage;
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_COMPANY, strCompany);
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_LANGUAGE, strLanguage);

	CString strTemplateFile;
	strTemplateFile.Format(cTemplateFile, strCompany, strLanguage, iComponentCnt);

	CString strExtPath;
	TCHAR tcExePath[MAX_PATH] = {_T("\0")};
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	strExtPath = tcExePath;
	strExtPath = strExtPath.Left(strExtPath.ReverseFind(_T('\\')) + 1);
	//
	CString strTemplateFullName;
	// commented by Demo
	//strTemplateFullName.Format(_T("%s%s%s"), strExtPath, _T("Templates\\OfferTemplates\\"), strTemplateFile);
	
	strTemplateFullName.Format(_T("%s%s%s"), strExtPath, _T("Templates\\"), _T("BaosongTmp.doc"));
	if (!PathFileExists(strTemplateFullName))
	{
		strTemplateFullName = _T("\0");
	}

	return strTemplateFullName;
}
//-----------------------------------------------------------------------
CString	CBMOfferMakerDlg::GetDrawingTemplateFileName(CString strECType, eCraneCtrlType eCtrlType)
{
	const TCHAR cTemplateFile[] = {_T("%s-%s-%s-%s.doc")};

	CString strCompany;
	CString strLanguage;
	CString strCtrlType;
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_COMPANY, strCompany);
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_LANGUAGE, strLanguage);
	switch(eCtrlType)
	{
	case eCraneCtrlType_Pendent:
		strCtrlType = _T("P");
		break;
	case eCraneCtrlType_Remote:
		strCtrlType = _T("R");
		break;	
	case eCraneCtrlType_BlueTooth:
		strCtrlType = _T("B");
		break;
	case eCraneCtrlType_Cabin:
		strCtrlType = _T("C");
		break;
	default:
		strCtrlType = _T("P");
		break;
	}

	CString strTemplateFile;
	strTemplateFile.Format(cTemplateFile, strCompany, strECType, strCtrlType, strLanguage);

	CString strExtPath;
	TCHAR tcExePath[MAX_PATH] = {_T("\0")};
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	strExtPath = tcExePath;
	strExtPath = strExtPath.Left(strExtPath.ReverseFind(_T('\\')) + 1);
	//
	CString strTemplateFullName;
	strTemplateFullName.Format(_T("%s%s%s"), strExtPath, _T("Templates\\DrawingTemplates\\"), strTemplateFile);
	if (!PathFileExists(strTemplateFullName))
	{
		strTemplateFullName = _T("\0");
	}

	return strTemplateFullName;
}
//-----------------------------------------------------------------------
CString CBMOfferMakerDlg::GetEndCarTemplateFileName(CString strECType, 
													TCHAR csDriverType, 
													int iMotorCnt)
{
	const TCHAR cTemplateFile[] = {_T("%s-ec-%s-%c-%d.doc")};

	CString strCompany;
	CString strLanguage;
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_COMPANY, strCompany);
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_LANGUAGE, strLanguage);

	CString strTemplateFile;
	strTemplateFile.Format(cTemplateFile, strCompany, strECType, csDriverType, iMotorCnt);

	CString strExtPath;
	TCHAR tcExePath[MAX_PATH] = {_T("\0")};
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	strExtPath = tcExePath;
	strExtPath = strExtPath.Left(strExtPath.ReverseFind(_T('\\')) + 1);
	//
	CString strTemplateFullName;
	strTemplateFullName.Format(_T("%s%s%s"), strExtPath, _T("Templates\\EndCarTemplates\\Offer\\"), strTemplateFile);
	if (!PathFileExists(strTemplateFullName))
	{
		strTemplateFullName = _T("\0");
	}
	return strTemplateFullName;
}
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::ExportToWordTemplate(CString strDocFile)
{
	int iSelectedCnt = 0;
	ComponentPriceList lstSelectedPrices;
	m_pComponentsInfoDlg->GetSelectedComponents(lstSelectedPrices);
	iSelectedCnt = lstSelectedPrices.size();
	if (0 == iSelectedCnt)
	{
		myMessageBox(IDS_OFFERMAKERDLG_NOSELECTEDPRICE, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	CString strTemplateFile = GetOfferTemplateFileName(iSelectedCnt);
	if (strTemplateFile.IsEmpty())
	{
		myMessageBox(IDS_OFFERMAKERDLG_NOTMEPLATEFILE, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	HRESULT hr = S_OK;
	CMSWordApplication wordApp;
	hr = wordApp.OpenWordFile(strTemplateFile);
	if (S_OK != hr)
	{
		myMessageBox(IDS_OFFERMAKERDLG_OPENMEPLATEFAILED, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// Export contacts information and setting
	StringPartnerList lstTextPartner;
	lstTextPartner.push_back(StringPartner(_T("(cucompany)"), m_pContactsInfoDlg->m_strCompany));
	lstTextPartner.push_back(StringPartner(_T("(customer)"), m_pContactsInfoDlg->m_strCustomer));
	lstTextPartner.push_back(StringPartner(_T("(cuphone)"), m_pContactsInfoDlg->m_strCusPhone));
	lstTextPartner.push_back(StringPartner(_T("(cufax)"), m_pContactsInfoDlg->m_strCusFax));
	lstTextPartner.push_back(StringPartner(_T("(cumail)"), m_pContactsInfoDlg->m_strCusEmail));
	lstTextPartner.push_back(StringPartner(_T("(cumobile)"), m_pContactsInfoDlg->m_strCusMobile));
	lstTextPartner.push_back(StringPartner(_T("(offernumber)"), m_pContactsInfoDlg->m_strOfferNo));
	lstTextPartner.push_back(StringPartner(_T("(mysales)"), m_pContactsInfoDlg->m_strSales));
	lstTextPartner.push_back(StringPartner(_T("(myphone)"), m_pContactsInfoDlg->m_strSalPhone));
	lstTextPartner.push_back(StringPartner(_T("(myfax)"), m_pContactsInfoDlg->m_strSalFax));
	lstTextPartner.push_back(StringPartner(_T("(mymail)"), m_pContactsInfoDlg->m_strSalEmail));
	lstTextPartner.push_back(StringPartner(_T("(mymobile)"), m_pContactsInfoDlg->m_strSalMobile));
	//
	lstTextPartner.push_back(StringPartner(_T("(specialnote)"), m_strSpecialNote));	
	lstTextPartner.push_back(StringPartner(_T("(downpayment)"), m_strDownPlayment));	
	lstTextPartner.push_back(StringPartner(_T("(lastpayment)"), m_strLastPlayment));	
	lstTextPartner.push_back(StringPartner(_T("(deliverytime)"), m_strDeliveryTime));	
	lstTextPartner.push_back(StringPartner(_T("(deliverytype)"), m_strDeliveryType));	

	// Export equipment name and price
	int iIndex = 0;
	int iAllPrice = 0;
	for (ComponentPriceList::iterator it = lstSelectedPrices.begin();
		it != lstSelectedPrices.end();
		it++, iIndex++)
	{
		ComPriceInfo price = *it;
		iAllPrice += (price.iComCount * price.iOfferPrice);

		CString strEquipmentName;
		TCHAR tcEquipmentID[64] = {_T("\0")};
		TCHAR tcEquipmentName[64] = {_T("\0")};
		_stprintf(tcEquipmentID, _T("(equippackage%d)"), iIndex+1);
		_stprintf(tcEquipmentName, _T("%s"), price.strComName);
		lstTextPartner.push_back(StringPartner(tcEquipmentID, tcEquipmentName));	

		ExportComponentDataToWordTemplate(&wordApp, iIndex, price);

		ExportTechDataToWordTemplate(&wordApp, iIndex, price, (CBMParamCalculator*)price.pCalculator);
	}	

	// Export all Equipment names and total price
	CString strAllPrice;
	strAllPrice.Format(_T("%d"), iAllPrice);
	lstTextPartner.push_back(StringPartner(_T("(total price)"), strAllPrice));	

	// Export currency
	lstTextPartner.push_back(StringPartner(_T("(currency)"), m_strCurrency));
	wordApp.ReplaceTexts(lstTextPartner);
	 
	if (S_OK == wordApp.SaveWordFile(strDocFile))
	{
		return TRUE;
	}

	return FALSE;
}
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::ExportComponentDataToWordTemplate(CMSWordApplication* pWordApp, 
														 int iComponentIndex,
														 ComPriceInfo component)
{
	CString strCount;
	CString strPrice;
	CString strTotalPrice;
	int iTotalPrice = component.iComCount * component.iOfferPrice;
	strCount.Format(_T("%d"), component.iComCount);
	strPrice.Format(_T("%d"), component.iOfferPrice);
	strTotalPrice.Format(_T("%d"), iTotalPrice);
	
	/* Equipment price table */
	pWordApp->SetTableCell(1, iComponentIndex+2, 2, strCount);
	pWordApp->SetTableCell(1, iComponentIndex+2, 3, component.strComName);
	pWordApp->SetTableCell(1, iComponentIndex+2, 4, strPrice);
	pWordApp->SetTableCell(1, iComponentIndex+2, 5, strTotalPrice);

	for (DetailPriceList::iterator it2 = component.lstDetails.begin();
		 it2 != component.lstDetails.end();
		 it2++)
	{
		DetailPriceInfo detail = *it2;
		int iRowIndex = 2;
		switch(detail.eType)
		{
		case eComponentType_Hoist:
			iRowIndex = 2;
			break;
		case eComponentType_EndCar:
			iRowIndex = 3;
			break;
		case eComponentType_BridgeDri:
			iRowIndex = 4;
			break;
		case eComponentType_BridgePannel:
			iRowIndex = 5;
			break;
		case eComponentType_Fastoon:
			iRowIndex = 6;
			break;
		case eComponentType_OperateUnit:
			iRowIndex = 7;
			break;
		case eComponentType_CranePower:
			iRowIndex = 8;
			break;
		case eComponentType_MainGirder:
			iRowIndex = 9;
			break;
		}
		
		/* Detail price tables */
		CString strDetailCount;
		CString strDetailTotalPrice;
		strDetailCount.Format(_T("%d"), detail.iCount);
		strDetailTotalPrice.Format(_T("%d"), detail.iTotalPrice);
		pWordApp->SetTableCell(iComponentIndex + 2, iRowIndex, 2, strDetailCount);
		pWordApp->SetTableCell(iComponentIndex + 2, iRowIndex, 4, detail.strCode);
		pWordApp->SetTableCell(iComponentIndex + 2, iRowIndex, 5, strDetailTotalPrice);
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::ExportTechDataToWordTemplate(CMSWordApplication* pWordApp, 
													int iComponentIndex,
													ComPriceInfo component, 
													CBMParamCalculator* pCalculator)
{
	if (NULL == pCalculator)
		return FALSE;

	CString strLanguage;
	BOOL bLangIsCn = FALSE;
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_LANGUAGE, strLanguage);
	bLangIsCn = (strLanguage == _T("CN")) ? TRUE : FALSE;

	/*	Hoist or Crane data */
	CString strTitle;
	TechDataList dataList;
	TechDataList additionsDataList;
	strTitle.Format(_T("%s%d:   %s\r\n"), bLangIsCn?_T("设备"):_T("Equipment"), iComponentIndex+1, component.strComName);
	if (pCalculator->m_bOnlyHoist)
	{
		pCalculator->GetHoistTechData(dataList);
		pCalculator->GetHoistSelAddTechData(additionsDataList);
	}
	else
	{
		TechDataList craneAdditions;
		pCalculator->GetCranesTechData(dataList);
		pCalculator->GetCraneSelAddTechData(craneAdditions);
		for (TechDataList::iterator it = craneAdditions.begin(); it != craneAdditions.end(); it++)
		{
			TechData data = *it;
			additionsDataList.push_back(data);
		}
	}
	pWordApp->AddStringToEnd(strTitle);
	for (TechDataList::iterator it = dataList.begin(); it != dataList.end(); it++)
	{
		TechData data = *it;
		pWordApp->AddStringToEnd(bLangIsCn ? data.strPrintText_CN : data.strPrintText_EN, FALSE);
	}

	if (pCalculator->m_bOnlyHoist)
	{
		/*	Standard additions data */
		strTitle.Format(_T("%s\r\n"), bLangIsCn ? _T("标准选项") : _T("Standard options"));
		pCalculator->GetHoistStdAddTechData(dataList);
		pWordApp->AddStringToEnd(strTitle);
		for (TechDataList::iterator it = dataList.begin(); it != dataList.end(); it++)
		{
			TechData data = *it;
			pWordApp->AddStringToEnd(_T("\t"), FALSE);
			pWordApp->AddStringToEnd(bLangIsCn ? data.strPrintText_CN : data.strPrintText_EN, FALSE);
			pWordApp->AddStringToEnd(_T("\r\n"), FALSE);
		}
	}

	/*	Selected additions data */
	strTitle.Format(_T("%s\r\n"), bLangIsCn ? _T("增加选项") : _T("Added options"));
	pWordApp->AddStringToEnd(strTitle);
	for (TechDataList::iterator it = additionsDataList.begin(); it != additionsDataList.end(); it++)
	{
		TechData data = *it;
		pWordApp->AddStringToEnd(_T("\t"), FALSE);
		pWordApp->AddStringToEnd(bLangIsCn ? data.strPrintText_CN : data.strPrintText_EN, FALSE);
		pWordApp->AddStringToEnd(_T("\r\n"), FALSE);
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::ExportDrawingToWordTemplate(ComponentPriceList lstSelPrices, 
												   CString strDrawingFolder)
{
	for (ComponentPriceList::iterator it = lstSelPrices.begin();
		it != lstSelPrices.end();
		it++)
	{
		ComPriceInfo info = *it;
		CBMParamCalculator* pCalculator = (CBMParamCalculator*)info.pCalculator;
		if (NULL == pCalculator || pCalculator->m_bOnlyHoist)
			continue;

		CString strTemplateFile = GetDrawingTemplateFileName(pCalculator->m_strECType, pCalculator->m_eCraneControl);
		if (strTemplateFile.IsEmpty())
		{
			myMessageBox(IDS_OFFERMAKERDLG_NOTMEPLATEFILE, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
			return FALSE;
		}

		HRESULT hr = S_OK;
		CMSWordApplication wordApp;
		hr = wordApp.OpenWordFile(strTemplateFile);
		if (S_OK != hr)
		{
			myMessageBox(IDS_OFFERMAKERDLG_OPENMEPLATEFAILED, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
			return FALSE;
		}

		ExportComponentDrawingToWordTemplate(&wordApp, pCalculator);

		const TCHAR tcRepace[] = {_T('\\'), _T('/'), _T(':'), _T('*'), _T('?'), _T('"'), _T('<'), _T('>'), _T('|')};
		TCHAR tcComName[MAX_PATH] = {0};
		_tcscpy(tcComName, info.strComName);
		TCHAR* p = tcComName;
		while(*p != _T('\0'))
		{
			for (int i = 0; i < sizeof(tcRepace) / sizeof(tcRepace[0]); i++)
			{
				if (*p == tcRepace[i])
				{
					*p = _T('-');
					break;
				}
			}
			p++;
		}

		TCHAR tcOutputFile[MAX_PATH] = {0};
		_stprintf(tcOutputFile, _T("%s\\%s%s"), strDrawingFolder, tcComName, _T(".doc"));
		wordApp.SaveWordFile(tcOutputFile);
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::ExportComponentDrawingToWordTemplate(CMSWordApplication* pWordApp, 
															CBMParamCalculator* pCalculator)
{
	if (NULL == pCalculator)
		return FALSE;

	// Get hoist dim data
	int iHoist_W = 0;
	int iHoist_S1 = 0;
	int iHoist_S2 = 0;
	int iHoist_S3 = 0;
	int iHoist_S4 = 0;
	int iHoist_H1 = 0;
	int iHoist_H3 = 0;
	int iHoist_R = 0;
	int iHoist_C = 0;
	CString strSQL;
	CString strHoistDimTableName;
	switch(pCalculator->m_eCraneType)
	{
	case eCraneType_GTS:
		strHoistDimTableName = g_tcTableName[eTableID_DimHoi_S];
		break;
	case eCraneType_GTD:
		strHoistDimTableName = g_tcTableName[eTableID_DimHoi_D];
		break;
	case eCraneType_GTF:
		strHoistDimTableName = g_tcTableName[eTableID_DimHoi_F];
		break;
	}
	CADOTable tbDimHoiSel;
	strSQL.Format(_T("SELECT * FROM %s WHERE HoiDimIndCod='%s'"), strHoistDimTableName, pCalculator->m_strHoiDimIndCode);
	if (tbDimHoiSel.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbDimHoiSel.GetRecordCnt() > 0)
	{
		TCHAR tcH2[32] = {0};
		tbDimHoiSel.GetRecordCell(0, L"W", (DWORD&)iHoist_W);
		tbDimHoiSel.GetRecordCell(0, L"S1", (DWORD&)iHoist_S1);
		tbDimHoiSel.GetRecordCell(0, L"S2", (DWORD&)iHoist_S2);
		tbDimHoiSel.GetRecordCell(0, L"S3", (DWORD&)iHoist_S3);
		tbDimHoiSel.GetRecordCell(0, L"S4", (DWORD&)iHoist_S4);
		tbDimHoiSel.GetRecordCell(0, L"H1", (DWORD&)iHoist_H1);
		tbDimHoiSel.GetRecordCell(0, L"H3", (DWORD&)iHoist_H3);
		tbDimHoiSel.GetRecordCell(0, L"R", (DWORD&)iHoist_R);
		tbDimHoiSel.GetRecordCell(0, L"C", (DWORD&)iHoist_C);
	}
	tbDimHoiSel.CloseTable();
	//
	int iEndCar_H = 0;
	int iEcWeiMax = 0;
	int iEcBufHei = 0;
	CADOTable tbEndCarMatPar;
	strSQL.Format(_T("SELECT * FROM %s WHERE EndCarSec='%s'"), g_tcTableName[eTableID_EndCarMatPar], pCalculator->m_strECStructure);
	if (tbEndCarMatPar.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbEndCarMatPar.GetRecordCnt() > 0)
	{
		tbEndCarMatPar.GetRecordCell(0, L"EndCarHei", (DWORD&)iEndCar_H);
		tbEndCarMatPar.GetRecordCell(0, L"EcWeiMax", (DWORD&)iEcWeiMax);
		tbEndCarMatPar.GetRecordCell(0, L"Bufhei", (DWORD&)iEcBufHei);
	}
	tbEndCarMatPar.CloseTable();
	//
	CString strWheelDim;
	strWheelDim.Format(_T("%c%c"), pCalculator->m_strEndCraCode.GetAt(1), pCalculator->m_strEndCraCode.GetAt(2));
	int iWheelDim = _ttol(strWheelDim);
	strSQL.Format(_T("SELECT JoiPlath FROM %s WHERE WheelDim=%d"), _T("JoiPlaDat"), iWheelDim);
	int iJoiPlath = GetLONGValueFromTable(strSQL, L"JoiPlath");
	//
	CString strCodBufSel = pCalculator->m_strEndCraCode.Left(3);
	strSQL.Format(_T("SELECT * FROM %s WHERE CodBufSel='%s'"), g_tcTableName[eTableID_Buffer], strCodBufSel);	
	int iBufLen = GetFLOATValueFromTable(strSQL, L"BufLen");

	int iDim0 = 0;
	int iDim1 = 0;
	int iDim2 = 0;
	int iDim3 = 0;
	int iDim4 = 0;
	int iDim5 = 0;
	int iDim6 = 0;
	int iDim7 = 0;
	int iDim8 = 0;
	int iDim9 = 0;
	int iDim10 = 0;
	int iDim11 = 0;
	int iDim12 = 0;
	int iDim13 = 0;
	int iDim14 = 0;
	int iDim15 = 0;
	int iDim16 = 0;
	int iDim17 = 0;
	int iDim18 = 0;
	int iDim19 = 0;
	int iDim20 = 0;
	int iDim21 = 0;
	int iDim22 = 0;
	if (eCraneType_GTD == pCalculator->m_eCraneType)
	{
		iDim2 = pCalculator->m_iHoist_b1 + 200;
		iDim3 = pCalculator->m_iHoist_b2 + 200;
		iDim5 = iEndCar_H + iJoiPlath + pCalculator->m_iGirder_h2 + pCalculator->m_iGirder_t4 + pCalculator->m_iGirder_Rh;
		iDim6 = iDim5 + iHoist_H1;
		iDim9 = pCalculator->m_iRoofToRailTop - iDim6;
		iDim10 = _ttol(pCalculator->m_strRaiGauge);
		iDim13 = pCalculator->m_iGirder_Rh + pCalculator->m_iGirder_t4 + pCalculator->m_iGirder_h1 + pCalculator->m_iGirder_t2 + 200;
		iDim16 = iHoist_H1 + iHoist_C;
		iDim17 = pCalculator->m_iRailTopToFloor + iDim6 - iDim16;
		iDim19 = pCalculator->m_iGirder_w1 + pCalculator->m_iGirder_w3 + 400;
		iDim20 = pCalculator->m_iGirder_w1 + 800;
		iDim21 = iHoist_R / 2 - pCalculator->m_iHoist_H2;
		iDim21 = abs(iDim21);
	}
	else if (eCraneType_GTS == pCalculator->m_eCraneType)
	{
		iDim2 = pCalculator->m_iHoist_H2 + iEcWeiMax + iHoist_S2;
		iDim3 = iHoist_S1 + iHoist_S2 + iHoist_W - iHoist_H1 - pCalculator->m_iHoist_H2 + iEcWeiMax;
		iDim5 = iEndCar_H + iJoiPlath + pCalculator->m_iGirder_h2 + pCalculator->m_iGirder_t4 + 50;
		iDim9 = pCalculator->m_iRoofToRailTop - iDim5;
		iDim13 = 50 + pCalculator->m_iGirder_t4 + pCalculator->m_iGirder_h1 + pCalculator->m_iGirder_t2;
		iDim16 = iHoist_C + pCalculator->m_iGirder_h2 + pCalculator->m_iGirder_t4 + 50;
		iDim17 = pCalculator->m_iRailTopToFloor + iDim5 - iDim16;
		iDim19 = pCalculator->m_iGirder_w1 / 2 + iHoist_S3 + 500;
		iDim20 = pCalculator->m_iGirder_w1 / 2 + iHoist_S4 + 500;
	}
	CBMParamCalculator::GetMinAndClearanceSize(pCalculator->m_eCraneType, pCalculator->m_iLoadOfCrane, iDim1, iDim0);
	iDim1 += iEcWeiMax;
	iDim4 = pCalculator->m_iRailTopToFloor;
	iDim7 = pCalculator->m_iRoofToRailTop;
	iDim8 = iDim7 + iDim4;
	iDim11 = pCalculator->GetWheelBase();
	iDim12 = iDim11 + iWheelDim + 100 + 2 * iBufLen;
	iDim14 = iDim4 + iDim5 - iDim13;
	iDim15 = iDim4 + iDim5 - 1000;
	iDim18 = iEcBufHei;
	iDim22 = iHoist_H3;

	StringPartnerList lstTextPartner;

	CString strTemp;
	strTemp.Format(_T("%d"), iDim0);
	lstTextPartner.push_back(StringPartner(_T("(dim0)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim1);
	lstTextPartner.push_back(StringPartner(_T("(dim1)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim2);
	lstTextPartner.push_back(StringPartner(_T("(dim2)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim3);
	lstTextPartner.push_back(StringPartner(_T("(dim3)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim4);
	lstTextPartner.push_back(StringPartner(_T("(dim4)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim5);
	lstTextPartner.push_back(StringPartner(_T("(dim5)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim6);
	lstTextPartner.push_back(StringPartner(_T("(dim6)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim7);
	lstTextPartner.push_back(StringPartner(_T("(dim7)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim8);
	lstTextPartner.push_back(StringPartner(_T("(dim8)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim9);
	lstTextPartner.push_back(StringPartner(_T("(dim9)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim10);
	lstTextPartner.push_back(StringPartner(_T("(dim10)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim11);
	lstTextPartner.push_back(StringPartner(_T("(dim11)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim12);
	lstTextPartner.push_back(StringPartner(_T("(dim12)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim13);
	lstTextPartner.push_back(StringPartner(_T("(dim13)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim14);
	lstTextPartner.push_back(StringPartner(_T("(dim14)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim15);
	lstTextPartner.push_back(StringPartner(_T("(dim15)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim16);
	lstTextPartner.push_back(StringPartner(_T("(dim16)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim17);
	lstTextPartner.push_back(StringPartner(_T("(dim17)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim18);
	lstTextPartner.push_back(StringPartner(_T("(dim18)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim19);
	lstTextPartner.push_back(StringPartner(_T("(dim19)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim20);
	lstTextPartner.push_back(StringPartner(_T("(dim20)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim21);
	lstTextPartner.push_back(StringPartner(_T("(dim21)"), strTemp));
	//
	strTemp.Format(_T("%d"), iDim22);
	lstTextPartner.push_back(StringPartner(_T("(dim22)"), strTemp));

	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_OFFERMAKERDLG_LANGUAGE);
	int iLangIndex = pComboBox->GetCurSel();

	strTemp.Format(_T("%0.2f"), float(pCalculator->m_iLoadOfCrane / 1000.0f));
	lstTextPartner.push_back(StringPartner(_T("(cralod)"), strTemp));
	//
	strTemp.Format(_T("%d"), int(pCalculator->m_fSpan * 1000));
	lstTextPartner.push_back(StringPartner(_T("(span1)"), strTemp));
	//
	strTemp.Format(_T("%d"), int(pCalculator->m_fSpan * 1000) + 2 * iEcWeiMax);
	lstTextPartner.push_back(StringPartner(_T("(span2)"), strTemp));
	//
	strTemp = pCalculator->m_strHoistCode.Right(pCalculator->m_strHoistCode.GetLength() - 2);
	lstTextPartner.push_back(StringPartner(_T("(hoistcode)"), strTemp));
	//
	strTemp.Format(_T("%0.2f"), pCalculator->m_fMaxWheLoad);
	lstTextPartner.push_back(StringPartner(_T("(maxwheelload)"), strTemp));
	//
	strTemp = pCalculator->m_strEndCraCode.Left(5);
	lstTextPartner.push_back(StringPartner(_T("(eccode)"), strTemp));
	//
	strTemp.Format(_T("%0.2f"), float(iDim17/1000.0f));
	lstTextPartner.push_back(StringPartner(_T("(hol)"), strTemp));
	//
	strTemp.Format(_T("%d"), int(pCalculator->m_fHoistHOL * 1000));
	lstTextPartner.push_back(StringPartner(_T("(hol1)"), strTemp));
	//
	strTemp.Format(_T("%0.2f"), pCalculator->m_fHoistHOL);
	lstTextPartner.push_back(StringPartner(_T("(holmax)"), strTemp));
	//
	strTemp.Format(_T("%0.2f"), pCalculator->m_fSpan);
	lstTextPartner.push_back(StringPartner(_T("(span)"), strTemp));
	//
	strTemp.Format(_T("%s"), pCalculator->m_strHoistingSpeed);
	lstTextPartner.push_back(StringPartner(_T("(hoispeed)"), strTemp));
	//
	strTemp.Format(_T("%s"), pCalculator->m_strTrolleySpeed);
	lstTextPartner.push_back(StringPartner(_T("(trospeed)"), strTemp));
	//
	strTemp.Format(_T("%s"), pCalculator->m_strBridgeSpeed);
	lstTextPartner.push_back(StringPartner(_T("(brispeed)"), strTemp));
	//
	if (0 == iLangIndex) strTemp.Format(_T("%s"), (eElecCtrlType_2Speeds == pCalculator->m_eHoistCtrlType) ? _T("双速") : _T("变频"));
	else strTemp.Format(_T("%s"), (eElecCtrlType_2Speeds == pCalculator->m_eHoistCtrlType) ? _T("2 Speeds") : _T("Inverter"));
	lstTextPartner.push_back(StringPartner(_T("(hoitype)"), strTemp));
	//
	if (0 == iLangIndex) strTemp.Format(_T("%s"), (eElecCtrlType_2Speeds == pCalculator->m_eTrolleyCtrlType) ? _T("双速") : _T("变频"));
	else strTemp.Format(_T("%s"), (eElecCtrlType_2Speeds == pCalculator->m_eTrolleyCtrlType) ? _T("2 Speeds") : _T("Inverter"));
	lstTextPartner.push_back(StringPartner(_T("(trotype)"), strTemp));
	//
	if (0 == iLangIndex) strTemp.Format(_T("%s"), (eElecCtrlType_2Speeds == pCalculator->m_eBridgeCtrlType) ? _T("双速") : _T("变频"));
	else strTemp.Format(_T("%s"), (eElecCtrlType_2Speeds == pCalculator->m_eBridgeCtrlType) ? _T("2 Speeds") : _T("Inverter"));
	lstTextPartner.push_back(StringPartner(_T("(britype)"), strTemp));
	//
	strTemp.Format(_T("%0.2f"),  float(pCalculator->m_iHoist_G1 / 1000.0f));
	lstTextPartner.push_back(StringPartner(_T("(weitro)"), strTemp));
	//
	strTemp.Format(_T("%0.2f"), float(pCalculator->m_iCraneTotalWgt / 1000.0f));
	lstTextPartner.push_back(StringPartner(_T("(weibri)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_iVoltage1_Main);
	lstTextPartner.push_back(StringPartner(_T("(V1)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_iVoltage2_Ctrl);
	lstTextPartner.push_back(StringPartner(_T("(V2)"), strTemp));
	//
	strTemp.Format(_T("%s"), pCalculator->m_strDutyOfCrane);
	lstTextPartner.push_back(StringPartner(_T("(cragro)"), strTemp));
	//
	strTemp.Format(_T("%s"), pCalculator->m_strDutyOfHoist);
	lstTextPartner.push_back(StringPartner(_T("(hoigro)"), strTemp));
	//
	strTemp.Format(_T("%s"), _T("M5(2m)"));	//TBD
	lstTextPartner.push_back(StringPartner(_T("(trogro)"), strTemp));
	//
	strTemp.Format(_T("%s"), _T("M5(2m)"));	//TBD
	lstTextPartner.push_back(StringPartner(_T("(brigro)"), strTemp));
	//
	float fHoistMotorPow = CBMParamCalculator::GetLeftValueOfRangeString(pCalculator->m_strHoist_Pow);
	float fTrolleyMotorPow = pCalculator->m_iTroDriNum * pCalculator->m_fHoist_DriPow;
	float fCraneMotorPow = pCalculator->m_iNumOfMotor * pCalculator->m_fEndCar_DriPow;
	strTemp.Format(_T("%0.2f"), fHoistMotorPow+fTrolleyMotorPow+fCraneMotorPow);
	lstTextPartner.push_back(StringPartner(_T("(suppow)"), strTemp));
	//
	strTemp.Format(_T("%0.2f"), pCalculator->m_fRailLenght);
	lstTextPartner.push_back(StringPartner(_T("(railen)"), strTemp));
	//
	strTemp.Format(_T("%s"), pCalculator->m_strRailType);
	lstTextPartner.push_back(StringPartner(_T("(railtype)"), strTemp));
	//
	strTemp.Format(_T("%d"), _ttol(pCalculator->m_strRaiGauge));
	lstTextPartner.push_back(StringPartner(_T("(sales)"), g_strUserID));
	//
	switch(pCalculator->m_eCraneControl)
	{
	case eCraneCtrlType_Remote:
		strTemp.Format(_T("%s"), _T("Radio control"));
		break;	
	case eCraneCtrlType_BlueTooth:
		strTemp.Format(_T("%s"), _T("Blue tooth"));
		break;
	case eCraneCtrlType_Pendent:
	case eCraneCtrlType_Cabin:
	default:
		strTemp.Format(_T("%s"), _T(""));
		break;
	}
	lstTextPartner.push_back(StringPartner(_T("(contype)"), strTemp));

	pWordApp->ReplaceTextsInTextBoxes(lstTextPartner);
	//pWordApp->ReplaceTexts(lstTextPartner);

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::ExportEndCarriageToWordTemplate(ComponentPriceList lstSelPrices, 
													   CString strEndCarFolder)
{
	for (ComponentPriceList::iterator it = lstSelPrices.begin();
		it != lstSelPrices.end();
		it++)
	{
		ComPriceInfo info = *it;
		CBMParamCalculator* pCalculator = (CBMParamCalculator*)info.pCalculator;
		if (NULL == pCalculator || pCalculator->m_bOnlyHoist)
			continue;

		CString strTemplateFile = GetEndCarTemplateFileName(pCalculator->m_strECType, pCalculator->m_strDriverCode.GetAt(5), pCalculator->m_iNumOfMotor/2);
		if (strTemplateFile.IsEmpty())
		{
			myMessageBox(IDS_OFFERMAKERDLG_NOTMEPLATEFILE, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
			continue;
		}

		HRESULT hr = S_OK;
		CMSWordApplication wordApp;
		hr = wordApp.OpenWordFile(strTemplateFile);
		if (S_OK != hr)
		{
			myMessageBox(IDS_OFFERMAKERDLG_OPENMEPLATEFAILED, IDS_DLGMSG_ERROR, MB_OK | MB_ICONERROR);
			return FALSE;
		}

		CString strTemp;
		StringPartnerList lstTextPartner;

		// Project Name
		wordApp.SetTableCell(1, 2, 2, m_strProjectName);
		
		// Component Name
		lstTextPartner.push_back(StringPartner(_T("(equicode)"), info.strComName));

		// Component Count
		strTemp.Format(_T("%d"), info.iComCount);
		lstTextPartner.push_back(StringPartner(_T("(ecunit)"), strTemp));
		wordApp.ReplaceTexts(lstTextPartner);
		
		ExportEndCarriageDataToWordTemplate(&wordApp, pCalculator);

		const TCHAR tcRepace[] = {_T('\\'), _T('/'), _T(':'), _T('*'), _T('?'), _T('"'), _T('<'), _T('>'), _T('|')};
		TCHAR tcComName[MAX_PATH] = {0};
		_tcscpy(tcComName, info.strComName);
		TCHAR* p = tcComName;
		while(*p != _T('\0'))
		{
			for (int i = 0; i < sizeof(tcRepace) / sizeof(tcRepace[0]); i++)
			{
				if (*p == tcRepace[i])
				{
					*p = _T('-');
					break;
				}
			}
			p++;
		}

		TCHAR tcOutputFile[MAX_PATH] = {0};
		_stprintf(tcOutputFile, _T("%s\\%s%s%s"), strEndCarFolder, _T("EC_"), tcComName, _T(".doc"));
		wordApp.SaveWordFile(tcOutputFile);
	}

	return TRUE;
}
//-----------------------------------------------------------------------