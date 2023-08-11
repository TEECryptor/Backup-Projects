/*
 * ECView.cpp
 * 
 * Implementation of class "CECView"
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

#include "stdafx.h"
#include "ECViewApp.h"
#include "ECView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*** Definition of class "CECView" *******************************************/

IMPLEMENT_DYNCREATE(CECView, CListViewEx)

const UINT CECView::m_nFirstCountry = IDR_AUSTRIA;
const UINT CECView::m_nLastCountry  = IDR_SWEDEN;

/*** Constructor *************************************************************/
CECView::CECView()
{
  // Build equivalent to deprecated global "_win_ver"
  if (m_winver == 0)
  {
    WORD wVersion = LOWORD(GetVersion());
    m_winver = MAKEWORD(HIBYTE(wVersion), LOBYTE(wVersion));
  }

  m_bVisualStyle = false;
  HINSTANCE hinstDll     = LoadLibrary(_T("COMCTL32.DLL"));
  if (hinstDll)
  {
    DLLGETVERSIONPROC pDllGetVersion =
      reinterpret_cast<DLLGETVERSIONPROC>(GetProcAddress(hinstDll,
        "DllGetVersion"));

    // Because some DLLs might not implement this function, you must test for
    // it explicitly. Depending on the particular DLL, the lack of a
    // DllGetVersion function can be a useful indicator of the version.
    if (pDllGetVersion)
    {
      DLLVERSIONINFO dvi = {sizeof(DLLVERSIONINFO)};

      m_bVisualStyle =
        SUCCEEDED((*pDllGetVersion)(&dvi)) && dvi.dwMajorVersion >= 6;
    }
    
    FreeLibrary(hinstDll);
  }

  m_bColorSortColumn  = TRUE;
  m_bEnableSortIcon   = TRUE;
  m_bKeepLabelLeft    = TRUE;
  m_bSubitemImages    = FALSE;
  m_bEnableCheckBoxes = FALSE;
  m_bUnderlineHot     = FALSE;
  m_bImageLoaded      = FALSE;
  m_bColumnSeparators = FALSE;
  m_bExplorerStyle    = FALSE;
  m_sortedBy          = COUNTRY;
}

/*** Public member functions *************************************************/

/*** Retrieve tooltip for the small icon *************************************/
const CString CECView::GetToolTip(int, int, UINT nFlags, BOOL&)
{
  CString str;

  if (nFlags == LVHT_ONITEMICON) VERIFY(str.LoadString(IDS_FLAG));

  return str;
}

/*** Will be called before this window will be created ***********************/
BOOL CECView::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.style |= LVS_OWNERDRAWFIXED | LVS_REPORT;

  return CListViewEx::PreCreateWindow(cs);
}

/*** Diagnostic functions ****************************************************/
#ifndef NDEBUG
void CECView::AssertValid() const
{
  CListViewEx::AssertValid();
}

void CECView::Dump(CDumpContext& dc) const
{
  CListViewEx::Dump(dc);
}

CECViewDoc* CECView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CECViewDoc)));
  return static_cast<CECViewDoc*>(m_pDocument);
}
#endif //NDEBUG

/*** Protected member functions **********************************************/

/*** Menu item "Display check boxes" has been selected ***********************/
void CECView::OnCheckboxes() 
{
  m_bEnableCheckBoxes = !m_bEnableCheckBoxes;
  CListCtrl& lcCountries = GetListCtrl();
  if (m_bEnableCheckBoxes)
    lcCountries.SetExtendedStyle(
      lcCountries.GetExtendedStyle() | LVS_EX_CHECKBOXES);
  else
    lcCountries.SetExtendedStyle(
      lcCountries.GetExtendedStyle() & ~LVS_EX_CHECKBOXES);
}

/*** Menu item "Clear Background Image" has been selected ********************/
void CECView::OnClearBackgroundimage()
{
  GetListCtrl().SetBkImage(_T(""));
  m_bImageLoaded = FALSE;
}

/*** Menu item "Color sort column" has been selected *************************/
void CECView::OnColorSortColumn() 
{
  m_bColorSortColumn = !m_bColorSortColumn;
  ColorSortColumn(m_bColorSortColumn);
}

/*** A column header has been clicked ****************************************/
void CECView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
  NM_LISTVIEW* pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);
  
  SORT_BY sortBy;

  switch (pNMListView->iSubItem)
  {
    default: ASSERT(false);
      /**/
    case 0:
      sortBy = COUNTRY;
      break;
    case 1:
      sortBy = AREA;
      break;
    case 2:
      sortBy = POPULATION;
      break;
    case 3:
      sortBy = CAPITAL;
      break;
  }

  if (m_sortedBy == sortBy)
    m_sortedBy = static_cast<SORT_BY>(-sortBy);
  else
    m_sortedBy = sortBy;

  GetListCtrl().SortItems(CompareCountries, m_sortedBy);
  
  CListViewEx::OnColumnclick(pNMHDR, pResult);
}

/*** Menu item "Column Separators" has been selected *************************/
void CECView::OnColumnseparators()
{
  m_bColumnSeparators = !m_bColumnSeparators;
  EnableColumnSeparators(m_bColumnSeparators);
  Invalidate            ();
}

/*** The window will be destroyed ********************************************/
void CECView::OnDestroy() 
{
  SaveState(_T("Settings"), _T("Overview"));
  
  CListCtrl& lcCountries = GetListCtrl();
  for (int nItem = lcCountries.GetItemCount(); --nItem >= 0;)
    delete reinterpret_cast<CString*>(lcCountries.GetItemData(nItem));

  CListViewEx::OnDestroy();
}

/*** Menu item "Explorer Style" has been selected ****************************/
void CECView::OnExplorerStyle()
{
  if (m_bColumnSeparators)
  {
    m_bColumnSeparators = FALSE;
    EnableColumnSeparators(FALSE);
  }
  m_bExplorerStyle = TRUE;
  EnableExplorerStyle();
}

/*** Menu item "Underline Hot" has been selected *****************************/
void CECView::OnHotUnderlining() 
{
  m_bUnderlineHot = !m_bUnderlineHot;
  CListCtrl& lcCountries = GetListCtrl();
  if (m_bUnderlineHot)
    lcCountries.SetExtendedStyle(
      lcCountries.GetExtendedStyle() |
      LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
  else
    lcCountries.SetExtendedStyle(
      lcCountries.GetExtendedStyle() & 
      ~(LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT));
}

/*** Will be called immediately before the view will be displayed ************/
void CECView::OnInitialUpdate()
{
  CListViewEx::OnInitialUpdate();

  CListCtrl& lcCountries = GetListCtrl();
  lcCountries.SetExtendedStyle(
    LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP |
    LVS_EX_INFOTIP       | LVS_EX_LABELTIP);

  CString    str;

  static const struct
  {
    UINT nColHdrId;
    int  nFormat;
    int  nWidth;
  } colData[] =
  {
    {IDS_COUNTRY,    LVCFMT_LEFT,   90},
    {IDS_AREA,       LVCFMT_RIGHT, 100},
    {IDS_POPULATION, LVCFMT_RIGHT, 140},
    {IDS_CAPITAL,    LVCFMT_LEFT,   80}
  };
  const int nColCount = sizeof colData / sizeof colData[0];

  for (int nColumn = 0; nColumn < nColCount; nColumn++)
  {
    VERIFY(str.LoadString(colData[nColumn].nColHdrId));
    lcCountries.InsertColumn(
      nColumn, str, colData[nColumn].nFormat, colData[nColumn].nWidth);
    if (nColumn > 0) EnableColumnHiding(nColumn, true);
  }

  // Build image list:
  // =================
  // load first icon two times because we need an one-based index
  m_imglstFlags.Create(16, 16, ILC_MASK, 25, 0);
  HICON hIcon;
  VERIFY((hIcon = theApp.LoadIcon(m_nFirstCountry)) != 0);
  m_imglstFlags.Add(hIcon);
  m_imglstFlags.Add(hIcon);

  // load rest of icons
  UINT nResId;
  for (nResId = m_nFirstCountry+1; nResId <= m_nLastCountry; nResId++)
  {
    VERIFY((hIcon = theApp.LoadIcon(nResId)) != 0);
    m_imglstFlags.Add(hIcon);
  }
  lcCountries.SetImageList(&m_imglstFlags, LVSIL_SMALL);

  // fill rows of listview control
  for (nResId = m_nFirstCountry; nResId <= m_nLastCountry; nResId++)
  {
    CString* pstr = new CString;

    VERIFY(pstr->LoadString(nResId));
    pstr->Replace(_T(','), _T('\0'));// tokenize string containing country info
    LPCTSTR psz = *pstr;

    int nItem         = nResId - m_nFirstCountry;
    int nItemInserted = lcCountries.InsertItem(
      LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT, nItem, psz,
      INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK, nItem+1,
      reinterpret_cast<LPARAM>(pstr));

    for (int nColumn = 1; nColumn < nColCount; nColumn++)
    {
      psz += _tcslen(psz)+1;    // move psz to next token
      lcCountries.SetItem(
        nItemInserted, nColumn, LVIF_IMAGE | LVIF_TEXT, psz, nItem+1, 0, 0, 0);
    }
  }

  EnableSortIcon   (m_bEnableSortIcon);
  ColorSortColumn  (m_bColorSortColumn);
  SetSortColumn    (1);
  KeepLabelLeft    ();
  EnableSubItemTips();
  RestoreState   (_T("Settings"), _T("Overview"));
  m_sortedBy = static_cast<SORT_BY>(GetSortColumn());
  lcCountries.SortItems(CompareCountries, m_sortedBy);
}

/*** Menu item "Keep Label Image Left" has been selected *********************/
void CECView::OnKeepLabelLeft() 
{
  m_bKeepLabelLeft = !m_bKeepLabelLeft;
  KeepLabelLeft(m_bKeepLabelLeft);
}

/*** Menu item "Load Background Image" has been selected *********************/
void CECView::OnLoadBackgroundimage()
{
  CString str;
  VERIFY(str.LoadString(IDS_FILTER));

  CFileDialog dlg(
    TRUE, 0, 0, OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY |
    OFN_PATHMUSTEXIST, str);

  // Get text of menu entry
  AfxGetMainWnd()->GetMenu()->GetMenuString(
    ID_LOAD_BACKGROUNDIMAGE, str, MF_BYCOMMAND);

  // Remove escape character (&) for navigation key
  for (int i = 0; (i = str.Find(_T('&'), i)) != -1; ++i)
  {
    str.Delete(i);
    if (str[i] != _T('&')) break;
  }

  // Remove trailing dots and spaces
  int n = str.GetLength();
  for (int i = 0; n > 0; ++i)
  {
    TBYTE c = str[--n];
    if (c != _T('.') && !_istspace(c))
    {
      str.Delete(n+1, i);
      break;
    }
  }

  // Initialize title of "File Open" dialog with text of this button
  dlg.m_pOFN->lpstrTitle = str;

  if (dlg.DoModal() == IDOK)
    // Load new nbackground image
    if (GetListCtrl().SetBkImage(
          const_cast<LPTSTR>((LPCTSTR)dlg.GetPathName())))
      m_bImageLoaded = TRUE;
}

/*** Menu item "Display Sort Arrow" has been selected ************************/
void CECView::OnSortArrow() 
{
  m_bEnableSortIcon = !m_bEnableSortIcon;
  EnableSortIcon(m_bEnableSortIcon);
}

/*** Menu item "Subitem Images" has been selected ****************************/
void CECView::OnSubitemImages() 
{
  m_bSubitemImages = !m_bSubitemImages;
  CListCtrl& lcCountries = GetListCtrl();
  if (m_bSubitemImages)
    lcCountries.SetExtendedStyle(
      lcCountries.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES);
  else
    lcCountries.SetExtendedStyle(
      lcCountries.GetExtendedStyle() & ~LVS_EX_SUBITEMIMAGES);
}

/*** A menu item will be updated *********************************************/
void CECView::OnUpdateCheckboxes(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_bEnableCheckBoxes);
}

void CECView::OnUpdateClearBackgroundimage(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_bImageLoaded);
}

void CECView::OnUpdateColorSortColumn(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_bColorSortColumn);
}

void CECView::OnUpdateColumnseparators(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(m_bColumnSeparators);
  pCmdUI->Enable  (!m_bExplorerStyle);
}

void CECView::OnUpdateExplorerStyle(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(m_bExplorerStyle);
  pCmdUI->Enable  (m_winver >= 0x0600 && m_bVisualStyle && !m_bExplorerStyle);
}

void CECView::OnUpdateHotUnderlining(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_bUnderlineHot);
}
void CECView::OnUpdateKeepLabelLeft(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_bKeepLabelLeft);
  pCmdUI->Enable  (!m_bSubitemImages);
}

void CECView::OnUpdateSortArrow(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_bEnableSortIcon);
}

void CECView::OnUpdateSubitemImages(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_bSubitemImages);
  pCmdUI->Enable  (!m_bKeepLabelLeft);
}

/*** Private member functions ************************************************/

/*** Compare window data by sort criterion ***********************************/
int CALLBACK CECView::CompareCountries(
  LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  LPCTSTR psz1 = *reinterpret_cast<CString*>(lParam1);
  LPCTSTR psz2 = *reinterpret_cast<CString*>(lParam2);
  bool    bComplementResult;
  int     result = 0;

  if (lParamSort < 0)
  {
    lParamSort = -lParamSort;
    bComplementResult = true;
  }
  else
    bComplementResult = false;

  // Sort window data by predefined sort criterion
  // (secondary sort criterion is always the window handle)
  for (;; lParamSort = COUNTRY, bComplementResult = false)
  {
    switch (lParamSort)
    {
      case COUNTRY:
      {
        LPCTSTR psz10 = GetToken(psz1, COUNTRY-1);
        LPCTSTR psz20 = GetToken(psz2, COUNTRY-1);

        VERIFY((result = _tcscmp(psz10, psz20)) != 0);
        break;
      }

      case AREA:
      {
        result = _ttoi(GetToken(psz1, AREA-1)) - _ttoi(GetToken(psz2, AREA-1));

        if (result == 0) continue;
        break;
      }

      case POPULATION:
      {
        double pop1    = _tstof(GetToken(psz1, POPULATION-1));
        double pop2	   = _tstof(GetToken(psz2, POPULATION-1));
        double fResult = pop1 - pop2;
        if (fResult > 0) result = static_cast<int>(fResult + 1.0);
        else if (fResult < 0) result = static_cast<int>(fResult - 1.0);
        else result = 0;

        if (result == 0) continue;
        break;
      }

      case CAPITAL:
      {
        LPCTSTR psz13 = GetToken(psz1, CAPITAL-1);
        LPCTSTR psz23 = GetToken(psz2, CAPITAL-1);

        result = _tcscmp(psz13, psz23);
        if (result == 0) continue;
        break;
      }

      default:
        ASSERT(false);
        break;
    }
    break;
  }

  if (bComplementResult) result = -result;
  return result;
}

/*** Get substring of country information ************************************/
LPCTSTR CECView::GetToken(LPCTSTR psz, int n)
{
  for (int i = 0; i < n; ++i) psz += _tcslen(psz)+1;

  return psz;
}

/*** Message handlers ********************************************************/
BEGIN_MESSAGE_MAP(CECView, CListViewEx)
  ON_COMMAND          (ID_SORT_ARROW, OnSortArrow)
  ON_COMMAND          (ID_CHECKBOXES, OnCheckboxes)
  ON_COMMAND          (ID_COLOR_SORT_COLUMN, OnColorSortColumn)
  ON_COMMAND          (ID_KEEP_LABEL_LEFT, OnKeepLabelLeft)
  ON_COMMAND          (ID_SUBITEM_IMAGES, OnSubitemImages)
  ON_COMMAND          (ID_HOT_UNDERLINING, OnHotUnderlining)
  ON_COMMAND          (ID_LOAD_BACKGROUNDIMAGE, OnLoadBackgroundimage)
  ON_COMMAND          (ID_CLEAR_BACKGROUNDIMAGE, OnClearBackgroundimage)
  ON_COMMAND          (ID_VIEW_EXPLORERSTYLE, OnExplorerStyle)
  ON_COMMAND          (ID_VIEW_COLUMNSEPARATORS, OnColumnseparators)
  ON_NOTIFY_REFLECT   (LVN_COLUMNCLICK, OnColumnclick)
  ON_UPDATE_COMMAND_UI(ID_COLOR_SORT_COLUMN, OnUpdateColorSortColumn)
  ON_UPDATE_COMMAND_UI(ID_SORT_ARROW, OnUpdateSortArrow)
  ON_UPDATE_COMMAND_UI(ID_KEEP_LABEL_LEFT, OnUpdateKeepLabelLeft)
  ON_UPDATE_COMMAND_UI(ID_SUBITEM_IMAGES, OnUpdateSubitemImages)
  ON_UPDATE_COMMAND_UI(ID_CHECKBOXES, OnUpdateCheckboxes)
  ON_UPDATE_COMMAND_UI(ID_HOT_UNDERLINING, OnUpdateHotUnderlining)
  ON_UPDATE_COMMAND_UI(ID_CLEAR_BACKGROUNDIMAGE, OnUpdateClearBackgroundimage)
  ON_UPDATE_COMMAND_UI(ID_VIEW_EXPLORERSTYLE, OnUpdateExplorerStyle)
  ON_UPDATE_COMMAND_UI(ID_VIEW_COLUMNSEPARATORS, OnUpdateColumnseparators)
  ON_WM_DESTROY       ()
END_MESSAGE_MAP()
