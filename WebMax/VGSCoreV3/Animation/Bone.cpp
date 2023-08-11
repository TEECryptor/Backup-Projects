#include "Bone.h"

CBone::CBone(void)
{
	//�����ĳ�ʼ��̬�任����Ը�����
	D3DXMatrixIdentity(&startPoseMat);

	//������ƫ�ƾ��󣬳�ʼ��̬���Meshԭ��ı任����
	D3DXMatrixIdentity(&offsetMat);

	//��ǰ֡�����ϳɵĺ�任�����Meshԭ�㣩�����ϵ��¾�����ˣ��ؼ�֡�б�
	D3DXMatrixIdentity(&combinedMat);

	//����SkinInfo�ı任����
	D3DXMatrixIdentity(&boneSkinMat);
}

CBone::~CBone(void)
{
	child.clear();
	frameMat.clear();
}


//��������������ӹ�����ƫ�ƾ���
VOID CBone::updateBonesOffsetMatrix(D3DXMATRIX* pMatTransform)
{
	//����ƫ�ƾ���
	offsetMat = startPoseMat * (*pMatTransform);

	//���㺢�ӹ�����ƫ�ƾ���
	std::vector<CBone*>::iterator it = child.begin();
	for (DWORD i = 0; i< child.size(); i++)
	{
		CBone* pBone = *(it + i);
		pBone->updateBonesOffsetMatrix(&offsetMat);
	}
}

//���¹Ǽ������й����ĺϳɾ���
VOID CBone::updateBonesCombinedMatrix(D3DXMATRIX* pMatTransform, DWORD frameIndex)
{
	//����ϳɾ���
	combinedMat = frameMat[frameIndex]  * startPoseMat * (*pMatTransform);

	//���㺢�ӹ����ĺϳɾ���
	std::vector<CBone*>::iterator it = child.begin();
	for (DWORD i = 0; i< child.size(); i++)
	{
		CBone* pBone = *(it + i);
		pBone->updateBonesCombinedMatrix(&combinedMat, frameIndex);
	}
}

//���¹Ǽ������й��������Meshԭ��ı任��������ISKinInfo��UpdateSkinMesh����
VOID CBone::updateBonesSkinMatrix()
{
	//����BoneSkin����
	D3DXMATRIX offsetMatInverse;
	D3DXMatrixInverse(&offsetMatInverse, NULL, &offsetMat);
	boneSkinMat = combinedMat * offsetMatInverse;

	//���㺢�ӹ�����BoneSkin����
	std::vector<CBone*>::iterator it = child.begin();
	for (DWORD i = 0; i< child.size(); i++)
	{
		CBone* pBone = *(it + i);
		pBone->updateBonesSkinMatrix();
	}

}