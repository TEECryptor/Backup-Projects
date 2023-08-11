// TestImageLibraryDoc.h : interface of the CTestImageLibraryDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTIMAGELIBRARYDOC_H__7D8E5206_ED90_46CE_936C_6185B5CA56C4__INCLUDED_)
#define AFX_TESTIMAGELIBRARYDOC_H__7D8E5206_ED90_46CE_936C_6185B5CA56C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestImageLibraryDoc : public CDocument
{
protected: // create from serialization only
	CTestImageLibraryDoc();
	DECLARE_DYNCREATE(CTestImageLibraryDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestImageLibraryDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestImageLibraryDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestImageLibraryDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTIMAGELIBRARYDOC_H__7D8E5206_ED90_46CE_936C_6185B5CA56C4__INCLUDED_)
