// TestImageButtonDlg.h : header file
//

#if !defined(AFX_TESTIMAGEBUTTONDLG_H__8EEBDBA2_468C_48B8_9F3C_022367457D27__INCLUDED_)
#define AFX_TESTIMAGEBUTTONDLG_H__8EEBDBA2_468C_48B8_9F3C_022367457D27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//------------------------------------------------
#include "GDIPlusImage.h"
#include "ImageButton.h"
//------------------------------------------------
// CTestImageButtonDlg dialog
//------------------------------------------------
class CTestImageButtonDlg : public CDialog
{
// Construction
public:
	CTestImageButtonDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestImageButtonDlg)
	enum { IDD = IDD_TESTIMAGEBUTTON_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestImageButtonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestImageButtonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//--------------------------------------------
private:
	CGDIPlusImage*	m_pGDIPlusImage;
	//
	CImageButton	m_Button;
	CImageButton	m_Button2;
	//--------------------------------------------
};
//------------------------------------------------
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTIMAGEBUTTONDLG_H__8EEBDBA2_468C_48B8_9F3C_022367457D27__INCLUDED_)
