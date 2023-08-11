//-----------------------------------------------------------------------
//	File Name:BMMessageDlg.h
//	Introduce:Class CBMMessageDlg define header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMMessageDlg dialog
//-----------------------------------------------------------------------
class CBMMessageDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMMessageDlg)
public:
	CBMMessageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMMessageDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MSGDLG };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedNo();
	//-------------------------------------------------------------------
public:
	void	SetMsgTextAndIcon(CString strCaption, CString strText, UINT nFlags);
	//-------------------------------------------------------------------
private:
	CString	m_strCaption;
	CString	m_strText;
	HICON	m_hIcon;
	UINT	m_nBtnFlags;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------