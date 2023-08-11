//-----------------------------------------------------------------------
//	File Name:BMCalcParameterDlg.h
//	Introduce:header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorStatic.h"
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
// CBMCalcParameterDlg dialog
//-----------------------------------------------------------------------
class CBMCalcParameterDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMCalcParameterDlg)

public:
	CBMCalcParameterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMCalcParameterDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CALCPARAM };
	//-------------------------------------------------------------------
	typedef struct tagCalcParamData
	{
		tagCalcParamData(CString strName, eTableDataType eType, LONG nCtrlDefault, LONG nCtrlValue, CString strDefault = _T(""), CString strCurValue = _T(""), BOOL bChanged = FALSE)
		{
			strParamName = strName;
			eDataType = eType;
			nDefaultCtrl = nCtrlDefault;
			nCurValueCtrl = nCtrlValue;
			strDefaultParam = strDefault;
			strCurParamValue = strCurValue;
			bIsChanged = bChanged;
		}
		//---------------------------------------------------------------
		tagCalcParamData & operator=(tagCalcParamData src)
		{
			strParamName = src.strParamName;
			eDataType = src.eDataType;
			nDefaultCtrl = src.nDefaultCtrl;
			nCurValueCtrl = src.nCurValueCtrl;
			strDefaultParam = src.strDefaultParam;
			strCurParamValue = src.strCurParamValue;
			bIsChanged = src.bIsChanged;
		}
		//---------------------------------------------------------------
		CString		strParamName;
		CString		strDefaultParam;
		CString		strCurParamValue;
		LONG		nDefaultCtrl;
		LONG		nCurValueCtrl;
		BOOL		bIsChanged;
		eTableDataType	eDataType;
	}CalcParamData;
	//-------------------------------------------------------------------
	typedef list<CalcParamData> CalcParamDataList;
	//-------------------------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDefault();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnEnChangeEditMgG2();
	afx_msg void OnEnChangeEditMgC2();
	afx_msg void OnEnChangeEditMgDh();
	afx_msg void OnCbnSelchangeComboMgT6();
	afx_msg void OnCbnSelchangeComboMgT7();
	afx_msg void OnCbnSelchangeComboMgF();
	afx_msg void OnEnChangeEditMgMd();
	afx_msg void OnEnChangeEditMgLd();
	afx_msg void OnEnChangeEditMgH();
	afx_msg void OnEnChangeEditMgV1();
	afx_msg void OnEnChangeEditMgV2();
	afx_msg void OnEnChangeEditMgN();
	afx_msg void OnEnChangeEditMgV3();
	afx_msg void OnEnChangeEditMgKpx();
	afx_msg void OnEnChangeEditMgKpz();
	afx_msg void OnEnChangeEditMgKbz();
	afx_msg void OnEnChangeEditMgR();
	afx_msg void OnCbnSelchangeComboMgFEc();
	afx_msg void OnEnChangeEditMgRmb();
	afx_msg void OnEnChangeEditMgWd();
	afx_msg void OnEnChangeEditMgEur1();
	afx_msg void OnEnChangeEditMgEur2();
	//-------------------------------------------------------------------
private:
	void	InitParamDataStruct();
	void	InitDataFromDatabase();
	void	SetParamHasChanged(LONG lCtrlID);
	//-------------------------------------------------------------------
private:
	BOOL				m_bIsInitFinished;
	CalcParamDataList	m_lstCalcParams;
	//
	CColorStatic		m_staticMGH;
	CColorStatic		m_staticMGV1;
	CColorStatic		m_staticMGV2;
	CColorStatic		m_staticMGN;
	CColorStatic		m_staticMGV3;
	CColorStatic		m_staticMGKPX;
	CColorStatic		m_staticMGKPZ;
	CColorStatic		m_staticMGKBZ;
	CColorStatic		m_staticMGR;	
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
