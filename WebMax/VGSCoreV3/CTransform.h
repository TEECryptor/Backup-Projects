/////////////////////////////////////////////////////////////////////////
// 名称: CTransform 类型定义
// 描述: CVertex 类型定义了空间变换
// 作者: Lixin	
// 日期: 2006-04-30
////////////////////////////////////////////////////////////////////////

#ifndef _CTRANSFORM_H
#define _CTRANSFORM_H

#include "VGSHeader.h"
#include "CSysFunc.h"

class VGS_EXPORT CTransform  
{
public:
	D3DXMATRIX			translate_matrix;   //平移矩阵
	D3DXMATRIX			rotation_matrix;    //旋转矩阵
	D3DXMATRIX			scale_matrix;		//缩放矩阵

	D3DXMATRIX          ori_matrix;         //包含动画的节点的初始化变换矩阵
	D3DXMATRIX			matrix;				//节点的当前相对父节点的变换矩阵

    //Edit by Baojiahui 
	D3DXMATRIX          worldMatrix;        //节点的世界变换矩阵

	D3DXVECTOR3			position;           //模型在父坐标系中的位置
	D3DXVECTOR3			rotation;			//模型绕父坐标轴的转角
	D3DXVECTOR3			scale;				//模型沿父坐标系的缩放

public:
	CTransform();
	~CTransform();

public:
	bool decomposeTransfromMatrix(const D3DXMATRIX& transMat, D3DXVECTOR3& trans, D3DXVECTOR3& rot,D3DXVECTOR3& scale);
private:    
	D3DXVECTOR3* V3Combine (D3DXVECTOR3* a,D3DXVECTOR3* b,D3DXVECTOR3* result,float ascl,float bscl);
};

#endif
