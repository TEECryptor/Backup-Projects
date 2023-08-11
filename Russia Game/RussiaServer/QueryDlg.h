#if !defined(AFX_QUERYDLG_H__8E96C441_DE5C_45B2_9EC5_6028F84A13A6__INCLUDED_)
#define AFX_QUERYDLG_H__8E96C441_DE5C_45B2_9EC5_6028F84A13A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QueryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// QueryDlg dialog

class QueryDlg : public CDialog
{
// Construction
public:
	QueryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(QueryDlg)
	enum { IDD = IDD_DIALOG_QUREY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(QueryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(QueryDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUERYDLG_H__8E96C441_DE5C_45B2_9EC5_6028F84A13A6__INCLUDED_)
