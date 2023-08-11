/////////////////////////////////////////////////////////////////////////
// 名称: Face类型定义--三角面类
// 描述: Face类型定义了三角面的各种属性
// 作者: lixin	
// 日期: 2006-04-30
////////////////////////////////////////////////////////////////////////

#ifndef _CFACE_H
#define _CFACE_H

#include "VGSHeader.h"
#include "CVertex.h"

//##ModelId=47EBC74E01A8
class VGS_EXPORT CFace
{

public:
	//##ModelId=47EBC74E01A9
	UINT				index;				//当前面在所属模型所有面中的编号
	//##ModelId=47EBC74E01AB
	CVertex*			vertices0;			//三个顶点指针
	//##ModelId=47EBC74E01B6
	CVertex*			vertices1;			//三个顶点指针
	//##ModelId=47EBC74E01BB
	CVertex*			vertices2;			//三个顶点指针
	//##ModelId=47EBC74E01C0
	D3DXVECTOR3			normal;				//法线
	//##ModelId=47EBC74E01C4
	BOOL				visible;			//是否可见
	//用于简化碰撞检测运算
	D3DXVECTOR3			worldNormal;		//顶点的世界坐标系中的法线

public:
	//##ModelId=47EBC74E01C5
	CFace();
	//##ModelId=47EBC74E01C6
	~CFace();

};

#endif
