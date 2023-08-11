
// ZJCAUSBKeyTestToolDlg.h : header file
//

#pragma once

#include "../Include/USBKeyApi.h"
#include "ZJCAUSBKeyEventThread.h"
#include "ZJCAUSBKeyTestThread.h"

#define		TEST_DATA_LEN	(1024*512)
#define		Random(x)		(rand()%x)

// CZJCAUSBKeyTestToolDlg dialog
class CZJCAUSBKeyTestToolDlg : public CDialogEx
{
// Construction
public:
	CZJCAUSBKeyTestToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ZJCAUSBKEYTESTTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckTestcsp();
	afx_msg void OnCbnSelchangeComboCspname();
	afx_msg void OnCbnSelchangeComboCspAsymmalg();
	afx_msg void OnCbnSelchangeComboCspSymmalg();
	afx_msg void OnCbnSelchangeComboCspHashalg();
	afx_msg void OnBnClickedCheckCspcryptP7();
	afx_msg void OnBnClickedCheckCspcryptP1();
	afx_msg void OnBnClickedCheckCspsignP7();
	afx_msg void OnBnClickedCheckCspsignP1();	
	afx_msg void OnBnClickedCheckTestp11();	
	afx_msg void OnBnClickedBtbOpenp11();
	afx_msg void OnCbnSelchangeComboP11Asymmalg();
	afx_msg void OnCbnSelchangeComboP11Symmalg();
	afx_msg void OnCbnSelchangeComboP11Hashalg();
	afx_msg void OnBnClickedCheckTestp11Event();
	afx_msg void OnBnClickedCheckP11cryptP1();
	afx_msg void OnBnClickedCheckP11signP1();	
	afx_msg void OnBnClickedCheckTestskf();
	afx_msg void OnBnClickedBtnOpenskf();
	afx_msg void OnCbnSelchangeComboSkfAsymmalg();
	afx_msg void OnCbnSelchangeComboSkfSymmalg();
	afx_msg void OnCbnSelchangeComboSkfHashalg();
	afx_msg void OnBnClickedCheckTestskfEvent();
	afx_msg void OnBnClickedCheckSkfcryptP1();
	afx_msg void OnBnClickedCheckSkfsignP1();
	afx_msg void OnBnClickedCheckLogfile();
	afx_msg void OnBnClickedBtnLogfile();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnClean();
	afx_msg void OnBnClickedBtnSavelog();

private:
	static ULONG CALLBACK USBKeyEventFunc(ULONG ulSoltID, LPTSTR lpDevName, ULONG ulEventType, LPVOID lpCaller);

private:
	void	InitCSPNameComboBox();
	void	InitAlgNameComboBox();
	void	PrintLogMessage(LPCSTR lpMsg, BOOL bIsError);
	void	CreateCSPApi();
	void	CreateP11Api();
	void	CreateSkfApi();
	void	DestoryUsbKeyApi();
	void	TestCSP();
	void	TestP11();
	void	TestSkf();
private:
	IUSBKeyApi*		m_pCSP;
	IUSBKeyApi*		m_pP11;
	IUSBKeyApi*		m_pSKF;
	BOOL			m_bTestCSP;
	BOOL			m_bTestP11;
	BOOL			m_bTestSKF;
	BOOL			m_bTestCspCryptP7;
	BOOL			m_bTestCspCryptP1;
	BOOL			m_bTestCspSignP7;
	BOOL			m_bTestCspSignP1;
	BOOL			m_bTestP11Event;
	BOOL			m_bTestP11CryptP1;
	BOOL			m_bTestP11SignP1;
	BOOL			m_bTestSkfEvent;
	BOOL			m_bTestSkfCryptP1;
	BOOL			m_bTestSkfSignP1;
	CString			m_strCSPName;
	CString			m_strP11File;
	CString			m_strSkfFile;
	ULONG			m_ulCspAsymmAlg;
	ULONG			m_ulCspSymmAlg;
	ULONG			m_ulCspHashAlg;
	ULONG			m_ulP11AsymmAlg;
	ULONG			m_ulP11SymmAlg;
	ULONG			m_ulP11HashAlg;
	ULONG			m_ulSkfAsymmAlg;
	ULONG			m_ulSkfSymmAlg;
	ULONG			m_ulSkfHashAlg;
	ULONG			m_ulTestDataLen;
	BYTE			m_btTestData[TEST_DATA_LEN];
	CHAR			m_csUserPIN[64];
	CZJCAUSBKeyEventThread*		m_pP11EventThread;
	CZJCAUSBKeyEventThread*		m_pSkfEventThread;
	CZJCAUSBKeyTestThread*		m_pUsbKeyTestThread;
	CRITICAL_SECTION			s_csDevEvent;
};
