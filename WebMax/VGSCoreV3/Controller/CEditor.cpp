/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // CEditor.cpp
* ����ժҪ�� // ���չʾ����ʵ��
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


#include "CEditor.h"
#include "..\RenderWindow.h"
#include "..\\VGSCore.h"

//int CEditor::m_EditorIndex = 0;

CEditor::CEditor(CScene* pMgr, const EditorInfo &info)
{

	m_disX = 0; 
	m_disY = 0;

	// m_TargetNode = NULL;
	m_pSceneMgr  = pMgr;
	m_pCamera    = NULL;

	// m_pTargetModel = NULL;

	//if (m_pSceneMgr)  
	//{
		// m_TargetNode = NULL;
	//	m_TargetNode = m_pSceneMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode(/*str*/);
	//}

	SetEditorInfo(info);	
}
CEditor::~CEditor()
{
	//if (m_TargetNode)
	//{
	//	m_TargetNode->detachAllObjects();
	//	
	//	// ɾ���ڵ�
	//	m_pSceneMgr->mSceneMgr->destroySceneNode(m_TargetNode->getName());
	//}

	m_pCamera = NULL;
	m_pSceneMgr = NULL;
}

// �õ�����������
CONTROLTYPE CEditor::GetControlType()  
{
	return CT_EDITOR;
}

string CEditor::GetName()
{
	return string(m_EditorInfo.Name);
}
void CEditor::SetName(const string &sName) // ����control��״̬
{
	ZeroMemory(m_EditorInfo.Name, sizeof(m_EditorInfo.Name));
	strcpy_s(m_EditorInfo.Name, sizeof(m_EditorInfo.Name), sName.c_str());
}

void CEditor::SetCamera(CCamera *pCamera)  // �����麯�����������������������Ĵ���
{
	if (!pCamera)
	{
		return;
	}
	if (pCamera != m_pCamera)
	{
		m_pCamera = pCamera;
		
		// �����������
		ZeroMemory(m_EditorInfo.Camera, sizeof(m_EditorInfo.Camera) / sizeof(m_EditorInfo.Camera[0]));

		string str = m_pCamera->getName();
		memcpy(m_EditorInfo.Camera, str.c_str(), strlen(str.c_str()));

	}
	// �Զ���׼Ŀ���
	// if (pCamera->m_pNode)
	{
		D3DXVECTOR3 vAt(m_EditorInfo.LookAtPos.x, m_EditorInfo.LookAtPos.y, m_EditorInfo.LookAtPos.z);
		pCamera->Lookat(&vAt, VTS_WORLD);
	}
}

// ����control��״̬
void CEditor::Update()                                             
{ 
	//if (!m_pCamera)
	//{
	//	return;
	//}

	//CSceneNode *pCamParent = m_pCamera->m_pNode;
	//if (!pCamParent)
	//{
	//	return;
	//}

	//float RotateSpeedX = 0;    // �� X ����ת�ĽǶ�
	//float RotateSpeedY = 0;    // �� Y ����ת�ĽǶ�

	//float timespan = (float)m_FrameEvent.timeSinceLastFrame; // ����һ֡��ʱ����, second
	//
	//// ������Զ���תģʽ
	//if (m_EditorInfo.bIsAutoRoundRoll)
	//{
	//	RotateSpeedX = m_EditorInfo.autoSpeedX * timespan * PI / 30.0f;
	//	RotateSpeedY = m_EditorInfo.autoSpeedY * timespan * PI / 30.0f;
	//}
	//else  // ���Զ���תģʽ
	//{
	//	if (!m_bMouseProcessed)
	//	{
	//		m_disX = m_MouseInfo.x - m_lastMouseInfo.x;   // ��Ļ�� x �������λ��, ��������ת��
	//		m_disY = m_MouseInfo.y - m_lastMouseInfo.y;       // ��Ļ�� y �������λ�ƣ�����̧ͷ���ͷ
	//		if (m_MouseInfo.mouseType == MOUSE_TYPE_MIDDLE && m_MouseInfo.hRenderWnd)  // �м�ƽ�����
	//		{
	//			//����Ļ�����趨ƽ���ϵĶ�Ӧ��
	//			D3DXVECTOR3 vCamDir = m_pCamera->GetDirection(VTS_WORLD);
	//			VECTOR3 vo, vx, vy;

	//			// һ����ͷ���ȷ��һ��ƽ��
	//			VECTOR3 planPoint(m_EditorInfo.LookAtPos.x, m_EditorInfo.LookAtPos.y, m_EditorInfo.LookAtPos.z);
	//			VECTOR3 planNormal(-vCamDir.x, -vCamDir.y, -vCamDir.z);

	//			// ������Ļ�ϵĵ�ӳ�䵽��ά�ռ�ָ��ƽ���ϵĵ�
	//			CViewport *pViewport = ((CRenderWindow*)m_MouseInfo.hRenderWnd)->GetViewport(m_pCamera);
	//			if (pViewport)
	//			{
	//				m_pSceneMgr->GetCoordScreenToWorldFromPlane(pViewport, m_MouseInfo.x, m_MouseInfo.y, planPoint, planNormal, vo);
	//				m_pSceneMgr->GetCoordScreenToWorldFromPlane(pViewport, m_MouseInfo.x + 1, m_MouseInfo.y, planPoint, planNormal, vx);
	//				m_pSceneMgr->GetCoordScreenToWorldFromPlane(pViewport, m_MouseInfo.x, m_MouseInfo.y + 1, planPoint, planNormal, vy);

	//				// ����Ļ��x��y�����λ�ƻ������������ϵ�е��ƶ�����
	//				vx.x = -m_disX * (vx.x - vo.x);
	//				vx.y = -m_disX * (vx.y - vo.y);
	//				vx.z = -m_disX * (vx.z - vo.z);
	//				vy.x = -m_disY * (vy.x - vo.x);
	//				vy.y = -m_disY * (vy.y - vo.y);
	//				vy.z = -m_disY * (vy.z - vo.z);
	//				vx.x += vy.x;
	//				vx.y += vy.y;
	//				vx.z += vy.z;

	//				MoveCamera((DWORD)m_pSceneMgr, (DWORD)m_pCamera, &vx, 0);   // �ƶ�һ�����λ��
	//			}
	//		}
	//		else if (m_MouseInfo.mouseType == MOUSE_TYPE_LEFT)  // �����קʵ����ת
	//		{
	//			if (m_EditorInfo.rosAxis == 0 || m_EditorInfo.rosAxis == 2) // ����xy���y����ת
	//			{
	//				if (m_disX != 0)
	//				{
	//					// RotateSpeedY = -(FLOAT)m_disX / 12.0f;   // �� x ���λ�Ƽ����� Y ����ת�ĽǶȣ�����Ϊ����ֵ
	//					RotateSpeedY = -(FLOAT)m_disX * m_EditorInfo.manualSpeedY / 200.0f;   // �� x ���λ�Ƽ����� Y ����ת�ĽǶȣ�����Ϊ����ֵ
	//					m_disX = (INT)(m_disX * 0.95f); // ���û�а��������ק������
	//				}
	//			}
	//			
	//			if (m_EditorInfo.rosAxis == 0 || m_EditorInfo.rosAxis == 1) // ����xy���x����ת
	//			{
	//				if (m_disY != 0)
	//				{
	//					// RotateSpeedX = -(FLOAT)m_disY / 8.0f;   // �� y ���λ�Ƽ����� X ����ת�ĽǶȣ�����Ϊ����ֵ
	//					RotateSpeedX = -(FLOAT)m_disY * m_EditorInfo.manualSpeedX / 200.0f;   // �� y ���λ�Ƽ����� X ����ת�ĽǶȣ�����Ϊ����ֵ
	//					m_disY = (INT)(m_disY * 0.95f);       // ���û�а��������ק������
	//				}
	//			}
	//		}

	//		m_bMouseProcessed = true;

	//	} // ��������¼�

	//	// ��������¼���ʵ����ת
	//	if (!m_bKeyProcessed)  
	//	{	
	//		if (m_Keyboard.key == VK_LEFT)
	//		{
	//			RotateSpeedY = m_EditorInfo.manualSpeedY / 50.0f/* * timespan*/;  // x���λ��, ��y��ת
	//		}
	//		if (m_Keyboard.key == VK_RIGHT)
	//		{	
	//			RotateSpeedY = -m_EditorInfo.manualSpeedY / 50.0f/* * timespan*/;	 // x���λ��, ��y��ת
	//		}
	//		if (m_Keyboard.key == VK_UP)
	//		{
	//			RotateSpeedX = m_EditorInfo.manualSpeedX / 50.0f/* * timespan*/;  // y���λ�ƣ���x��ת
	//		}			
	//		if (m_Keyboard.key == VK_DOWN)
	//		{
	//			RotateSpeedX = -m_EditorInfo.manualSpeedX / 50.0f/* * timespan*/;  // y���λ�ƣ���x��ת
	//		}

	//		// m_bKeyProcessed = true;

	//	} // ��������¼�

	//} // ���Զ���תģʽ

	//// �� �����right�� ��ת
	//if (RotateSpeedX != 0)
	//{
	//	D3DXVECTOR3 vTargetPoint(m_EditorInfo.LookAtPos.x, m_EditorInfo.LookAtPos.y, m_EditorInfo.LookAtPos.z);
	//	D3DXVECTOR3 vXAxis = m_pCamera->GetRight(VTS_WORLD);
	//	m_pCamera->RoundEx(&vTargetPoint, &vXAxis, RotateSpeedX);
	//}
	//// �� ��������ϵ��y�� ��ת
	//if (RotateSpeedY != 0)
	//{
	//	D3DXVECTOR3 vTargetPoint(m_EditorInfo.LookAtPos.x, m_EditorInfo.LookAtPos.y, m_EditorInfo.LookAtPos.z);
	//	D3DXVECTOR3 vYAxis(0, 1, 0);  // ��������ϵY��
	//	m_pCamera->RoundEx(&vTargetPoint, &vYAxis, RotateSpeedY);
	//}


	////  ��������¼���ʵ��������ͷ
	//if (!m_bKeyProcessed)
	//{
	//	if (m_Keyboard.key == VK_END)  // ��Զ��ͷ
	//	{
	//		//D3DXVECTOR3 pFrom = m_pCamera->GetPosition(VTS_WORLD);
	//		//D3DXVECTOR3 pAt   = m_pCamera->GetLookat(VTS_WORLD);
	//		D3DXVECTOR3 dir = m_pCamera->GetDirection(VTS_WORLD);
	//		// D3DXVec3Subtract(&dir, &pAt, &pFrom);

	//		FLOAT len = D3DXVec3Length(&dir); 
	//		FLOAT dollyFactor = 0.02f * m_EditorInfo.dollySpeed;
	//		FLOAT dollyValue  = len * dollyFactor;   // ÿ���ƽ��ľ���			
	//		FLOAT dollyFarDis  = m_EditorInfo.dollyFarDis;   // ��Զ����
	//		
	//		if (len < dollyFarDis - dollyValue)
	//		{
	//			m_pCamera->Dolly(dollyValue);
	//		}
	//	}
	//	else if (m_Keyboard.key == VK_HOME) // ������ͷ
	//	{
	//		/*D3DXVECTOR3 pFrom = m_pCamera->GetFrom();
	//		D3DXVECTOR3 pAt   = m_pCamera->GetAt();
	//		D3DXVECTOR3 dir;
	//		D3DXVec3Subtract(&dir, &pAt, &pFrom);*/
	//		D3DXVECTOR3 dir = m_pCamera->GetDirection(VTS_WORLD);

	//		FLOAT len = D3DXVec3Length(&dir); 
	//		FLOAT dollyFactor = 0.02f * m_EditorInfo.dollySpeed;
	//		FLOAT dollyValue  = len * dollyFactor;           // ÿ���ƽ��ľ���						
	//		FLOAT dollyNearDis = m_EditorInfo.dollyNearDis;  // �������
	//		
	//		if (len > dollyNearDis + dollyValue)
	//		{
	//			m_pCamera->Dolly(-dollyValue);
	//		}
	//	}

	//	m_bKeyProcessed = true;

	//} // if m_pKeyboard

}


// �趨editor��Ŀ��㣬�������Ŀ���Ĺ�ϵ
void CEditor::LookAt(const VECTOR3 &v3, INT lookatType/* = 1*/)
{
	if (!m_pCamera)
	{
		return;
	}
	CSceneNode *pCamParent = m_pCamera->m_pNode;     // ����ĸ��ڵ�

	// ��Ŀ���ڵ��ƶ���v3��
	if (pCamParent)
	{
// 		D3DXVECTOR3 vCamPos = pCamParent->worldPosition;                        // ��¼����ĳ�ʼλ��
// 		VECTOR3 v3CamPos(vCamPos.x, vCamPos.y, vCamPos.z);
// 		SetNodePosition(m_pSceneMgr, pCamParent, v3);                           // �ƶ�����ڵ㵽Ŀ��λ��
// 		SetCameraPosition((DWORD)m_pSceneMgr, (DWORD)m_pCamera, &v3CamPos); // �ƶ��������ʼλ��

		// �����ָ��v3
		D3DXVECTOR3 vAt(v3.x, v3.y, v3.z);
		m_pCamera->Lookat(&vAt, VTS_WORLD);
		m_EditorInfo.LookAtPos.x = v3.x;
		m_EditorInfo.LookAtPos.y = v3.y;
		m_EditorInfo.LookAtPos.z = v3.z;
		m_EditorInfo.LookAtModelIndex = 0xffffffff;
		m_EditorInfo.LookAtType = lookatType; // 0���������� 1�������е�һ��  2��ģ��3�������Ŀ���

		// m_pTargetModel = NULL;
	}
}

// lookat ĳ��ģ��
void CEditor::LookAtModel(CModel *pEntity) 
{
	//VECTOR3 vModel;
	//GetModelPosition((DWORD)m_pSceneMgr, (DWORD)pEntity, &vModel);
	//VECTOR3 At(vModel.x, vModel.y, vModel.z);
	//LookAt(At);
	//m_EditorInfo.LookAtModelIndex = (DWORD)pEntity;
	//m_EditorInfo.LookAtType = 2;    // 0���������� 1�������е�һ��  2��ģ��3�������Ŀ���

}

// �õ�lookat��ģ��
CModel* CEditor::GetTargetModel()
{
	if (m_EditorInfo.LookAtType == 2 && m_EditorInfo.LookAtModelIndex != 0xffffffff)
	{
		CModel *pEntity = (CModel*)m_EditorInfo.LookAtModelIndex;
		if (pEntity)
		{
			if (!m_pSceneMgr->GetModelNameByIndex(m_EditorInfo.LookAtModelIndex).empty())
			{
				return pEntity;
			}
		}
	}
	return 0;
}

// ���ò���
void CEditor::SetEditorInfo(const EditorInfo &Info)
{
	m_EditorInfo = Info;

	/*std::string sCamName = std::string(m_EditorInfo.Camera);
	if (m_pSceneMgr)
	{
		CCamera *pCamera = m_pSceneMgr->GetCameraByName(sCamName);
		if (pCamera)
		{
			SetCamera(pCamera);
		}
	}*/
}    

// ��ȡ����
void CEditor::GetEditorInfo(EditorInfo &Info)
{
	Info = m_EditorInfo;
}      

CScene* CEditor::GetSceneManager()
{
	return m_pSceneMgr;
}