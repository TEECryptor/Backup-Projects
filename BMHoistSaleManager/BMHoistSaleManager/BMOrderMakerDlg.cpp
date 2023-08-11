//-----------------------------------------------------------------------
//	File Name:BMOrderMakerDlg.cpp
//	Introduce:Class CBMOrderMakerDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMOrderMakerDlg.h"
#include <shlwapi.h>
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOrderMakerDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMOrderMakerDlg, CDialog)
CBMOrderMakerDlg::CBMOrderMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMOrderMakerDlg::IDD, pParent)
{
	for (int i = 0; i < MAX_COMPONENT_CNT; i++)
		m_pOrderOptionsDlg[i] = NULL;
}
//-----------------------------------------------------------------------
CBMOrderMakerDlg::~CBMOrderMakerDlg()
{
}
//-----------------------------------------------------------------------
void CBMOrderMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_ORDEROPTIONS, m_tabOptions);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMOrderMakerDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ORDERDLG_PRINT, &CBMOrderMakerDlg::OnBnClickedBtnOrderdlgPrint)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ORDEROPTIONS, &CBMOrderMakerDlg::OnTcnSelchangeTabOrderoptions)
	ON_BN_CLICKED(IDC_BTN_ORDERDLG_ENDCAR, &CBMOrderMakerDlg::OnBnClickedBtnOrderdlgEndcar)
	ON_BN_CLICKED(IDC_BTN_ORDERDLG_MAINGIRDER, &CBMOrderMakerDlg::OnBnClickedBtnOrderdlgMaingirder)
	ON_BN_CLICKED(IDC_BTN_ORDERDLG_STEELSTRUCTURE, &CBMOrderMakerDlg::OnBnClickedBtnOrderdlgSteelstructure)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMOrderMakerDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMOrderMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
/*
	LOGFONT lf;
	CFont* pDlgFont = GetFont();
	pDlgFont->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	lf.lfHeight = -MulDiv(8, GetDeviceCaps(CPaintDC(this).GetSafeHdc(), LOGPIXELSY), 72);
	CFont tabFont;
	tabFont.CreateFontIndirect(&lf);
	m_tabOptions.SetFont(&tabFont);*/
	m_tabOptions.SetBkColor(RGB(239, 235, 222));

	InitCaptionText();
	InitComponentTabCtrl();
	if (m_tabOptions.GetItemCount() > 0)
	{
		m_pOrderOptionsDlg[0]->ShowWindow(SW_SHOW);
		m_pOrderOptionsDlg[0]->UpdateWindow();
	}

	CTime tmDate = CTime::GetCurrentTime();
	CDateTimeCtrl* pDateDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATE_ORDERDLG_DELIVERYDATE);
	pDateDateCtrl->SetTime(&tmDate);
	//
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ORDERDLG_DELIVERYTYPE);
	pComboBox->AddString(_T("A"));
	pComboBox->AddString(_T("B"));
	pComboBox->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMOrderMakerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	for (int i = 0; i < MAX_COMPONENT_CNT; i++)
	{
		if (NULL != m_pOrderOptionsDlg[i])
		{
			delete m_pOrderOptionsDlg[i];
			m_pOrderOptionsDlg[i] = NULL;
		}
	}
}
//-----------------------------------------------------------------------
void CBMOrderMakerDlg::OnTcnSelchangeTabOrderoptions(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iCurTabIndex = m_tabOptions.GetCurSel();
	int iTotalItem = m_tabOptions.GetItemCount();
	for (int iIndex = 0; iIndex < iTotalItem; iIndex++)
	{
		if (NULL == m_pOrderOptionsDlg[iIndex])
			continue;

		if (iIndex == iCurTabIndex)
		{
			m_pOrderOptionsDlg[iIndex]->ShowWindow(SW_SHOW);
			m_pOrderOptionsDlg[iIndex]->UpdateWindow();
		}
		else
		{
			m_pOrderOptionsDlg[iIndex]->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}
//-----------------------------------------------------------------------
void CBMOrderMakerDlg::OnBnClickedBtnOrderdlgPrint()
{	
	CFileDialog dlg(FALSE, _T("doc"), (LPCTSTR)m_strProjectName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Word Document (*.doc)|*.doc|Word 2007 Document (*.docx)|*.docx||"));
	if (dlg.DoModal() == IDOK)
	{
		CString strDocFile = dlg.GetPathName();
		if (ExportToWordTemplate(strDocFile))
		{			
			if (IDYES == myMessageBox(IDS_ORDERMAKERDLG_PRINTSUCCESS, IDS_MSGDLG_SUCCESS, MB_YESNO | MB_ICONINFORMATION))
				ShellExecute(NULL, _T("open"), strDocFile, NULL, NULL, SW_MAXIMIZE);
		}
	}
}
//-----------------------------------------------------------------------
void CBMOrderMakerDlg::OnBnClickedBtnOrderdlgMaingirder()
{
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
		if (ExportMainGirderToWordTemplate(tcPath))
		{			
			if (IDYES == myMessageBox(IDS_ORDERMAKERDLG_PRINTENDCARSUCCESS, IDS_MSGDLG_SUCCESS, MB_YESNO | MB_ICONINFORMATION))
				ShellExecute(NULL, _T("open"), tcPath, NULL, NULL, SW_MAXIMIZE);
		}
	} 
}
//-----------------------------------------------------------------------
void CBMOrderMakerDlg::OnBnClickedBtnOrderdlgEndcar()
{
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
		if (ExportEndCarriageToWordTemplate(tcPath))
		{			
			if (IDYES == myMessageBox(IDS_ORDERMAKERDLG_PRINTENDCARSUCCESS, IDS_MSGDLG_SUCCESS, MB_YESNO | MB_ICONINFORMATION))
				ShellExecute(NULL, _T("open"), tcPath, NULL, NULL, SW_MAXIMIZE);
		}
	} 
}
//-----------------------------------------------------------------------
void CBMOrderMakerDlg::OnBnClickedBtnOrderdlgSteelstructure()
{
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
		if (ExportSteelStructureToWordTemplate(tcPath))
		{			
			if (IDYES == myMessageBox(IDS_ORDERMAKERDLG_PRINTENDCARSUCCESS, IDS_MSGDLG_SUCCESS, MB_YESNO | MB_ICONINFORMATION))
				ShellExecute(NULL, _T("open"), tcPath, NULL, NULL, SW_MAXIMIZE);
		}
	} 
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMOrderMakerDlg::SetOfferNo(CString strOfferNo)
{
	m_strOfferNo = strOfferNo;
}
//-----------------------------------------------------------------------
void CBMOrderMakerDlg::InitCaptionText()
{
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE OfferNo='%s'", g_tcTableName[eTableID_OffersInfo], m_strOfferNo);

	BOOL bResult = TRUE;
	CADOTable tbOfferInfo;
	bResult = tbOfferInfo.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult || tbOfferInfo.GetRecordCnt() == 0)
		return;

	CString strCompany;
	CString strProjectName;
	CString strCustomer;
	TCHAR tcTemp[256] = {0};
	tbOfferInfo.GetRecordCell(0, L"CompanyShortName", tcTemp, 256);	strCompany = tcTemp;
	tbOfferInfo.GetRecordCell(0, L"ProjectName", tcTemp, 256);		strProjectName = tcTemp;
	tbOfferInfo.GetRecordCell(0, L"ContactName", tcTemp, 256);		strCustomer = tcTemp;
	tbOfferInfo.CloseTable();
	m_strProjectName = strProjectName;
	//
	SetDlgItemText(IDC_EDIT_ORDERDLG_PROJECTNAME, strProjectName);
	SetDlgItemText(IDC_EDIT_ORDERDLG_CUSTOMER, strCustomer);
	m_strDlgCaption.Format(_T("%s + %s"), strCompany, strProjectName);
	SetWindowText(m_strDlgCaption);

	CString strFullCompany;
	CString strAddress;
	CString strTel;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE CompanyShortName='%s'", g_tcTableName[eTableID_CustomerInfo], strCompany);
	strFullCompany = GetStringValueFromTable(strOpenOperation, L"FullName", g_pOfferDatabase);
	strAddress = GetStringValueFromTable(strOpenOperation, L"Address", g_pOfferDatabase);
	strOpenOperation.Format(L"SELECT * FROM %s WHERE ContactName='%s'", g_tcTableName[eTableID_ContactsInfo], strCustomer);
	strTel = GetStringValueFromTable(strOpenOperation, L"CellPhone", g_pOfferDatabase);
	SetDlgItemText(IDC_EDIT_ORDERDLG_COMPANY, strFullCompany);
	SetDlgItemText(IDC_EDIT_ORDERDLG_ADDRESS, strAddress);
	SetDlgItemText(IDC_EDIT_ORDERDLG_PHONE, strTel);	
}
//-----------------------------------------------------------------------
void CBMOrderMakerDlg::InitComponentTabCtrl()
{
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE OfferNo='%s'", g_tcTableName[eTableID_ComponentInfo], m_strOfferNo);

	BOOL bResult = TRUE;
	CADOTable tbComponent;
	bResult = tbComponent.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult || tbComponent.GetRecordCnt() == 0)
		return;

	CRect rcClient;
	m_tabOptions.GetClientRect(&rcClient);
	rcClient.left += 4;
	rcClient.right -= 4;
	rcClient.top += 22;
	rcClient.bottom -= 4;

	int iRealIndex = 0;
	DWORD dwComponentCnt = tbComponent.GetRecordCnt();
	for (DWORD dwIndex = 0; dwIndex < dwComponentCnt; dwIndex++)
	{
		TCHAR tcComName[64] = _T("\0");
		TCHAR tcHoistCode[64] = _T("\0");
		DWORD dwLoadOfCrane = 0;
		DWORD dwLoadOfHoist = 0;
		tbComponent.GetRecordCell(dwIndex, L"ComponentName", tcComName, 64);
		tbComponent.GetRecordCell(dwIndex, L"HoistCode", tcHoistCode, 64);
		tbComponent.GetRecordCell(dwIndex, L"CraneLoad", dwLoadOfCrane);
		tbComponent.GetRecordCell(dwIndex, L"HoistLoad", dwLoadOfHoist);
		if (_tcslen(tcHoistCode) == 0 || 0 == dwLoadOfCrane || 0 == dwLoadOfHoist)
			continue;

		CString strTabText;
		strTabText.Format(_T("%s: %s "), g_tcTabIndex[dwIndex], tcComName);
		m_tabOptions.InsertItem(iRealIndex, strTabText);
		//
		m_pOrderOptionsDlg[iRealIndex] = new CBMOrderOptionDlg();
		m_pOrderOptionsDlg[iRealIndex]->SetOfferNoAndComName(m_strOfferNo, tcComName);
		m_pOrderOptionsDlg[iRealIndex]->Create(IDD_DIALOG_ORDEROPTIONS, &m_tabOptions);
		m_pOrderOptionsDlg[iRealIndex]->MoveWindow(&rcClient);
		iRealIndex++;
	}
	tbComponent.CloseTable();
}
//-----------------------------------------------------------------------
BOOL CBMOrderMakerDlg::ExportToWordTemplate(CString strDocFile)
{
	CString strTemplateFile = GetOfferOrderTemplateFile(_T("OrderTemplates"), _T("BM"), _T("CN"), m_tabOptions.GetItemCount());
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
	
	CTime tmDate;
	CString strCompany;
	CString strCustomer;
	CString strAddress;
	CString strTel;
	CString strDeliveryDate;
	CString strDeliveryType;
	CString strNote;
	GetDlgItemText(IDC_EDIT_ORDERDLG_COMPANY, strCompany);
	GetDlgItemText(IDC_EDIT_ORDERDLG_CUSTOMER, strCustomer);
	GetDlgItemText(IDC_EDIT_ORDERDLG_ADDRESS, strAddress);
	GetDlgItemText(IDC_EDIT_ORDERDLG_PHONE, strTel);
	GetDlgItemText(IDC_COMBO_ORDERDLG_DELIVERYTYPE, strDeliveryType);
	GetDlgItemText(IDC_EDIT_ORDERDLG_NOTE, strNote);
	CDateTimeCtrl* pDateDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATE_ORDERDLG_DELIVERYDATE);
	pDateDateCtrl->GetTime(tmDate);
	strDeliveryDate.Format(_T("%d-%02d-%02d"), tmDate.GetYear(), tmDate.GetMonth(), tmDate.GetDay());

	// Export general information and setting
	StringPartnerList lstTextPartner;
	lstTextPartner.push_back(StringPartner(_T("(cucompany)"), strCompany));
	lstTextPartner.push_back(StringPartner(_T("(customer)"), strCustomer));
	lstTextPartner.push_back(StringPartner(_T("(cuadd)"), strAddress));
	lstTextPartner.push_back(StringPartner(_T("(cumobile)"), strTel));
	lstTextPartner.push_back(StringPartner(_T("(delidata)"), strDeliveryDate));
	lstTextPartner.push_back(StringPartner(_T("(deliverytype)"), strDeliveryType));
	lstTextPartner.push_back(StringPartner(_T("(specialnote2)"), strNote));
	wordApp.ReplaceTexts(lstTextPartner);
	
	CString strProjectName;
	CString strOrderNumber;
	CString strSellingPrice;
	GetDlgItemText(IDC_EDIT_ORDERDLG_PROJECTNAME, strProjectName);
	GetDlgItemText(IDC_EDIT_ORDERDLG_ORDERNUMBER, strOrderNumber);
	GetDlgItemText(IDC_EDIT_ORDERDLG_PRICE, strSellingPrice);
	wordApp.SetTableCell(1, 1, 2, strProjectName);
	wordApp.SetTableCell(1, 1, 4, strOrderNumber);
	wordApp.SetTableCell(1, 1, 6, strSellingPrice);

	for (int i = 0; i < MAX_COMPONENT_CNT; i++)
	{
		if (m_pOrderOptionsDlg[i] == NULL)
			continue;
		
		CString strComName;
		CString strComType;
		CString strComCount;
		CString strComNote;
		CString strComAdditional;
		int iComCount = 0;
		m_pOrderOptionsDlg[i]->GetGeneralInfo(strComName, strComType, iComCount, strComNote, strComAdditional);
		strComCount.Format(_T("%d"), iComCount);
		wordApp.SetTableCell(1, 2+i, 2, strComName);
		wordApp.SetTableCell(1, 2+i, 4, strComType);
		wordApp.SetTableCell(1, 2+i, 6, strComCount);
		//
		wordApp.SetTableCell(2+i, 1, 2, strComName);
		wordApp.SetTableCell(2+i, 1, 4, strComType);
		wordApp.SetTableCell(2+i, 1, 6, strComCount);

		for (int j = 0; j < eComponentType_Cnt; j++)
		{
			CString strCode;
			CString strFreeText;
			CString strCount;
			int iCount = 0;
			if (m_pOrderOptionsDlg[i]->GetComponentInfo(eComponentType(j), strCode, iCount, strFreeText))
			{
				strCount.Format(_T("%d"), iCount);
				wordApp.SetTableCell(2+i, 3+j, 2, strCount);
				wordApp.SetTableCell(2+i, 3+j, 4, strCode);
				wordApp.SetTableCell(2+i, 3+j, 5, strFreeText);		
			}
		}

		wordApp.SetTableCell(2+i, 10, 4, strComNote);
		wordApp.SetTableCell(2+i, 11, 4, strComAdditional);		
	}

	hr = wordApp.SaveWordFile(strDocFile);
	EndWaitCursor();

	return (S_OK == hr) ? TRUE : FALSE;
}
//-----------------------------------------------------------------------
BOOL CBMOrderMakerDlg::ExportMainGirderToWordTemplate(CString strMainGirderFolder)
{
	for (int i = 0; i < MAX_COMPONENT_CNT; i++)
	{
		if (m_pOrderOptionsDlg[i] == NULL)
			continue;
		
		CBMParamCalculator* pCalculator = m_pOrderOptionsDlg[i]->GetCalculator();
		if (pCalculator == NULL || pCalculator->m_bOnlyHoist)
			continue;

		CString strTemplateFile = GetMainGirderTemplateFile(_T("Order"), _T("BM"), _T("CN"), pCalculator->m_CalcDatalst.GA13, pCalculator->m_CalcDatalst.DB23, pCalculator->m_CalcDatalst.DB4, pCalculator->m_CalcDatalst.EC13);
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

		CString strComName;
		CString strComType;
		CString strComCount;
		CString strComNote;
		CString strComAdditional;
		int iComCount = 0;
		m_pOrderOptionsDlg[i]->GetGeneralInfo(strComName, strComType, iComCount, strComNote, strComAdditional);
		
		ExportMainGirderDataToWordTemplate(&wordApp, pCalculator, 1);

		const TCHAR tcRepace[] = {_T('\\'), _T('/'), _T(':'), _T('*'), _T('?'), _T('"'), _T('<'), _T('>'), _T('|')};
		TCHAR tcComName[MAX_PATH] = {0};
		_tcscpy(tcComName, strComName);
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
		_stprintf(tcOutputFile, _T("%s\\%s%s%s"), strMainGirderFolder, _T("Order_MG_"), tcComName, _T(".doc"));
		wordApp.SaveWordFile(tcOutputFile);
		EndWaitCursor();
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOrderMakerDlg::ExportEndCarriageToWordTemplate(CString strEndCarFolder)
{
	for (int i = 0; i < MAX_COMPONENT_CNT; i++)
	{
		if (m_pOrderOptionsDlg[i] == NULL)
			continue;
		
		CBMParamCalculator* pCalculator = m_pOrderOptionsDlg[i]->GetCalculator();
		if (pCalculator == NULL || pCalculator->m_bOnlyHoist)
			continue;

		CString strTemplateFile = GetEndCarTemplateFile(_T("Order"), _T("BM"), _T("CN"), pCalculator->m_CalcDatalst.DB23, pCalculator->m_CalcDatalst.DB31, pCalculator->m_CalcDatalst.DB4);
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

		CString strComName;
		CString strComType;
		CString strComCount;
		CString strComNote;
		CString strComAdditional;
		int iComCount = 0;
		m_pOrderOptionsDlg[i]->GetGeneralInfo(strComName, strComType, iComCount, strComNote, strComAdditional);

		CString strTemp;
		StringPartnerList lstTextPartner;

		// Project Name
		wordApp.SetTableCell(1, 2, 2, m_strProjectName);
		
		// Component Name
		lstTextPartner.push_back(StringPartner(_T("(equicode)"), strComName));

		// Component Count
		strTemp.Format(_T("%d"), iComCount);
		lstTextPartner.push_back(StringPartner(_T("(ecunit)"), strTemp));
		wordApp.ReplaceTexts(lstTextPartner);
		
		ExportEndCarriageDataToWordTemplate(&wordApp, pCalculator);

		const TCHAR tcRepace[] = {_T('\\'), _T('/'), _T(':'), _T('*'), _T('?'), _T('"'), _T('<'), _T('>'), _T('|')};
		TCHAR tcComName[MAX_PATH] = {0};
		_tcscpy(tcComName, strComName);
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
		_stprintf(tcOutputFile, _T("%s\\%s%s%s"), strEndCarFolder, _T("Order_EC_"), tcComName, _T(".doc"));
		wordApp.SaveWordFile(tcOutputFile);
		EndWaitCursor();
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMOrderMakerDlg::ExportSteelStructureToWordTemplate(CString strSteelStructureFolder)
{
	for (int i = 0; i < MAX_COMPONENT_CNT; i++)
	{
		if (m_pOrderOptionsDlg[i] == NULL)
			continue;
		
		CBMParamCalculator* pCalculator = m_pOrderOptionsDlg[i]->GetCalculator();
		if (pCalculator == NULL || pCalculator->m_bOnlyHoist)
			continue;

		CString strTemplateFile = GetSteelStructureTemplateFile(_T("Order"), _T("BM"), _T("CN"), pCalculator->m_CalcDatalst.DB23, pCalculator->m_CalcDatalst.DB31, pCalculator->m_CalcDatalst.DB4);
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

		CString strComName;
		CString strComType;
		CString strComCount;
		CString strComNote;
		CString strComAdditional;
		int iComCount = 0;
		m_pOrderOptionsDlg[i]->GetGeneralInfo(strComName, strComType, iComCount, strComNote, strComAdditional);

		CString strTemp;
		StringPartnerList lstTextPartner;

		// Project Name
		wordApp.SetTableCell(1, 2, 2, m_strProjectName);
		
		// Component Name
		lstTextPartner.push_back(StringPartner(_T("(equicode)"), strComName));

		// Component Count
		strTemp.Format(_T("%d"), iComCount);
		lstTextPartner.push_back(StringPartner(_T("(ecunit)"), strTemp));
		wordApp.ReplaceTexts(lstTextPartner);
		
		ExportSteelStructureDataToWordTemplate(&wordApp, pCalculator);

		const TCHAR tcRepace[] = {_T('\\'), _T('/'), _T(':'), _T('*'), _T('?'), _T('"'), _T('<'), _T('>'), _T('|')};
		TCHAR tcComName[MAX_PATH] = {0};
		_tcscpy(tcComName, strComName);
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
		_stprintf(tcOutputFile, _T("%s\\%s%s%s"), strSteelStructureFolder, _T("Order_SS_"), tcComName, _T(".doc"));
		wordApp.SaveWordFile(tcOutputFile);
		EndWaitCursor();
	}

	return TRUE;
}
//-----------------------------------------------------------------------