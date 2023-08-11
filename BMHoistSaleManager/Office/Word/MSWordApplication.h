//-----------------------------------------------
//	File Name:MSWordApplication.h
//	Introduce:Class CMSWordApplication header file
//	Date:
//------------------------------------------------
#pragma once
//------------------------------------------------
#import "C:\\Program Files\\Microsoft Office\\OFFICE12\\MSWORD.OLB"  no_namespace raw_interfaces_only \
																	 rename("FindText","_FindText") \
																	 rename("Rectangle","_Rectangle") \
																	 rename("ExitWindows","_ExitWindows") \
																	 rename("Fields","_Fields") \
																	 rename("Field","_Field") 
//------------------------------------------------
#include "CApplication.h"
#include "CDocument0.h"
#include "CDocuments.h"
#include "CRange.h"
#include "CFind.h"
#include "CReplacement.h"
//
#include "CTable0.h"
#include "CTables0.h"
#include "CCell.h"
#include "CCells.h"
//
#include "CSelection.h"
#include "CShapes.h"
#include "CShape.h"
//------------------------------------------------
#include <list>
using namespace std;
//------------------------------------------------
typedef struct tagStringPartner
{
	tagStringPartner()
	{
		strSrcString = _T("\0");
		strDesString = _T("\0");
	}
	//--------------------------------------------
	tagStringPartner(CString src, CString des)
	{
		strSrcString = src;
		strDesString = des;
	}
	//--------------------------------------------
	CString strSrcString;
	CString strDesString;
}StringPartner;
//------------------------------------------------
typedef list<StringPartner>	StringPartnerList;
//------------------------------------------------


//------------------------------------------------
class CMSWordApplication
{

public:
	CMSWordApplication(void);
	virtual ~CMSWordApplication(void);
	//--------------------------------------------
public:
	HRESULT	OpenWordFile(CString strFile);
	HRESULT SaveWordFile(CString strNewFile);
	HRESULT	ReplaceTexts(StringPartnerList lstText);
	HRESULT	ReplaceTextsInTextBoxes(StringPartnerList lstText);
	HRESULT	SetTableCell(long lTableIndex, long lRow, long lColumn, CString strText);
	HRESULT AddStringAfter(CString strPosName, CString strText, BOOL bNewParagraph = TRUE);
	HRESULT	AddStringToEnd(CString strText, BOOL bNewParagraph = TRUE);
	//--------------------------------------------
private:	
	CApplication	m_WordApp; 
	CDocuments		m_WordDocs; 
	CDocument0		m_WordDoc; 
	//--------------------------------------------
};
//------------------------------------------------
