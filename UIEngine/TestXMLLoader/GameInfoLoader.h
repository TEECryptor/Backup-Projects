//------------------------------------------------
//	Name:GameInfoLoader.h
//	Introduce:The game information loader class, load 
//		      all games information from definition file.
//	Creator:Youfang Yao
//	Date:2007-04-04
//	Version:1.0V
//------------------------------------------------
#ifndef	_GAMEINFO_LOADER_H_
#define _GAMEINFO_LOADER_H_
//------------------------------------------------
#include "XMLHelper.h"	
#include <vector>
using namespace std;
//------------------------------------------------
typedef struct tagGameInfo
{
	int		iType;
	TCHAR	szDisplayName[128];
	TCHAR	szGUID[40];
}GameInfo, *LPGAMEINFO;
//------------------------------------------------
class CGameInfoLoader
{
public:
	CGameInfoLoader();
	~CGameInfoLoader();
	//--------------------------------------------
public:
	//Set the XML file name for game information definition 
	BOOL	SetGameInfoFile(LPCTSTR lpszFile);
	//Return the game information count 
	UINT	GetGameInfoCnt();
	//Return the game information based on index 
	void	GetGameInfo(int iIndex, LPGAMEINFO lpGameInfo);
	//--------------------------------------------
private:
	//Load all game information from XML file
	BOOL	LoadGameInfo(IXMLDOMDocumentPtr pDoc);
	//--------------------------------------------
private:
	IXMLDOMDocumentPtr		m_pXMLDoc;
	vector<LPGAMEINFO>		m_lstGameInfo;
	//--------------------------------------------
};
//------------------------------------------------
#endif	//_GAMEINFO_LOADER_H_
//------------------------------------------------