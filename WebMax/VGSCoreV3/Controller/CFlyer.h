#pragma once

#include "Controller.h"

#include "..\CCamera.h"

// Flyer 控制类
class VGS_EXPORT CFlyer : public IController
{

public : // interface

	virtual CONTROLTYPE		GetControlType();        // 得到控制器类型	
	virtual void			Update();    
	virtual string			GetName();
	virtual void			SetName(const string &sName);  // 更新control的状态
	virtual void            SetCamera(CCamera *pCamera);  // 设置相机

public:

	CFlyer(const FlyerInfo &info);
	~CFlyer();

	void SetFlyerInfo(const FlyerInfo &Info);  // 设置参数
	void GetFlyerInfo(FlyerInfo &Info);        // 获取参数

private:
	FlyerInfo        m_FlyerInfo;		    // FlyerInfo参数信息

	VECTOR3     m_TranslateVector;
	float       m_MoveScale;
	float       m_RotScale;
};