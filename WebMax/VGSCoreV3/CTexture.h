/////////////////////////////////////////////////////////////////////////
// 名称: CTexture类型定义--场景中的贴图对象
// 描述: CTexture类型定义了贴图的各种属性
// 作者: lixin	
// 日期: 2005-12-20
////////////////////////////////////////////////////////////////////////

#ifndef _CTEXTURE_H
#define _CTEXTURE_H

#include "VGSHeader.h"
#include "ResourceManager.h"
#include "CSysFunc.h"

//##ModelId=47EBC72E03C8
class VGS_EXPORT CTexture : public CResource
{	
public:
	//##ModelId=47EBC72E03C9
	INT						index;

	//##ModelId=47EBC72E03CB
	CHAR					path[224];				//贴图路径
	//##ModelId=47EBC72E03CC
	// CHAR					pathAndName[256];		// 贴图名称和路径
	std::string             m_sFilePathandName;     // 贴图文件名称
	//##ModelId=47EBC72E03CD
	int					width;					//贴图宽度
	//##ModelId=47EBC72E03CE
	int					height;					//贴图高度
	//##ModelId=47EBC72E03CF
	BOOL					bCreated;				//贴图是否已经创建，用于渲染队列
	UINT					MipCount;				//贴图的MipMap数量
	//##ModelId=47EBC72E03D1
	D3DFORMAT				m_format;						//每个像素所占的字节数
	//##ModelId=47EBC72E03D6
	_D3DXIMAGE_INFO			imageInfo;				//图像文件格式
	//##ModelId=47EBC72E03DA
	UINT					ZipQuality;				//设置压缩为N3D时的压缩质量0-100
	BOOL					withAlpha;				//包含Alpha通道
	BOOL                    isOpacity;              //是否是透明贴图
	BOOL                    isDynamic;              //是否是动态贴图
	//UINT					dataSize;				//图像文件大小
	//BYTE*					data;					//图像数据
    UINT                    imageIdx;               //图片数据索引

	//##ModelId=47EBC72E03DD
	INT						brightness;				//亮度
	//##ModelId=47EBC72E03DE
	INT						contrast;				//对比度

	//##ModelId=47EBC72E03DF
	BOOL					bAlphaTest;				//是否Alpha测试
	//##ModelId=47EBC72E03E0
	BOOL					bAlphaBlend;			//是否Alpha混色

	//##ModelId=47EBC72E03E2
	LPDIRECT3DTEXTURE9		m_textureBuf;				//D3D9贴图缓冲
	//##ModelId=47EBC72E03E6
	BYTE*					buffer;					//缓冲数据

	BOOL					bIsOverlayTexture;		//是否用于Overlay

	BOOL					bUseByScene;			//是否被场景所用
protected:
	VGS_TextureType			m_textureType;

public:
	//设置贴图的alpha值
	VOID					SetOpacity(INT opacity);	
	//释放贴图资源
	virtual VOID			Release();
	//只释放D3D贴图表面
	virtual VOID			ReleaseBufferOnly();

	virtual std::string             getTextrueFileName(int index = 0);

	/** 改变纹理贴图的图像文件.
	@param strFileName in: 新的图像文件名称.
	*/
	HRESULT ChangeImage(const std::string & strFileName);
	/** 改变透明贴图的图像文件.--added by linxq --2009-6-3
	@param strFileName in: 新的图像文件名称.
	*/
	HRESULT ChangeTransparentImage(const std::string & strFileName);

	VGS_TextureType GetType() const { return m_textureType; }

public:
	CTexture();
	virtual ~CTexture();

};

#endif
