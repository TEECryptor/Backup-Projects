//-----------------------------------------------------------------------
//	File Name:BMCustomManagerDlg.h
//	Introduce:header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMLoginDlg dialog
//-----------------------------------------------------------------------
class CBMLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMLoginDlg)

public:
	CBMLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMLoginDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BMLOGIN };
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();

	//-------------------------------------------------------------------
private:
	void	InitControlsPos();
	//-------------------------------------------------------------------
private:
	CBitmap*	m_pKImage;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
