//----------------------------------------------------
//	File Name:AFUProductDlg.h
//	Introduce:Dialog class CAFUProductDlg interfaces definition
//----------------------------------------------------
#pragma once
//----------------------------------------------------


//----------------------------------------------------
//	Name:CAFUProductDlg
//	Introduce:Class CAFUProductDlg
//----------------------------------------------------
class CAFUProductDlg : public CDialog
{
	DECLARE_DYNAMIC(CAFUProductDlg)
	//------------------------------------------------
public:
	CAFUProductDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAFUProductDlg();
	//------------------------------------------------
// Dialog Data
	enum { IDD = IDD_DIALOG_GOODS };
	//------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	//------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	//------------------------------------------------
public:
	void	SetInitializeData(CString strProductID, CString strProductName, float fPrice, long lCount, CString strOther);
	void	GetUpdatedData(CString &strProductID, CString &strProductName, float &fPrice, long &lCount, CString &strOther);
	//------------------------------------------------
private:
	CString	m_strProductID;
	CString	m_strProductName;
	float	m_fPrice;
	long	m_lCount;
	CString	m_strOther;
	//------------------------------------------------
};
//----------------------------------------------------
