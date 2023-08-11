//-----------------------------------------
//	FileName:Texture.cpp
//	Introduce:
//	Creator:Youfang Yao
//	Date:11-09-06
//-----------------------------------------
#include "stdafx.h"
#include <TCHAR.h>
#include "Texture.h"
//-----------------------------------------
//	Name:CTexture
//	Introduce:Constructor
//-----------------------------------------
CTexture::CTexture()
 : m_lpTexture(NULL)
{
	lstrcpy(m_szTexFileName, _T("\0"));
}
//-----------------------------------------
//	Name:~CTexture
//	Introduce:Distructor
//-----------------------------------------
CTexture::~CTexture()
{
	if(m_lpTexture)
	{
		m_lpTexture->Release();
		m_lpTexture = NULL;
	}
}
//-----------------------------------------
/*	Name:GetTexturePointer
	Introduce:Return the current texture object pointer
	Param:N/A
	Return:The current texture pointer
*/
//-----------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexturePointer(int &iIndex, long &lTimeLen, BOOL bLoop)
{
	iIndex = 0;
	lTimeLen = 0;
	return m_lpTexture;
}
//-----------------------------------------

/*	Name:GetTexFileName
	Introduce:Return the texture file name string
	Param:N/A
	Return:The texture file name string
*/
//-----------------------------------------
LPCTSTR CTexture::GetTexFileName()
{
	return m_szTexFileName;
}
//-----------------------------------------



//-----------------------------------------
//	Name:CStaticTexture
//	Introduce:Constructor
//-----------------------------------------
CStaticTexture::CStaticTexture()
{
}
//-----------------------------------------
//	Name:~CStaticTexture
//	Introduce:Distructor
//-----------------------------------------
CStaticTexture::~CStaticTexture()
{
}
//-----------------------------------------
//	Name:LoadTextureFromFile
//	Introduce:To load the texture object from file
//-----------------------------------------
HRESULT	CStaticTexture::LoadTextureFromFile(LPDIRECT3DDEVICE9 lpd3dDevice, LPCTSTR lpszTexFile)
{
	if(lpd3dDevice == NULL || lpszTexFile == NULL)
		return E_POINTER;
	
	if(m_lpTexture)
		m_lpTexture->Release();

	HRESULT hr = S_OK;

	//To load the texture from file
	hr = D3DXCreateTextureFromFile(lpd3dDevice, lpszTexFile, &m_lpTexture);
	if(FAILED(hr))
		return hr;
	
	//Save the texture file name
	TCHAR szTexFile[MAX_PATH] = {0};
	lstrcpy(szTexFile, lpszTexFile);
	_tcslwr(szTexFile);
	lstrcpy(m_szTexFileName, szTexFile);

	return S_OK;
}
//-----------------------------------------



//-----------------------------------------
//	Name:CDynamicTexture
//	Introduce:Constructor
//-----------------------------------------
CDynamicTexture::CDynamicTexture()
{
	lstrcpy(m_szTexFilePath, _T("\0"));
}
//-----------------------------------------
//	Name:~CDynamicTexture
//	Introduce:Distructor
//-----------------------------------------
CDynamicTexture::~CDynamicTexture()
{
	CStaticTexture* pTexture = NULL;
	for(int iCnt = 0;iCnt < m_lstTextureItem.size(); iCnt++)
	{
		pTexture = m_lstTextureItem[iCnt];
		if(pTexture)
		{
			delete pTexture;
			pTexture = NULL;
		}
	}
	m_lstTextureItem.clear();
}
//-----------------------------------------	
//	Name:SetTextreFilePath
//	Introduce:To set the frame texture file path string
//-----------------------------------------	
void CDynamicTexture::SetTextreFilePath(LPCTSTR lpszPath)
{
	lstrcpy(m_szTexFilePath, lpszPath);
}
//-----------------------------------------	
//	Name:LoadTextureFromFile
//	Introduce:To load the texture objects from file
//-----------------------------------------	
HRESULT	CDynamicTexture::LoadTextureFromFile(LPDIRECT3DDEVICE9 lpd3dDevice, LPCTSTR lpszTexFile)
{
	if(lpd3dDevice == NULL || lpszTexFile == NULL)
		return E_POINTER;

	HRESULT hr = S_OK;

	//Release the texture list
	CStaticTexture* pTexture = NULL;
	for(int iCnt = 0;iCnt < m_lstTextureItem.size(); iCnt++)
	{
		pTexture = m_lstTextureItem[iCnt];
		if(pTexture)
		{
			delete pTexture;
			pTexture = NULL;
		}
	}
	m_lstTextureItem.clear();

	//Release the texture time lenght list
	m_lstTexTimeLen.clear();

	TCHAR szFileName[MAX_PATH] = {0};
	ifstream ilkFile(lpszTexFile);

	//Load the dynamic texture definition file
	while(true)
	{
		//Read a frame texture file name
		ilkFile.getline(szFileName, MAX_PATH);
		if(lstrlen(szFileName) == 0)
		{
			break;
		}            		
		TCHAR szFullFileName[MAX_PATH] = {0};
		lstrcpy(szFullFileName, m_szTexFilePath);
		lstrcat(szFullFileName, szFileName);
		_tcslwr(szFullFileName);

		//Read the time lenght for current frame
		TCHAR szTime[32] = {_T("\0")};
		ilkFile.getline(szTime, 32);
		long lTimeLen = _ttol(szTime);

		//Load the frame texture object from file
		CStaticTexture* pStaticTex = new CStaticTexture();
		hr = pStaticTex->LoadTextureFromFile(lpd3dDevice, szFullFileName);
		if(FAILED(hr))
		{
			delete pStaticTex;
			pStaticTex = NULL;
			return hr;
		}

		//Add the frame texture pointer to the list
		m_lstTextureItem.push_back(pStaticTex);
		m_lstTexTimeLen.push_back(lTimeLen);
	}

	//Save the texture file name
	TCHAR szTexFile[MAX_PATH] = {0};
	lstrcpy(szTexFile, lpszTexFile);
	_tcslwr(szTexFile);
	lstrcpy(m_szTexFileName, szTexFile);

	return S_OK;
}
//-----------------------------------------
//	Name:GetTexturePointer
//	Introduce:To return the texture pointer
//-----------------------------------------	
LPDIRECT3DTEXTURE9	CDynamicTexture::GetTexturePointer(int &iIndex, long &lTimeLen, BOOL bLoop)
{
	if(m_lstTextureItem.size() <= 0)
	{
		iIndex = 0;
		lTimeLen = 0;
		return NULL;
	}

	//At the begin
	if(iIndex < 0)
	{
		iIndex = 0;
	}

	//At the end
	if(iIndex >= m_lstTextureItem.size())
	{
		if(bLoop)
		{
			iIndex = 0;
		}
		else
		{
			iIndex = m_lstTextureItem.size() - 1;
		}
	}

	//The time lenght for this frame texture
	lTimeLen = m_lstTexTimeLen[iIndex];

	//Return the frame texture
	int iTmpIndex = 0;
	long lTempTimeLen = 0;
	return m_lstTextureItem[iIndex]->GetTexturePointer(iTmpIndex, lTempTimeLen);
}
//-----------------------------------------	

