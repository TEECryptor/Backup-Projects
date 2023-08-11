// PlainTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SynoTFKeySDKTest.h"
#include "PlainTextDlg.h"


// CPlainTextDlg dialog

IMPLEMENT_DYNAMIC(CPlainTextDlg, CDialog)

CPlainTextDlg::CPlainTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlainTextDlg::IDD, pParent)
{
	m_strPlainText = _T("");
	m_ulTestType = 0;
	m_ulAlgId = 0;
}

CPlainTextDlg::~CPlainTextDlg()
{
}

void CPlainTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlainTextDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPlainTextDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPlainTextDlg message handlers


BOOL CPlainTextDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ULONG ulIndex = 0;
	CComboBox* pAlgComboBox = ((CComboBox*)GetDlgItem(IDC_PLAINTEXTDLG_COMBO_ALG));
	pAlgComboBox->ResetContent();	
	switch (m_ulTestType)
	{
	case 1:		//测试签名/验签数据
		ulIndex = pAlgComboBox->AddString(_T("RSA"));
		pAlgComboBox->SetItemData(ulIndex, USBKEY_SIGNALG_RSA);		
		ulIndex = pAlgComboBox->AddString(_T("RSA_MD5"));
		pAlgComboBox->SetItemData(ulIndex, USBKEY_SIGNALG_RSA_MD5);
		ulIndex = pAlgComboBox->AddString(_T("RSA_SHA1"));
		pAlgComboBox->SetItemData(ulIndex, USBKEY_SIGNALG_RSA_SHA1);
		ulIndex = pAlgComboBox->AddString(_T("RSA_SHA256"));
		pAlgComboBox->SetItemData(ulIndex, USBKEY_SIGNALG_RSA_SHA256);
		//ulIndex = pAlgComboBox->AddString(_T("SM2"));
		//pAlgComboBox->SetItemData(ulIndex, USBKEY_ASYMMALG_RSA);
		break;
	case 2:		//测试加密/解密数据
		ulIndex = pAlgComboBox->AddString(_T("RSA"));
		pAlgComboBox->SetItemData(ulIndex, USBKEY_ASYMMALG_RSA);
		//ulIndex = pAlgComboBox->AddString(_T("SM2"));
		//pAlgComboBox->SetItemData(ulIndex, USBKEY_ASYMMALG_RSA);
		ulIndex = pAlgComboBox->AddString(_T("DES"));
		pAlgComboBox->SetItemData(ulIndex, USBKEY_SYMMALG_DES);
		ulIndex = pAlgComboBox->AddString(_T("3DES"));
		pAlgComboBox->SetItemData(ulIndex, USBKEY_SYMMALG_3DES);
		break;
	case 3:		//测试数字信封
		ulIndex = pAlgComboBox->AddString(_T("RSA_DES"));
		pAlgComboBox->SetItemData(ulIndex, USBKEY_SYMMALG_DES);
		ulIndex = pAlgComboBox->AddString(_T("RSA_3DES"));
		pAlgComboBox->SetItemData(ulIndex, USBKEY_SYMMALG_3DES);
		break;
	default:
		break;
	}
	if (pAlgComboBox->GetCount() > 0)
	{
		pAlgComboBox->SetCurSel(0);
	}

	GetDlgItem(IDC_PLAINTEXTDLG_EDIT_PLAIN)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
}


void CPlainTextDlg::OnBnClickedOk()
{
	ULONG ulSelIndex = 0;
	CComboBox* pAlgComboBox = ((CComboBox*)GetDlgItem(IDC_PLAINTEXTDLG_COMBO_ALG));
	ulSelIndex = pAlgComboBox->GetCurSel();
	m_ulAlgId = pAlgComboBox->GetItemData(ulSelIndex);
	//
	GetDlgItemText(IDC_PLAINTEXTDLG_EDIT_PLAIN, m_strPlainText);

	CDialog::OnOK();
}
