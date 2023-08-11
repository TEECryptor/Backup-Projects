/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // VGSGUIOverlay.h
* ����ժҪ�� // Overlyarʵ��
* ����˵���� // 
* ��ǰ�汾�� // V2.0
* ��    �ߣ� // ���²�
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
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
	// �ͷŶ�����滺��
	virtual void Invalidate();  

	virtual void Render();

	/** ����GUI����������λ�á���С���ο������б仯ʱ��
	*/
	virtual HRESULT UpdateRect(const RECT& rc);

	void SetOpacity(int nOpactity);
	virtual HRESULT SetRotation(float fAngle);


	/**���Overlayʹ�õ�ͼ���ļ�
	@param strBaseFile in : ������ͼ�ļ�
	@param strOpacFile in : ͸����ͼ�ļ�
	*/
	void GetOverlayImageFile(std::string& strBaseFile, std::string& strOpacFile);
	/**����Overlayʹ�õ�ͼ���ļ�
	@param strBaseFile in : ������ͼ�ļ�
	@param strOpacFile in : ͸����ͼ�ļ�
	*/
	void SetOverlayImageFile(const std::string& strBaseFile, const std::string& strOpacFile);

protected:
	virtual void InitVertex();
	virtual bool CreateVBuffer();
	virtual bool CreateIBuffer();
protected:
	//!���㻺��
	LPDIRECT3DVERTEXBUFFER9		m_VBuffer;
	//!���㻺���������ڴ�����
	VOID*						m_pVertexBuf;
	//!��������	
	LPDIRECT3DINDEXBUFFER9		m_pIBuffer;		    
	//!���������������ڴ�����
	unsigned short*				m_pIndexBuf;	    
	//!��ʼ���Ķ���λ�� 	
	D3DXVECTOR2		m_Point[4];			
	//!������Ŀ
	UINT				m_nVertexCount;		
	//!�������� 
	CVertex*		m_pVertex;			
	//!���б�
	CFace*			m_pFace;			
	//!����Ŀ 
	UINT				m_nFaceCount;	
};