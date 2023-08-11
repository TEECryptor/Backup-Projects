/////////////////////////////////////////////////////////////////////////
// ����: CTransform ���Ͷ���
// ����: CVertex ���Ͷ����˿ռ�任
// ����: Lixin	
// ����: 2006-04-30
////////////////////////////////////////////////////////////////////////

#ifndef _CTRANSFORM_H
#define _CTRANSFORM_H

#include "VGSHeader.h"
#include "CSysFunc.h"

class VGS_EXPORT CTransform  
{
public:
	D3DXMATRIX			translate_matrix;   //ƽ�ƾ���
	D3DXMATRIX			rotation_matrix;    //��ת����
	D3DXMATRIX			scale_matrix;		//���ž���

	D3DXMATRIX          ori_matrix;         //���������Ľڵ�ĳ�ʼ���任����
	D3DXMATRIX			matrix;				//�ڵ�ĵ�ǰ��Ը��ڵ�ı任����

    //Edit by Baojiahui 
	D3DXMATRIX          worldMatrix;        //�ڵ������任����

	D3DXVECTOR3			position;           //ģ���ڸ�����ϵ�е�λ��
	D3DXVECTOR3			rotation;			//ģ���Ƹ��������ת��
	D3DXVECTOR3			scale;				//ģ���ظ�����ϵ������

public:
	CTransform();
	~CTransform();

public:
	bool decomposeTransfromMatrix(const D3DXMATRIX& transMat, D3DXVECTOR3& trans, D3DXVECTOR3& rot,D3DXVECTOR3& scale);
private:    
	D3DXVECTOR3* V3Combine (D3DXVECTOR3* a,D3DXVECTOR3* b,D3DXVECTOR3* result,float ascl,float bscl);
};

#endif
