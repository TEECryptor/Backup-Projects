#include "LuaEngine.h"

#include "VgsApi.h"
#include "..\VGSCore.h"

//使用静态链接
//#if defined(DEBUG) || defined(_DEBUG)
//#pragma comment(lib, "..//lua//luaplus//LuaPlusLibD_1100.lib")//static lib
//#else
//#pragma comment(lib, "luaplus//LuaPlusLib_1100.lib")//static lib
//#endif 

// namespace VGS
// {
	LuaEngine::LuaEngine(CVGSCore* vgs)
		:m_pVgsCore(vgs)
		,m_bIinitialised(false)
	{
		RegisterVGSAPI();
	}
	LuaEngine::~LuaEngine()
	{

	}
	const char * LuaEngine::GetLastErrorInfo()
	{
		return m_strErrorInfo.c_str();
	}

	bool LuaEngine::DoFile(const char * szLuaFile)
	{
		if (strlen(szLuaFile) <= 0)
			return false;
		m_bIinitialised = m_luaStateOwner->DoFile(szLuaFile) == 0 ? true: false;
		return m_bIinitialised;
	}
	bool LuaEngine::DoString(const char * szLuaCmd)
	{
		m_luaStateOwner->DoString(szLuaCmd);
		return true;
	}

	void LuaEngine::OnInitVgs()
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> Lua_InitVgs = m_luaStateOwner->GetGlobal("OnInitVgs");
		Lua_InitVgs();
	}
	void LuaEngine::OnExit()
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> Lua_Exit = m_luaStateOwner->GetGlobal("OnExit");
		Lua_Exit();
	}
	void LuaEngine::OnResize()
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> Lua_Resize = m_luaStateOwner->GetGlobal("OnResize");
		Lua_Resize();
	}
	void LuaEngine::OnActive()
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> Lua_Active = m_luaStateOwner->GetGlobal("OnActive");
		Lua_Active();
	}
	void LuaEngine::OnStartOneFrame()
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> Lua_Active = m_luaStateOwner->GetGlobal("OnStartOneFrame");
		Lua_Active();
	}
	void LuaEngine::OnEndOneFrame()
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> Lua_Active = m_luaStateOwner->GetGlobal("OnEndOneFrame");
		Lua_Active();
	}

	void LuaEngine::OnKeyPressEvent(int nKey, int FuncKeys)
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> Lua_KeyPress = m_luaStateOwner->GetGlobal("OnKeyPress");
		Lua_KeyPress(nKey, FuncKeys);
	}
	void LuaEngine::OnKeyDownEvent(int nKey, int FuncKeys)
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> Lua_KeyDown = m_luaStateOwner->GetGlobal("OnKeyDown");
		Lua_KeyDown(nKey, FuncKeys);
	}
	void LuaEngine::OnKeyUpEvent(int nKey, int FuncKeys)
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> Lua_KeyUp = m_luaStateOwner->GetGlobal("OnKeyUp");
		Lua_KeyUp(nKey, FuncKeys);
	}
	void LuaEngine::AllMouseEnter(int keyModifiers, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> func = m_luaStateOwner->GetGlobal("OnMouseEnter");
		func(keyModifiers, x, y);
	}
	void LuaEngine::AllMouseLeave(int keyModifiers, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> func = m_luaStateOwner->GetGlobal("OnMouseLeave");
		func(keyModifiers, x, y);
	}
	void LuaEngine::AllMouseDown(int mouseID, int keyModifiers, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> func = m_luaStateOwner->GetGlobal("OnMouseDown");
		func(mouseID, keyModifiers, x, y);
	}
	void LuaEngine::AllMouseUp(int nMouseID, int keyModifiers, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> func = m_luaStateOwner->GetGlobal("OnMouseUp");
		func(nMouseID, keyModifiers, x, y);
	}
	void LuaEngine::AllMouseDoubleClick(int nMouseID, int keyModifiers, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> func = m_luaStateOwner->GetGlobal("OnMouseDblClick");
		func(nMouseID, keyModifiers, x, y);
	}
	void LuaEngine::AllMouseMove(int keyModifiers, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> func = m_luaStateOwner->GetGlobal("OnMouseMove");
		func(keyModifiers, x, y);
	}
	void LuaEngine::AllMouseWheel(int keyModifiers, int x, int y, int delta)
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> func = m_luaStateOwner->GetGlobal("OnMouseWheel");
		func(keyModifiers, x, y, delta);
	}
	void LuaEngine::OnMouseEnter(const char * szLuaFuction, int FuncKeys, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		if (strlen(szLuaFuction) <= 0)
			return;
		LuaPlus::LuaFunction<void> Lua_MouseEnter = m_luaStateOwner->GetGlobal(szLuaFuction);
		Lua_MouseEnter(FuncKeys, x, y);
	}
	void LuaEngine::OnMouseLeave(const char * szLuaFuction, int FuncKeys, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		if (strlen(szLuaFuction) <= 0)
			return;
		LuaPlus::LuaFunction<void> Lua_MouseLeave = m_luaStateOwner->GetGlobal(szLuaFuction);
		Lua_MouseLeave(FuncKeys, x, y);
	}
	void LuaEngine::OnMouseDown(const char * szLuaFuction, int nMouseID, int FuncKeys, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		if (strlen(szLuaFuction) <= 0)
			return;
		LuaPlus::LuaFunction<void> Lua_MouseDown = m_luaStateOwner->GetGlobal(szLuaFuction);
		Lua_MouseDown(FuncKeys, x, y);
	}
	void LuaEngine::OnMouseUp(const char * szLuaFuction, int nMouseID, int FuncKeys, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		if (strlen(szLuaFuction) <= 0)
			return;
		LuaPlus::LuaFunction<void> Lua_MouseUp = m_luaStateOwner->GetGlobal(szLuaFuction);
		Lua_MouseUp(FuncKeys, x, y);
	}
	void LuaEngine::OnMouseMove(const char * szLuaFuction, int nMouseID, int FuncKeys, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		if (strlen(szLuaFuction) <= 0)
			return;
		LuaPlus::LuaFunction<void> Lua_MouseMove = m_luaStateOwner->GetGlobal(szLuaFuction);
		Lua_MouseMove(FuncKeys, x, y);

	}
	void LuaEngine::OnMouseDoubleClick(const char * szLuaFuction, int nMouseID, int FuncKeys, int x, int y)
	{
		if (!m_bIinitialised)
			return;
		if (strlen(szLuaFuction) <= 0)
			return;
		LuaPlus::LuaFunction<void> Lua_MouseDblClick = m_luaStateOwner->GetGlobal(szLuaFuction);
		Lua_MouseDblClick(FuncKeys, x, y);
	}
	void LuaEngine::OnFsCommand(const char* viewName, const char* commands, const char* args)
	{
		if (!m_bIinitialised)
			return;
		LuaPlus::LuaFunction<void> Lua_FsCommand = m_luaStateOwner->GetGlobal("OnFlashCallBack");
		Lua_FsCommand(viewName, commands, args);
	}

	void LuaEngine::SetDebugCommand(const char * szDebugCmd)
	{
		if (!m_bIinitialised)
			return;
		if (strlen(szDebugCmd) <= 0)
			return;
		m_luaStateOwner->GetGlobals().SetString("debug_cmd",szDebugCmd);
	}

	const char * LuaEngine::GetLastDebugPrmVal(char * szParam)
	{
		LuaObject table = m_luaStateOwner->GetGlobal("value_param");
		if (!table.IsNil() && table.IsTable())
		{
			m_strDebugDumpParam = table["param"].GetString();
			m_strDebugDumpValue = table["value"].GetString();

			size_t st = m_strDebugDumpParam.length();
			if(st>256)
				st = 256;
			strncpy(szParam, m_strDebugDumpParam.c_str(), st);
			return m_strDebugDumpValue.c_str();
		}
		return 0;
	}

	void LuaEngine::RegisterVGSAPI()
	{
		//注册规则:注册字符串保持与VGS.h文件的API名称一致,函数地址为新封装的API

		m_luaStateOwner->GetGlobals().Register("Sleep", VGS_Sleep);
		m_luaStateOwner->GetGlobals().Register("MessageBox", VGS_MessageBox);
		m_luaStateOwner->GetGlobals().Register("SendMsgToHost", VGS_SendMsgToHost);
		
		//--->测试与演示用
		m_luaStateOwner->GetGlobals().Register("createFlashOverlay", (*m_pVgsCore), &CVGSCore::Lua_createFlashOverlay);
		m_luaStateOwner->GetGlobals().Register("createRenderTexture", (*m_pVgsCore), &CVGSCore::Lua_createRenderTexture);
		m_luaStateOwner->GetGlobals().Register("createFlashRTT", (*m_pVgsCore), &CVGSCore::Lua_createFlashRTT);
		m_luaStateOwner->GetGlobals().Register("setFlashRenderState", (*m_pVgsCore), &CVGSCore::Lua_setFlashRenderState);

		//<---

		//----场景操作相关----start
//  		m_luaStateOwner->GetGlobals().RegisterDirect("SetMainScene", VGS_SetMainScene);
 		m_luaStateOwner->GetGlobals().RegisterDirect("GetViewportFillMode", (*m_pVgsCore), &CVGSCore::GetViewportFillMode);
 		m_luaStateOwner->GetGlobals().RegisterDirect("SetViewportFillMode", (*m_pVgsCore), &CVGSCore::SetViewportFillMode);
 		m_luaStateOwner->GetGlobals().RegisterDirect("AddViewPort", (*m_pVgsCore), &CVGSCore::AddViewport);
 		m_luaStateOwner->GetGlobals().RegisterDirect("SetViewportCamera", (*m_pVgsCore), &CVGSCore::SetViewportCamera);
 		m_luaStateOwner->GetGlobals().RegisterDirect("GetViewportCamera", (*m_pVgsCore), &CVGSCore::GetViewportCamera);
 		m_luaStateOwner->GetGlobals().Register("GetViewPortRect", (*m_pVgsCore), &CVGSCore::Lua_GetViewPortRect);
 		m_luaStateOwner->GetGlobals().RegisterDirect("SetViewPortRect", (*m_pVgsCore), &CVGSCore::SetViewportRect);
 		m_luaStateOwner->GetGlobals().RegisterDirect("UpdateViewportAspectRatio", (*m_pVgsCore), &CVGSCore::UpdateViewportAspectRatio);
 		m_luaStateOwner->GetGlobals().RegisterDirect("IsViewportRenderGUI", (*m_pVgsCore), &CVGSCore::IsViewportRenderGUI);
 		m_luaStateOwner->GetGlobals().RegisterDirect("SetViewportRenderGUI", (*m_pVgsCore), &CVGSCore::SetViewportRenderGUI);
 		m_luaStateOwner->GetGlobals().RegisterDirect("GetSceneBackColor", (*m_pVgsCore), &CVGSCore::GetSceneBackColor);
 		m_luaStateOwner->GetGlobals().RegisterDirect("SetSceneBackColor", (*m_pVgsCore), &CVGSCore::SetSceneBackColor);
 		//----场景操作相关----end
 
 		//----模型操作----start
 		m_luaStateOwner->GetGlobals().RegisterDirect("GetModelCount", (*m_pVgsCore), &CVGSCore::GetModelCount);
 		m_luaStateOwner->GetGlobals().RegisterDirect("GetModelHandleByIndex", (*m_pVgsCore), &CVGSCore::GetModelHandleByIndex);
 		m_luaStateOwner->GetGlobals().Register("GetModelNameByIndex", (*m_pVgsCore), &CVGSCore::Lua_GetModelNameByIndex);
 		m_luaStateOwner->GetGlobals().Register("GetModelNameByHandle", (*m_pVgsCore), &CVGSCore::Lua_GetModelNameByHandle);
 		m_luaStateOwner->GetGlobals().RegisterDirect("GetModelHandleByName", (*m_pVgsCore), &CVGSCore::GetModelHandleByName);
 		m_luaStateOwner->GetGlobals().RegisterDirect("DeleteModel", (*m_pVgsCore), &CVGSCore::DeleteModel);
  		m_luaStateOwner->GetGlobals().Register("GetModelPosition", (*m_pVgsCore), &CVGSCore::Lua_GetModelPosition);
  		m_luaStateOwner->GetGlobals().Register("SetModelPosition", (*m_pVgsCore), &CVGSCore::Lua_SetModelPosition);
  		m_luaStateOwner->GetGlobals().Register("MoveModel", (*m_pVgsCore), &CVGSCore::Lua_MoveModel);
 
 		m_luaStateOwner->GetGlobals().Register("GetModelRotation", (*m_pVgsCore), &CVGSCore::Lua_GetModelRotation);
 		m_luaStateOwner->GetGlobals().Register("SetModelRotation", (*m_pVgsCore), &CVGSCore::Lua_SetModelRotation);
 		m_luaStateOwner->GetGlobals().Register("RotateModel", (*m_pVgsCore), &CVGSCore::Lua_RotateModel);
 		m_luaStateOwner->GetGlobals().Register("GetModelScale", (*m_pVgsCore), &CVGSCore::Lua_GetModelScale);
 		m_luaStateOwner->GetGlobals().Register("SetModelScale", (*m_pVgsCore), &CVGSCore::Lua_SetModelScale);
 		m_luaStateOwner->GetGlobals().Register("ScaleModel", (*m_pVgsCore), &CVGSCore::Lua_ScaleModel);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetModelBillboardStatus", (*m_pVgsCore), &CVGSCore::GetModelBillboardStatus);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetModelBillboardStatus", (*m_pVgsCore), &CVGSCore::SetModelBillboardStatus);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetModelCollisionStatus", (*m_pVgsCore), &CVGSCore::GetModelCollisionStatus);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetModelCollisionStatus", (*m_pVgsCore), &CVGSCore::SetModelCollisionStatus);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetModelAsGround", (*m_pVgsCore), &CVGSCore::GetModelAsGround);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetModelAsGround", (*m_pVgsCore), &CVGSCore::SetModelAsGround);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetModelVisibility", (*m_pVgsCore), &CVGSCore::GetModelVisibility);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetModelVisibility", (*m_pVgsCore), &CVGSCore::SetModelVisibility);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetModelClickabel", (*m_pVgsCore), &CVGSCore::GetModelClickabel);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetModelClickable", (*m_pVgsCore), &CVGSCore::SetModelClickable);
//		m_luaStateOwner->GetGlobals().RegisterDirect("IsModelHasKeyframeAnimation", (*m_pVgsCore), &CVGSCore::IsModelHasKeyframeAnimation);
//		m_luaStateOwner->GetGlobals().RegisterDirect("IsModelHasSkeletonAnimation", (*m_pVgsCore), &CVGSCore::IsModelHasSkeletonAnimation);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetModelMaterial", (*m_pVgsCore), &CVGSCore::SetModelMaterial);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetSubModelCount", (*m_pVgsCore), &CVGSCore::GetSubModelCount);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetSubModelMaterialName", (*m_pVgsCore), &CVGSCore::GetSubModelMaterialName);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetSubModelMaterialHandle", (*m_pVgsCore), &CVGSCore::GetSubModelMaterialHandle);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetSubModelMaterial", (*m_pVgsCore), &CVGSCore::SetSubModelMaterial);
//		//----模型操作----end
//
//		//----camera----start
//		m_luaStateOwner->GetGlobals().RegisterDirect("CreateCamera", (*m_pVgsCore), &CVGSCore::CreateCamera);
//		m_luaStateOwner->GetGlobals().RegisterDirect("DeleteCamerabyHandle", (*m_pVgsCore), &CVGSCore::DeleteCamerabyHandle);
//		m_luaStateOwner->GetGlobals().RegisterDirect("DeleteCamerabyName", (*m_pVgsCore), &CVGSCore::DeleteCamerabyName);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetCameraCount", (*m_pVgsCore), &CVGSCore::GetCameraCount);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetCameraHandleByIndex", (*m_pVgsCore), &CVGSCore::GetCameraHandleByIndex);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetCameraHandleByName", (*m_pVgsCore), &CVGSCore::GetCameraHandleByName);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetCameraNameByHandle", (*m_pVgsCore), &CVGSCore::GetCameraNameByHandle);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetCameraNameByIndex", (*m_pVgsCore), &CVGSCore::GetCameraNameByIndex);
//		m_luaStateOwner->GetGlobals().RegisterDirect("MoveCamera", (*m_pVgsCore), &CVGSCore::MoveCamera);
//		m_luaStateOwner->GetGlobals().RegisterDirect("RotateCamera", (*m_pVgsCore), &CVGSCore::RotateCamera);
//		m_luaStateOwner->GetGlobals().RegisterDirect("RoundCamera", (*m_pVgsCore), &CVGSCore::RoundCamera);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetCameraDirection", (*m_pVgsCore), &CVGSCore::GetCameraDirection);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetCameraDirection", (*m_pVgsCore), &CVGSCore::SetCameraDirection);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetCameraLookAtModel", (*m_pVgsCore), &CVGSCore::SetCameraLookAtModel);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetCameraFov", (*m_pVgsCore), &CVGSCore::SetCameraFov);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetCameraNearClip", (*m_pVgsCore), &CVGSCore::SetCameraNearClip);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetCameraFarClip", (*m_pVgsCore), &CVGSCore::SetCameraFarClip);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetCameraPosition", (*m_pVgsCore), &CVGSCore::SetCameraPosition);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetBillboardCamera", (*m_pVgsCore), &CVGSCore::SetBillboardCamera);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetBillboardCamera", (*m_pVgsCore), &CVGSCore::GetBillboardCamera);
//		//----camera----end
//
//		//----灯光----start
//		m_luaStateOwner->GetGlobals().RegisterDirect("DeleteLight", (*m_pVgsCore), &CVGSCore::DeleteLight);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetLightCount", (*m_pVgsCore), &CVGSCore::GetLightCount);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetLightHandleByIndex", (*m_pVgsCore), &CVGSCore::GetLightHandleByIndex);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetLightNameByIndex", (*m_pVgsCore), &CVGSCore::GetLightNameByIndex);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetLightNameByHandle", (*m_pVgsCore), &CVGSCore::GetLightNameByHandle);
//		m_luaStateOwner->GetGlobals().RegisterDirect("GetLightHandleByName", (*m_pVgsCore), &CVGSCore::GetLightHandleByName);
//		m_luaStateOwner->GetGlobals().RegisterDirect("MoveLight", (*m_pVgsCore), &CVGSCore::MoveLight);
//// 		m_luaStateOwner->GetGlobals().RegisterDirect("GetLightType", (*m_pVgsCore), &CVGSCore::GetLightType);
//// 		m_luaStateOwner->GetGlobals().RegisterDirect("SetLightType", (*m_pVgsCore), &CVGSCore::SetLightType);
//// 		m_luaStateOwner->GetGlobals().RegisterDirect("GetLightDiffuse", (*m_pVgsCore), &CVGSCore::GetLightDiffuse);
//// 		m_luaStateOwner->GetGlobals().RegisterDirect("SetLightDiffuse", (*m_pVgsCore), &CVGSCore::SetLightDiffuse);
//// 		m_luaStateOwner->GetGlobals().RegisterDirect("GetLightAmbient", (*m_pVgsCore), &CVGSCore::GetLightAmbient);
//// 		m_luaStateOwner->GetGlobals().RegisterDirect("SetLightAmbient", (*m_pVgsCore), &CVGSCore::SetLightAmbient);
//// 		m_luaStateOwner->GetGlobals().RegisterDirect("GetLightMultiply", (*m_pVgsCore), &CVGSCore::GetLightMultiply);
//// 		m_luaStateOwner->GetGlobals().RegisterDirect("SetLightMultiply", (*m_pVgsCore), &CVGSCore::SetLightMultiply);
//// 		m_luaStateOwner->GetGlobals().RegisterDirect("GetLightPosition", (*m_pVgsCore), &CVGSCore::GetLightPosition);
//		m_luaStateOwner->GetGlobals().RegisterDirect("SetLightPosition", (*m_pVgsCore), &CVGSCore::SetLightPosition);
//// 		m_luaStateOwner->GetGlobals().RegisterDirect("GetLightDirection", (*m_pVgsCore), &CVGSCore::GetLightDirection);
//// 		m_luaStateOwner->GetGlobals().RegisterDirect("SetLightDirection", (*m_pVgsCore), &CVGSCore::SetLightDirection);
		//----灯光----end
	}
// }
