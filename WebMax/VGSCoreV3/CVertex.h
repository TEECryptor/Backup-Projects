/////////////////////////////////////////////////////////////////////////
// 名称: CVertex 类型定义--三维顶点类
// 描述: CVertex 类型定义了点的各种属性
// 作者: Lixin	
// 日期: 2006-04-30
////////////////////////////////////////////////////////////////////////

#ifndef _CVERTEX_H
#define _CVERTEX_H

#include "VGSHeader.h"

const int MAX_BONE_COUNT = 4;  // 每个顶点最多受几个骨头影响

//顶点的UV
typedef struct _UV
{
	//uv坐标
	float					u;
	float					v;
}_UV;

class VGS_EXPORT CVertex
{	
public:
	UINT				index;				//当前顶点在所属模型顶点中的编号
	D3DXVECTOR3			position;			//顶点相对于模型中心的相对位置
	D3DXVECTOR3			normal;				//顶点法线
	D3DCOLOR			diffuse;			//顶点的颜色
    //UV坐标的层数, 8位表示8层，1-8，目前只保存四层
	//BYTE				uvFlag; 
	//顶点的map Channel(1-8) UV，根据层贴图坐标标志位，部分有可能没有
	_UV					uv1;
	_UV					uv2;
	_UV					uv3;
	_UV					uv4;
	//FLOAT				su, sv;				//Diffuse贴图坐标
	//FLOAT				sw, sz;				//LightMap贴图坐标

	FLOAT				weight[MAX_BONE_COUNT];          // 受每个骨头影响的权重

	//用于简化碰撞检测运算
	D3DXVECTOR3			worldPosition;		//顶点经过变换后的世界坐标

public:
	CVertex();
	~CVertex();

};

#endif
