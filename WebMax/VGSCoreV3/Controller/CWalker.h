/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // CWalker.h
* 内容摘要： // 漫游模式类定义
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

#pragma once


#include "..\VGSHeader.h"
#include "Controller.h"
#include "..\CCamera.h"

// Walker 控制类
class VGS_EXPORT CWalker : public IController
{

public : // interface

	virtual CONTROLTYPE		GetControlType();  // 得到控制器类型	
	virtual void			Update();    
	virtual string			GetName();
	virtual void			SetName(const string &sName);// 更新control的状态

	virtual void SetCamera(CCamera *pCamera);

	// virtual void SetMouseState(const MouseState &ms);                  // 设置鼠标状态
	// virtual void SetKeyboardState(const OIS::Keyboard *pKeyboard);     // 设置键盘状态
	// virtual void SetFrameState(const FrameEvent &FrameEvent)           // 设置帧信息

public:

	CWalker(const WalkerInfo &Info);
	~CWalker();

	void SetWalkerInfo(const WalkerInfo &Info);  // 设置参数
	void GetWalkerInfo(WalkerInfo &Info);        // 获取参数

private:
	WalkerInfo        m_WalkerInfo;		    // Editor参数信息

	VECTOR3     m_TranslateVector;
	float       m_MoveScale;
	float       m_RotScale;

	float       m_MaxUpperAngle; // 弧度表示的最大仰角
	float       m_MaxLowerAngle; // 弧度表示的最大俯角
};