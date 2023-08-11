#ifndef _XBASETEXTURE_MANAGER_H_
#define _XBASETEXTURE_MANAGER_H_
#include "../xcomdoc/xStringHash.h"
#include "CD3DInclude.h"
#include "xResPackageMgr.h"
class _A3D_API_  CTextureName
{
public:
	std::wstring  m_Name;
    D3DFORMAT     m_Format;
public:
	CTextureName();
	CTextureName(const wchar_t* fileName);
};

inline bool operator == (const CTextureName& lhv , const CTextureName& rhv)
{
	return xStringHash(lhv.m_Name.c_str() ) + lhv.m_Format == xStringHash(rhv.m_Name.c_str() ) + rhv.m_Format ;
}

inline bool operator < (const CTextureName& lhv , const CTextureName& rhv)
{
	return xStringHash(lhv.m_Name.c_str() ) + lhv.m_Format < xStringHash(rhv.m_Name.c_str() ) + rhv.m_Format ;
}

inline size_t hash_value(const CTextureName& _Keyval)
{
	return ( xStringHash(_Keyval.m_Name.c_str() ) + _Keyval.m_Format ) ;
}



class   CBaseTextureLoader : public xResPathManager
{
public:	
	CBaseTextureLoader();
	HBaseTexture         load(const CTextureName& texName , bool bLoadImm  , bool arg);
    CA3DDeviceWrapper*   GetDevice(){ return m_DeviceWrapper ; }
    void                 SetDevice(CA3DDeviceWrapper* pDevice){ m_DeviceWrapper = pDevice ; }
protected:
	virtual unsigned int _getResSize(xA3DTexture* pRes);
	virtual bool         _isResLoaded(xA3DTexture* pRes);
	virtual bool         _preLoadResource(const CTextureName& strResName   , xA3DTexture* & pRes , int& ResSize, unsigned int arg)  { return true ; }
	virtual bool         _postLoadResource(const CTextureName& strResName  , xA3DTexture* & pRes , int& ResSize, unsigned int arg) { return true ; }
	virtual bool         _loadResource(const CTextureName& strResName   , xA3DTexture* & pRes , int& ResSize, unsigned int arg);
	virtual bool         _unloadResource(const CTextureName& strResName , xA3DTexture* & pRes , unsigned int& TotalResSize);
	virtual void         _deleteResource(const CTextureName& strResName , xA3DTexture* pRes);
protected:
	xA3DTexture*        _loadTexture( const CTextureName& _texName , const wchar_t* fileName ,BYTE* buf , int bufLen , unsigned int arg);
protected:
    CA3DDeviceWrapper*     m_DeviceWrapper;
};


#endif

