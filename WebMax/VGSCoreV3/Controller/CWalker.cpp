/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // CWalker.cpp
* ����ժҪ�� // ����ģʽʵ��
* ����˵���� // 
* ��ǰ�汾�� // V2.0
* ��    �ߣ� // ���²�
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
* ---------------------------------------------------------------
* 
*
*********************************************************************/

#include "CWalker.h"

CWalker::CWalker(const WalkerInfo &Info)
{
	m_TranslateVector.y = 0;
	m_TranslateVector.z = 0;

	m_disX = 0;
	m_disY = 0;

	m_pCamera    = NULL;

	SetWalkerInfo(Info);
}
CWalker::~CWalker()
{
	m_pCamera = NULL;
}

// �õ�����������
CONTROLTYPE CWalker::GetControlType()  
{
	return CT_WALKER;
}

string CWalker::GetName()
{
	return string(m_WalkerInfo.Name);
}
void CWalker::SetName(const string &sName) // ����control��״̬
{
	ZeroMemory(m_WalkerInfo.Name, sizeof(m_WalkerInfo.Name));
	strcpy_s(m_WalkerInfo.Name, sizeof(m_WalkerInfo.Name), sName.c_str());
}

void CWalker::SetCamera(CCamera *pCamera)  // �����麯�����������������������Ĵ���
{
	if (pCamera != m_pCamera)
	{
		m_pCamera = pCamera;
		
		ZeroMemory(m_WalkerInfo.Camera, sizeof(m_WalkerInfo.Camera) / sizeof(m_WalkerInfo.Camera[0]));

		string str = m_pCamera->getName();
		memcpy(m_WalkerInfo.Camera, str.c_str(), strlen(str.c_str()));
	}
}

// ����control��״̬
void CWalker::Update()                                             
{
	float realYawSpeed  = 0; // ̧ͷ����ͷ�ĽǶ�
	float realPitchSpeed = 0; // ����ת��ĽǶ�
	float step           = 0; // ��ǰ��������ߵľ���
	float liftSpeed      = 0;  // �����½����ٶ�

	float timespan = (float)m_FrameEvent.timeSinceLastFrame; // ����һ֡��ʱ����, second
	
	if (!m_bMouseProcessed)
	{
		if (m_MouseInfo.mouseType == MOUSE_TYPE_LEFT)  // �����קʵ����ת
		{
			m_disX = m_MouseInfo.x - m_lastMouseInfo.x;   // ��Ļ�� x �������λ��, ��������ת��
			m_disY = m_MouseInfo.y - m_lastMouseInfo.y;       // ��Ļ�� y �������λ�ƣ�����̧ͷ���ͷ

			if (m_disX != 0)
			{
				realYawSpeed = -(float)m_disX * m_WalkerInfo.rosSpeed / 800.0f;   // �� x ���λ�Ƽ���Ƕȣ�����Ϊ����ֵ
			}
			if (m_disY != 0)
			{
				realPitchSpeed = -(float)m_disY * m_WalkerInfo.rosSpeed / 800.0f;   // �� y ���λ�Ƽ���Ƕȣ�����Ϊ����ֵ
			}	
		}
		m_bMouseProcessed = true;
	}


	if (!m_bKeyProcessed)  // ��������¼���ʵ����ת
	{

		if (m_Keyboard.key == VK_LEFT)
		{
			realYawSpeed = m_WalkerInfo.rosSpeed * timespan;  // ��ת
		}
		if (m_Keyboard.key == VK_RIGHT)
		{	
			realYawSpeed = -m_WalkerInfo.rosSpeed * timespan;	 // ��ת
		}
		if (m_Keyboard.key == VK_UP)
		{
			step = -m_WalkerInfo.walkSpeed * timespan;  // ǰ��
		}			
		if (m_Keyboard.key == VK_DOWN)
		{
			step = m_WalkerInfo.walkSpeed * timespan;  // ����
		}

		if (!m_WalkerInfo.bWeight)  // ���ʧȥ����
		{
			if (m_Keyboard.key == VK_NEXT)   // �½�
			{
				liftSpeed = -m_WalkerInfo.liftSpeed * timespan;
			}
			if (m_Keyboard.key == VK_PRIOR)  // ����
			{
				liftSpeed = m_WalkerInfo.liftSpeed * timespan;
			}			
		}
		else // ���������Ӱ��
		{
			if (m_Keyboard.key == VK_NEXT)   // ��ͷ
			{
				realPitchSpeed = -m_WalkerInfo.rosSpeed * timespan / 5.0f;
			}
			if (m_Keyboard.key == VK_PRIOR)  // ̧ͷ
			{
				realPitchSpeed = m_WalkerInfo.rosSpeed * timespan / 5.0f;
			}
		}
		m_bKeyProcessed = true;
	} // if 

	// ʵ���������ת
	if (realPitchSpeed != 0)  // y���λ��
	{
		if (realPitchSpeed > 0.0001 || realPitchSpeed < -0.0001) // != 0
		{
			D3DXVECTOR3 vRot(realPitchSpeed, 0, 0);
			m_pCamera->Rotate(&vRot, VRS_WORLD2);		
		}
	}

	// �� y ����ת
	if (realYawSpeed != 0)
	{		
		D3DXVECTOR3 vRot(0, realYawSpeed, 0);
		m_pCamera->Rotate(&vRot, VRS_WORLD2);  // �ƴ���from��ƽ������������ϵ��������ת
	}

	// ǰ�������
	if (step != 0)
	{
		D3DXVECTOR3 vRot(0, 0, step);
		m_pCamera->Move(vRot, VTS_WORLD);
	}

	// ����
	if (liftSpeed != 0)
	{
		D3DXVECTOR3 vRot(0, liftSpeed, 0);
		m_pCamera->Move(vRot, VTS_WORLD);
		// m_pCamera->moveRelative(Vector3(0, liftSpeed, 0));
	}
	
}

// ���ò���
void CWalker::SetWalkerInfo(const WalkerInfo &Info)
{
	m_WalkerInfo = Info;

	m_MaxUpperAngle = Info.upperAngle *  0.0174533f;  // �Ƕ�ת��Ϊ����
	m_MaxLowerAngle = -Info.lowerAngle *  0.0174533f;  // �Ƕ�ת��Ϊ���� 
}    

// ��ȡ����
void CWalker::GetWalkerInfo(WalkerInfo &Info)
{
	Info = m_WalkerInfo;
}      