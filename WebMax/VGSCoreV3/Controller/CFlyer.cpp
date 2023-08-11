#include "CFlyer.h"

#pragma   warning(disable:4786)  // 暂时屏蔽掉warning

CFlyer::CFlyer(const FlyerInfo &info)
{
	m_TranslateVector.x = 0;
	m_TranslateVector.y = 0;
	m_TranslateVector.z = 0;

	SetFlyerInfo(info);
}
CFlyer::~CFlyer()
{
	
}

// 得到控制器类型
CONTROLTYPE CFlyer::GetControlType()  
{
	return CT_FLYER;
}

string CFlyer::GetName()
{
	return string(m_FlyerInfo.Name);
}
void CFlyer::SetName(const string &sName) // 更新control的状态
{
	ZeroMemory(m_FlyerInfo.Name, sizeof(m_FlyerInfo.Name));
    strcpy_s(m_FlyerInfo.Name, sizeof(m_FlyerInfo.Name), sName.c_str());
}

void CFlyer::SetCamera(CCamera *pCamera)  // 设置虚函数，便于子类中增加其他的处理
{
	if (pCamera != m_pCamera)
	{
		m_pCamera = pCamera;
		
		ZeroMemory(m_FlyerInfo.Camera, sizeof(m_FlyerInfo.Camera) / sizeof(m_FlyerInfo.Camera[0]));

		string str = m_pCamera->getName();
		memcpy(m_FlyerInfo.Camera, str.c_str(), strlen(str.c_str()));
	}
}

// 更新control的状态
void CFlyer::Update()                                             
{

}

// 设置参数
void CFlyer::SetFlyerInfo(const FlyerInfo &Info)
{
	m_FlyerInfo = Info;
}    

// 获取参数
void CFlyer::GetFlyerInfo(FlyerInfo &Info)
{
	Info = m_FlyerInfo;
}      