// PSIDToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PSIDTool.h"
#include "PSIDToolDlg.h"
#include ".\psidtooldlg.h"
#include "ComDefine.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CPSIDToolDlg dialog



CPSIDToolDlg::CPSIDToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPSIDToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPSIDToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPSIDToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPENSETTING, OnBnClickedBtnOpensetting)
	ON_BN_CLICKED(IDC_BTN_OPENSETTING2, OnBnClickedBtnOpensetting2)
	ON_BN_CLICKED(IDC_BTN_OPENSETTING3, OnBnClickedBtnOpensetting3)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CPSIDToolDlg message handlers

BOOL CPSIDToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	DWORD dwClr = RGB(255, 0, 0);
	TCHAR csClr[32] = {_T("\0")};
	_stprintf(_T(csClr), _T("%02X%02X%02X"), GetRValue(dwClr), GetGValue(dwClr), GetBValue(dwClr));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPSIDToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPSIDToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPSIDToolDlg::OnBnClickedBtnOpensetting()
{
	TCHAR szPath[MAX_PATH] = {_T("\0")};
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath = szPath;
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));

	CFileDialog dlgOpen(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, _T("*.exe|*.exe||"), this);
	dlgOpen.m_ofn.lpstrTitle = strPath;
	if(dlgOpen.DoModal() == IDOK)
	{
		CString strFileName = dlgOpen.GetPathName();
		SetDlgItemText(IDC_EDIT_SETTING, strFileName);
	}
}

void CPSIDToolDlg::OnBnClickedBtnOpensetting2()
{
	TCHAR szPath[MAX_PATH] = {_T("\0")};
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath = szPath;
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));

	CFileDialog dlgOpen(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, _T("*.exe|*.exe||"), this);
	dlgOpen.m_ofn.lpstrTitle = strPath;
	if(dlgOpen.DoModal() == IDOK)
	{
		CString strFileName = dlgOpen.GetPathName();
		SetDlgItemText(IDC_EDIT_INPUT, strFileName);
	}
}

void CPSIDToolDlg::OnBnClickedBtnOpensetting3()
{
	TCHAR szPath[MAX_PATH] = {_T("\0")};
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath = szPath;
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));

	CFileDialog dlgOpen(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, _T("*.exe|*.exe||"), this);
	dlgOpen.m_ofn.lpstrTitle = strPath;
	if(dlgOpen.DoModal() == IDOK)
	{
		CString strFileName = dlgOpen.GetPathName();
		SetDlgItemText(IDC_EDIT_DISPLAY, strFileName);
	}
}

void CPSIDToolDlg::OnBnClickedBtnSave()
{
	TCHAR szPath[MAX_PATH] = {_T("\0")};
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath = szPath;
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));

	CString strSaveFileName;
	CFileDialog dlgOpen(FALSE, _T("exe"), _T("PersonalCard"), OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("可执行文件(*.exe)|*.exe||"), this);
	dlgOpen.m_ofn.lpstrTitle = strPath;
	if(dlgOpen.DoModal() == IDOK)
	{
		strSaveFileName = dlgOpen.GetPathName();
		SetDlgItemText(IDC_EDIT_SAVE, strSaveFileName);
	}
	else
	{
		return;
	}	

	CString strSettingFile;
	CString strInputFile;
	CString strDisplayFile;
	GetDlgItemText(IDC_EDIT_SETTING, strSettingFile);
	GetDlgItemText(IDC_EDIT_INPUT, strInputFile);
	GetDlgItemText(IDC_EDIT_DISPLAY, strDisplayFile);

	DWORD dwOffset = 0;

	char* pDataBuff = NULL;
	DWORD dwDataLen = 0;
	DWORD dwFlag = FT_UNKNOW;

	//Create a new setting app exe file
	ofstream outf(strSaveFileName, ios::binary);
	if(outf.is_open() == 0) 
		return;

	//Write setting file	
	dwFlag = FT_SETTING;
	ifstream infSetting(strSettingFile, ios::binary);
	if(infSetting.is_open() == 0)
	{
		outf.close();
		return;
	}
	infSetting.seekg(0, ios::beg);
	infSetting.seekg(0, ios::end);
	dwDataLen = infSetting.tellg();
	dwOffset = dwDataLen;
	infSetting.seekg(0, ios::beg);
	//
	pDataBuff = new char[dwDataLen];
	infSetting.read(pDataBuff, dwDataLen);
	infSetting.close();
	//
	outf.write(pDataBuff, dwDataLen);
	delete []pDataBuff;
	pDataBuff = NULL;

	//Write flag
	outf.write((const char*)&dwFlag, sizeof(DWORD));

	//Write input file
	dwFlag = FT_INPUT;
	ifstream infInput(strInputFile, ios::binary);
	if(infInput.is_open())
	{
		infInput.seekg(0, ios::beg);
		infInput.seekg(0, ios::end);
		dwDataLen = infInput.tellg();
		infInput.seekg(0, ios::beg);
		pDataBuff = new char[dwDataLen];
		infInput.read(pDataBuff, dwDataLen);
		infInput.close();

		//Write flag and size
		outf.write((const char*)&dwFlag, sizeof(DWORD));
		outf.write((const char*)&dwDataLen, sizeof(DWORD));

		//Write data
		outf.write(pDataBuff, dwDataLen);
		delete []pDataBuff;
		pDataBuff = NULL;
	}

	//Write display file
	dwFlag = FT_DISPLAY;
	ifstream infDisplay(strDisplayFile, ios::binary);
	if(infDisplay.is_open())
	{
		infDisplay.seekg(0, ios::beg);
		infDisplay.seekg(0, ios::end);
		dwDataLen = infDisplay.tellg();
		infDisplay.seekg(0, ios::beg);
		//
		pDataBuff = new char[dwDataLen];
		infDisplay.read(pDataBuff, dwDataLen);
		infDisplay.close();

		//Write flag and size
		outf.write((const char*)&dwFlag, sizeof(DWORD));
		outf.write((const char*)&dwDataLen, sizeof(DWORD));

		//Write data
		outf.write(pDataBuff, dwDataLen);
		delete []pDataBuff;
		pDataBuff = NULL;
	}

	//Write the offset for additional data
	outf.write((const char*)&dwOffset, sizeof(DWORD));
	outf.close();
}
