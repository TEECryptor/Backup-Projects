// TestImageLibraryView.h : interface of the CTestImageLibraryView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTIMAGELIBRARYVIEW_H__92042A92_2DD7_4EEE_9D82_6E7A1E7C7FB2__INCLUDED_)
#define AFX_TESTIMAGELIBRARYVIEW_H__92042A92_2DD7_4EEE_9D82_6E7A1E7C7FB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LibImage.h"

class CTestImageLibraryView : public CView
{
protected: // create from serialization only
	CTestImageLibraryView();
	DECLARE_DYNCREATE(CTestImageLibraryView)

// Attributes
public:
	CTestImageLibraryDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestImageLibraryView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestImageLibraryView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestImageLibraryView)
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CLibImage*	m_pImage;
};

#ifndef _DEBUG  // debug version in TestImageLibraryView.cpp
inline CTestImageLibraryDoc* CTestImageLibraryView::GetDocument()
   { return (CTestImageLibraryDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTIMAGELIBRARYVIEW_H__92042A92_2DD7_4EEE_9D82_6E7A1E7C7FB2__INCLUDED_)
