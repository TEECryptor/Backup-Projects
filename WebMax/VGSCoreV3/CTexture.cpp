#include "CTexture.h"
#include "VGSSupport.h"
#include "VGSRender.h"
#include "Root.h"

//##ModelId=47EBC72E03E8
CTexture::CTexture()
	:m_textureType(TLT_BASAL)
{
	index = -1;					//��ͼ����Ⱦ�����еı��
	//ZeroMemory(name, sizeof(name));
	//ZeroMemory(path, sizeof(path));	
	// ZeroMemory(pathAndName, sizeof(pathAndName));		
	m_sFilePathandName = "";
	width = 0;					//��ͼ���
	height = 0;					//��ͼ�߶�
	m_format = D3DFMT_R8G8B8;	
	ZipQuality = 100;
	withAlpha = FALSE;				//����Alphaͨ��
	isOpacity = FALSE;
	isDynamic = FALSE;

	bCreated = FALSE;				//��ͼ�Ƿ��Ѿ�����
	bUseByScene = TRUE;				//�Ƿ񱻳�������

	MipCount = 3;
	
	brightness = 0;	
	contrast = 0;	

	bAlphaTest = FALSE;	
	bAlphaBlend = FALSE;
	buffer = NULL;	//��������
	m_textureBuf = NULL;

	bIsOverlayTexture = FALSE;
}

//##ModelId=47EBC72E03E9
CTexture::~CTexture()
{
	Release();
}

//-----------------------------------------------------------------------------
// ������: SetOpacity()
// ��  ��: INT opacity
// ��  ;: ������ͼ��alphaֵ
//-----------------------------------------------------------------------------
VOID CTexture::SetOpacity(INT opacity)
{
	UINT bp_dif,bufSize;
	if (opacity > 255) opacity = 255;
	if (opacity < 0) opacity = 0;
	BYTE alpha = (BYTE)(opacity/100.0f * 255);

	switch(m_format)
	{
	case D3DFMT_A8R8G8B8:
		bp_dif = 4;
		break;
	case D3DFMT_A4R4G4B4:
		bp_dif = 2;
		break;
	default:
		return;
	}

	bufSize = width * height * bp_dif;
	D3DLOCKED_RECT DifLockRect;
	RECT DifRect;
	DifRect.left = DifRect.top = 0;
	DifRect.right = width;
	DifRect.bottom = height;
	if (FAILED((m_textureBuf)->LockRect(0, &DifLockRect, &DifRect, D3DLOCK_DISCARD)))
		return;
	
	buffer = new BYTE[bufSize];
	memcpy(buffer, (BYTE*)DifLockRect.pBits, bufSize);
	//unlock texture rect
	m_textureBuf->UnlockRect(0);

	//A8R8G8B8
	if (m_format == D3DFMT_A8R8G8B8 )
	{	
		for (int m =0; m< DifRect.bottom; m++)
		{
			for (int n = 0; n< DifRect.right; n++)
			{
				INT index = 4*(m*DifRect.right + n);
				//set pixel alpha
				buffer[index + 3] = alpha;
			}
		}
	}
	//A4R4G4B4
	else
	{
		for (int m =0; m< DifRect.bottom; m++)
		{
			for (int n = 0; n< DifRect.right; n++)
			{
				INT index = 2*(m*DifRect.right + n);
				buffer[index + 1] = (alpha & 0xf0) | (buffer[index + 1] & 0x0f); 
			}
		}
	}

	//���ڴ�����mipmap����������LockRectҪ��ÿ��Level�ı������������䣬������ֶ��ķ��������޷������
	//���ã�D3D�ṩ��D3DXLoadSurfaceFromMemory�ķ������Խ���������
	UINT totallevels = m_textureBuf->GetLevelCount();
	for (UINT miplevel = 0; miplevel< totallevels; miplevel++)
	{
		LPDIRECT3DSURFACE9 surflevel;
		m_textureBuf->GetSurfaceLevel(miplevel,&surflevel);
		
		RECT SourceRect = {0,0,width, height};
		D3DXLoadSurfaceFromMemory(surflevel, NULL, NULL,
			buffer, m_format,
			width * bp_dif,
			NULL, &SourceRect, D3DX_FILTER_LINEAR, 0);
		
		surflevel->Release();
	}

	SAFE_DELETE_ARRAY(buffer);
}

//-----------------------------------------------------------------------------
// ������: Release()
// ��  ��: 
// ��  ;: �ͷ���ͼ��Դ
//-----------------------------------------------------------------------------
VOID CTexture::Release()
{
	index = -1;											//��ͼ����Ⱦ�����еı��
	//ZeroMemory(path, sizeof(path));	
	// ZeroMemory(pathAndName, sizeof(pathAndName));	
	m_sFilePathandName = "";
	width = 0;											//��ͼ���
	height = 0;											//��ͼ�߶�
	m_format = D3DFMT_R5G6B5;									//ÿ��������ռ���ֽ���
	ZipQuality = 0;										//����ѹ��ΪN3Dʱ��ѹ������0-100
	withAlpha = FALSE;									//����Alphaͨ��
    isOpacity = FALSE;								    //�Ƿ���͸����ͼ
    isDynamic = FALSE;                                  //�Ƿ��Ƕ�̬��ͼ
	bCreated = FALSE;									//��ͼ�Ƿ��Ѿ�����
	brightness = 0;										//����
	
	SAFE_RELEASE_D3D(m_textureBuf);
	SAFE_DELETE_ARRAY(buffer);
}

//ֻ�ͷ�D3D��ͼ����
VOID CTexture::ReleaseBufferOnly()
{
	index = -1;				
//	ZeroMemory(name, sizeof(name));
//	ZeroMemory(path, sizeof(path));	
	// ZeroMemory(pathAndName, sizeof(pathAndName));
	m_sFilePathandName = "";
	width = 0;									
	height = 0;									
	m_format = D3DFMT_R5G6B5;							
	ZipQuality = 0;								
	withAlpha = FALSE;	
	isOpacity = FALSE;								    //�Ƿ���͸����ͼ
    isDynamic = FALSE;                                  //�Ƿ��Ƕ�̬��ͼ							
					
	bCreated = FALSE;						

	brightness = 0;								
	

	SAFE_RELEASE_D3D(m_textureBuf);
	SAFE_DELETE_ARRAY(buffer);
}

std::string CTexture::getTextrueFileName(int index/* = 0*/)
{
	return _GetPureFileNameStr(m_sFilePathandName);
}

HRESULT CTexture::ChangeImage(const std::string & strFileName)
{
	if (!strFileName.empty() && NULL != m_textureBuf)
	{
		CImage *pImage = new CImage();
		if (pImage)
		{
			_loadImageFromFile(strFileName, pImage);

			HRESULT hr = NULL;
			int nLevel = m_textureBuf->GetLevelCount();
			for (int i = 0; i< nLevel; i++)
			{
				LPDIRECT3DSURFACE9 surf;
				m_textureBuf->GetSurfaceLevel(i,&surf);

				hr = D3DXLoadSurfaceFromFileInMemory(surf, NULL, NULL,
				pImage->data, pImage->dataSize, 
				NULL, D3DX_DEFAULT, 0, NULL);

				SAFE_RELEASE_D3D(surf);
			}

			pImage->Release();
			SAFE_DELETE(pImage);

			m_sFilePathandName = strFileName;

			return hr;
		}
	}

	return E_FAIL;
}
HRESULT CTexture::ChangeTransparentImage(const std::string & strFileName)
{
	if (TRUE != bAlphaBlend || D3DFMT_A8 != m_format || strFileName.empty())
		return E_FAIL;

	CImage *pImage = new CImage();
	if (pImage)
	{
		_loadImageFromFile(strFileName, pImage);
		if (NULL == pImage->data)
		{
			pImage->Release();
			SAFE_DELETE(pImage);
			return E_FAIL;
		}
		//�ͷ���������
		SAFE_RELEASE_D3D(m_textureBuf);

		// �ȴ���һ��A8R8G8B8��texture
		LPDIRECT3DTEXTURE9 pTempTexture = NULL;
		D3DXIMAGE_INFO pSrcInfo;
		if (FAILED(D3DXCreateTextureFromFileInMemoryEx(/*g_VGSRender*/GetRoot()->getVGSRender().m_pd3dDevice,
			pImage->data, pImage->dataSize, D3DX_DEFAULT, D3DX_DEFAULT,	MipCount,
			0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,NULL,
			&pSrcInfo,NULL,&pTempTexture) ) )
			return E_FAIL;

		// �ô�����A8R8G8B8��texture����һ��A8 texture
		width  = pSrcInfo.Width;
		height = pSrcInfo.Height;

		//����Ŀ����ͼ����
		D3DXCreateTexture(/*g_VGSRender*/GetRoot()->getVGSRender().m_pd3dDevice,
			width, 
			height, 
			MipCount, 
			0, 
			m_format,
			D3DPOOL_MANAGED,
			&m_textureBuf);

		UINT bufSize = width * height * 4;

		D3DLOCKED_RECT OriLockRect;
		RECT OriRect;
		OriRect.left   = OriRect.top = 0;
		OriRect.right  = width;
		OriRect.bottom = height;
		if (FAILED(pTempTexture->LockRect(0, &OriLockRect, &OriRect, D3DLOCK_DISCARD)))
			return 0;
		BYTE* OriBuf = new BYTE[bufSize];
		BYTE* DesBuf = new BYTE[bufSize / 4];
		memcpy(OriBuf, (BYTE*)OriLockRect.pBits, bufSize);
		//unlock texture rect
		pTempTexture->UnlockRect(0);
		bufSize = bufSize/4;

		BOOL bWithAlpha = _IsTransparentFormat(imageInfo.Format);
		// ����A8��ͼ
		for (UINT i = 0; i < bufSize; i++)
		{
			if (bWithAlpha)
			{
				DesBuf[i] = OriBuf[i * 4 + 3];
			}
			else
			{
				DesBuf[i] = (UINT)(0.299 * OriBuf[i * 4 + 0] +0.587 * OriBuf[i * 4 + 1] +0.114 * OriBuf[i * 4 + 2]);
			}
		} // for
		SAFE_RELEASE_D3D(pTempTexture);

		//��д�ò���ʵ���ͼ����
		UINT totallevels = 0;
		if (MipCount == 0)
			totallevels = m_textureBuf->GetLevelCount();
		else
			totallevels = MipCount;

		for (UINT miplevel = 0; miplevel< totallevels; miplevel++)
		{
			LPDIRECT3DSURFACE9 surflevel;
			m_textureBuf->GetSurfaceLevel(miplevel,&surflevel);

			RECT SourceRect = {0, 0, width, height};
			D3DXLoadSurfaceFromMemory(surflevel, NULL, NULL,
				DesBuf, m_format,
				width,
				NULL, &SourceRect, D3DX_FILTER_LINEAR, 0);

			surflevel->Release();
		} // for

		//���չ��ɻ����ڴ�
		SAFE_DELETE_ARRAY(OriBuf);
		SAFE_DELETE_ARRAY(DesBuf); 

		pImage->Release();
		SAFE_DELETE(pImage);

		m_sFilePathandName = strFileName;

		return S_OK;
	}
	return E_FAIL;
}
