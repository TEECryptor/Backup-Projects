// CDynamicTex.h: interface for the CDynamicTex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDYNAMICTEX_H__73DFEEE0_2784_43D8_B023_02854EE17CBC__INCLUDED_)
#define AFX_CDYNAMICTEX_H__73DFEEE0_2784_43D8_B023_02854EE17CBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"
#include "CTexture.h"

#include <vector>
#include <string>
#include <algorithm>

class VGS_EXPORT CDynamicTex : public CTexture
{
public:
	CDynamicTex(LPDIRECT3DDEVICE9 pd3dDevice,int nWidth, int nHeight, D3DFORMAT fmt, int framecount);
	~CDynamicTex();

public:
	/** D3D Device对象
	*/
	LPDIRECT3DDEVICE9				m_pd3dDevice;
	/** 动态贴图当前使用的贴图缓存，在显存中
	*/
	LPDIRECT3DTEXTURE9		m_pCurTexture;
	/** 动态贴图的贴图队列，在内存中
	*/
	std::vector<LPDIRECT3DTEXTURE9>	m_vecTextures;
	/** 图像文件的列表.
	*/
	std::vector<std::string> m_vecFileName;

	//帧贴图的图像数据索引
	std::vector<int>		imagesIdx;          
	//动态贴图名称
	std::string				name;	



	/** 是否正在播放
	*/
	bool	m_bPlay;
	/** 是否循环播放
	*/
	bool	m_bLoop;
	/** 动态贴图播放的帧率，即播放周期/贴图总数.区别于渲染FPS.单位毫秒.
	*/
	DWORD	m_nFPS;
	/** 播放周期，单位秒.
	*/
	float	m_fDuration;

	/** 已经创建的贴图数量
	*/
	int		m_nCreatedCount;
	
	/** 动态贴图的帧数
	*/
	int		m_frameCnt;		

	/** 当前播放的帧
	*/
	int		m_nCurFrame;
	/** 
	*/
	int		m_nLstFrame;
public:

	/** 填充图像到贴图缓冲
	*/
	HRESULT							FillImageToTextureBuffer(size_t imageIndex, DWORD dataSize, BYTE* pData);

	//改变贴图到某一帧
	HRESULT							ChangeFrameTexture(INT frameIndex);
	//更新贴图
	void							Update();
	//播放
	void							Play();
	//暂停
	void							Pause();
	//停止
	void							Stop();
	//释放贴图缓冲
	void							Invalidate();
	//重置
	HRESULT							Restore();
	//释放资源
	void							Release();		

	inline LPDIRECT3DTEXTURE9	GetCurTexture() { return m_pCurTexture;};
	
	/** 获取动态贴图的播放周期，单位为秒.
	*/
	inline float		GetDuration() { return m_fDuration;}
	/** 设置动态贴图的播放周期，单位为秒.
	*/
	inline void	SetDuration(float fDuration) { m_fDuration = fDuration; m_nFPS = (int)(1000 * fDuration/GetImageCount());}

	/** 按索引获取贴图文件名
	*/
	std::string GetFileName(size_t nIdx);
	/** 获取贴图数量
	*/
	inline int	GetImageCount() {return m_vecTextures.size(); }
	/** 按索引获修改贴图某张图像
	@param nIdx in: 索引，必须是有效值.
	@param strFileName in:新的图像文件名.
	*/
	HRESULT	ChangeImage(size_t nIdx, const std::string & strFileName);
	/** 在最后添加一张图像
	@param strFileName in:图像文件名.
	*/
	HRESULT	AddImage(const std::string & strFileName);
	/** 按索引删除图某张图像
	@param nIdx in: 索引，必须是有效值.
	*/
	bool DeleteImage(size_t nIdx);
	/** 清空所有图像.
	*/
	void ClearImages();
protected:
	/** 初始化动态贴图对象
	*/
	void	Init();


};

#endif // !defined(AFX_CDYNAMICTEX_H__73DFEEE0_2784_43D8_B023_02854EE17CBC__INCLUDED_)
