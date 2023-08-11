// NodeKeyFrame.h: interface for the CNodeKeyFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEKEYFRAME_H__6019323F_BD7A_4BF5_83D7_7FACE1F19823__INCLUDED_)
#define AFX_NODEKEYFRAME_H__6019323F_BD7A_4BF5_83D7_7FACE1F19823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\VGSHeader.h"
#include "..\Node.h"
// 
// enum _KEYFRAME_TYPE
// {
// 	_KFT_NONE = 0,
// 	_KFT_MODEL,
// 	_KFT_CAMERA,
// 	_KFT_LIGHT
// };

// �ڵ�ؼ�֡������һ֡
class VGS_EXPORT CNodeKeyFrame  
{
public:
	CNodeKeyFrame(CSceneNode* pNode);
	virtual ~CNodeKeyFrame();
	
	// �ӿ�
	virtual VGS_ANIMATIONTYPE getType() = 0;
	virtual VOID Release();
	virtual VOID update();  // ���½ڵ�

	virtual VOID setTranslate(const VECTOR3 &vPos);
	virtual VOID setRotation(const VGS_QUATERNION &quat);
	virtual VOID setScale(const VECTOR3 &scale);
	
public:
	CSceneNode*       m_pNode;  // �ڵ�

	D3DXQUATERNION    m_quat;
	D3DXVECTOR3       m_pos;
	D3DXVECTOR3       m_scale;

	// BOOL              m_bInitialed;  // �Ƿ��Ѿ���ʼ��
};

#endif // !defined(AFX_NODEKEYFRAME_H__6019323F_BD7A_4BF5_83D7_7FACE1F19823__INCLUDED_)
