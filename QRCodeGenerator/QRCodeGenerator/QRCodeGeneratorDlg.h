// QRCodeGeneratorDlg.h : header file
//

#pragma once

#include <atlimage.h>

// CQRCodeGeneratorDlg dialog
class CQRCodeGeneratorDlg : public CDialog
{
// Construction
public:
	CQRCodeGeneratorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_QRCODEGENERATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CImage*			m_pQRImage;
	CImage*			m_pLogoImage;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpenlogo();
	afx_msg void OnBnClickedBtnGencode();
	afx_msg void OnBnClickedBtnSavetofile();
};
