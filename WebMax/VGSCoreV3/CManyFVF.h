/////////////////////////////////////////////////////////////////////////
// 名称: 各种顶点结构和对应FVF定义 
// 作者: lixin
// 日期: 2005-09-23
/////////////////////////////////////////////////////////////////////////

#ifndef _CMANYFVF_H
#define _CMANYFVF_H

#include "VGSHeader.h"

// const int MAX_BONE_COUNT = 4;  // 每个顶点最多受几个骨头影响

//T1类型模型顶点结构定义，没有贴图，没有法向量，只有颜色，用于坐标箭头和纯色线段
typedef struct _VERTEXDECL_POSCOLOR 
{
	D3DXVECTOR3		position;		//顶点的位置
	D3DCOLOR		diffuse;
}VERTEXDECL_POSCOLOR;
//T1类型模型顶点FVF定义
#define FVF_POSCOLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE)


//T2类型模型顶点结构定义，没有贴图，有法向量，有颜色，用于有材质而无贴图的模型
typedef struct _VERTEXDECL_POSNORMAL
{
	D3DXVECTOR3		position;		//顶点的位置
	D3DXVECTOR3		normal;			//顶点的法线
}VERTEXDECL_POSNORMAL;
//T2类型模型顶点FVF定义
#define FVF_POSNORMAL (D3DFVF_XYZ | D3DFVF_NORMAL )


//T3类型模型顶点结构定义,场景中的自发光模型顶点结构定义，包含顶点位置和Diffuse贴图
typedef struct _VERTEXDECL_POSCOLORCOORD1
{
	D3DXVECTOR3		position;		//顶点的位置
	FLOAT			su, sv;			//顶点贴图坐标
}VERTEXDECL_POSCOLORCOORD1;
//T3类型模型顶点FVF定义
#define FVF_POSCOLORCOORD1 (D3DFVF_XYZ | D3DFVF_TEX1)


//T4类型模型顶点结构定义,场景中的受光照影响模型（非自发光）顶点结构定义，
//包含顶点位置、顶点法线、顶点颜色和Diffuse贴图
typedef struct _VERTEXDECL_POSNORMALCOLORCOORD1
{
	D3DXVECTOR3		position;		//顶点的位置
	D3DXVECTOR3		normal;			//顶点的法线
	FLOAT			su, sv;			//顶点贴图坐标
}VERTEXDECL_POSNORMALCOLORCOORD1;
//T4类型模型顶点FVF定义
#define FVF_POSNORMALCOLORCOORD1 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

//T5类型模型顶点结构定义,场景中的自发光模型顶点结构定义，
//包含顶点、Diffuse贴图、LightMap贴图
typedef struct _VERTEXDECL_POSNORMALCOLORCOORD2
{
	D3DXVECTOR3		position;		//顶点的位置
	D3DXVECTOR3		normal;			//顶点的法线
	FLOAT			su1, sv1;			//顶点Diffuse贴图坐标
	FLOAT			su2, sv2;			//顶点的光贴图坐标
}VERTEXDECL_POSNORMALCOLORCOORD2;
//T5类型模型顶点FVF定义
#define FVF_POSNORMALCOLORCOORD2 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2)

//T6类型模型顶点结构定义,场景中的基本模型顶点结构定义
typedef struct _VERTEXDECL_POSNORMALCOLORCOORD3
{
	D3DXVECTOR3		position;		//顶点的位置
	D3DXVECTOR3		normal;			//顶点的法线
	FLOAT			su1, sv1;			//顶点Diffuse贴图坐标
	FLOAT			su2, sv2;			//顶点的光贴图坐标
	FLOAT			su3, sv3;		
}VERTEXDECL_POSNORMALCOLORCOORD3;
//T6类型模型顶点FVF定义
#define FVF_POSNORMALCOLORCOORD3 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3)

//T7类型模型顶点结构定义,场景中的基本模型顶点结构定义，
//包含顶点位置、法线、颜色、4层贴图坐标
typedef struct _VERTEXDECL_POSNORMALCOLORCOORD4
{
	D3DXVECTOR3		position;		// 顶点的位置
	D3DXVECTOR3		normal;			// 顶点的法线
	FLOAT			su1, sv1;			//第一层贴图坐标
	FLOAT			su2, sv2;			//第二层贴图坐标
	FLOAT			su3, sv3;			//第三层贴图坐标
	FLOAT			su4, sv4;			//第四层贴图坐标
}VERTEXDECL_POSNORMALCOLORCOORD4;
//T7类型模型顶点FVF定义
#define FVF_POSNORMALCOLORCOORD4 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX4)


//RHW MESH, 在变换后的视图空间中绘制模型的MESH FVF
typedef struct _VERTEXDECL_POSRHWCOLORCOORD1
{
	D3DXVECTOR3		position;		//顶点的位置
	FLOAT			rhw;			//顶点的RHW
	FLOAT			su, sv;
}VERTEXDECL_POSRHWCOLORCOORD1;
//RHW MESH VERTEX FVF定义
#define FVF_POSRHWCOLORCOORD1 (D3DFVF_XYZRHW | D3DFVF_TEX1); 

//骨骼蒙皮模型的顶点结构1
typedef struct _VERTEXDECL_SKINMESHCOORD1
{
	D3DXVECTOR3		position;		
	FLOAT			weight[4];			//最多受四个骨骼影响的权重	
	D3DXVECTOR3		normal;
	FLOAT			su1, sv1;			//贴图坐标
}VERTEXDECL_SKINMESHCOORD1;
//T8类型模型顶点FVF定义
#define FVF_SKINMESHCOORD1 (D3DFVF_XYZB4 | D3DFVF_NORMAL | D3DFVF_TEX1)

//骨骼蒙皮模型的顶点结构2
typedef struct _VERTEXDECL_SKINMESHCOORD2
{
	D3DXVECTOR3		position;		
	FLOAT			weight[4];			//最多受四个骨骼影响的权重	
	D3DXVECTOR3		normal;
	FLOAT			su1, sv1;			//第1层贴图坐标
	FLOAT			su2, sv2;			//第2层贴图坐标
}VERTEXDECL_SKINMESHCOORD2;
//T8类型模型顶点FVF定义
#define FVF_SKINMESHCOORD2 (D3DFVF_XYZB4 | D3DFVF_NORMAL | D3DFVF_TEX2)

#endif
