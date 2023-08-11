// TestImageButtonDlg.h : header file
//

#if !defined(AFX_TESTIMAGEBUTTONDLG_H__E8AC6D1F_56EE_442A_A5BF_C540BB3EC776__INCLUDED_)
#define AFX_TESTIMAGEBUTTONDLG_H__E8AC6D1F_56EE_442A_A5BF_C540BB3EC776__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "ImageButton.h"

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CTestImageButtonDlg dialog
#define  MAX_LEN   1000

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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//-----------------------------------------------
private:
	//-----------------------------------------------
	//To load the button ini file
	void	LoadButtonIniFile(char* file);
	//To load the button text file
	void	LoadButtonTxtFile(char* file);
	//-----------------------------------------------
	//Get the string value from the ini file
	CString		MyGetPrivateProfileString(const CString strSection, const CString strEntry, const CString strDefault, const CString strIniFile);
	//Get the int value from the ini file
	int			MyGetPrivateProfileInt(const CString strSection, const CString strEntry, const int iDefault, const CString strIniFile);
	//-----------------------------------------------
private:
	//-----------------------------------------------
	vector<CImageButton*>		m_lstBtn;
	//-----------------------------------------------
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTIMAGEBUTTONDLG_H__E8AC6D1F_56EE_442A_A5BF_C540BB3EC776__INCLUDED_)
