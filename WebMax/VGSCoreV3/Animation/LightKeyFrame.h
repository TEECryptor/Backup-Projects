
#pragma once

#include "NodeKeyFrame.h"
#include "..\VGSHeader.h"
#include "..\Node.h"
#include "..\CLight.h"

//�ƹ�Ŀɱ����S3D
typedef struct VGS_DIRLIGHT_KFR_PARAM
{			
	D3DXVECTOR3             dir;   // �ƹ�ķ�����������ϵ	
}VGS_DIRLIGHT_KFR_PARAM;

typedef struct VGS_SPOTLIGHT_KFR_PARAM
{	
	//�ڽ����
	float					theta;     // �ڽ�
	float					phi;       // ���
	float					falloff;   // ˥��
	
	D3DXVECTOR3             dir;       // �ƹ�ķ�����������ϵ
	
}VGS_SPOTLIGHT_KFR_PARAM;

class VGS_EXPORT CLightKeyFrame : public CNodeKeyFrame
{
public:
	CLightKeyFrame(CSceneNode *pNode);
	~CLightKeyFrame();
	
	VOID Release(); 
	VOID update();  // ���ط���
	VGS_ANIMATIONTYPE getType();
	
private:

	D3DXVECTOR3         m_pos;           // ��������ϵλ��	
	D3DCOLORVALUE       m_color;         // ��ɫ, ���Ѿ�����multiply�Ľ��	
	// float		        m_multiply;      // ǿ��

	// ��Χ��˥��
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

	bool m_bHasParamAnimation;  // �Ƿ��нڵ�仯֮�����������������������ɫ/�����
};