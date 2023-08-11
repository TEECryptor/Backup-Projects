/////////////////////////////////////////////////////////////////////////
// �ļ���: Skeleton.h: �Ǽܽṹ��
// ��  ��: CSkeleton
// ��  ��: һ��CSkeleton�����ʾһ���Ǽܣ����ɺܶ��Bone����һ���Ĳ�ι�ϵ���
// ��  ��: lixin
// ��  ��: 2009-06-09
/////////////////////////////////////////////////////////////////////////

#ifndef _SKELETON_H_
#define _SKELETON_H_

#include "..\\VGSDef.h"

#include "Bone.h"

class VGS_EXPORT CSkeleton
{
private: 
	//����ͷ����
	CBone*							m_pRootBone;

public:
	//�Ǽ����Mesh�ı任����
	D3DXMATRIX						skeletonTransform;

	//����ISKinInfo��UpdateSkinMesh����Ĺ����任�����б��þ�������Meshԭ������ϵ�У�����ڹ�����ʼ��̬�ı任
	std::vector<D3DXMATRIX>			bonesSkinMatrix;

public:
	//�õ�����������
	CBone*							getRootBone() {return m_pRootBone;}
	//���ø���������
	VOID							setRootBone(CBone* pBone) {m_pRootBone = pBone;}
	
	//����Ǽ��е����й�����ƫ�ƾ���
	VOID							updateBonesOffsetMatrix();

	//���¹Ǽ������й����ĺϳɾ���
	VOID							updateBonesCombinedMatrix(DWORD frameIndex);

	//���¹Ǽ������й��������Meshԭ��ı任��������ISKinInfo��UpdateSkinMesh����
	VOID							updateBonesSkinMatrix();


public:
	CSkeleton();
	~CSkeleton();
};

#endif //_SKELETON_H