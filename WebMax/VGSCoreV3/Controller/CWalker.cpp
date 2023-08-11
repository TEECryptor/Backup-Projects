/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // CWalker.cpp
* 内容摘要： // 漫游模式实现
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

// 得到控制器类型
CONTROLTYPE CWalker::GetControlType()  
{
	return CT_WALKER;
}

string CWalker::GetName()
{
	return string(m_WalkerInfo.Name);
}
void CWalker::SetName(const string &sName) // 更新control的状态
{
	ZeroMemory(m_WalkerInfo.Name, sizeof(m_WalkerInfo.Name));
	strcpy_s(m_WalkerInfo.Name, sizeof(m_WalkerInfo.Name), sName.c_str());
}

void CWalker::SetCamera(CCamera *pCamera)  // 设置虚函数，便于子类中增加其他的处理
{
	if (pCamera != m_pCamera)
	{
		m_pCamera = pCamera;
		
		ZeroMemory(m_WalkerInfo.Camera, sizeof(m_WalkerInfo.Camera) / sizeof(m_WalkerInfo.Camera[0]));

		string str = m_pCamera->getName();
		memcpy(m_WalkerInfo.Camera, str.c_str(), strlen(str.c_str()));
	}
}

// 更新control的状态
void CWalker::Update()                                             
{
	float realYawSpeed  = 0; // 抬头、低头的角度
	float realPitchSpeed = 0; // 左右转向的角度
	float step           = 0; // 向前或向后行走的距离
	float liftSpeed      = 0;  // 上升下降的速度

	float timespan = (float)m_FrameEvent.timeSinceLastFrame; // 与上一帧的时间间隔, second
	
	if (!m_bMouseProcessed)
	{
		if (m_MouseInfo.mouseType == MOUSE_TYPE_LEFT)  // 左键拖拽实现旋转
		{
			m_disX = m_MouseInfo.x - m_lastMouseInfo.x;   // 屏幕上 x 方向相对位移, 控制左右转向
			m_disY = m_MouseInfo.y - m_lastMouseInfo.y;       // 屏幕上 y 方向相对位移，控制抬头或低头

			if (m_disX != 0)
			{
				realYawSpeed = -(float)m_disX * m_WalkerInfo.rosSpeed / 800.0f;   // 用 x 相对位移计算角度，常数为经验值
			}
			if (m_disY != 0)
			{
				realPitchSpeed = -(float)m_disY * m_WalkerInfo.rosSpeed / 800.0f;   // 用 y 相对位移计算角度，常数为经验值
			}	
		}
		m_bMouseProcessed = true;
	}


	if (!m_bKeyProcessed)  // 处理键盘事件，实现旋转
	{

		if (m_Keyboard.key == VK_LEFT)
		{
			realYawSpeed = m_WalkerInfo.rosSpeed * timespan;  // 左转
		}
		if (m_Keyboard.key == VK_RIGHT)
		{	
			realYawSpeed = -m_WalkerInfo.rosSpeed * timespan;	 // 右转
		}
		if (m_Keyboard.key == VK_UP)
		{
			step = -m_WalkerInfo.walkSpeed * timespan;  // 前行
		}			
		if (m_Keyboard.key == VK_DOWN)
		{
			step = m_WalkerInfo.walkSpeed * timespan;  // 后退
		}

		if (!m_WalkerInfo.bWeight)  // 如果失去重力
		{
			if (m_Keyboard.key == VK_NEXT)   // 下降
			{
				liftSpeed = -m_WalkerInfo.liftSpeed * timespan;
			}
			if (m_Keyboard.key == VK_PRIOR)  // 上升
			{
				liftSpeed = m_WalkerInfo.liftSpeed * timespan;
			}			
		}
		else // 如果受重力影响
		{
			if (m_Keyboard.key == VK_NEXT)   // 低头
			{
				realPitchSpeed = -m_WalkerInfo.rosSpeed * timespan / 5.0f;
			}
			if (m_Keyboard.key == VK_PRIOR)  // 抬头
			{
				realPitchSpeed = m_WalkerInfo.rosSpeed * timespan / 5.0f;
			}
		}
		m_bKeyProcessed = true;
	} // if 

	// 实现相机的旋转
	if (realPitchSpeed != 0)  // y相对位移
	{
		if (realPitchSpeed > 0.0001 || realPitchSpeed < -0.0001) // != 0
		{
			D3DXVECTOR3 vRot(realPitchSpeed, 0, 0);
			m_pCamera->Rotate(&vRot, VRS_WORLD2);		
		}
	}

	// 绕 y 轴旋转
	if (realYawSpeed != 0)
	{		
		D3DXVECTOR3 vRot(0, realYawSpeed, 0);
		m_pCamera->Rotate(&vRot, VRS_WORLD2);  // 绕穿过from点平行于世界坐标系的坐标旋转
	}

	// 前进或后退
	if (step != 0)
	{
		D3DXVECTOR3 vRot(0, 0, step);
		m_pCamera->Move(vRot, VTS_WORLD);
	}

	// 升降
	if (liftSpeed != 0)
	{
		D3DXVECTOR3 vRot(0, liftSpeed, 0);
		m_pCamera->Move(vRot, VTS_WORLD);
		// m_pCamera->moveRelative(Vector3(0, liftSpeed, 0));
	}
	
}

// 设置参数
void CWalker::SetWalkerInfo(const WalkerInfo &Info)
{
	m_WalkerInfo = Info;

	m_MaxUpperAngle = Info.upperAngle *  0.0174533f;  // 角度转换为弧度
	m_MaxLowerAngle = -Info.lowerAngle *  0.0174533f;  // 角度转换为弧度 
}    

// 获取参数
void CWalker::GetWalkerInfo(WalkerInfo &Info)
{
	Info = m_WalkerInfo;
}      