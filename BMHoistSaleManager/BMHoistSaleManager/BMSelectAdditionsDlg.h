//-----------------------------------------------------------------------
//	File Name:BMSelectAdditionsDlg.h
//	Introduce:header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include "ColorListCtrl.h"
#include <list>
//-----------------------------------------------------------------------
typedef std::list<CString>	StringList;
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMSelectAdditionsDlg dialog
//-----------------------------------------------------------------------
class CBMSelectAdditionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CBMSelectAdditionsDlg)

public:
	CBMSelectAdditionsDlg(eAdditionType eType, CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMSelectAdditionsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ADDITIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedOk();
	//-------------------------------------------------------------------
public:
	void	SetSelAdditions(StringList lstSelNames);
	void	GetSelAdditions(StringList &lstSelNames);
	//-------------------------------------------------------------------
private:
	void	InitTablesDataStruct();
	void	InitDataLstControls();
	void	FillAdditionLists();
	//-------------------------------------------------------------------
private:	
	CImageList			m_imgNULL;
	eAdditionType		m_eAdditionType;
	TableData			m_stuAdditionTable;
	CColorListCtrl		m_lstCtlAdditions;
	CColorListCtrl		m_lstCtlSelected;
	StringList			m_lstSelAdditions;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
