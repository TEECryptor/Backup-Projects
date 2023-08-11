//------------------------------------------------	
//	Name:XMLHelper.cpp
//	Introduce:The cpp file for XML file loader help class
//	Creator:Youfang Yao
//	Date:2007-04-03
//	Version:1.0V
//------------------------------------------------	
#include "stdafx.h"
#include "XMLHelper.h"
//------------------------------------------------
//	Name:CXMLHelper
//	Introduce:Constructor
//------------------------------------------------	
CXMLHelper::CXMLHelper()
{
}
//------------------------------------------------
//	Name:~CXMLHelper
//	Introduce:Distructor
//------------------------------------------------	
CXMLHelper::~CXMLHelper()
{
}
//------------------------------------------------	
//	Name:LoadXMLFile
//	Introduce:Load a XML file use specific document pointer
//------------------------------------------------	
BOOL CXMLHelper::LoadXMLFile(IXMLDOMDocumentPtr &pDocument, LPCTSTR lpszFile)
{
	if(NULL == lpszFile)
		return FALSE;

	HRESULT hr = S_OK;
	hr = CoInitialize(NULL);
	if(FAILED(hr))
		return FALSE;

	if(NULL == pDocument)
	{
		hr = pDocument.CreateInstance(__uuidof(DOMDocument));
		if(FAILED(hr)) 
			return FALSE;
	}
	pDocument->async = FALSE;     

	hr = pDocument->load(_bstr_t(lpszFile));
	if (VARIANT_TRUE != hr)
		return FALSE;
	
	return TRUE;
}
//------------------------------------------------	
//	Name:GetNodeAttribute
//	Introduce:Return a string attribute of the specific node by attribute name
//------------------------------------------------	
BOOL CXMLHelper::GetNodeAttribute(IXMLDOMElementPtr pElement, LPCTSTR lpszName, LPTSTR lpValue)
{
	if(NULL == pElement)
		return FALSE;

	if(NULL == lpszName || NULL == lpValue)
		return FALSE;

	_variant_t var;
	var = pElement->getAttribute(lpszName);
	if(var.vt != VT_BSTR)
		return FALSE;

	TCHAR* lpszVal = _com_util::ConvertBSTRToString(var.bstrVal);
	lstrcpy(lpValue, lpszVal);

	delete lpszVal;
	lpszVal = NULL;

	return TRUE;
}
//------------------------------------------------	
//	Name:GetNodeAttribute
//	Introduce:Return a int attribute of the specific node by attribute name
//------------------------------------------------	
BOOL CXMLHelper::GetNodeAttribute(IXMLDOMElementPtr pElement, LPCTSTR lpszName, int* lpValue)
{
	if(NULL == lpValue)
		return FALSE;

	TCHAR szValue[32] = {_T("\0")};
	if(!GetNodeAttribute(pElement, lpszName, szValue))
		return FALSE;

	*lpValue = _ttoi(szValue);

	return TRUE;
}
//------------------------------------------------	
//	Name:GetNodeAttribute
//	Introduce:Return a long attribute of the specific node by attribute name
//------------------------------------------------	
BOOL CXMLHelper::GetNodeAttribute(IXMLDOMElementPtr pElement, LPCTSTR lpszName, long* lpValue)
{
	if(NULL == lpValue)
		return FALSE;

	TCHAR szValue[32] = {_T("\0")};
	if(!GetNodeAttribute(pElement, lpszName, szValue))
		return FALSE;

	*lpValue = _ttol(szValue);

	return TRUE;
}
//------------------------------------------------	
