
#pragma once

#include "NodeKeyFrame.h"
#include "..\VGSHeader.h"
#include "..\Node.h"
#include "..\CLight.h"

//灯光的可变参数S3D
typedef struct VGS_DIRLIGHT_KFR_PARAM
{			
	D3DXVECTOR3             dir;   // 灯光的方向，世界坐标系	
}VGS_DIRLIGHT_KFR_PARAM;

typedef struct VGS_SPOTLIGHT_KFR_PARAM
{	
	//内角外角
	float					theta;     // 内角
	float					phi;       // 外角
	float					falloff;   // 衰减
	
	D3DXVECTOR3             dir;       // 灯光的方向，世界坐标系
	
}VGS_SPOTLIGHT_KFR_PARAM;

class VGS_EXPORT CLightKeyFrame : public CNodeKeyFrame
{
public:
	CLightKeyFrame(CSceneNode *pNode);
	~CLightKeyFrame();
	
	VOID Release(); 
	VOID update();  // 重载方法
	VGS_ANIMATIONTYPE getType();
	
private:

	D3DXVECTOR3         m_pos;           // 世界坐标系位置	
	D3DCOLORVALUE       m_color;         // 颜色, 是已经乘了multiply的结果	
	// float		        m_multiply;      // 强度

	// 范围和衰减
	float       m_Range;
	float       m_Attenuation0;
	float       m_Attenuation1;
	float       m_Attenuation2;

	VGS_DIRLIGHT_KFR_PARAM*   m_pDirParam;
	VGS_SPOTLIGHT_KFR_PARAM*  m_pSpotParam;

public:
	CLight*		        m_pLight;

	VOID setPosition(const VECTOR3 &vPos);
	VOID setColor(float r, float g, float b, float a);
	VOID setRange(float range = (float)MAXDWORD);
	VOID setAttenuation(float attenuation0 = 1, float attenuation1 = 0, float attenuation2 = 0);
	VOID setDirection(const VECTOR3 &dir);
	VOID setAngle(float theta, float phi, float fallof = 1.0);

	D3DCOLORVALUE getColor(){return m_color;}
	float         getRange(){return m_Range;}
	VECTOR3       getDirection()
	{
		if (m_pDirParam)
		{
			return VECTOR3(m_pDirParam->dir.x, m_pDirParam->dir.y, m_pDirParam->dir.z);
		}
		else if (m_pSpotParam)
		{
			return VECTOR3(m_pSpotParam->dir.x, m_pSpotParam->dir.y, m_pSpotParam->dir.z);
		}
		return VECTOR3(0, 0, 0);
	}
	void getAngle(float &theta, float &phi, float &fallof)
	{
		if (m_pSpotParam)
		{
			theta  = m_pSpotParam->theta;
			phi    = m_pSpotParam->phi;
			fallof = m_pSpotParam->falloff;
		}
		
	}

	bool m_bHasParamAnimation;  // 是否有节点变化之外的其他参数动画，例如颜色/方向等
};