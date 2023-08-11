#pragma once



// CHomepageView form view

class CHomepageView : public CFormView
{
	DECLARE_DYNCREATE(CHomepageView)

protected:
	CHomepageView();           // protected constructor used by dynamic creation
	virtual ~CHomepageView();

public:
	enum { IDD = IDD_DIALOG_HOMEPAGE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	CBitmap*		m_pKImage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


