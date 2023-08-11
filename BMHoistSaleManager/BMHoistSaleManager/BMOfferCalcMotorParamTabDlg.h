#pragma once

#include <atlimage.h>

// CBMOfferCalcMotorParamTabDlg dialog

class CBMOfferCalcMotorParamTabDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMOfferCalcMotorParamTabDlg)

public:
	CBMOfferCalcMotorParamTabDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMOfferCalcMotorParamTabDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MOTOR_PARAMTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//-------------------------------------------------------------------
public:
	void		SetCurParamTableIndex(int iIndex);
	//-------------------------------------------------------------------
private:
	int			m_iCurParamTableIndex;
	CImage*		m_pParamTableImages[4];
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
