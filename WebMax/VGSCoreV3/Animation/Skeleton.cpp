/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // CSkeleton.cpp
* 内容摘要： // 骨架对象
* 其它说明： // 
* 当前版本： // V2.0
* 作    者： // Lixin
* 完成日期： // 
* 
* 修改日期      版本号     修改人	      修改内容
* ---------------------------------------------------------------
* 
*
*********************************************************************/

#include "Skeleton.h"

CSkeleton::CSkeleton()
{
	D3DXMatrixIdentity(&skeletonTransform);
	m_pRootBone = NULL;
}

CSkeleton::~CSkeleton()
{
	bonesSkinMatrix.clear();
}

//计算骨架中的所有骨骼的偏移矩阵
VOID CSkeleton::updateBonesOffsetMatrix()
{
	if (m_pRootBone == NULL) return;

	m_pRootBone->updateBonesOffsetMatrix(&skeletonTransform);
}

//更新骨架中所有骨骼的合成矩阵
VOID CSkeleton::updateBonesCombinedMatrix(DWORD frameIndex)
{
	if (m_pRootBone == NULL) return;

	m_pRootBone->updateBonesCombinedMatrix(&skeletonTransform, frameIndex);
}

//更新骨架中所有骨骼相对于Mesh原点的变换矩阵，用于ISKinInfo的UpdateSkinMesh计算
VOID CSkeleton::updateBonesSkinMatrix()
{
	if (m_pRootBone == NULL) return;

	m_pRootBone->updateBonesSkinMatrix();
}