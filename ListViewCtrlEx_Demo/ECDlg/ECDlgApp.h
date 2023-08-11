/*
 * ECDlgApp.h
 * 
 * Interface of class "CECDlgApp"
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

/*** Declaration of class "CECDlgApp" ****************************************/
class CECDlgApp: public CWinApp
{
  public:
  // Overrides
  // ClassWizard generated virtual function overrides
  virtual BOOL InitInstance();

  DECLARE_MESSAGE_MAP()
};

extern CECDlgApp theApp;
