/////////////////////////////////////////////////////////////////////////
// ��  ��: CAxis.h
// ��  ��: XYZ��ģ�Ͷ����壬���ڵ�ѡ����ʱ��ʾ
// ��  ��: lixin
// ��  ��: 2006-05-15
/////////////////////////////////////////////////////////////////////////

#ifndef _CRENDERBOUNDBOX_H
#define _CRENDERBOUNDBOX_H

#include "CManyFVF.h"
#include "CSysFunc.h"
#include "CTransform.h"

//##ModelId=47EBC7450399
class VGS_EXPORT CRenderBoundBox  
{
private:
	//##ModelId=47EBC745039B
	LPDIRECT3DDEVICE9		m_pd3dDevice;

	//##ModelId=47EBC74503AA
	LPDIRECT3DVERTEXBUFFER9  m_RBBoxVBuffer;		//���߶��㻺��
	//##ModelId=47EBC74503AF
    VERTEXDECL_POSCOLOR			 *m_pRBBoxVertices;	//���߶��㻺���������ڴ�����
	//##ModelId=47EBC74503B4
	D3DCOLOR				 RBBoxColor;	//��Χ����ɫ
	//##ModelId=47EBC74503B8
    UINT                     m_RBBoxVectCnt;  //��Χ�ж������
	//##ModelId=47EBC74503B9
	UINT                     m_RBBoxLineCnt;   //��Χ�б���

	//##ModelId=47EBC74503BB
	LPDIRECT3DINDEXBUFFER9   m_RBBoxIBuffer;	//��Χ����������
	//##ModelId=47EBC74503BF
	UINT					 *m_pRBBoxIndices;	//��Χ�����������������ڴ�����

	//##ModelId=47EBC74503C1
	CTransform				 transform;		//�任����

	//��ʼ������
	//##ModelId=47EBC74503C5
	VOID					InitRBBoxVertex();
	//�������㻺��
	//##ModelId=47EBC74503C8
	HRESULT					CreateRBBoxVertexBuf();
	//������������
	//##ModelId=47EBC74503C9
	HRESULT					CreateRBBoxIndexBuf();

	//�Ѱ�Χ�л�����
	//##ModelId=47EBC74503CA
	VOID					DrawRBBox();


public:
	//draw
	//##ModelId=47EBC74503CB
	VOID					Draw();
	//���ñ任����
	//##ModelId=47EBC74503CC
	VOID					SetTransformMatrix(D3DXMATRIX* pMat);
	//##ModelId=47EBC74503CE
	VOID					SetScaleMatrix(D3DXMATRIX* pMat);
	//##ModelId=47EBC74503D0
	VOID					SetRotationMatrix(D3DXMATRIX* pMat);
	//##ModelId=47EBC74503D8
	VOID					SetTranslateMatrix(D3DXMATRIX* pMat);
	VOID                    SetWorldMatrix(D3DXMATRIX* pMat);
	//update matrix
	//##ModelId=47EBC74503DA
	VOID					MultiplyMatrix();
	//�õ��任����
	//##ModelId=47EBC74503DB
	D3DXMATRIX*				GetTransformMatrix();
	//���ô�С
	//##ModelId=47EBC74503DC
	VOID					SetScale(FLOAT scaleValue);
	//���ö���
	//##ModelId=47EBC74503DE
	HRESULT                 SetRBBoxVertex(D3DXVECTOR3 minCorner, D3DXVECTOR3 maxCorner);
	//�ͷ���Դ
	//##ModelId=47EBC7460000
	VOID					Release();

public:
	//##ModelId=47EBC7460001
	CRenderBoundBox(LPDIRECT3DDEVICE9	pd3dDevice);
	//##ModelId=47EBC7460003
	~CRenderBoundBox();

};

#endif
