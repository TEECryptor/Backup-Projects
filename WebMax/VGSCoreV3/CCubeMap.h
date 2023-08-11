////////////////////////////////////////////////////////////////////////////////////////////////
// 名称: CEnvMap类型定义--环境贴图
// 描述: 对某个模型应用环境贴图的实现，包括动态和静态的，使用了DX8中的Cube Map技术
//			立方贴图类型定义，该类实现了立方贴图的创建，由于立方贴图与位置
//         有关，所以立方体贴图属性放置在了CModel类中，随着模型的位置改变而改变。
//		   如果放在CMaterial类中，会比较难处理，因为可能有多个物体用到这个CubeMap
//         而每个物体的位置又不相同。
//         CCubeMap类实现了两种CubeMap，静态CubeMap,动态CubeMap。静态CubeMap通过指定
//		   6幅图片来实现，而动态CubeMap是根据周围环境实时渲染生成的6幅图像来实现的，为了
//		   节省实时渲染带来的开销，CCubeMap类可以指定需要渲染到立方体贴图的模型，
//         这样可以大大节省运算量。
// 作者: Lixin
// 日期: 2007-1-12
// 版本说明: 
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CCUBEMAP_H
#define _CCUBEMAP_H

#include "VGSHeader.h"
#include "CImage.h"
#include "CTexture.h"

#include <vector>
using std::vector;

class VGS_EXPORT CCubeMap : public CTexture
{
public:
	LPDIRECT3DDEVICE9				m_pd3dDevice;									//d3d设备
	LPDIRECT3DCUBETEXTURE9			m_pCubeTex;										//立方贴图

	BOOL							m_Mode;											//贴图模式	
	D3DFORMAT						m_Format;                                       //cube map的格式
	INT								m_MipCount;										//MipMapCount
	INT								m_Size;                                         //cube map的大小

	//静态CubeMap变量
	INT								imagesIdx[6];									//6幅图像的编号，按照(+-)x, (+-)y, (+-)z 的顺序


	//动态CubeMap变量
	LPDIRECT3DSURFACE9				m_pZBuffer;										//用于渲染cubeMap的ZBuffer
	vector<DWORD>					m_ReflectionModels;								//动态cubeMap需要渲染的模型状态列表

	BOOL							isRendering;									//是否正在渲染
	DWORD							m_pCenterModel;									//CubeMap反射的中心
	BOOL                            bCreated;										//是否已经创建
	BOOL							createdTexCount;								//已经创建的贴图数量

	/** 静态立方贴图时，保存6张图像的文件名,按照(+-)x, (+-)y, (+-)z 的顺序.--added by linxq --2009-6-3
	*/
	std::vector<std::string>	m_vecFileName;

public:
	//初始化静态CubeMap
	HRESULT		InitStaticCubeMap();	
	//填充图像到贴图缓冲
	HRESULT		FillImageToCubeTexBuffer(INT faceIndex, DWORD dataSize, BYTE* pData);
	/** 直接使用6张图像设置静态立方贴图.--added by linxq --2009-6-3
	@param vecFileName in:图像文件名称.
	@return : 是否成功.
	*/
	HRESULT		FillImagesToTexture(const std::vector<std::string>& vecFileName);
	/** 按索引获取静态立方贴图的图像文件.--added by linxq --2009-6-3
	@param nIdx in: 有效的索引号.
	@return : 文件名称.
	*/
	const char*	GetImageByIndex(size_t nIdx);

	//初始化动态CubeMap
	HRESULT		InitDynamicCubeMap();

	//增加图像到静态CubeMap
	BOOL AddStaticEnvTexture(DWORD frontImage, DWORD backImage,
							 DWORD leftImage, DWORD rightImage,
							 DWORD topImage, DWORD bottomImage);

	//增加模型到动态CubeMap的渲染模型状态列表
	VOID AddEnvModel(DWORD pModel);	
	VOID removeEnvModel(DWORD modelHandle);

	/** 获取动态立方贴图的模型数量.--added by linxq --2009-6-3
	@return : 映射的模型数量.
	*/
	inline int GetEnvModelCount() { return m_ReflectionModels.size();}
	/** 按索引获取动态立方贴图的模型句柄.--added by linxq --2009-6-3
	@param nIdx in: 有效的索引号.
	@return : 模型句柄.
	*/
	DWORD GetEnvModelHandle(size_t nIdx);
	/** 删除全部反射模型.--added by linxq --2009-6-3
	*/
	void RemoveAllEnvModels();
	
	//释放资源
	VOID Release();	

	//virtual std::string             getTextrueFileName(int index = 0);
	
public:
	// mode : 0 -- 静态 1 -- 动态
	CCubeMap(LPDIRECT3DDEVICE9 pDevice, INT mode, D3DFORMAT format, INT width, INT mipCount);
	~CCubeMap();
};

#endif
