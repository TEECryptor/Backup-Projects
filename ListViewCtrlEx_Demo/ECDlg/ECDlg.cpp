/*
 * ECDlg.cpp
 * 
 * Implementation of class "CECDlg"
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

#include "stdafx.h"
#include "ECDlgApp.h"
#include "ECDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*** Definition of the class "CECListCtrlEx" *********************************/

/*** Public member functions *************************************************/

/*** Retrieve tooltip for the small icon *************************************/
const CString CECListCtrlEx::GetToolTip(int, int, UINT nFlags, BOOL&)
{
  CString str;

  if (nFlags == LVHT_ONITEMICON) VERIFY(str.LoadString(IDS_FLAG));

  return str;
}

/*** Definition of the class "CAboutDlg" *************************************/
class CAboutDlg: public CDialog
{
  public:
  // Dialog Data
  enum {IDD = IDD_ABOUTBOX};

  CAboutDlg(): CDialog(IDD) {}

  protected:
  // ClassWizard generated virtual function overrides
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
};

/*** Protected member functions **********************************************/

/*** Data exchange: member variables <--> controls ***************************/
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

/*** Message handler table ***************************************************/
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


/*** Definition of the class "CECDlg" ****************************************/

const UINT CECDlg::m_nFirstCountry = IDR_AUSTRIA;
const UINT CECDlg::m_nLastCountry  = IDR_SWEDEN;

/*** Constructor *************************************************************/
CECDlg::CECDlg(CWnd* pParent): CDialog(IDD, pParent)
{
  m_bColorSortColumn        = TRUE;
  m_bEnableSortIcon         = TRUE;
  m_bEnableCheckBoxes       = FALSE;
  m_bSubitemImages          = FALSE;
  m_bUnderlineHot           = FALSE;
  m_bKeepLabelLeft          = TRUE;
  m_bEnableColumnSeparators = FALSE;
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon    = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_sortedBy = COUNTRY;
}

/*** Protected member functions **********************************************/

/*** Data exchange: member variables <--> controls ***************************/
void CECDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control            (pDX, IDC_COUNTRIES, m_lcCountries);
  DDX_Control            (pDX, IDC_BTN_LOADBKIMG, m_btnLoadBkImg);
  DDX_Control            (pDX, IDC_BTN_CLEARBKIMG, m_btnClearBkImg);
  DDX_Check              (pDX, IDC_CHECK_SORTCOLOR, m_bColorSortColumn);
  DDX_Check              (pDX, IDC_CHECK_SORTARROW, m_bEnableSortIcon);
  DDX_Check              (pDX, IDC_CHECK_CHECKBOXES, m_bEnableCheckBoxes);
  DDX_Check              (pDX, IDC_CHECK_SUBIMAGES, m_bSubitemImages);
  DDX_Check              (pDX, IDC_CHECK_UNDERLINE_HOT, m_bUnderlineHot);
  DDX_Check              (pDX, IDC_CHECK_LABEL_IMAGE, m_bKeepLabelLeft);
  DDX_Check              (pDX, IDC_CHECK_COLUMNSEPARATORS,
                          m_bEnableColumnSeparators);
}

/*** Clear background image **************************************************/
void CECDlg::OnBnClickedBtnClearbkimg()
{
  m_lcCountries.SetBkImage    (_T(""));
  m_btnClearBkImg.EnableWindow(FALSE);
}

/*** Load background image ***************************************************/
void CECDlg::OnBnClickedBtnLoadbkimg()
{
  CString str;
  VERIFY(str.LoadString(IDS_FILTER));

  CFileDialog dlg(
    TRUE, 0, 0, OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY |
    OFN_PATHMUSTEXIST, str);

  m_btnLoadBkImg.GetWindowText(str);  // Get caption of load button

  // Remove escape character (&) for navigation key
  for (int i = 0; (i = str.Find(_T('&'), i)) != -1; ++i)
  {
    str.Delete(i);
    if (str[i] != _T('&')) break;
  }

  // Initialize title of "File Open" dialog with text of this button
  dlg.m_pOFN->lpstrTitle = str;

  if (dlg.DoModal() == IDOK)
    // Load new background image
    if (m_lcCountries.SetBkImage(
          const_cast<LPTSTR>((LPCTSTR)dlg.GetPathName())))
      m_btnClearBkImg.EnableWindow();
}

/*** Turn on/off checkboxes **************************************************/
void CECDlg::OnCheckCheckboxes() 
{
  UpdateData();
  if (m_bEnableCheckBoxes)
    m_lcCountries.SetExtendedStyle(
      m_lcCountries.GetExtendedStyle() | LVS_EX_CHECKBOXES);
  else
    m_lcCountries.SetExtendedStyle(
      m_lcCountries.GetExtendedStyle() & ~LVS_EX_CHECKBOXES);
}

/*** Turn on/off column separators *******************************************/
void CECDlg::OnCheckColumnSeparators()
{
  UpdateData                          ();
  m_lcCountries.EnableColumnSeparators(m_bEnableColumnSeparators);
  m_lcCountries.Invalidate            ();
}

/*** Turn on/off explorer style **********************************************/
void CECDlg::OnCheckExplorerStyle()
{
  // Explorer style displays implicit column separators ...
  //   ... therefore disable explicit column separators
  if (m_bEnableColumnSeparators)
  {
    m_lcCountries.EnableColumnSeparators(FALSE);
    m_bEnableColumnSeparators = FALSE;
    UpdateData(FALSE);
  }
  m_lcCountries.EnableExplorerStyle                   ();
  GetDlgItem(IDC_CHECK_COLUMNSEPARATORS)->EnableWindow(FALSE);
  GetDlgItem(IDC_CHECK_EXPLORER_STYLE)->EnableWindow  (FALSE);
}

/*** Hold label image on the left side (or not) ******************************/
void CECDlg::OnCheckLabelImage() 
{
  UpdateData();
  if (m_bKeepLabelLeft)
  {
    GetDlgItem(IDC_CHECK_SUBIMAGES)->EnableWindow(FALSE);
    m_lcCountries.KeepLabelLeft                  ();
    m_lcCountries.Invalidate                     ();
  }
  else
  {
    m_lcCountries.KeepLabelLeft                  (false);
    m_lcCountries.Invalidate                     ();
    GetDlgItem(IDC_CHECK_SUBIMAGES)->EnableWindow();
  }
}

/*** Turn on/off sort arrow **************************************************/
void CECDlg::OnCheckSortarrow() 
{
  UpdateData                  ();
  m_lcCountries.EnableSortIcon(m_bEnableSortIcon);
}

/*** Turn on/off coloring of sort column *************************************/
void CECDlg::OnCheckSortcolor() 
{
  UpdateData                   ();
  m_lcCountries.ColorSortColumn(m_bColorSortColumn);
}

/*** Turn on/off subitem images **********************************************/
void CECDlg::OnCheckSubimages() 
{
  UpdateData();
  if (m_bSubitemImages)
  {
    GetDlgItem(IDC_CHECK_LABEL_IMAGE)->EnableWindow(FALSE);
    m_lcCountries.SetExtendedStyle(
      m_lcCountries.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES);
  }
  else
  {
    m_lcCountries.SetExtendedStyle(
      m_lcCountries.GetExtendedStyle() & ~LVS_EX_SUBITEMIMAGES);
    GetDlgItem(IDC_CHECK_LABEL_IMAGE)->EnableWindow();
  }
}

/*** Turn on/off hot underlining *********************************************/
void CECDlg::OnCheckUnderlineHot() 
{
  UpdateData();
  if (m_bUnderlineHot)
    m_lcCountries.SetExtendedStyle(
      m_lcCountries.GetExtendedStyle() |
      LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
  else
    m_lcCountries.SetExtendedStyle(
      m_lcCountries.GetExtendedStyle() &
      ~(LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT));
}

/*** A column header has been clicked ****************************************/
void CECDlg::OnColumnclickCountries(NMHDR* pNMHDR, LRESULT* pResult) 
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

  m_lcCountries.SortItems(CompareCountries, m_sortedBy);
  
  *pResult = 0;
}

/*** The window will be destroyed ********************************************/
void CECDlg::OnDestroy() 
{
  CDialog::OnDestroy     ();
  m_lcCountries.SaveState(_T("Settings"), _T("Overview"));
  
  for (int nItem = m_lcCountries.GetItemCount(); --nItem >= 0;)
    delete reinterpret_cast<CString*>(m_lcCountries.GetItemData(nItem));
}

/*** Will be called before the first display of the dialog *******************/
BOOL CECDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu)
  {
    CString strAboutMenu;
    VERIFY(strAboutMenu.LoadString(IDS_ABOUTBOX));
    if (!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);			// Set big icon
  SetIcon(m_hIcon, FALSE);		// Set small icon

  // Build equivalent to deprecated global "_win_ver"
  WORD wVersion = LOWORD(GetVersion());
  UINT winver   = MAKEWORD(HIBYTE(wVersion), LOBYTE(wVersion));

  bool      bVisualStyle = false;
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

      bVisualStyle =
        SUCCEEDED((*pDllGetVersion)(&dvi)) && dvi.dwMajorVersion >= 6;
    }
    
    FreeLibrary(hinstDll);
  }

  m_lcCountries.SetExtendedStyle(
    LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP |
    LVS_EX_INFOTIP       | LVS_EX_LABELTIP);

  // create columns of listview control
  CString str;

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

  for (int nColumn = 0; nColumn < nColCount; ++nColumn)
  {
    VERIFY(str.LoadString(colData[nColumn].nColHdrId));
    m_lcCountries.InsertColumn(
      nColumn, str, colData[nColumn].nFormat, colData[nColumn].nWidth);
    if (nColumn > 0) m_lcCountries.EnableColumnHiding(nColumn, true);
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
  for (UINT nResId = m_nFirstCountry+1; nResId <= m_nLastCountry; ++nResId)
  {
    VERIFY((hIcon = theApp.LoadIcon(nResId)) != 0);
    m_imglstFlags.Add(hIcon);
  }
  m_lcCountries.SetImageList(&m_imglstFlags, LVSIL_SMALL);

  // fill rows of listview control
  for (UINT nResId = m_nFirstCountry; nResId <= m_nLastCountry; ++nResId)
  {
    CString* pstr = new CString;

    VERIFY(pstr->LoadString(nResId));
    pstr->Replace(_T(','), _T('\0'));// tokenize string containing country info
    LPCTSTR psz = *pstr;

    int nItem         = nResId - m_nFirstCountry;
    int nItemInserted =
      m_lcCountries.InsertItem(
        LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT, nItem, psz,
        INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK, nItem+1,
        reinterpret_cast<LPARAM>(pstr));

    for (int nColumn = 1; nColumn < nColCount; ++nColumn)
    {
      psz += _tcslen(psz)+1;    // move psz to next token
      m_lcCountries.SetItem(
        nItemInserted, nColumn, LVIF_IMAGE | LVIF_TEXT, psz, nItem+1, 0, 0, 0);
    }
  }

  if (winver >= 0x0600 && bVisualStyle)
    GetDlgItem(IDC_CHECK_EXPLORER_STYLE)->ShowWindow(SW_SHOW);

  m_lcCountries.EnableSortIcon     (TRUE, 1);
  m_lcCountries.ColorSortColumn    (m_bColorSortColumn);
  m_lcCountries.KeepLabelLeft      ();
  m_lcCountries.EnableSubItemTips  ();
  m_lcCountries.RestoreState       (_T("Settings"), _T("Overview"));
  m_sortedBy = static_cast<SORT_BY>(m_lcCountries.GetSortColumn());
  m_lcCountries.SortItems(CompareCountries, m_sortedBy);

  UpdateData(FALSE);

  return TRUE;  // return TRUE  unless you set the focus to a control
}

/*** Draw icon in minimized mode *********************************************/
void CECDlg::OnPaint() 
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialog::OnPaint();
  }
}

/*** The system calls this to obtain the cursor to display while the user ****/
/*** drags the minimized window                                           ****/
HCURSOR CECDlg::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}

/*** System menu entry has been selected *************************************/
void CECDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  }
  else
  {
    CDialog::OnSysCommand(nID, lParam);
  }
}

/*** Private member functions ************************************************/

/*** Compare window data by sort criterion ***********************************/
int CALLBACK CECDlg::CompareCountries(LPARAM lParam1, LPARAM lParam2, 
                                      LPARAM lParamSort)
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
        double pop2    = _tstof(GetToken(psz2, POPULATION-1));
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
LPCTSTR CECDlg::GetToken(LPCTSTR psz, int n)
{
  for (int i = 0; i < n; ++i) psz += _tcslen(psz)+1;

  return psz;
}

/*** Message handler table ***************************************************/
BEGIN_MESSAGE_MAP(CECDlg, CDialog)
  ON_BN_CLICKED      (IDC_CHECK_EXPLORER_STYLE, OnCheckExplorerStyle)
  ON_BN_CLICKED      (IDC_CHECK_SORTCOLOR, OnCheckSortcolor)
  ON_BN_CLICKED      (IDC_CHECK_SORTARROW, OnCheckSortarrow)
  ON_BN_CLICKED      (IDC_CHECK_CHECKBOXES, OnCheckCheckboxes)
  ON_BN_CLICKED      (IDC_CHECK_SUBIMAGES, OnCheckSubimages)
  ON_BN_CLICKED      (IDC_CHECK_UNDERLINE_HOT, OnCheckUnderlineHot)
  ON_BN_CLICKED      (IDC_CHECK_LABEL_IMAGE, OnCheckLabelImage)
  ON_BN_CLICKED      (IDC_BTN_LOADBKIMG, OnBnClickedBtnLoadbkimg)
  ON_BN_CLICKED      (IDC_BTN_CLEARBKIMG, OnBnClickedBtnClearbkimg)
  ON_BN_CLICKED      (IDC_CHECK_COLUMNSEPARATORS, OnCheckColumnSeparators)
  ON_NOTIFY          (LVN_COLUMNCLICK, IDC_COUNTRIES, OnColumnclickCountries)
  ON_WM_DESTROY      ()
  ON_WM_PAINT        ()
  ON_WM_QUERYDRAGICON()
  ON_WM_SYSCOMMAND   ()
END_MESSAGE_MAP()
