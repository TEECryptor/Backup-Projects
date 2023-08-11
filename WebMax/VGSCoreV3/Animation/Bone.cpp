#include "Bone.h"

CBone::CBone(void)
{
	//骨骼的初始姿态变换，相对父骨骼
	D3DXMatrixIdentity(&startPoseMat);

	//骨骼的偏移矩阵，初始姿态相对Mesh原点的变换矩阵
	D3DXMatrixIdentity(&offsetMat);

	//当前帧骨骼合成的后变换（相对Mesh原点），从上到下矩阵相乘，关键帧列表
	D3DXMatrixIdentity(&combinedMat);

	//用于SkinInfo的变换矩阵
	D3DXMatrixIdentity(&boneSkinMat);
}

CBone::~CBone(void)
{
	child.clear();
	frameMat.clear();
}


//计算骨骼及所有子骨骼的偏移矩阵
VOID CBone::updateBonesOffsetMatrix(D3DXMATRIX* pMatTransform)
{
	//计算偏移矩阵
	offsetMat = startPoseMat * (*pMatTransform);

	//计算孩子骨骼的偏移矩阵
	std::vector<CBone*>::iterator it = child.begin();
	for (DWORD i = 0; i< child.size(); i++)
	{
		CBone* pBone = *(it + i);
		pBone->updateBonesOffsetMatrix(&offsetMat);
	}
}

//更新骨架中所有骨骼的合成矩阵
VOID CBone::updateBonesCombinedMatrix(D3DXMATRIX* pMatTransform, DWORD frameIndex)
{
	//计算合成矩阵
	combinedMat = frameMat[frameIndex]  * startPoseMat * (*pMatTransform);

	//计算孩子骨骼的合成矩阵
	std::vector<CBone*>::iterator it = child.begin();
	for (DWORD i = 0; i< child.size(); i++)
	{
		CBone* pBone = *(it + i);
		pBone->updateBonesCombinedMatrix(&combinedMat, frameIndex);
	}
}

//更新骨架中所有骨骼相对于Mesh原点的变换矩阵，用于ISKinInfo的UpdateSkinMesh计算
VOID CBone::updateBonesSkinMatrix()
{
	//计算BoneSkin矩阵
	D3DXMATRIX offsetMatInverse;
	D3DXMatrixInverse(&offsetMatInverse, NULL, &offsetMat);
	boneSkinMat = combinedMat * offsetMatInverse;

	//计算孩子骨骼的BoneSkin矩阵
	std::vector<CBone*>::iterator it = child.begin();
	for (DWORD i = 0; i< child.size(); i++)
	{
		CBone* pBone = *(it + i);
		pBone->updateBonesSkinMatrix();
	}

}