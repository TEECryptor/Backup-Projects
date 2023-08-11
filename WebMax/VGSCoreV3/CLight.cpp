// CLight.cpp: implementation of the CLight class.
//
//////////////////////////////////////////////////////////////////////

#include "CLight.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74900A4
CLight::CLight(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_type = VGS_MOVABLETYPE_LIGHT;

	m_pd3dDevice = pd3dDevice;

	enable = TRUE;
	kfrCnt = 0;			
	kfrCurFrame = 0;
	currentTimer = 0;
	kfrFrameTime = 40;	
	kfrRepeatCnt = -1;
	kfrRepeatIndex = 1;	//��1��ʼ��������1�β���
	isPlaying = FALSE;
	playKfr = TRUE;
	multiply = 1.0f;
	diffuse.r = 1.0f;
	diffuse.g = 1.0f;
	diffuse.b = 1.0f;
	diffuse.a = 1.0f;
	ambient = 0.1f * diffuse;

	//һ��Ҫ��D3DLIGHT8�����в�����ʼ��Ϊ0
	ZeroMemory(&param, sizeof(param));
	pAnimOmniLight = NULL;
    pAnimDirectionalLight = NULL;
	pAnimSpotLight = NULL;
//	kfrInfo = NULL;
}

//##ModelId=47EBC74900A5
CLight::~CLight()
{

}

//-------------------------------------------------------------------
//����: move()
//����: 
//��;: �ƶ��ƹ�
//-------------------------------------------------------------------
//##ModelId=47EBC7490097
VOID CLight::move(D3DXVECTOR3* pVec)
{
	position.x = param.Position.x += pVec->x;
	position.y = param.Position.y += pVec->y;
	position.z = param.Position.z += pVec->z;	
}

//-------------------------------------------------------------------
//����: move()
//����: 
//��;: ��ת�ƹ�
//-------------------------------------------------------------------
//##ModelId=47EBC7490099
VOID CLight::rotate(D3DXVECTOR3* pVec)
{
	D3DXMATRIX rosMat;
	D3DXMatrixRotationYawPitchRoll(&rosMat, pVec->x, pVec->y, pVec->z);
	direction.x = param.Direction.x;
	direction.y = param.Direction.y;
	direction.z = param.Direction.z;
	D3DXVec3TransformCoord(&direction, &direction, &rosMat);
	param.Direction.x = direction.x;
	param.Direction.y = direction.y;
	param.Direction.z = direction.z;
}

//-------------------------------------------------------------------
//����: setPosition()
//����: 
//��;: ���õƹ��λ��
//-------------------------------------------------------------------
//##ModelId=47EBC749009D
VOID CLight::setPosition(D3DXVECTOR3* pVec)
{
	position = *pVec;
	param.Position.x = position.x;
	param.Position.y = position.y;
	param.Position.z = position.z;
}

//-------------------------------------------------------------------
//����: setDirection()
//����: 
//��;: ���õƹ�ķ���
//-------------------------------------------------------------------
//##ModelId=47EBC749009F
VOID CLight::setDirection(D3DXVECTOR3* pDir)
{
	direction = *pDir;
	param.Direction.x = direction.x;
	param.Direction.y = direction.y;
	param.Direction.z = direction.z;
}

//-------------------------------------------------------------------
//����: getPosition()
//����: 
//��;: �õ��ƹ��λ��
//-------------------------------------------------------------------
//##ModelId=47EBC74900A1
D3DXVECTOR3* CLight::getPosition()
{
	return &position;
}

//-------------------------------------------------------------------
//����: getDirection()
//����: 
//��;: �õ��ƹ�ķ���
//-------------------------------------------------------------------
//##ModelId=47EBC74900A2
D3DXVECTOR3* CLight::getDirection()
{
	return &direction;
}
//��¡�ƹ�
CLight*	CLight::clone(std::string newLightName)
{
	CLight* pNewLight = new CLight(m_pd3dDevice);
	
	//OMNI
	if (param.Type == D3DLIGHT_POINT)
	{
		//�ƹ������Ϣ
		pNewLight->ambient = ambient;
		pNewLight->diffuse = diffuse;
		//ǿ��
		pNewLight->multiply = multiply;

		pNewLight->param.Type= param.Type;
		pNewLight->param.Diffuse = param.Diffuse;
		pNewLight->param.Specular = param.Specular;
		pNewLight->param.Range = param.Range;
		pNewLight->param.Attenuation0 = param.Attenuation0;
		pNewLight->param.Attenuation1 = param.Attenuation1;
		pNewLight->param.Attenuation2 = param.Attenuation2;
		
		//������Ϣ
		if (pAnimOmniLight)
		{
			pNewLight->pAnimOmniLight = new N3D_OMNI[kfrCnt];
			for (UINT i = 0; i< kfrCnt; i++)
			{
				pNewLight->pAnimOmniLight[i].color = pAnimOmniLight[i].color;
				pNewLight->pAnimOmniLight[i].multiply = pAnimOmniLight[i].multiply;
				pNewLight->pAnimOmniLight[i].attenuationRange = pAnimOmniLight[i].attenuationRange;
				pNewLight->pAnimOmniLight[i].attenuation0 = pAnimOmniLight[i].attenuation0;
				pNewLight->pAnimOmniLight[i].attenuation1 = pAnimOmniLight[i].attenuation1;
				pNewLight->pAnimOmniLight[i].attenuation2 = pAnimOmniLight[i].attenuation2;
			}
		}
	}

	//SPOT	
	if (param.Type == D3DLIGHT_SPOT)
	{
		//�ƹ������Ϣ
		pNewLight->ambient = ambient;
		pNewLight->diffuse = diffuse;
		//ǿ��
		pNewLight->multiply = multiply;

		pNewLight->param.Type= param.Type;
		pNewLight->param.Diffuse = param.Diffuse;
		pNewLight->param.Specular = param.Specular;
		pNewLight->param.Range = param.Range;

		pNewLight->param.Attenuation0 = param.Attenuation0;
		pNewLight->param.Attenuation1 = param.Attenuation1;
		pNewLight->param.Attenuation2 = param.Attenuation2;
		
		// ����������ϵ�еķ���
		pNewLight->param.Direction = param.Direction;

        //��������ķ�Χ��
		pNewLight->param.Theta = param.Theta;
		pNewLight->param.Phi = param.Phi;
		pNewLight->param.Falloff = param.Falloff;

		//������Ϣ
		if (pAnimSpotLight)
		{
			pNewLight->pAnimSpotLight = new N3D_SPOTLIGHT[kfrCnt];
			for (UINT i = 0; i< kfrCnt; i++)
			{
				pNewLight->pAnimSpotLight[i].color = pAnimOmniLight[i].color;
				pNewLight->pAnimSpotLight[i].multiply = pAnimSpotLight[i].multiply;
				pNewLight->pAnimSpotLight[i].direction = pAnimSpotLight[i].direction;
				pNewLight->pAnimSpotLight[i].attenuationRange = pAnimSpotLight[i].attenuationRange;
				pNewLight->pAnimSpotLight[i].attenuation0 = pAnimSpotLight[i].attenuation0;
				pNewLight->pAnimSpotLight[i].attenuation1 = pAnimSpotLight[i].attenuation1;
				pNewLight->pAnimSpotLight[i].attenuation2 = pAnimSpotLight[i].attenuation2;
				pNewLight->pAnimSpotLight[i].innerAngle = pAnimSpotLight[i].innerAngle;
				pNewLight->pAnimSpotLight[i].outerAngle = pAnimSpotLight[i].outerAngle;
				pNewLight->pAnimSpotLight[i].fallOff = pAnimSpotLight[i].fallOff;
			}
		}
	}

	//Direction	
	if (param.Type == D3DLIGHT_DIRECTIONAL)
	{
		//�ƹ������Ϣ
		pNewLight->ambient = ambient;
		pNewLight->diffuse = diffuse;
		//ǿ��
		pNewLight->multiply = multiply;

		pNewLight->param.Type= param.Type;
		pNewLight->param.Diffuse = param.Diffuse;
		pNewLight->param.Specular = param.Specular;
		pNewLight->param.Range = param.Range;

		pNewLight->param.Attenuation0 = param.Attenuation0;
		pNewLight->param.Attenuation1 = param.Attenuation1;
		pNewLight->param.Attenuation2 = param.Attenuation2;
		
		// ����������ϵ�еķ���
		pNewLight->param.Direction = param.Direction;
        
		//������Ϣ
		if (pAnimDirectionalLight)
		{
			pNewLight->pAnimDirectionalLight = new N3D_DIRECTIONAL[kfrCnt];
			for (UINT i = 0; i< kfrCnt; i++)
			{
				pNewLight->pAnimDirectionalLight[i].color = pAnimDirectionalLight[i].color;
				pNewLight->pAnimDirectionalLight[i].multiply = pAnimDirectionalLight[i].multiply;
				pNewLight->pAnimDirectionalLight[i].direction = pAnimDirectionalLight[i].direction;
				pNewLight->pAnimDirectionalLight[i].attenuationRange = pAnimDirectionalLight[i].attenuationRange;
				pNewLight->pAnimDirectionalLight[i].attenuation0 = pAnimDirectionalLight[i].attenuation0;
				pNewLight->pAnimDirectionalLight[i].attenuation1 = pAnimDirectionalLight[i].attenuation1;
				pNewLight->pAnimDirectionalLight[i].attenuation2 = pAnimDirectionalLight[i].attenuation2;
			}
		}
	}
	return pNewLight;
}

//���õƹ�ļ���״̬
void CLight::SetLightEnable(bool status)
{
	m_pd3dDevice->SetLight(index, &param);
	m_pd3dDevice->LightEnable(index, status);
}


//-------------------------------------------------------------------
//����: Release()
//����: 
//��;: �ͷ��ڴ�
//-------------------------------------------------------------------
//##ModelId=47EBC74900A3
VOID CLight::Release()
{
    SAFE_DELETE_ARRAY(pAnimOmniLight);        
	SAFE_DELETE_ARRAY(pAnimDirectionalLight); 
	SAFE_DELETE_ARRAY(pAnimSpotLight);
	//	SAFE_DELETE(kfrInfo);	
}


// VOID CLight::SetName(const std::string &sName)
// {
// 	name = sName;
// }
// std::string CLight::GetName()
// {
// 	return name;
// }

// �õ��ƹ�����
VGS_LIGHT_TYPE CLight::getType()
{
	VGS_LIGHT_TYPE type = VLT_OMNI;
	if (param.Type == D3DLIGHT_SPOT)
	{
		type = VLT_SPOT;
	}
	else if (param.Type == D3DLIGHT_DIRECTIONAL)
	{
		type = VLT_DIR;
	}
	
	return type;
}

// ���õƹ�����
VOID CLight::setType(VGS_LIGHT_TYPE type)
{
	if (type == VLT_OMNI)
	{
		param.Type = D3DLIGHT_POINT;
	}
	else if (type == VLT_SPOT)
	{
		param.Type = D3DLIGHT_SPOT;
	}
	else if (type == VLT_DIR)
	{
		param.Type = D3DLIGHT_DIRECTIONAL;
	}
}

void CLight::UpdateFromNode()
{
	position = m_pNode->getPosition(VTS_WORLD);
}