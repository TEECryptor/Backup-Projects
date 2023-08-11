//-----------------------------------------------
//	File Name:MSWordApplication.cpp
//	Introduce:Class CMSWordApplication implement file
//	Date:
//------------------------------------------------
#include "StdAfx.h"
#include "MSWordApplication.h"
//------------------------------------------------



//------------------------------------------------
CMSWordApplication::CMSWordApplication(void)
{
}
//------------------------------------------------
CMSWordApplication::~CMSWordApplication(void)
{
	m_WordDoc.ReleaseDispatch(); 
	m_WordDocs.ReleaseDispatch(); 
	m_WordApp.ReleaseDispatch(); 
}
//------------------------------------------------



//------------------------------------------------
HRESULT	CMSWordApplication::OpenWordFile(CString strFile)
{
	HRESULT hr = S_OK;

	CoInitialize(NULL);

	COleVariant vFalse((short)FALSE); 
	COleVariant vOpt(DISP_E_PARAMNOTFOUND, VT_ERROR); 
	if (!m_WordApp.CreateDispatch(_T("Word.Application"), NULL)) 
	{ 
		TRACE("\nmyWord.CreateDispatch failed!!"); 
		return S_FALSE; 
	} 

	m_WordDocs = m_WordApp.get_Documents();
	m_WordDoc = m_WordDocs.Open(COleVariant(strFile),   vOpt,   vOpt,   
								vOpt,   vOpt,   vOpt,   vOpt,   vOpt,  
								vOpt,   vOpt,   vOpt,   vOpt,   vOpt,  
								vOpt,   vOpt, vOpt);
	return hr;
}
//------------------------------------------------
HRESULT	CMSWordApplication::SaveWordFile(CString strNewFile)
{
	HRESULT hr = S_OK;

	COleVariant vFalse((short)FALSE);
	COleVariant vOpt(DISP_E_PARAMNOTFOUND, VT_ERROR); 
	m_WordDoc.SaveAs(COleVariant(strNewFile),   vOpt,   vOpt,   
					vOpt,   vOpt,   vOpt,   vOpt,   vOpt,  
					vOpt,   vOpt,   vOpt,   vOpt,   vOpt,  
					vOpt,   vOpt, vOpt);

	m_WordApp.Quit(vFalse, vOpt, vOpt); 

	return hr;
}
//------------------------------------------------
HRESULT	CMSWordApplication::ReplaceTexts(StringPartnerList lstText)
{
	HRESULT hr = S_OK;

	COleVariant vFalse((short)FALSE);
	COleVariant vOpt(DISP_E_PARAMNOTFOUND, VT_ERROR);

	CRange range = m_WordDoc.get_Content();
	CFind fndInDoc = range.get_Find();

	CReplacement rpInDoc;
	fndInDoc.ClearFormatting(); 
	rpInDoc = fndInDoc.get_Replacement(); 
	rpInDoc.ClearFormatting(); 
	for (StringPartnerList::iterator it = lstText.begin();
		 it != lstText.end();
		 it++)
	{
		StringPartner strings = *it;

		fndInDoc.Execute(&COleVariant(strings.strSrcString), vOpt, vOpt, vOpt, vOpt, 
						 vOpt, vOpt, vOpt, vOpt, &COleVariant(strings.strDesString), 
						 &COleVariant((short)wdReplaceAll), vOpt, vOpt, vOpt, vOpt); 
	}
	return hr;
}
//------------------------------------------------
HRESULT	CMSWordApplication::ReplaceTextsInTextBoxes(StringPartnerList lstText)
{
	HRESULT hr = S_OK;

	COleVariant vFalse((short)FALSE);
	COleVariant vOpt(DISP_E_PARAMNOTFOUND, VT_ERROR);

	CShapes shapes = m_WordDoc.get_Shapes();
	long lcount = shapes.get_Count();
	for (long lIndex = 1; lIndex < lcount; lIndex++)
	{
		CShape shape;
		shape = shapes.Item(&COleVariant((LONGLONG)lIndex));
		shape.Select(&COleVariant((short)TRUE));
		CSelection sel = m_WordApp.get_Selection();

		CReplacement rpInDoc;
		CFind fndInDoc = sel.get_Find();
		fndInDoc.ClearFormatting(); 
		rpInDoc = fndInDoc.get_Replacement(); 
		rpInDoc.ClearFormatting(); 
		for (StringPartnerList::iterator it = lstText.begin();
			 it != lstText.end();
			 it++)
		{
			StringPartner strings = *it;

			fndInDoc.Execute(&COleVariant(strings.strSrcString), vOpt, vOpt, vOpt, vOpt, 
							 vOpt, vOpt, vOpt, vOpt, &COleVariant(strings.strDesString), 
							 &COleVariant((short)wdReplaceAll), vOpt, vOpt, vOpt, vOpt); 
		}
	}
	return hr;
}
//------------------------------------------------
HRESULT	CMSWordApplication::SetTableCell(long lTableIndex, 
										 long lRow, 
										 long lColumn, 
										 CString strText)
{
	HRESULT hr = S_OK;

	CTables0 tables;
	CTable0 table;
	tables = m_WordDoc.get_Tables();
	long lTableCnt = tables.get_Count();
	if (lTableIndex > lTableCnt)
		return S_FALSE;

	table = tables.Item(lTableIndex);
	CCell cell = table.Cell(lRow, lColumn);
	CRange range = cell.get_Range();
	range.put_Text(strText);

	return hr;
}
//------------------------------------------------
HRESULT CMSWordApplication::AddStringAfter(CString strPosName, 
										   CString strText, 
										   BOOL bNewParagraph)
{
	HRESULT hr = S_OK;

	COleVariant vFalse((short)FALSE);
	COleVariant vOpt(DISP_E_PARAMNOTFOUND, VT_ERROR);

	CSelection selection;   
	selection = m_WordApp.get_Selection();
	//CRange range = m_WordDoc.get_Content();
	//CFind fndInDoc = range.get_Find();
	CFind fndInDoc = selection.get_Find();
	CReplacement rpInDoc;
	fndInDoc.ClearFormatting(); 
	rpInDoc = fndInDoc.get_Replacement(); 
	rpInDoc.ClearFormatting(); 

	fndInDoc.Execute(&COleVariant(strPosName), vOpt, vOpt, vOpt, vOpt, 
					 vOpt, vOpt, vOpt, vOpt, vOpt, 
					 vOpt, vOpt, vOpt, vOpt, vOpt); 

	selection.MoveDown(COleVariant((short)6), vOpt, vOpt);
	if (bNewParagraph) 
	{
		selection.TypeParagraph();   
	}
	selection.TypeText(strText); 

	return hr;
}
//------------------------------------------------
HRESULT	CMSWordApplication::AddStringToEnd(CString strText,
										   BOOL bNewParagraph)
{
	HRESULT hr = S_OK;

	CSelection selection;   
	selection = m_WordApp.get_Selection();

	COleVariant vOpt(DISP_E_PARAMNOTFOUND, VT_ERROR);
	selection.EndKey(COleVariant((short)6), vOpt);
	if (bNewParagraph) 
	{
		selection.TypeParagraph();   
	}
	selection.TypeText(strText);   

	return hr;
}
//------------------------------------------------
