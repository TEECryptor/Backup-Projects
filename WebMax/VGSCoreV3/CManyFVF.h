/////////////////////////////////////////////////////////////////////////
// ����: ���ֶ���ṹ�Ͷ�ӦFVF���� 
// ����: lixin
// ����: 2005-09-23
/////////////////////////////////////////////////////////////////////////

#ifndef _CMANYFVF_H
#define _CMANYFVF_H

#include "VGSHeader.h"

// const int MAX_BONE_COUNT = 4;  // ÿ����������ܼ�����ͷӰ��

//T1����ģ�Ͷ���ṹ���壬û����ͼ��û�з�������ֻ����ɫ�����������ͷ�ʹ�ɫ�߶�
typedef struct _VERTEXDECL_POSCOLOR 
{
	D3DXVECTOR3		position;		//�����λ��
	D3DCOLOR		diffuse;
}VERTEXDECL_POSCOLOR;
//T1����ģ�Ͷ���FVF����
#define FVF_POSCOLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE)


//T2����ģ�Ͷ���ṹ���壬û����ͼ���з�����������ɫ�������в��ʶ�����ͼ��ģ��
typedef struct _VERTEXDECL_POSNORMAL
{
	D3DXVECTOR3		position;		//�����λ��
	D3DXVECTOR3		normal;			//����ķ���
}VERTEXDECL_POSNORMAL;
//T2����ģ�Ͷ���FVF����
#define FVF_POSNORMAL (D3DFVF_XYZ | D3DFVF_NORMAL )


//T3����ģ�Ͷ���ṹ����,�����е��Է���ģ�Ͷ���ṹ���壬��������λ�ú�Diffuse��ͼ
typedef struct _VERTEXDECL_POSCOLORCOORD1
{
	D3DXVECTOR3		position;		//�����λ��
	FLOAT			su, sv;			//������ͼ����
}VERTEXDECL_POSCOLORCOORD1;
//T3����ģ�Ͷ���FVF����
#define FVF_POSCOLORCOORD1 (D3DFVF_XYZ | D3DFVF_TEX1)


//T4����ģ�Ͷ���ṹ����,�����е��ܹ���Ӱ��ģ�ͣ����Է��⣩����ṹ���壬
//��������λ�á����㷨�ߡ�������ɫ��Diffuse��ͼ
typedef struct _VERTEXDECL_POSNORMALCOLORCOORD1
{
	D3DXVECTOR3		position;		//�����λ��
	D3DXVECTOR3		normal;			//����ķ���
	FLOAT			su, sv;			//������ͼ����
}VERTEXDECL_POSNORMALCOLORCOORD1;
//T4����ģ�Ͷ���FVF����
#define FVF_POSNORMALCOLORCOORD1 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

//T5����ģ�Ͷ���ṹ����,�����е��Է���ģ�Ͷ���ṹ���壬
//�������㡢Diffuse��ͼ��LightMap��ͼ
typedef struct _VERTEXDECL_POSNORMALCOLORCOORD2
{
	D3DXVECTOR3		position;		//�����λ��
	D3DXVECTOR3		normal;			//����ķ���
	FLOAT			su1, sv1;			//����Diffuse��ͼ����
	FLOAT			su2, sv2;			//����Ĺ���ͼ����
}VERTEXDECL_POSNORMALCOLORCOORD2;
//T5����ģ�Ͷ���FVF����
#define FVF_POSNORMALCOLORCOORD2 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2)

//T6����ģ�Ͷ���ṹ����,�����еĻ���ģ�Ͷ���ṹ����
typedef struct _VERTEXDECL_POSNORMALCOLORCOORD3
{
	D3DXVECTOR3		position;		//�����λ��
	D3DXVECTOR3		normal;			//����ķ���
	FLOAT			su1, sv1;			//����Diffuse��ͼ����
	FLOAT			su2, sv2;			//����Ĺ���ͼ����
	FLOAT			su3, sv3;		
}VERTEXDECL_POSNORMALCOLORCOORD3;
//T6����ģ�Ͷ���FVF����
#define FVF_POSNORMALCOLORCOORD3 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3)

//T7����ģ�Ͷ���ṹ����,�����еĻ���ģ�Ͷ���ṹ���壬
//��������λ�á����ߡ���ɫ��4����ͼ����
typedef struct _VERTEXDECL_POSNORMALCOLORCOORD4
{
	D3DXVECTOR3		position;		// �����λ��
	D3DXVECTOR3		normal;			// ����ķ���
	FLOAT			su1, sv1;			//��һ����ͼ����
	FLOAT			su2, sv2;			//�ڶ�����ͼ����
	FLOAT			su3, sv3;			//��������ͼ����
	FLOAT			su4, sv4;			//���Ĳ���ͼ����
}VERTEXDECL_POSNORMALCOLORCOORD4;
//T7����ģ�Ͷ���FVF����
#define FVF_POSNORMALCOLORCOORD4 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX4)


//RHW MESH, �ڱ任�����ͼ�ռ��л���ģ�͵�MESH FVF
typedef struct _VERTEXDECL_POSRHWCOLORCOORD1
{
	D3DXVECTOR3		position;		//�����λ��
	FLOAT			rhw;			//�����RHW
	FLOAT			su, sv;
}VERTEXDECL_POSRHWCOLORCOORD1;
//RHW MESH VERTEX FVF����
#define FVF_POSRHWCOLORCOORD1 (D3DFVF_XYZRHW | D3DFVF_TEX1); 

//������Ƥģ�͵Ķ���ṹ1
typedef struct _VERTEXDECL_SKINMESHCOORD1
{
	D3DXVECTOR3		position;		
	FLOAT			weight[4];			//������ĸ�����Ӱ���Ȩ��	
	D3DXVECTOR3		normal;
	FLOAT			su1, sv1;			//��ͼ����
}VERTEXDECL_SKINMESHCOORD1;
//T8����ģ�Ͷ���FVF����
#define FVF_SKINMESHCOORD1 (D3DFVF_XYZB4 | D3DFVF_NORMAL | D3DFVF_TEX1)

//������Ƥģ�͵Ķ���ṹ2
typedef struct _VERTEXDECL_SKINMESHCOORD2
{
	D3DXVECTOR3		position;		
	FLOAT			weight[4];			//������ĸ�����Ӱ���Ȩ��	
	D3DXVECTOR3		normal;
	FLOAT			su1, sv1;			//��1����ͼ����
	FLOAT			su2, sv2;			//��2����ͼ����
}VERTEXDECL_SKINMESHCOORD2;
//T8����ģ�Ͷ���FVF����
#define FVF_SKINMESHCOORD2 (D3DFVF_XYZB4 | D3DFVF_NORMAL | D3DFVF_TEX2)

#endif
