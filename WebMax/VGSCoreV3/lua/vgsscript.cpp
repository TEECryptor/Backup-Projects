#include "vgsscript.h"
#include "../VGSCore.h"

extern void* g_hHostAppHandle;//宿主程序的窗口句柄，如果有.
extern CVGSCore	globalVgsCore;

const char *  __stdcall GetVgsScripterVersion()
{
	return "VGS Scripter 0.0.1";
}

const char * __stdcall GetVgsScriptLastError()
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		return globalVgsCore.getLuaEnginePtr()->GetLastErrorInfo();
	else
		return "";
}
bool	__stdcall ExecuteLuaScript(const char * szScriptFile)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		return globalVgsCore.getLuaEnginePtr()->DoFile(szScriptFile);
	else
		return false;
}
bool	__stdcall ExecuteLuaCmd(const char * szLuaCmd)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		return globalVgsCore.getLuaEnginePtr()->DoString(szLuaCmd);
	else
		return false;
}
void __stdcall OnInitVgs()
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnInitVgs();
}
void __stdcall OnExitEvent()
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnExit();
}
void __stdcall OnResizeEvent()
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnResize();
}
void __stdcall OnActiveEvent()
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnActive();
}
void __stdcall OnStartOneFrameEvent()
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnStartOneFrame();
}
void __stdcall OnEndOneFrameEvent()
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnEndOneFrame();
}
void __stdcall OnKeyPressEvent(int nKey, unsigned char FuncKeys)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnKeyPressEvent(nKey, FuncKeys);
}
void __stdcall OnKeyDownEvent(int nKey, unsigned char FuncKeys)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnKeyDownEvent(nKey, FuncKeys);
}
void __stdcall OnKeyUpEvent(int nKey, unsigned char FuncKeys)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnKeyUpEvent(nKey, FuncKeys);
}
void __stdcall OnMouseEnterEvent(const char * szLuaFuction, unsigned char FuncKeys, int x, int y)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnMouseEnter(szLuaFuction, FuncKeys, x, y);
}
void __stdcall OnMouseLeaveEvent(const char * szLuaFuction, unsigned char FuncKeys, int x, int y)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnMouseLeave(szLuaFuction, FuncKeys, x, y);
}
void __stdcall OnMouseDownEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnMouseDown(szLuaFuction, nMouseID, FuncKeys, x, y);
}
void __stdcall OnMouseUpEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnMouseUp(szLuaFuction, nMouseID, FuncKeys, x, y);
}
void __stdcall OnMouseMoveEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnMouseMove(szLuaFuction, nMouseID, FuncKeys, x, y);
}
void __stdcall OnMouseDoubleClickEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->OnMouseDoubleClick(szLuaFuction, nMouseID, FuncKeys, x, y);
}

void __stdcall SetDebugCmd(const char * szDebugCmd)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		globalVgsCore.getLuaEnginePtr()->SetDebugCommand(szDebugCmd);
}
void __stdcall SetHostHandleToLua(void* hWnd)
{
	g_hHostAppHandle = hWnd;
}
const char * __stdcall GetLastDebugPrmVal(char * szParam)
{
	if (globalVgsCore.getLuaEnginePtr() !=0 )
		return globalVgsCore.getLuaEnginePtr()->GetLastDebugPrmVal(szParam);
	else
		return 0;
}
