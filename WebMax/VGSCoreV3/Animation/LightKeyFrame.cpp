#include "LightKeyFrame.h"

CLightKeyFrame::CLightKeyFrame(CSceneNode *pNode) : CNodeKeyFrame(pNode)
{	
	m_bHasParamAnimation = false;

	m_pLight = NULL;
	if (pNode && pNode->getMovableType() == VGS_MOVABLETYPE_LIGHT)
	{
		m_pLight = (CLight*)(pNode->getMovableObject());
	}

	m_pDirParam  = NULL;
	m_pSpotParam = NULL;
}

CLightKeyFrame::~CLightKeyFrame()
{
	Release();
}

VOID CLightKeyFrame::Release()
{
	SAFE_DELETE(m_pSpotParam);
	SAFE_DELETE(m_pDirParam);

	m_pLight = NULL;
	m_bHasParamAnimation = false;
}

// 重载方法
VOID CLightKeyFrame::update()
{
	if (m_pLight)
	{
		CNodeKeyFrame::update();
		// m_pLight->setPosition(&m_pos);

		 D3DXVECTOR3* v3pPos = m_pLight->getPosition();
		 m_pLight->param.Position.x = v3pPos->x;
		 m_pLight->param.Position.y = v3pPos->y;
		 m_pLight->param.Position.z = v3pPos->z;


		if (m_bHasParamAnimation)
		{
			m_pLight->param.Diffuse  = m_color;
			m_pLight->param.Specular = m_color;

			m_pLight->param.Range = m_Range;
			m_pLight->param.Attenuation0 = m_Attenuation0;
			m_pLight->param.Attenuation1 = m_Attenuation1;
			m_pLight->param.Attenuation2 = m_Attenuation2;

			if (m_pLight->getType() == VLT_DIR && m_pDirParam)
			{
				m_pLight->setDirection(&m_pDirParam->dir);
			}
			else if (m_pLight->getType() == VLT_SPOT && m_pSpotParam)
			{
				m_pLight->setDirection(&m_pSpotParam->dir);
				m_pLight->param.Theta = m_pSpotParam->theta;
				m_pLight->param.Phi = m_pSpotParam->phi;
				m_pLight->param.Falloff = m_pSpotParam->falloff;
			}
		}

		m_pLight->SetLightEnable(true);  // 更新参数，并使能
	}
}

VOID CLightKeyFrame::setPosition(const VECTOR3 &vPos)
{
	m_pos = D3DXVECTOR3(vPos.x, vPos.y, vPos.z);
}
VOID CLightKeyFrame::setColor(float r, float g, float b, float a)
{
	m_color = D3DXCOLOR(r, g, b, a);
}
VOID CLightKeyFrame::setRange(float range/* = (float)MAXDWORD*/)
{
	m_Range = range;
	if (m_Range < 0)
	{
		m_Range = (float)MAXDWORD;
	}
}
VOID CLightKeyFrame::setAttenuation(float attenuation0/* = 1*/, float attenuation1/* = 0*/, float attenuation2/* = 0*/)
{
	m_Attenuation0 = attenuation0;
	m_Attenuation1 = attenuation1;
	m_Attenuation2 = attenuation2;
}
VOID CLightKeyFrame::setDirection(const VECTOR3 &dir)
{
	if (m_pLight->getType() == VLT_DIR)
	{
		if (!m_pDirParam)
		{
			m_pDirParam = new VGS_DIRLIGHT_KFR_PARAM();
		}
		m_pDirParam->dir = D3DXVECTOR3(dir.x, dir.y, dir.z);
	}
	else if (m_pLight->getType() == VLT_SPOT)
	{
		if (!m_pSpotParam)
		{
			m_pSpotParam = new VGS_SPOTLIGHT_KFR_PARAM();
		}
		m_pSpotParam->dir = D3DXVECTOR3(dir.x, dir.y, dir.z);
	}

}
VOID CLightKeyFrame::setAngle(float theta, float phi, float fallof/* = 1.0*/)
{
	if (m_pLight->getType() == VLT_SPOT)
	{
		if (!m_pSpotParam)
		{
			m_pSpotParam = new VGS_SPOTLIGHT_KFR_PARAM();
		}
		m_pSpotParam->theta   = theta;
		m_pSpotParam->phi     = phi;
		m_pSpotParam->falloff = fallof;
	}
}

VGS_ANIMATIONTYPE CLightKeyFrame::getType()
{
	return VAT_ANIMATIONTYPE_LIGHT;
}