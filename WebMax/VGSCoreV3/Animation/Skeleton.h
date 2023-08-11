/////////////////////////////////////////////////////////////////////////
// 文件名: Skeleton.h: 骨架结构类
// 类  名: CSkeleton
// 描  述: 一个CSkeleton对象表示一副骨架，它由很多个Bone按照一定的层次关系组成
// 作  者: lixin
// 日  期: 2009-06-09
/////////////////////////////////////////////////////////////////////////

#ifndef _SKELETON_H_
#define _SKELETON_H_

#include "..\\VGSDef.h"

#include "Bone.h"

class VGS_EXPORT CSkeleton
{
private: 
	//根骨头对象
	CBone*							m_pRootBone;

public:
	//骨架相对Mesh的变换矩阵
	D3DXMATRIX						skeletonTransform;

	//用于ISKinInfo的UpdateSkinMesh计算的骨骼变换矩阵列表，该矩阵是在Mesh原点坐标系中，相对于骨骼初始姿态的变换
	std::vector<D3DXMATRIX>			bonesSkinMatrix;

public:
	//得到根骨骼对象
	CBone*							getRootBone() {return m_pRootBone;}
	//设置根骨骼对象
	VOID							setRootBone(CBone* pBone) {m_pRootBone = pBone;}
	
	//计算骨架中的所有骨骼的偏移矩阵
	VOID							updateBonesOffsetMatrix();

	//更新骨架中所有骨骼的合成矩阵
	VOID							updateBonesCombinedMatrix(DWORD frameIndex);

	//更新骨架中所有骨骼相对于Mesh原点的变换矩阵，用于ISKinInfo的UpdateSkinMesh计算
	VOID							updateBonesSkinMatrix();


public:
	CSkeleton();
	~CSkeleton();
};

#endif //_SKELETON_H