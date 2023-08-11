/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // VGSGUIOverlay.h
* 内容摘要： // Overlyar实现
* 其它说明： // 
* 当前版本： // V2.0
* 作    者： // 傅新波
* 完成日期： // 
* 
* 修改日期      版本号     修改人	      修改内容
* ---------------------------------------------------------------
* 
*
*********************************************************************/


#pragma once

#include "CGuiObject.h"

class VGS_EXPORT COverlay : public CGUIObject
{
public:

	COverlay(LPDIRECT3DDEVICE9 pd3dDevice, const char* sName);
	virtual ~COverlay();

	virtual void Create();
	virtual void Destroy();
	// 释放顶点和面缓冲
	virtual void Invalidate();  

	virtual void Render();

	/** 更新GUI的区域，用于位置、大小、参考坐标有变化时。
	*/
	virtual HRESULT UpdateRect(const RECT& rc);

	void SetOpacity(int nOpactity);
	virtual HRESULT SetRotation(float fAngle);


	/**获得Overlay使用的图像文件
	@param strBaseFile in : 基本贴图文件
	@param strOpacFile in : 透明贴图文件
	*/
	void GetOverlayImageFile(std::string& strBaseFile, std::string& strOpacFile);
	/**设置Overlay使用的图像文件
	@param strBaseFile in : 基本贴图文件
	@param strOpacFile in : 透明贴图文件
	*/
	void SetOverlayImageFile(const std::string& strBaseFile, const std::string& strOpacFile);

protected:
	virtual void InitVertex();
	virtual bool CreateVBuffer();
	virtual bool CreateIBuffer();
protected:
	//!顶点缓冲
	LPDIRECT3DVERTEXBUFFER9		m_VBuffer;
	//!顶点缓冲锁定的内存区域
	VOID*						m_pVertexBuf;
	//!索引缓冲	
	LPDIRECT3DINDEXBUFFER9		m_pIBuffer;		    
	//!索引缓冲锁定的内存区域
	unsigned short*				m_pIndexBuf;	    
	//!初始化的顶点位置 	
	D3DXVECTOR2		m_Point[4];			
	//!顶点数目
	UINT				m_nVertexCount;		
	//!顶点数据 
	CVertex*		m_pVertex;			
	//!面列表
	CFace*			m_pFace;			
	//!面数目 
	UINT				m_nFaceCount;	
};