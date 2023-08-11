/////////////////////////////////////////////////////////////////////////
// ����: CVertex ���Ͷ���--��ά������
// ����: CVertex ���Ͷ����˵�ĸ�������
// ����: Lixin	
// ����: 2006-04-30
////////////////////////////////////////////////////////////////////////

#ifndef _CVERTEX_H
#define _CVERTEX_H

#include "VGSHeader.h"

const int MAX_BONE_COUNT = 4;  // ÿ����������ܼ�����ͷӰ��

//�����UV
typedef struct _UV
{
	//uv����
	float					u;
	float					v;
}_UV;

class VGS_EXPORT CVertex
{	
public:
	UINT				index;				//��ǰ����������ģ�Ͷ����еı��
	D3DXVECTOR3			position;			//���������ģ�����ĵ����λ��
	D3DXVECTOR3			normal;				//���㷨��
	D3DCOLOR			diffuse;			//�������ɫ
    //UV����Ĳ���, 8λ��ʾ8�㣬1-8��Ŀǰֻ�����Ĳ�
	//BYTE				uvFlag; 
	//�����map Channel(1-8) UV�����ݲ���ͼ�����־λ�������п���û��
	_UV					uv1;
	_UV					uv2;
	_UV					uv3;
	_UV					uv4;
	//FLOAT				su, sv;				//Diffuse��ͼ����
	//FLOAT				sw, sz;				//LightMap��ͼ����

	FLOAT				weight[MAX_BONE_COUNT];          // ��ÿ����ͷӰ���Ȩ��

	//���ڼ���ײ�������
	D3DXVECTOR3			worldPosition;		//���㾭���任�����������

public:
	CVertex();
	~CVertex();

};

#endif
