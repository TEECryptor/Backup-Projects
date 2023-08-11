// NewDataFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SynoTFKeySDKTest.h"
#include "NewDataFileDlg.h"
#include "afxdialogex.h"


// CNewDataFileDlg dialog

IMPLEMENT_DYNAMIC(CNewDataFileDlg, CDialog)

CNewDataFileDlg::CNewDataFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDataFileDlg::IDD, pParent)
{

}

CNewDataFileDlg::~CNewDataFileDlg()
{
}

void CNewDataFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewDataFileDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNewDataFileDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewDataFileDlg message handlers


BOOL CNewDataFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_NEWDATAFILEDLG_EDIT_FILENAME)->SetFocus();

	return FALSE; 
}

void CNewDataFileDlg::OnBnClickedOk()
{
	CString strFileSize;

	//	�ļ���
	GetDlgItemText(IDC_NEWDATAFILEDLG_EDIT_FILENAME, m_strFileName);
	if (m_strFileName.GetLength() <= 0)
	{		
		MessageBox(_T("�ļ�������Ϊ��!"), _T("��ʾ"));
		GetDlgItem(IDC_NEWDATAFILEDLG_EDIT_FILENAME)->SetFocus();
		return;
	}

	//	�ļ�����
	GetDlgItemText(IDC_NEWDATAFILEDLG_EDIT_FILEDATA, m_strFileData);
	if (m_strFileData.GetLength() <= 0)
	{		
		MessageBox(_T("�ļ����ݲ���Ϊ��!"), _T("��ʾ"));
		GetDlgItem(IDC_NEWDATAFILEDLG_EDIT_FILEDATA)->SetFocus();
		return;
	}

	CDialog::OnOK();
}

