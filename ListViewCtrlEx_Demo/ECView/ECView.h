/*
 * ECView.h
 * 
 * Interface of class "CECView"
 * (list view, which displays all countries of the European community)
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

#include "ECViewDoc.h"
#include "ListViewCtrlEx.h"

/*** Declaration of class "CECView" ******************************************/
class CECView: public CListViewEx
{
  DECLARE_DYNCREATE(CECView)

  // Attributes
  public:
  CECViewDoc*   GetDocument();
  const CString GetToolTip (int, int, UINT nFlags, BOOL&);

  // Implementation
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump       (CDumpContext& dc) const;
#endif

  protected: 
  CECView();    // create from serialization only

  // Overrides
  // ClassWizard generated virtual function overrides
  virtual void OnInitialUpdate(); // called first time after construct
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

  // Generated message map functions
  afx_msg void OnCheckboxes                ();   
  afx_msg void OnClearBackgroundimage      ();
  afx_msg void OnColorSortColumn           ();
  afx_msg void OnColumnclick               (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnColumnseparators          ();
  afx_msg void OnDestroy                   ();
  afx_msg void OnExplorerStyle             ();
  afx_msg void OnHotUnderlining            ();
  afx_msg void OnLoadBackgroundimage       ();
  afx_msg void OnKeepLabelLeft             ();
  afx_msg void OnSortArrow                 ();
  afx_msg void OnSubitemImages             ();
  afx_msg void OnUpdateCheckboxes          (CCmdUI* pCmdUI);
  afx_msg void OnUpdateClearBackgroundimage(CCmdUI *pCmdUI);
  afx_msg void OnUpdateColorSortColumn     (CCmdUI* pCmdUI);
  afx_msg void OnUpdateColumnseparators    (CCmdUI *pCmdUI);
  afx_msg void OnUpdateExplorerStyle       (CCmdUI *pCmdUI);
  afx_msg void OnUpdateHotUnderlining      (CCmdUI* pCmdUI);
  afx_msg void OnUpdateKeepLabelLeft       (CCmdUI* pCmdUI);
  afx_msg void OnUpdateSortArrow           (CCmdUI* pCmdUI);
  afx_msg void OnUpdateSubitemImages       (CCmdUI* pCmdUI);

  DECLARE_MESSAGE_MAP()

  private:
  enum SORT_BY {COUNTRY = 1, AREA, POPULATION, CAPITAL};

  static int CALLBACK CompareCountries(LPARAM lParam1, LPARAM lParam2,
                                       LPARAM lParamSort);
  static LPCTSTR      GetToken        (LPCTSTR psz, int n);

  UINT              m_winver;
  bool              m_bVisualStyle;
  static const UINT m_nFirstCountry;
  static const UINT m_nLastCountry;
  CImageList        m_imglstFlags;
  BOOL	            m_bColorSortColumn;
  BOOL	            m_bEnableSortIcon;
  BOOL              m_bKeepLabelLeft;
  BOOL              m_bSubitemImages;
  BOOL	            m_bEnableCheckBoxes;
  BOOL	            m_bUnderlineHot;
  BOOL              m_bImageLoaded;
  BOOL              m_bColumnSeparators;
  BOOL              m_bExplorerStyle;
  SORT_BY           m_sortedBy;
};

#ifdef NDEBUG   // non-debug version in ECView.cpp
  inline CECViewDoc* CECView::GetDocument()
  {
    return static_cast<CECViewDoc*>(m_pDocument);
  }
#endif
