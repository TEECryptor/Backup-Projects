#ifndef __VGS_API_H__
#define __VGS_API_H__

//!
//!文件 vgsapi.h
//!描述 对VGS提供的API接口函数进行Lua的包装，提供给Lua脚本调用.
//!		注册使用的函数名称(即最终Lua脚本中能使用的C++函数名)仍然与VGS.h提供的保持一致.
//!返回值约定:
//!		1.VGS.h中BOOL通过返回0和-1表示,VGS.h中VOID的返回0,有返回值的封装返回的是返回值个数;
//!		2.VGS.h中通过传入参数来获取返回值的,在Lua中将返回多个返回值.
//!作者 linxq
//!创建 2008-12-08
//!更新 2009-12-4
//!

#include "luaplus/LuaPlus.h"

using namespace LuaPlus;

extern unsigned long g_ulMainScene;//声明全局变量，主场景句柄

// 额外函数
int VGS_Sleep(LuaState * state);
int VGS_MessageBox(LuaState * state);
//向宿主程序发一条消息
int VGS_SendMsgToHost(LuaState * state);

// int VGS_Init(LuaState * state);
// int VGS_Exit(LuaState * state);
// 
// int VGS_CreateScene(LuaState * state);
// int VGS_DeleteScene(LuaState * state);
// int VGS_ResetScene(LuaState * state);
// int VGS_DeleteAllScene(LuaState * state);
//----场景句柄操作----start
// int VGS_GetSceneCount(LuaState * state);
// int VGS_GetSceneHandleByIndex(LuaState * state);
// int VGS_SetMainScene(LuaState * state);//设置主场景，需要传入一个索引号，默认为0
// int VGS_GetSceneNameByIndex(LuaState * state);
// int VGS_GetSceneHandleByName(LuaState * state);
// int VGS_GetSceneNameByHandle(LuaState * state);
// int VGS_GetSceneIndexByHandle(LuaState * state);
//----场景句柄操作----end

//----场景操作相关----start
//render
int VGS_RenderAll(LuaState * state);
// int VGS_RenderScene(LuaState * state);
// int VGS_RenderCurrentFrame(LuaState * state);
// int VGS_GetSceneRenderStatus(LuaState * state);
// int VGS_SetSceneRenderStatus(LuaState * state);
// int VGS_GetRenderTargetStatus(LuaState * state);
// int VGS_SetRenderTargetStatus(LuaState * state);
int VGS_GetViewportFillMode(LuaState * state);
int VGS_SetViewportFillMode(LuaState * state);
//viewport
int VGS_AddViewPort(LuaState * state);
int VGS_GetViewportCamera(LuaState * state);
int VGS_SetViewportCamera(LuaState * state);
int VGS_GetViewPortRect(LuaState * state);
int VGS_SetViewPortRect(LuaState * state);
int VGS_UpdateViewportAspectRatio(LuaState * state);
int VGS_IsViewportRenderGUI(LuaState * state);
int VGS_SetViewportRenderGUI(LuaState * state);
int VGS_GetSceneBackColor(LuaState * state);
int VGS_SetSceneBackColor(LuaState * state);
//s3d
// int VGS_ImportS3DFile(LuaState * state);
// int VGS_DeleteS3DLoader(LuaState * state);
// int VGS_IsTextureImageUsedbyS3D(LuaState * state);
// int VGS_GetS3DTextureImageFile(LuaState * state);
//v3d
// int VGS_OpenV3DFile(LuaState * state);
// int VGS_SaveSceneAsV3D(LuaState * state);
//n3d
// int VGS_SaveSceneAsN3D(LuaState * state);
//e3d
// int VGS_LoadE3DFile(LuaState * state);
// int VGS_SaveSceneAsE3D(LuaState * state);
//----场景渲染相关----end

//----模型操作----start
//get
int VGS_GetModelCount(LuaState * state);
int VGS_GetModelHandleByIndex(LuaState * state);
int VGS_GetModelNameByIndex(LuaState * state);
int VGS_GetModelNameByHandle(LuaState * state);
int VGS_GetModelHandleByName(LuaState * state);
//delete
int VGS_DeleteModel(LuaState * state);
//transform
int VGS_GetModelPosition(LuaState * state);
int VGS_SetModelPosition(LuaState * state);
int VGS_MoveModel(LuaState * state);
int VGS_GetModelRotation(LuaState * state);
int VGS_SetModelRotation(LuaState * state);
int VGS_RotateModel(LuaState * state);
int VGS_GetModelScale(LuaState * state);
int VGS_SetModelScale(LuaState * state);
int VGS_ScaleModel(LuaState * state);
//bounding volume
// int VGS_GetModelBoundsBox(LuaState * state);
// int VGS_GetModelBoundsSphere(LuaState * state);
// int VGS_SetModelBoundsBoxVisibility(LuaState * state);
//pick
// int VGS_GetModelUnderLoc(LuaState * state);
//property
// int VGS_GetModelInfo(LuaState * state);
// int VGS_SetModelInfo(LuaState * state);
int VGS_GetModelBillboardStatus(LuaState * state);
int VGS_SetModelBillboardStatus(LuaState * state);
int VGS_GetModelCollisionStatus(LuaState * state);
int VGS_SetModelCollisionStatus(LuaState * state);
int VGS_GetModelAsGround(LuaState * state);
int VGS_SetModelAsGround(LuaState * state);
int VGS_GetModelVisibility(LuaState * state);
int VGS_SetModelVisibility(LuaState * state);
int VGS_GetModelClickabel(LuaState * state);
int VGS_SetModelClickable(LuaState * state);
int VGS_IsModelHasKeyframeAnimation(LuaState * state);
int VGS_IsModelHasSkeletonAnimation(LuaState * state);
int VGS_SetModelMaterial(LuaState * state);
int VGS_GetSubModelCount(LuaState * state);
int VGS_GetSubModelMaterialName(LuaState * state);
int VGS_GetSubModelMaterialHandle(LuaState * state);
int VGS_SetSubModelMaterial(LuaState * state);
//----模型操作----end

//----camera----start
int VGS_CreateCamera(LuaState * state);
int VGS_DeleteCamerabyHandle(LuaState * state);
int VGS_DeleteCamerabyName(LuaState * state);
//get/set
// int VGS_GetPerspectiveCamera(LuaState * state);
// int VGS_CreatePerspectiveCamera(LuaState * state);
int VGS_GetCameraCount(LuaState * state);
int VGS_GetCameraHandleByIndex(LuaState * state);
int VGS_GetCameraHandleByName(LuaState * state);
int VGS_GetCameraNameByHandle(LuaState * state);
int VGS_GetCameraNameByIndex(LuaState * state);
// int VGS_SetCameraNamebyHandle(LuaState * state);
// int VGS_SetCameraNamebyName(LuaState * state);
//transform
int VGS_MoveCamera(LuaState * state);
int VGS_RotateCamera(LuaState * state);
int VGS_RoundCamera(LuaState * state);
//property
// int VGS_SetCameraAspect(LuaState * state);
// int VGS_GetCameraInfo(LuaState * state);
// int VGS_SetCameraInfo(LuaState * state);
int VGS_GetCameraDirection(LuaState * state);
int VGS_SetCameraDirection(LuaState * state);
int VGS_SetCameraLookAtModel(LuaState * state);
int VGS_SetCameraFov(LuaState * state);
int VGS_SetCameraNearClip(LuaState * state);
int VGS_SetCameraFarClip(LuaState * state);
int VGS_SetCameraPosition(LuaState * state);
int VGS_SetBillboardCamera(LuaState * state);
int VGS_GetBillboardCamera(LuaState * state);
//----camera----end

//----灯光----start
int VGS_DeleteLight(LuaState * state);
int VGS_GetLightCount(LuaState * state);
int VGS_GetLightHandleByIndex(LuaState * state);
int VGS_GetLightNameByIndex(LuaState * state);
int VGS_GetLightNameByHandle(LuaState * state);
int VGS_GetLightHandleByName(LuaState * state);
// int VGS_SetLightName(LuaState * state);
int VGS_MoveLight(LuaState * state);
//int VGS_GetLightInfo(LuaState * state);
//int VGS_SetLightInfo(LuaState * state);
int VGS_GetLightType(LuaState * state);
int VGS_SetLightType(LuaState * state);
int VGS_GetLightDiffuse(LuaState * state);
int VGS_SetLightDiffuse(LuaState * state);
int VGS_GetLightAmbient(LuaState * state);
int VGS_SetLightAmbient(LuaState * state);
int VGS_GetLightMultiply(LuaState * state);
int VGS_SetLightMultiply(LuaState * state);
int VGS_GetLightPosition(LuaState * state);
int VGS_SetLightPosition(LuaState * state);
int VGS_GetLightDirection(LuaState * state);
int VGS_SetLightDirection(LuaState * state);

// int VGS_SetLightPosition(LuaState * state);
//----灯光----end

//----材质相关----start
//create/delete
// int VGS_CreateMaterial(LuaState * state);
// int VGS_DeleteMaterialByHandle(LuaState * state);
// int VGS_DeleteMaterialByName(LuaState * state);
//get mtrl
int VGS_GetMaterialCount(LuaState * state);
int VGS_GetMaterialHandleByName(LuaState * state);
int VGS_GetMaterialHandleByIndex(LuaState * state);
int VGS_GetMaterialNameByHandle(LuaState * state);
//mtrl info
// int VGS_GetMaterialInfo(LuaState * state);
// int VGS_SetMaterialInfo(LuaState * state);
int VGS_GetMaterialAmbient(LuaState * state);
int VGS_SetMaterialAmbient(LuaState * state);
int VGS_GetMaterialDiffuse(LuaState * state);
int VGS_SetMaterialDiffuse(LuaState * state);
int VGS_GetMaterialSpecular(LuaState * state);
int VGS_SetMaterialSpecular(LuaState * state);
int VGS_GetMaterialSpecularLevel(LuaState * state);
int VGS_SetMaterialSpecularLevel(LuaState * state);
int VGS_GetMaterialShininess(LuaState * state);
int VGS_SetMaterialShininess(LuaState * state);
int VGS_GetMaterialEmissive(LuaState * state);
int VGS_SetMaterialEmissive(LuaState * state);
int VGS_GetMaterialOpacity(LuaState * state);
int VGS_SetMaterialOpacity(LuaState * state);
int VGS_GetMaterialTwoSide(LuaState * state);
int VGS_SetMaterialTwoSide(LuaState * state);
int VGS_GetMaterialAlphaTest(LuaState * state);
int VGS_SetMaterialAlphaTest(LuaState * state);
int VGS_GetMaterialFillMode(LuaState * state);
int VGS_SetMaterialFillMode(LuaState * state);
int VGS_GetMaterialAlphaTestValue(LuaState * state);
int VGS_SetMaterialAlphaTestValue(LuaState * state);

//uv speed
// int VGS_GetMaterialUVSpeed(LuaState * state);
// int VGS_SetMaterialUVSpeed(LuaState * state);
//texture-normal
int VGS_GetMaterialTextureCount(LuaState * state);
int VGS_GetMaterialTextureHandle(LuaState * state);
int VGS_ClearMaterialTexture(LuaState * state);
// int VGS_RemoveMaterialTexture(LuaState * state);
int VGS_GetTextureType(LuaState * state);
//texture-blend info
int VGS_GetTextureBlenderInfo(LuaState * state);
int VGS_SetTextureBlenderInfo(LuaState * state);
//texture-mipmaps
int VGS_GetTextureMipmaps(LuaState * state);
int VGS_SetTextureMipmaps(LuaState * state);
//texture-uv index
int VGS_GetTextureUVIndex(LuaState * state);
int VGS_SetTextureUVIndex(LuaState * state);
//texture-basic
int VGS_GetBasalTextureImage(LuaState * state);
int VGS_AddMaterialBasalTexture(LuaState * state);
int VGS_GetTextureAsReflect(LuaState * state);
int VGS_SetTextureAsReflect(LuaState * state);
int VGS_GetTextureAsOpacity(LuaState * state);
int VGS_SetTextureAsOpacity(LuaState * state);
int VGS_SetBasalTextureImage(LuaState * state);
//texture-cubic
int VGS_GetCubicTextureImages(LuaState * state);
int VGS_AddMaterialCubicTexture(LuaState * state);
int VGS_SetCubicTextureImages(LuaState * state);
//texture-dynamic
int VGS_GetDynamicTextureImageCount(LuaState * state);
int VGS_GetDynamicTextureImageByIndex(LuaState * state);
int VGS_AddMaterialDynamicTexture(LuaState * state);
int VGS_SetDynamicTextureImages(LuaState * state);
int VGS_SetDynamicTextureImageByIndex(LuaState * state);
int VGS_AddDynamicTextureImage(LuaState * state);
int VGS_DeleteDynamicTextureImageByIndex(LuaState * state);
int VGS_GetDynamicDuration(LuaState * state);
int VGS_SetDynamicTextureDuration(LuaState * state);
//texture-bump
int VGS_AddMaterialBumpTexture(LuaState * state);
int VGS_GetBumpTextureImage(LuaState * state);
int VGS_SetBumpTextureImage(LuaState * state);
int VGS_GetTextureAsBump(LuaState * state);
int VGS_SetTextureAsBump(LuaState * state);
//texture-辅助
int VGS_GetTextureImageCount(LuaState * state);
int VGS_GetTextureImageNamebyIndex(LuaState * state);
int VGS_IsImageInused(LuaState * state);
//----材质相关----end

//----背景图----start
int VGS_GetBackgroundHandle(LuaState * state);
int VGS_SetBackground(LuaState * state);
int VGS_RemoveBackground(LuaState * state);
int VGS_GetBackgroundImage(LuaState * state);
int VGS_IsBackgroundVisible(LuaState * state);
int VGS_SetBackgroundVisibility(LuaState * state);
int VGS_GetBackgroundRect(LuaState * state);
int VGS_SetBackgroundRect(LuaState * state);
int VGS_FullScreenBackground(LuaState * state);
//----背景图----end

//----GUI----start
int VGS_DeleteGUIObject(LuaState * state);
int VGS_GetAllGUIObjectsCount(LuaState * state);
int VGS_GetGUIObjectCount(LuaState * state);
int VGS_GetAllGUIObjectsList(LuaState * state);
int VGS_GetGUIObjectType(LuaState * state);
int VGS_GetGUIObjetHandleByIndex(LuaState * state);
int VGS_GetGUIObjectHandleByName(LuaState * state);
int VGS_GetGUIObjectNameByHandle(LuaState * state);
int VGS_SetGUIObjectName(LuaState * state);

int VGS_GetGUIObjOffsetOrigin(LuaState * state);
int VGS_SetGUIObjOffsetOrigin(LuaState * state);
int VGS_GetGUIObjUseRelativePos(LuaState * state);
int VGS_SetGUIObjUseRelativePos(LuaState * state);
int VGS_GetGUIObjUseRelativeSize(LuaState * state);
int VGS_SetGUIObjUseRelativeSize(LuaState * state);
int VGS_GetGUIObjectRect(LuaState * state);
int VGS_SetGUIObjectRect(LuaState * state);
int VGS_GetGUIObjectAbsoluteRect(LuaState * state);
int VGS_SetGUIObjectAbsoluteRect(LuaState * state);
int VGS_SetGUIObjectPosition(LuaState * state);
int VGS_SetGUIObjectSize(LuaState * state);
int VGS_GetGUIObjectOpacity(LuaState * state);
int VGS_SetGUIObjectOpacity(LuaState * state);
int VGS_GetGUIObjectVisibility(LuaState * state);
int VGS_SetGUIObjectVisibility(LuaState * state);
//
int VGS_GetGUIObjectZOrder(LuaState * state);
int VGS_MoveGUIObjectUp(LuaState * state);
int VGS_MoveGUIObjectDown(LuaState * state);
int VGS_GotoGUIObjectsTop(LuaState * state);
int VGS_GotoGUIObjectsBottom(LuaState * state);
//overlay
int VGS_CreateOverlayFromFile(LuaState * state);
int VGS_GetOverlayCount(LuaState * state);
int VGS_GetOverlayImageFile(LuaState * state);
int VGS_SetOverlayImageFile(LuaState * state);
//button
int VGS_SetButtonEnable(LuaState * state);
int VGS_IsButtonEnable(LuaState * state);
int VGS_CreateButtonFromFile(LuaState * state);
int VGS_GetButtonMouseUpImageFile(LuaState * state);
int VGS_SetButtonMouseUpImageFile(LuaState * state);
int VGS_GetButtonMouseOverImageFile(LuaState * state);
int VGS_SetButtonMouseOverImageFile(LuaState * state);
int VGS_GetButtonMouseDownImageFile(LuaState * state);
int VGS_SetButtonMouseDownImageFile(LuaState * state);
//导航图
int VGS_CreateNavigater(LuaState * state);
int VGS_GetNavigaterMap(LuaState * state);
int VGS_SetNavigaterMap(LuaState * state);
int VGS_GetNavigaterSceneRect(LuaState * state);
int VGS_SetNavigaterSceneRect(LuaState * state);
int VGS_AddNavigaterPointer(LuaState * state);
int VGS_CloneNavigaterPointer(LuaState * state);
int VGS_DeleteNavigaterPointer(LuaState * state);
int VGS_GetNavPointerCount(LuaState * state);
int VGS_GetNavPointerNameByIndex(LuaState * state);
int VGS_RenamePointer(LuaState * state);
int VGS_GetNavPointerImage(LuaState * state);
int VGS_SetNavPointerImage(LuaState * state);
int VGS_GetNavPointerObj(LuaState * state);
int VGS_SetNavPointerObj(LuaState * state);
int VGS_GetNavPointerVisiblilty(LuaState * state);
int VGS_SetNavPointerVisiblilty(LuaState * state);
int VGS_GetNavPointerOpacity(LuaState * state);
int VGS_SetNavPointerOpacity(LuaState * state);
int VGS_GetNavPointerSize(LuaState * state);
int VGS_SetNavPointerSize(LuaState * state);
int VGS_GetPointerPosition(LuaState * state);
int VGS_SetPointerPosition(LuaState * state);
//----GUI----end

//----控制对象----start
//walker
int VGS_GetWalkerProp(LuaState * state);
int VGS_SetWalkerProp(LuaState * state);
//editor
int VGS_GetEditorProp(LuaState * state);
int VGS_SetEditorProp(LuaState * state);
int VGS_SetEditorLookAt(LuaState * state);
int VGS_SetEditorLookAtModel(LuaState * state);
//flyer
int VGS_GetFlyerProp(LuaState * state);
int VGS_SetFlyerProp(LuaState * state);
//----控制对象----end

//----关键帧动画----start
int VGS_GetAnimationCount(LuaState * state);
int VGS_GetAnimationNameByIndex(LuaState * state);
int VGS_PlayAllAnimation(LuaState * state);
int VGS_PlayAnimation(LuaState * state);
int VGS_PauseAllAnimation(LuaState * state);
int VGS_PauseAnimation(LuaState * state);
int VGS_StopAllAnimation(LuaState * state);
int VGS_StopAnimation(LuaState * state);
int VGS_GetKeyframeCount(LuaState * state);
int VGS_GetCurrentFrame(LuaState * state);
int VGS_GotoFrame(LuaState * state);
int VGS_SetAllKfrPlaySpeed(LuaState * state);
int VGS_GetDefaultFPS(LuaState * state);
int VGS_GetKfrPlaySpeed(LuaState * state);
int VGS_SetKfrPlaySpeed(LuaState * state);
int VGS_GetAnimationLoop(LuaState * state);
int VGS_SetAnimationLoop(LuaState * state);
//----关键帧动画----end

//----辅助功能函数----start
int VGS_UpdateBackBuf(LuaState * state);
int VGS_CreateCameraRenderThumb(LuaState * state);
// int VGS_GetWorldPointUnderLoc(LuaState * state);
int VGS_GetVGSLastErrStr(LuaState * state);
//----辅助功能函数----end

#endif