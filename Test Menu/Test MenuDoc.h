// Test MenuDoc.h : interface of the CTestMenuDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTMENUDOC_H__CDE01F4F_F2F9_447F_AEF7_F5DE66912957__INCLUDED_)
#define AFX_TESTMENUDOC_H__CDE01F4F_F2F9_447F_AEF7_F5DE66912957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestMenuDoc : public CDocument
{
protected: // create from serialization only
	CTestMenuDoc();
	DECLARE_DYNCREATE(CTestMenuDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestMenuDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestMenuDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestMenuDoc)
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTMENUDOC_H__CDE01F4F_F2F9_447F_AEF7_F5DE66912957__INCLUDED_)
