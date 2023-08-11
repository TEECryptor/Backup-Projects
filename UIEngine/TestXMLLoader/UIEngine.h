//------------------------------------------------
//	Name:UIEngine.h
//	Introduce:The UI engine class, load UI elements
//			  from skin file.
//	Creator:Youfang Yao
//	Date:2007-04-03
//	Version:1.0V
//------------------------------------------------
#ifndef	_UIENGINE_H_
#define _UIENGINE_H_
//------------------------------------------------
#include "XMLHelper.h"	
#include <vector>
using namespace std;
//------------------------------------------------
//	Name:BKImageInfo
//	Introduce:The background image information definition
//------------------------------------------------
typedef struct tagBKImageInfo
{
	TCHAR	szImage[MAX_PATH];
	TCHAR	szColorStr[32];
}BKImageInfo, *LPBKIMAGEINFO;
//------------------------------------------------
//	Name:ImgObjInfo
//	Introduce:The image object information definition
//------------------------------------------------
typedef struct tagImgObjInfo
{
	TCHAR	szImage[MAX_PATH];
	int		iLeft;
	int		iTop;
}ImgObjInfo, *LPIMGOBJINFO;
//------------------------------------------------
//	Name:BtnState
//	Introduce:Button state definition
//------------------------------------------------	
typedef enum eBtnState
{
	BS_NORMAL		= 0,
	BS_SELECTED		= 1,
	BS_DOWN			= 2,
	BS_DISABLE		= 3,
	BS_CNT
}BtnState;
//------------------------------------------------
//	Name:ButtonInfo
//	Introduce:Button information definition
//------------------------------------------------		
typedef struct tagButtonInfo
{
	long	lID;
	int		iLeft;
	int		iTop;
	TCHAR	szImage[BS_CNT][MAX_PATH];
	TCHAR	szHitStr[256];
}ButtonInfo, *LPBUTTONINFO;
//------------------------------------------------
//	Name:CUIEngine
//	Introduce:The UI engine class
//------------------------------------------------
class CUIEngine
{
public:
	CUIEngine();
	~CUIEngine();
	//--------------------------------------------
public:
	//Set the XML file name for skin definition 
	BOOL	SetSkinFile(LPCTSTR lpszFile);
	//Return background image count
	UINT	GetBKImageCnt();
	//Return background image information
	void	GetBKImageInfo(int iIndex, LPBKIMAGEINFO lpBKImage);
	//Return image object count
	UINT	GetImageObjCnt();
	//Return background image information
	void	GetImageObjInfo(int iIndex, LPIMGOBJINFO lpImgObjInfo);
	//Return the button count 
	UINT	GetButtonCnt();
	//Return the button information based on index 
	void	GetButtonObj(int iIndex, LPBUTTONINFO lpButton);
	//--------------------------------------------
private:
	//Load a background image files from XML file
	BOOL	LoadBKImages(IXMLDOMDocumentPtr pDoc);
	//Load the all image files information from XML file
	BOOL	LoadImgObjs(IXMLDOMDocumentPtr pDoc);
	//Load all button information from XML file
	BOOL	LoadButtons(IXMLDOMDocumentPtr pDoc);
	//Release all the objects loaded
	void	Release();
	//--------------------------------------------
private:
	IXMLDOMDocumentPtr		m_pXMLDoc;
	//
	vector<LPBKIMAGEINFO>	m_lstBKImageInfo;
	vector<LPIMGOBJINFO>	m_lstImgObjInfo;
	vector<LPBUTTONINFO>	m_lstButtonInfo;
	//--------------------------------------------
};
//------------------------------------------------
#endif	//_UIENGINE_H_	
//------------------------------------------------	
