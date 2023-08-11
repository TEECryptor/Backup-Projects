/////////////////////////////////////////////////////////////////////////
// 名称: CMaterial类型定义--场景中的材质对象
// 描述: CMaterial类型定义了材质的各种属性
// 作者: Lixin	
// 日期: 2006-04-30
////////////////////////////////////////////////////////////////////////

#ifndef _CMATERIAL_H
#define _CMATERIAL_H

#include "VGSHeader.h"
#include "CTexture.h"
#include "CSysFunc.h"
#include "BumpMap.h"
#include "CCubeMap.h"
#include "CDynamicTex.h"
#include "RTFMap.h"
#include ".\fileio\N3D_def.h"
//#include "ResourceManager.h"
//#include "TextureManager.h"

#define TWO_SIDED           0x01
#define REF_TEXTURE_MAPPING 0x02
#define NO_TEXTURE          0x04
#define FLAT_SHADING		0x0000
#define GOURAUD_SHADING     0x08
#define PHONG_SHADING       0x20


#include <vector>
#include <set>
using std::vector;
using namespace N3D;

//Add by Baojiahui ----------------------------------------------------------------------------------
//change the stucture in order to group the mtrl by submesh 
class VGS_EXPORT VGS_RenderSubmesh
{
public:
	VGS_RenderSubmesh(){modelPointer = subMeshID = 0;}
	DWORD			modelPointer;					//subMesh所在的model指针
	int			subMeshID;						//subMesh的索引号
	inline friend bool operator < (const VGS_RenderSubmesh& lv, const VGS_RenderSubmesh& rv)
	{
		return lv.modelPointer < rv.modelPointer && lv.subMeshID < rv.subMeshID;
	}
	inline friend bool operator == (const VGS_RenderSubmesh& lv, const VGS_RenderSubmesh& rv)
	{
		return lv.modelPointer == rv.modelPointer && lv.subMeshID == rv.subMeshID;
	}
};

typedef struct _BlenderInfo
{
	D3DTEXTUREOP			ColorBlendOption;       // 颜色混色方式
	D3DCOLOR 		        ColorBlendParam;        // 颜色混色参数
	
	D3DTEXTUREOP			AlphaBlendOption;       // alpha混色方式
	D3DCOLOR 		        AlphaBlendParam;        // alpha混色参数
	

	_BlenderInfo()
	{
		ColorBlendOption = D3DTOP_MODULATE;
		ColorBlendParam = 0;
		AlphaBlendOption = D3DTOP_MODULATE;
		AlphaBlendParam = 0;
	}
}_BlenderInfo;

typedef struct TextureLayer_old
{
	CHAR					Name[N3D_MAX_NAME_LENGTH];     // 贴图名称
	
	VGS_TextureType         type;                          // 贴图类型
	int						UVIndex;                       // 使用的UV索引
	_BlenderInfo            Blenderinfo;                   // 混色参数
	DWORD					pTexture;
}TextureLayer_old;

class VGS_EXPORT CTextureLayer : public CVGSGetRoot
{
public:
	std::string             m_sName;
	VGS_TextureType         type;                          // 贴图类型
	N3D_TextureParam        param;                         // texture 参数
	int						UVIndex;                       // 使用的UV索引
	_BlenderInfo            Blenderinfo;                   // 混色参数
	float                   USpeed;
	float                   VSpeed;
	CTexture*               pTexture;
	INT                     mipMap;

	VGS_REFLECT_TYPE        m_refletionType;  //  当type == TLT_REFLECT时，表示反射方式

	UINT getFrameCount();
	std::string getFrameFileName(INT Index);
	//add by Nick, 2008年12月13日，用于体现贴图层的变换
	D3DTEXTURETRANSFORMFLAGS	transformFlag;
	CTextureLayer()
	{
	  USpeed = 0;
	  VSpeed = 0;
	  UVIndex = 0;
	  pTexture = NULL; //防止野指针
	}
	~CTextureLayer()
	{
		// CTextureManager::getSingleton().removeResource(pTexture);
// 		SAFE_DELETE(pTexture);
	}

private:
	
};


class VGS_EXPORT CMaterial : public CResource
{
public:
	INT					index;						//材质在渲染队列中的编号
	D3DMATERIAL9		d3dmtrl;					//材质数据
 
	D3DCOLORVALUE		specular;					//高光色
	FLOAT				specularLevel;				//高光强度	
	
	//UINT				opacity;					//透明度
	//INT					oriOpacity;					//原始透明度
	INT					type;						//材质类型
	BOOL				twoSide;					//是否为双面材质
	_D3DFILLMODE		fillMode;					//填充模式

	BOOL				bShowTexture;				//是否显示贴图

	BOOL                bReadyToRender;                  //贴图是否已经下载完毕

	// BOOL                bUseDiffuse;                //是否使用模型的diffuse颜色

	UINT				selfIllumination;			//自发光


	UINT				m_FVF;	
	UINT				m_FVFSize;	

	vector<VGS_RenderSubmesh>  subMeshList;				//使用这个材质的SubMesh列表

	//是否使用AlphaTest方式(不使用混色，使用ALPHA测试)来渲染使用该材质的模型
	BOOL				bAlphaTest;	
	BOOL				bAlphaBlend;				//是否使用Alpha混色

	INT                 AlpahaTestValue;            // AlphaTest的门限值，小于该门限的点将会被剔除

	BOOL				bIsOverlayMtrl;				//是否是Overlay材质

	BOOL				bCulled;					//该材质的所有模型队列是否都被剪裁

	vector<CTextureLayer*>    m_vpTexLayers;     //材质层列表

	INT					bumpWaterIndex;           //bumpwater材质 的序列号，不是bumpwater则为 -1

	BOOL                m_bUseDiffuse;            // 是否使用diffuse颜色
	D3DXMATRIX			m_transformMat[8];			//贴图的变换矩阵

	/** 材质对应的当前模型.当前主要用于法线贴图渲染时灯光方向的计算.
	*/
	CModel*				m_pCurModel;
public:
		
	VOID				Release();

	CMaterial();
	~CMaterial();

	BOOL                IsTransparent();        // 是否透明

	BOOL                IsReadyToRender();      // 贴图是否已经加载完毕

	std::string getName();
	VOID setName(const std::string &sName);

	UINT getTexureLayerCount();

	// texture层相关函数
	CTextureLayer* getTextureLayerByIndex(INT index);
	VOID clearTextureLayerByIndex(size_t index);
	VOID clearTextureLayerByHandle(CTextureLayer *pLayer);

	// 增加一层普通贴图
	CTextureLayer* createTextureLayer(const std::string &sImageFileName, std::string &sTextureName, INT uvIndex, size_t nIndex = 100000);
	
	// 增加一层透明贴图
	CTextureLayer* createOpacityTextureLayer(const std::string &sImageFileName, std::string &sTextureName, INT uvIndex, size_t nIndex = 100000);
	
	/** 增加一层动态贴图
	@param szTextureName in:贴图名称
	@param vecFileName in:所有图像名称列表
	@param nDuration in:播放周期
	@param nUVidx in:UV索引
	@return:CTextureLayer对象指针.
	*/
	CTextureLayer* CreateDynamicTextureLayer(const char* szTextureName, const std::vector<std::string>& vecFileName, float fDuration, int nUVidx, size_t nIndex = 100000);

	// 创建静态cubemap
	CTextureLayer* createStaticCubemapTextureLayer(const CHAR* pTextureName, const vector<std::string> &vsFileName, int nSize, size_t nIndex = 100000);

	// 创建动态cubemap
	CTextureLayer* createDynamicCubemapTextureLayer(const CHAR* pTextureName, int nSize, size_t nIndex = 100000);

	// 创建bump贴图
	CTextureLayer* createBumpTextureLayer(const CHAR* pTextureName/*, const CHAR* pTextureFileName*/, size_t nIndex = 100000);
	/** 创建实时反射贴图层.
	@param strTextureName in:贴图名称.
	@param nWidth in:贴图宽度.
	@param nHeight in:贴图高度.
	*/
	CTextureLayer* CreateRTreflectTextureLayer(const std::string& strTextureName, int nWidth, int nHeight, size_t nIndex = 100000);

	CTextureLayer* CreateFlashTextureLayer(const std::string& strTextureName, const std::string& strFlashPath,
		int flashWidth, int flashHeight, int nWidth, int nHeight, size_t nIndex = 100000);

	//将当前材质设置到设备
	VOID				SetToDevice(LPDIRECT3DDEVICE9 pd3dDevice, BOOL bRenderBump, CModel* pModel = NULL);

	//清空每层贴图通道的内容和状态
	VOID				ClearTextureMem(LPDIRECT3DDEVICE9 pd3dDevice);
	//设置贴图层
	VOID				SetTextureLayer(LPDIRECT3DDEVICE9 pd3dDevice, CTextureLayer* pTexLayer, int CurTexStage);
	//设置凹凸贴图层
	//说明: 如果材质包含凹凸水面贴图，则先将其它贴图层混色，并将混色渲染的结果与凹凸混色
	VOID				SetBumpWaveTextureLayerAndOP(LPDIRECT3DDEVICE9 pd3dDevice, CTextureLayer* pTextureLayer);
	//移动贴图层的UV
	VOID				MoveTextureUV(LPDIRECT3DDEVICE9 pd3dDevice, int index, float uSpeed, float vSpeed, D3DTEXTURETRANSFORMFLAGS flag);
	//判断贴图层中是否包含凹凸水面贴图
	CTextureLayer*		IsContainBumpWaveMap();

	// 设置材质的透明度
	VOID setOpacity(INT Opacity);
	INT getOpacity(){return m_Opacity;};

	// 材质是否包含透明贴图
	BOOL hasTransparentTexture();

	// 将使用本材质的submesh添加到列表
	VOID AddSubmeshToList(const VGS_RenderSubmesh& rsm);

private:
	UINT				m_Opacity;					//透明度
	INT					m_OriOpacity;					//原始透明度
};

#endif
