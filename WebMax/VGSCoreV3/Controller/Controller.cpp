#include "controller.h"

// 从windows消息参数中获取鼠标按键类型
MOUSE_TYPE getMouseType(UINT wParam)
{
	MOUSE_TYPE type = MOUSE_TYPE_NONE;
	if (wParam & MK_LBUTTON)
	{
		type = MOUSE_TYPE_LEFT;
	}
	else if (wParam & MK_RBUTTON)
	{
		type = MOUSE_TYPE_RIGHT;
	}
	else if (wParam & MK_MBUTTON)
	{
		type = MOUSE_TYPE_MIDDLE;
	}
	return type;
}

// 从windows消息参数中获取键盘辅助键类型
EXTENTEDKEY_TYPE getExtenedKeyType(UINT wParam)
{
	EXTENTEDKEY_TYPE type = EKT_NONE;
	if (wParam & MK_CONTROL)
	{
		type = EKT_CTRL;
	}
	else if (wParam & MK_SHIFT)
	{
		type = EKT_SHIFT;
	}
	else if (wParam & MK_ALT)
	{
		type = EKT_ALT;
	}
	return type;
}

// 构造函数
IController::IController()
{	
	m_pCamera = NULL;

	m_disX = m_disY = 0;

	ZeroMemory(&m_lastMouseInfo, sizeof(m_lastMouseInfo));
	ZeroMemory(&m_lastKeyState, sizeof(m_lastKeyState));

	m_FrameEvent.timeSinceLastEvent = 0;
	m_FrameEvent.timeSinceLastFrame = 0;
}

// 设置鼠标状态
void IController::SetMouseState(const VGSMouseState &ms)
{
	m_lastMouseInfo = m_MouseInfo;
	m_MouseInfo = ms;

	m_bMouseProcessed = false;
}
// 设置键盘状态
void IController::SetKeyboardState(const VGSKeyState &ks)
{
	m_lastKeyState = m_Keyboard;
	m_Keyboard = ks;
	
	m_bKeyProcessed = false;
}