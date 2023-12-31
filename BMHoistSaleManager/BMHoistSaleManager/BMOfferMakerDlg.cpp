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
	ON_BN_CLICKED(IDC_BTN_STEELSTRUCTURE, &CBMOfferMakerDlg::OnBnClickedBtnSteelstructure)
	ON_CBN_SELCHANGE(IDC_COMBO_OFFERMAKERDLG_SALES, &CBMOfferMakerDlg::OnCbnSelchangeComboOffermakerdlgSales)
	ON_WM_LBUTTONUP()
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
	pComboBox->AddString(_T("BM"));
	pComboBox->AddString(_T("GT"));
	SelectComboBoxItem(pComboBox, g_LoginUserInfo.C38);
	//
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_OFFERMAKERDLG_LANGUAGE);
	pComboBox->AddString(_T("CN"));
	pComboBox->AddString(_T("EN"));
	pComboBox->SetCurSel(0);
	//
	FillSalesComoboBox();
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
void CBMOfferMakerDlg::OnBnClickedBtnSteelstructure()
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
		if (ExportSteelStructureToWordTemplate(lstSelectedPrices, tcPath))
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

	CString strCompany;
	CString strLanguage;
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_COMPANY, strCompany);
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_LANGUAGE, strLanguage);
	CString strTemplateFile = GetOfferOrderTemplateFile(_T("OfferTemplates"), strCompany, strLanguage, iSelectedCnt);
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
	
	BeginWaitCursor();
	
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
	 
	hr = wordApp.SaveWordFile(strDocFile);
	EndWaitCursor();

	return (S_OK == hr) ? TRUE : FALSE;
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
	TechDataList hoistDataList;
	TechDataList craneDataList;
	TechDataList craneHoistDataList;
	TechDataList hoistStdAddDataList;
	TechDataList hoistAddDataList;
	TechDataList craneAddDataList;
	TechDataList additionsDataList;
	if (_T("BMZ") == pCalculator->m_CalcDatalst.GA1) 
		pCalculator->GetHoistTechDataEx(hoistDataList);
	else 
		pCalculator->GetHoistTechData(hoistDataList);
	pCalculator->GetHoistStdAddTechData(hoistStdAddDataList);
	pCalculator->GetHoistSelAddTechData(hoistAddDataList);
	pCalculator->GetCranesTechData(craneDataList);
	if (_T("BMZ") == pCalculator->m_CalcDatalst.GA1) 
		pCalculator->GetCranesTechHoistDataEx(craneHoistDataList);
	else 
		pCalculator->GetCranesTechHoistData(craneHoistDataList);
	pCalculator->GetCraneSelAddTechData(craneAddDataList);


	strTitle.Format(_T("%s%d:   %s\r\n"), bLangIsCn?_T("设备"):_T("Equipment"), iComponentIndex+1, component.strComName);
	pWordApp->AddStringToEnd(strTitle);	
	if (pCalculator->m_bOnlyHoist)
	{
		//Hoist Data
		for (TechDataList::iterator it = hoistDataList.begin(); it != hoistDataList.end(); it++)
		{
			TechData data = *it;
			pWordApp->AddStringToEnd(bLangIsCn ? data.strPrintText_CN : data.strPrintText_EN, FALSE);
		}
		//Hoist standart additions data
		if (hoistStdAddDataList.size() > 0)
		{
			strTitle.Format(_T("%s\r\n"), bLangIsCn ? _T("标准选项") : _T("Standard options"));
			pWordApp->AddStringToEnd(strTitle);
			for (TechDataList::iterator it = hoistStdAddDataList.begin(); it != hoistStdAddDataList.end(); it++)
			{
				TechData data = *it;
				pWordApp->AddStringToEnd(_T("\t"), FALSE);
				pWordApp->AddStringToEnd(bLangIsCn ? data.strPrintText_CN : data.strPrintText_EN, FALSE);
				pWordApp->AddStringToEnd(_T("\r\n"), FALSE);
			}
		}
		//Hoist additions data
		if (hoistAddDataList.size() > 0)
		{
			strTitle.Format(_T("%s\r\n"), bLangIsCn ? _T("增加选项") : _T("Added options"));
			pWordApp->AddStringToEnd(strTitle);
			for (TechDataList::iterator it = hoistAddDataList.begin(); it != hoistAddDataList.end(); it++)
			{
				TechData data = *it;
				pWordApp->AddStringToEnd(_T("\t"), FALSE);
				pWordApp->AddStringToEnd(bLangIsCn ? data.strPrintText_CN : data.strPrintText_EN, FALSE);
				pWordApp->AddStringToEnd(_T("\r\n"), FALSE);
			}
		}
	}
	else
	{	//Crane data
		for (TechDataList::iterator it = craneDataList.begin(); it != craneDataList.end(); it++)
		{
			TechData data = *it;
			pWordApp->AddStringToEnd(bLangIsCn ? data.strPrintText_CN : data.strPrintText_EN, FALSE);
		}
		for (TechDataList::iterator it = craneHoistDataList.begin(); it != craneHoistDataList.end(); it++)
		{
			TechData data = *it;
			pWordApp->AddStringToEnd(bLangIsCn ? data.strPrintText_CN : data.strPrintText_EN, FALSE);
		}
		//
		if (hoistAddDataList.size() > 0)
		{
			strTitle.Format(_T("%s\r\n"), bLangIsCn ? _T("增加选项") : _T("Added options"));
			pWordApp->AddStringToEnd(strTitle);
			for (TechDataList::iterator it = hoistAddDataList.begin(); it != hoistAddDataList.end(); it++)
			{
				TechData data = *it;
				pWordApp->AddStringToEnd(_T("\t"), FALSE);
				pWordApp->AddStringToEnd(bLangIsCn ? data.strPrintText_CN : data.strPrintText_EN, FALSE);
				pWordApp->AddStringToEnd(_T("\r\n"), FALSE);
			}
		}
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::ExportDrawingToWordTemplate(ComponentPriceList lstSelPrices, 
												   CString strDrawingFolder)
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_OFFERMAKERDLG_LANGUAGE);
	int iLangIndex = pComboBox->GetCurSel();

	CString strCompany;
	CString strLanguage;
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_COMPANY, strCompany);
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_LANGUAGE, strLanguage);

	for (ComponentPriceList::iterator it = lstSelPrices.begin();
		it != lstSelPrices.end();
		it++)
	{
		ComPriceInfo info = *it;
		CBMParamCalculator* pCalculator = (CBMParamCalculator*)info.pCalculator;
		if (NULL == pCalculator || pCalculator->m_bOnlyHoist)
			continue;

		CString strTemplateFile = GetMainGirderTemplateFile(_T("Offer"), strCompany, strLanguage, pCalculator->m_CalcDatalst.GA13, pCalculator->m_CalcDatalst.DB23, pCalculator->m_CalcDatalst.DB4, pCalculator->m_CalcDatalst.EC13);
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

		BeginWaitCursor();
		ExportMainGirderDataToWordTemplate(&wordApp, pCalculator, iLangIndex);

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
		_stprintf(tcOutputFile, _T("%s\\%s%s%s"), strDrawingFolder, _T("Offer_MG_"), tcComName, _T(".doc"));
		wordApp.SaveWordFile(tcOutputFile);
		EndWaitCursor();
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::ExportEndCarriageToWordTemplate(ComponentPriceList lstSelPrices, 
													   CString strEndCarFolder)
{
	CString strCompany;
	CString strLanguage;
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_COMPANY, strCompany);
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_LANGUAGE, strLanguage);

	for (ComponentPriceList::iterator it = lstSelPrices.begin();
		it != lstSelPrices.end();
		it++)
	{
		ComPriceInfo info = *it;
		CBMParamCalculator* pCalculator = (CBMParamCalculator*)info.pCalculator;
		if (NULL == pCalculator || pCalculator->m_bOnlyHoist)
			continue;

		CString strTemplateFile = GetEndCarTemplateFile(_T("Offer"), strCompany, strLanguage, pCalculator->m_CalcDatalst.DB23, pCalculator->m_CalcDatalst.DB31, pCalculator->m_CalcDatalst.DB4);
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
		BeginWaitCursor();

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
		_stprintf(tcOutputFile, _T("%s\\%s%s%s"), strEndCarFolder, _T("Offer_EC_"), tcComName, _T(".doc"));
		wordApp.SaveWordFile(tcOutputFile);
		EndWaitCursor();
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::ExportSteelStructureToWordTemplate(ComponentPriceList lstSelPrices, 
														  CString strSteelStructureFolder)
{	
	CString strCompany;
	CString strLanguage;
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_COMPANY, strCompany);
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_LANGUAGE, strLanguage);

	for (ComponentPriceList::iterator it = lstSelPrices.begin();
		it != lstSelPrices.end();
		it++)
	{
		ComPriceInfo info = *it;
		CBMParamCalculator* pCalculator = (CBMParamCalculator*)info.pCalculator;
		if (NULL == pCalculator || pCalculator->m_bOnlyHoist)
			continue;

		CString strTemplateFile = GetSteelStructureTemplateFile(_T("Offer"), strCompany, strLanguage, pCalculator->m_CalcDatalst.GA12, pCalculator->m_CalcDatalst.DB51, pCalculator->m_CalcDatalst.MB81);
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
		BeginWaitCursor();

		// Project Name
		wordApp.SetTableCell(1, 2, 2, m_strProjectName);
		
		// Component Name
		lstTextPartner.push_back(StringPartner(_T("(equicode)"), info.strComName));

		// Component Count
		strTemp.Format(_T("%d"), info.iComCount);
		lstTextPartner.push_back(StringPartner(_T("(ecunit)"), strTemp));
		wordApp.ReplaceTexts(lstTextPartner);
		
		ExportSteelStructureDataToWordTemplate(&wordApp, pCalculator);

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
		_stprintf(tcOutputFile, _T("%s\\%s%s%s"), strSteelStructureFolder, _T("Offer_SS_"), tcComName, _T(".doc"));
		wordApp.SaveWordFile(tcOutputFile);
		EndWaitCursor();
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::ExportMainGirderCalcDataToTemplate(ComponentPriceList lstSelPrices, CString strSaveFolder)
{
	CString strLanguage;
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_LANGUAGE, strLanguage);

	for (ComponentPriceList::iterator it = lstSelPrices.begin();
		it != lstSelPrices.end();
		it++)
	{
		ComPriceInfo info = *it;
		CBMParamCalculator* pCalculator = (CBMParamCalculator*)info.pCalculator;
		if (NULL == pCalculator || pCalculator->m_bOnlyHoist)
			continue;
	
		CString strTemplateFile = GetMainGirderCalcDataTemplateFile(strLanguage, pCalculator->m_CalcDatalst.GA12);
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

		BeginWaitCursor();
		// Component Name
		pCalculator->m_CalcDatalst.equicode = info.strComName;
		ExportMainGirderCalcDataForTest(&wordApp, pCalculator);

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
		_stprintf(tcOutputFile, _T("%s\\%s%s%s"), strSaveFolder, _T("Offer_MGCalcData_"), tcComName, _T(".doc"));
		wordApp.SaveWordFile(tcOutputFile);
		EndWaitCursor();
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOfferMakerDlg::ExportEndCarriageCalcDataToTemplate(ComponentPriceList lstSelPrices, CString strSaveFolder)
{
	CString strLanguage;
	GetDlgItemText(IDC_COMBO_OFFERMAKERDLG_LANGUAGE, strLanguage);

	for (ComponentPriceList::iterator it = lstSelPrices.begin();
		it != lstSelPrices.end();
		it++)
	{
		ComPriceInfo info = *it;
		CBMParamCalculator* pCalculator = (CBMParamCalculator*)info.pCalculator;
		if (NULL == pCalculator || pCalculator->m_bOnlyHoist)
			continue;
	
		CString strTemplateFile = GetEndCarriageCalcDataTemplateFile(strLanguage, pCalculator->m_CalcDatalst.DB23);
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

		BeginWaitCursor();
		
		// Component Name
		pCalculator->m_CalcDatalst.equicode = info.strComName;
		ExportEndCarriageCalcDataForTest(&wordApp, pCalculator);

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
		_stprintf(tcOutputFile, _T("%s\\%s%s%s"), strSaveFolder, _T("Offer_ECCalcData_"), tcComName, _T(".doc"));
		wordApp.SaveWordFile(tcOutputFile);
		EndWaitCursor();
	}

	return TRUE;
}
//-----------------------------------------------------------------------
void CBMOfferMakerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd* pFakeBtn1 = GetDlgItem(IDC_STATIC_FAKEBTN1);
	CWnd* pFakeBtn2 = GetDlgItem(IDC_STATIC_FAKEBTN2);
	CWnd* pFakeBtn3 = GetDlgItem(IDC_STATIC_FAKEBTN3);
	CRect rcFakeBtn1, rcFakeBtn2, rcFakeBtn3;
	pFakeBtn1->GetWindowRect(&rcFakeBtn1);
	pFakeBtn2->GetWindowRect(&rcFakeBtn2);
	pFakeBtn3->GetWindowRect(&rcFakeBtn3);
	ScreenToClient(&rcFakeBtn1);
	ScreenToClient(&rcFakeBtn2);
	ScreenToClient(&rcFakeBtn3);

	if (rcFakeBtn1.PtInRect(point) == true)
	{
	}
	else if(rcFakeBtn2.PtInRect(point) == true)
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
			ExportMainGirderCalcDataToTemplate(lstSelectedPrices, tcPath);
			if (IDYES == myMessageBox(IDS_OFFERMAKERDLG_PRINTDRAWINGSUCCESS, IDS_MSGDLG_SUCCESS, MB_YESNO | MB_ICONINFORMATION))
				ShellExecute(NULL, _T("open"), tcPath, NULL, NULL, SW_MAXIMIZE);
		} 
	}
	else if(rcFakeBtn3.PtInRect(point) == true)
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
			ExportEndCarriageCalcDataToTemplate(lstSelectedPrices, tcPath);
			if (IDYES == myMessageBox(IDS_OFFERMAKERDLG_PRINTDRAWINGSUCCESS, IDS_MSGDLG_SUCCESS, MB_YESNO | MB_ICONINFORMATION))
				ShellExecute(NULL, _T("open"), tcPath, NULL, NULL, SW_MAXIMIZE);
		} 
	}

	CDialog::OnLButtonUp(nFlags, point);
}
//-----------------------------------------------------------------------
