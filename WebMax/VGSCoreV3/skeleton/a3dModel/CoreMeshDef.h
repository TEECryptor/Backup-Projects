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




//骨骼动画的顶点
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

//静态Mesh的顶点
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
	//Bone的旋转Quaternian
	D3DXQUATERNION m_Rotate;
	//Bone的平移向量
	D3DXVECTOR3    m_Trans;
	D3DXVECTOR3    m_Scale;
public:
    void toMatrix(D3DXMATRIX& mat);
};

class _A3D_API_ xBoneData
{
public:
	//组合后的向量
	D3DXMATRIX       m_Matrix;
	//Bone的平移向量
	CBoneTrans       m_BoneTrans;
	//相对父节点的．
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
	//Bone的Name
	wchar_t          m_BoneName[32];
	union{
	int32            m_ParentIndex;//导出后，这个是ParentIndex
	int32            m_UC4AddStart;
	};

	int32            m_ParentBoneID; //导出前，这个是ParentID;
	int32            m_BoneID;
	//初始矩阵
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
	 eActType_Blend         = 4,//两个动作融合(两个动作的按比例融合)
	 eActType_Transition    = 5,//两个动作过渡
	 eActType_Mix           = 6,//多个动作的混合
};

class _A3D_API_ CActionInfo
{
public:
	int32             m_lTime;         //动作延续时间长短
	int32             m_iFirstFrame;   //动作第一帧
	int32             m_iLastFrame;    //动作最后一帧
	int32             m_nFrame;
	eActionType       m_eActType;

};


#endif
