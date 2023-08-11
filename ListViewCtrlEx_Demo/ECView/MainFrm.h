/*
 * MainFrm.h
 * 
 * Interface of class "CMainFrame"
 * (document class of application, which displays all countries of the
 *  European community)
 * 
 * Date: 15.09.2010
 * 
 * Author(s):
 * Thomas Holte
 * 
 * Copyright (c) 2010 Thomas Holte
 * All Rights Reserved
 * 
 */

/*** Declaration of class "CMainFrame" ***************************************/
class CMainFrame: public CFrameWnd
{
  
  DECLARE_DYNCREATE(CMainFrame)

  // Implementation
  public:
#ifdef _DEBUG
  virtual void AssertValid()                 const;
  virtual void Dump       (CDumpContext& dc) const;
#endif

  // Overrides
  // ClassWizard generated virtual function overrides
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

  protected: // create from serialization only
  CMainFrame() {}

  // Generated message map functions
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

  DECLARE_MESSAGE_MAP()
};
