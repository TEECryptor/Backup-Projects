/*
 * ECDlg.h
 * 
 * Interface of class "CECDlg"
 * (dialog box, which displays all countries of the European community)
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

#include "ListViewCtrlEx.h"

/*** Definition of class "CECListCtrlEx" *************************************/
class CECListCtrlEx: public CListCtrlEx
{
  public:
  const CString GetToolTip(int, int, UINT nFlags, BOOL&);
};

/*** Declaration of class "CECDlg" *******************************************/
class CECDlg: public CDialog
{
  // Construction
  public:
  CECDlg(CWnd* pParent = 0);	          // standard constructor

  // Dialog Data
  enum { IDD = IDD_EC_DIALOG };
  CECListCtrlEx m_lcCountries;
  BOOL	        m_bColorSortColumn;
  BOOL	        m_bEnableSortIcon;
  BOOL	        m_bEnableCheckBoxes;
  BOOL	        m_bSubitemImages;
  BOOL	        m_bUnderlineHot;
  BOOL	        m_bKeepLabelLeft;
  BOOL          m_bEnableColumnSeparators;
  CButton       m_btnClearBkImg;
  CButton       m_btnLoadBkImg;

  protected:
  // ClassWizard generated virtual function overrides
  virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

  // Generated message map functions
  afx_msg void    OnBnClickedBtnClearbkimg();
  afx_msg void    OnBnClickedBtnLoadbkimg ();
  afx_msg void    OnCheckCheckboxes       ();
  afx_msg void    OnCheckColumnSeparators ();
  afx_msg void    OnCheckExplorerStyle    ();
  afx_msg void    OnCheckLabelImage       ();
  afx_msg void    OnCheckSortarrow        ();
  afx_msg void    OnCheckSortcolor        ();
  afx_msg void    OnCheckSubimages        ();
  afx_msg void    OnCheckUnderlineHot     ();
  afx_msg void    OnColumnclickCountries  (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void    OnDestroy               ();
  virtual BOOL    OnInitDialog            ();
  afx_msg void    OnPaint                 ();
  afx_msg HCURSOR OnQueryDragIcon         ();
  afx_msg void    OnSysCommand            (UINT nID, LPARAM lParam);

  DECLARE_MESSAGE_MAP()

  HICON m_hIcon;

  private:
  enum SORT_BY {COUNTRY = 1, AREA, POPULATION, CAPITAL};

  static int CALLBACK CompareCountries(LPARAM lParam1, LPARAM lParam2,
                                       LPARAM lParamSort);
  static LPCTSTR      GetToken        (LPCTSTR psz, int n);

  static const UINT m_nFirstCountry;
  static const UINT m_nLastCountry;
  CImageList        m_imglstFlags;
  SORT_BY           m_sortedBy;
};
