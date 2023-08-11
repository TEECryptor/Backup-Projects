
#pragma once

#include "NodeKeyFrame.h"
#include "..\VGSHeader.h"
#include "..\Node.h"
#include "..\CCamera.h"

class VGS_EXPORT CCameraKeyFrame : public CNodeKeyFrame
{
public:
	CCameraKeyFrame(CSceneNode *pNode);
	~CCameraKeyFrame();

	VOID Release(); 
	VOID update();  // 重载方法
	VGS_ANIMATIONTYPE getType();

private:
	CCamera*     m_Camera;
	D3DXVECTOR3  m_from;
	D3DXVECTOR3  m_at;
	// D3DXVECTOR3  m_up;
	FLOAT    m_nearClip;
	FLOAT    m_farClip;
	FLOAT    m_Roll;
	FLOAT    m_fov;

public:
	VOID setFrom(const VECTOR3 &vPos);
	VOID setAt(const VECTOR3 &vAt);
	VOID setClip(float nearClip, float farClip);
	VOID setFov(float fov);
	VOID setRoll(float roll);
	VOID setNearClip(float nearClip);
	VOID setFarClip(float farClip);

	float getFov(){return m_fov;}
	float getRoll(){return m_Roll;}
	float getNearClip(){return m_nearClip;}
	float getFarClip(){return m_farClip;}

	bool m_bHasParamAnimation;  // 是否有节点变化之外的其他参数动画，例如fov，clip
};