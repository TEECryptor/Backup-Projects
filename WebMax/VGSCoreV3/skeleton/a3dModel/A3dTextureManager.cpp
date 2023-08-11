#include "A3dTextureManager.h"
struct D3DX_IMAGE_LOAD_INFO 
{  
    UINT Width;
    UINT Height;
    UINT Depth;
    UINT MipLevels;
    DWORD Usage;
    D3DFORMAT Format;
    D3DPOOL Pool;
    DWORD Filter;
    DWORD MipFilter;
    D3DCOLOR ColorKey;

};

bool fillLoadInfo(D3DX_IMAGE_LOAD_INFO& loadInfo )
{
    ZeroMemory( &loadInfo, sizeof(D3DX_IMAGE_LOAD_INFO) );
    loadInfo.Width     = D3DX_DEFAULT;
    loadInfo.Height    = D3DX_DEFAULT;
    loadInfo.Depth     = D3DX_DEFAULT;
    loadInfo.MipLevels = D3DX_DEFAULT;
    loadInfo.Usage     = 0;//;
    loadInfo.Format    = D3DFMT_FROM_FILE;;
    loadInfo.Pool      = D3DPOOL_MANAGED;
    loadInfo.Filter    = D3DX_DEFAULT ;
    loadInfo.MipFilter = D3DX_DEFAULT;
    loadInfo.ColorKey  = 0xFF000000;
    return true;
}

bool prepareLoadInfo(D3DXIMAGE_INFO& imgInfo , D3DX_IMAGE_LOAD_INFO& loadInfo)
{
    fillLoadInfo(loadInfo);
    loadInfo.Width     = imgInfo.Width;
    loadInfo.Height    = imgInfo.Height;
    loadInfo.Depth     = imgInfo.Depth;
    loadInfo.MipLevels = imgInfo.MipLevels;
    loadInfo.Format    = imgInfo.Format;
	if(loadInfo.Format == D3DFMT_R8G8B8 )
		loadInfo.Format = D3DFMT_A8R8G8B8;
    return true;
}

CTextureName::CTextureName()
{
	m_Name       = L"";
}

CTextureName::CTextureName(const wchar_t* fileName)
{
	m_Name       = fileName;
}

CBaseTextureLoader::CBaseTextureLoader()
{
    m_DeviceWrapper = NULL;
}

bool  CBaseTextureLoader::_isResLoaded(xA3DTexture* pRes)
{
    return pRes != NULL;
}

HBaseTexture CBaseTextureLoader::load(const CTextureName& texName , bool bLoadImm  , bool arg)
{
     CBaseTextureMgr* pMgr = dynamic_cast<CBaseTextureMgr*>(this);
     return pMgr->add(texName, arg ,bLoadImm);
}

inline bool XEvol_IsAbsPath(const wchar_t* strResName)
{
    //Case /usr/hello.jpg (UNIX)
    if(strResName[0] == '/')
        return true;

    if( wcslen(strResName) <3)
        return false;

    //Case C:\\Hello.jpg
    if(strResName[1] == ':' &&(strResName[2] == '\\' || strResName[2] == '/') )
        return true;
    //Case \\192.168.0.1\ddd.jpg
    if(strResName[0] == '\\' && strResName[1] == '\\'   )
        return true;
    return false;
}
/*
//创建透明贴图D3DFMT_A8，用于透明混色
// 参数1 pTex in vgstexture指针，保存创建结果
// 参数2 dataSize 图像文件buffer长度
// 参数3 pData    图像文件buffer，是一个图像文件的二进制数据流
IDirect3DTexture9* CreateTransparentTexture(IDirect3DDevice9* pDevice ,   void* pData , DWORD dataSize , const wchar_t* _TransparentTexture)
{
    //获得图像信息
    D3DXIMAGE_INFO d3dImageInfo;
    D3DXGetImageInfoFromFileInMemory(pData, dataSize, &d3dImageInfo);
    IDirect3DTexture9* p2DTexture = NULL;
    //判断贴图的格式，如果是A8格式的透明贴图
    HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(pDevice,   pData,   dataSize,
        d3dImageInfo.Width,
        d3dImageInfo.Height,
        d3dImageInfo.MipLevels,
        0,
        d3dImageInfo.Format,
        D3DPOOL_MANAGED,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        NULL,
        &d3dImageInfo,
        NULL,
        &p2DTexture);

    if(p2DTexture == NULL)
        return NULL;

    if (d3dImageInfo.Format == D3DFMT_A8)
    {
        return hr;
    }

    //判断贴图的格式，如果是32位或16位的格式的不透明贴图
    if (d3dImageInfo.Format == D3DFMT_X8R8G8B8 || d3dImageInfo.Format == D3DFMT_R8G8B8 || d3dImageInfo.Format == D3DFMT_R5G6B5 || d3dImageInfo.Format == D3DFMT_L8)
    {
        //从临时贴图对象中取数据，并拷贝到新的贴图对象
        UINT bufSize   = pTex->width * pTex->height * 4;
        D3DLOCKED_RECT OriLockRect;
        RECT OriRect;
        OriRect.left   = OriRect.top = 0;
        OriRect.right  = pTex->width;
        OriRect.bottom = pTex->height;
        if (FAILED(pDTexture->LockRect(0, &OriLockRect, &OriRect,  D3DLOCK_DISCARD)))
            return E_FAIL;
        BYTE* DesBuf = new BYTE[bufSize/4];
        BYTE* OriBuf = (BYTE*)OriLockRect.pBits;
        for(UINT i =0 ;i< bufSize/4; i++)
            DesBuf[i] = (UINT)(0.299 * OriBuf[i*4+0] +0.587 * OriBuf[i*4+1] +0.114 * OriBuf[i*4+2]);
        pDTexture->UnlockRect(0);
        SAFE_RELEASE_D3D(pDTexture);

        //重写该层材质的贴图缓存
        UINT totallevels = 0;
        if (pTex->mipCount == 0)
            totallevels = pTex->textureBuf->GetLevelCount();
        else
            totallevels = pTex->mipCount;

        for (UINT miplevel = 0; miplevel< totallevels; miplevel++)
        {
            LPDIRECT3DSURFACE9 surflevel;
            pTex->textureBuf->GetSurfaceLevel(miplevel,&surflevel);

            RECT SourceRect = {0, 0, pTex->width, pTex->height};
            D3DXLoadSurfaceFromMemory(surflevel, NULL, NULL,
                DesBuf, pTex->BP,
                pTex->width,
                NULL, &SourceRect, D3DX_FILTER_LINEAR, 0);
            SAFE_RELEASE_D3D(surflevel);
        }

        //回收过渡缓冲内存
        SAFE_DELETE_ARRAY(DesBuf);  
        pTex->bCreated = TRUE;
        return S_OK;
    }
    //判断贴图的格式，如果是32位或16位的格式的透明贴图
    if (pTex->BP == D3DFMT_A8R8G8B8)
    {
        //先创建A8R8G8B8格式的贴图到临时贴图对象
        LPDIRECT3DTEXTURE9 pDTexture;
        if (FAILED(D3DXCreateTextureFromFileInMemoryEx(m_pd3dDevice,
            pData,
            dataSize,
            pTex->width,
            pTex->height,
            pTex->mipCount,
            0,
            D3DFMT_A8R8G8B8,
            D3DPOOL_MANAGED,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            NULL,
            &pTex->imageInfo,
            NULL,
            &pDTexture) ) )
            return E_FAIL;

        //再转化为A8格式的透明贴图
        pTex->BP = D3DFMT_A8;
        if (FAILED(CreateEmptyTexture(pTex, 0, D3DPOOL_MANAGED)))
        {
            SAFE_RELEASE_D3D(pDTexture);
            return E_FAIL;
        }

        //从临时贴图对象中取数据，并拷贝到新的贴图对象
        UINT bufSize = pTex->width * pTex->height * 4;
        D3DLOCKED_RECT OriLockRect;
        RECT OriRect;
        OriRect.left = OriRect.top = 0;
        OriRect.right = pTex->width;
        OriRect.bottom = pTex->height;
        if (FAILED(pDTexture->LockRect(0, &OriLockRect, &OriRect, D3DLOCK_DISCARD)))
            return E_FAIL;
        BYTE* DesBuf = new BYTE[bufSize/4];
        BYTE* OriBuf = (BYTE*)OriLockRect.pBits;
        for(UINT i =0 ;i< bufSize/4; i++)
            DesBuf[i] = OriBuf[i*4 + 3];
        pDTexture->UnlockRect(0);
        SAFE_RELEASE_D3D(pDTexture);

        //重写该层材质的贴图缓存
        UINT totallevels = 0;
        if (pTex->mipCount == 0)
            totallevels = pTex->textureBuf->GetLevelCount();
        else
            totallevels = pTex->mipCount;

        for (UINT miplevel = 0; miplevel< totallevels; miplevel++)
        {
            LPDIRECT3DSURFACE9 surflevel;
            pTex->textureBuf->GetSurfaceLevel(miplevel,&surflevel);

            RECT SourceRect = {0, 0, pTex->width, pTex->height};
            D3DXLoadSurfaceFromMemory(surflevel, NULL, NULL,
                DesBuf, pTex->BP,
                pTex->width,
                NULL, &SourceRect, D3DX_FILTER_LINEAR, 0);
            SAFE_RELEASE_D3D(surflevel);
        }

        //回收过渡缓冲内存
        SAFE_DELETE_ARRAY(DesBuf);  
        pTex->bCreated = TRUE;
        return S_OK;	
    }

    return E_FAIL;
}
*/

xA3DTexture* CBaseTextureLoader::_loadTexture( const CTextureName& _texName , const wchar_t* fileName ,BYTE* buf , int bufLen , unsigned int arg)
{
    HRESULT hr = E_FAIL;
    D3DXIMAGE_INFO      imgInfo;
    hr = D3DXGetImageInfoFromFileInMemory(buf , (UINT)bufLen ,&imgInfo);
    if(! FAILED(hr) )
    {
        D3DX_IMAGE_LOAD_INFO loadInfo;
        if(prepareLoadInfo(imgInfo , loadInfo) == false)
            return NULL;

        if(_texName.m_Format != D3DFMT_UNKNOWN) 
            loadInfo.Format = _texName.m_Format;

        //if(imgInfo.ResourceType == D3DRTYPE_TEXTURE)
        //{
            IDirect3DTexture9* p2DTexture = NULL;
            if(loadInfo.Format == D3DFMT_A8 && (imgInfo.Format == D3DFMT_L8 || imgInfo.Format == D3DFMT_X8R8G8B8 || imgInfo.Format == D3DFMT_R8G8B8 ) )
            {
                IDirect3DTexture9* pSysTexture = NULL;
                D3DXCreateTextureFromFileInMemoryEx( m_DeviceWrapper->m_pDevice , buf , bufLen , loadInfo.Width , loadInfo.Height , loadInfo.MipLevels , loadInfo.Usage , D3DFMT_L8 , D3DPOOL_SYSTEMMEM , loadInfo.Filter , loadInfo.MipFilter , loadInfo.ColorKey ,  &imgInfo , NULL , &pSysTexture );

                if(pSysTexture == NULL)
                    return NULL;

                m_DeviceWrapper->m_pDevice->CreateTexture(loadInfo.Width , loadInfo.Height ,  1 , 0 , D3DFMT_A8 , loadInfo.Pool , &p2DTexture , NULL);

                if(p2DTexture == NULL)
                {
                    XSAFE_RELEASE(pSysTexture);
                    return NULL;
                }

                IDirect3DSurface9* pSysSurf = NULL;
                IDirect3DSurface9* pTexSurf = NULL;
                pSysTexture->GetSurfaceLevel(0 , &pSysSurf);
                p2DTexture->GetSurfaceLevel(0 , &pTexSurf);
       
                D3DLOCKED_RECT _srcRect;
                pSysSurf->LockRect(&_srcRect , NULL , D3DLOCK_NOOVERWRITE);
                D3DLOCKED_RECT _dstRect;
                pTexSurf->LockRect(&_dstRect , NULL , D3DLOCK_DISCARD    );

                memcpy(_dstRect.pBits , _srcRect.pBits , _srcRect.Pitch * loadInfo.Height);
                pTexSurf->UnlockRect();
                pSysSurf->UnlockRect();


                //hr = D3DXLoadSurfaceFromSurface(pTexSurf , NULL , NULL , pSysSurf , NULL , NULL , D3DX_FILTER_NONE , 0);
                
                XSAFE_RELEASE(pSysSurf);
                XSAFE_RELEASE(pTexSurf);
                XSAFE_RELEASE(pSysTexture);

                if(p2DTexture)
                {
                    D3DSURFACE_DESC _desc;
                    p2DTexture->GetLevelDesc(0 , &_desc);
                    xA3DTexture* pA3DTexture = new xA3DTexture;
                    pA3DTexture->m_FileFormat = imgInfo.Format;
                    pA3DTexture->m_pTexture   = p2DTexture;
                    return pA3DTexture;
                }
                return NULL;

            }
            else
            {
                D3DXCreateTextureFromFileInMemoryEx( m_DeviceWrapper->m_pDevice , buf , bufLen , loadInfo.Width , loadInfo.Height , loadInfo.MipLevels , loadInfo.Usage , loadInfo.Format , loadInfo.Pool , loadInfo.Filter , loadInfo.MipFilter , loadInfo.ColorKey ,  &imgInfo , NULL , &p2DTexture );
                if(p2DTexture)
                {
                    D3DSURFACE_DESC _desc;
                    p2DTexture->GetLevelDesc(0 , &_desc);
                    xA3DTexture* pA3DTexture = new xA3DTexture;
                    pA3DTexture->m_FileFormat = imgInfo.Format;
                    pA3DTexture->m_pTexture   = p2DTexture;
                    return pA3DTexture;
                }
                return NULL;
            }


        //}
        //else if(imgInfo.ResourceType == D3DRTYPE_CUBETEXTURE)
        //{
        //    IDirect3DCubeTexture9* pCubeTexture = NULL;
        //    D3DXCreateCubeTextureFromFileInMemoryEx( m_pDevice , buf , bufLen , loadInfo.Width , loadInfo.MipLevels , loadInfo.Usage , loadInfo.Format , loadInfo.Pool , loadInfo.Filter , loadInfo.MipFilter , loadInfo.ColorKey ,  &imgInfo , NULL , &pCubeTexture);
        //    m_pTexture = pCubeTexture;
        //}
        //else if(imgInfo.ResourceType == D3DRTYPE_VOLUMETEXTURE)
        //{
        //    IDirect3DVolumeTexture9* pVolumeTexture = NULL;
        //    D3DXCreateVolumeTextureFromFileInMemoryEx( m_pDevice , buf , bufLen , loadInfo.Width , loadInfo.Height , loadInfo.Depth , loadInfo.MipLevels , loadInfo.Usage , loadInfo.Format , loadInfo.Pool , loadInfo.Filter , loadInfo.MipFilter , loadInfo.ColorKey ,  &imgInfo , NULL , &pVolumeTexture );
        //    m_pTexture = pVolumeTexture;
        //}

    }

    return NULL;
}

bool  CBaseTextureLoader::_loadResource(const CTextureName& _texName, xA3DTexture*& pTexture , int& ResSize , unsigned int arg)
{
	const wchar_t* texName = _texName.m_Name.c_str(); 
	std::wstring full_name;
	ResSize = 0;

	if(XEvol_IsAbsPath( texName  ) )
	{
		pTexture = _loadTexture(_texName , texName,NULL,0,arg);
		if( pTexture != NULL)
		{
			ResSize = 1;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		int nResItem = (int)this->m_Packages.size();
		for(int i = 0 ; i < nResItem ; i++)
		{
			xResPkgPathItem& item = this->m_Packages[i];
			if(item.m_Package.failed())
			{
                full_name = item.m_ResDir + std::wstring(texName)  ;
                FILE* fp = _wfopen(full_name.c_str() , L"rb");
                if(fp != NULL)
                {
                    fseek(fp , 0 , SEEK_END);
                    int len = ftell(fp);
                    fseek(fp , 0 , SEEK_SET);
                    BYTE* fBuf = new BYTE[len];
                    fread(fBuf , 1 , len , fp);
                    fclose(fp);
                    pTexture = _loadTexture(_texName , full_name.c_str() , fBuf , len  , arg);
                    delete [] fBuf;

                    if( pTexture != NULL )
                    {
                        ResSize = 1;
                        return true;
                    }
                }

			}
			else
			{
				full_name = item.m_ResDir +  texName ;
				xcomdocstream* pstream = item.m_Package.open_stream( (const wchar_t*)full_name.c_str());
				if(pstream)
				{
					int len = (int)pstream->data_len();
					BYTE* buf = new BYTE[len + 1];

					//读到内存里来
					int offs = pstream->stream_tellr();
					pstream->stream_seekr(xcdsd_beg,0);
					pstream->read(buf,len);
					pstream->stream_seekr(xcdsd_beg,offs);
					buf[len] = 0;

					pTexture = _loadTexture(_texName , texName, buf , len  , arg);
					if( pTexture != NULL)
					{
						ResSize = 1;
						delete[] buf;
						pstream->Release();
						return true;
					}
					else
					{
						delete[]  buf;
					}
					pstream->Release();
				}

			}
		}
	}

	return false;
}


bool  CBaseTextureLoader::_unloadResource(const CTextureName& strResName , xA3DTexture* & pRes , unsigned int& TotalResSize)
{
	int ResSize = 1;
	if(ResSize < 0) ResSize = 0;
	TotalResSize = ResSize;
    XSAFE_DELETE(pRes);
	return false;
}

void  CBaseTextureLoader::_deleteResource(const CTextureName& strResName , xA3DTexture* pRes)
{
	XSAFE_DELETE(pRes);
	return ;
}

unsigned int  CBaseTextureLoader::_getResSize(xA3DTexture* pRes)
{
	if(pRes == NULL)
		return 0;
	return 1;
}


