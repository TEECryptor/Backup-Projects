//-----------------------------------------
//	FileName:Texture.h
//	Introduce:
//	Creator:Youfang Yao
//	Date:11-09-06
//-----------------------------------------
#ifndef	_TEXTURE_H_
#define	_TEXTURE_H_
//-----------------------------------------
#include <d3d9.h>
#include <d3dx9tex.h>
#include <vector>
#include <fstream>
//-----------------------------------------
using namespace std;
//-----------------------------------------
//	Name:CTexture
//	Introduce:The texture object base class definition
//-----------------------------------------
class CTexture
{
public:
	CTexture();
	virtual ~CTexture();
	//-------------------------------------
public:
	/*	Name:LoadTextureFromFile
		Introduce:Load the texture object from file
		Param:lpd3dDevice:i:The D3D device interface pointer
		      lpszTexFile:i:The texture full file name
		Return:Retuen S_OK if successful
	*/
	virtual HRESULT LoadTextureFromFile(LPDIRECT3DDEVICE9 lpd3dDevice, LPCTSTR lpszTexFile) = 0;

	/*	Name:GetTexturePointer
		Introduce:Return the current texture object pointer
		Param:N/A
		Return:The current texture pointer
	*/
	virtual LPDIRECT3DTEXTURE9 GetTexturePointer(int &iIndex, long &lTimeLen, BOOL bLoop = true);

	/*	Name:GetTexFileName
		Introduce:Return the texture file name string
		Param:N/A
		Return:The texture file name string
	*/
	virtual LPCTSTR GetTexFileName();
	//-------------------------------------
protected:
	TCHAR				m_szTexFileName[MAX_PATH];	//The texture full file name
	LPDIRECT3DTEXTURE9	m_lpTexture;				//The texture object pointer
	//-------------------------------------
};
//-----------------------------------------



//-----------------------------------------
//	Name:CStaticTexture
//	Introduce:The static texture object definition
//-----------------------------------------
class CStaticTexture : public CTexture
{
public:
	CStaticTexture();
	virtual ~CStaticTexture();
	//-------------------------------------
public:
	//To load the texture object from file
	HRESULT		LoadTextureFromFile(LPDIRECT3DDEVICE9 lpd3dDevice, LPCTSTR lpszTexFile);
	//-------------------------------------
};
//-----------------------------------------



//-----------------------------------------
//	Name:CDynamicTexture
//	Introduce:The dynamic texture object definition
//-----------------------------------------
class CDynamicTexture : public CTexture
{
public:
	CDynamicTexture();
	virtual ~CDynamicTexture();
	//-------------------------------------
public:	
	//To set the frame texture file path string
	void				SetTextreFilePath(LPCTSTR lpszPath);
	//To load the texture objects from file
	HRESULT				LoadTextureFromFile(LPDIRECT3DDEVICE9 lpd3dDevice, LPCTSTR lpszTexFile);
	//To return the texture pointer
	LPDIRECT3DTEXTURE9	GetTexturePointer(int &iIndex, long &lTimeLen, BOOL bLoop = true);
	//-------------------------------------
private:
	TCHAR					m_szTexFilePath[MAX_PATH];	//The frame texture file path string
	vector<CStaticTexture*>	m_lstTextureItem;			//The texture objects list
	vector<long>			m_lstTexTimeLen;			//The time lenght for every frame texture
	//-------------------------------------
};
//-----------------------------------------
#endif	//_TEXTURE_H_
//-----------------------------------------