// NodeKeyFrame.cpp: implementation of the CNodeKeyFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "NodeKeyFrame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeKeyFrame::CNodeKeyFrame(CSceneNode* pNode)
{
	m_pNode = pNode;
}

CNodeKeyFrame::~CNodeKeyFrame()
{
	Release();
}

VOID CNodeKeyFrame::Release()
{
	m_pNode = NULL;
}

VOID CNodeKeyFrame::setTranslate(const VECTOR3 &vPos)
{
	m_pos = D3DXVECTOR3(vPos.x, vPos.y, vPos.z);
}  
VOID CNodeKeyFrame::setRotation(const VGS_QUATERNION &quat)
{
	m_quat = D3DXQUATERNION(quat.x, quat.y, quat.z, quat.w);
}  
VOID CNodeKeyFrame::setScale(const VECTOR3 &scale)
{	
	m_scale = D3DXVECTOR3(scale.x, scale.y, scale.z);
}  
// 更新节点
VOID CNodeKeyFrame::update()
{
	if (m_pNode)
	{
		//获得变换矩阵
		D3DXMATRIX transMat;
		D3DXMatrixTransformation(&transMat, 
								&D3DXVECTOR3(0,0,0), 
								&D3DXQUATERNION(0, 0, 0, 0), 
								&m_scale, 
								&D3DXVECTOR3(0,0,0), 
								&m_quat, 
								&m_pos);

		m_pNode->setMatrix(transMat, VTS_PARENT);

		/*m_pNode->setScale(&m_scale, VTS_PARENT);
		m_pNode->setRotationByQuat(&m_quat, VTS_PARENT);
		m_pNode->setPosition(&m_pos,VTS_PARENT);*/
	}
}  
