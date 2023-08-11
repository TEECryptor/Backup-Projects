// SynoTFKeySDKTestDlg.h : header file
//

#pragma once

#include "./TFKeySDKTester/Defs.h"
#include "./TFKeySDKTester/ISDKTester.h"

// CSynoTFKeySDKTestDlg dialog
class CSynoTFKeySDKTestDlg : public CDialog
{
// Construction
public:
	CSynoTFKeySDKTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SYNOTFKEYSDKTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	ISDKTester*	m_pSDKTester;
	ULONG		m_ulCurLoginType;

public:
	SDK_TYPE	m_eSelSDKType;
	CString		m_strDllFile;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString		m_strSelDevName;
	ULONG		m_hSession;
	ULONG		m_hSessionKey;
	BOOL		m_bExitThread;
	BOOL		m_bListenEvent;
	HANDLE		m_hEventThread;
	ULONG		m_ulEventThreadID;
private:
	void	_SetControlInitStatus();
	void	_EnumDevices();
	BOOL	_ReadFileData(FILE* file, BYTE* lpData, ULONG *pulDataLen);
public:
	ULONG	SDKFuncMsgCallback(LPCTSTR lpcsResultMsg);
	ULONG	ListenKeyEvents();
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedMaindlgBtnRefreshlist();
	afx_msg void OnTvnSelchangedMaindlgTreeList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMaindlgBtnDevinfo();
	afx_msg void OnBnClickedMaindlgBtnDevinit();
	afx_msg void OnBnClickedMaindlgBtnSetlabel();
	afx_msg void OnBnClickedMaindlgBtnDevauth();
	afx_msg void OnBnClickedMaindlgBtnChangeauthkey();
	afx_msg void OnBnClickedMaindlgCheckListenevent();
	afx_msg void OnBnClickedMaindlgBtnOpensession();
	afx_msg void OnBnClickedMaindlgBtnClosesession();
	afx_msg void OnBnClickedMaindlgBtnSessioninfo();
	afx_msg void OnBnClickedMaindlgBtnLoginsession();
	afx_msg void OnBnClickedMaindlgBtnLogoutsession();
	afx_msg void OnBnClickedMaindlgBtnChangepin();
	afx_msg void OnBnClickedMaindlgBtnUnlockpin();
	afx_msg void OnBnClickedMaindlgBtnGenkeypair();
	afx_msg void OnBnClickedMaindlgBtnImportkeypair();
	afx_msg void OnBnClickedMaindlgExportsignpubkey();
	afx_msg void OnBnClickedMaindlgExportexchpubkey();
	afx_msg void OnBnClickedMaindlgBtnGeneratekey();
	afx_msg void OnBnClickedMaindlgBtnReleasekey();
	afx_msg void OnBnClickedMaindlgBtnClearinfo();
	afx_msg void OnBnClickedMaindlgBtnSignverify();
	afx_msg void OnBnClickedMaindlgBtnCrypto();
	afx_msg void OnBnClickedMaindlgBtnEnvenlop();
	afx_msg void OnBnClickedMaindlgBtnImportcert();
	afx_msg void OnBnClickedMaindlgBtnExportcert();
	afx_msg void OnBnClickedMaindlgBtnEnumfiles();
	afx_msg void OnBnClickedMaindlgBtnCreatefile();
	afx_msg void OnBnClickedMaindlgBtnReadfile();
	afx_msg void OnBnClickedMaindlgBtnFileinfo();
};
