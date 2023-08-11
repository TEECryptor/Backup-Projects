#include "TextureManager.h"
#include "CSysFunc.h"
#include "VGSRender.h"
#include "VGSSupport.h"

#include "CTexture.h"
#include "CDynamicTex.h"
#include "CCubemap.h"
#include "BumpMap.h"
#include "SwfTexture.H"
#include "Root.h"

//template <> CTextureManager* Singleton<CTextureManager>::ms_Singleton = 0;

CTextureManager::CTextureManager()
	:mVGSRender(0)
{
	
}
CTextureManager::~CTextureManager()
{
	Release();
}
VOID CTextureManager::Release()
{
// 	std::map<std::string, CTexture*>::iterator it = m_vTextureMap.begin();
// 	while (it != m_vTextureMap.end())
// 	{
// 		if (it->second)
// 		{
// 			SAFE_RELEASE(it->second);
// 			SAFE_DELETE(it->second);
// 		}
// 		++it;
// 	}
// 	m_vTextureMap.clear();
}
void CTextureManager::setVGSRender(CVGSRender* render)
{
	mVGSRender = render;
}

// 由一个图像文件，创建一个texture对象
CTexture* CTextureManager::createTexture(const std::string &sImageFile, BOOL bAlphaOnly/* = FALSE*/, INT mipMap/* = DEFAULT_MIPMAP*/, /*D3DFORMAT format = D3DFMT_UNKNOWN,*/ const std::string &sTextureName/* = ""*/)
{
	std::string sNewName = sTextureName;  // 新的贴图名称
	std::string sImageName = _GetPureFileNameStr(sImageFile);

	if (!sImageName.empty())
	{
		CResource *pRes = getResource(sImageName);
		if (pRes)
		{
			pRes->AddRef();
			return (CTexture*)pRes;
		}
	}
	else
	{
		return 0;
	} // if 

	// 创建texture对象
	CTexture *pTexture = new CTexture();
	if (pTexture)
	{
		pTexture->m_sName = sNewName;
		
		strcpy(pTexture->path, _GetPureFilePathStr(sImageFile).c_str());
		// strcat(pTexture->pathAndName, sImageFile.c_str());
		pTexture->m_sFilePathandName = sImageFile;
		
		_D3DXIMAGE_INFO imageInfo;
		D3DXGetImageInfoFromFile((sImageFile.c_str()), &imageInfo);
		
		pTexture->m_format = imageInfo.Format;
		
		//zip quality
		pTexture->ZipQuality = 55;
		
		char errInfo[1024];
		ZeroMemory(errInfo, 1024);
		//创建图像的Buffer,并从Buffer创建贴图
 		FILE* pTexFile = fopen(pTexture->m_sFilePathandName.c_str(), "rb");
 		if (pTexFile == NULL)
 		{
			strcpy(errInfo, "can not find texture file:");
			strcat(errInfo, pTexture->m_sFilePathandName.c_str());
			MessageBox(NULL, (errInfo), TEXT("read texture file error"), MB_OK | MB_ICONERROR);
			ZeroMemory(errInfo, sizeof(errInfo) );
			return 0;
 		}
 		else
 		{			
			CImage* pImage = new CImage();
			fseek(pTexFile, 0, SEEK_END);
			pImage->dataSize = ftell(pTexFile);
			fseek(pTexFile, 0, SEEK_SET);
			pImage->data = new BYTE[pImage->dataSize];
			fread(pImage->data, pImage->dataSize, 1, pTexFile);
			fclose(pTexFile);
			if (pImage->data == NULL)
			{
				strcpy(errInfo, "read texture file:");
				strcat(errInfo, pTexture->m_sFilePathandName.c_str());
				strcat(errInfo, "error");
				MessageBox(NULL, (errInfo), TEXT("read local texture file error"), MB_OK | MB_ICONERROR);
				ZeroMemory(errInfo, sizeof(errInfo) );
				return 0;
			}
 			
 			if (!bAlphaOnly) // 
 			{
				if ( FAILED(D3DXCreateTextureFromFileInMemoryEx(mVGSRender->m_pd3dDevice,
					pImage->data,
					pImage->dataSize,
					D3DX_DEFAULT,
					D3DX_DEFAULT,
					mipMap,
					0,
					pTexture->m_format,
					D3DPOOL_MANAGED,
					D3DX_DEFAULT,
					D3DX_DEFAULT,
					NULL,
					&pTexture->imageInfo,
					NULL,
					&pTexture->m_textureBuf) ) )
				return 0;
				pTexture->width  = pTexture->imageInfo.Width;
				pTexture->height = pTexture->imageInfo.Height;
				pTexture->MipCount = mipMap;
 			}
 			else 
 			{
				// 先创建一个A8R8G8B8的texture
				LPDIRECT3DTEXTURE9 pDTexture = NULL;
				D3DXIMAGE_INFO pSrcInfo;
				if (FAILED(D3DXCreateTextureFromFileInMemoryEx(mVGSRender->m_pd3dDevice,
					pImage->data,
					pImage->dataSize,
					D3DX_DEFAULT,
					D3DX_DEFAULT,
					mipMap,
					0,
					D3DFMT_A8R8G8B8,
					D3DPOOL_MANAGED,
					D3DX_DEFAULT,
					D3DX_DEFAULT,
					NULL,
					&pSrcInfo,
					NULL,
					&pDTexture) ) )
				return 0;

				// 用创建的A8R8G8B8的texture构造一个A8 texture
				pTexture->width  = pSrcInfo.Width;
				pTexture->height = pSrcInfo.Height;
				pTexture->m_format = D3DFMT_A8; // 透明
				pTexture->MipCount = mipMap;

				//创建目标贴图缓存
				D3DXCreateTexture(mVGSRender->m_pd3dDevice,
								pTexture->width, 
								pTexture->height, 
								mipMap, 
								0, 
								pTexture->m_format,
								D3DPOOL_MANAGED,
								&pTexture->m_textureBuf);

				UINT bufSize = pTexture->width * pTexture->height * 4;
				
				D3DLOCKED_RECT OriLockRect;
				RECT OriRect;
				OriRect.left   = OriRect.top = 0;
				OriRect.right  = pTexture->width;
				OriRect.bottom = pTexture->height;
				if (FAILED(pDTexture->LockRect(0, &OriLockRect, &OriRect, D3DLOCK_DISCARD)))
					return 0;
				BYTE* OriBuf = new BYTE[bufSize];
				BYTE* DesBuf = new BYTE[bufSize / 4];
				memcpy(OriBuf, (BYTE*)OriLockRect.pBits, bufSize);
				//unlock texture rect
				pDTexture->UnlockRect(0);
				bufSize = bufSize/4;

				BOOL bWithAlpha = _IsTransparentFormat(imageInfo.Format);
				//if (bWithAlpha)
				{
					// 构造A8贴图
					for (UINT i = 0; i < bufSize; i++)
					{
						BYTE alpha;
						if (bWithAlpha)
						{
							alpha = OriBuf[i * 4 + 3];
						}
						else
						{
							alpha = (UINT)(0.299 * OriBuf[i * 4 + 0] +0.587 * OriBuf[i * 4 + 1] +0.114 * OriBuf[i * 4 + 2]);
						}
						DesBuf[i] = alpha;
					} // for
				} // if
				SAFE_RELEASE_D3D(pDTexture);
				
				//重写该层材质的贴图缓存
				UINT totallevels = 0;
				if (pTexture->MipCount == 0)
					totallevels = pTexture->m_textureBuf->GetLevelCount();
				else
					totallevels = pTexture->MipCount;
				
				for (UINT miplevel = 0; miplevel< totallevels; miplevel++)
				{
					LPDIRECT3DSURFACE9 surflevel;
					pTexture->m_textureBuf->GetSurfaceLevel(miplevel,&surflevel);
					
					RECT SourceRect = {0, 0, pTexture->width, pTexture->height};
					D3DXLoadSurfaceFromMemory(surflevel, NULL, NULL,
						DesBuf, pTexture->m_format,
						pTexture->width,
						NULL, &SourceRect, D3DX_FILTER_LINEAR, 0);
					
					surflevel->Release();
				} // for
				
				//回收过渡缓冲内存
				SAFE_DELETE_ARRAY(OriBuf);
				SAFE_DELETE_ARRAY(DesBuf); 
				
				pTexture->bAlphaBlend = TRUE;

 			} // if bAlphaOnly			
 
 		} // if pTexFile
		
		// m_vTextureMap[pTexture->m_sName] = pTexture;  // 保存到map
		addResource(pTexture); // 保存resource


		pTexture->bCreated = TRUE;

	} // if pTexture


	return pTexture;
}

CTexture* CTextureManager::createEmptyTexture(const std::string &sName, D3DFORMAT format, INT width, INT height, INT mipmap)
{
	std::string sNewName = sName;  // 新的贴图名称
	if (!sNewName.empty())
	{
		CResource *pRes = getResource(sNewName);
		if (pRes)
		{
			pRes->AddRef();
			return (CTexture *)pRes;
		}
	}
	else
	{
		return 0;
	}
	// 创建texture对象
	CTexture *pTexture = new CTexture();
	if (pTexture)
	{
		pTexture->m_sName  = sNewName;
		pTexture->m_format       = format;
		pTexture->width    = width;
		pTexture->height   = height;
		pTexture->MipCount = mipmap;
		pTexture->imageIdx = -1;
		pTexture->m_textureBuf = NULL;
		pTexture->bCreated = FALSE;

		addResource(pTexture); // 保存resource
	}
	return pTexture;
}

BOOL CTextureManager::createTextureFromBuffer(CTexture *pTexture, char* buffer, DWORD len, BOOL bAlphaOnly/* = FALSE*/)
{
	if (!pTexture)
	{
		return FALSE;
	}
	if (!bAlphaOnly) // 
	{
		if ( FAILED(D3DXCreateTextureFromFileInMemoryEx(mVGSRender->m_pd3dDevice,
			buffer,
			len,
			pTexture->width,
			pTexture->height,
			pTexture->MipCount,
			0,
			pTexture->m_format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			NULL,
			NULL,
			NULL,
			&pTexture->m_textureBuf) ) 
		    )
		return 0;
	}
	else 
	{
		// 先创建一个A8R8G8B8的texture
		LPDIRECT3DTEXTURE9 pDTexture = NULL;
		if (FAILED(D3DXCreateTextureFromFileInMemoryEx(mVGSRender->m_pd3dDevice,
			buffer,
			len,
			pTexture->width,
			pTexture->height,
			pTexture->MipCount,
			0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			NULL,
			&pTexture->imageInfo,
			NULL,
			&pDTexture) ) )
			return 0;

		// 用创建的A8R8G8B8的texture构造一个A8 texture
		pTexture->m_format = D3DFMT_A8; // 透明

		//创建目标贴图缓存
		D3DXCreateTexture(mVGSRender->m_pd3dDevice,
			pTexture->width, 
			pTexture->height, 
			pTexture->MipCount, 
			0, 
			pTexture->m_format,
			D3DPOOL_MANAGED,
			&pTexture->m_textureBuf);

		UINT bufSize = pTexture->width * pTexture->height * 4;

		D3DLOCKED_RECT OriLockRect;
		RECT OriRect;
		OriRect.left   = OriRect.top = 0;
		OriRect.right  = pTexture->width;
		OriRect.bottom = pTexture->height;
		if (FAILED(pDTexture->LockRect(0, &OriLockRect, &OriRect, D3DLOCK_DISCARD)))
			return 0;
		BYTE* OriBuf = new BYTE[bufSize];
		BYTE* DesBuf = new BYTE[bufSize / 4];
		memcpy(OriBuf, (BYTE*)OriLockRect.pBits, bufSize);
		//unlock texture rect
		pDTexture->UnlockRect(0);
		bufSize = bufSize/4;

		BOOL bWithAlpha = _IsTransparentFormat(pTexture->imageInfo.Format);
		//if (bWithAlpha)
		{
			// 构造A8贴图
			for (UINT i = 0; i < bufSize; i++)
			{
				BYTE alpha;
				if (bWithAlpha)
				{
					alpha = OriBuf[i * 4 + 3];
				}
				else
				{
					alpha = (UINT)(0.299 * OriBuf[i * 4 + 0] +0.587 * OriBuf[i * 4 + 1] +0.114 * OriBuf[i * 4 + 2]);
				}
				DesBuf[i] = alpha;
			} // for
		} // if
		SAFE_RELEASE_D3D(pDTexture);

		//重写该层材质的贴图缓存
		UINT totallevels = 0;
		if (pTexture->MipCount == 0)
			totallevels = pTexture->m_textureBuf->GetLevelCount();
		else
			totallevels = pTexture->MipCount;

		for (UINT miplevel = 0; miplevel< totallevels; miplevel++)
		{
			LPDIRECT3DSURFACE9 surflevel;
			pTexture->m_textureBuf->GetSurfaceLevel(miplevel,&surflevel);

			RECT SourceRect = {0, 0, pTexture->width, pTexture->height};
			D3DXLoadSurfaceFromMemory(surflevel, NULL, NULL,
				DesBuf, pTexture->m_format,
				pTexture->width,
				NULL, &SourceRect, D3DX_FILTER_LINEAR, 0);

			surflevel->Release();
		} // for

		//回收过渡缓冲内存
		SAFE_DELETE_ARRAY(OriBuf);
		SAFE_DELETE_ARRAY(DesBuf); 

		pTexture->bAlphaBlend = TRUE;
	} // if bAlphaOnly	

	pTexture->bCreated = TRUE;  // 已经创建

	return TRUE;
}

// 由一个图像文件buffer，创建一个texture对象
// CResource* CTextureManager::createTextureFromBuffer(const std::string &sTextureName, const VOID* pBuffer, BOOL bAlphaOnly/* = FALSE*/, INT mipMap/* = DEFAULT_MIPMAP*/)
// {
// 	std::string sNewName = sTextureName;  // 新的贴图名称
// 
// 	if (!sNewName.empty())
// 	{
// 		CResource *pRes = getResource(sNewName);
// 		if (pRes)
// 		{
// 			return pRes;
// 		}
// 	}
// 	else
// 	{
// 		return 0;
// 	}
// 
// 	// 创建texture对象
// 	CTexture *pTexture = new CTexture();
// 	if (pTexture)
// 	{
// 		pTexture->m_sName = sNewName;
// 		
// 		_D3DXIMAGE_INFO imageInfo;
// 		D3DXGetImageInfoFromFile(sImageFile.c_str(), &imageInfo);
// 		
// 		pTexture->m_format = imageInfo.Format;
// 		
// 		//zip quality
// 		pTexture->ZipQuality = 55;
// 		
// 		char errInfo[1024];
// 		ZeroMemory(errInfo, 1024);
// 		//创建图像的Buffer,并从Buffer创建贴图
//  		FILE* pTexFile = fopen(pTexture->pathAndName, "rb");
//  		if (pTexFile == NULL)
//  		{
// 			strcpy(errInfo, "can not find texture file:");
// 			strcat(errInfo, pTexture->pathAndName);
// 			MessageBox(NULL, errInfo, "read texture file error", MB_OK | MB_ICONERROR);
// 			ZeroMemory(errInfo, sizeof(errInfo) );
// 			return 0;
//  		}
//  		else
//  		{			
// 			CImage* pImage = new CImage();
// 			fseek(pTexFile, 0, SEEK_END);
// 			pImage->dataSize = ftell(pTexFile);
// 			fseek(pTexFile, 0, SEEK_SET);
// 			pImage->data = new BYTE[pImage->dataSize];
// 			fread(pImage->data, pImage->dataSize, 1, pTexFile);
// 			fclose(pTexFile);
// 			if (pImage->data == NULL)
// 			{
// 				strcpy(errInfo, "read texture file:");
// 				strcat(errInfo, pTexture->pathAndName);
// 				strcat(errInfo, "error");
// 				MessageBox(NULL, errInfo, "read local texture file error", MB_OK | MB_ICONERROR);
// 				ZeroMemory(errInfo, sizeof(errInfo) );
// 				return 0;
// 			}
//  			
//  			if (!bAlphaOnly) // 
//  			{
// 				if ( FAILED(D3DXCreateTextureFromFileInMemoryEx(mVGSRender->m_pd3dDevice,
// 					pImage->data,
// 					pImage->dataSize,
// 					D3DX_DEFAULT,
// 					D3DX_DEFAULT,
// 					mipMap,
// 					0,
// 					pTexture->m_format,
// 					D3DPOOL_MANAGED,
// 					D3DX_DEFAULT,
// 					D3DX_DEFAULT,
// 					NULL,
// 					&pTexture->imageInfo,
// 					NULL,
// 					&pTexture->textureBuf) ) )
// 				return 0;
//  			}
//  			else 
//  			{
// 				// 先创建一个A8R8G8B8的texture
// 				LPDIRECT3DTEXTURE9 pDTexture = NULL;
// 				D3DXIMAGE_INFO pSrcInfo;
// 				if (FAILED(D3DXCreateTextureFromFileInMemoryEx(mVGSRender->m_pd3dDevice,
// 					pImage->data,
// 					pImage->dataSize,
// 					D3DX_DEFAULT,
// 					D3DX_DEFAULT,
// 					mipMap,
// 					0,
// 					D3DFMT_A8R8G8B8,
// 					D3DPOOL_MANAGED,
// 					D3DX_DEFAULT,
// 					D3DX_DEFAULT,
// 					NULL,
// 					&pSrcInfo,
// 					NULL,
// 					&pDTexture) ) )
// 				return 0;
// 
// 				// 用创建的A8R8G8B8的texture构造一个A8 texture
// 				pTexture->width  = pSrcInfo.Width;
// 				pTexture->height = pSrcInfo.Height;
// 				pTexture->m_format = D3DFMT_A8; // 透明
// 				pTexture->MipCount = mipMap;
// 
// 				//创建目标贴图缓存
// 				D3DXCreateTexture(mVGSRender->m_pd3dDevice,
// 								pTexture->width, 
// 								pTexture->height, 
// 								mipMap, 
// 								0, 
// 								pTexture->m_format,
// 								D3DPOOL_MANAGED,
// 								&pTexture->textureBuf);
// 
// 				UINT bufSize = pTexture->width * pTexture->height * 4;
// 				
// 				D3DLOCKED_RECT OriLockRect;
// 				RECT OriRect;
// 				OriRect.left   = OriRect.top = 0;
// 				OriRect.right  = pTexture->width;
// 				OriRect.bottom = pTexture->height;
// 				if (FAILED(pDTexture->LockRect(0, &OriLockRect, &OriRect, D3DLOCK_DISCARD)))
// 					return 0;
// 				BYTE* OriBuf = new BYTE[bufSize];
// 				BYTE* DesBuf = new BYTE[bufSize / 4];
// 				memcpy(OriBuf, (BYTE*)OriLockRect.pBits, bufSize);
// 				//unlock texture rect
// 				pDTexture->UnlockRect(0);
// 				bufSize = bufSize/4;
// 
// 				BOOL bWithAlpha = _IsTransparentFormat(imageInfo.Format);
// 				//if (bWithAlpha)
// 				{
// 					// 构造A8贴图
// 					for (UINT i = 0; i < bufSize; i++)
// 					{
// 						BYTE alpha;
// 						if (bWithAlpha)
// 						{
// 							alpha = OriBuf[i * 4 + 3];
// 						}
// 						else
// 						{
// 							alpha = (UINT)(0.299 * OriBuf[i * 4 + 0] +0.587 * OriBuf[i * 4 + 1] +0.114 * OriBuf[i * 4 + 2]);
// 						}
// 						DesBuf[i] = alpha;
// 					} // for
// 				} // if
// 				SAFE_RELEASE_D3D(pDTexture);
// 				
// 				//重写该层材质的贴图缓存
// 				UINT totallevels = 0;
// 				if (pTexture->MipCount == 0)
// 					totallevels = pTexture->textureBuf->GetLevelCount();
// 				else
// 					totallevels = pTexture->MipCount;
// 				
// 				for (UINT miplevel = 0; miplevel< totallevels; miplevel++)
// 				{
// 					LPDIRECT3DSURFACE9 surflevel;
// 					pTexture->textureBuf->GetSurfaceLevel(miplevel,&surflevel);
// 					
// 					RECT SourceRect = {0, 0, pTexture->width, pTexture->height};
// 					D3DXLoadSurfaceFromMemory(surflevel, NULL, NULL,
// 						DesBuf, pTexture->m_format,
// 						pTexture->width,
// 						NULL, &SourceRect, D3DX_FILTER_LINEAR, 0);
// 					
// 					surflevel->Release();
// 				} // for
// 				
// 				//回收过渡缓冲内存
// 				SAFE_DELETE_ARRAY(OriBuf);
// 				SAFE_DELETE_ARRAY(DesBuf); 
// 				
// 				pTexture->bAlphaBlend = TRUE;
// 
//  			} // if bAlphaOnly			
//  
//  		} // if pTexFile
// 		
// 		// m_vTextureMap[pTexture->m_sName] = pTexture;  // 保存到map
// 		addResource(pTexture); // 保存resource
// 
// 	} // if pTexture
// 
// 
// 	return pTexture;
// 
// }

bool CTextureManager::IsTextureExist(const std::string &sName)
{
	return IsResourceExist(sName);
}
CDynamicTex* CTextureManager::CreateDynamicTexture(const std::string& strName, const std::vector<std::string>& vecFileName, float fDuration)
{
	//新的贴图名称
	std::string sNewName = strName;  
	if (!sNewName.empty())
	{
		CResource *pRes = getResource(sNewName);
		if (pRes)
		{
			pRes->AddRef();
			return (CDynamicTex*)pRes;
		}
	}
	else
	{
		// 避免重名
		int index = 0;
		char tmp[10];
		std::string strFileName = _GetPureFileNameStr(vecFileName[0]);
		sNewName = strFileName;
		while (IsResourceExist(sNewName))
		{
			sNewName = strFileName;
			sNewName.append(std::string(itoa(index++, tmp, 10)));
		}
	}

	D3DXIMAGE_INFO imgInfo;
	memset(&imgInfo, 0, sizeof(imgInfo));
	D3DXGetImageInfoFromFile((vecFileName[0].c_str()), &imgInfo);

	CDynamicTex* pDynamicMap = new CDynamicTex(mVGSRender->m_pd3dDevice,imgInfo.Width,imgInfo.Height, imgInfo.Format,  vecFileName.size() );
	if(pDynamicMap)
	{
		pDynamicMap->m_sName = sNewName;
		for (size_t i = 0; i < vecFileName.size(); i++)
		{
			pDynamicMap->AddImage(vecFileName[i]);
			pDynamicMap->SetDuration(fDuration);
		}
		addResource(pDynamicMap);
		pDynamicMap->bCreated = TRUE;
		return pDynamicMap;
	}
	return NULL;
}
CCubeMap* CTextureManager::CreateStaticCubeTexture(const std::string& strName, const vector<std::string> &vecFileName, int nSize)
{
	//新的贴图名称
	std::string sNewName = strName;  
	if (!sNewName.empty())
	{
		CResource *pRes = getResource(sNewName);
		if (pRes)
		{
			pRes->AddRef();
			return (CCubeMap*)pRes;
		}
	}
	else
	{
		// 避免重名
		int index = 0;
		char tmp[10];
		std::string strFileName = _GetPureFileNameStr(vecFileName[0]);
		sNewName = strFileName;
		while (IsResourceExist(sNewName))
		{
			sNewName = strFileName;
			sNewName.append(std::string(itoa(index++, tmp, 10)));
		}
	}
	CCubeMap* pCubeMap = new CCubeMap(mVGSRender->m_pd3dDevice, 0, D3DFMT_R5G6B5, nSize, 1);
	if(pCubeMap)
	{
		pCubeMap->m_sName = strName;
		pCubeMap->FillImagesToTexture(vecFileName);

		addResource(pCubeMap);
		pCubeMap->bCreated = TRUE;
		return pCubeMap;
	}
	return NULL;
}
CCubeMap* CTextureManager::CreateDynamicCubeTexture(const std::string& strName, int nSize)
{
	//新的贴图名称
	std::string sNewName = strName;  
	if (!sNewName.empty())
	{
		CResource *pRes = getResource(sNewName);
		if (pRes)
		{
			pRes->AddRef();
			return (CCubeMap*)pRes;
		}
	}
	else
	{
		// 避免重名
		int index = 0;
		char tmp[10];
		while (IsResourceExist(sNewName))
		{
			sNewName.append(std::string(itoa(index++, tmp, 10)));
		}
	}
	CCubeMap* pCubeMap = new CCubeMap(mVGSRender->m_pd3dDevice, 1, D3DFMT_R5G6B5, nSize, 1);
	if(pCubeMap)
	{
		pCubeMap->m_sName = strName;

		addResource(pCubeMap);
		pCubeMap->bCreated = TRUE;
		return pCubeMap;
	}
	return NULL;
}

CBumpMap* CTextureManager::CreateBumpTexture(const std::string& strName)
{
	//新的贴图名称
	std::string sNewName = strName;  
	if (!sNewName.empty())
	{
		CResource *pRes = getResource(sNewName);
		if (pRes)
		{
			pRes->AddRef();
			return (CBumpMap*)pRes;
		}
	}
	else
	{
		// 避免重名
		int index = 0;
		char tmp[256];
		while (IsResourceExist(sNewName))
		{
			sNewName.append(std::string(itoa(index++, tmp, 10)));
		}
	}
	CBumpMap* pBump = new CBumpMap(mVGSRender->m_pd3dDevice, &mVGSRender->m_d3dpp, mVGSRender->d3dParam.SupportBestBumpFormat);
	if(pBump)
	{
		pBump->m_sName = strName;

		addResource(pBump);
		pBump->bCreated = TRUE;
		return pBump;
	}
	return NULL;
}
CRTFMap* CTextureManager::CreateRTreflectTexture(const std::string& strName,int nWidth, int nHeight)
{
	std::string sNewName = strName;  
	if (!sNewName.empty())
	{
		CResource *pRes = getResource(sNewName);
		if (pRes)
		{
			pRes->AddRef();
			return (CRTFMap*)pRes;
		}
	}
	else
	{
		// 避免重名
		int index = 0;
		char tmp[256];
		while (IsResourceExist(sNewName))
		{
			sNewName.append(std::string(itoa(index++, tmp, 10)));
		}
	}
	CRTFMap* pMap = new CRTFMap(strName,mVGSRender->m_pd3dDevice, &mVGSRender->m_d3dpp,nWidth, nHeight, D3DFMT_R5G6B5/*D3DFMT_A8R8G8B8*/);
	if(pMap)
	{
		pMap->m_sName = strName;

		addResource(pMap);
		pMap->bCreated = TRUE;
		return pMap;
	}
	return NULL;
}


SwfTexture* CTextureManager::CreateFlashTexture(const std::string& strName, const std::string& strFlashPath,
							   int flashWidth, int flashHeight, int nWidth, int nHeight)
{
	//新的贴图名称
	std::string sNewName = strName;  
	if (!sNewName.empty())
	{
		CResource *pRes = getResource(sNewName);
		if (pRes)
		{
			pRes->AddRef();
			return (SwfTexture*)pRes;
		}
	}
	else
	{
		// 避免重名
		int index = 0;
		char tmp[10];
		std::string strFileName = _GetPureFileNameStr(strFlashPath);
		sNewName = strFileName;
		while (IsResourceExist(sNewName))
		{
			sNewName = strFileName;
			sNewName.append(std::string(itoa(index++, tmp, 10)));
		}
	}
	SwfTexture* pSwfTex = new SwfTexture(sNewName,
		mVGSRender->m_hWnd, 
		mVGSRender->m_pd3dDevice,
		/*m_spClientSite,*/ 
		strFlashPath,  
		flashWidth, flashHeight,
		nWidth, nHeight);
	if (pSwfTex)
	{
		addResource(pSwfTex);
		pSwfTex->bCreated = TRUE;
		return pSwfTex;
	}
	return NULL;
}


