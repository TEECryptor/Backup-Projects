/////////////////////////////////////////////////////////////////////////
// ��  ��: CRenderBoundBox.cpp
// ��  ��: ģ�͵���Ⱦ��Χ�ж����壬������ʾ����İ�Χ��
// ��  ��: baojiahui
// ��  ��: 2006-12-27
/////////////////////////////////////////////////////////////////////////

#include "CRenderBoundBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC7460001
CRenderBoundBox::CRenderBoundBox(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_pRBBoxVertices = NULL;
    m_pRBBoxIndices= NULL;
	
	
	
	//���ð�Χ�е���ɫ
	RBBoxColor = D3DCOLOR_RGBA(255, 255, 255, 255);	
	//���ð�Χ�ж���ĸ���
	m_RBBoxVectCnt = 32;
	//���ð�Χ�бߵĸ���
	m_RBBoxLineCnt = 24;

	if (FAILED( CreateRBBoxVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("������Χ�ж��㻺�����"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);
	if (FAILED(CreateRBBoxIndexBuf() ))
		MessageBox(GetActiveWindow(), TEXT("������Χ�������������"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);

	
}

//##ModelId=47EBC7460003
CRenderBoundBox::~CRenderBoundBox()
{
	
}


//-----------------------------------------------------------------------------
// ������: InitLineVertex()
// ��  ��: 
// ��  ;: ��ʼ�����߶���
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503C5
VOID CRenderBoundBox::InitRBBoxVertex()
{
	//box��32������
	for(UINT i=0;i<32;i++){
    	m_pRBBoxVertices[i].position = D3DXVECTOR3(1.0f,1.0f,1.0f);
	    m_pRBBoxVertices[i].diffuse = RBBoxColor;
	}
}

//-----------------------------------------------------------------------------
// ������: CreateRBBoxIndexBuf()
// ��  ��: 
// ��  ;: ������Χ�е���������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503C9
HRESULT CRenderBoundBox::CreateRBBoxIndexBuf()
{	
	//������ǰģ�͵���������
	if( FAILED( m_pd3dDevice ->CreateIndexBuffer(m_RBBoxLineCnt * 2 * sizeof(UINT),
		0,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_RBBoxIBuffer, NULL) ) )
	{
		return E_FAIL;
	}
	
	//�����ڴ�
	if( FAILED(m_RBBoxIBuffer->Lock(0, m_RBBoxLineCnt *2* sizeof(UINT), (VOID**)&m_pRBBoxIndices, 0) ))
	{
		return E_FAIL;
	}
	//������������
	UINT RBBoxindexData[] ={
		    0,1,
			0,2,
			0,3,
			4,5,
			4,6,
			4,7,
			8,9,
            8,10,
			8,11,
			12,13,
			12,14,
			12,15,
			16,17,
			16,18,
			16,19,
			20,21,
			20,22,
			20,23,
			24,25,
			24,26,
			24,27,
			28,29,
			28,30,
			28,31
	};
	//����������������
	for(UINT i=0; i<sizeof(RBBoxindexData)/sizeof(UINT); i++)
	{
		m_pRBBoxIndices[i]=RBBoxindexData[i];
	}
	
	//Unlock��������
	m_RBBoxIBuffer->Unlock();	
	
	return S_OK;
}
//-----------------------------------------------------------------------------
// ������: CreateRBBoxVertexBuf()
// ��  ��: 
// ��  ;: ������Χ�ж��㻺��
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503C8
HRESULT CRenderBoundBox::CreateRBBoxVertexBuf()
{
	//crate vertex buffer obj for axis line
	if( FAILED( m_pd3dDevice->CreateVertexBuffer(m_RBBoxVectCnt * sizeof(VERTEXDECL_POSCOLOR),
		0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_RBBoxVBuffer, NULL) ) )
	{
		return E_FAIL;
	}
	
	//�����ڴ�
	if( FAILED(m_RBBoxVBuffer->Lock(0, m_RBBoxVectCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pRBBoxVertices, 0) ))
	{
		return E_FAIL;
	}
	
	//��ʼ���ᶥ��
	InitRBBoxVertex();
	
	//Unlock���㻺��
	m_RBBoxVBuffer->Unlock();
	
	return S_OK;
}


//-----------------------------------------------------------------------------
// ������: DrawRBBox()
// ��  ��: 
// ��  ;: �Ѱ�Χ�л�����
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503CA
VOID CRenderBoundBox::DrawRBBox()
{
	//��Ⱦģ��
	m_pd3dDevice->SetStreamSource( 0, m_RBBoxVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//����FVF
	m_pd3dDevice->SetFVF(FVF_POSCOLOR); //DX9ΪSetFVF,DX8���������
	//����IB
	m_pd3dDevice->SetIndices(m_RBBoxIBuffer);
	//�ı������������������������ϴ���ȵ����������ٶ�**************
	   m_pd3dDevice->DrawIndexedPrimitive(D3DPT_LINELIST , 0, 0,
		   m_RBBoxVectCnt, //��ǰģ�͵Ķ�����
		   0, //��ǰģ�͵��������������
		   m_RBBoxLineCnt); //��ǰģ�͵�����
}



//-----------------------------------------------------------------------------
// ������: Draw()
// ��  ��: 
// ��  ;: ������ϵ������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503CB
VOID CRenderBoundBox::Draw()
{
	//�����Χ�е��ƶ�����ת
	// m_pd3dDevice->SetTransform( D3DTS_WORLD, &transform.worldMatrix);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &transform.matrix);
	DrawRBBox();
	
}
//-----------------------------------------------------------------------------
// ������: SetRBBoxVertex()
// ��  ��: BoundingBox bBox --��Χ�е�minCorner��maxCorner
// ��  ;: ����ģ�͵İ�Χ�еĶ���
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503DE
HRESULT CRenderBoundBox::SetRBBoxVertex(D3DXVECTOR3 minCorner, D3DXVECTOR3 maxCorner)
{
  	//�����ڴ�
	if( FAILED(m_RBBoxVBuffer->Lock(0, m_RBBoxVectCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pRBBoxVertices, 0) ))
	{
		return E_FAIL;
	}
	
	//���ö���
	m_pRBBoxVertices[0].position = D3DXVECTOR3(minCorner.x,minCorner.y,minCorner.z);
	m_pRBBoxVertices[1].position = D3DXVECTOR3(minCorner.x+(maxCorner.x-minCorner.x)/4.0f,minCorner.y,minCorner.z);
	m_pRBBoxVertices[2].position = D3DXVECTOR3(minCorner.x,minCorner.y+(maxCorner.y-minCorner.y)/4.0f,minCorner.z);
	m_pRBBoxVertices[3].position = D3DXVECTOR3(minCorner.x,minCorner.y,minCorner.z+(maxCorner.z-minCorner.z)/4.0f);
	
	m_pRBBoxVertices[4].position = D3DXVECTOR3(minCorner.x,maxCorner.y,minCorner.z);
    m_pRBBoxVertices[5].position = D3DXVECTOR3(minCorner.x+(maxCorner.x-minCorner.x)/4.0f,maxCorner.y,minCorner.z);
	m_pRBBoxVertices[6].position = D3DXVECTOR3(minCorner.x,maxCorner.y-(maxCorner.y-minCorner.y)/4.0f,minCorner.z);
    m_pRBBoxVertices[7].position = D3DXVECTOR3(minCorner.x,maxCorner.y,minCorner.z+(maxCorner.z-minCorner.z)/4.0f);

	m_pRBBoxVertices[8].position = D3DXVECTOR3(maxCorner.x,maxCorner.y,minCorner.z);
	m_pRBBoxVertices[9].position = D3DXVECTOR3(maxCorner.x-(maxCorner.x-minCorner.x)/4.0f,maxCorner.y,minCorner.z);
	m_pRBBoxVertices[10].position = D3DXVECTOR3(maxCorner.x,maxCorner.y-(maxCorner.y-minCorner.y)/4.0f,minCorner.z);
    m_pRBBoxVertices[11].position = D3DXVECTOR3(maxCorner.x,maxCorner.y,minCorner.z+(maxCorner.z-minCorner.z)/4.0f);
	
	m_pRBBoxVertices[12].position = D3DXVECTOR3(maxCorner.x,minCorner.y,minCorner.z);
	m_pRBBoxVertices[13].position = D3DXVECTOR3(maxCorner.x-(maxCorner.x-minCorner.x)/4.0f,minCorner.y,minCorner.z);
	m_pRBBoxVertices[14].position = D3DXVECTOR3(maxCorner.x,minCorner.y+(maxCorner.y-minCorner.y)/4.0f,minCorner.z);
    m_pRBBoxVertices[15].position = D3DXVECTOR3(maxCorner.x,minCorner.y,minCorner.z+(maxCorner.z-minCorner.z)/4.0f);

	m_pRBBoxVertices[16].position = D3DXVECTOR3(minCorner.x,minCorner.y,maxCorner.z);
	m_pRBBoxVertices[17].position = D3DXVECTOR3(minCorner.x+(maxCorner.x-minCorner.x)/4.0f,minCorner.y,maxCorner.z);
	m_pRBBoxVertices[18].position = D3DXVECTOR3(minCorner.x,minCorner.y+(maxCorner.y-minCorner.y)/4.0f,maxCorner.z);
	m_pRBBoxVertices[19].position = D3DXVECTOR3(minCorner.x,minCorner.y,maxCorner.z-(maxCorner.z-minCorner.z)/4.0f);

	m_pRBBoxVertices[20].position = D3DXVECTOR3(minCorner.x,maxCorner.y,maxCorner.z);
    m_pRBBoxVertices[21].position = D3DXVECTOR3(minCorner.x+(maxCorner.x-minCorner.x)/4.0f,maxCorner.y,maxCorner.z);
    m_pRBBoxVertices[22].position = D3DXVECTOR3(minCorner.x,maxCorner.y-(maxCorner.y-minCorner.y)/4.0f,maxCorner.z);
    m_pRBBoxVertices[23].position = D3DXVECTOR3(minCorner.x,maxCorner.y,maxCorner.z-(maxCorner.z-minCorner.z)/4.0f);

	m_pRBBoxVertices[24].position = D3DXVECTOR3(maxCorner.x,maxCorner.y,maxCorner.z);
    m_pRBBoxVertices[25].position = D3DXVECTOR3(maxCorner.x-(maxCorner.x-minCorner.x)/4.0f,maxCorner.y,maxCorner.z);
    m_pRBBoxVertices[26].position = D3DXVECTOR3(maxCorner.x,maxCorner.y-(maxCorner.y-minCorner.y)/4.0f,maxCorner.z);
    m_pRBBoxVertices[27].position = D3DXVECTOR3(maxCorner.x,maxCorner.y,maxCorner.z-(maxCorner.z-minCorner.z)/4.0f);

	m_pRBBoxVertices[28].position = D3DXVECTOR3(maxCorner.x,minCorner.y,maxCorner.z);
	m_pRBBoxVertices[29].position = D3DXVECTOR3(maxCorner.x-(maxCorner.x-minCorner.x)/4.0f,minCorner.y,maxCorner.z);
	m_pRBBoxVertices[30].position = D3DXVECTOR3(maxCorner.x,minCorner.y+(maxCorner.y-minCorner.y)/4.0f,maxCorner.z);
	m_pRBBoxVertices[31].position = D3DXVECTOR3(maxCorner.x,minCorner.y,maxCorner.z-(maxCorner.z-minCorner.z)/4.0f);
	
	//Unlock���㻺��
	m_RBBoxVBuffer->Unlock();
	
	return S_OK;

}
//-----------------------------------------------------------------------------
// ������: SetTransformMatrix()
// ��  ��: 
// ��  ;: ���ñ任����
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503CC
VOID CRenderBoundBox::SetTransformMatrix(D3DXMATRIX* pMat)
{
	transform.matrix = *pMat;
}	

//-----------------------------------------------------------------------------
// ������: SetScaleMatrix()
// ��  ��: D3DXMATRIX* pMat
// ��  ;: ���ñ任����
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503CE
VOID CRenderBoundBox::SetScaleMatrix(D3DXMATRIX* pMat)
{
	transform.scale_matrix = *pMat;
}

//-----------------------------------------------------------------------------
// ������: SetRotationMatrix()
// ��  ��: D3DXMATRIX* pMat
// ��  ;: ���ñ任����
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503D0
VOID CRenderBoundBox::SetRotationMatrix(D3DXMATRIX* pMat)
{
	transform.rotation_matrix = *pMat;
}

//-----------------------------------------------------------------------------
// ������: SetTranslateMatrix()
// ��  ��: D3DXMATRIX* pMat
// ��  ;: ���ñ任����
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503D8
VOID CRenderBoundBox::SetTranslateMatrix(D3DXMATRIX* pMat)
{
	transform.translate_matrix = *pMat;
}

VOID CRenderBoundBox::SetWorldMatrix(D3DXMATRIX* pMat)
{
    transform.worldMatrix = *pMat;
}

//-----------------------------------------------------------------------------
// ������: GetTransformMatrix()
// ��  ��: 
// ��  ;: �õ��任����
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503DB
D3DXMATRIX* CRenderBoundBox::GetTransformMatrix()
{
	return &transform.matrix;
}	

//-----------------------------------------------------------------------------
// ������: SetScale()
// ��  ��: 
// ��  ;: ���ô�С
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503DC
VOID CRenderBoundBox::SetScale(FLOAT scaleValue)
{
	D3DXMatrixScaling(&transform.scale_matrix, scaleValue, scaleValue, scaleValue);
	MultiplyMatrix();
}

//-----------------------------------------------------------------------------
// ������: MultiplyMatrix()
// ��  ��: 
// ��  ;: 
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503DA
VOID CRenderBoundBox::MultiplyMatrix()
{
	//cal all matrix
	D3DXMatrixMultiply(&(transform.matrix), 
					   &(transform.scale_matrix), 
					   &(transform.rotation_matrix));
	D3DXMatrixMultiply(&(transform.matrix), 
					   &(transform.matrix), 
					   &(transform.translate_matrix ));
}

//�ͷ���Դ
//##ModelId=47EBC7460000
VOID CRenderBoundBox::Release()
{
	SAFE_RELEASE_D3D(m_RBBoxVBuffer);
	SAFE_RELEASE_D3D(m_RBBoxIBuffer);
}
