/*
 * ECViewDoc.h
 * 
 * Interface of class "CECViewDoc"
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

/*** Declaration of class "CECViewDoc" ***************************************/
class CECViewDoc: public CDocument
{
  DECLARE_DYNCREATE(CECViewDoc)

  // Implementation
  public:
#ifdef _DEBUG
  virtual void AssertValid()                 const;
  virtual void Dump       (CDumpContext& dc) const;
#endif

  // Overrides
  // ClassWizard generated virtual function overrides
  public:
  virtual BOOL OnNewDocument();
  virtual void Serialize    (CArchive& ar);

  protected: // create from serialization only
  CECViewDoc() {}

  DECLARE_MESSAGE_MAP()
};
