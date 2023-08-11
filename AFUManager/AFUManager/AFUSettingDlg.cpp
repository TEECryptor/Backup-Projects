// AFUSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AFUManager.h"
#include "AFUSettingDlg.h"


// CAFUSettingDlg dialog

IMPLEMENT_DYNAMIC(CAFUSettingDlg, CDialog)

CAFUSettingDlg::CAFUSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAFUSettingDlg::IDD, pParent)
{

}

CAFUSettingDlg::~CAFUSettingDlg()
{
}

void CAFUSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAFUSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAFUSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_MULTIPAGE, &CAFUSettingDlg::OnBnClickedRadioMultipage)
	ON_BN_CLICKED(IDC_RADIO_SINGLEPAGE, &CAFUSettingDlg::OnBnClickedRadioSinglepage)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CAFUSettingDlg::OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CAFUSettingDlg message handlers

BOOL CAFUSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strCountPerPage;
	strCountPerPage.Format(_T("%d"), m_lRecordCntPerPage);
	SetDlgItemText(IDC_EDIT_DBFILE, m_strDBFile);
	SetDlgItemText(IDC_EDIT_RECORDCNT, strCountPerPage);
	if (TRUE == m_bIsMultiPage)
	{
		((CButton*)GetDlgItem(IDC_RADIO_MULTIPAGE))->SetCheck(BST_CHECKED);
		OnBnClickedRadioMultipage();
	}
	else
	{		
		((CButton*)GetDlgItem(IDC_RADIO_SINGLEPAGE))->SetCheck(BST_CHECKED);
		OnBnClickedRadioSinglepage();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAFUSettingDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_DBFILE, m_strDBFile);

	if (TRUE == m_bIsMultiPage)
	{
		CString strCountPerPage = 0;
		GetDlgItemText(IDC_EDIT_RECORDCNT, strCountPerPage);
		m_lRecordCntPerPage = _ttol(strCountPerPage);
	}

	OnOK();
}
void CAFUSettingDlg::OnBnClickedRadioMultipage()
{
	m_bIsMultiPage = TRUE;
	GetDlgItem(IDC_EDIT_RECORDCNT)->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_RECORDCNT))->SetFocus();
}

void CAFUSettingDlg::OnBnClickedRadioSinglepage()
{
	m_bIsMultiPage = FALSE;
	GetDlgItem(IDC_EDIT_RECORDCNT)->EnableWindow(FALSE);
}
void CAFUSettingDlg::OnBnClickedButtonBrowse()
{
	CFileDialog dlgOpen(TRUE, NULL, NULL,  OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("MS Access Files (*.mdb)|*.mdb|All Files (*.*)|*.*||"));
	if (IDOK == dlgOpen.DoModal())
	{
	}
}



void CAFUSettingDlg::SetInitializeData(CString strDBFile, 
									   BOOL bIsMultiPage, 
									   LONG lRecordCntPerPage)
{
	m_strDBFile = strDBFile;
	m_bIsMultiPage = bIsMultiPage;
	m_lRecordCntPerPage = lRecordCntPerPage;
}

void CAFUSettingDlg::GetSettingData(CString &strDBFile, 
									BOOL &bIsMultiPage, 
									LONG &lRecordCntPerPage)
{
	strDBFile = m_strDBFile;
	bIsMultiPage = m_bIsMultiPage;
	lRecordCntPerPage = m_lRecordCntPerPage;
}
