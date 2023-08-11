#include "MovableObject.h"

CMovableObject::CMovableObject()
{
	m_sMouseOverFunc = "";
	m_sMouseDBClickFunc = "";
	//m_sMouseLClickFunc = "";
	m_sMouseLDownFunc = "";
	m_sMouseLUpFunc = "";
	m_sMouseRDownFunc = "";
	m_sMouseRUpFunc = "";

	m_type = VGS_MOVABLETYPE_NONE;
	setName("MovableObject");

	m_pNode = NULL;
	// m_bParentTransformChanged = true;
}

CMovableObject::~CMovableObject()
{
	Release();
}

VOID CMovableObject::Release()
{
	m_sMouseOverFunc = "";
	m_sMouseDBClickFunc = "";
	//m_sMouseLClickFunc = "";
	m_sMouseLDownFunc = "";
	m_sMouseLUpFunc = "";
	m_sMouseRDownFunc = "";
	m_sMouseRUpFunc = "";

	m_type = VGS_MOVABLETYPE_NONE;
	setName("MovableObject");
}

// 设置事件函数名称
VOID CMovableObject::setEventFuncName(VGS_MOUSEEVENT_TYPE type, const std::string &sFuncName)
{
	switch (type)
	{
	case VGS_MOUSEEVENT_TYPE_OVER:
		setMouseOverFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_DBCLICK:
		setMouseDBClickFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_LEFTDOWN:
		setMouseLDwonFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_LEFTUP:
		setMouseLUpFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_RIGHTDOWN:
		setMouseRDownFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_RIGHTUP:
		setMouseRUpFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_MOVE:
		setMouseMoveFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_LEAVE:
		setMouseLeaveFuncName(sFuncName);
		break;
	default:
		break;
	}
}

// 获取事件函数名称
std::string CMovableObject::getEventFuncName(VGS_MOUSEEVENT_TYPE type)
{
	std::string str = "";
	switch (type)
	{
	case VGS_MOUSEEVENT_TYPE_OVER:
		str = getMouseOverFuncName();
		break;
	case VGS_MOUSEEVENT_TYPE_DBCLICK:
		str = getMouseDBClickFuncName();
		break;
	case VGS_MOUSEEVENT_TYPE_LEFTDOWN:
		str = getMouseLDwonFuncName();
		break;
	case VGS_MOUSEEVENT_TYPE_LEFTUP:
		str = getMouseLUpFuncName();
		break;
	case VGS_MOUSEEVENT_TYPE_RIGHTDOWN:
		str = getMouseRDownFuncName();
		break;
	case VGS_MOUSEEVENT_TYPE_RIGHTUP:
		str = getMouseRUpFuncName();
		break;
	case VGS_MOUSEEVENT_TYPE_MOVE:        // 移动
		str = getMouseMoveFuncName();
		break;
	case VGS_MOUSEEVENT_TYPE_LEAVE:        // 离开按钮 
		str = getMouseLeaveFuncName();
		break;
	default:
		break;
	}
	return str;
}