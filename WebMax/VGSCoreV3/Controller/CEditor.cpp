/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // CEditor.cpp
* 内容摘要： // 物件展示过程实现
* 其它说明： // 
* 当前版本： // V2.0
* 作    者： // 傅新波
* 完成日期： // 
* 
* 修改日期      版本号     修改人	      修改内容
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
	//	// 删除节点
	//	m_pSceneMgr->mSceneMgr->destroySceneNode(m_TargetNode->getName());
	//}

	m_pCamera = NULL;
	m_pSceneMgr = NULL;
}

// 得到控制器类型
CONTROLTYPE CEditor::GetControlType()  
{
	return CT_EDITOR;
}

string CEditor::GetName()
{
	return string(m_EditorInfo.Name);
}
void CEditor::SetName(const string &sName) // 更新control的状态
{
	ZeroMemory(m_EditorInfo.Name, sizeof(m_EditorInfo.Name));
	strcpy_s(m_EditorInfo.Name, sizeof(m_EditorInfo.Name), sName.c_str());
}

void CEditor::SetCamera(CCamera *pCamera)  // 设置虚函数，便于子类中增加其他的处理
{
	if (!pCamera)
	{
		return;
	}
	if (pCamera != m_pCamera)
	{
		m_pCamera = pCamera;
		
		// 更新相机名称
		ZeroMemory(m_EditorInfo.Camera, sizeof(m_EditorInfo.Camera) / sizeof(m_EditorInfo.Camera[0]));

		string str = m_pCamera->getName();
		memcpy(m_EditorInfo.Camera, str.c_str(), strlen(str.c_str()));

	}
	// 自动对准目标点
	// if (pCamera->m_pNode)
	{
		D3DXVECTOR3 vAt(m_EditorInfo.LookAtPos.x, m_EditorInfo.LookAtPos.y, m_EditorInfo.LookAtPos.z);
		pCamera->Lookat(&vAt, VTS_WORLD);
	}
}

// 更新control的状态
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

	//float RotateSpeedX = 0;    // 绕 X 轴旋转的角度
	//float RotateSpeedY = 0;    // 绕 Y 轴旋转的角度

	//float timespan = (float)m_FrameEvent.timeSinceLastFrame; // 与上一帧的时间间隔, second
	//
	//// 如果是自动旋转模式
	//if (m_EditorInfo.bIsAutoRoundRoll)
	//{
	//	RotateSpeedX = m_EditorInfo.autoSpeedX * timespan * PI / 30.0f;
	//	RotateSpeedY = m_EditorInfo.autoSpeedY * timespan * PI / 30.0f;
	//}
	//else  // 非自动旋转模式
	//{
	//	if (!m_bMouseProcessed)
	//	{
	//		m_disX = m_MouseInfo.x - m_lastMouseInfo.x;   // 屏幕上 x 方向相对位移, 控制左右转向
	//		m_disY = m_MouseInfo.y - m_lastMouseInfo.y;       // 屏幕上 y 方向相对位移，控制抬头或低头
	//		if (m_MouseInfo.mouseType == MOUSE_TYPE_MIDDLE && m_MouseInfo.hRenderWnd)  // 中键平移相机
	//		{
	//			//求屏幕点在设定平面上的对应点
	//			D3DXVECTOR3 vCamDir = m_pCamera->GetDirection(VTS_WORLD);
	//			VECTOR3 vo, vx, vy;

	//			// 一个点和法线确定一个平面
	//			VECTOR3 planPoint(m_EditorInfo.LookAtPos.x, m_EditorInfo.LookAtPos.y, m_EditorInfo.LookAtPos.z);
	//			VECTOR3 planNormal(-vCamDir.x, -vCamDir.y, -vCamDir.z);

	//			// 计算屏幕上的点映射到三维空间指定平面上的点
	//			CViewport *pViewport = ((CRenderWindow*)m_MouseInfo.hRenderWnd)->GetViewport(m_pCamera);
	//			if (pViewport)
	//			{
	//				m_pSceneMgr->GetCoordScreenToWorldFromPlane(pViewport, m_MouseInfo.x, m_MouseInfo.y, planPoint, planNormal, vo);
	//				m_pSceneMgr->GetCoordScreenToWorldFromPlane(pViewport, m_MouseInfo.x + 1, m_MouseInfo.y, planPoint, planNormal, vx);
	//				m_pSceneMgr->GetCoordScreenToWorldFromPlane(pViewport, m_MouseInfo.x, m_MouseInfo.y + 1, planPoint, planNormal, vy);

	//				// 将屏幕上x，y方向的位移换算成世界坐标系中的移动距离
	//				vx.x = -m_disX * (vx.x - vo.x);
	//				vx.y = -m_disX * (vx.y - vo.y);
	//				vx.z = -m_disX * (vx.z - vo.z);
	//				vy.x = -m_disY * (vy.x - vo.x);
	//				vy.y = -m_disY * (vy.y - vo.y);
	//				vy.z = -m_disY * (vy.z - vo.z);
	//				vx.x += vy.x;
	//				vx.y += vy.y;
	//				vx.z += vy.z;

	//				MoveCamera((DWORD)m_pSceneMgr, (DWORD)m_pCamera, &vx, 0);   // 移动一段相对位移
	//			}
	//		}
	//		else if (m_MouseInfo.mouseType == MOUSE_TYPE_LEFT)  // 左键拖拽实现旋转
	//		{
	//			if (m_EditorInfo.rosAxis == 0 || m_EditorInfo.rosAxis == 2) // 可绕xy轴或y轴旋转
	//			{
	//				if (m_disX != 0)
	//				{
	//					// RotateSpeedY = -(FLOAT)m_disX / 12.0f;   // 用 x 相对位移计算绕 Y 轴旋转的角度，常数为经验值
	//					RotateSpeedY = -(FLOAT)m_disX * m_EditorInfo.manualSpeedY / 200.0f;   // 用 x 相对位移计算绕 Y 轴旋转的角度，常数为经验值
	//					m_disX = (INT)(m_disX * 0.95f); // 如果没有按下左键拖拽，减速
	//				}
	//			}
	//			
	//			if (m_EditorInfo.rosAxis == 0 || m_EditorInfo.rosAxis == 1) // 可绕xy轴或x轴旋转
	//			{
	//				if (m_disY != 0)
	//				{
	//					// RotateSpeedX = -(FLOAT)m_disY / 8.0f;   // 用 y 相对位移计算绕 X 轴旋转的角度，常数为经验值
	//					RotateSpeedX = -(FLOAT)m_disY * m_EditorInfo.manualSpeedX / 200.0f;   // 用 y 相对位移计算绕 X 轴旋转的角度，常数为经验值
	//					m_disY = (INT)(m_disY * 0.95f);       // 如果没有按下左键拖拽，减速
	//				}
	//			}
	//		}

	//		m_bMouseProcessed = true;

	//	} // 处理鼠标事件

	//	// 处理键盘事件，实现旋转
	//	if (!m_bKeyProcessed)  
	//	{	
	//		if (m_Keyboard.key == VK_LEFT)
	//		{
	//			RotateSpeedY = m_EditorInfo.manualSpeedY / 50.0f/* * timespan*/;  // x相对位移, 绕y轴转
	//		}
	//		if (m_Keyboard.key == VK_RIGHT)
	//		{	
	//			RotateSpeedY = -m_EditorInfo.manualSpeedY / 50.0f/* * timespan*/;	 // x相对位移, 绕y轴转
	//		}
	//		if (m_Keyboard.key == VK_UP)
	//		{
	//			RotateSpeedX = m_EditorInfo.manualSpeedX / 50.0f/* * timespan*/;  // y相对位移，绕x轴转
	//		}			
	//		if (m_Keyboard.key == VK_DOWN)
	//		{
	//			RotateSpeedX = -m_EditorInfo.manualSpeedX / 50.0f/* * timespan*/;  // y相对位移，绕x轴转
	//		}

	//		// m_bKeyProcessed = true;

	//	} // 处理键盘事件

	//} // 非自动旋转模式

	//// 绕 相机的right轴 旋转
	//if (RotateSpeedX != 0)
	//{
	//	D3DXVECTOR3 vTargetPoint(m_EditorInfo.LookAtPos.x, m_EditorInfo.LookAtPos.y, m_EditorInfo.LookAtPos.z);
	//	D3DXVECTOR3 vXAxis = m_pCamera->GetRight(VTS_WORLD);
	//	m_pCamera->RoundEx(&vTargetPoint, &vXAxis, RotateSpeedX);
	//}
	//// 绕 世界坐标系的y轴 旋转
	//if (RotateSpeedY != 0)
	//{
	//	D3DXVECTOR3 vTargetPoint(m_EditorInfo.LookAtPos.x, m_EditorInfo.LookAtPos.y, m_EditorInfo.LookAtPos.z);
	//	D3DXVECTOR3 vYAxis(0, 1, 0);  // 世界坐标系Y轴
	//	m_pCamera->RoundEx(&vTargetPoint, &vYAxis, RotateSpeedY);
	//}


	////  处理键盘事件，实现推拉镜头
	//if (!m_bKeyProcessed)
	//{
	//	if (m_Keyboard.key == VK_END)  // 拉远镜头
	//	{
	//		//D3DXVECTOR3 pFrom = m_pCamera->GetPosition(VTS_WORLD);
	//		//D3DXVECTOR3 pAt   = m_pCamera->GetLookat(VTS_WORLD);
	//		D3DXVECTOR3 dir = m_pCamera->GetDirection(VTS_WORLD);
	//		// D3DXVec3Subtract(&dir, &pAt, &pFrom);

	//		FLOAT len = D3DXVec3Length(&dir); 
	//		FLOAT dollyFactor = 0.02f * m_EditorInfo.dollySpeed;
	//		FLOAT dollyValue  = len * dollyFactor;   // 每次推进的距离			
	//		FLOAT dollyFarDis  = m_EditorInfo.dollyFarDis;   // 最远距离
	//		
	//		if (len < dollyFarDis - dollyValue)
	//		{
	//			m_pCamera->Dolly(dollyValue);
	//		}
	//	}
	//	else if (m_Keyboard.key == VK_HOME) // 拉近镜头
	//	{
	//		/*D3DXVECTOR3 pFrom = m_pCamera->GetFrom();
	//		D3DXVECTOR3 pAt   = m_pCamera->GetAt();
	//		D3DXVECTOR3 dir;
	//		D3DXVec3Subtract(&dir, &pAt, &pFrom);*/
	//		D3DXVECTOR3 dir = m_pCamera->GetDirection(VTS_WORLD);

	//		FLOAT len = D3DXVec3Length(&dir); 
	//		FLOAT dollyFactor = 0.02f * m_EditorInfo.dollySpeed;
	//		FLOAT dollyValue  = len * dollyFactor;           // 每次推进的距离						
	//		FLOAT dollyNearDis = m_EditorInfo.dollyNearDis;  // 最近距离
	//		
	//		if (len > dollyNearDis + dollyValue)
	//		{
	//			m_pCamera->Dolly(-dollyValue);
	//		}
	//	}

	//	m_bKeyProcessed = true;

	//} // if m_pKeyboard

}


// 设定editor的目标点，绑定相机与目标点的关系
void CEditor::LookAt(const VECTOR3 &v3, INT lookatType/* = 1*/)
{
	if (!m_pCamera)
	{
		return;
	}
	CSceneNode *pCamParent = m_pCamera->m_pNode;     // 相机的父节点

	// 将目标点节点移动到v3处
	if (pCamParent)
	{
// 		D3DXVECTOR3 vCamPos = pCamParent->worldPosition;                        // 记录相机的初始位置
// 		VECTOR3 v3CamPos(vCamPos.x, vCamPos.y, vCamPos.z);
// 		SetNodePosition(m_pSceneMgr, pCamParent, v3);                           // 移动相机节点到目标位置
// 		SetCameraPosition((DWORD)m_pSceneMgr, (DWORD)m_pCamera, &v3CamPos); // 移动相机到初始位置

		// 把相机指向v3
		D3DXVECTOR3 vAt(v3.x, v3.y, v3.z);
		m_pCamera->Lookat(&vAt, VTS_WORLD);
		m_EditorInfo.LookAtPos.x = v3.x;
		m_EditorInfo.LookAtPos.y = v3.y;
		m_EditorInfo.LookAtPos.z = v3.z;
		m_EditorInfo.LookAtModelIndex = 0xffffffff;
		m_EditorInfo.LookAtType = lookatType; // 0：场景中心 1：场景中的一点  2：模型3：相机的目标点

		// m_pTargetModel = NULL;
	}
}

// lookat 某个模型
void CEditor::LookAtModel(CModel *pEntity) 
{
	//VECTOR3 vModel;
	//GetModelPosition((DWORD)m_pSceneMgr, (DWORD)pEntity, &vModel);
	//VECTOR3 At(vModel.x, vModel.y, vModel.z);
	//LookAt(At);
	//m_EditorInfo.LookAtModelIndex = (DWORD)pEntity;
	//m_EditorInfo.LookAtType = 2;    // 0：场景中心 1：场景中的一点  2：模型3：相机的目标点

}

// 得到lookat的模型
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

// 设置参数
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

// 获取参数
void CEditor::GetEditorInfo(EditorInfo &Info)
{
	Info = m_EditorInfo;
}      

CScene* CEditor::GetSceneManager()
{
	return m_pSceneMgr;
}