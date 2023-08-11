/////////////////////////////////////////////////////////////////////////
// 名称: Face类型定义--三角面类
// 描述: Face类型定义了三角面的各种属性
// 作者: lixin	
// 日期: 2006-04-30
////////////////////////////////////////////////////////////////////////

#include "CFace.h"
 
//##ModelId=47EBC74E01C5
CFace::CFace()
{
//	visible = TRUE;
	vertices0 = NULL;
	vertices1 = NULL;
	vertices2 = NULL;
	normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//##ModelId=47EBC74E01C6
CFace::~CFace()
{

}
