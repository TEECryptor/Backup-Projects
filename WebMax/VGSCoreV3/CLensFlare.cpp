////////////////////////////////////////////////////////////////////////////////
// 名  称: CLensFlare
// 描  述: 用于光源在摄像机可是范围内产生
// 作  者: zhangfan
// 日  期: 2006-12-15
////////////////////////////////////////////////////////////////////////////////

#include "CLensFlare.h"
#include "CSysFunc.h"
#include "CMaterial.h"
#include "CTexture.h"
//-----------------------------------------------------------------------------
// 方法名: CLensFlare()
// 参  数: LPDIRECT3DDEVICE8 pD3DDevice
// 描  述: CLensFlare构造函数
//-----------------------------------------------------------------------------
//##ModelId=47EBC74900FD
CLensFlare::CLensFlare(LPDIRECT3DDEVICE9 pD3DDevice,
					   RECT* pRect,
					   BYTE* pSunTex, 
					   INT	sunTexSize,
					   BYTE* pTex1,
					   INT tex1Size,
					   BYTE* pTex2, 
					   INT tex2Size,
					   BYTE* pTex3,
					   INT tex3Size,
					   D3DXVECTOR3* pSunPos)
{
	m_pd3dDevice = pD3DDevice;
	m_vSunPos = *pSunPos;
	m_pRect = pRect;

	m_pSunTex = NULL;	
	m_pTex1 = NULL;		
	m_pTex2 = NULL;		
	m_pTex3 = NULL;	
	
	//以窗体的高度为准
	INT width = m_pRect->right - m_pRect->left;
	INT height = m_pRect->bottom - m_pRect->top;
	m_formatRect.left = width/2 - height/2; 
	m_formatRect.right = m_formatRect.left + height;
	m_formatRect.top = m_pRect->top;
	m_formatRect.bottom = m_pRect->bottom;

	//创建LensFlare
	Create(pSunTex, sunTexSize, 
			pTex1, tex1Size,
			pTex2, tex2Size,
			pTex3, tex3Size);
}

//-----------------------------------------------------------------------------
// 方法名: ~CLensFlare()
// 参  数: 
// 描  述: CLensFlare析构函数
//-----------------------------------------------------------------------------

//##ModelId=47EBC7490110
CLensFlare::~CLensFlare()
{
}


//创建Spot
//##ModelId=47EBC74900E1
COverlay* CLensFlare::CreateSpot(FLOAT relativeSize, 
							   FLOAT refLineY,
								LPDIRECT3DTEXTURE9 pTex, 
								D3DCOLOR color)
{
	//创建Overlay
	COverlay* pSunTex_Overlay = new COverlay(m_pd3dDevice, "");
	if (pSunTex_Overlay == NULL)
		return NULL;
/*
	pSunTex_Overlay->refLineY = refLineY;
	pSunTex_Overlay->relativeSize = relativeSize;
	INT length = (m_formatRect.bottom - m_formatRect.top)/2;
	length *= relativeSize;
	RECT rect;
	rect.left = (m_formatRect.right + m_formatRect.left)/2 - length;
	rect.right = (m_formatRect.right + m_formatRect.left)/2 + length;
	rect.top = (m_formatRect.bottom + m_formatRect.top)/2 - length;
	rect.bottom = (m_formatRect.bottom + m_formatRect.top)/2 + length;
	pSunTex_Overlay->SetRect(&rect);

	CMaterial* pMtrl = new CMaterial();
	CTexture* pTexture = new CTexture;
	TextureLayer* pTexLayer = new TextureLayer();
    pTexLayer->pTexture=(DWORD)pTexture;
	
	pTexture->textureBuf = pTex;
	pMtrl->texLayersList.push_back(pTexLayer);
	pSunTex_Overlay->SetMaterial(pMtrl);
	pSunTex_Overlay->SetColor(&color);
*/
	return pSunTex_Overlay;
}

//-----------------------------------------------------------------------------
// 方法名: Create()
// 参  数: const char* pSunTex,const char* pTex1,
//		   const char* pTex2,const char* pTex3
// 描  述: 创建一个CLensFlare的类
//-----------------------------------------------------------------------------
//##ModelId=47EBC74900EC
HRESULT	CLensFlare::Create(BYTE* pSunTex, 
					   INT	sunTexSize,
					   BYTE* pTex1,
					   INT tex1Size,
					   BYTE* pTex2, 
					   INT tex2Size,
					   BYTE* pTex3,
					   INT tex3Size)
{
	D3DXCreateTextureFromFileInMemory(m_pd3dDevice, pSunTex, sunTexSize, &m_pSunTex);
	D3DXCreateTextureFromFileInMemory(m_pd3dDevice, pTex1, tex1Size, &m_pTex1);	
	D3DXCreateTextureFromFileInMemory(m_pd3dDevice, pTex2, tex2Size, &m_pTex2);
	D3DXCreateTextureFromFileInMemory(m_pd3dDevice, pTex3, tex3Size, &m_pTex3);
	
	//太阳
	COverlay* pSpot = CreateSpot(0.8f, 1.0f, m_pSunTex, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	if (pSpot) 	
		m_Spot.push_back(pSpot);

	//第1个圈
	pSpot = CreateSpot(0.08f, 0.6f, m_pTex1, D3DXCOLOR(1.0f, 0.6f, 0.0f, 0.5f));
	if (pSpot) 	
		m_Spot.push_back(pSpot);

	//第2个圈
	pSpot = CreateSpot(0.13f, 0.5f, m_pTex2, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f));
	if (pSpot) 	
		m_Spot.push_back(pSpot);

	//第3个圈
	pSpot = CreateSpot(0.05f, 0.4f, m_pTex2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	if (pSpot) 	
		m_Spot.push_back(pSpot);

	//第4个圈
	pSpot = CreateSpot(0.06f, 0.1f, m_pTex3, D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f));
	if (pSpot) 	
		m_Spot.push_back(pSpot);

	//第5个圈
	pSpot = CreateSpot(0.08f, -0.2f, m_pTex1, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	if (pSpot) 	
		m_Spot.push_back(pSpot);

	//第6个圈
	pSpot = CreateSpot(0.18f, -0.4f, m_pTex1, D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.3f));
	if (pSpot) 	
		m_Spot.push_back(pSpot);

	//第7个圈
	pSpot = CreateSpot(0.62f, -1.0f, m_pTex2, D3DXCOLOR(1.0f, 0.9f, 0.0f, 0.25f));
	if (pSpot) 	
		m_Spot.push_back(pSpot);

	//第8个圈
	pSpot = CreateSpot(1.8f, -1.3f, m_pTex3, D3DXCOLOR(1.0f, 0.6f, 0.0f, 0.15f));
	if (pSpot) 	
		m_Spot.push_back(pSpot);

    return S_OK;
}

//释放内存
//##ModelId=47EBC7490119
VOID CLensFlare::Release()
{
	SAFE_RELEASE_D3D(m_pSunTex);	
	SAFE_RELEASE_D3D(m_pTex1);	
	SAFE_RELEASE_D3D(m_pTex2);	
	SAFE_RELEASE_D3D(m_pTex3);
	
	INT spotCnt = m_Spot.size();
	std::vector<COverlay*>::iterator ppOverlay = m_Spot.begin();
	for (INT i = 0; i< spotCnt; i++)
	{
		COverlay* pOverlay = *(ppOverlay + i);
        
		CTexture* pTex = (CTexture*)(pOverlay->GetMaterial()->m_vpTexLayers[0]->pTexture);
		SAFE_DELETE(pTex);

		CMaterial *pMaterial = pOverlay->GetMaterial();
		SAFE_RELEASE(pMaterial);
		SAFE_DELETE(pMaterial);

		pOverlay->Invalidate();
		SAFE_DELETE(pOverlay);
	}
	m_Spot.clear();
}
