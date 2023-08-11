#ifndef __VGS_SCRIPT_H__
#define __VGS_SCRIPT_H__

//!
//!文件 VgsScript.hpp
//!描述 这里提供C标准的API，用于操作VGS的脚本.
//!		脚本引擎由VGS引擎创建和管理，外部程序只是使用.
//!特别说明 	功能键状态说明:使用一个字节:00000000,位1表示存在该键,D0(最低位)-Ctrl, D1-Shift, D2-Alt,D3~D7保留
//!作者 linxq
//!创建 2008-12-08
//!更新 2010-6-10
//!

#if defined(_WINDLL)
#define LUA_C_EXPORT __declspec(dllexport)
#else
#define LUA_C_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

//!
//!描述 获取VGS脚本引擎的版本描述.
//!
LUA_C_EXPORT const char *  __stdcall GetVgsScripterVersion();

//!
//!描述 获取最近一次操作失败的信息.
//!返回 错误信息.
//!
LUA_C_EXPORT const char * __stdcall GetVgsScriptLastError();
//!
//!描述 执行一个Lua脚本文件.
//!参数 szScriptFile 脚本文件的名称
//!返回 true表示成功,false表示失败可通过GetVgsScriptLastError()获取错误信息.
//!
LUA_C_EXPORT bool	__stdcall ExecuteLuaScript(const char * szScriptFile);
//!
//!描述 执行一个Lua脚本语句.
//!参数 szFuntionName 脚本函数的名称
//!返回 true表示成功,false表示失败可通过GetVgsScriptLastError()获取错误信息.
//!
LUA_C_EXPORT bool	__stdcall ExecuteLuaCmd(const char * szLuaCmd);

//!
//!描述 引擎初始化的事件.
//!
LUA_C_EXPORT void __stdcall OnInitVgs();
//!
//!描述 引擎退出的事件.
//!
LUA_C_EXPORT void __stdcall OnExitEvent();
//!
//!描述 窗体Resize的事件.
//!
LUA_C_EXPORT void __stdcall OnResizeEvent();
//!
//!描述 程序激活的事件.
//!
LUA_C_EXPORT void __stdcall OnActiveEvent();
//!
//!描述 每渲染一帧前的事件.
//!
LUA_C_EXPORT void __stdcall OnStartOneFrameEvent();
//!
//!描述 每渲染一帧后的事件.
//!
LUA_C_EXPORT void __stdcall OnEndOneFrameEvent();
//!
//!描述 按键事件.
//!参数 nKey 按键值
//!参数 FuncKeys 功能键状态
//!
LUA_C_EXPORT void __stdcall OnKeyPressEvent(int nKey, unsigned char FuncKeys);
//!
//!描述 按键按下事件.
//!参数 nKey 按键值
//!参数 FuncKeys 功能键状态
//!
LUA_C_EXPORT void __stdcall OnKeyDownEvent(int nKey, unsigned char FuncKeys);
//!
//!描述 按键弹起事件.
//!参数 nKey 按键值
//!参数 FuncKeys 功能键状态
//!
LUA_C_EXPORT void __stdcall OnKeyUpEvent(int nKey, unsigned char FuncKeys);
//!
//!描述 鼠标进入事件.
//!参数 szLuaFuction Lua脚本事件函数
//!参数 FuncKeys 功能键状态
//!参数 x 屏幕坐标
//!参数 y 屏幕坐标
//!		
LUA_C_EXPORT void __stdcall OnMouseEnterEvent(const char * szLuaFuction, unsigned char FuncKeys, int x, int y);
//!
//!描述 鼠标离开事件.
//!参数 szLuaFuction Lua脚本事件函数
//!参数 FuncKeys 功能键状态
//!参数 x 屏幕坐标
//!参数 y 屏幕坐标
//!		
LUA_C_EXPORT void __stdcall OnMouseLeaveEvent(const char * szLuaFuction, unsigned char FuncKeys, int x, int y);
//!
//!描述 鼠标按下事件.
//!参数 szLuaFuction Lua脚本事件函数
//!参数 nMouseID 鼠标ID:0-左，1-右，2-中，其他值保留
//!参数 FuncKeys 功能键状态
//!参数 x 屏幕坐标
//!参数 y 屏幕坐标
//!		
LUA_C_EXPORT void __stdcall OnMouseDownEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y);
//!
//!描述 鼠标弹起事件.
//!参数 szLuaFuction Lua脚本事件函数
//!参数 nMouseID 鼠标ID:0-左，1-右，2-中，其他值保留
//!参数 FuncKeys 功能键状态
//!参数 x 屏幕坐标
//!参数 y 屏幕坐标
//!		
LUA_C_EXPORT void __stdcall OnMouseUpEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y);
//!
//!描述 鼠标移动事件.
//!参数 szLuaFuction Lua脚本事件函数
//!参数 nMouseID 鼠标ID:0-左，1-右，2-中，其他值保留
//!参数 FuncKeys 功能键状态
//!参数 x 屏幕坐标
//!参数 y 屏幕坐标
//!		
LUA_C_EXPORT void __stdcall OnMouseMoveEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y);
//!
//!描述 鼠标双击事件.
//!参数 szLuaFuction Lua脚本事件函数
//!参数 nMouseID 鼠标ID:0-左，1-右，2-中，其他值保留
//!参数 FuncKeys 功能键状态
//!参数 x 屏幕坐标
//!参数 y 屏幕坐标
//!		
LUA_C_EXPORT void __stdcall OnMouseDoubleClickEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y);

//!描述	测试使用——
//执行一条调试指令
LUA_C_EXPORT void __stdcall SetDebugCmd(const char * szDebugCmd);
//把宿主的窗口句柄设置给Lua使用
LUA_C_EXPORT void __stdcall SetHostHandleToLua(void* hWnd);
//获取最近一次的参数/值对
LUA_C_EXPORT const char * __stdcall GetLastDebugPrmVal(char * szParam);

#ifdef __cplusplus
}
#endif

#endif