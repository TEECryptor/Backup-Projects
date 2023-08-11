#include "VgsApi.h"
#include "../VGSDef.h"
#include "../VGSCore.h"

#include <string>

// #pragma comment(lib, "vgs/VGSCoreDLL.lib")

unsigned long g_ulMainScene = 0;//定义全局变量并初始化，主场景句柄

void* g_hHostAppHandle = 0;//宿主程序的窗口句柄，如果有.

int VGS_Sleep(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		int ms = args[1].GetInteger();
		Sleep(ms);
	}
	return 0;
}
int VGS_MessageBox(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsString())
	{
		std::string str = args[1].GetString();
		std::string str2 = args[2].GetString();
		MessageBoxA(0, str.c_str(), str2.c_str(), MB_OK);
	}
	return 0;
}
//传入参数:1-msg ID, 2-wparam, 3-lparam
int VGS_SendMsgToHost(LuaState * state)
{
	if (g_hHostAppHandle <= 0)
	{
		MessageBoxA(0, "未设置宿主程序的窗口句柄！", "错误", MB_OK);
		return 0;
	}
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger() && args[3].IsInteger())
	{
		UINT msg = args[1].GetInteger();
		WPARAM wPrm = args[2].GetInteger();
		LPARAM lPrm = args[3].GetInteger();
		::SendMessageA((HWND)(g_hHostAppHandle), msg, wPrm, lPrm);
	}
	return 0;
}

// unsigned long g_hMainScene =  GetSceneHandleByIndex(0);

// int VGS_GetSceneCount(LuaState * state)
// {
// 	LuaStack args(state);
// 	
// 	int nCount = GetSceneCount();
// 	state->PushInteger(nCount);
// 
// 	return 1;
// }
// int VGS_GetSceneHandleByIndex(LuaState * state)
// {
// 	LuaStack args(state);
// 	if (args[1].IsInteger())
// 	{
// 		unsigned long ulScene =  GetSceneHandleByIndex(args[1].GetInteger());
// 		state->PushInteger(ulScene);
// 		return 1;
// 	}
// 	return 0;
// }
//int VGS_SetMainScene(LuaState * state)
//{
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		int idx = args[1].GetInteger();
//		if (idx < 0)
//			idx = 0;
//		g_ulMainScene =  GetSceneHandleByIndex(idx);
//	}
//	return 0;
//}
////----场景操作相关----start
////render
//int VGS_RenderAll(LuaState * state)
//{
//	RenderAll();
//	return 0;
//}
//// int VGS_RenderScene(LuaState * state)
//// {
//// 	LuaStack args(state);
//// 	if (args[1].IsInteger())
//// 	{
//// 		unsigned long ulScene =  args[1].GetInteger();
//// 		if (0 != ulScene)
//// 		{
//// 			RenderScene(ulScene);
//// 			return 0;
//// 		}
//// 	}
//// 	return 0;
//// }
//// int VGS_RenderCurrentFrame(LuaState * state)
//// {
//// 	LuaStack args(state);
//// 	if (args[1].IsInteger())
//// 	{
//// 		unsigned long ulScene =  args[1].GetInteger();
//// 		if (0 != ulScene)
//// 		{
//// 			RenderCurrentFrame(ulScene);
//// 			return 0;
//// 		}
//// 	}
//// 	return 0;
//// }
//int VGS_GetViewportFillMode(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulViewport =  args[1].GetInteger();
//		if (0 == ulViewport)
//		{
//			return 0;
//		}
//		int nFillMode = GetViewportFillMode(g_ulMainScene, ulViewport);
//		state->PushInteger(nFillMode);
//		return 1;
//	}
//	return 0;
//}
//int VGS_SetViewportFillMode(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger())
//	{
//		unsigned long ulViewport =  args[1].GetInteger();
//		int nFillMode = args[2].GetInteger();
//		if (0 == ulViewport)
//		{
//			return 0;
//		}
//		SetViewportFillMode(g_ulMainScene, ulViewport, nFillMode);
//		return 0;
//	}
//	return 0;
//}
////viewport
//int VGS_AddViewPort(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger()
//		 && args[3].IsNumber()  && args[4].IsNumber()  && args[5].IsNumber()  && args[6].IsNumber() )
//	{
//		unsigned long ulCamera =  args[1].GetInteger();
//		if (0 == ulCamera)
//		{
//			return 0;
//		}
//		int nLevel = args[2].GetInteger();
//		float left = (float)args[3].GetNumber();
//		float top = (float)args[4].GetNumber();
//		float width = (float)args[5].GetNumber();
//		float height = (float)args[6].GetNumber();
//		unsigned long ulVp = AddViewport(g_ulMainScene, ulCamera, nLevel, left, top, width, height);
//		state->PushInteger(ulVp);
//		return 1;
//	}
//	return 0;
//}
//int VGS_SetViewportCamera(LuaState * state)
//{
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger())
//	{
//		unsigned long ulViewport =  args[1].GetInteger();
//		unsigned long ulCamera =  args[2].GetInteger();
//		if (0 == ulViewport || 0 == ulCamera)
//		{
//			return 0;
//		}
//		SetViewportCamera(ulViewport, ulCamera);
//		return 0;
//	}
//	return 0;
//}
//int VGS_GetViewportCamera(LuaState * state)
//{
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulViewport =  args[1].GetInteger();
//		if (0 == ulViewport)
//		{
//			return 0;
//		}
//		unsigned long ulCamera =  GetViewportCamera(ulViewport);
//		state->PushInteger(ulCamera);
//		return 1;
//	}
//	return 0;
//}
//int VGS_GetViewPortRect(LuaState * state)
//{
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulViewport =  args[1].GetInteger();
//		if (0 == ulViewport)
//		{
//			return 0;
//		}
//		float left = 0.0f, top = 0.0f, width = 0.0f, height = 0.0f;
//		GetViewportRect(ulViewport, left, top, width, height);
//		state->PushNumber(left);
//		state->PushNumber(top);
//		state->PushNumber(width);
//		state->PushNumber(height);
//		return 4;
//	}
//	return 0;
//}
//int VGS_SetViewPortRect(LuaState * state)
//{
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		//Rect采用table传递:{left, top, width, height}
//		unsigned long ulViewport =  args[1].GetInteger();
//		if (0 == ulViewport)
//		{
//			return 0;
//		}
//		LuaObject table = args[2];
//		float left = (float)table[1].GetNumber();
//		float top = (float)table[2].GetNumber();
//		float width = (float)table[3].GetNumber();
//		float height = (float)table[4].GetNumber();
//		SetViewportRect(ulViewport, left, top, width, height);
//		return 0;
//	}
//	return 0;
//}
//int VGS_UpdateViewportAspectRatio(LuaState * state)
//{
//	LuaStack args(state);
//	if (args[1].IsInteger() )
//	{
//		unsigned long ulViewport =  args[1].GetInteger();
//		if (0 == ulViewport)
//		{
//			return 0;
//		}
//		UpdateViewportAspectRatio(ulViewport);
//		return 0;
//	}
//	return 0;
//}
//int VGS_IsViewportRenderGUI(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulViewport =  args[1].GetInteger();
//		if (0 == ulViewport)
//		{
//			return 0;
//		}
//		bool bres = IsViewportRenderGUI(g_ulMainScene, ulViewport);
//		state->PushBoolean(bres);
//		return 1;
//	}
//	return 0;
//}
//int VGS_SetViewportRenderGUI(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger()&& args[2].IsBoolean())
//	{
//		unsigned long ulViewport =  args[1].GetInteger();
//		if (0 == ulViewport)
//		{
//			return 0;
//		}
//		bool bFlag = args[2].GetBoolean();
//		SetViewportRenderGUI(g_ulMainScene, ulViewport, bFlag);
//		return 0;
//	}
//	return 0;
//}
//int VGS_GetSceneBackColor(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulViewport =  args[1].GetInteger();
//		if (0 == ulViewport)
//		{
//			return 0;
//		}
//		unsigned long bgcolor = GetSceneBackColor(g_ulMainScene, ulViewport);
//		state->PushInteger(bgcolor);
//		return 1;
//	}
//	return 0;
//}
//int VGS_SetSceneBackColor(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger())
//	{
//		unsigned long ulViewport =  args[1].GetInteger();
//		if (0 == ulViewport)
//		{
//			return 0;
//		}
//		unsigned long bgcolor = args[2].GetInteger();
//		SetSceneBackColor(g_ulMainScene, ulViewport, bgcolor);
//		return 0;
//	}
//	return 0;
//}
////----场景操作相关----end
//int VGS_GetModelCount(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	int nCount = GetModelCount(g_ulMainScene);
//	state->PushInteger(nCount);
//	return 1;
//}
//int VGS_GetModelHandleByIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		int nIndex = args[1].GetInteger();
//		unsigned long ulModel = GetModelHandleByIndex(g_ulMainScene, nIndex);
//		state->PushInteger(ulModel);
//		return 1;
//	}
//	return 0;
//}
//int VGS_GetModelNameByIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger())
//	{
//		int nIndex = args[1].GetInteger();
//		char buf[MAX_NAMESTR_LENGTH];
//		ZeroMemory(buf, MAX_NAMESTR_LENGTH);
//		bool bFlag = GetModelNameByIndex(g_ulMainScene, nIndex, buf);
//		if (!bFlag)
//			return 0;
//		state->PushString(buf);
//		return 1;
//	}
//	return 0;
//}
//int VGS_GetModelNameByHandle(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		char buf[MAX_NAMESTR_LENGTH];
//		ZeroMemory(buf, MAX_NAMESTR_LENGTH);
//		bool bFlag = GetModelNameByHandle(g_ulMainScene, ulModel, buf);
//		if (!bFlag)
//			return 0;
//		state->PushString(buf);
//		return 1;
//	}
//	return 0;
//}
//int VGS_GetModelHandleByName(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsString())
//	{
//		std::string strName = args[1].GetString();
//		if (strName.empty())
//		{
//			return 0;
//		}
//		unsigned long ulModel = GetModelHandleByName(g_ulMainScene, (char *)strName.c_str());
//		state->PushInteger(ulModel);
//		return 1;
//	}
//	return 0;
//}
////delete
//int VGS_DeleteModel(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsString())
//	{
//		std::string strName = args[1].GetString();
//		if (strName.empty())
//		{
//			return 0;
//		}
//		bool bFlag = DeleteModel(g_ulMainScene, (char *)strName.c_str());
//		state->PushBoolean(bFlag);
//		return 1;
//	}
//	return 0;
//}
////transform
//int VGS_GetModelPosition(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		VECTOR3 pos;
//		GetModelPosition(g_ulMainScene, ulModel, &pos);
//		state->PushNumber(pos.x);
//		state->PushNumber(pos.y);
//		state->PushNumber(pos.z);
//		return 3;
//	}
//	return 0;
//}
//int VGS_SetModelPosition(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VECTOR3 pos;
//		pos.x = (float)table[1].GetNumber();
//		pos.y = (float)table[2].GetNumber();
//		pos.z = (float)table[3].GetNumber();
//
//		SetModelPosition(g_ulMainScene, ulModel, &pos);
//
//		return 0;
//	}
//	return 0;
//}
//int VGS_MoveModel(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable() && args[3].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VECTOR3 pos;
//		pos.x = (float)table[1].GetNumber();
//		pos.y = (float)table[2].GetNumber();
//		pos.z = (float)table[3].GetNumber();
//		
//		unsigned int nFlag = args[3].GetInteger();
//
//		MoveModel(g_ulMainScene, ulModel, &pos, nFlag);
//
//		return 0;
//	}
//	return 0;
//}
//int VGS_GetModelRotation(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		VECTOR3 pos;
//		if (GetModelRotation(g_ulMainScene, ulModel, &pos))
//		{
//			state->PushNumber(pos.x);
//			state->PushNumber(pos.y);
//			state->PushNumber(pos.z);
//			return 3;
//		}
//	}
//	return 0;
//}
//int VGS_SetModelRotation(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		LuaObject table = args[2];
//		VECTOR3 v3;
//		v3.x = table[1].GetNumber();
//		v3.y = table[2].GetNumber();
//		v3.z = table[3].GetNumber();
//		SetModelRotation(g_ulMainScene, ulModel, &v3);
//	}
//	return 0;
//}
//int VGS_RotateModel(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable() && args[3].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		LuaObject table = args[2];
//		VECTOR3 v3;
//		v3.x = table[1].GetNumber();
//		v3.y = table[2].GetNumber();
//		v3.z = table[3].GetNumber();
//
//		VGS_TRANSFORMSPACE ts = (VGS_TRANSFORMSPACE)(args[3].GetInteger());
//
//		RotateModel(g_ulMainScene, ulModel, &v3, ts);
//	}
//	return 0;
//}
//int VGS_GetModelScale(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		VECTOR3 pos;
//		if (GetModelScale(g_ulMainScene, ulModel, &pos))
//		{
//			state->PushNumber(pos.x);
//			state->PushNumber(pos.y);
//			state->PushNumber(pos.z);
//			return 3;
//		}
//	}
//	return 0;
//}
//int VGS_SetModelScale(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		LuaObject table = args[2];
//		VECTOR3 v3;
//		v3.x = table[1].GetNumber();
//		v3.y = table[2].GetNumber();
//		v3.z = table[3].GetNumber();
//		SetModelScale(g_ulMainScene, ulModel, &v3);
//	}
//	return 0;
//}
//int VGS_ScaleModel(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		LuaObject table = args[2];
//		VECTOR3 v3;
//		v3.x = table[1].GetNumber();
//		v3.y = table[2].GetNumber();
//		v3.z = table[3].GetNumber();
//		ScaleModel(g_ulMainScene, ulModel, &v3);
//	}
//	return 0;
//}
////property
//// int VGS_GetModelInfo(LuaState * state)
//// {
//// 	if (0 == g_ulMainScene)
//// 		return 0;
//// 	LuaStack args(state);
//// 	if (args[1].IsInteger())
//// 	{
//// 		unsigned long ulModel = args[1].GetInteger();
//// 		if (0 == ulModel)
//// 		{
//// 			return 0;
//// 		}
//// 		VGS_ModelInfo info;
//// 		if (FALSE == GetModelInfo(g_ulMainScene, ulModel, info))
//// 			return 0;
//// 		state->PushBoolean(info.isBillboard);
//// 		state->PushBoolean(info.isCollision);
//// 		state->PushBoolean(info.isGround);
//// 		state->PushBoolean(info.isVisible);
//// 		state->PushBoolean(info.isClickable);
//// 		state->PushInteger((int)(info.dirctionAxis));
//// 		return 6;
//// 	}
//// 	return 0;
//// }
//// int VGS_SetModelInfo(LuaState * state)
//// {
//// 
//// 	return 0;
//// }
//int VGS_GetModelBillboardStatus(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		VGS_AXIS_TYPE axis;
//		if (FALSE == GetModelBillboardStatus(g_ulMainScene, ulModel, axis))
//			state->PushBoolean(false);
//		else
//			state->PushBoolean(true);
//		return 1;
//	}
//
//	return 0;
//}
//int VGS_SetModelBillboardStatus(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsBoolean() && args[3].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		VGS_AXIS_TYPE axis = (VGS_AXIS_TYPE)args[3].GetInteger();
//		SetModelBillboardStatus(g_ulMainScene, ulModel, args[2].GetBoolean(), axis);
//	}
//
//	return 0;
//}
//int VGS_GetModelCollisionStatus(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		if (FALSE == GetModelCollisionStatus(g_ulMainScene, ulModel))
//			state->PushBoolean(false);
//		else
//			state->PushBoolean(true);
//		return 1;
//	}
//
//	return 0;
//}
//int VGS_SetModelCollisionStatus(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsBoolean())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		SetModelCollisionStatus(g_ulMainScene, ulModel, args[2].GetBoolean());
//	}
//
//	return 0;
//}
//int VGS_GetModelAsGround(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		if (FALSE == GetModelAsGround(g_ulMainScene, ulModel))
//			state->PushBoolean(false);
//		else
//			state->PushBoolean(true);
//		return 1;
//	}
//
//	return 0;
//}
//int VGS_SetModelAsGround(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsBoolean())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		SetModelAsGround(g_ulMainScene, ulModel, args[2].GetBoolean());
//	}
//
//	return 0;
//}
//int VGS_GetModelVisibility(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		if (FALSE == GetModelVisibility(g_ulMainScene, ulModel))
//			state->PushBoolean(false);
//		else
//			state->PushBoolean(true);
//		return 1;
//	}
//
//	return 0;
//}
//int VGS_SetModelVisibility(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsBoolean())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		SetModelVisibility(g_ulMainScene, ulModel, args[2].GetBoolean());
//	}
//
//	return 0;
//}
//int VGS_GetModelClickabel(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		if (FALSE == GetModelClickabel(g_ulMainScene, ulModel))
//			state->PushBoolean(false);
//		else
//			state->PushBoolean(true);
//		return 1;
//	}
//
//	return 0;
//}
//int VGS_SetModelClickable(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsBoolean())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		SetModelClickable(g_ulMainScene, ulModel, args[2].GetBoolean());
//	}
//
//	return 0;
//}
//
//int VGS_IsModelHasKeyframeAnimation(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		if (FALSE == IsModelHasKeyframeAnimation(g_ulMainScene, ulModel))
//			state->PushBoolean(false);
//		else
//			state->PushBoolean(true);
//		return 1;
//	}
//	return 0;
//}
//int VGS_IsModelHasSkeletonAnimation(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel)
//		{
//			return 0;
//		}
//		if (FALSE == IsModelHasSkeletonAnimation(g_ulMainScene, ulModel))
//			state->PushBoolean(false);
//		else
//			state->PushBoolean(true);
//		return 1;
//	}
//	return 0;
//}
//int VGS_SetModelMaterial(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsString())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		std::string strMtrlName = args[2].GetString();
//		if (0 == ulModel || strMtrlName.empty())
//		{
//			return 0;
//		}
//		SetModelMaterial(g_ulMainScene, ulModel, strMtrlName.c_str());
//	}
//	return 0;
//}
//int VGS_GetSubModelCount(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		if (0 == ulModel )
//		{
//			return 0;
//		}
//		int nCount = GetSubModelCount(g_ulMainScene, ulModel);
//		state->PushInteger(nCount);
//		return 1;
//	}
//
//	return 0;
//}
//int VGS_GetSubModelMaterialName(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		int nIndex = args[2].GetInteger();
//		if (0 == ulModel || nIndex < 0)
//		{
//			return 0;
//		}
//		char name[MAX_NAMESTR_LENGTH];
//		ZeroMemory(name, MAX_NAMESTR_LENGTH);
//		GetSubModelMaterialName(ulModel,nIndex, name);
//		state->PushString(name);
//		return 1;
//	}
//	return 0;
//}
//int VGS_GetSubModelMaterialHandle(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		int nIndex = args[2].GetInteger();
//		if (0 == ulModel || nIndex < 0)
//		{
//			return 0;
//		}
//		unsigned long ulMtrl = GetSubModelMaterialHandle(ulModel,nIndex);
//		state->PushInteger(ulMtrl);
//		return 1;
//	}
//	return 0;
//}
//int VGS_SetSubModelMaterial(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger() && args[3].IsString())
//	{
//		unsigned long ulModel = args[1].GetInteger();
//		int nIndex = args[2].GetInteger();
//		std::string strMtrl = args[3].GetString();
//		if (0 == ulModel || nIndex < 0 || strMtrl.empty())
//		{
//			return 0;
//		}
//		SetSubModelMaterial(g_ulMainScene, ulModel,nIndex, strMtrl.c_str());
//	}
//	return 0;
//}
////----模型操作----end
////----camera----start
//int VGS_CreateCamera(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsString())
//	{
//		std::string strCamera = args[1].GetString();
//		if (strCamera.empty())
//		{
//			return 0;
//		}
//		unsigned long hCamera = CreateCamera(g_ulMainScene, (CHAR *)strCamera.c_str());
//		state->PushInteger(hCamera);
//		return 1;
//	}
//	return 0;
//}
//int VGS_DeleteCamerabyHandle(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		if (hCamera <= 0)
//		{
//			return 0;
//		}
//		DeleteCamerabyHandle(g_ulMainScene, hCamera);
//	}
//	return 0;
//}
//int VGS_DeleteCamerabyName(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsString())
//	{
//		std::string strName = args[1].GetString();
//		if (strName.empty())
//			return 0;
//		DeleteCamerabyName(g_ulMainScene, (CHAR *)strName.c_str());
//	}
//	return 0;
//}
////get/set
//int VGS_GetCameraCount(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	int nCount = GetCameraCount (g_ulMainScene);
//	state->PushInteger(nCount);
//	return 1;
//}
//int VGS_GetCameraHandleByIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hCamera = 0l;
//		int nIndex = args[1].GetInteger();
//		if (nIndex >= 0)
//		{
//			hCamera = GetCameraHandleByIndex(g_ulMainScene, nIndex);
//		}
//		state->PushInteger(hCamera);
//		return 1;
//	}
//	return 0;
//}
//int VGS_GetCameraHandleByName(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsString())
//	{
//		unsigned long hCamera = 0l;
//		std::string strName = args[1].GetString();
//		if (!strName.empty())
//		{
//			hCamera = GetCameraHandleByName(g_ulMainScene, (CHAR *)strName.c_str());
//		}
//		state->PushInteger(hCamera);
//		return 1;
//	}
//	return 0;
//}
//int VGS_GetCameraNameByHandle(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		std::string strName = args[1].GetString();
//		if (!strName.empty())
//		{
//			hCamera = GetCameraHandleByName(g_ulMainScene, (CHAR *)strName.c_str());
//		}
//		state->PushInteger(hCamera);
//		return 1;
//	}
//	return 0;
//}
//int VGS_GetCameraNameByIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		int nIndex = args[1].GetInteger();
//		if (nIndex >= 0)
//		{
//			char name[MAX_NAMESTR_LENGTH];
//			ZeroMemory(name, MAX_NAMESTR_LENGTH);
//			GetCameraNameByIndex(g_ulMainScene, nIndex, name);
//			state->PushString(name);
//			return 1;
//		}
//	}
//	return 0;
//}
////transform
//int VGS_MoveCamera(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable() && args[3].IsInteger())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		if (0 == hCamera)
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VECTOR3 pos;
//		pos.x = (float)table[1].GetNumber();
//		pos.y = (float)table[2].GetNumber();
//		pos.z = (float)table[3].GetNumber();
//		unsigned int coordEnum = args[3].GetInteger();
//
//		MoveCamera(g_ulMainScene, hCamera, &pos, coordEnum);
//	}
//	return 0;
//}
//int VGS_RotateCamera(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable() && args[3].IsInteger())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		if (0 == hCamera)
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VECTOR3 pos;
//		pos.x = (float)table[1].GetNumber();
//		pos.y = (float)table[2].GetNumber();
//		pos.z = (float)table[3].GetNumber();
//		unsigned int coordEnum = args[3].GetInteger();
//
//		RotateCamera(g_ulMainScene, hCamera, &pos, coordEnum);
//	}
//	return 0;
//}
//int VGS_RoundCamera(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable() && args[3].IsInteger() && args[4].IsNumber())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		if (0 == hCamera)
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VECTOR3 pos;
//		pos.x = (float)table[1].GetNumber();
//		pos.y = (float)table[2].GetNumber();
//		pos.z = (float)table[3].GetNumber();
//		int axisType = args[3].GetInteger();
//		float fAngle = args[4].GetNumber();
//
//		RoundCamera(g_ulMainScene, hCamera, &pos, axisType, fAngle);
//	}
//	return 0;
//}
////property
//int VGS_GetCameraDirection(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		if (0 == hCamera)
//			return 0;
//		VECTOR3 dir;
//		ZeroMemory(&dir, sizeof(dir));
//		GetCameraDirection(g_ulMainScene, hCamera, dir);
//		state->PushNumber(dir.x);
//		state->PushNumber(dir.y);
//		state->PushNumber(dir.z);
//		return 3;
//	}
//	return 0;
//}
//int VGS_SetCameraDirection(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		if (0 == hCamera)
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VECTOR3 pos;
//		pos.x = (float)table[1].GetNumber();
//		pos.y = (float)table[2].GetNumber();
//		pos.z = (float)table[3].GetNumber();
//
//		SetCameraDirection(g_ulMainScene, hCamera, pos);
//	}
//	return 0;
//}
//int VGS_SetCameraLookAtModel(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		unsigned long hModel = args[2].GetInteger();
//		if (0 == hCamera || 0 == hModel)
//			return 0;
//		SetCameraLookAtModel(g_ulMainScene, hCamera, hModel);
//	}
//	return 0;
//}
//int VGS_SetCameraFov(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsNumber())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		if (0 == hCamera)
//			return 0;
//		float fov = args[2].GetNumber();
//		SetCameraFov(g_ulMainScene, hCamera, fov);
//	}
//	return 0;
//}
//int VGS_SetCameraNearClip(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsNumber())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		if (0 == hCamera)
//			return 0;
//		float nearClip = args[2].GetNumber();
//		SetCameraNearClip(g_ulMainScene, hCamera, nearClip);
//	}
//	return 0;
//}
//int VGS_SetCameraFarClip(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsNumber())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		if (0 == hCamera)
//			return 0;
//		float farClip = args[2].GetNumber();
//		SetCameraFarClip(g_ulMainScene, hCamera, farClip);
//	}
//	return 0;
//}
//int VGS_SetCameraPosition(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		if (0 == hCamera)
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VECTOR3 pos;
//		pos.x = (float)table[1].GetNumber();
//		pos.y = (float)table[2].GetNumber();
//		pos.z = (float)table[3].GetNumber();
//
//		SetCameraPosition(g_ulMainScene, hCamera, &pos);
//	}
//	return 0;
//}
//int VGS_SetBillboardCamera(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hCamera = args[1].GetInteger();
//		if (0 == hCamera)
//			return 0;
//		SetBillboardCamera(g_ulMainScene, hCamera);
//	}
//	return 0;
//}
//int VGS_GetBillboardCamera(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	unsigned long hCamera = 0L;
//	hCamera = GetBillboardCamera(g_ulMainScene);
//	state->PushInteger(hCamera);
//	return 1;
//}
////----camera----end
////----灯光----start
//int VGS_DeleteLight(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (0 == hLight)
//			return 0;
//		DeleteLight(g_ulMainScene, hLight);
//	}
//	return 0;
//}
//
//int VGS_GetLightCount(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	int nCount = GetLightCount(g_ulMainScene);
//	state->PushInteger(nCount);
//	return 1;
//}
//
//int VGS_GetLightHandleByIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		int nIndex = args[1].GetInteger();
//		if (nIndex < 0)
//			return 0;
//		unsigned long hLight = GetLightHandleByIndex(g_ulMainScene , nIndex);
//		state->PushInteger(hLight);
//		return 1;
//	}
//	return 0;
//}
//
//int VGS_GetLightNameByIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		int nIndex = args[1].GetInteger();
//		if (nIndex < 0)
//			return 0;
//		char name[MAX_NAMESTR_LENGTH];
//		ZeroMemory(name, MAX_NAMESTR_LENGTH);
//		GetLightNameByIndex(g_ulMainScene , nIndex, name);
//		state->PushString(name);
//		return 1;
//	}
//	return 0;
//}
//
//int VGS_GetLightNameByHandle(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0)
//			return 0;
//		char name[MAX_NAMESTR_LENGTH];
//		ZeroMemory(name, MAX_NAMESTR_LENGTH);
//		GetLightNameByHandle(g_ulMainScene , hLight, name);
//		state->PushString(name);
//		return 1;
//	}
//	return 0;
//}
//
//int VGS_GetLightHandleByName(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsString())
//	{
//		std::string strName = args[1].GetString();
//		if ( strName.empty())
//			return 0;
//		unsigned long hLight = GetLightHandleByName(g_ulMainScene, (char *)strName.c_str());
//		state->PushInteger(hLight);
//		return 1;
//	}
//
//	return 0;
//}
//
//int VGS_MoveLight(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0)
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VECTOR3 vec;
//		vec.x = (float)table[1].GetNumber();
//		vec.y = (float)table[2].GetNumber();
//		vec.z = (float)table[3].GetNumber();
//
//		MoveLight(g_ulMainScene, hLight, &vec);
//	}
//
//	return 0;
//}
//void VGS_GetLightInfo(unsigned long hLight, LightInfo & info)
//{
//	ZeroMemory(&info, sizeof(info));
//	if (hLight <= 0)
//		return;
//	GetLightInfo(g_ulMainScene, hLight, &info);
//}
//int VGS_GetLightType(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		state->PushInteger(info.Type);
//		return 1;
//	}
//	return 0;
//}
//int VGS_SetLightType(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		VGS_LIGHT_TYPE nType = (VGS_LIGHT_TYPE)(args[2].GetInteger());
//		if (hLight <= 0 || nType < 0 )
//			return 0;
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		info.Type = nType;
//		SetLightInfo(g_ulMainScene, hLight, &info);
//	}
//	return 0;
//}
//int VGS_GetLightDiffuse(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0  )
//			return 0;
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		VGSRGB color = info.Diffuse;
//		state->PushInteger(color.red);
//		state->PushInteger(color.green);
//		state->PushInteger(color.blue);
//		return 3;		
//	}
//	return 0;
//}
//int VGS_SetLightDiffuse(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0 )
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VGSRGB color;
//		color.red = table[1].GetInteger();
//		color.green = table[2].GetInteger();
//		color.blue = table[3].GetInteger();
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		info.Diffuse = color;
//		SetLightInfo(g_ulMainScene, hLight, &info);
//	}
//	return 0;
//}
//int VGS_GetLightAmbient(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0  )
//			return 0;
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		VGSRGB color = info.Ambient;
//		state->PushInteger(color.red);
//		state->PushInteger(color.green);
//		state->PushInteger(color.blue);
//		return 3;		
//	}
//
//	return 0;
//}
//int VGS_SetLightAmbient(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0 )
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VGSRGB color;
//		color.red = table[1].GetInteger();
//		color.green = table[2].GetInteger();
//		color.blue = table[3].GetInteger();
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		info.Ambient = color;
//		SetLightInfo(g_ulMainScene, hLight, &info);
//	}
//	return 0;
//}
//int VGS_GetLightMultiply(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0  )
//			return 0;
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		float fMultiply = info.Multiply;
//		state->PushNumber(fMultiply);
//		return 1;		
//	}
//
//	return 0;
//}
//int VGS_SetLightMultiply(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsNumber())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0 )
//			return 0;
//		float fMultiply = args[2].GetFloat();
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		info.Multiply = fMultiply;
//		SetLightInfo(g_ulMainScene, hLight, &info);
//	}
//	return 0;
//}
//int VGS_GetLightPosition(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0  )
//			return 0;
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		VECTOR3 vec = info.Position;
//		state->PushNumber(vec.x);
//		state->PushNumber(vec.y);
//		state->PushNumber(vec.z);
//		return 3;		
//	}
//	return 0;
//}
//int VGS_SetLightPosition(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0 )
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VECTOR3 vec;
//		vec.x = table[1].GetFloat();
//		vec.y = table[2].GetFloat();
//		vec.z = table[3].GetFloat();
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		info.Position = vec;
//		SetLightInfo(g_ulMainScene, hLight, &info);
//	}
//
//	return 0;
//}
//int VGS_GetLightDirection(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0  )
//			return 0;
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		VECTOR3 vec = info.Direction;
//		state->PushNumber(vec.x);
//		state->PushNumber(vec.y);
//		state->PushNumber(vec.z);
//		return 3;		
//	}
//	return 0;
//}
//int VGS_SetLightDirection(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long hLight = args[1].GetInteger();
//		if (hLight <= 0 )
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VECTOR3 vec;
//		vec.x = table[1].GetFloat();
//		vec.y = table[2].GetFloat();
//		vec.z = table[3].GetFloat();
//		LightInfo  info;
//		VGS_GetLightInfo(hLight, info);
//		info.Direction = vec;
//		SetLightInfo(g_ulMainScene, hLight, &info);
//	}
//	return 0;
//}
////----灯光----end
////----材质相关----start
////create/delete
//// int VGS_CreateMaterial(LuaState * state)
//// {
//// 	if (0 == g_ulMainScene)
//// 		return 0;
//// 
//// 	return 0;
//// }
//// int VGS_DeleteMaterialByHandle(LuaState * state)
//// {
//// 	if (0 == g_ulMainScene)
//// 		return 0;
//// 	return 0;
//// }
//// int VGS_DeleteMaterialByName(LuaState * state)
//// {
//// 	if (0 == g_ulMainScene)
//// 		return 0;
//// 	return 0;
//// }
////get mtrl
//int VGS_GetMaterialCount(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	int nCount = GetMaterialCount(g_ulMainScene);
//	state->PushInteger(nCount);
//	return 1;
//}
//int VGS_GetMaterialHandleByIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		int nIndex = args[1].GetInteger();
//		if (nIndex < 0)
//			return 0;
//		unsigned long hMtrl = GetMaterialHandleByIndex(g_ulMainScene , nIndex);
//		state->PushInteger(hMtrl);
//		return 1;
//	}
//	return 0;
//}
//int VGS_GetMaterialHandleByName(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsString())
//	{
//		std::string strName = args[1].GetString();
//		if ( strName.empty())
//			return 0;
//		unsigned long hMtrl = GetMaterialHandleByName(g_ulMainScene, (char *)strName.c_str());
//		state->PushInteger(hMtrl);
//		return 1;
//	}
//	return 0;
//}
//int VGS_GetMaterialNameByHandle(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		char name[MAX_NAMESTR_LENGTH];
//		ZeroMemory(name, MAX_NAMESTR_LENGTH);
//		GetMaterialNameByHandle(g_ulMainScene , hMtrl, name);
//		state->PushString(name);
//		return 1;
//	}
//	return 0;
//}
////mtrl info
//// int VGS_GetMaterialInfo(LuaState * state);
//// int VGS_SetMaterialInfo(LuaState * state);
//int VGS_GetMaterialAmbient(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		MtrlInfo info;
//		ZeroMemory(&info, sizeof(info));
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		VGSRGB color = info.Ambient;
//		state->PushInteger(color.red);
//		state->PushInteger(color.green);
//		state->PushInteger(color.blue);
//		return 3;		
//	}
//	return 0;
//}
//int VGS_SetMaterialAmbient(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0 )
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VGSRGB color;
//		color.red = table[1].GetInteger();
//		color.green = table[2].GetInteger();
//		color.blue = table[3].GetInteger();
//		MtrlInfo  info;
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		info.Ambient = color;
//		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
//	}
//	return 0;
//}
//int VGS_GetMaterialDiffuse(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		MtrlInfo info;
//		ZeroMemory(&info, sizeof(info));
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		VGSRGB color = info.Diffuse;
//		state->PushInteger(color.red);
//		state->PushInteger(color.green);
//		state->PushInteger(color.blue);
//		return 3;		
//	}
//	return 0;
//}
//int VGS_SetMaterialDiffuse(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0 )
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VGSRGB color;
//		color.red = table[1].GetInteger();
//		color.green = table[2].GetInteger();
//		color.blue = table[3].GetInteger();
//		MtrlInfo  info;
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		info.Diffuse = color;
//		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
//	}
//	return 0;
//}
//int VGS_GetMaterialSpecular(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		MtrlInfo info;
//		ZeroMemory(&info, sizeof(info));
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		VGSRGB color = info.Specular;
//		state->PushInteger(color.red);
//		state->PushInteger(color.green);
//		state->PushInteger(color.blue);
//		return 3;		
//	}
//	return 0;
//}
//int VGS_SetMaterialSpecular(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsTable())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0 )
//			return 0;
//		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
//		LuaObject table = args[2];
//		VGSRGB color;
//		color.red = table[1].GetInteger();
//		color.green = table[2].GetInteger();
//		color.blue = table[3].GetInteger();
//		MtrlInfo  info;
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		info.Specular = color;
//		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
//	}
//	return 0;
//}
//int VGS_GetMaterialSpecularLevel(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		MtrlInfo info;
//		ZeroMemory(&info, sizeof(info));
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		float fValue = info.SpecularLevel;
//		state->PushNumber(fValue);
//		return 1;		
//	}
//	return 0;
//}
//int VGS_SetMaterialSpecularLevel(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsNumber())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0 )
//			return 0;
//		float fValue = args[2].GetFloat();
//		MtrlInfo  info;
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		info.SpecularLevel = fValue;
//		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
//	}
//	return 0;
//}
//int VGS_GetMaterialShininess(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		MtrlInfo info;
//		ZeroMemory(&info, sizeof(info));
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		float fValue = info.shininess;
//		state->PushNumber(fValue);
//		return 1;		
//	}
//	return 0;
//}
//int VGS_SetMaterialShininess(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsNumber())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0 )
//			return 0;
//		float fValue = args[2].GetFloat();
//		MtrlInfo  info;
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		info.shininess = fValue;
//		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
//	}
//	return 0;
//}
//int VGS_GetMaterialEmissive(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		MtrlInfo info;
//		ZeroMemory(&info, sizeof(info));
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		float fValue = info.Emissive;
//		state->PushNumber(fValue);
//		return 1;		
//	}
//	return 0;
//}
//int VGS_SetMaterialEmissive(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsNumber())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0 )
//			return 0;
//		float fValue = args[2].GetFloat();
//		MtrlInfo  info;
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		info.Emissive = fValue;
//		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
//	}
//	return 0;
//}
//int VGS_GetMaterialOpacity(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		MtrlInfo info;
//		ZeroMemory(&info, sizeof(info));
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		int nValue = info.Opacity;
//		state->PushInteger(nValue);
//		return 1;		
//	}
//	return 0;
//}
//int VGS_SetMaterialOpacity(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0 )
//			return 0;
//		int nValue = args[2].GetInteger();
//		MtrlInfo  info;
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		info.Opacity = nValue;
//		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
//	}
//	return 0;
//}
//int VGS_GetMaterialTwoSide(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		MtrlInfo info;
//		ZeroMemory(&info, sizeof(info));
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		bool bValue = info.bTwoSide;
//		state->PushBoolean(bValue);
//		return 1;		
//	}
//	return 0;
//}
//int VGS_SetMaterialTwoSide(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsBoolean())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0 )
//			return 0;
//		bool bValue = args[2].GetBoolean();
//		MtrlInfo  info;
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		info.bTwoSide = bValue;
//		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
//	}
//	return 0;
//}
//int VGS_GetMaterialAlphaTest(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		MtrlInfo info;
//		ZeroMemory(&info, sizeof(info));
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		bool bValue = info.bAlphaTest;
//		state->PushBoolean(bValue);
//		return 1;		
//	}
//	return 0;
//}
//int VGS_SetMaterialAlphaTest(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsBoolean())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0 )
//			return 0;
//		bool bValue = args[2].GetBoolean();
//		MtrlInfo  info;
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		info.bAlphaTest = bValue;
//		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
//	}
//	return 0;
//}
//int VGS_GetMaterialFillMode(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		MtrlInfo info;
//		ZeroMemory(&info, sizeof(info));
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		int nValue = info.FillMode;
//		state->PushInteger(nValue);
//		return 1;		
//	}
//	return 0;
//}
//int VGS_SetMaterialFillMode(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0 )
//			return 0;
//		int nValue = args[2].GetInteger();
//		MtrlInfo  info;
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		info.FillMode = nValue;
//		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
//	}
//	return 0;
//}
//int VGS_GetMaterialAlphaTestValue(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0)
//			return 0;
//		MtrlInfo info;
//		ZeroMemory(&info, sizeof(info));
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		int nValue = info.AlphaTestValue;
//		state->PushInteger(nValue);
//		return 1;		
//	}
//	return 0;
//}
//int VGS_SetMaterialAlphaTestValue(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsInteger())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		if (hMtrl <= 0 )
//			return 0;
//		int nValue = args[2].GetInteger();
//		MtrlInfo  info;
//		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
//		info.AlphaTestValue = nValue;
//		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
//	}
//	return 0;
//}
//
////uv speed
//// int VGS_GetMaterialUVSpeed(LuaState * state);
//// int VGS_SetMaterialUVSpeed(LuaState * state);
////texture-normal
//int VGS_GetMaterialTextureCount(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	LuaStack args(state);
//	if (args[1].IsInteger() && args[2].IsString())
//	{
//		unsigned long hMtrl = args[1].GetInteger();
//		std::string strName = args[2].GetString();
//		if (hMtrl <= 0 || strName.empty())
//			return 0;
//		int nCount = GetMaterialTextureCount(g_ulMainScene, strName.c_str());
//		state->PushInteger(nCount);
//		return 1;		
//	}
//	return 0;
//}
//int VGS_GetMaterialTextureHandle(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//
//	return 0;
//}
//int VGS_ClearMaterialTexture(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_GetTextureType(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
////texture-blend info
//int VGS_GetTextureBlenderInfo(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetTextureBlenderInfo(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
////texture-mipmaps
//int VGS_GetTextureMipmaps(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetTextureMipmaps(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
////texture-uv index
//int VGS_GetTextureUVIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetTextureUVIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
////texture-basic
//int VGS_GetBasalTextureImage(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_AddMaterialBasalTexture(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_GetTextureAsReflect(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetTextureAsReflect(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_GetTextureAsOpacity(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetTextureAsOpacity(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetBasalTextureImage(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
////texture-cubic
//int VGS_GetCubicTextureImages(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_AddMaterialCubicTexture(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetCubicTextureImages(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
////texture-dynamic
//int VGS_GetDynamicTextureImageCount(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_GetDynamicTextureImageByIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_AddMaterialDynamicTexture(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetDynamicTextureImages(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetDynamicTextureImageByIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_AddDynamicTextureImage(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_DeleteDynamicTextureImageByIndex(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_GetDynamicDuration(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetDynamicTextureDuration(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
////texture-bump
//int VGS_AddMaterialBumpTexture(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_GetBumpTextureImage(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetBumpTextureImage(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_GetTextureAsBump(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
//int VGS_SetTextureAsBump(LuaState * state)
//{
//	if (0 == g_ulMainScene)
//		return 0;
//	return 0;
//}
////texture-辅助
//// int VGS_GetTextureImageCount(LuaState * state)
//// int VGS_GetTextureImageNamebyIndex(LuaState * state)
//// int VGS_IsImageInused(LuaState * state)
////----材质相关----end
// int VGS_SetMainScene(LuaState * state)
// {
// 	LuaStack args(state);
// 	if (args[1].IsInteger())
// 	{
// 		int idx = args[1].GetInteger();
// 		if (idx < 0)
// 			idx = 0;
// 		// 临时屏蔽掉 g_ulMainScene =  GetSceneHandleByIndex(idx);
// 	}
// 	return 0;
// }
//----场景操作相关----start
//render
int VGS_RenderAll(LuaState * state)
{
	// 临时屏蔽掉 RenderAll();
	return 0;
}
// int VGS_RenderScene(LuaState * state)
// {
// 	LuaStack args(state);
// 	if (args[1].IsInteger())
// 	{
// 		unsigned long ulScene =  args[1].GetInteger();
// 		if (0 != ulScene)
// 		{
// 			RenderScene(ulScene);
// 			return 0;
// 		}
// 	}
// 	return 0;
// }
// int VGS_RenderCurrentFrame(LuaState * state)
// {
// 	LuaStack args(state);
// 	if (args[1].IsInteger())
// 	{
// 		unsigned long ulScene =  args[1].GetInteger();
// 		if (0 != ulScene)
// 		{
// 			RenderCurrentFrame(ulScene);
// 			return 0;
// 		}
// 	}
// 	return 0;
// }
int VGS_GetViewportFillMode(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulViewport =  args[1].GetInteger();
		if (0 == ulViewport)
		{
			return 0;
		}
		// 临时屏蔽掉 int nFillMode = GetViewportFillMode(g_ulMainScene, ulViewport);
		// 临时屏蔽掉 state->PushInteger(nFillMode);
		return 1;
	}
	return 0;
}
int VGS_SetViewportFillMode(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long ulViewport =  args[1].GetInteger();
		int nFillMode = args[2].GetInteger();
		if (0 == ulViewport)
		{
			return 0;
		}
		// 临时屏蔽掉 SetViewportFillMode(g_ulMainScene, ulViewport, nFillMode);
		return 0;
	}
	return 0;
}
//viewport
int VGS_AddViewPort(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger()
		 && args[3].IsNumber()  && args[4].IsNumber()  && args[5].IsNumber()  && args[6].IsNumber() )
	{
		unsigned long ulCamera =  args[1].GetInteger();
		if (0 == ulCamera)
		{
			return 0;
		}
		int nLevel = args[2].GetInteger();
		float left = (float)args[3].GetNumber();
		float top = (float)args[4].GetNumber();
		float width = (float)args[5].GetNumber();
		float height = (float)args[6].GetNumber();
		// 临时屏蔽掉 unsigned long ulVp = AddViewport(g_ulMainScene, ulCamera, nLevel, left, top, width, height);
		// 临时屏蔽掉 state->PushInteger(ulVp);
		return 1;
	}
	return 0;
}
int VGS_SetViewportCamera(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long ulViewport =  args[1].GetInteger();
		unsigned long ulCamera =  args[2].GetInteger();
		if (0 == ulViewport || 0 == ulCamera)
		{
			return 0;
		}
		// 临时屏蔽掉 SetViewportCamera(ulViewport, ulCamera);
		return 0;
	}
	return 0;
}
int VGS_GetViewportCamera(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulViewport =  args[1].GetInteger();
		if (0 == ulViewport)
		{
			return 0;
		}
		// 临时屏蔽掉 unsigned long ulCamera =  GetViewportCamera(ulViewport);
		// 临时屏蔽掉 state->PushInteger(ulCamera);
		return 1;
	}
	return 0;
}
int VGS_GetViewPortRect(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulViewport =  args[1].GetInteger();
		if (0 == ulViewport)
		{
			return 0;
		}
		float left = 0.0f, top = 0.0f, width = 0.0f, height = 0.0f;
		// 临时屏蔽掉 GetViewportRect(ulViewport, left, top, width, height);
		state->PushNumber(left);
		state->PushNumber(top);
		state->PushNumber(width);
		state->PushNumber(height);
		return 4;
	}
	return 0;
}
int VGS_SetViewPortRect(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		//Rect采用table传递:{left, top, width, height}
		unsigned long ulViewport =  args[1].GetInteger();
		if (0 == ulViewport)
		{
			return 0;
		}
		LuaObject table = args[2];
		float left = (float)table[1].GetNumber();
		float top = (float)table[2].GetNumber();
		float width = (float)table[3].GetNumber();
		float height = (float)table[4].GetNumber();
		// 临时屏蔽掉 SetViewportRect(ulViewport, left, top, width, height);
		return 0;
	}
	return 0;
}
int VGS_UpdateViewportAspectRatio(LuaState * state)
{
	LuaStack args(state);
	if (args[1].IsInteger() )
	{
		unsigned long ulViewport =  args[1].GetInteger();
		if (0 == ulViewport)
		{
			return 0;
		}
		// 临时屏蔽掉 UpdateViewportAspectRatio(ulViewport);
		return 0;
	}
	return 0;
}
int VGS_IsViewportRenderGUI(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulViewport =  args[1].GetInteger();
		if (0 == ulViewport)
		{
			return 0;
		}
		// 临时屏蔽掉 bool bres = IsViewportRenderGUI(g_ulMainScene, ulViewport);
		// 临时屏蔽掉 state->PushBoolean(bres);
		return 1;
	}
	return 0;
}
int VGS_SetViewportRenderGUI(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger()&& args[2].IsBoolean())
	{
		unsigned long ulViewport =  args[1].GetInteger();
		if (0 == ulViewport)
		{
			return 0;
		}
		bool bFlag = args[2].GetBoolean();
		// 临时屏蔽掉 SetViewportRenderGUI(g_ulMainScene, ulViewport, bFlag);
		return 0;
	}
	return 0;
}
int VGS_GetSceneBackColor(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulViewport =  args[1].GetInteger();
		if (0 == ulViewport)
		{
			return 0;
		}
// 临时屏蔽掉 		unsigned long bgcolor = GetSceneBackColor(g_ulMainScene, ulViewport);
		// 临时屏蔽掉 state->PushInteger(bgcolor);
		return 1;
	}
	return 0;
}
int VGS_SetSceneBackColor(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long ulViewport =  args[1].GetInteger();
		if (0 == ulViewport)
		{
			return 0;
		}
		unsigned long bgcolor = args[2].GetInteger();
		// 临时屏蔽掉 SetSceneBackColor(g_ulMainScene, ulViewport, bgcolor);
		return 0;
	}
	return 0;
}
//----场景操作相关----end
int VGS_GetModelCount(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	// 临时屏蔽掉 int nCount = GetModelCount(g_ulMainScene);
	// 临时屏蔽掉 state->PushInteger(nCount);
	return 1;
}
int VGS_GetModelHandleByIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		int nIndex = args[1].GetInteger();
		// 临时屏蔽掉 unsigned long ulModel = GetModelHandleByIndex(g_ulMainScene, nIndex);
		// 临时屏蔽掉 state->PushInteger(ulModel);
		return 1;
	}
	return 0;
}
int VGS_GetModelNameByIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		int nIndex = args[1].GetInteger();
		char buf[MAX_NAMESTR_LENGTH];
		ZeroMemory(buf, MAX_NAMESTR_LENGTH);
		// 临时屏蔽掉 bool bFlag = GetModelNameByIndex(g_ulMainScene, nIndex, buf);
		// 临时屏蔽掉 if (!bFlag)
		// 临时屏蔽掉 	return 0;
		state->PushString(buf);
		return 1;
	}
	return 0;
}
int VGS_GetModelNameByHandle(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		char buf[MAX_NAMESTR_LENGTH];
		ZeroMemory(buf, MAX_NAMESTR_LENGTH);
		// 临时屏蔽掉 bool bFlag = GetModelNameByHandle(g_ulMainScene, ulModel, buf);
		// 临时屏蔽掉 if (!bFlag)
		// 临时屏蔽掉 	return 0;
		state->PushString(buf);
		return 1;
	}
	return 0;
}
int VGS_GetModelHandleByName(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsString())
	{
		std::string strName = args[1].GetString();
		if (strName.empty())
		{
			return 0;
		}
		// 临时屏蔽掉 unsigned long ulModel = GetModelHandleByName(g_ulMainScene, (char *)strName.c_str());
		// 临时屏蔽掉 state->PushInteger(ulModel);
		return 1;
	}
	return 0;
}
//delete
int VGS_DeleteModel(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsString())
	{
		std::string strName = args[1].GetString();
		if (strName.empty())
		{
			return 0;
		}
		// 临时屏蔽掉 bool bFlag = DeleteModel(g_ulMainScene, (char *)strName.c_str());
		// 临时屏蔽掉 state->PushBoolean(bFlag);
		return 1;
	}
	return 0;
}
//transform
int VGS_GetModelPosition(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		VECTOR3 pos;
		// 临时屏蔽掉 GetModelPosition(g_ulMainScene, ulModel, &pos);
		state->PushNumber(pos.x);
		state->PushNumber(pos.y);
		state->PushNumber(pos.z);
		return 3;
	}
	return 0;
}
int VGS_SetModelPosition(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VECTOR3 pos;
		pos.x = (float)table[1].GetNumber();
		pos.y = (float)table[2].GetNumber();
		pos.z = (float)table[3].GetNumber();

// 临时屏蔽掉 		SetModelPosition(g_ulMainScene, ulModel, &pos);

		return 0;
	}
	return 0;
}
int VGS_MoveModel(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable() && args[3].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VECTOR3 pos;
		pos.x = (float)table[1].GetNumber();
		pos.y = (float)table[2].GetNumber();
		pos.z = (float)table[3].GetNumber();
		
		unsigned int nFlag = args[3].GetInteger();

// 临时屏蔽掉 		MoveModel(g_ulMainScene, ulModel, &pos, nFlag);

		return 0;
	}
	return 0;
}
int VGS_GetModelRotation(LuaState * state)
{
	/*if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		VECTOR3 pos;
		if (GetModelRotation(g_ulMainScene, ulModel, &pos))
		{
			state->PushNumber(pos.x);
			state->PushNumber(pos.y);
			state->PushNumber(pos.z);
			return 3;
		}
	}*/
	return 0;
}
int VGS_SetModelRotation(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		LuaObject table = args[2];
		VECTOR3 v3;
		v3.x = table[1].GetNumber();
		v3.y = table[2].GetNumber();
		v3.z = table[3].GetNumber();
// 临时屏蔽掉 		SetModelRotation(g_ulMainScene, ulModel, &v3);
	}
	return 0;
}
int VGS_RotateModel(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable() && args[3].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		LuaObject table = args[2];
		VECTOR3 v3;
		v3.x = table[1].GetNumber();
		v3.y = table[2].GetNumber();
		v3.z = table[3].GetNumber();

		VGS_TRANSFORMSPACE ts = (VGS_TRANSFORMSPACE)(args[3].GetInteger());

// 临时屏蔽掉 		RotateModel(g_ulMainScene, ulModel, &v3, ts);
	}
	return 0;
}
int VGS_GetModelScale(LuaState * state)
{
	/*if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		VECTOR3 pos;
		if (GetModelScale(g_ulMainScene, ulModel, &pos))
		{
			state->PushNumber(pos.x);
			state->PushNumber(pos.y);
			state->PushNumber(pos.z);
			return 3;
		}
	}*/
	return 0;
}
int VGS_SetModelScale(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		LuaObject table = args[2];
		VECTOR3 v3;
		v3.x = table[1].GetNumber();
		v3.y = table[2].GetNumber();
		v3.z = table[3].GetNumber();
// 临时屏蔽掉 		SetModelScale(g_ulMainScene, ulModel, &v3);
	}
	return 0;
}
int VGS_ScaleModel(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		LuaObject table = args[2];
		VECTOR3 v3;
		v3.x = table[1].GetNumber();
		v3.y = table[2].GetNumber();
		v3.z = table[3].GetNumber();
// 临时屏蔽掉 		ScaleModel(g_ulMainScene, ulModel, &v3);
	}
	return 0;
}
//property
// int VGS_GetModelInfo(LuaState * state)
// {
// 	if (0 == g_ulMainScene)
// 		return 0;
// 	LuaStack args(state);
// 	if (args[1].IsInteger())
// 	{
// 		unsigned long ulModel = args[1].GetInteger();
// 		if (0 == ulModel)
// 		{
// 			return 0;
// 		}
// 		VGS_ModelInfo info;
// 		if (FALSE == GetModelInfo(g_ulMainScene, ulModel, info))
// 			return 0;
// 		state->PushBoolean(info.isBillboard);
// 		state->PushBoolean(info.isCollision);
// 		state->PushBoolean(info.isGround);
// 		state->PushBoolean(info.isVisible);
// 		state->PushBoolean(info.isClickable);
// 		state->PushInteger((int)(info.dirctionAxis));
// 		return 6;
// 	}
// 	return 0;
// }
// int VGS_SetModelInfo(LuaState * state)
// {
// 
// 	return 0;
// }
int VGS_GetModelBillboardStatus(LuaState * state)
{
	//if (0 == g_ulMainScene)
	//	return 0;
	//LuaStack args(state);
	//if (args[1].IsInteger())
	//{
	//	unsigned long ulModel = args[1].GetInteger();
	//	if (0 == ulModel)
	//	{
	//		return 0;
	//	}
	//	VGS_AXIS_TYPE axis;
	//	if (FALSE == GetModelBillboardStatus(g_ulMainScene, ulModel, axis))
	//		state->PushBoolean(false);
	//	else
	//		state->PushBoolean(true);
	//	return 1;
	//}

	return 0;
}
int VGS_SetModelBillboardStatus(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsBoolean() && args[3].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		VGS_AXIS_TYPE axis = (VGS_AXIS_TYPE)args[3].GetInteger();
// 临时屏蔽掉 		SetModelBillboardStatus(g_ulMainScene, ulModel, args[2].GetBoolean(), axis);
	}

	return 0;
}
int VGS_GetModelCollisionStatus(LuaState * state)
{
	/*if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		if (FALSE == GetModelCollisionStatus(g_ulMainScene, ulModel))
			state->PushBoolean(false);
		else
			state->PushBoolean(true);
		return 1;
	}*/

	return 0;
}
int VGS_SetModelCollisionStatus(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsBoolean())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
// 临时屏蔽掉 		SetModelCollisionStatus(g_ulMainScene, ulModel, args[2].GetBoolean());
	}

	return 0;
}
int VGS_GetModelAsGround(LuaState * state)
{
	//if (0 == g_ulMainScene)
	//	return 0;
	//LuaStack args(state);
	//if (args[1].IsInteger())
	//{
	//	unsigned long ulModel = args[1].GetInteger();
	//	if (0 == ulModel)
	//	{
	//		return 0;
	//	}
	//	if (FALSE == GetModelAsGround(g_ulMainScene, ulModel))
	//		state->PushBoolean(false);
	//	else
	//		state->PushBoolean(true);
	//	return 1;
	//}

	return 0;
}
int VGS_SetModelAsGround(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsBoolean())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
// 临时屏蔽掉 		SetModelAsGround(g_ulMainScene, ulModel, args[2].GetBoolean());
	}

	return 0;
}
int VGS_GetModelVisibility(LuaState * state)
{
	/*if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		if (FALSE == GetModelVisibility(g_ulMainScene, ulModel))
			state->PushBoolean(false);
		else
			state->PushBoolean(true);
		return 1;
	}*/

	return 0;
}
int VGS_SetModelVisibility(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsBoolean())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
// 临时屏蔽掉 		SetModelVisibility(g_ulMainScene, ulModel, args[2].GetBoolean());
	}

	return 0;
}
int VGS_GetModelClickabel(LuaState * state)
{
	/*if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		if (FALSE == GetModelClickabel(g_ulMainScene, ulModel))
			state->PushBoolean(false);
		else
			state->PushBoolean(true);
		return 1;
	}*/

	return 0;
}
int VGS_SetModelClickable(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsBoolean())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
// 临时屏蔽掉 		SetModelClickable(g_ulMainScene, ulModel, args[2].GetBoolean());
	}

	return 0;
}

int VGS_IsModelHasKeyframeAnimation(LuaState * state)
{
	/*if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
		if (FALSE == IsModelHasKeyframeAnimation(g_ulMainScene, ulModel))
			state->PushBoolean(false);
		else
			state->PushBoolean(true);
		return 1;
	}*/
	return 0;
}
int VGS_IsModelHasSkeletonAnimation(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel)
		{
			return 0;
		}
// 临时屏蔽掉 		if (FALSE == IsModelHasSkeletonAnimation(g_ulMainScene, ulModel))
		// 临时屏蔽掉 	state->PushBoolean(false);
	// 临时屏蔽掉 	else
	// 临时屏蔽掉 		state->PushBoolean(true);
		return 1;
	}
	return 0;
}
int VGS_SetModelMaterial(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsString())
	{
		unsigned long ulModel = args[1].GetInteger();
		std::string strMtrlName = args[2].GetString();
		if (0 == ulModel || strMtrlName.empty())
		{
			return 0;
		}
// 临时屏蔽掉 		SetModelMaterial(g_ulMainScene, ulModel, strMtrlName.c_str());
	}
	return 0;
}
int VGS_GetSubModelCount(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		if (0 == ulModel )
		{
			return 0;
		}
// 临时屏蔽掉 		int nCount = GetSubModelCount(g_ulMainScene, ulModel);
	// 临时屏蔽掉 	state->PushInteger(nCount);
		return 1;
	}

	return 0;
}
int VGS_GetSubModelMaterialName(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		int nIndex = args[2].GetInteger();
		if (0 == ulModel || nIndex < 0)
		{
			return 0;
		}
		char name[MAX_NAMESTR_LENGTH];
		ZeroMemory(name, MAX_NAMESTR_LENGTH);
// 临时屏蔽掉 		GetSubModelMaterialName(ulModel,nIndex, name);
		state->PushString(name);
		return 1;
	}
	return 0;
}
int VGS_GetSubModelMaterialHandle(LuaState * state)
{
	/*if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long ulModel = args[1].GetInteger();
		int nIndex = args[2].GetInteger();
		if (0 == ulModel || nIndex < 0)
		{
			return 0;
		}
		unsigned long ulMtrl = GetSubModelMaterialHandle(ulModel,nIndex);
		state->PushInteger(ulMtrl);
		return 1;
	}*/
	return 0;
}
int VGS_SetSubModelMaterial(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger() && args[3].IsString())
	{
		unsigned long ulModel = args[1].GetInteger();
		int nIndex = args[2].GetInteger();
		std::string strMtrl = args[3].GetString();
		if (0 == ulModel || nIndex < 0 || strMtrl.empty())
		{
			return 0;
		}
// 临时屏蔽掉 		SetSubModelMaterial(g_ulMainScene, ulModel,nIndex, strMtrl.c_str());
	}
	return 0;
}
//----模型操作----end
//----camera----start
int VGS_CreateCamera(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsString())
	{
		std::string strCamera = args[1].GetString();
		if (strCamera.empty())
		{
			return 0;
		}
// 临时屏蔽掉 		unsigned long hCamera = CreateCamera(g_ulMainScene, (CHAR *)strCamera.c_str());
	// 临时屏蔽掉 	state->PushInteger(hCamera);
		return 1;
	}
	return 0;
}
int VGS_DeleteCamerabyHandle(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hCamera = args[1].GetInteger();
		if (hCamera <= 0)
		{
			return 0;
		}
// 临时屏蔽掉 		DeleteCamerabyHandle(g_ulMainScene, hCamera);
	}
	return 0;
}
int VGS_DeleteCamerabyName(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsString())
	{
		std::string strName = args[1].GetString();
		if (strName.empty())
			return 0;
// 临时屏蔽掉 		DeleteCamerabyName(g_ulMainScene, (CHAR *)strName.c_str());
	}
	return 0;
}
//get/set
int VGS_GetCameraCount(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
// 临时屏蔽掉 	int nCount = GetCameraCount (g_ulMainScene);
// 临时屏蔽掉 	state->PushInteger(nCount);
	return 1;
}
int VGS_GetCameraHandleByIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hCamera = 0l;
		int nIndex = args[1].GetInteger();
		if (nIndex >= 0)
		{
// 临时屏蔽掉 			hCamera = GetCameraHandleByIndex(g_ulMainScene, nIndex);
		}
		state->PushInteger(hCamera);
		return 1;
	}
	return 0;
}
int VGS_GetCameraHandleByName(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsString())
	{
		unsigned long hCamera = 0l;
		std::string strName = args[1].GetString();
		if (!strName.empty())
		{
// 临时屏蔽掉 			hCamera = GetCameraHandleByName(g_ulMainScene, (CHAR *)strName.c_str());
		}
		state->PushInteger(hCamera);
		return 1;
	}
	return 0;
}
int VGS_GetCameraNameByHandle(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hCamera = args[1].GetInteger();
		std::string strName = args[1].GetString();
		if (!strName.empty())
		{
// 临时屏蔽掉 			hCamera = GetCameraHandleByName(g_ulMainScene, (CHAR *)strName.c_str());
		}
		state->PushInteger(hCamera);
		return 1;
	}
	return 0;
}
int VGS_GetCameraNameByIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		int nIndex = args[1].GetInteger();
		if (nIndex >= 0)
		{
			char name[MAX_NAMESTR_LENGTH];
			ZeroMemory(name, MAX_NAMESTR_LENGTH);
// 临时屏蔽掉 			GetCameraNameByIndex(g_ulMainScene, nIndex, name);
			state->PushString(name);
			return 1;
		}
	}
	return 0;
}
//transform
int VGS_MoveCamera(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable() && args[3].IsInteger())
	{
		unsigned long hCamera = args[1].GetInteger();
		if (0 == hCamera)
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VECTOR3 pos;
		pos.x = (float)table[1].GetNumber();
		pos.y = (float)table[2].GetNumber();
		pos.z = (float)table[3].GetNumber();
		unsigned int coordEnum = args[3].GetInteger();

// 临时屏蔽掉 		MoveCamera(g_ulMainScene, hCamera, &pos, coordEnum);
	}
	return 0;
}
int VGS_RotateCamera(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable() && args[3].IsInteger())
	{
		unsigned long hCamera = args[1].GetInteger();
		if (0 == hCamera)
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VECTOR3 pos;
		pos.x = (float)table[1].GetNumber();
		pos.y = (float)table[2].GetNumber();
		pos.z = (float)table[3].GetNumber();
		unsigned int coordEnum = args[3].GetInteger();

// 临时屏蔽掉 		RotateCamera(g_ulMainScene, hCamera, &pos, coordEnum);
	}
	return 0;
}
int VGS_RoundCamera(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable() && args[3].IsInteger() && args[4].IsNumber())
	{
		unsigned long hCamera = args[1].GetInteger();
		if (0 == hCamera)
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VECTOR3 pos;
		pos.x = (float)table[1].GetNumber();
		pos.y = (float)table[2].GetNumber();
		pos.z = (float)table[3].GetNumber();
		int axisType = args[3].GetInteger();
		float fAngle = args[4].GetNumber();

// 临时屏蔽掉 		RoundCamera(g_ulMainScene, hCamera, &pos, axisType, fAngle);
	}
	return 0;
}
//property
int VGS_GetCameraDirection(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hCamera = args[1].GetInteger();
		if (0 == hCamera)
			return 0;
		VECTOR3 dir;
		ZeroMemory(&dir, sizeof(dir));
// 临时屏蔽掉 		GetCameraDirection(g_ulMainScene, hCamera, dir);
		state->PushNumber(dir.x);
		state->PushNumber(dir.y);
		state->PushNumber(dir.z);
		return 3;
	}
	return 0;
}
int VGS_SetCameraDirection(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long hCamera = args[1].GetInteger();
		if (0 == hCamera)
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VECTOR3 pos;
		pos.x = (float)table[1].GetNumber();
		pos.y = (float)table[2].GetNumber();
		pos.z = (float)table[3].GetNumber();

// 临时屏蔽掉 		SetCameraDirection(g_ulMainScene, hCamera, pos);
	}
	return 0;
}
int VGS_SetCameraLookAtModel(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long hCamera = args[1].GetInteger();
		unsigned long hModel = args[2].GetInteger();
		if (0 == hCamera || 0 == hModel)
			return 0;
// 临时屏蔽掉 		SetCameraLookAtModel(g_ulMainScene, hCamera, hModel);
	}
	return 0;
}
int VGS_SetCameraFov(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsNumber())
	{
		unsigned long hCamera = args[1].GetInteger();
		if (0 == hCamera)
			return 0;
		float fov = args[2].GetNumber();
// 临时屏蔽掉 		SetCameraFov(g_ulMainScene, hCamera, fov);
	}
	return 0;
}
int VGS_SetCameraNearClip(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsNumber())
	{
		unsigned long hCamera = args[1].GetInteger();
		if (0 == hCamera)
			return 0;
		float nearClip = args[2].GetNumber();
// 临时屏蔽掉 		SetCameraNearClip(g_ulMainScene, hCamera, nearClip);
	}
	return 0;
}
int VGS_SetCameraFarClip(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsNumber())
	{
		unsigned long hCamera = args[1].GetInteger();
		if (0 == hCamera)
			return 0;
		float farClip = args[2].GetNumber();
// 临时屏蔽掉 		SetCameraFarClip(g_ulMainScene, hCamera, farClip);
	}
	return 0;
}
int VGS_SetCameraPosition(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long hCamera = args[1].GetInteger();
		if (0 == hCamera)
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VECTOR3 pos;
		pos.x = (float)table[1].GetNumber();
		pos.y = (float)table[2].GetNumber();
		pos.z = (float)table[3].GetNumber();

// 临时屏蔽掉 		SetCameraPosition(g_ulMainScene, hCamera, &pos);
	}
	return 0;
}
int VGS_SetBillboardCamera(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hCamera = args[1].GetInteger();
		if (0 == hCamera)
			return 0;
// 临时屏蔽掉 		SetBillboardCamera(g_ulMainScene, hCamera);
	}
	return 0;
}
int VGS_GetBillboardCamera(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	unsigned long hCamera = 0L;
// 临时屏蔽掉 	hCamera = GetBillboardCamera(g_ulMainScene);
	state->PushInteger(hCamera);
	return 1;
}
//----camera----end
//----灯光----start
int VGS_DeleteLight(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hLight = args[1].GetInteger();
		if (0 == hLight)
			return 0;
// 临时屏蔽掉 		DeleteLight(g_ulMainScene, hLight);
	}
	return 0;
}

int VGS_GetLightCount(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
// 临时屏蔽掉 	int nCount = GetLightCount(g_ulMainScene);
// 临时屏蔽掉 	state->PushInteger(nCount);
	return 1;
}

int VGS_GetLightHandleByIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		int nIndex = args[1].GetInteger();
		if (nIndex < 0)
			return 0;
// 临时屏蔽掉 		unsigned long hLight = GetLightHandleByIndex(g_ulMainScene , nIndex);
// 临时屏蔽掉 		state->PushInteger(hLight);
		return 1;
	}
	return 0;
}

int VGS_GetLightNameByIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		int nIndex = args[1].GetInteger();
		if (nIndex < 0)
			return 0;
		char name[MAX_NAMESTR_LENGTH];
		ZeroMemory(name, MAX_NAMESTR_LENGTH);
// 临时屏蔽掉 		GetLightNameByIndex(g_ulMainScene , nIndex, name);
		state->PushString(name);
		return 1;
	}
	return 0;
}

int VGS_GetLightNameByHandle(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0)
			return 0;
		char name[MAX_NAMESTR_LENGTH];
		ZeroMemory(name, MAX_NAMESTR_LENGTH);
// 临时屏蔽掉 		GetLightNameByHandle(g_ulMainScene , hLight, name);
		state->PushString(name);
		return 1;
	}
	return 0;
}

int VGS_GetLightHandleByName(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsString())
	{
		std::string strName = args[1].GetString();
		if ( strName.empty())
			return 0;
// 临时屏蔽掉 		unsigned long hLight = GetLightHandleByName(g_ulMainScene, (char *)strName.c_str());
	// 临时屏蔽掉 	state->PushInteger(hLight);
		return 1;
	}

	return 0;
}

int VGS_MoveLight(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0)
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VECTOR3 vec;
		vec.x = (float)table[1].GetNumber();
		vec.y = (float)table[2].GetNumber();
		vec.z = (float)table[3].GetNumber();

// 临时屏蔽掉 		MoveLight(g_ulMainScene, hLight, &vec);
	}

	return 0;
}
void VGS_GetLightInfo(unsigned long hLight, LightInfo & info)
{
	ZeroMemory(&info, sizeof(info));
	if (hLight <= 0)
		return;
// 临时屏蔽掉 	GetLightInfo(g_ulMainScene, hLight, &info);
}
int VGS_GetLightType(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hLight = args[1].GetInteger();
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		state->PushInteger(info.Type);
		return 1;
	}
	return 0;
}
int VGS_SetLightType(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long hLight = args[1].GetInteger();
		VGS_LIGHT_TYPE nType = (VGS_LIGHT_TYPE)(args[2].GetInteger());
		if (hLight <= 0 || nType < 0 )
			return 0;
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		info.Type = nType;
// 临时屏蔽掉 		SetLightInfo(g_ulMainScene, hLight, &info);
	}
	return 0;
}
int VGS_GetLightDiffuse(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0  )
			return 0;
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		VGSRGB color = info.Diffuse;
		state->PushInteger(color.red);
		state->PushInteger(color.green);
		state->PushInteger(color.blue);
		return 3;		
	}
	return 0;
}
int VGS_SetLightDiffuse(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0 )
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VGSRGB color;
		color.red = table[1].GetInteger();
		color.green = table[2].GetInteger();
		color.blue = table[3].GetInteger();
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		info.Diffuse = color;
// 临时屏蔽掉 		SetLightInfo(g_ulMainScene, hLight, &info);
	}
	return 0;
}
int VGS_GetLightAmbient(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0  )
			return 0;
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		VGSRGB color = info.Ambient;
		state->PushInteger(color.red);
		state->PushInteger(color.green);
		state->PushInteger(color.blue);
		return 3;		
	}

	return 0;
}
int VGS_SetLightAmbient(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0 )
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VGSRGB color;
		color.red = table[1].GetInteger();
		color.green = table[2].GetInteger();
		color.blue = table[3].GetInteger();
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		info.Ambient = color;
// 临时屏蔽掉 		SetLightInfo(g_ulMainScene, hLight, &info);
	}
	return 0;
}
int VGS_GetLightMultiply(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0  )
			return 0;
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		float fMultiply = info.Multiply;
		state->PushNumber(fMultiply);
		return 1;		
	}

	return 0;
}
int VGS_SetLightMultiply(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsNumber())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0 )
			return 0;
		float fMultiply = args[2].GetFloat();
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		info.Multiply = fMultiply;
// 临时屏蔽掉 		SetLightInfo(g_ulMainScene, hLight, &info);
	}
	return 0;
}
int VGS_GetLightPosition(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0  )
			return 0;
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		VECTOR3 vec = info.Position;
		state->PushNumber(vec.x);
		state->PushNumber(vec.y);
		state->PushNumber(vec.z);
		return 3;		
	}
	return 0;
}
int VGS_SetLightPosition(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0 )
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VECTOR3 vec;
		vec.x = table[1].GetFloat();
		vec.y = table[2].GetFloat();
		vec.z = table[3].GetFloat();
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		info.Position = vec;
// 临时屏蔽掉 		SetLightInfo(g_ulMainScene, hLight, &info);
	}

	return 0;
}
int VGS_GetLightDirection(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0  )
			return 0;
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		VECTOR3 vec = info.Direction;
		state->PushNumber(vec.x);
		state->PushNumber(vec.y);
		state->PushNumber(vec.z);
		return 3;		
	}
	return 0;
}
int VGS_SetLightDirection(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long hLight = args[1].GetInteger();
		if (hLight <= 0 )
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VECTOR3 vec;
		vec.x = table[1].GetFloat();
		vec.y = table[2].GetFloat();
		vec.z = table[3].GetFloat();
		LightInfo  info;
		VGS_GetLightInfo(hLight, info);
		info.Direction = vec;
// 临时屏蔽掉 		SetLightInfo(g_ulMainScene, hLight, &info);
	}
	return 0;
}
//----灯光----end
//----材质相关----start
//create/delete
// int VGS_CreateMaterial(LuaState * state)
// {
// 	if (0 == g_ulMainScene)
// 		return 0;
// 
// 	return 0;
// }
// int VGS_DeleteMaterialByHandle(LuaState * state)
// {
// 	if (0 == g_ulMainScene)
// 		return 0;
// 	return 0;
// }
// int VGS_DeleteMaterialByName(LuaState * state)
// {
// 	if (0 == g_ulMainScene)
// 		return 0;
// 	return 0;
// }
//get mtrl
int VGS_GetMaterialCount(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
// 临时屏蔽掉 	int nCount = GetMaterialCount(g_ulMainScene);
// 临时屏蔽掉 	state->PushInteger(nCount);
	return 1;
}
int VGS_GetMaterialHandleByIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		int nIndex = args[1].GetInteger();
		if (nIndex < 0)
			return 0;
// 临时屏蔽掉 		unsigned long hMtrl = GetMaterialHandleByIndex(g_ulMainScene , nIndex);
		// 临时屏蔽掉 state->PushInteger(hMtrl);
		return 1;
	}
	return 0;
}
int VGS_GetMaterialHandleByName(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsString())
	{
		std::string strName = args[1].GetString();
		if ( strName.empty())
			return 0;
// 临时屏蔽掉 		unsigned long hMtrl = GetMaterialHandleByName(g_ulMainScene, (char *)strName.c_str());
	// 临时屏蔽掉 	state->PushInteger(hMtrl);
		return 1;
	}
	return 0;
}
int VGS_GetMaterialNameByHandle(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		char name[MAX_NAMESTR_LENGTH];
		ZeroMemory(name, MAX_NAMESTR_LENGTH);
// 临时屏蔽掉 		GetMaterialNameByHandle(g_ulMainScene , hMtrl, name);
		state->PushString(name);
		return 1;
	}
	return 0;
}
//mtrl info
// int VGS_GetMaterialInfo(LuaState * state);
// int VGS_SetMaterialInfo(LuaState * state);
int VGS_GetMaterialAmbient(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		MtrlInfo info;
		ZeroMemory(&info, sizeof(info));
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		VGSRGB color = info.Ambient;
		state->PushInteger(color.red);
		state->PushInteger(color.green);
		state->PushInteger(color.blue);
		return 3;		
	}
	return 0;
}
int VGS_SetMaterialAmbient(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0 )
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VGSRGB color;
		color.red = table[1].GetInteger();
		color.green = table[2].GetInteger();
		color.blue = table[3].GetInteger();
		MtrlInfo  info;
		// 临时屏蔽掉 GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		info.Ambient = color;
// 临时屏蔽掉 		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
	}
	return 0;
}
int VGS_GetMaterialDiffuse(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		MtrlInfo info;
		ZeroMemory(&info, sizeof(info));
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		VGSRGB color = info.Diffuse;
		state->PushInteger(color.red);
		state->PushInteger(color.green);
		state->PushInteger(color.blue);
		return 3;		
	}
	return 0;
}
int VGS_SetMaterialDiffuse(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0 )
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VGSRGB color;
		color.red = table[1].GetInteger();
		color.green = table[2].GetInteger();
		color.blue = table[3].GetInteger();
		MtrlInfo  info;
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		info.Diffuse = color;
// 临时屏蔽掉 		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
	}
	return 0;
}
int VGS_GetMaterialSpecular(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		MtrlInfo info;
		ZeroMemory(&info, sizeof(info));
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		VGSRGB color = info.Specular;
		state->PushInteger(color.red);
		state->PushInteger(color.green);
		state->PushInteger(color.blue);
		return 3;		
	}
	return 0;
}
int VGS_SetMaterialSpecular(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsTable())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0 )
			return 0;
		//说明：传入的参数必须是：{1.1,2.2,3.3}，否则无法解析
		LuaObject table = args[2];
		VGSRGB color;
		color.red = table[1].GetInteger();
		color.green = table[2].GetInteger();
		color.blue = table[3].GetInteger();
		MtrlInfo  info;
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		info.Specular = color;
		// 临时屏蔽掉 SetMaterialInfo(g_ulMainScene, hMtrl, &info);
	}
	return 0;
}
int VGS_GetMaterialSpecularLevel(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		MtrlInfo info;
		ZeroMemory(&info, sizeof(info));
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		float fValue = info.SpecularLevel;
		state->PushNumber(fValue);
		return 1;		
	}
	return 0;
}
int VGS_SetMaterialSpecularLevel(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsNumber())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0 )
			return 0;
		float fValue = args[2].GetFloat();
		MtrlInfo  info;
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		info.SpecularLevel = fValue;
		// 临时屏蔽掉 SetMaterialInfo(g_ulMainScene, hMtrl, &info);
	}
	return 0;
}
int VGS_GetMaterialShininess(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		MtrlInfo info;
		ZeroMemory(&info, sizeof(info));
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		float fValue = info.shininess;
		state->PushNumber(fValue);
		return 1;		
	}
	return 0;
}
int VGS_SetMaterialShininess(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsNumber())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0 )
			return 0;
		float fValue = args[2].GetFloat();
		MtrlInfo  info;
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		info.shininess = fValue;
	// 临时屏蔽掉 	SetMaterialInfo(g_ulMainScene, hMtrl, &info);
	}
	return 0;
}
int VGS_GetMaterialEmissive(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		MtrlInfo info;
		ZeroMemory(&info, sizeof(info));
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		float fValue = info.Emissive;
		state->PushNumber(fValue);
		return 1;		
	}
	return 0;
}
int VGS_SetMaterialEmissive(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsNumber())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0 )
			return 0;
		float fValue = args[2].GetFloat();
		MtrlInfo  info;
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		info.Emissive = fValue;
		// 临时屏蔽掉 SetMaterialInfo(g_ulMainScene, hMtrl, &info);
	}
	return 0;
}
int VGS_GetMaterialOpacity(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		MtrlInfo info;
		ZeroMemory(&info, sizeof(info));
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		int nValue = info.Opacity;
		state->PushInteger(nValue);
		return 1;		
	}
	return 0;
}
int VGS_SetMaterialOpacity(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0 )
			return 0;
		int nValue = args[2].GetInteger();
		MtrlInfo  info;
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		info.Opacity = nValue;
		// 临时屏蔽掉 SetMaterialInfo(g_ulMainScene, hMtrl, &info);
	}
	return 0;
}
int VGS_GetMaterialTwoSide(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		MtrlInfo info;
		ZeroMemory(&info, sizeof(info));
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		bool bValue = info.bTwoSide;
		state->PushBoolean(bValue);
		return 1;		
	}
	return 0;
}
int VGS_SetMaterialTwoSide(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsBoolean())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0 )
			return 0;
		bool bValue = args[2].GetBoolean();
		MtrlInfo  info;
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		info.bTwoSide = bValue;
		// 临时屏蔽掉 SetMaterialInfo(g_ulMainScene, hMtrl, &info);
	}
	return 0;
}
int VGS_GetMaterialAlphaTest(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		MtrlInfo info;
		ZeroMemory(&info, sizeof(info));
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		bool bValue = info.bAlphaTest;
		state->PushBoolean(bValue);
		return 1;		
	}
	return 0;
}
int VGS_SetMaterialAlphaTest(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsBoolean())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0 )
			return 0;
		bool bValue = args[2].GetBoolean();
		MtrlInfo  info;
		// 临时屏蔽掉 GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		info.bAlphaTest = bValue;
// 临时屏蔽掉 		SetMaterialInfo(g_ulMainScene, hMtrl, &info);
	}
	return 0;
}
int VGS_GetMaterialFillMode(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		MtrlInfo info;
		ZeroMemory(&info, sizeof(info));
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		int nValue = info.FillMode;
		state->PushInteger(nValue);
		return 1;		
	}
	return 0;
}
int VGS_SetMaterialFillMode(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0 )
			return 0;
		int nValue = args[2].GetInteger();
		MtrlInfo  info;
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		info.FillMode = nValue;
		// 临时屏蔽掉 SetMaterialInfo(g_ulMainScene, hMtrl, &info);
	}
	return 0;
}
int VGS_GetMaterialAlphaTestValue(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0)
			return 0;
		MtrlInfo info;
		ZeroMemory(&info, sizeof(info));
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		int nValue = info.AlphaTestValue;
		state->PushInteger(nValue);
		return 1;		
	}
	return 0;
}
int VGS_SetMaterialAlphaTestValue(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsInteger())
	{
		unsigned long hMtrl = args[1].GetInteger();
		if (hMtrl <= 0 )
			return 0;
		int nValue = args[2].GetInteger();
		MtrlInfo  info;
// 临时屏蔽掉 		GetMaterialInfo(g_ulMainScene, hMtrl, &info);
		info.AlphaTestValue = nValue;
		// 临时屏蔽掉 SetMaterialInfo(g_ulMainScene, hMtrl, &info);
	}
	return 0;
}

//uv speed
// int VGS_GetMaterialUVSpeed(LuaState * state);
// int VGS_SetMaterialUVSpeed(LuaState * state);
//texture-normal
int VGS_GetMaterialTextureCount(LuaState * state)
{
	/*if (0 == g_ulMainScene)
		return 0;
	LuaStack args(state);
	if (args[1].IsInteger() && args[2].IsString())
	{
		unsigned long hMtrl = args[1].GetInteger();
		std::string strName = args[2].GetString();
		if (hMtrl <= 0 || strName.empty())
			return 0;
		int nCount = GetMaterialTextureCount(g_ulMainScene, strName.c_str());
		state->PushInteger(nCount);
		return 1;		
	}*/
	return 0;
}
int VGS_GetMaterialTextureHandle(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;

	return 0;
}
int VGS_ClearMaterialTexture(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_GetTextureType(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
//texture-blend info
int VGS_GetTextureBlenderInfo(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetTextureBlenderInfo(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
//texture-mipmaps
int VGS_GetTextureMipmaps(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetTextureMipmaps(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
//texture-uv index
int VGS_GetTextureUVIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetTextureUVIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
//texture-basic
int VGS_GetBasalTextureImage(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_AddMaterialBasalTexture(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_GetTextureAsReflect(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetTextureAsReflect(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_GetTextureAsOpacity(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetTextureAsOpacity(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetBasalTextureImage(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
//texture-cubic
int VGS_GetCubicTextureImages(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_AddMaterialCubicTexture(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetCubicTextureImages(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
//texture-dynamic
int VGS_GetDynamicTextureImageCount(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_GetDynamicTextureImageByIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_AddMaterialDynamicTexture(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetDynamicTextureImages(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetDynamicTextureImageByIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_AddDynamicTextureImage(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_DeleteDynamicTextureImageByIndex(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_GetDynamicDuration(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetDynamicTextureDuration(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
//texture-bump
int VGS_AddMaterialBumpTexture(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_GetBumpTextureImage(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetBumpTextureImage(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_GetTextureAsBump(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
int VGS_SetTextureAsBump(LuaState * state)
{
	if (0 == g_ulMainScene)
		return 0;
	return 0;
}
//texture-辅助
// int VGS_GetTextureImageCount(LuaState * state)
// int VGS_GetTextureImageNamebyIndex(LuaState * state)
// int VGS_IsImageInused(LuaState * state)
//----材质相关----end
