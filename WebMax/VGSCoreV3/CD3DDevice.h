/////////////////////////////////////////////////////////////////////////
// 名称: CD3DDevice.h
// 描述: 设备检测类，用于检测D3D设备的性能
// 作者: lixin
// 日期: 2006-06-26
/////////////////////////////////////////////////////////////////////////

#ifndef _CD3DDEVICE_H
#define _CD3DDEVICE_H

#include <string>

#include "VGSHeader.h"
#include "CSysFunc.h"

//显示卡信息结构体
typedef struct _ADAPTER
{
	D3DADAPTER_IDENTIFIER9	HardDriveInfo;				//当前显卡硬件驱动信息
	D3DDISPLAYMODE			DisplayMode;				//当前显卡显示模式
	HMONITOR				Moniter;					//与当前D3D设备连接的显示器句柄
	UINT					MultiSampleTypeCnt;			//抗拒齿类型数量
	D3DMULTISAMPLE_TYPE*	pMultiSampleType;			//抗拒齿类型列表
	DWORD*					pMultiSampleQualityLevels;	//
	
	D3DCAPS9				HALCap;						//HAL设备处理能力的描述对象
	D3DCAPS9				REFCap;						//REF设备处理能力描述对象
	UINT					VideoMemSize;				//显存大小
	
    DWORD					MaxTextureBlendStages;		//Maximum number of texture-blending stages supported
    DWORD					MaxSimultaneousTextures;	//Maximum number of textures that can be simultaneously bound to the texture blending stages.
	
	BOOL					IsSupportTexGenSphereMap;	//是否支持球型贴图
	BOOL					IsSupportCUBEMAP;			//当前设备是否支持cube
	BOOL					IsSupportDynamicTexture;	//当前设备是否支持动态贴图
	BOOL                    IsSupportTex64;				//是否支持64位贴图
	BOOL                    IsSupportGPUNormal;			//是否支持GPU生成法线贴图	
	D3DFORMAT				SupportBestBumpFormat;		//支持得最好的凹凸贴图类型
	FLOAT					SupportVS_Version;			//是否支持Vertex Shader, 版本号
	FLOAT					SupportPS_Version;			//是否支持Pixel Shader, 版本号
}_ADAPTER;

//创建D3D设备所需要的基本参数结构体，当系统有多个显卡时，选择并保存当前系统的最佳配置
typedef struct D3D_PARAM
{
	D3DDEVTYPE				DevType;					//开发类型HAL | REF
	DWORD					VertexProcessType;			//顶点处理类型
	D3DMULTISAMPLE_TYPE		MultiSampleType;			//抗锯齿类型2X or 4X?
	DWORD					MultiSampleQuality;			//抗锯齿质量，目前没有使用
	
    DWORD					MaxTextureBlendStages;		//Maximum number of texture-blending stages supported
    DWORD					MaxSimultaneousTextures;	//Maximum number of textures that can be simultaneously bound to the texture blending stages.
	
	BOOL					IsSupportTexGenSphereMap;	//是否支持球型贴图
	BOOL					IsSupportCUBEMAP;			//当前设备是否支持cube
	BOOL					IsSupportDynamicTexture;	//当前设备是否支持动态贴图
	BOOL                    IsSupportTex64;				//是否支持64位贴图
	BOOL                    IsSupportGPUNormal;			//是否支持GPU生成法线贴图
	D3DFORMAT				SupportBestBumpFormat;		//支持得最好的凹凸贴图类型	
	FLOAT					SupportVS_Version;			//是否支持Vertex Shader, 版本号
	FLOAT					SupportPS_Version;			//是否支持Pixel Shader, 版本号
}D3D_PARAM;

//CD3DDevice类----------------------------------------------------------------------
class VGS_EXPORT CD3DDevice  
{
private:
	LPDIRECT3D9					m_pd3d;						//d3d对象
	
public:
	UINT						adapterCnt;					//显卡数量
	_ADAPTER*					pAdapters;					//显卡列表
	
	//得到当前系统的所有显示设备参数
	HRESULT						GetAllAdapterAbility();	
	//得到当前最合适的设备参数
	HRESULT						ChooseBestD3DParam(D3D_PARAM* pD3DParam);
	//确定设备在某种状态下的处理能力
	HRESULT						ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT Format );
	//释放内存和相关资源
	VOID						Release();
	//检测是否支持Bump
	D3DFORMAT					ConfirmBumpUV(D3DCAPS9* pCaps, D3DFORMAT backBufFormat);
	//检测是否支持Cube
	BOOL						ConfirmCubeMap(D3DCAPS9* pCaps);
	//检测是否支持DynamicTexture
	BOOL						ConfirmDynamicTexture(D3DCAPS9* pCaps);
	//检测是否支持硬件法线贴图
    BOOL                        ConfirmGPUNormal(_ADAPTER* pAdapter);
	
	
public:
	CD3DDevice(LPDIRECT3D9 pd3d);
	~CD3DDevice();
	
};

#endif
