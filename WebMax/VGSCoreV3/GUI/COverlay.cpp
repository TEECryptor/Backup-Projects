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
	// ��������
	Create();

}
COverlay::~COverlay()
{
	Destroy();
}
void COverlay::Destroy()
{
	Invalidate();	

	//ɾ��ģ�Ͷ����е������ݣ���ʡ�ڴ�
	SAFE_DELETE_ARRAY(m_pFace);	
	//ɾ��ģ�Ͷ����еĶ������ݣ���ʡ�ڴ�
	SAFE_DELETE_ARRAY(m_pVertex);
}
// �ͷŶ�����滺��
void COverlay::Invalidate()
{
	//�ͷŶ��㻺��
	SAFE_RELEASE_D3D(m_VBuffer);	
	//�ͷ���������
	SAFE_RELEASE_D3D(m_pIBuffer);	


	SAFE_RELEASE_D3D(m_pSelBoxVbuf);
	SAFE_RELEASE_D3D(m_pSelBoxIbuf);	
}

void COverlay::InitVertex()
{
	m_nVertexCount = 4;

	//�����б�
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

	//����������
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
	//�������㻺�����������
	CreateVBuffer();
	CreateIBuffer();
}

//-----------------------------------------------------------------------------
// ������: CreateVBuffer()
// ��  ��: 
// ��  ��: ������ǰOverlay�Ķ��㻺��
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

	//�����ڴ�
	if( FAILED(m_VBuffer->Lock(0, m_nVertexCount * m_FVFSize, (VOID**)&(m_pVertexBuf), D3DLOCK_DISCARD) ))
		return false;

	//��������������
	for (UINT i = 0; i< m_nVertexCount; i++)
	{
		//����������ݵ����㻺��
		((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].position = m_pVertex[i].position;
		//		((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].diffuse  = m_pVertex[i].diffuse;
		((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].rhw      = 1.0f;
		((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].su       = m_pVertex[i].uv1.u;
		((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].sv       = m_pVertex[i].uv1.v;
	}
	//Unlock���㻺��
	m_VBuffer->Unlock();

	return true;
}
//-----------------------------------------------------------------------------
// ������: CreateIBuffer()
// ��  ��: 
// ��  ��: ������ǰOverlay����������
//-----------------------------------------------------------------------------
bool COverlay::CreateIBuffer()
{
	//������ǰģ�͵���������
	if( FAILED(m_pd3dDevice->CreateIndexBuffer(m_nFaceCount *3 * sizeof(UINT),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIBuffer, NULL) ) )
	{
		return FALSE;
	}

	//�����ڴ�
	if( FAILED(m_pIBuffer->Lock(0, m_nFaceCount *3* sizeof(UINT), (VOID**)&m_pIndexBuf, 0) ))
	{
		return FALSE;
	}

	//����������������
	for (UINT i =0; i < m_nFaceCount; i++)
	{
		m_pIndexBuf[3 * i]     = m_pFace[i].vertices0->index;
		m_pIndexBuf[3 * i + 1] = m_pFace[i].vertices1->index;
		m_pIndexBuf[3 * i + 2] = m_pFace[i].vertices2->index;
	}
	//Unlock��������
	m_pIBuffer->Unlock();
	return true;
}

void COverlay::Render()
{
	if (!m_bVisible) 
		return;
	// ���ò��ʣ���Ⱦ����
	if (m_pMtrl)
		m_pMtrl->SetToDevice(m_pd3dDevice, 0);
	m_pd3dDevice->SetStreamSource( 0, m_VBuffer, 0, m_FVFSize );
	//����FVF
	m_pd3dDevice->SetFVF(m_FVF); //DX9ΪSetFVF,DX8���������	
	//����IB
	m_pd3dDevice->SetIndices(m_pIBuffer);

	//�ı������������������������ϴ���ȵ����������ٶ�**************
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		m_nVertexCount, //��ǰģ�͵Ķ�����
		0, //��ǰģ�͵��������������
		m_nFaceCount); //��ǰģ�͵�����
}
HRESULT COverlay::UpdateRect(const RECT& rc)
{
	if (!m_VBuffer || !m_pIBuffer)
		return E_FAIL;
	//�µ�λ��
	m_Point[0].x = m_pVertex[0].position.x = (FLOAT)rc.left;
	m_Point[0].y = m_pVertex[0].position.y = (FLOAT)rc.top;

	m_Point[1].x = m_pVertex[1].position.x = (FLOAT)rc.right;
	m_Point[1].y = m_pVertex[1].position.y = (FLOAT)rc.top;

	m_Point[2].x = m_pVertex[2].position.x = (FLOAT)rc.right;
	m_Point[2].y = m_pVertex[2].position.y = (FLOAT)rc.bottom;

	m_Point[3].x = m_pVertex[3].position.x = (FLOAT)rc.left;
	m_Point[3].y = m_pVertex[3].position.y = (FLOAT)rc.bottom;

	//����֮ǰ�ı任
	// 	SetRotation(m_fAngle);//???����δ��������

	//�����ڴ�
	if( FAILED(m_VBuffer->Lock(0, m_nVertexCount * m_FVFSize, (VOID**)&(m_pVertexBuf), D3DLOCK_DISCARD) ))
		return E_FAIL;

	//����������ݵ����㻺��
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[0].position = m_pVertex[0].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[1].position = m_pVertex[1].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[2].position = m_pVertex[2].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[3].position = m_pVertex[3].position;

	//Unlock���㻺��
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
	//�����ڴ�
	if( FAILED(m_VBuffer->Lock(0, m_nVertexCount * m_FVFSize, (VOID**)&(m_pVertexBuf), D3DLOCK_DISCARD) ))
		return /*E_FAIL*/;

	//��������������
	/*
	for (UINT i=0; i< m_nVertexCount; i++)
	{
	DWORD color = ((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].diffuse;
	INT red =  (color >> 16) & 0xff;
	INT green =  (color >> 8) & 0xff;
	INT blue =  color & 0xff;
	//����������ݵ����㻺��
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].diffuse = D3DCOLOR_RGBA(red, green, blue, nTmp);							
	m_pVertex[i].diffuse = ((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[i].diffuse;
	}
	*/

	//Unlock���㻺��
	m_VBuffer->Unlock();
}
HRESULT COverlay::SetRotation(float fAngle)
{
	m_fAngle = fAngle;
	if (0.0f == m_fAngle)
		return S_OK;
	//�õ����е��ĸ���������
	D3DXVECTOR2 p0 = m_Point[0];
	D3DXVECTOR2 p1 = m_Point[1];
	D3DXVECTOR2 p2 = m_Point[2];
	D3DXVECTOR2 p3 = m_Point[3];

	//���ҵ���ת�����ĵ�
	D3DXVECTOR2 center = (p2 + p0)/2;

	D3DXMATRIX transMat;
	D3DXMATRIX rosMat;
	D3DXMATRIX allMat;
	//���Ƶ�0��
	D3DXMatrixTranslation(&transMat, -center.x, -center.y, 0.0f);
	//��ת
	D3DXMatrixRotationZ(&rosMat, m_fAngle);
	D3DXMatrixMultiply(&allMat, &transMat, &rosMat);
	//�ƻ���
	D3DXMatrixTranslation(&transMat, center.x, center.y, 0.0f);
	D3DXMatrixMultiply(&allMat, &allMat, &transMat);
	//�����µ�
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

	//�����ڴ�
	if( FAILED(m_VBuffer->Lock(0, m_nVertexCount * m_FVFSize, (VOID**)&(m_pVertexBuf), D3DLOCK_DISCARD) ))
		return E_FAIL;

	//����������ݵ����㻺��
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[0].position = m_pVertex[0].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[1].position = m_pVertex[1].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[2].position = m_pVertex[2].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[3].position = m_pVertex[3].position;

	//Unlock���㻺��
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
				m_pMtrl->clearTextureLayerByHandle(pTxt);//��ǰ�÷���δʵ�֣����Ի�ȡ�Ľ���ǲ���ȷ�ģ���Ҫ�ٲ��ԡ���linxq 09-4-13
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