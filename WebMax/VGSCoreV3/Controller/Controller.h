#pragma once

#include "..\VGSHeader.h"
//#include "..\VGSDef.h"
#include "..\Event\VGSFrameListener.h"
//#include "..\CCamera.h"

using namespace std;

//// 控制对象的类型
//enum CONTROLTYPE
//{
//	CT_EDITOR = 0,
//	CT_WALKER,
//	CT_FLYER
//};
// 从顶视图看，旋转方向
enum AROUNDWISE
{
	AW_CLOCKWISE = 0,
	AW_ANTICLOCKWISE
};
enum MOUSE_TYPE
{ 
	MOUSE_TYPE_NONE = 0,
	MOUSE_TYPE_LEFT,
	MOUSE_TYPE_MIDDLE,
	MOUSE_TYPE_RIGHT,
	MOUSE_TYPE_DBLEFT
};
enum EXTENTEDKEY_TYPE
{
	EKT_NONE = 0,
	EKT_ALT,
	EKT_CTRL,
	EKT_SHIFT
};
// 鼠标状态
struct VGSMouseState
{
	INT x;  // 相对于渲染窗口的平面坐标
	INT y;

	EXTENTEDKEY_TYPE extentedKey;  // 辅助键盘键
	MOUSE_TYPE       mouseType;    // 鼠标按键

	DWORD hRenderWnd;  // 渲染窗口
};

// 从windows消息参数中获取鼠标按键类型
extern MOUSE_TYPE getMouseType(UINT wParam);

// 从windows消息参数中获取键盘辅助键类型
extern EXTENTEDKEY_TYPE getExtenedKeyType(UINT wParam);

// 键盘按键
struct VGSKeyState
{
	WORD key;
	EXTENTEDKEY_TYPE extentedKey;  // 辅助键盘键

	DWORD hRenderWnd;  // 渲染窗口
};

// 控制对象walker、editor、flyer的基类
class VGS_EXPORT IController
{

public:

	virtual CONTROLTYPE		GetControlType() = 0;  // 得到控制器类型	
	virtual void			Update() = 0;          // 更新control的状态
	virtual string			GetName() = 0;
	virtual void			SetName(const string &sName) = 0;
	
	virtual void SetMouseState(const VGSMouseState &ms);
// 	virtual void SetMouseState(const VGSMouseState &ms)                  // 设置鼠标状态
// 	{
// 		m_MouseInfo = ms;
// 	}
	virtual void SetKeyboardState(const VGSKeyState &ks);
// 	virtual void SetKeyboardState(const VGSKeyState &ks)     // 设置键盘状态
// 	{
// 		m_Keyboard = ks;
// 	}
	virtual void SetFrameState(const FrameEvent &FrameEvent)         // 设置帧信息
	{
		m_FrameEvent = FrameEvent;
	}
	void SetCamera(CCamera *pCamera)  // 设置虚函数，便于子类中增加其他的处理
	{
		m_pCamera = pCamera;
	}
	virtual CCamera* GetCamera()
	{
		return m_pCamera;
	}

public:

	IController();

	virtual ~IController()
	{
		Release();
	}

	virtual VOID Release(){m_pCamera = NULL;}
protected:
	
	VGSMouseState       m_MouseInfo;		 // 当前的鼠标信息
	VGSKeyState  	    m_Keyboard;          // 当前的按键状态
	FrameEvent          m_FrameEvent;        // 帧信息
	CCamera              *m_pCamera;         // 相机指针

	INT m_disX;  // 鼠标在屏幕的移动位移，X方向
	INT m_disY;  // 鼠标在屏幕的移动位移，Y方向

	VGSMouseState       m_lastMouseInfo;   // 前一次的鼠标状态
	VGSKeyState         m_lastKeyState;    // 前一次的按键状态

	bool                m_bKeyProcessed;   // 是否已经处理过
	bool                m_bMouseProcessed;

};