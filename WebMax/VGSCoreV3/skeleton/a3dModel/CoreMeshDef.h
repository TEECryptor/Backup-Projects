#ifndef __XBASE_COREMESH_DEFINE_H__
#define __XBASE_COREMESH_DEFINE_H__
#include "../xcomdoc/xcomdoc.h"
#include "CD3DInclude.h"
#include "TextureManager.h"

class _A3D_API_ CSkeletonID
{
public:
	int32 m_HiWord;
	int32 m_LoWord;
public:
	bool operator == (const CSkeletonID& rhv) const 
	{
		return (m_HiWord == rhv.m_HiWord) && (m_LoWord == rhv.m_LoWord);
	}
	bool operator != (const CSkeletonID& rhv) const
	{
		return (m_HiWord != rhv.m_HiWord) || (m_LoWord != rhv.m_LoWord);
	}
};




//���������Ķ���
class _A3D_API_ xSkinMeshVertex
{
public:
	D3DXVECTOR3     m_Position;
	D3DXVECTOR3     m_Normal;
	D3DCOLOR        m_Diffuse;
	D3DXVECTOR3     m_Tangent;
	float           m_weight[4];
	unsigned short  m_windex[4];
	

	//==========================
	D3DXVECTOR2     m_uv[1];
};

//��̬Mesh�Ķ���
class _A3D_API_ xStaticMeshVertex
{
public:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Normal;
	D3DCOLOR    m_Diffuse;
	//D3DXVECTOR4 m_Tangent;

	//==========================
	D3DXVECTOR2 m_uv[1];
};

class _A3D_API_ CBoneTrans
{
public:
	//Bone����תQuaternian
	D3DXQUATERNION m_Rotate;
	//Bone��ƽ������
	D3DXVECTOR3    m_Trans;
	D3DXVECTOR3    m_Scale;
public:
    void toMatrix(D3DXMATRIX& mat);
};

class _A3D_API_ xBoneData
{
public:
	//��Ϻ������
	D3DXMATRIX       m_Matrix;
	//Bone��ƽ������
	CBoneTrans       m_BoneTrans;
	//��Ը��ڵ�ģ�
	D3DXMATRIX       m_LocaleTM;

public:
    void fromBoneTrans(CBoneTrans& trans ,const D3DXMATRIX& wsMat , const D3DXMATRIX& InitTMInv)
    {
        m_BoneTrans = trans;
        trans.toMatrix(m_LocaleTM);
        D3DXMatrixMultiply(&m_Matrix, &InitTMInv , &wsMat);
    }
};

class _A3D_API_ xMeshFace
{
public:
    int m_Idx[3];
};


class _A3D_API_ xSkinBone
{
public:
	//Bone��Name
	wchar_t          m_BoneName[32];
	union{
	int32            m_ParentIndex;//�����������ParentIndex
	int32            m_UC4AddStart;
	};

	int32            m_ParentBoneID; //����ǰ�������ParentID;
	int32            m_BoneID;
	//��ʼ����
	D3DXMATRIX  m_InitMT;
	D3DXMATRIX  m_InitMTInv;
};

typedef std::vector<xSkinBone > vSkinBones;

enum eActionType
{
     eActType_None          = 0, 
     eActType_Skeleton      = 1,
	 eActType_Keyframe      = 2,
	 eActType_Frame         = 3,
	 eActType_Blend         = 4,//���������ں�(���������İ������ں�)
	 eActType_Transition    = 5,//������������
	 eActType_Mix           = 6,//��������Ļ��
};

class _A3D_API_ CActionInfo
{
public:
	int32             m_lTime;         //��������ʱ�䳤��
	int32             m_iFirstFrame;   //������һ֡
	int32             m_iLastFrame;    //�������һ֡
	int32             m_nFrame;
	eActionType       m_eActType;

};


#endif
