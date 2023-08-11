/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // CSkeleton.cpp
* ����ժҪ�� // �Ǽܶ���
* ����˵���� // 
* ��ǰ�汾�� // V2.0
* ��    �ߣ� // Lixin
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
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

//����Ǽ��е����й�����ƫ�ƾ���
VOID CSkeleton::updateBonesOffsetMatrix()
{
	if (m_pRootBone == NULL) return;

	m_pRootBone->updateBonesOffsetMatrix(&skeletonTransform);
}

//���¹Ǽ������й����ĺϳɾ���
VOID CSkeleton::updateBonesCombinedMatrix(DWORD frameIndex)
{
	if (m_pRootBone == NULL) return;

	m_pRootBone->updateBonesCombinedMatrix(&skeletonTransform, frameIndex);
}

//���¹Ǽ������й��������Meshԭ��ı任��������ISKinInfo��UpdateSkinMesh����
VOID CSkeleton::updateBonesSkinMatrix()
{
	if (m_pRootBone == NULL) return;

	m_pRootBone->updateBonesSkinMatrix();
}