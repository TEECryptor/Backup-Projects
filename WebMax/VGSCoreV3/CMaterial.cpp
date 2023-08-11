/////////////////////////////////////////////////////////////////////////
// 名称: CMaterial类型定义--场景中的材质对象
// 描述: CMaterial类型定义了材质的各种属性
// 作者: Lixin	
// 日期: 2006-04-30
////////////////////////////////////////////////////////////////////////

#include "CMaterial.h"
//#include "MaterialManager.h"
//#include "TextureManager.h"
#include "VGSRender.h"
#include "VGSSupport.h"

#include "SwfTexture.H"
#include "Root.h"

UINT CTextureLayer::getFrameCount()
{
	switch (type)
	{
	case TLT_BASAL:
	case TLT_OPACITY:
	case TLT_REFLECT:
	{
		CTexture* pTex = (CTexture*)pTexture;
		if (pTex)
		{
			return 1;
		}
		break;
	}
	case TLT_BUMPMAP:
		return 1;
		break;
	case TLT_CUBEMAP:
	{	
		CCubeMap *pTex = (CCubeMap*)pTexture;
		if (pTex)
		{
			return 6;
		}
		break;
	}
	default:
		break;
	}
	return 0;
}
std::string CTextureLayer::getFrameFileName(INT Index)
{
	switch (type)
	{
	case TLT_BASAL:
	case TLT_OPACITY:
	case TLT_REFLECT:
		{
			CTexture* pTex = (CTexture*)pTexture;
			if (pTex)
			{
				return pTex->getTextrueFileName();
			}
			break;
	}
	case TLT_BUMPMAP:
	{
		CBumpMap* pTex = (CBumpMap*)pTexture;
		if (pTex)
		{
			return pTex->getTextrueFileName();
		}
		break;
	}
	case TLT_CUBEMAP:
	{		
		CCubeMap *pTex = (CCubeMap*)pTexture;
		if (pTex)
		{
			return pTex->getTextrueFileName((VGS_CUBIC_FACE)Index);
		}
		break;
	}
	default:
		break;
	}
	return "";
}


CMaterial::CMaterial()
{
	m_Opacity = 100;
	selfIllumination = FALSE;
	twoSide = FALSE;

	//一定要初始化
	ZeroMemory(&d3dmtrl, sizeof(D3DMATERIAL9));

	index = -1;

	bIsOverlayMtrl = NULL;
//	cubeMapPower = 255;

	fillMode = _D3DFILLMODE(3); //D3DFILL_SOLID

	bAlphaTest = FALSE;
	bAlphaBlend = FALSE;

	bShowTexture = TRUE;
	bReadyToRender = FALSE;
	bCulled = FALSE;
	// bUseDiffuse =TRUE;
	bumpWaterIndex = -1;

	m_bUseDiffuse = true;

	m_pCurModel = NULL;
}

CMaterial::~CMaterial()
{
	Release();
}

//释放资源
VOID CMaterial::Release()
{
	//Clear the subMeshList
	subMeshList.clear();

    //清空贴图资源
	std::vector<CTextureLayer*>::iterator it = m_vpTexLayers.begin(), end = m_vpTexLayers.end();
	for (; it != end; ++it)
	{
		SAFE_DELETE(*it);
	}
	m_vpTexLayers.clear();

	m_Opacity = 100;
	selfIllumination = FALSE;
	twoSide = FALSE;

	//一定要初始化
	ZeroMemory(&d3dmtrl, sizeof(D3DMATERIAL9));

	index = -1;
}

BOOL CMaterial::IsReadyToRender()
{
	if (!bReadyToRender)
	{
		UINT texLayerCnt = m_vpTexLayers.size();
		std::vector<CTextureLayer*>::iterator pptexLayer = m_vpTexLayers.begin();
		bReadyToRender =TRUE;
		for(UINT m = 0; m < texLayerCnt; m++ )
		{
			CTextureLayer* pTexLayer =*(pptexLayer+m);
			if (NULL == pTexLayer)
			{
				continue;
			}

			if(pTexLayer->type ==TLT_BASAL || pTexLayer->type ==TLT_OPACITY)
			{
				if(((CTexture*)(pTexLayer->pTexture))->m_textureBuf==NULL)
				{
					bReadyToRender = FALSE;
					return FALSE;
				}
			}
		}
	}
	return bReadyToRender;
}
// 材质是否透明
BOOL CMaterial::IsTransparent()       
{
	if (/*bAlphaBlend || */m_Opacity < 100)
	{
		return TRUE;
	}
	
	// return hasTransparentTexture();
	return FALSE;
}

// 设置材质的透明度
VOID CMaterial::setOpacity(INT Opacity)
{
	m_Opacity = Opacity;
	if (m_Opacity < 100)
	{
		bAlphaBlend = TRUE;
	}
	else
	{
		bAlphaBlend = hasTransparentTexture();
	}
}
// 材质是否包含透明贴图
BOOL CMaterial::hasTransparentTexture()
{
	vector<CTextureLayer*>::iterator it = m_vpTexLayers.begin(), end = m_vpTexLayers.end();
	for (; it != end; ++it)
	{
		if (NULL == (*it))
		{
			continue;
		}

		if ((*it)->type == TLT_OPACITY)
		{
			return TRUE;
		}

	}
	return FALSE;
}

std::string CMaterial::getName()
{
	return m_sName;
}
VOID CMaterial::setName(const std::string &sName)
{
	m_sName = sName;
}

UINT CMaterial::getTexureLayerCount()
{
	return m_vpTexLayers.size();
}

CTextureLayer* CMaterial::getTextureLayerByIndex(INT index)
{
	if (index >= 0 && index < (int)m_vpTexLayers.size())
	{
		return m_vpTexLayers[index];
	}
	return 0;
}

// texture层相关函数
CTextureLayer* CMaterial::createTextureLayer(const std::string &sImageFileName, std::string &sTextureName, INT uvIndex, size_t nIndex)
{
	CTextureLayer *pLayer = new CTextureLayer();
	if (pLayer)
	{
		CTexture *pTexture = (CTexture *)m_pRoot->GetTextureManager().createTexture(sImageFileName);
		std::string sLayerName = sTextureName;
		if (sLayerName.empty())
		{
			sLayerName = _GetPureFileNameStr(sImageFileName);
		}
		ZeroMemory(pLayer, sizeof(CTextureLayer));
		pLayer->m_sName = sLayerName;
		pLayer->pTexture = pTexture;
		pLayer->type = TLT_BASAL;
		pLayer->UVIndex = uvIndex;
		pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.AlphaBlendParam = 0;
		pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.ColorBlendParam = 0;

		if (nIndex >= m_vpTexLayers.size())
			m_vpTexLayers.push_back(pLayer); // 添加到texture layer
		else
			m_vpTexLayers.insert(m_vpTexLayers.begin() + nIndex, pLayer);

		if (this->getOpacity() < 100 || sImageFileName.find(".png") != std::string::npos 
			|| sImageFileName.find(".tga") != std::string::npos)
		{
			this->bAlphaBlend = true;
		}
	}
	return pLayer;
}

// 增加一层透明贴图
CTextureLayer* CMaterial::createOpacityTextureLayer(const std::string &sImageFileName, std::string &sTextureName, INT uvIndex, size_t nIndex)
{
	CTextureLayer *pLayer = new CTextureLayer();
	if (pLayer)
	{
		CTexture *pTexture = (CTexture *)m_pRoot->GetTextureManager().createTexture(sImageFileName, TRUE);
		std::string sLayerName = sTextureName;
		if (sLayerName.empty())
		{
			sLayerName = _GetPureFileNameStr(sImageFileName);
		}
		ZeroMemory(pLayer, sizeof(CTextureLayer));
		pLayer->m_sName = sLayerName;
		pLayer->pTexture = pTexture;
		pLayer->type = TLT_OPACITY;
		pLayer->UVIndex = uvIndex;
		pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.AlphaBlendParam = 0;
		pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(VBO_ADD);
		pLayer->Blenderinfo.ColorBlendParam = 0;

		if (!bAlphaTest)
		{
			bAlphaTest = TRUE;
 			AlpahaTestValue = DEFAULT_ALPHATESTVALUE; // 64
		}

		if (nIndex >= m_vpTexLayers.size())
			m_vpTexLayers.push_back(pLayer); // 添加到texture layer
		else
			m_vpTexLayers.insert(m_vpTexLayers.begin() + nIndex, pLayer);

		this->bAlphaBlend = true;
	}
	return pLayer;
}

CTextureLayer* CMaterial::CreateDynamicTextureLayer(const char* szTextureName, const std::vector<std::string>& vecFileName, float fDuration, int nUVidx, size_t nIndex)
{
	if (vecFileName.empty())
	{
		return 0;
	}
	CTextureLayer *pLayer = new CTextureLayer();
	if (pLayer)
	{
		std::string sLayerName = szTextureName;
		if (sLayerName.empty())
		{
			sLayerName = _GetPureFileNameStr(vecFileName[0]);
		}

		D3DXIMAGE_INFO imgInfo;
		memset(&imgInfo, 0, sizeof(imgInfo));
		D3DXGetImageInfoFromFile(vecFileName[0].c_str(), &imgInfo);

		CDynamicTex* pDynamicMap = m_pRoot->GetTextureManager().CreateDynamicTexture(szTextureName, vecFileName, fDuration);

		ZeroMemory(pLayer, sizeof(CTextureLayer));
		pLayer->m_sName = sLayerName;
		pLayer->pTexture = pDynamicMap;
		pLayer->type = TLT_DYNAMIC;
		pLayer->UVIndex = nUVidx;
		pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.AlphaBlendParam = 0;
		pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(VBO_ADD);
		pLayer->Blenderinfo.ColorBlendParam = 0;

		if (!bAlphaTest)
		{
			bAlphaTest = TRUE;
			AlpahaTestValue = DEFAULT_ALPHATESTVALUE;
		}

		if (nIndex >= m_vpTexLayers.size())
			m_vpTexLayers.push_back(pLayer); // 添加到texture layer
		else
			m_vpTexLayers.insert(m_vpTexLayers.begin() + nIndex, pLayer);
	}
	return pLayer;
}

// 创建静态cubemap
CTextureLayer* CMaterial::createStaticCubemapTextureLayer(const CHAR* pTextureName, const vector<std::string> &vsFileName, int nSize, size_t nIndex)
{
	//判断是否支持
	if (!/*g_VGSRender*/GetRoot()->getVGSRender().IsSupportCUBEMAP())
	{
		return 0;
	}

	if (vsFileName.size() != 6)
	{
		return 0;
	}
	std::string sTextureName;
	if (pTextureName)
	{
		sTextureName = std::string(pTextureName);
	}
	else
	{
		sTextureName = _GetPureFileNameStr(vsFileName[0]);
	}

	// 创建贴图层
	CTextureLayer *pLayer = new CTextureLayer();
	if (pLayer)
	{	
		// 创建cubemap
		CCubeMap* pCubeMap = m_pRoot->GetTextureManager().CreateStaticCubeTexture(sTextureName,vsFileName, nSize);

		pLayer->m_sName = sTextureName;
		pLayer->pTexture = pCubeMap;
		pLayer->type = TLT_CUBEMAP;
		pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.AlphaBlendParam = 0;
		pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.ColorBlendParam = 0;

		if (nIndex >= m_vpTexLayers.size())
			m_vpTexLayers.push_back(pLayer); // 添加到texture layer
		else
			m_vpTexLayers.insert(m_vpTexLayers.begin() + nIndex, pLayer);
	}
	return pLayer;
}

// 创建动态cubemap
CTextureLayer* CMaterial::createDynamicCubemapTextureLayer(const CHAR* pTextureName, int nSize, size_t nIndex)
{
	//判断是否支持
	if (!/*g_VGSRender*/GetRoot()->getVGSRender().IsSupportCUBEMAP())
	{
		return 0;
	}

	std::string sTextureName;
	if (pTextureName)
	{
		sTextureName = std::string(pTextureName);
	}
	else
	{
		sTextureName = "";
	}
	
	// 创建贴图层
	CTextureLayer *pLayer = new CTextureLayer();
	if (pLayer)
	{	
		// 创建cubemap
		CCubeMap* pCubeMap =m_pRoot->GetTextureManager().CreateDynamicCubeTexture(sTextureName,nSize);

		pLayer->m_sName = sTextureName;
		pLayer->pTexture = pCubeMap;
		pLayer->type = TLT_CUBEMAP;
		pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(/*VBO_MODULATE*/VBO_DISABLE);
		pLayer->Blenderinfo.AlphaBlendParam = 0;
		pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.ColorBlendParam = 0;
		
		if (nIndex >= m_vpTexLayers.size())
			m_vpTexLayers.push_back(pLayer); // 添加到texture layer
		else
			m_vpTexLayers.insert(m_vpTexLayers.begin() + nIndex, pLayer);
	}
	return pLayer;
}

// 创建bump贴图
CTextureLayer* CMaterial::createBumpTextureLayer(const CHAR* pTextureName/*, const CHAR* pTextureFileName*/, size_t nIndex)
{		
	
	std::string sTextureName = "";
	//if (pTextureFileName)
	{
		if (pTextureName)
		{
			sTextureName = std::string(pTextureName);
		}
		//else
		//{
		//	sTextureName = _GetPureFileNameStr(std::string(pTextureFileName));
		//}
	}
	
	// 创建贴图层
	CTextureLayer *pLayer = new CTextureLayer();
	if (pLayer)
	{	
		// 创建cubemap
		CBumpMap* pBump = m_pRoot->GetTextureManager().CreateBumpTexture(sTextureName);
		if(pBump)
		{
			pBump->m_pMaterial = this;			
		}
		// 
		pLayer->m_sName = sTextureName;
		pLayer->pTexture = /*(DWORD)*/pBump;
		pLayer->type = TLT_BUMPMAP;
		pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.AlphaBlendParam = 0;
		pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.ColorBlendParam = 0;
		
		m_vpTexLayers.push_back(pLayer); // 添加到texture layer

		UINT modelCount = subMeshList.size();
		for (UINT i = 0; i < modelCount; ++i)
		{
			CModel *pModel = (CModel*)subMeshList[i].modelPointer;
			if (pModel)
			{
				pModel->addBumpmap(pBump);
			}
		}

		if (nIndex >= m_vpTexLayers.size())
			m_vpTexLayers.push_back(pLayer); // 添加到texture layer
		else
			m_vpTexLayers.insert(m_vpTexLayers.begin() + nIndex, pLayer);
	}
	return pLayer;
}
CTextureLayer* CMaterial::CreateRTreflectTextureLayer(const std::string& strTextureName,int nWidth, int nHeight, size_t nIndex)
{
	std::string strName = "";
	if (strTextureName.empty())
	{
		strName = m_sName + "_RTreflect";
	}
	else
	{
		strName = strTextureName;
	}

	// 创建贴图层
	CTextureLayer *pLayer = new CTextureLayer();
	if (pLayer)
	{	
		// 创建
		CRTFMap* pMap = m_pRoot->GetTextureManager().CreateRTreflectTexture(strTextureName,nWidth, nHeight);
		if (pMap)
			pMap->m_pMaterial = this;
		// 
		pLayer->m_sName = strName;
		pLayer->pTexture = pMap;
		pLayer->type = TLT_RTFMAP;
		pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.AlphaBlendParam = 0;
		pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.ColorBlendParam = 0;

		if (nIndex >= m_vpTexLayers.size())
			m_vpTexLayers.push_back(pLayer); // 添加到texture layer
		else
			m_vpTexLayers.insert(m_vpTexLayers.begin() + nIndex, pLayer);
	}
	return pLayer;
}

CTextureLayer* CMaterial::CreateFlashTextureLayer(const std::string& strTextureName, const std::string& strFlashPath,
												  int flashWidth, int flashHeight, int nWidth, int nHeight, size_t nIndex)
{
	std::string strName = "";
	if (strTextureName.empty())
	{
		strName = m_sName + "_RTreflect";
	}
	else
	{
		strName = strTextureName;
	}

	CTextureLayer *pLayer = new CTextureLayer();
	if (pLayer)
	{	
		// 创建
		SwfTexture* pFlash = m_pRoot->GetTextureManager().CreateFlashTexture(strName, strFlashPath, flashWidth, flashHeight,nWidth, nHeight);
		// 
		pLayer->m_sName = strName;
		pLayer->pTexture = pFlash;
		pLayer->type = TLT_FLASH;
		pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.AlphaBlendParam = 0;
		pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(VBO_MODULATE);
		pLayer->Blenderinfo.ColorBlendParam = 0;

		if (nIndex >= m_vpTexLayers.size())
			m_vpTexLayers.push_back(pLayer); // 添加到texture layer
		else
			m_vpTexLayers.insert(m_vpTexLayers.begin() + nIndex, pLayer);
	}
	return pLayer;
}

VOID CMaterial::clearTextureLayerByIndex(size_t index)
{
	if (index >=0 && index < m_vpTexLayers.size())
	{
		SAFE_DELETE((m_vpTexLayers[index]));
// 		m_vpTexLayers.erase(m_vpTexLayers.begin() + index);
	}
}
VOID CMaterial::clearTextureLayerByHandle(CTextureLayer *pLayer)
{
	std::vector<CTextureLayer*>::iterator it, end = m_vpTexLayers.end();
	it = std::find(m_vpTexLayers.begin(), m_vpTexLayers.end(), pLayer);
	if (it != end)
	{
		SAFE_DELETE((*it));
// 		m_vpTexLayers.erase(it);
	}
}

//将当前材质设置到设备
VOID CMaterial::SetToDevice(LPDIRECT3DDEVICE9 pd3dDevice, BOOL bRenderBump, CModel* pModel)
{
	m_pCurModel = pModel;
	//贴图通道的编号
	int CurTexStage = 0;
	
	//清空所有贴图通道的内容和状态
	ClearTextureMem(pd3dDevice);

	//材质本身的颜色
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	//判断是否是双面材质
	if (twoSide )
	{
		pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	
	}
	else
	{
		pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	}

	if (m_Opacity < 100)
	{
		pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	else
	{
		pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	//如果模型是半透明模型，设置ZWRITEENABLE为False，并打开Alpha混色模式
	if (bAlphaBlend) //opacity<100则bAlphaBlend为true
	{
		//渲染半透明物体的渲染状态
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	}
	//如果是不透明物体，设置ZWRITEENABLE为True, 并关闭Alpha混色
	else
	{
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	//材质自发光状态的设置，Overlay默认为自发光材质
	if (selfIllumination == 100)
	{
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
		pd3dDevice->SetMaterial(&d3dmtrl);
	}																																																				
	else 
	{
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pd3dDevice->SetMaterial(&d3dmtrl);
		pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_SHADEMODE, type);
	}

	//是否打开Alpha测试
	if(bAlphaTest)
	{
		pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_ALPHAREF, AlpahaTestValue);
		pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	else
	    pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//for test 2010-7-9
// 	if (GetRoot()->getGfxMgrptr() && GetRoot()->getGfxMgrptr()->isRenderWireframe())
// 	{
// 		pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
// 	}
// 	else
	// 填充模式
	pd3dDevice->SetRenderState(D3DRS_FILLMODE, fillMode);

	
	//多层贴图通道的混色处理，两种情况：包含凹凸贴图或不包含。不包含直接渲染，包含先渲染其它贴图到表面，再与凹凸混色
	CTextureLayer* pTextureLayer = IsContainBumpWaveMap();
	//包含凹凸贴图的材质
	if (pTextureLayer && (pModel || bRenderBump))//认为是按模型而非按材质来渲染的
// 	if (pTextureLayer && bRenderBump)	
	{	
		SetBumpWaveTextureLayerAndOP(pd3dDevice, pTextureLayer);
	}
	//不包含凹凸贴图的材质
	else 
	{
		//贴图层数
		int texLayerCnt = m_vpTexLayers.size();
		//材质有贴图，则遍历贴图层列表逐层按类型贴图并混色
		std::vector<CTextureLayer*>::iterator ppTexLayer = m_vpTexLayers.begin();
		for(int i = 0; i< texLayerCnt; i++)
		{
			CTextureLayer* pTexLayer = *(ppTexLayer+i);
			//设置当前层的贴图
			SetTextureLayer(pd3dDevice, pTexLayer, i);
		}
	}
	//for test	2010-7-9
// 	if (m_sName == "PM"   && GetRoot()->getGfxMgrptr()  )
// 	{
// 		GetRoot()->getGfxMgrptr()->setDeviceTexture(0);
// 	}

// 	pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE/*D3DTEXF_POINT*/);
// 	pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE/*D3DTEXF_POINT*/);
// 	pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE/*D3DTEXF_POINT*/);

}

//---------------------------------------------------------------------------------------------------
//方法: SetBumpWaveTextureLayerAndOP()
//参数: 
//用途: 凹凸贴图的混色要分开设置
//说明: 如果材质包含凹凸贴图，则先将其它贴图层混色，并将混色渲染的结果与凹凸混色
//---------------------------------------------------------------------------------------------------
VOID CMaterial::SetBumpWaveTextureLayerAndOP(LPDIRECT3DDEVICE9 pd3dDevice, CTextureLayer* pTextureLayer)
{
	int CurTexStage = 0;
	
	//设置贴图坐标的Wrap模式
	pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	//设置该通道贴图使用的贴图坐标通道
	pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXCOORDINDEX, pTextureLayer->UVIndex);
	
	//凹凸贴图对象
	CBumpMap* pBumpMap = (CBumpMap*)pTextureLayer->pTexture;
	
	//设置贴图Buffer
	pd3dDevice->SetTexture(CurTexStage, pBumpMap->getBumpWavemap());
	
	//设置贴图混色的两个源，Bump贴图放在上一层
	pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
	
	//设置混色模式
	if( pBumpMap->m_BumpMapFormat == D3DFMT_V8U8 )  //让材质上的环境反射图产生凹凸效果
		pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);	
	else //贴图上产生明暗效果
		pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_COLOROP, D3DTOP_BUMPENVMAPLUMINANCE);	
	
	//设置Bump贴图通道的相关参数，产生凹凸效果
	pBumpMap->UpdateBumpWaveMap(CurTexStage);
	
	CurTexStage++;
	//贴图坐标
	pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP);
	pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSV,  D3DTADDRESS_CLAMP); 
	pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION );
	pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED | D3DTTFF_COUNT3);
	pd3dDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + CurTexStage), &pBumpMap->m_compositeTexPorjMatrix);
	
	//其它图层混合后渲染出的贴图表面，与Bump进行混色
	pd3dDevice->SetTexture(CurTexStage, pBumpMap->m_compositeTexForBump);
	
	pd3dDevice->SetTextureStageState (CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
	//如果有Color Factor混色
	if(pTextureLayer->Blenderinfo.ColorBlendOption == D3DTOP_BLENDFACTORALPHA)
		pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, pTextureLayer->Blenderinfo.ColorBlendParam);
	
	//设置混色方式
	pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_COLOROP, pTextureLayer->Blenderinfo.ColorBlendOption);
	
	//Alha透明混色----------------------------------------------------------------------------------------------
	if (bAlphaBlend)
	{
		//设置Alpha源1,2
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	
		if (CurTexStage == 0)
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		else
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		
		//如果有Alpha Factor混合
		if(pTextureLayer->Blenderinfo.AlphaBlendOption == D3DTOP_BLENDFACTORALPHA)
			pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, pTextureLayer->Blenderinfo.AlphaBlendParam);
		
		//设置混色方式
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAOP, pTextureLayer->Blenderinfo.AlphaBlendOption);
	}
	
	//Alha透明测试----------------------------------------------------------------------------------------------
	if (bAlphaTest)
	{
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	}	
}

//---------------------------------------------------------------------------------------------------
//方法: SetTextureLayer()
//参数: TextureLayer* pTexLayer: 材质的某一层贴图
//用途: 设置材质的某一层贴图
//说明: 一个材质可以包含很多层贴图，每层贴图可以使用不同的图像和贴图坐标，每层贴图之间
//可以设置不同的混色方式，包含颜色混色和Alpha混色
//---------------------------------------------------------------------------------------------------
VOID CMaterial::SetTextureLayer(LPDIRECT3DDEVICE9 pd3dDevice, CTextureLayer* pTexLayer, int CurTexStage)
{
	if (NULL == pTexLayer)
	{
		return;
	}

	switch(pTexLayer->type)
	{
	//普通贴图通道
	case TLT_BASAL:
	//透明贴图通道
	case TLT_OPACITY:
		//设置贴图坐标的Wrap模式
		if(bIsOverlayMtrl) //Overlay的贴图直接平铺
		{
			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		    pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);		
		}
		else	//普通模型的贴图采用贴图坐标包裹
		{
		   pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		   pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		}

		//设置该通道贴图使用的贴图坐标通道
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXCOORDINDEX, pTexLayer->UVIndex);

		//设置贴图Buffer
		pd3dDevice->SetTexture(CurTexStage, ((CTexture*)(pTexLayer->pTexture))->m_textureBuf);

		//新版引擎将动态UV设置为模型所在材质的每个textureLayer中
		if(pTexLayer->USpeed != 0 ||pTexLayer->VSpeed != 0)
		{
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
			MoveTextureUV(pd3dDevice, CurTexStage, pTexLayer->USpeed, pTexLayer->VSpeed, D3DTTFF_COUNT2);
		}
		else//关掉贴图的变换，防止上一个状态对贴图状态的影响
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

		break;
	
	case TLT_BUMPMAP://凹凸贴图通道
		{
			//凹凸贴图对象
			CBumpMap* pBumpMap = (CBumpMap*)pTexLayer->pTexture;
			//跳过，不在这里渲染，因为我们希望凹凸水面对所有贴图层起作用
			if (pBumpMap->m_type == VBT_BUMPWATER) return;

			//设置贴图坐标的Wrap模式
			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

			//设置该通道贴图使用的贴图坐标通道
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXCOORDINDEX, pTexLayer->UVIndex);

			//凹凸反射
			if (pBumpMap->m_type == VBT_BUMPENVIRONMENT)
			{
				//设置贴图Buffer
				pd3dDevice->SetTexture(CurTexStage, pBumpMap->m_pBumpEnvMap);

				//设置贴图混色的两个源，Bump贴图放在上一层
				pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_COLORARG2, D3DTA_CURRENT );

				//设置混色模式
				if( pBumpMap->m_BumpMapFormat == D3DFMT_V8U8 )  //让材质上的环境反射图产生凹凸效果
					pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);	
				else //贴图上产生明暗效果
					pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_COLOROP, D3DTOP_BUMPENVMAPLUMINANCE);	

				//设置Bump贴图通道的相关参数，产生凹凸效果
				pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_BUMPENVMAT00, F2DW(pBumpMap->m_Noise1));
				pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_BUMPENVMAT01, F2DW(0.0f));
				pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_BUMPENVMAT10, F2DW(0.0f));
				pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_BUMPENVMAT11, F2DW(pBumpMap->m_Noise2));
				pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_BUMPENVLSCALE, F2DW(4.0f));
				pd3dDevice->SetTextureStageState( CurTexStage, D3DTSS_BUMPENVLOFFSET, F2DW(0.0f));
			}
			//Dot3Product 法线贴图
			else if (pBumpMap->m_type == VBT_BUMPNORMARLMAP)
			{
				//设置贴图Buffer
				pd3dDevice->SetTexture(CurTexStage, pBumpMap->m_pNormalMap);

				// 向量变换为RGB
				pBumpMap->VectortoRGBA(0);	
				//变换为RGB的向量登录为TextureFactor值
				pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, pBumpMap->m_dwFactor);

				//纹理的RGB和光源向量内积运算
				pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);	
				//运算中使用内积	
				pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_COLORARG2, D3DTA_TFACTOR);
				pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
			}
		}
		break;

	//实时生成反射贴图通道
	case TLT_RTFMAP:
		{
			//设置贴图坐标的Wrap模式
			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR/*D3DTEXF_POINT*/);
			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR/*D3DTEXF_POINT*/);
			
			//设置该通道贴图使用的贴图坐标为相机空间贴图坐标
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
			//设置该贴图通道的矩阵变换
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3 | D3DTTFF_PROJECTED);
			//设置贴图Buffer
			CRTFMap* pRTFMap = (CRTFMap*)pTexLayer->pTexture;
			pd3dDevice->SetTexture(CurTexStage, pRTFMap->m_textureBuf);
			
			//设置贴图的变换矩阵
			pd3dDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + CurTexStage), &(pRTFMap->m_texProjMatrix));
			
		}
		break;
		
	//静态反射贴图通道
	case TLT_REFLECT:
		//设置贴图坐标的Wrap模式
		pd3dDevice->SetSamplerState( CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		pd3dDevice->SetSamplerState( CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		pd3dDevice->SetSamplerState( CurTexStage, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
		
		//设置该通道贴图使用的贴图坐标为相机空间反射贴图坐标
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXCOORDINDEX, pTexLayer->UVIndex);
		//设置该贴图通道的矩阵变换
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, pTexLayer->transformFlag);
		//设置贴图Buffer
		pd3dDevice->SetTexture(CurTexStage, pTexLayer->pTexture->m_textureBuf);

		//新版引擎将动态UV设置为模型所在材质的每个textureLayer中
		if(pTexLayer->USpeed != 0 ||pTexLayer->VSpeed != 0)
			MoveTextureUV(pd3dDevice, CurTexStage, pTexLayer->USpeed, pTexLayer->VSpeed, pTexLayer->transformFlag);

		break;

	//实时阴影贴图通道
	case TLT_REALTIMESHADOW:
		//暂未实现，保留接口
		break;

	//立方图贴图通道
	case TLT_CUBEMAP:
		{
			//获得CubeMap对象
			CCubeMap* pCubeMap = (CCubeMap*)pTexLayer->pTexture;
			if (!pCubeMap) break;

			//设置贴图坐标的Wrap模式
			pd3dDevice->SetSamplerState( CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			pd3dDevice->SetSamplerState( CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
			pd3dDevice->SetSamplerState( CurTexStage, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

			//设置该通道贴图使用的贴图坐标为相机空间反射贴图坐标
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
			//设置该贴图通道的矩阵变换
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);
			//设置贴图Buffer
			pd3dDevice->SetTexture(CurTexStage, pCubeMap->m_pCubeTex);

			//新版引擎将动态UV设置为模型所在材质的每个textureLayer中
			if(pTexLayer->USpeed != 0 ||pTexLayer->VSpeed != 0)
				MoveTextureUV(pd3dDevice, CurTexStage, pTexLayer->USpeed, pTexLayer->VSpeed, D3DTTFF_COUNT3);
		}
		break;

	//动态贴图通道
	case TLT_DYNAMIC:
		{
			//设置贴图坐标的Wrap模式
			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
			
			//设置该通道贴图使用的贴图坐标通道
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXCOORDINDEX, pTexLayer->UVIndex);

			//新版引擎将动态UV设置为模型所在材质的每个textureLayer中
			if(pTexLayer->USpeed != 0 ||pTexLayer->VSpeed != 0)
			{
				pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
				MoveTextureUV(pd3dDevice, CurTexStage, pTexLayer->USpeed, pTexLayer->VSpeed, D3DTTFF_COUNT2);
			}
			else//关掉贴图的变换，防止上一个状态对贴图状态的影响
				pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

			//设置动态贴图Buffer
			CDynamicTex* pDyTex =(CDynamicTex*)pTexLayer->pTexture;

			if (pDyTex  &&  pDyTex->GetImageCount() > 0)
			{
				pDyTex->Update();
				pd3dDevice->SetTexture(CurTexStage, pDyTex->GetCurTexture());
			}

		}
		break;

	/*
	//实时凹凸通道
	case TLT_BUMPWATER:
		//暂未实现，用于动态波动表面(譬如水、云)的渲染
		break;
		*/

	//电影贴图通道
	case TLT_MOVIE:
		//暂未实现，用于实现电影贴图
		break;

	//Flash贴图通道
	case TLT_FLASH:
		{
			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			pd3dDevice->SetSamplerState(CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
			//设置该通道贴图使用的贴图坐标通道
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXCOORDINDEX, pTexLayer->UVIndex);

			SwfTexture* pTex =(SwfTexture*)pTexLayer->pTexture;
			if(pTex)
			{
				pTex->GetFrame();
				pd3dDevice->SetTexture(CurTexStage, pTex->frameRenderTexDest);
			}

			//新版引擎将动态UV设置为模型所在材质的每个textureLayer中
			if(pTexLayer->USpeed != 0 ||pTexLayer->VSpeed != 0)
			{
				pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
				MoveTextureUV(pd3dDevice, CurTexStage,pTexLayer->USpeed, pTexLayer->VSpeed, D3DTTFF_COUNT2);
			}
			else//关掉贴图的变换，防止上一个状态对贴图状态的影响
				pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		}
		break;
	
	//贴图层不存在
	case TLT_INVALID:
		break;

	//暂不不支持的贴图通道
	default:
		break;
	}
			
	//设置混色模式，颜色混色------------------------------------------------------------------------------------------
	//设置颜色源1,2
	if (pTexLayer->type != TLT_BUMPMAP)
	{
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		if (CurTexStage == 0)
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		else
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_COLORARG2, D3DTA_CURRENT);

		//如果有Color Factor混色
		if(pTexLayer->Blenderinfo.ColorBlendOption == D3DTOP_BLENDFACTORALPHA)
			pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, pTexLayer->Blenderinfo.ColorBlendParam);

		//设置混色方式
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_COLOROP, pTexLayer->Blenderinfo.ColorBlendOption);
	}
	
	//Alha透明混色----------------------------------------------------------------------------------------------
	if (bAlphaBlend)
	{
		//设置Alpha源1,2
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	
		if (CurTexStage == 0)
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		else
			pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

		//如果有Alpha Factor混合
		if(pTexLayer->Blenderinfo.AlphaBlendOption == D3DTOP_BLENDFACTORALPHA)
			pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, pTexLayer->Blenderinfo.AlphaBlendParam);

		//设置混色方式
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAOP, pTexLayer->Blenderinfo.AlphaBlendOption);
	}

	//Alha透明测试----------------------------------------------------------------------------------------------
	if (bAlphaTest)
	{
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pd3dDevice->SetTextureStageState(CurTexStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	}	
}

//移动贴图层的UV
VOID CMaterial::MoveTextureUV(LPDIRECT3DDEVICE9 pd3dDevice, int index, float uSpeed, float vSpeed, D3DTEXTURETRANSFORMFLAGS flag)
{
	switch (flag)
	{
	case D3DTTFF_COUNT2:
		m_transformMat[index]._31 += uSpeed;
		m_transformMat[index]._32 += vSpeed;
		break;
	case D3DTTFF_COUNT3:
		m_transformMat[index]._41 += uSpeed;
		m_transformMat[index]._42 += vSpeed;
		break;
	}
	pd3dDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + index), &m_transformMat[index]);
}

//---------------------------------------------------------------------------------------------------
//方法: IsContainBumpWaveMap()
//参数: 
//用途: 判断贴图层中是否包含凹凸水面贴图
//说明: 如果材质包含凹凸水面贴图，则先将其它贴图层混色，并将混色渲染的结果与凹凸混色
//---------------------------------------------------------------------------------------------------
CTextureLayer* CMaterial::IsContainBumpWaveMap()
{
	int texLayerCnt = m_vpTexLayers.size();
	std::vector<CTextureLayer*>::iterator ppTexLayer = m_vpTexLayers.begin();
	for(int i = 0; i< texLayerCnt; i++)
	{
		CTextureLayer* pTexLayer = *(ppTexLayer+i);
		if (!pTexLayer)
		{
			continue;
		}
		if (pTexLayer->type == TLT_BUMPMAP)
		{
			CBumpMap* pBumpMap = (CBumpMap*)pTexLayer->pTexture;
			if (pBumpMap->m_type == VBT_BUMPWATER)
				return pTexLayer;
		}
	}
	return NULL;
}

//清空贴图内存
VOID CMaterial::ClearTextureMem(LPDIRECT3DDEVICE9 pd3dDevice)
{
	//set tex-0, tex-1.. tex-8
	for (INT i = 0; i < 8; i++)
	{
		pd3dDevice->SetTexture(i, NULL);
		pd3dDevice->SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
		pd3dDevice->SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, 0);
	}
}

// 将使用本材质的submesh添加到列表
VOID CMaterial::AddSubmeshToList(const VGS_RenderSubmesh& rsm)
{
	if (find(subMeshList.begin(), subMeshList.end(), rsm) == subMeshList.end())
	{
		subMeshList.push_back(rsm);
		stable_sort(subMeshList.begin(), subMeshList.end());
	}
}

