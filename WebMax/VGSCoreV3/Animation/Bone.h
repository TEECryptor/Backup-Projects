/////////////////////////////////////////////////////////////////////////
// 文件名: Bone.h: 骨骼结构类
// 类  名: CBone
// 描  述: 一个CBone对象表示一个骨头，按照一定的层级关系的多个Bone组成一个Skeleton
// 作  者: lixin
// 日  期: 2009-06-09
/////////////////////////////////////////////////////////////////////////
#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <vector>
#include "..\\VGSDef.h"

class VGS_EXPORT CBone
{
public:
	//骨骼的名字
	std::string						name;

	//骨骼的初始姿态变换，相对父骨骼
	D3DXMATRIX						startPoseMat;

	//骨骼的孩子骨骼列表
	std::vector<CBone*>				child;


	//骨骼的偏移矩阵，初始姿态相对Mesh原点的变换矩阵
	D3DXMATRIX						offsetMat;

	//每一帧相对初始姿态的变换矩阵，相对父骨骼
	std::vector<D3DXMATRIX>			frameMat;

	//当前帧骨骼合成的后变换（相对Mesh原点），从上到下矩阵相乘，关键帧列表
	D3DXMATRIX						combinedMat;

	//用于SkinInfo的变换矩阵
	D3DXMATRIX						boneSkinMat;


public:
	//计算骨骼及所有子骨骼的偏移矩阵
	VOID							updateBonesOffsetMatrix(D3DXMATRIX* pMatTransform);

	//更新骨架中所有骨骼的合成矩阵
	VOID							updateBonesCombinedMatrix(D3DXMATRIX* pMatTransform, DWORD frameIndex);

	//更新骨架中所有骨骼相对于Mesh原点的变换矩阵，用于ISKinInfo的UpdateSkinMesh计算
	VOID							updateBonesSkinMatrix();
public:
	CBone(void);
	~CBone(void);
};
