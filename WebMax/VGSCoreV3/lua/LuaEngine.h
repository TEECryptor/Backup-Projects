#ifndef VGS_LUA_ENGINE_H
#define VGS_LUA_ENGINE_H
#include <string>

//!
//!�ļ� VgsLua.h
//!���� ʹ��Luaplus��VGS��Ҫʹ�õĽű����з�װ������ע��VGS��API��Lua��Ҳ����ʵ�ֶ�Lua�ű��ĵ���.
//!���� linxq
//!���� 2010-5-27
//!���� 2010-5-27
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
		//!���� ��ȡ���һ�εĴ�����Ϣ.
		//!
		const char * GetLastErrorInfo();
		//!
		//!���� ִ��һ��VGS�ű��ļ�.
		//!���� szLuaFile Lua�ű��ļ�������.
		//!���� true��ʾ�ɹ�,falseʧ�ܿ�ͨ��GetLastErrorInfo()��ȡ������Ϣ.
		//!
		bool DoFile(const char * szLuaFile);
		//!
		//!���� ִ��һ��Lua���.
		//!���� szLuaCmd Lua���.
		//!���� true��ʾ�ɹ�,falseʧ�ܿ�ͨ��GetLastErrorInfo()��ȡ������Ϣ.
		//!
		bool DoString(const char * szLuaCmd);

		//!
		//!���� �����ʼ�����¼�.
		//!
		void OnInitVgs();
		//!
		//!���� �����˳����¼�.
		//!
		void OnExit();
		//!
		//!���� ����Resize���¼�.
		//!
		void OnResize();
		//!
		//!���� ���򼤻���¼�.
		//!
		void OnActive();
		//!
		//!���� ÿ��Ⱦһ֡ǰ���¼�.
		//!
		void OnStartOneFrame();
		//!
		//!���� ÿ��Ⱦһ֡����¼�.
		//!
		void OnEndOneFrame();
		//!
		//!���� �����¼�.
		//!���� nKey ����ֵ
		//!���� FuncKeys ���ܼ�״̬
		//!
		void OnKeyPressEvent(int nKey, int keyModifiers);
		void OnKeyDownEvent(int nKey, int keyModifiers);
		void OnKeyUpEvent(int nKey, int keyModifiers);
		//!
		//!���� ȫ������갴���¼�.
		//!���� id ���ID��:0-left,1-right,2-middle
		//!���� keyModifiers ���ܼ�״̬:1-ctrl,2-alt,3-shift
		//!���� x ��Ļ����
		//!���� y ��Ļ����
		//!		
		void AllMouseEnter(int keyModifiers, int x, int y);
		void AllMouseLeave(int keyModifiers, int x, int y);
		void AllMouseDown(int mouseID, int keyModifiers, int x, int y);
		void AllMouseUp(int nMouseID, int keyModifiers, int x, int y);
		void AllMouseDoubleClick(int nMouseID, int keyModifiers, int x, int y);
		void AllMouseMove(int keyModifiers, int x, int y);
		void AllMouseWheel(int keyModifiers, int x, int y, int delta);
		//!
		//!���� �������¼�.
		//!���� szLuaFuction Lua�ű��¼�����
		//!���� FuncKeys ���ܼ�״̬
		//!���� x ��Ļ����
		//!���� y ��Ļ����
		//!		
		void OnMouseEnter(const char * szLuaFuction, int keyModifiers, int x, int y);
		//!
		//!���� ����뿪�¼�.
		//!���� szLuaFuction Lua�ű��¼�����
		//!���� FuncKeys ���ܼ�״̬
		//!���� x ��Ļ����
		//!���� y ��Ļ����
		//!		
		void OnMouseLeave(const char * szLuaFuction, int keyModifiers, int x, int y);
		//!
		//!���� ��갴���¼�.
		//!���� szLuaFuction Lua�ű��¼�����
		//!���� nMouseID ���ID:0-��1-�ң�2-�У�����ֵ����
		//!���� FuncKeys ���ܼ�״̬
		//!���� x ��Ļ����
		//!���� y ��Ļ����
		//!		
		void OnMouseDown(const char * szLuaFuction, int nMouseID, int keyModifiers, int x, int y);
		//!
		//!���� ��굯���¼�.
		//!���� szLuaFuction Lua�ű��¼�����
		//!���� nMouseID ���ID:0-��1-�ң�2-�У�����ֵ����
		//!���� FuncKeys ���ܼ�״̬
		//!���� x ��Ļ����
		//!���� y ��Ļ����
		//!		
		void OnMouseUp(const char * szLuaFuction, int nMouseID, int keyModifiers, int x, int y);
		//!
		//!���� ����ƶ��¼�.
		//!���� szLuaFuction Lua�ű��¼�����
		//!���� nMouseID ���ID:0-��1-�ң�2-�У�����ֵ����
		//!���� FuncKeys ���ܼ�״̬
		//!���� x ��Ļ����
		//!���� y ��Ļ����
		//!		
		void OnMouseMove(const char * szLuaFuction, int nMouseID, int keyModifiers, int x, int y);
		//!
		//!���� ���˫���¼�.
		//!���� szLuaFuction Lua�ű��¼�����
		//!���� nMouseID ���ID:0-��1-�ң�2-�У�����ֵ����
		//!���� FuncKeys ���ܼ�״̬
		//!���� x ��Ļ����
		//!���� y ��Ļ����
		//!		
		void OnMouseDoubleClick(const char * szLuaFuction, int nMouseID, int keyModifiers, int x, int y);

		void OnFsCommand(const char* viewName, const char* commands, const char* args);

		//following for debug
		void SetDebugCommand(const char * szDebugCmd);
		const char * GetLastDebugPrmVal(char * szParam);

	protected:
		//<LuaState����ָ��.
		LuaPlus::LuaStateOwner		m_luaStateOwner;		
		//<���һ�β���ʧ�ܵĴ�����Ϣ.
		std::string					m_strErrorInfo;			

		//following for debug
		std::string					m_strDebugDumpParam;
		std::string					m_strDebugDumpValue;

		CVGSCore*	m_pVgsCore;

		bool	m_bIinitialised;
	private:
		//!
		//!���� ע��VGS��API.
		//!
		void RegisterVGSAPI();
	private://���ṩ���Ƹ�ֵ����
// 		LuaEngine(const LuaEngine &){}
		LuaEngine & operator = (const LuaEngine &);

	};
// }
#endif
