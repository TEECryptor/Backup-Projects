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

#include "COverlay.h"
#include "..\csysfunc.h"
#include "..\CVertex.h"
#include "..\CFace.h"
#include "..\CMaterial.h"

COverlay::COverlay(LPDIRECT3DDEVICE9 pd3dDevice, const char* sName)
          :CGUIObject(pd3dDevice, sName)
{
	m_ObjType = VGS_GUIOBJ_TYPE_OVERLAY;

	m_pVertexBuf = NULL;
	m_pIndexBuf = NULL;
	m_pIBuffer = NULL;	
	memset(m_Point, 0, sizeof(D3DXVECTOR2)*4);
	m_nVertexCount = 0;
	m_pVertex = NULL;
	m_pFace = NULL;
	m_nFaceCount = 0;

	InitVertex();
	// 创建对象
	Create();

}
COverlay::~COverlay()
{
	Destroy();
}
void COverlay::Destroy()
{
	Invalidate();	

	//删除模型对象中的面数据，节省内存
	SAFE_DELETE_ARRAY(m_pFace);	
	//删除模型对象中的顶点数据，节省内存
	SAFE_DELETE_ARRAY(m_pVertex);
}
// 释放顶点和面缓冲
void COverlay::Invalidate()
{
	//释放顶点缓冲
	SAFE_RELEASE_D3D(m_VBuffer);	
	//释放索引缓冲
	SAFE_RELEASE_D3D(m_pIBuffer);	


	SAFE_RELEASE_D3D(m_pSelBoxVbuf);
	SAFE_RELEASE_D3D(m_pSelBoxIbuf);	
}

void COverlay::InitVertex()
{
	m_nVertexCount = 4;

	//顶点列表
	m_pVertex = new CVertex[m_nVertexCount];

	m_pVertex[0].index = 0;
	m_pVertex[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVertex[0].diffuse = D3DCOLOR_RGBA(255,255,255,255);
	m_pVertex[0].uv1.u = 0.0f;
	m_pVertex[0].uv1.v = 0.0f;

	m_pVertex[1].index = 1;
	m_pVertex[1].position = D3DXVECTOR3(96.0f, 0.0f, 0.0f);
	m_pVertex[1].diffuse = D3DCOLOR_RGBA(255,255,255,255);
	m_pVertex[1].uv1.u = 1.0f;
	m_pVertex[1].uv1.v = 0.0f;

	m_pVertex[2].index = 2;
	m_pVertex[2].position = D3DXVECTOR3(96.0f, 24.0f, 0.0f);
	m_pVertex[2].diffuse = D3DCOLOR_RGBA(255,255,255,255);
	m_pVertex[2].uv1.u = 1.0f;
	m_pVertex[2].uv1.v = 1.0f;

	m_pVertex[3].index = 3;
	m_pVertex[3].position = D3DXVECTOR3(0.0f, 24.0f, 0.0f);
	m_pVertex[3].diffuse = D3DCOLOR_RGBA(255,255,255,255);
	m_pVertex[3].uv1.u = 0.0f;
	m_pVertex[3].uv1.v = 1.0f;

	m_Point[0].x = m_pVertex[0].position.x;
	m_Point[0].y = m_pVertex[0].position.y;

	m_Point[1].x = m_pVertex[1].position.x;
	m_Point[1].y = m_pVertex[1].position.y;

	m_Point[2].x = m_pVertex[2].position.x;
	m_Point[2].y = m_pVertex[2].position.y;

	m_Point[3].x = m_pVertex[3].position.x;
	m_Point[3].y = m_pVertex[3].position.y;

	//顶点索引表
	m_nFaceCount = 2;
	m_pFace = new CFace[m_nFaceCount];

	m_pFace[0].vertices0 = &(m_pVertex[0]);
	m_pFace[0].vertices1 = &(m_pVertex[2]);
	m_pFace[0].vertices2 = &(m_pVertex[1]);

	m_pFace[1].vertices0 = &(m_pVertex[0]);
	m_pFace[1].vertices1 = &(m_pVertex[3]);
	m_pFace[1].vertices2 = &(m_pVertex[2]);
}
void COverlay::Create()
{
	//创建顶点缓冲和索引缓冲
	CreateVBuffer();
	CreateIBuffer();
}

//-----------------------------------------------------------------------------
// 方法名: CreateVBuffer()
// 参  数: 
// 描  述: 创建当前Overlay的顶点缓冲
//-----------------------------------------------------------------------------
bool COverlay::CreateVBuffer()
{
	if( FAILED( m_pd3dDevice->CreateVertexBuffer( m_nVertexCount * m_FVFSize, 
		0, 
		m_FVF, 
		D3DPOOL_MANAGED,	
		&(m_VBuffer), 
		NULL ) ) )
	{

		return false;
	}

	//锁定内存
	if( FAILED(m_VBuffer->Lock(0, m_nVertexCount * m_FVFSize, (VOID**)&(m_pVertexBuf), D3DLOCK_DISCARD) ))
		return false;

	//逐个顶点填充数据
	for (UINT i = 0; i< m_nVertexCount; i++)
	{
		//填充坐标数据到顶点缓冲
		((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].position = m_pVertex[i].position;
		//		((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].diffuse  = m_pVertex[i].diffuse;
		((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].rhw      = 1.0f;
		((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].su       = m_pVertex[i].uv1.u;
		((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].sv       = m_pVertex[i].uv1.v;
	}
	//Unlock顶点缓冲
	m_VBuffer->Unlock();

	return true;
}
//-----------------------------------------------------------------------------
// 方法名: CreateIBuffer()
// 参  数: 
// 描  述: 创建当前Overlay的索引缓冲
//-----------------------------------------------------------------------------
bool COverlay::CreateIBuffer()
{
	//建立当前模型的索引缓冲
	if( FAILED(m_pd3dDevice->CreateIndexBuffer(m_nFaceCount *3 * sizeof(UINT),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIBuffer, NULL) ) )
	{
		return FALSE;
	}

	//锁定内存
	if( FAILED(m_pIBuffer->Lock(0, m_nFaceCount *3* sizeof(UINT), (VOID**)&m_pIndexBuf, 0) ))
	{
		return FALSE;
	}

	//逐个面填充索引数据
	for (UINT i =0; i < m_nFaceCount; i++)
	{
		m_pIndexBuf[3 * i]     = m_pFace[i].vertices0->index;
		m_pIndexBuf[3 * i + 1] = m_pFace[i].vertices1->index;
		m_pIndexBuf[3 * i + 2] = m_pFace[i].vertices2->index;
	}
	//Unlock索引缓冲
	m_pIBuffer->Unlock();
	return true;
}

void COverlay::Render()
{
	if (!m_bVisible) 
		return;
	// 设置材质，渲染纹理
	if (m_pMtrl)
		m_pMtrl->SetToDevice(m_pd3dDevice, 0);
	m_pd3dDevice->SetStreamSource( 0, m_VBuffer, 0, m_FVFSize );
	//设置FVF
	m_pd3dDevice->SetFVF(m_FVF); //DX9为SetFVF,DX8用这个函数	
	//设置IB
	m_pd3dDevice->SetIndices(m_pIBuffer);

	//改变索引缓冲区的面的数量将会较大幅度的提升运算速度**************
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		m_nVertexCount, //当前模型的顶点数
		0, //当前模型的索引缓冲区起点
		m_nFaceCount); //当前模型的面数
}
HRESULT COverlay::UpdateRect(const RECT& rc)
{
	if (!m_VBuffer || !m_pIBuffer)
		return E_FAIL;
	//新的位置
	m_Point[0].x = m_pVertex[0].position.x = (FLOAT)rc.left;
	m_Point[0].y = m_pVertex[0].position.y = (FLOAT)rc.top;

	m_Point[1].x = m_pVertex[1].position.x = (FLOAT)rc.right;
	m_Point[1].y = m_pVertex[1].position.y = (FLOAT)rc.top;

	m_Point[2].x = m_pVertex[2].position.x = (FLOAT)rc.right;
	m_Point[2].y = m_pVertex[2].position.y = (FLOAT)rc.bottom;

	m_Point[3].x = m_pVertex[3].position.x = (FLOAT)rc.left;
	m_Point[3].y = m_pVertex[3].position.y = (FLOAT)rc.bottom;

	//保持之前的变换
	// 	SetRotation(m_fAngle);//???——未明白用意

	//锁定内存
	if( FAILED(m_VBuffer->Lock(0, m_nVertexCount * m_FVFSize, (VOID**)&(m_pVertexBuf), D3DLOCK_DISCARD) ))
		return E_FAIL;

	//填充坐标数据到顶点缓冲
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[0].position = m_pVertex[0].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[1].position = m_pVertex[1].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[2].position = m_pVertex[2].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[3].position = m_pVertex[3].position;

	//Unlock顶点缓冲
	m_VBuffer->Unlock();

	return S_OK;

}

void COverlay::SetOpacity(int nOpactity)	
{
	m_nOpacity = nOpactity;	
	int nTmp = nOpactity;
	if (nOpactity >= 100)
		nTmp = 255;
	else
		nTmp = nOpactity * 255/100;
	//锁定内存
	if( FAILED(m_VBuffer->Lock(0, m_nVertexCount * m_FVFSize, (VOID**)&(m_pVertexBuf), D3DLOCK_DISCARD) ))
		return /*E_FAIL*/;

	//逐个顶点填充数据
	/*
	for (UINT i=0; i< m_nVertexCount; i++)
	{
	DWORD color = ((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].diffuse;
	INT red =  (color >> 16) & 0xff;
	INT green =  (color >> 8) & 0xff;
	INT blue =  color & 0xff;
	//填充坐标数据到顶点缓冲
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].diffuse = D3DCOLOR_RGBA(red, green, blue, nTmp);							
	m_pVertex[i].diffuse = ((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].diffuse;
	}
	*/

	//Unlock顶点缓冲
	m_VBuffer->Unlock();
}
HRESULT COverlay::SetRotation(float fAngle)
{
	m_fAngle = fAngle;
	if (0.0f == m_fAngle)
		return S_OK;
	//得到现有的四个顶点坐标
	D3DXVECTOR2 p0 = m_Point[0];
	D3DXVECTOR2 p1 = m_Point[1];
	D3DXVECTOR2 p2 = m_Point[2];
	D3DXVECTOR2 p3 = m_Point[3];

	//先找到旋转的中心点
	D3DXVECTOR2 center = (p2 + p0)/2;

	D3DXMATRIX transMat;
	D3DXMATRIX rosMat;
	D3DXMATRIX allMat;
	//先移到0点
	D3DXMatrixTranslation(&transMat, -center.x, -center.y, 0.0f);
	//旋转
	D3DXMatrixRotationZ(&rosMat, m_fAngle);
	D3DXMatrixMultiply(&allMat, &transMat, &rosMat);
	//移回来
	D3DXMatrixTranslation(&transMat, center.x, center.y, 0.0f);
	D3DXMatrixMultiply(&allMat, &allMat, &transMat);
	//计算新点
	D3DXVec2TransformCoord(&p0, &p0, &allMat);
	D3DXVec2TransformCoord(&p1, &p1, &allMat);
	D3DXVec2TransformCoord(&p2, &p2, &allMat);
	D3DXVec2TransformCoord(&p3, &p3, &allMat);

	m_pVertex[0].position.x = p0.x;
	m_pVertex[0].position.y = p0.y;

	m_pVertex[1].position.x = p1.x;
	m_pVertex[1].position.y = p1.y;

	m_pVertex[2].position.x = p2.x;
	m_pVertex[2].position.y = p2.y;

	m_pVertex[3].position.x = p3.x;
	m_pVertex[3].position.y = p3.y;

	//锁定内存
	if( FAILED(m_VBuffer->Lock(0, m_nVertexCount * m_FVFSize, (VOID**)&(m_pVertexBuf), D3DLOCK_DISCARD) ))
		return E_FAIL;

	//填充坐标数据到顶点缓冲
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[0].position = m_pVertex[0].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[1].position = m_pVertex[1].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[2].position = m_pVertex[2].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[3].position = m_pVertex[3].position;

	//Unlock顶点缓冲
	m_VBuffer->Unlock();

	return S_OK;
}

void COverlay::GetOverlayImageFile(std::string& strBaseFile, std::string& strOpacFile)
{
	if (m_pMtrl && m_pMtrl->getTexureLayerCount() > 0)
	{
		CTextureLayer* pTxt = m_pMtrl->getTextureLayerByIndex(0);
		strBaseFile = pTxt->pTexture->getTextrueFileName();
		if (m_pMtrl->hasTransparentTexture())
		{
			pTxt = m_pMtrl->getTextureLayerByIndex(1);
			strOpacFile = pTxt->pTexture->getTextrueFileName();
		}
	}			
}

void COverlay::SetOverlayImageFile(const std::string& strBaseFile, const std::string& strOpacFile)
{
	if (m_pMtrl)
	{
		CTextureLayer* pTxt = 0;
		if (m_pMtrl->hasTransparentTexture())
		{
			pTxt = m_pMtrl->getTextureLayerByIndex(1);
			if (pTxt)
				m_pMtrl->clearTextureLayerByHandle(pTxt);//当前该方法未实现，所以获取的结果是不正确的，需要再测试——linxq 09-4-13
		}
		if (m_pMtrl->getTexureLayerCount() > 0)
		{
			pTxt = m_pMtrl->getTextureLayerByIndex(0);
			if (pTxt)
				m_pMtrl->clearTextureLayerByHandle(pTxt);
		}
		if (strBaseFile.length() > 0)
			m_pMtrl->createTextureLayer(strBaseFile, m_pMtrl->getName() + "_txt0", 0);
		if (strOpacFile.length() > 0)
		{
			m_pMtrl->createOpacityTextureLayer(strOpacFile, m_pMtrl->getName() + "txt1", 0);
		}
	}			
}