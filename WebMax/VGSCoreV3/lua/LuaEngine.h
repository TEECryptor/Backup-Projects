#ifndef VGS_LUA_ENGINE_H
#define VGS_LUA_ENGINE_H
#include <string>

//!
//!文件 VgsLua.h
//!描述 使用Luaplus对VGS中要使用的脚本进行封装，包括注册VGS的API给Lua，也包括实现对Lua脚本的调用.
//!作者 linxq
//!创建 2010-5-27
//!更新 2010-5-27
//!
#pragma warning( disable: 4661)
#pragma warning( disable: 4251)
#pragma warning( disable: 4204)

#include "luaplus/LuaPlus.h"
using namespace LuaPlus;
#ifndef VGS_LUA_EXPORT
#if defined(_USRDLL) || defined(_LIB)
#define VGS_LUA_EXPORT __declspec(dllexport)
#else
#define VGS_LUA_EXPORT __declspec(dllimport)
#endif
#endif

#include "../VGSHeader.h"

// class CVGSCore;

// namespace VGS
// {
	class VGS_LUA_EXPORT LuaEngine
	{
	public:
		LuaEngine(CVGSCore* vgs);
		~LuaEngine();

		//!
		//!描述 获取最后一次的错误信息.
		//!
		const char * GetLastErrorInfo();
		//!
		//!描述 执行一个VGS脚本文件.
		//!参数 szLuaFile Lua脚本文件的名称.
		//!返回 true表示成功,false失败可通过GetLastErrorInfo()获取错误信息.
		//!
		bool DoFile(const char * szLuaFile);
		//!
		//!描述 执行一条Lua语句.
		//!参数 szLuaCmd Lua语句.
		//!返回 true表示成功,false失败可通过GetLastErrorInfo()获取错误信息.
		//!
		bool DoString(const char * szLuaCmd);

		//!
		//!描述 引擎初始化的事件.
		//!
		void OnInitVgs();
		//!
		//!描述 引擎退出的事件.
		//!
		void OnExit();
		//!
		//!描述 窗体Resize的事件.
		//!
		void OnResize();
		//!
		//!描述 程序激活的事件.
		//!
		void OnActive();
		//!
		//!描述 每渲染一帧前的事件.
		//!
		void OnStartOneFrame();
		//!
		//!描述 每渲染一帧后的事件.
		//!
		void OnEndOneFrame();
		//!
		//!描述 按键事件.
		//!参数 nKey 按键值
		//!参数 FuncKeys 功能键状态
		//!
		void OnKeyPressEvent(int nKey, int keyModifiers);
		void OnKeyDownEvent(int nKey, int keyModifiers);
		void OnKeyUpEvent(int nKey, int keyModifiers);
		//!
		//!描述 全部的鼠标按下事件.
		//!参数 id 鼠标ID号:0-left,1-right,2-middle
		//!参数 keyModifiers 功能键状态:1-ctrl,2-alt,3-shift
		//!参数 x 屏幕坐标
		//!参数 y 屏幕坐标
		//!		
		void AllMouseEnter(int keyModifiers, int x, int y);
		void AllMouseLeave(int keyModifiers, int x, int y);
		void AllMouseDown(int mouseID, int keyModifiers, int x, int y);
		void AllMouseUp(int nMouseID, int keyModifiers, int x, int y);
		void AllMouseDoubleClick(int nMouseID, int keyModifiers, int x, int y);
		void AllMouseMove(int keyModifiers, int x, int y);
		void AllMouseWheel(int keyModifiers, int x, int y, int delta);
		//!
		//!描述 鼠标进入事件.
		//!参数 szLuaFuction Lua脚本事件函数
		//!参数 FuncKeys 功能键状态
		//!参数 x 屏幕坐标
		//!参数 y 屏幕坐标
		//!		
		void OnMouseEnter(const char * szLuaFuction, int keyModifiers, int x, int y);
		//!
		//!描述 鼠标离开事件.
		//!参数 szLuaFuction Lua脚本事件函数
		//!参数 FuncKeys 功能键状态
		//!参数 x 屏幕坐标
		//!参数 y 屏幕坐标
		//!		
		void OnMouseLeave(const char * szLuaFuction, int keyModifiers, int x, int y);
		//!
		//!描述 鼠标按下事件.
		//!参数 szLuaFuction Lua脚本事件函数
		//!参数 nMouseID 鼠标ID:0-左，1-右，2-中，其他值保留
		//!参数 FuncKeys 功能键状态
		//!参数 x 屏幕坐标
		//!参数 y 屏幕坐标
		//!		
		void OnMouseDown(const char * szLuaFuction, int nMouseID, int keyModifiers, int x, int y);
		//!
		//!描述 鼠标弹起事件.
		//!参数 szLuaFuction Lua脚本事件函数
		//!参数 nMouseID 鼠标ID:0-左，1-右，2-中，其他值保留
		//!参数 FuncKeys 功能键状态
		//!参数 x 屏幕坐标
		//!参数 y 屏幕坐标
		//!		
		void OnMouseUp(const char * szLuaFuction, int nMouseID, int keyModifiers, int x, int y);
		//!
		//!描述 鼠标移动事件.
		//!参数 szLuaFuction Lua脚本事件函数
		//!参数 nMouseID 鼠标ID:0-左，1-右，2-中，其他值保留
		//!参数 FuncKeys 功能键状态
		//!参数 x 屏幕坐标
		//!参数 y 屏幕坐标
		//!		
		void OnMouseMove(const char * szLuaFuction, int nMouseID, int keyModifiers, int x, int y);
		//!
		//!描述 鼠标双击事件.
		//!参数 szLuaFuction Lua脚本事件函数
		//!参数 nMouseID 鼠标ID:0-左，1-右，2-中，其他值保留
		//!参数 FuncKeys 功能键状态
		//!参数 x 屏幕坐标
		//!参数 y 屏幕坐标
		//!		
		void OnMouseDoubleClick(const char * szLuaFuction, int nMouseID, int keyModifiers, int x, int y);

		void OnFsCommand(const char* viewName, const char* commands, const char* args);

		//following for debug
		void SetDebugCommand(const char * szDebugCmd);
		const char * GetLastDebugPrmVal(char * szParam);

	protected:
		//<LuaState智能指针.
		LuaPlus::LuaStateOwner		m_luaStateOwner;		
		//<最近一次操作失败的错误信息.
		std::string					m_strErrorInfo;			

		//following for debug
		std::string					m_strDebugDumpParam;
		std::string					m_strDebugDumpValue;

		CVGSCore*	m_pVgsCore;

		bool	m_bIinitialised;
	private:
		//!
		//!描述 注册VGS的API.
		//!
		void RegisterVGSAPI();
	private://不提供复制赋值方法
// 		LuaEngine(const LuaEngine &){}
		LuaEngine & operator = (const LuaEngine &);

	};
// }
#endif
