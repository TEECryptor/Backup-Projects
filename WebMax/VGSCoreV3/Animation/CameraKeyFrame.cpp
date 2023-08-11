#include "CameraKeyFrame.h"

CCameraKeyFrame::CCameraKeyFrame(CSceneNode *pNode) : CNodeKeyFrame(pNode)
{
	m_fov = 45 * ATOS;
	m_nearClip = 5.0f;
	m_farClip = 1000000.0f;

	m_bHasParamAnimation = false;

	m_Camera = NULL;
	if (pNode && pNode->getMovableType() == VGS_MOVABLETYPE_CAMERA && pNode->getMovableObject() != 0)
	{
		m_Camera = (CCamera*)(pNode->getMovableObject());
	}
}
CCameraKeyFrame::~CCameraKeyFrame()
{
	Release();
}

VOID CCameraKeyFrame::Release()
{
	m_Camera = NULL;
	m_bHasParamAnimation = false;
}

VGS_ANIMATIONTYPE CCameraKeyFrame::getType()
{
	return VAT_ANIMATIONTYPE_CAMERA;
}

// ÖØÔØ·½·¨
VOID CCameraKeyFrame::update()
{
	if (m_Camera)
	{
		CNodeKeyFrame::update();

		if (m_bHasParamAnimation)
		{
			/*m_Camera->SetPosition(&m_from, VTS_WORLD);
			m_Camera->Lookat(&m_at, VTS_WORLD);
			m_Camera->SetRoll(m_Roll);*/
			m_Camera->SetFov(m_fov);
			m_Camera->setNearClip(m_nearClip);
			m_Camera->setFarClip(m_farClip);
		}

	}
}

VOID CCameraKeyFrame::setFrom(const VECTOR3 &vPos)
{
	m_from = D3DXVECTOR3(vPos.x, vPos.y, vPos.z);
}
VOID CCameraKeyFrame::setAt(const VECTOR3 &vAt)
{
	m_at = D3DXVECTOR3(vAt.x, vAt.y, vAt.z);
}
VOID CCameraKeyFrame::setClip(float nearClip, float farClip)
{
	m_nearClip = nearClip;
	m_farClip  = farClip;
}
VOID CCameraKeyFrame::setFov(float fov)
{
	m_fov = fov;
}
VOID CCameraKeyFrame::setRoll(float roll)
{
	m_Roll = roll;
}


VOID CCameraKeyFrame::setNearClip(float nearClip)
{
	m_nearClip = nearClip;
}
VOID CCameraKeyFrame::setFarClip(float farClip)
{
	m_farClip = farClip;
}