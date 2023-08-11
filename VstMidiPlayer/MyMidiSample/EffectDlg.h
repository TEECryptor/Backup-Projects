#pragma once


// CEffectDlg dialog

class CEffectDlg : public CDialog
{
	DECLARE_DYNAMIC(CEffectDlg)

public:
	CEffectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEffectDlg();

// Dialog Data
	enum { IDD = IDD_DLG_EFFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
