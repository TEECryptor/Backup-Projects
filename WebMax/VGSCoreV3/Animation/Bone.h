/////////////////////////////////////////////////////////////////////////
// �ļ���: Bone.h: �����ṹ��
// ��  ��: CBone
// ��  ��: һ��CBone�����ʾһ����ͷ������һ���Ĳ㼶��ϵ�Ķ��Bone���һ��Skeleton
// ��  ��: lixin
// ��  ��: 2009-06-09
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
	//����������
	std::string						name;

	//�����ĳ�ʼ��̬�任����Ը�����
	D3DXMATRIX						startPoseMat;

	//�����ĺ��ӹ����б�
	std::vector<CBone*>				child;


	//������ƫ�ƾ��󣬳�ʼ��̬���Meshԭ��ı任����
	D3DXMATRIX						offsetMat;

	//ÿһ֡��Գ�ʼ��̬�ı任������Ը�����
	std::vector<D3DXMATRIX>			frameMat;

	//��ǰ֡�����ϳɵĺ�任�����Meshԭ�㣩�����ϵ��¾�����ˣ��ؼ�֡�б�
	D3DXMATRIX						combinedMat;

	//����SkinInfo�ı任����
	D3DXMATRIX						boneSkinMat;


public:
	//��������������ӹ�����ƫ�ƾ���
	VOID							updateBonesOffsetMatrix(D3DXMATRIX* pMatTransform);

	//���¹Ǽ������й����ĺϳɾ���
	VOID							updateBonesCombinedMatrix(D3DXMATRIX* pMatTransform, DWORD frameIndex);

	//���¹Ǽ������й��������Meshԭ��ı任��������ISKinInfo��UpdateSkinMesh����
	VOID							updateBonesSkinMatrix();
public:
	CBone(void);
	~CBone(void);
};
