#ifndef __VGS_SCRIPT_H__
#define __VGS_SCRIPT_H__

//!
//!�ļ� VgsScript.hpp
//!���� �����ṩC��׼��API�����ڲ���VGS�Ľű�.
//!		�ű�������VGS���洴���͹����ⲿ����ֻ��ʹ��.
//!�ر�˵�� 	���ܼ�״̬˵��:ʹ��һ���ֽ�:00000000,λ1��ʾ���ڸü�,D0(���λ)-Ctrl, D1-Shift, D2-Alt,D3~D7����
//!���� linxq
//!���� 2008-12-08
//!���� 2010-6-10
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
//!���� ��ȡVGS�ű�����İ汾����.
//!
LUA_C_EXPORT const char *  __stdcall GetVgsScripterVersion();

//!
//!���� ��ȡ���һ�β���ʧ�ܵ���Ϣ.
//!���� ������Ϣ.
//!
LUA_C_EXPORT const char * __stdcall GetVgsScriptLastError();
//!
//!���� ִ��һ��Lua�ű��ļ�.
//!���� szScriptFile �ű��ļ�������
//!���� true��ʾ�ɹ�,false��ʾʧ�ܿ�ͨ��GetVgsScriptLastError()��ȡ������Ϣ.
//!
LUA_C_EXPORT bool	__stdcall ExecuteLuaScript(const char * szScriptFile);
//!
//!���� ִ��һ��Lua�ű����.
//!���� szFuntionName �ű�����������
//!���� true��ʾ�ɹ�,false��ʾʧ�ܿ�ͨ��GetVgsScriptLastError()��ȡ������Ϣ.
//!
LUA_C_EXPORT bool	__stdcall ExecuteLuaCmd(const char * szLuaCmd);

//!
//!���� �����ʼ�����¼�.
//!
LUA_C_EXPORT void __stdcall OnInitVgs();
//!
//!���� �����˳����¼�.
//!
LUA_C_EXPORT void __stdcall OnExitEvent();
//!
//!���� ����Resize���¼�.
//!
LUA_C_EXPORT void __stdcall OnResizeEvent();
//!
//!���� ���򼤻���¼�.
//!
LUA_C_EXPORT void __stdcall OnActiveEvent();
//!
//!���� ÿ��Ⱦһ֡ǰ���¼�.
//!
LUA_C_EXPORT void __stdcall OnStartOneFrameEvent();
//!
//!���� ÿ��Ⱦһ֡����¼�.
//!
LUA_C_EXPORT void __stdcall OnEndOneFrameEvent();
//!
//!���� �����¼�.
//!���� nKey ����ֵ
//!���� FuncKeys ���ܼ�״̬
//!
LUA_C_EXPORT void __stdcall OnKeyPressEvent(int nKey, unsigned char FuncKeys);
//!
//!���� ���������¼�.
//!���� nKey ����ֵ
//!���� FuncKeys ���ܼ�״̬
//!
LUA_C_EXPORT void __stdcall OnKeyDownEvent(int nKey, unsigned char FuncKeys);
//!
//!���� ���������¼�.
//!���� nKey ����ֵ
//!���� FuncKeys ���ܼ�״̬
//!
LUA_C_EXPORT void __stdcall OnKeyUpEvent(int nKey, unsigned char FuncKeys);
//!
//!���� �������¼�.
//!���� szLuaFuction Lua�ű��¼�����
//!���� FuncKeys ���ܼ�״̬
//!���� x ��Ļ����
//!���� y ��Ļ����
//!		
LUA_C_EXPORT void __stdcall OnMouseEnterEvent(const char * szLuaFuction, unsigned char FuncKeys, int x, int y);
//!
//!���� ����뿪�¼�.
//!���� szLuaFuction Lua�ű��¼�����
//!���� FuncKeys ���ܼ�״̬
//!���� x ��Ļ����
//!���� y ��Ļ����
//!		
LUA_C_EXPORT void __stdcall OnMouseLeaveEvent(const char * szLuaFuction, unsigned char FuncKeys, int x, int y);
//!
//!���� ��갴���¼�.
//!���� szLuaFuction Lua�ű��¼�����
//!���� nMouseID ���ID:0-��1-�ң�2-�У�����ֵ����
//!���� FuncKeys ���ܼ�״̬
//!���� x ��Ļ����
//!���� y ��Ļ����
//!		
LUA_C_EXPORT void __stdcall OnMouseDownEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y);
//!
//!���� ��굯���¼�.
//!���� szLuaFuction Lua�ű��¼�����
//!���� nMouseID ���ID:0-��1-�ң�2-�У�����ֵ����
//!���� FuncKeys ���ܼ�״̬
//!���� x ��Ļ����
//!���� y ��Ļ����
//!		
LUA_C_EXPORT void __stdcall OnMouseUpEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y);
//!
//!���� ����ƶ��¼�.
//!���� szLuaFuction Lua�ű��¼�����
//!���� nMouseID ���ID:0-��1-�ң�2-�У�����ֵ����
//!���� FuncKeys ���ܼ�״̬
//!���� x ��Ļ����
//!���� y ��Ļ����
//!		
LUA_C_EXPORT void __stdcall OnMouseMoveEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y);
//!
//!���� ���˫���¼�.
//!���� szLuaFuction Lua�ű��¼�����
//!���� nMouseID ���ID:0-��1-�ң�2-�У�����ֵ����
//!���� FuncKeys ���ܼ�״̬
//!���� x ��Ļ����
//!���� y ��Ļ����
//!		
LUA_C_EXPORT void __stdcall OnMouseDoubleClickEvent(const char * szLuaFuction, int nMouseID, unsigned char FuncKeys, int x, int y);

//!����	����ʹ�á���
//ִ��һ������ָ��
LUA_C_EXPORT void __stdcall SetDebugCmd(const char * szDebugCmd);
//�������Ĵ��ھ�����ø�Luaʹ��
LUA_C_EXPORT void __stdcall SetHostHandleToLua(void* hWnd);
//��ȡ���һ�εĲ���/ֵ��
LUA_C_EXPORT const char * __stdcall GetLastDebugPrmVal(char * szParam);

#ifdef __cplusplus
}
#endif

#endif