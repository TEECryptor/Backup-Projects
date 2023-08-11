//------------------------------------------------
//	Name:GameInfoLoader.cpp
//	Introduce:The cpp file for game information loader class.
//	Creator:Youfang Yao
//	Date:2007-04-04
//	Version:1.0V
//------------------------------------------------
#include "stdafx.h"
#include "GameInfoLoader.h"
//------------------------------------------------
//	Name:CGameInfoLoader
//	Introduce:Constructor
//------------------------------------------------
CGameInfoLoader::CGameInfoLoader()
 : m_pXMLDoc(NULL)
{
}
//------------------------------------------------
//	Name:~CGameInfoLoader
//	Introduce:Distructor
//------------------------------------------------
CGameInfoLoader::~CGameInfoLoader()
{
	LPGAMEINFO lpGameInfo = NULL;
	for(int i = 0; i < m_lstGameInfo.size(); i++)
	{
		lpGameInfo = m_lstGameInfo[i];
		if(NULL != lpGameInfo)
		{
			delete lpGameInfo;
			lpGameInfo = NULL;
		}
	}
}
//------------------------------------------------
//	Name:SetGameInfoFile
//	Introduce:Set the XML file name for game information definition 
//------------------------------------------------
BOOL CGameInfoLoader::SetGameInfoFile(LPCTSTR lpszFile)
{
	if(NULL == lpszFile)
		return FALSE;

	if(NULL != m_pXMLDoc)
		m_pXMLDoc.Release();

	if(!CXMLHelper::LoadXMLFile(m_pXMLDoc, lpszFile))
		return FALSE;

	if(!LoadGameInfo(m_pXMLDoc))
		return FALSE;

	return TRUE;
}
//------------------------------------------------
//	Name:GetGameInfoCnt
//	Introduce:Return the game information count 
//------------------------------------------------
UINT CGameInfoLoader::GetGameInfoCnt()
{
	return m_lstGameInfo.size();
}
//------------------------------------------------
//	Name:GetGameInfo
//	Introduce:Return the game information based on index 
//------------------------------------------------
void CGameInfoLoader::GetGameInfo(int iIndex, LPGAMEINFO lpGameInfo)
{
	if(NULL == lpGameInfo)
		return;

	if(iIndex < 0 || iIndex > m_lstGameInfo.size()-1)
		return;

	LPGAMEINFO lpGame = m_lstGameInfo[iIndex];
	if(NULL != lpGame)
		memcpy(lpGameInfo, lpGame, sizeof(GameInfo));
}
//------------------------------------------------


//------------------------------------------------
//	Name:LoadGameInfo
//	Introduce:Load all game information from XML file
//------------------------------------------------
BOOL CGameInfoLoader::LoadGameInfo(IXMLDOMDocumentPtr pDoc)
{
	IXMLDOMNodeListPtr pIDOMNodeList = NULL;
	IXMLDOMNodePtr pIDOMNode = NULL;
	IXMLDOMElementPtr pElement = NULL;

	HRESULT hr = S_OK;
	pIDOMNode = pDoc->selectSingleNode(_T("//Games"));
	if(NULL == pIDOMNode) 
		return FALSE;

	hr = pIDOMNode->get_childNodes(&pIDOMNodeList);
	if(FAILED(hr)) 
		return FALSE;

	pElement = pIDOMNodeList->nextNode();
	while(NULL != pElement)
	{	
		LPGAMEINFO lpGameInfo = new GameInfo;
		memset(lpGameInfo, 0, sizeof(GameInfo));
		CXMLHelper::GetNodeAttribute(pElement, _T("type"), &lpGameInfo->iType);
		CXMLHelper::GetNodeAttribute(pElement, _T("displayname"), lpGameInfo->szDisplayName);
		CXMLHelper::GetNodeAttribute(pElement, _T("GUID"), lpGameInfo->szGUID);
		m_lstGameInfo.push_back(lpGameInfo);
		
		pElement = pIDOMNodeList->nextNode();
	}

	return TRUE;
}
//------------------------------------------------
