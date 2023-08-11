// Test MenuView.h : interface of the CTestMenuView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTMENUVIEW_H__5FB765F5_6913_480F_9BFF_55B62BAD9CC7__INCLUDED_)
#define AFX_TESTMENUVIEW_H__5FB765F5_6913_480F_9BFF_55B62BAD9CC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestMenuView : public CView
{
protected: // create from serialization only
	CTestMenuView();
	DECLARE_DYNCREATE(CTestMenuView)

// Attributes
public:
	CTestMenuDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestMenuView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestMenuView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestMenuView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Test MenuView.cpp
inline CTestMenuDoc* CTestMenuView::GetDocument()
   { return (CTestMenuDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTMENUVIEW_H__5FB765F5_6913_480F_9BFF_55B62BAD9CC7__INCLUDED_)
