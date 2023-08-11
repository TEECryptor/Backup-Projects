//------------------------------------------------
//	Name:UIEngine.cpp
//	Introduce:The cpp file for UI engine class
//	Creator:Youfang Yao
//	Date:2007-04-03
//	Version:1.0V
//------------------------------------------------	
#include "stdafx.h"
#include "UIEngine.h"
//------------------------------------------------	
//	Name:CUIEngine
//	Introduce:Constructor
//------------------------------------------------	
CUIEngine::CUIEngine()
 : m_pXMLDoc(NULL)
{
}
//------------------------------------------------
//	Name:~CUIEngine
//	Introduce:Distructor
//------------------------------------------------		
CUIEngine::~CUIEngine()
{
	Release();
}
//------------------------------------------------	
//	Name:SetSkinFile
//	Introduce:Set the XML file name for skin definition 
//------------------------------------------------	
BOOL CUIEngine::SetSkinFile(LPCTSTR lpszFile)
{
	if(NULL == lpszFile)
		return FALSE;

	if(NULL != m_pXMLDoc)
		m_pXMLDoc.Release();

	if(!CXMLHelper::LoadXMLFile(m_pXMLDoc, lpszFile))
		return FALSE;

	Release();

	if(!LoadBKImages(m_pXMLDoc))
		return FALSE;

	if(!LoadImgObjs(m_pXMLDoc))
		return FALSE;

	if(!LoadButtons(m_pXMLDoc))
		return FALSE;

	return TRUE;
}
//------------------------------------------------
//	Name:GetBKImageCnt
//	Introduce:Return background image count
//------------------------------------------------
UINT CUIEngine::GetBKImageCnt()
{
	return m_lstBKImageInfo.size();
}
//------------------------------------------------
//	Name:GetBKImageInfo
//	Introduce:Return background image information
//------------------------------------------------
void CUIEngine::GetBKImageInfo(int iIndex, LPBKIMAGEINFO lpBKImage)
{
	if(NULL == lpBKImage)
		return;

	if(iIndex < 0 || iIndex > m_lstBKImageInfo.size()-1)
		return;

	LPBKIMAGEINFO lpBKInfo = m_lstBKImageInfo[iIndex];
	if(NULL != lpBKInfo)
		memcpy(lpBKImage, lpBKInfo, sizeof(BKImageInfo));
}
//------------------------------------------------
//	Name:GetImageObjCnt
//	Introduce:Return image object count
//------------------------------------------------
UINT CUIEngine::GetImageObjCnt()
{
	return m_lstImgObjInfo.size();
}
//------------------------------------------------
//	Name:GetImageObjInfo
//	Introduce:Return the image object information
//------------------------------------------------
void CUIEngine::GetImageObjInfo(int iIndex, LPIMGOBJINFO lpImgObjInfo)
{
	if(NULL == lpImgObjInfo)
		return;

	if(iIndex < 0 || iIndex > m_lstImgObjInfo.size()-1)
		return;

	LPIMGOBJINFO lpImgInfo = m_lstImgObjInfo[iIndex];
	if(NULL != lpImgInfo)
		memcpy(lpImgObjInfo, lpImgInfo, sizeof(ImgObjInfo));
}
//------------------------------------------------
//	Name:GetButtonCnt
//	Introduce:Return the button count 
//------------------------------------------------	
UINT CUIEngine::GetButtonCnt()
{
	return m_lstButtonInfo.size();
}
//------------------------------------------------
//	Name:GetButtonObj
//	Introduce:Return the button information based on index 
//------------------------------------------------
void CUIEngine::GetButtonObj(int iIndex, LPBUTTONINFO lpButton)
{
	if(NULL == lpButton)
		return;

	if(iIndex < 0 || iIndex > m_lstButtonInfo.size()-1)
		return;

	LPBUTTONINFO lpObj = m_lstButtonInfo[iIndex];
	if(NULL != lpObj)
		memcpy(lpButton, lpObj, sizeof(ButtonInfo));
}
//------------------------------------------------



//------------------------------------------------
//	Name:LoadBKImages
//	Introduce:Load a background image files from XML file
//------------------------------------------------
BOOL CUIEngine::LoadBKImages(IXMLDOMDocumentPtr pDoc)
{
	IXMLDOMNodeListPtr pIDOMNodeList = NULL;
	IXMLDOMNodePtr pIDOMNode = NULL;
	IXMLDOMElementPtr pElement = NULL;

	HRESULT hr = S_OK;
	pIDOMNode = pDoc->selectSingleNode(_T("//BK"));
	if(NULL == pIDOMNode) 
		return FALSE;

	hr = pIDOMNode->get_childNodes(&pIDOMNodeList);
	if(FAILED(hr)) 
		return FALSE;

	pElement = pIDOMNodeList->nextNode();
	while(NULL != pElement)
	{	
		LPBKIMAGEINFO lpBKInfo = new BKImageInfo;
		memset(lpBKInfo, 0, sizeof(BKImageInfo));
		CXMLHelper::GetNodeAttribute(pElement, _T("img"), lpBKInfo->szImage);
		CXMLHelper::GetNodeAttribute(pElement, _T("color"), lpBKInfo->szColorStr);
		m_lstBKImageInfo.push_back(lpBKInfo);

		pElement = pIDOMNodeList->nextNode();
	}

	return TRUE;
}
//------------------------------------------------
//	Name:LoadImgObjs
//	Introduce:Load the all image files information from XML file
//------------------------------------------------
BOOL CUIEngine::LoadImgObjs(IXMLDOMDocumentPtr pDoc)
{
	IXMLDOMNodeListPtr pIDOMNodeList = NULL;
	IXMLDOMNodePtr pIDOMNode = NULL;
	IXMLDOMElementPtr pElement = NULL;

	HRESULT hr = S_OK;
	pIDOMNode = pDoc->selectSingleNode(_T("//Images"));
	if(NULL == pIDOMNode) 
		return FALSE;

	hr = pIDOMNode->get_childNodes(&pIDOMNodeList);
	if(FAILED(hr)) 
		return FALSE;

	pElement = pIDOMNodeList->nextNode();
	while(NULL != pElement)
	{	
		LPIMGOBJINFO lpImgInfo = new ImgObjInfo;
		memset(lpImgInfo, 0, sizeof(ImgObjInfo));
		CXMLHelper::GetNodeAttribute(pElement, _T("img"), lpImgInfo->szImage);
		CXMLHelper::GetNodeAttribute(pElement, _T("left"), &lpImgInfo->iLeft);
		CXMLHelper::GetNodeAttribute(pElement, _T("top"), &lpImgInfo->iTop);
		m_lstImgObjInfo.push_back(lpImgInfo);

		pElement = pIDOMNodeList->nextNode();
	}

	return TRUE;
}
//------------------------------------------------
//	Name:LoadButtons
//	Introduce:Load all button information from XML file
//------------------------------------------------
BOOL CUIEngine::LoadButtons(IXMLDOMDocumentPtr pDoc)
{
	IXMLDOMNodeListPtr pIDOMNodeList = NULL;
	IXMLDOMNodePtr pIDOMNode = NULL;
	IXMLDOMElementPtr pElement = NULL;

	HRESULT hr = S_OK;
	pIDOMNode = pDoc->selectSingleNode(_T("//Buttons"));
	if(NULL == pIDOMNode) 
		return FALSE;

	hr = pIDOMNode->get_childNodes(&pIDOMNodeList);
	if(FAILED(hr)) 
		return FALSE;

	pElement = pIDOMNodeList->nextNode();
	while(NULL != pElement)
	{	
		LPBUTTONINFO lpBtnInfo = new ButtonInfo;
		memset(lpBtnInfo, 0, sizeof(ButtonInfo));
		CXMLHelper::GetNodeAttribute(pElement, _T("ID"), &lpBtnInfo->lID);
		CXMLHelper::GetNodeAttribute(pElement, _T("left"), &lpBtnInfo->iLeft);
		CXMLHelper::GetNodeAttribute(pElement, _T("top"), &lpBtnInfo->iTop);
		CXMLHelper::GetNodeAttribute(pElement, _T("normal"), lpBtnInfo->szImage[BS_NORMAL]);
		CXMLHelper::GetNodeAttribute(pElement, _T("selected"), lpBtnInfo->szImage[BS_SELECTED]);
		CXMLHelper::GetNodeAttribute(pElement, _T("down"), lpBtnInfo->szImage[BS_DOWN]);
		CXMLHelper::GetNodeAttribute(pElement, _T("disable"), lpBtnInfo->szImage[BS_DISABLE]);
		CXMLHelper::GetNodeAttribute(pElement, _T("hitstr"), lpBtnInfo->szHitStr);
		m_lstButtonInfo.push_back(lpBtnInfo);
		
		pElement = pIDOMNodeList->nextNode();
	}

	return TRUE;
}	
//------------------------------------------------
//	Name:Release
//	Introduce:Release all the objects loaded
//------------------------------------------------
void CUIEngine::Release()
{
	LPBKIMAGEINFO lpBKInfo = NULL;
	for(int i = 0; i < m_lstBKImageInfo.size(); i++)
	{
		lpBKInfo = m_lstBKImageInfo[i];
		if(NULL != lpBKInfo)
		{
			delete lpBKInfo;
			lpBKInfo = NULL;
		}
	}
	m_lstBKImageInfo.clear();

	LPIMGOBJINFO lpImgObjInfo = NULL;
	for(i = 0; i < m_lstImgObjInfo.size(); i++)
	{
		lpImgObjInfo = m_lstImgObjInfo[i];
		if(NULL != lpImgObjInfo)
		{
			delete lpImgObjInfo;
			lpImgObjInfo = NULL;
		}
	}
	m_lstImgObjInfo.clear();

	LPBUTTONINFO lpButtonInfo = NULL;
	for(i = 0; i < m_lstButtonInfo.size(); i++)
	{
		lpButtonInfo = m_lstButtonInfo[i];
		if(NULL != lpButtonInfo)
		{
			delete lpButtonInfo;
			lpButtonInfo = NULL;
		}
	}
	m_lstButtonInfo.clear();
}
//------------------------------------------------


