/*
 * ECViewApp.h
 * 
 * Interface of class "CECViewApp"
 * (application, which displays all countries of the European community)
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

/*** Declaration of class "CECViewApp" ***************************************/
class CECViewApp: public CWinApp
{
  // Overrides
  // ClassWizard generated virtual function overrides
  public:
  virtual BOOL InitInstance();

  // Implementation
  afx_msg void OnAppAbout();

  DECLARE_MESSAGE_MAP()
};

extern CECViewApp theApp;    // The one and only CECViewApp object
