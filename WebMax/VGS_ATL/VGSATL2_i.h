

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Sat Sep 04 19:24:30 2010
 */
/* Compiler settings for .\VGSATL2.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __VGSATL2_i_h__
#define __VGSATL2_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IVGS2_FWD_DEFINED__
#define __IVGS2_FWD_DEFINED__
typedef interface IVGS2 IVGS2;
#endif 	/* __IVGS2_FWD_DEFINED__ */


#ifndef ___IVGS2Events_FWD_DEFINED__
#define ___IVGS2Events_FWD_DEFINED__
typedef interface _IVGS2Events _IVGS2Events;
#endif 	/* ___IVGS2Events_FWD_DEFINED__ */


#ifndef __VGS2_FWD_DEFINED__
#define __VGS2_FWD_DEFINED__

#ifdef __cplusplus
typedef class VGS2 VGS2;
#else
typedef struct VGS2 VGS2;
#endif /* __cplusplus */

#endif 	/* __VGS2_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IVGS2_INTERFACE_DEFINED__
#define __IVGS2_INTERFACE_DEFINED__

/* interface IVGS2 */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVGS2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F66246AF-D0D7-46D3-891F-9EFBC9FAC257")
    IVGS2 : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelVisibility( 
            BSTR modelName,
            BOOL visible) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDynamicUV( 
            BSTR modelName,
            FLOAT u,
            FLOAT v,
            FLOAT speed) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddFlashCtrl( 
            BSTR flashName,
            BSTR flashFile,
            INT alignMode,
            INT left,
            INT top,
            INT width,
            INT height,
            BOOL loop) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFlashRect( 
            BSTR flashName,
            INT alignMode,
            INT left,
            INT top,
            INT width,
            INT height) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFlashCtrlVisibility( 
            BSTR flashName,
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRenderStatus( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFlashPlayStatus( 
            BSTR flashName,
            INT status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteFlashCtrl( 
            BSTR flashName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveCamera( 
            BSTR cameraName,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT stepCnt,
            INT coordEnum) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveCurrentCamera( 
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT stepCnt,
            INT coordEnum) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowDefaultLogo( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetControlStatus( 
            INT status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCameraClip( 
            BSTR cameraName,
            FLOAT nearClip,
            FLOAT farClip) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCurrentCameraClip( 
            FLOAT nearClip,
            FLOAT farClip) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DollyCurrentCamera( 
            FLOAT value,
            INT stepCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DollyCamera( 
            BSTR cameraName,
            FLOAT value,
            INT stepCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetBackgroundColor( 
            INT red,
            INT green,
            INT blue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PreloadN3DFile( 
            BSTR strN3DFileName,
            BSTR N3DName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GotoN3D( 
            BSTR N3DName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCurrentCameraByName( 
            BSTR camName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayCameraKfr( 
            BSTR camName,
            INT repeatCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCurrentCameraFrom( 
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCurrentCameraAt( 
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopCameraKfr( 
            BSTR camName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetServer( 
            BSTR strServer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelClickable( 
            BSTR modelName,
            BOOL clickable) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateOverlayFromImage( 
            BSTR overlayName,
            BSTR oriImageName,
            BSTR alphaImageName,
            INT alignMode,
            INT left,
            INT top,
            INT width,
            INT height) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteOverlay( 
            BSTR overlayName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOverlayOpacity( 
            BSTR overlayName,
            INT opacity) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateButtonFromImage( 
            BSTR btnName,
            BSTR mouseUpImage,
            BSTR mouseUpImage_alpha,
            BSTR mouseOverImage,
            BSTR mouseOverImage_alpha,
            BSTR mouseDownImage,
            BSTR mouseDownImage_alpha,
            INT alignMode,
            INT x,
            INT y,
            INT width,
            INT height,
            BSTR callBackFunc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteButton( 
            BSTR btnName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TranslateModel( 
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT coord) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetEditCtrl_DollySpeed( 
            FLOAT dollySpeed) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetEditCtrl_RosAxis( 
            INT rosFlag) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetEditCtrl_DollyDistance( 
            FLOAT nearDis,
            FLOAT farDis) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetEditCtrl_AutoPitchRoundRoll( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetEditCtrl_AutoParam( 
            FLOAT PitchSpeedValue,
            FLOAT RoundSpeedValue,
            FLOAT RollSpeedValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetEditCtrl_ManualParam( 
            FLOAT RoundSpeed,
            FLOAT RollSpeed) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveCurrentCameraTo( 
            FLOAT from_x,
            FLOAT from_y,
            FLOAT from_z,
            FLOAT at_x,
            FLOAT at_y,
            FLOAT at_z,
            INT frameCnt,
            INT accType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFBL( 
            FLOAT fblValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOverlayWidth( 
            BSTR overlayName,
            INT width) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOverlayHeight( 
            BSTR overlayName,
            INT height) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOverlayAlignMode( 
            BSTR overlayName,
            INT alignMode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOverlayRect( 
            BSTR overlayName,
            INT left,
            INT top,
            INT width,
            INT height) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveOverlay( 
            BSTR overlayName,
            INT x,
            INT y) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveCurrentCameraFromTo( 
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveCurrentCameraAtTo( 
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveCurrentCameraFrom( 
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveCurrentCameraAt( 
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadN3DFile( 
            BSTR strN3DFileName,
            BSTR N3DName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResetScene( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelMtrl( 
            BSTR modelName,
            INT subMeshID,
            BSTR mtrlName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMtrlOpacity( 
            BSTR mtrlName,
            INT opacity) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCurrentCameraByIndex( 
            INT camIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetWalkCtrl_ManualParam( 
            FLOAT moveSpeed,
            FLOAT moveAcc,
            FLOAT rotateSpeed,
            FLOAT rotateAcc,
            FLOAT height) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelMouseHint( 
            BSTR modelName,
            BOOL bFlag) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableMouseDownCallback( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DisableMouseDownCallback( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableMouseUpCallback( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DisableMouseUpCallback( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableMouseDBClickCallback( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DisableMouseDBClickCallback( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableMouseMoveCallback( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DisableMouseMoveCallback( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelMouseMtrl( 
            BSTR modelName,
            BSTR mouseOverMtrl,
            BSTR mouseDownMtrl) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelOpacity( 
            BSTR modelName,
            INT opacity) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateModelGroup( 
            BSTR modelGroupName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateBumpWaterTexture( 
            BSTR bumpWaterName,
            INT width,
            INT height,
            INT xSize,
            INT ySize,
            FLOAT radius,
            FLOAT speed,
            FLOAT centerX,
            FLOAT centerY) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RotateCurrentCamera( 
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT stepCnt,
            INT coordEnum) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RotateCamera( 
            BSTR cameraName,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT stepCnt,
            INT coordEnum) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FocusCurrentCameraTo( 
            BSTR model1Name,
            BSTR model2Name,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            FLOAT distance,
            INT stepCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HideAllModels( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowAllModels( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetModelHandleByName( 
            BSTR modelName,
            VARIANT *modelHandle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RestoreModelMtrl( 
            BSTR modelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RestoreAllModelsMtrl( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateMtrl( 
            BSTR mtrlName,
            INT red,
            INT green,
            INT blue,
            INT alpha,
            INT selfIllu) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMtrlColor( 
            BSTR mtrlName,
            INT red,
            INT green,
            INT blue,
            INT alpha) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteMtrl( 
            BSTR mtrlName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMtrlIllumination( 
            BSTR mtrlName,
            INT illumination) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveModel( 
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT coordEnum) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveModelTo( 
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT frameCnt,
            INT accModel) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RestoreModelOpacity( 
            BSTR modelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RestoreAllModelsOpacity( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerGoForward( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerGoBackward( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerGoLeftward( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerGoRightward( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerStop( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerTurnLeft( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerTurnRight( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerTurnUp( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerTurnDown( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerZoomIn( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerZoomOut( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerGoUp( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayerGoDown( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelColor( 
            BSTR modelName,
            INT red,
            INT green,
            INT blue,
            INT alpha) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelFBLStatus( 
            BSTR modelName,
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetBumpWaterSpeed( 
            BSTR waterName,
            FLOAT speed) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetBumpWaterRadius( 
            BSTR waterName,
            FLOAT radius) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSkyBoxModel( 
            BSTR modelName,
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCameraFrom( 
            BSTR cameraName,
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCameraAt( 
            BSTR cameraName,
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCameraFov( 
            BSTR cameraName,
            FLOAT fov) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCurrentCameraFov( 
            FLOAT fov) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateFSMotionBlur( 
            INT blurCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFSMotionBlurPower( 
            INT value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartFSMotionBlur( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopFSMotionBlur( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCameraKFRFSMotionBlur( 
            BSTR camName,
            INT startFrame,
            INT endFrame,
            INT power,
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFlashCurrentFrame( 
            BSTR flashName,
            INT frameIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCameraKfrCallBackFunc( 
            BSTR camName,
            INT frameIndex,
            BSTR funcName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateLensFlare( 
            FLOAT x,
            FLOAT y,
            FLOAT z,
            BOOL coverFlag) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateFog( 
            INT fogMode,
            INT red,
            INT green,
            INT blue,
            FLOAT density,
            FLOAT start,
            FLOAT end) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFogMode( 
            INT mode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFogColor( 
            INT red,
            INT blue,
            INT green) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFogDensity( 
            FLOAT density) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFogField( 
            FLOAT start,
            FLOAT end) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFogRenderStatus( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFilter( 
            BSTR overlayName,
            BOOL status,
            INT opacity) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelMouseColor( 
            BSTR modelName,
            INT mouseOver_Red,
            INT mouseOver_Green,
            INT mouseOver_Blue,
            INT mouseOver_Alpha,
            INT mouseClick_Red,
            INT mouseClick_Green,
            INT mouseClick_Blue,
            INT mouseClick_Alpha) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMouseMoveEventTimer( 
            INT timeStep) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetNavigaterVisibility( 
            BSTR navName,
            BOOL visible) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RestoreModelColor( 
            BSTR modelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RestoreAllModelsColor( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAlphaTestStatus( 
            BSTR mtrlName,
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetZOneModel( 
            BSTR modelName,
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLensFlareVisibility( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCollisionDistance( 
            FLOAT distance) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateStaticCubeMap( 
            BSTR name,
            BSTR frontImage,
            BSTR backImage,
            BSTR leftImage,
            BSTR rightImage,
            BSTR topImage,
            BSTR bottomImge,
            INT size) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMtrlCubeMap( 
            BSTR mtrlName,
            BSTR cubeMapName,
            FLOAT power) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLightColor( 
            BSTR lightName,
            INT red,
            INT green,
            INT blue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLightPower( 
            BSTR lightName,
            FLOAT power) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLightAmbient( 
            BSTR lightName,
            INT red,
            INT green,
            INT blue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLightPosition( 
            BSTR lightName,
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLightRange( 
            BSTR lightName,
            FLOAT range) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLightAttenuation( 
            BSTR lightName,
            FLOAT attenuation0,
            FLOAT attenuation1,
            FLOAT attenuation2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLightStatus( 
            BSTR lightName,
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateMovieTexture( 
            BSTR texName,
            BSTR moviePath,
            INT movieType,
            BOOL withAudio,
            INT repeatCnt,
            BSTR mtrlName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMovieMtrl( 
            BSTR mtrlName,
            BSTR movieTexName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelKfrRepeatCnt( 
            BSTR modelName,
            INT kfrRepeatCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAllModelKfrRepeatCnt( 
            INT kfrRepeatCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateBackdropFromImage( 
            BSTR backdropName,
            BSTR oriImageName,
            BSTR alphaImageName,
            INT alignMode,
            INT left,
            INT top,
            INT width,
            INT height) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateSpecularMap( 
            BSTR name,
            BSTR imageName,
            INT width,
            INT height,
            INT mipLevel) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSpecularMapMtrl( 
            BSTR mtrlName,
            BSTR specularMapName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateCamera( 
            BSTR name,
            FLOAT fromX,
            FLOAT fromY,
            FLOAT fromZ,
            FLOAT atX,
            FLOAT atY,
            FLOAT atZ,
            FLOAT fov,
            FLOAT nearClip,
            FLOAT farClip) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteModel( 
            BSTR modelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCanNavigateByPointer( 
            BSTR navName,
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetPlayerCanMove( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetPlayerCanRos( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetPlayerCanZoom( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetPlayerCanLift( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteMovieTexture( 
            BSTR mtrlName,
            BSTR movieTexName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelKfrPlayStatus( 
            BSTR name,
            BOOL kfrPlay) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayModelKfr( 
            BSTR modelName,
            INT startFrame,
            INT endFrame,
            INT repeatCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAllModelsKfrPlayStatus( 
            BOOL bKfrPlay) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelEventOnMouseMove( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetButtonVisibility( 
            BSTR btnName,
            BOOL visible) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAntiAliasLevel( 
            INT level) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMtrlSpecularCubeMap( 
            BSTR mtrlName,
            BSTR specularCubeMapName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetButtonActive( 
            BSTR name,
            BOOL bActive) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateSectionMap( 
            BSTR sectioMapName,
            INT width,
            INT height,
            BSTR sliceModelName,
            FLOAT leftTopX,
            FLOAT leftTopY,
            FLOAT leftTopZ,
            FLOAT rightTopX,
            FLOAT rightTopY,
            FLOAT rightTopZ,
            FLOAT leftBottomX,
            FLOAT leftBottomY,
            FLOAT leftBottomZ,
            FLOAT rightBottomX,
            FLOAT rightBottomY,
            FLOAT rightBottomZ,
            BSTR imageName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExecuteSlice( 
            BSTR sectionMapName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOverlayVisibility( 
            BSTR overlayName,
            BOOL visible) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateAudio( 
            BSTR name,
            BSTR audioFile,
            INT repeatCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteAudioByName( 
            BSTR name) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteAudioByIndex( 
            INT index) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteAllAudio( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAudioFile( 
            BSTR audioName,
            BSTR audioFile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAudioRepeatCount( 
            BSTR audioName,
            INT repeatCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayAudio( 
            BSTR audioName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PauseAudio( 
            BSTR audioName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopAudio( 
            BSTR audioName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RewindAudio( 
            BSTR audioName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteModelGroup( 
            BSTR name) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteModelGroupByIndex( 
            INT index) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteAllModelGroup( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddModelToModelGroup( 
            BSTR modelGroupName,
            BSTR modelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteModelFromModelGroup( 
            BSTR modelGroupName,
            BSTR modelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearModelGroup( 
            BSTR modelGroupName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelMouseHintType( 
            BSTR modelName,
            INT type) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLightKfrPlayStatus( 
            BSTR lightName,
            BOOL bFlag) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAllLightKfrPlayStatus( 
            BOOL bFlag) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLightKfrRepeatCnt( 
            BSTR lightName,
            INT repeatCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAllLightKfrRepeatCnt( 
            INT repeatCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAllModelKfrPlayStatus( 
            BOOL kfrPlay) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelPosition( 
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelRotation( 
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddNavigaterPointer( 
            BSTR navigaterName,
            BSTR navPointerName,
            INT navPointerType,
            BSTR camOrModelName,
            BSTR navPointerOriImage,
            BSTR navPointerAlphaImage,
            INT width,
            INT height) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetNavigaterPointerVisibility( 
            BSTR navigaterName,
            BSTR navPointerName,
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteNavigaterPointer( 
            BSTR navigaterName,
            BSTR navPointerName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PauseCameraKfr( 
            BSTR camName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResumeCameraKfr( 
            BSTR cameraName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetNavigaterPointerObject( 
            BSTR navName,
            BSTR navPointerName,
            INT objType,
            BSTR objName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetPlayerZoomSpeed( 
            FLOAT speed) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOverlayFullScreen( 
            BSTR OverlayName,
            BOOL xFull,
            BOOL yFull) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PushOverlayFront( 
            BSTR OverlayName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PushOverlayBack( 
            BSTR overlayName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteAllModel( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayKeyFrame( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PauseKeyFrame( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopKeyFrame( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetKeyFrameRepeatCount( 
            INT repeatCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GotoFrame( 
            INT frame) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetKeyFramePlaySpeed( 
            INT fps) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateFSGlow( 
            FLOAT BloomValue,
            FLOAT BlurValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFSGlowValue( 
            FLOAT BloomValue,
            FLOAT BlurValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartFSGlow( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EndFSGlow( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMtrlSpecular( 
            BSTR mtrlName,
            INT red,
            INT green,
            INT blue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMtrlAmbient( 
            BSTR mtrlName,
            INT red,
            INT green,
            INT blue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMtrlSpecularLevel( 
            BSTR mtrlName,
            INT level) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMtrlSpecularPower( 
            BSTR mtrlName,
            INT power) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlayKeyFrame1( 
            INT startFrame,
            INT endFrame) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCameraKfrPlayStatus( 
            BSTR camName,
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAllModelVisibility( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAllCameraKfrPlayStatus( 
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCameraKfrRepeatCnt( 
            BSTR camName,
            INT repeatCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAllCameraKfrRepeatCnt( 
            INT repeatCnt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDynamicTexPlaySpeed( 
            BSTR UseDyTexMtrlName,
            INT texLayerIdx,
            INT fps) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetModelPositionByPivot( 
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDynamicUVStatus( 
            BSTR modelName,
            BOOL status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveFlash( 
            BSTR flashName,
            INT x,
            INT y) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOverlayMtrl( 
            BSTR overlayName,
            BSTR mtrlName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetZoneTriggerModelCallBackFunc( 
            BSTR modelName,
            BSTR funcName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAlphaTestLevel( 
            INT level) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetButtonOpacity( 
            BSTR buttonName,
            INT opacity) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CurCamGotoFrame( 
            INT frame) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSceneEditMode( 
            INT EditMode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloneModel( 
            BSTR oriModelName,
            BSTR desModelName,
            INT ID1,
            INT ID2,
            FLOAT posX,
            FLOAT posY,
            FLOAT posZ) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theModelPosX( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theModelPosY( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theModelPosZ( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theModelScreenPosX( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theModelScreenPosY( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInfoFromModel( 
            BSTR modelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateNewText( 
            BSTR textString) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theTextHandle( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTextPosition( 
            long textHandle,
            UINT x,
            UINT y) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTextColor( 
            long textHandle,
            INT red,
            INT green,
            INT blue,
            INT alpha) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteText( 
            long textHandle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInfoFromCamera( 
            BSTR cameraName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theCamFromPosX( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theCamFromPosY( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theCamFromPosZ( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theCamAtPosX( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theCamAtPosY( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theCamAtPosZ( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTextString( 
            long textHandle,
            BSTR textString) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadScene( 
            BSTR SceneName,
            BSTR ParentSceneName,
            BSTR N3DFileURL,
            FLOAT m11,
            FLOAT m12,
            FLOAT m13,
            FLOAT m14,
            FLOAT m21,
            FLOAT m22,
            FLOAT m23,
            FLOAT m24,
            FLOAT m31,
            FLOAT m32,
            FLOAT m33,
            FLOAT m34,
            FLOAT m41,
            FLOAT m42,
            FLOAT m43,
            FLOAT m44) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteScene( 
            BSTR SceneName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloneScene( 
            BSTR destSceneName,
            BSTR srcSceneName,
            BSTR ParentSceneName,
            FLOAT m11,
            FLOAT m12,
            FLOAT m13,
            FLOAT m14,
            FLOAT m21,
            FLOAT m22,
            FLOAT m23,
            FLOAT m24,
            FLOAT m31,
            FLOAT m32,
            FLOAT m33,
            FLOAT m34,
            FLOAT m41,
            FLOAT m42,
            FLOAT m43,
            FLOAT m44) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateRTFTexture( 
            BSTR TextureName,
            BSTR refPlaneName,
            INT height,
            INT width,
            FLOAT power) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddModelToRTFTexture( 
            BSTR RTFTextureName,
            BSTR ModelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateDynamicCubeMap( 
            BSTR cubeMapame,
            BSTR centerModelName,
            INT size,
            INT power) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddDynamicCubeMapReflectionModel( 
            BSTR cubeMapName,
            BSTR ModelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteDynamicCubeMapReflectionModel( 
            BSTR cubeMapName,
            BSTR modelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateRealWater( 
            BSTR name,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT gridX,
            INT gridY) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddRealWaterRTTModel( 
            BSTR RealWaterName,
            BSTR ModelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddRealWaterRTFModel( 
            BSTR RealWaterName,
            BSTR ModelName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRealWaterCubeMap( 
            BSTR RealWaterName,
            BSTR CubeMapName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateColorEffect( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetColorEffect( 
            FLOAT brightness,
            FLOAT contrast,
            FLOAT red,
            FLOAT green,
            FLOAT blue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTextureImageFromURL( 
            BSTR mtrlName,
            INT texLayerIdx,
            BSTR imageURL) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetEditCtrlMaxUpDownAngle( 
            FLOAT upAngle,
            FLOAT downAngle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMtrlTexture( 
            BSTR mtrlName,
            INT textureLayerIndex,
            INT textureType,
            BSTR textureName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddFlashCtrlSemaphore( 
            BSTR flashName,
            BSTR SemaphoreName,
            BSTR SemaphoreFunc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteFlashCtrlSemaphore( 
            BSTR flashName,
            BSTR SemaphoreName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theFlashVar( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFlashVaribleInfo( 
            BSTR flashName,
            BSTR varibleName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFlashVarible( 
            BSTR flashName,
            BSTR varibleName,
            BSTR varibleValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadN3DScene( 
            BSTR SceneName,
            BSTR ParentSceneName,
            BSTR N3DFileURL,
            FLOAT posX,
            FLOAT posY,
            FLOAT posZ,
            FLOAT rotX,
            FLOAT rotY,
            FLOAT rotZ,
            FLOAT scaleX,
            FLOAT scaleY,
            FLOAT scaleZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInfoFromSceneResource( 
            BSTR SceneName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theScenePosX( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theScenePosY( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theScenePosZ( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theSceneRotX( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theSceneRotY( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theSceneRotZ( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theSceneScaleX( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theSceneScaleY( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_theSceneScaleZ( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSceneResColor( 
            BSTR SceneName,
            INT red,
            INT green,
            INT blue,
            INT alpha) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetButtonRect( 
            BSTR buttonName,
            INT x,
            INT y,
            INT width,
            INT height) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAddtionalKeys( 
            /* [in] */ BYTE left,
            /* [in] */ BYTE right,
            /* [in] */ BYTE up,
            /* [in] */ BYTE down) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAddtionalKeysStatus( 
            /* [in] */ BOOL bEnable) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSubBrowserType( 
            /* [in] */ LONG subType) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_wbType( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_wbType( 
            /* [in] */ LONG newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVGS2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVGS2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVGS2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVGS2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVGS2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVGS2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVGS2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVGS2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelVisibility )( 
            IVGS2 * This,
            BSTR modelName,
            BOOL visible);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetDynamicUV )( 
            IVGS2 * This,
            BSTR modelName,
            FLOAT u,
            FLOAT v,
            FLOAT speed);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddFlashCtrl )( 
            IVGS2 * This,
            BSTR flashName,
            BSTR flashFile,
            INT alignMode,
            INT left,
            INT top,
            INT width,
            INT height,
            BOOL loop);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFlashRect )( 
            IVGS2 * This,
            BSTR flashName,
            INT alignMode,
            INT left,
            INT top,
            INT width,
            INT height);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFlashCtrlVisibility )( 
            IVGS2 * This,
            BSTR flashName,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRenderStatus )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFlashPlayStatus )( 
            IVGS2 * This,
            BSTR flashName,
            INT status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteFlashCtrl )( 
            IVGS2 * This,
            BSTR flashName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveCamera )( 
            IVGS2 * This,
            BSTR cameraName,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT stepCnt,
            INT coordEnum);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveCurrentCamera )( 
            IVGS2 * This,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT stepCnt,
            INT coordEnum);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowDefaultLogo )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetControlStatus )( 
            IVGS2 * This,
            INT status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCameraClip )( 
            IVGS2 * This,
            BSTR cameraName,
            FLOAT nearClip,
            FLOAT farClip);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentCameraClip )( 
            IVGS2 * This,
            FLOAT nearClip,
            FLOAT farClip);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DollyCurrentCamera )( 
            IVGS2 * This,
            FLOAT value,
            INT stepCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DollyCamera )( 
            IVGS2 * This,
            BSTR cameraName,
            FLOAT value,
            INT stepCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetBackgroundColor )( 
            IVGS2 * This,
            INT red,
            INT green,
            INT blue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PreloadN3DFile )( 
            IVGS2 * This,
            BSTR strN3DFileName,
            BSTR N3DName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GotoN3D )( 
            IVGS2 * This,
            BSTR N3DName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentCameraByName )( 
            IVGS2 * This,
            BSTR camName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayCameraKfr )( 
            IVGS2 * This,
            BSTR camName,
            INT repeatCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentCameraFrom )( 
            IVGS2 * This,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentCameraAt )( 
            IVGS2 * This,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StopCameraKfr )( 
            IVGS2 * This,
            BSTR camName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetServer )( 
            IVGS2 * This,
            BSTR strServer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelClickable )( 
            IVGS2 * This,
            BSTR modelName,
            BOOL clickable);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateOverlayFromImage )( 
            IVGS2 * This,
            BSTR overlayName,
            BSTR oriImageName,
            BSTR alphaImageName,
            INT alignMode,
            INT left,
            INT top,
            INT width,
            INT height);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteOverlay )( 
            IVGS2 * This,
            BSTR overlayName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetOverlayOpacity )( 
            IVGS2 * This,
            BSTR overlayName,
            INT opacity);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateButtonFromImage )( 
            IVGS2 * This,
            BSTR btnName,
            BSTR mouseUpImage,
            BSTR mouseUpImage_alpha,
            BSTR mouseOverImage,
            BSTR mouseOverImage_alpha,
            BSTR mouseDownImage,
            BSTR mouseDownImage_alpha,
            INT alignMode,
            INT x,
            INT y,
            INT width,
            INT height,
            BSTR callBackFunc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteButton )( 
            IVGS2 * This,
            BSTR btnName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TranslateModel )( 
            IVGS2 * This,
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT coord);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetEditCtrl_DollySpeed )( 
            IVGS2 * This,
            FLOAT dollySpeed);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetEditCtrl_RosAxis )( 
            IVGS2 * This,
            INT rosFlag);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetEditCtrl_DollyDistance )( 
            IVGS2 * This,
            FLOAT nearDis,
            FLOAT farDis);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetEditCtrl_AutoPitchRoundRoll )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetEditCtrl_AutoParam )( 
            IVGS2 * This,
            FLOAT PitchSpeedValue,
            FLOAT RoundSpeedValue,
            FLOAT RollSpeedValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetEditCtrl_ManualParam )( 
            IVGS2 * This,
            FLOAT RoundSpeed,
            FLOAT RollSpeed);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveCurrentCameraTo )( 
            IVGS2 * This,
            FLOAT from_x,
            FLOAT from_y,
            FLOAT from_z,
            FLOAT at_x,
            FLOAT at_y,
            FLOAT at_z,
            INT frameCnt,
            INT accType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFBL )( 
            IVGS2 * This,
            FLOAT fblValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetOverlayWidth )( 
            IVGS2 * This,
            BSTR overlayName,
            INT width);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetOverlayHeight )( 
            IVGS2 * This,
            BSTR overlayName,
            INT height);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetOverlayAlignMode )( 
            IVGS2 * This,
            BSTR overlayName,
            INT alignMode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetOverlayRect )( 
            IVGS2 * This,
            BSTR overlayName,
            INT left,
            INT top,
            INT width,
            INT height);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveOverlay )( 
            IVGS2 * This,
            BSTR overlayName,
            INT x,
            INT y);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveCurrentCameraFromTo )( 
            IVGS2 * This,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveCurrentCameraAtTo )( 
            IVGS2 * This,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveCurrentCameraFrom )( 
            IVGS2 * This,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveCurrentCameraAt )( 
            IVGS2 * This,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadN3DFile )( 
            IVGS2 * This,
            BSTR strN3DFileName,
            BSTR N3DName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResetScene )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelMtrl )( 
            IVGS2 * This,
            BSTR modelName,
            INT subMeshID,
            BSTR mtrlName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMtrlOpacity )( 
            IVGS2 * This,
            BSTR mtrlName,
            INT opacity);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentCameraByIndex )( 
            IVGS2 * This,
            INT camIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetWalkCtrl_ManualParam )( 
            IVGS2 * This,
            FLOAT moveSpeed,
            FLOAT moveAcc,
            FLOAT rotateSpeed,
            FLOAT rotateAcc,
            FLOAT height);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelMouseHint )( 
            IVGS2 * This,
            BSTR modelName,
            BOOL bFlag);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnableMouseDownCallback )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DisableMouseDownCallback )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnableMouseUpCallback )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DisableMouseUpCallback )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnableMouseDBClickCallback )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DisableMouseDBClickCallback )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnableMouseMoveCallback )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DisableMouseMoveCallback )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelMouseMtrl )( 
            IVGS2 * This,
            BSTR modelName,
            BSTR mouseOverMtrl,
            BSTR mouseDownMtrl);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelOpacity )( 
            IVGS2 * This,
            BSTR modelName,
            INT opacity);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateModelGroup )( 
            IVGS2 * This,
            BSTR modelGroupName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateBumpWaterTexture )( 
            IVGS2 * This,
            BSTR bumpWaterName,
            INT width,
            INT height,
            INT xSize,
            INT ySize,
            FLOAT radius,
            FLOAT speed,
            FLOAT centerX,
            FLOAT centerY);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RotateCurrentCamera )( 
            IVGS2 * This,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT stepCnt,
            INT coordEnum);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RotateCamera )( 
            IVGS2 * This,
            BSTR cameraName,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT stepCnt,
            INT coordEnum);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FocusCurrentCameraTo )( 
            IVGS2 * This,
            BSTR model1Name,
            BSTR model2Name,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            FLOAT distance,
            INT stepCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HideAllModels )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowAllModels )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetModelHandleByName )( 
            IVGS2 * This,
            BSTR modelName,
            VARIANT *modelHandle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RestoreModelMtrl )( 
            IVGS2 * This,
            BSTR modelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RestoreAllModelsMtrl )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateMtrl )( 
            IVGS2 * This,
            BSTR mtrlName,
            INT red,
            INT green,
            INT blue,
            INT alpha,
            INT selfIllu);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMtrlColor )( 
            IVGS2 * This,
            BSTR mtrlName,
            INT red,
            INT green,
            INT blue,
            INT alpha);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteMtrl )( 
            IVGS2 * This,
            BSTR mtrlName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMtrlIllumination )( 
            IVGS2 * This,
            BSTR mtrlName,
            INT illumination);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveModel )( 
            IVGS2 * This,
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT coordEnum);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveModelTo )( 
            IVGS2 * This,
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT frameCnt,
            INT accModel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RestoreModelOpacity )( 
            IVGS2 * This,
            BSTR modelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RestoreAllModelsOpacity )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerGoForward )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerGoBackward )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerGoLeftward )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerGoRightward )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerStop )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerTurnLeft )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerTurnRight )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerTurnUp )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerTurnDown )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerZoomIn )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerZoomOut )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerGoUp )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayerGoDown )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelColor )( 
            IVGS2 * This,
            BSTR modelName,
            INT red,
            INT green,
            INT blue,
            INT alpha);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelFBLStatus )( 
            IVGS2 * This,
            BSTR modelName,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetBumpWaterSpeed )( 
            IVGS2 * This,
            BSTR waterName,
            FLOAT speed);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetBumpWaterRadius )( 
            IVGS2 * This,
            BSTR waterName,
            FLOAT radius);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetSkyBoxModel )( 
            IVGS2 * This,
            BSTR modelName,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCameraFrom )( 
            IVGS2 * This,
            BSTR cameraName,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCameraAt )( 
            IVGS2 * This,
            BSTR cameraName,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCameraFov )( 
            IVGS2 * This,
            BSTR cameraName,
            FLOAT fov);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentCameraFov )( 
            IVGS2 * This,
            FLOAT fov);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateFSMotionBlur )( 
            IVGS2 * This,
            INT blurCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFSMotionBlurPower )( 
            IVGS2 * This,
            INT value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartFSMotionBlur )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StopFSMotionBlur )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCameraKFRFSMotionBlur )( 
            IVGS2 * This,
            BSTR camName,
            INT startFrame,
            INT endFrame,
            INT power,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFlashCurrentFrame )( 
            IVGS2 * This,
            BSTR flashName,
            INT frameIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCameraKfrCallBackFunc )( 
            IVGS2 * This,
            BSTR camName,
            INT frameIndex,
            BSTR funcName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateLensFlare )( 
            IVGS2 * This,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            BOOL coverFlag);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateFog )( 
            IVGS2 * This,
            INT fogMode,
            INT red,
            INT green,
            INT blue,
            FLOAT density,
            FLOAT start,
            FLOAT end);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFogMode )( 
            IVGS2 * This,
            INT mode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFogColor )( 
            IVGS2 * This,
            INT red,
            INT blue,
            INT green);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFogDensity )( 
            IVGS2 * This,
            FLOAT density);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFogField )( 
            IVGS2 * This,
            FLOAT start,
            FLOAT end);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFogRenderStatus )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFilter )( 
            IVGS2 * This,
            BSTR overlayName,
            BOOL status,
            INT opacity);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelMouseColor )( 
            IVGS2 * This,
            BSTR modelName,
            INT mouseOver_Red,
            INT mouseOver_Green,
            INT mouseOver_Blue,
            INT mouseOver_Alpha,
            INT mouseClick_Red,
            INT mouseClick_Green,
            INT mouseClick_Blue,
            INT mouseClick_Alpha);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMouseMoveEventTimer )( 
            IVGS2 * This,
            INT timeStep);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetNavigaterVisibility )( 
            IVGS2 * This,
            BSTR navName,
            BOOL visible);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RestoreModelColor )( 
            IVGS2 * This,
            BSTR modelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RestoreAllModelsColor )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAlphaTestStatus )( 
            IVGS2 * This,
            BSTR mtrlName,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetZOneModel )( 
            IVGS2 * This,
            BSTR modelName,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLensFlareVisibility )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCollisionDistance )( 
            IVGS2 * This,
            FLOAT distance);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateStaticCubeMap )( 
            IVGS2 * This,
            BSTR name,
            BSTR frontImage,
            BSTR backImage,
            BSTR leftImage,
            BSTR rightImage,
            BSTR topImage,
            BSTR bottomImge,
            INT size);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMtrlCubeMap )( 
            IVGS2 * This,
            BSTR mtrlName,
            BSTR cubeMapName,
            FLOAT power);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLightColor )( 
            IVGS2 * This,
            BSTR lightName,
            INT red,
            INT green,
            INT blue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLightPower )( 
            IVGS2 * This,
            BSTR lightName,
            FLOAT power);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLightAmbient )( 
            IVGS2 * This,
            BSTR lightName,
            INT red,
            INT green,
            INT blue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLightPosition )( 
            IVGS2 * This,
            BSTR lightName,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLightRange )( 
            IVGS2 * This,
            BSTR lightName,
            FLOAT range);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLightAttenuation )( 
            IVGS2 * This,
            BSTR lightName,
            FLOAT attenuation0,
            FLOAT attenuation1,
            FLOAT attenuation2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLightStatus )( 
            IVGS2 * This,
            BSTR lightName,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateMovieTexture )( 
            IVGS2 * This,
            BSTR texName,
            BSTR moviePath,
            INT movieType,
            BOOL withAudio,
            INT repeatCnt,
            BSTR mtrlName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMovieMtrl )( 
            IVGS2 * This,
            BSTR mtrlName,
            BSTR movieTexName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelKfrRepeatCnt )( 
            IVGS2 * This,
            BSTR modelName,
            INT kfrRepeatCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAllModelKfrRepeatCnt )( 
            IVGS2 * This,
            INT kfrRepeatCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateBackdropFromImage )( 
            IVGS2 * This,
            BSTR backdropName,
            BSTR oriImageName,
            BSTR alphaImageName,
            INT alignMode,
            INT left,
            INT top,
            INT width,
            INT height);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateSpecularMap )( 
            IVGS2 * This,
            BSTR name,
            BSTR imageName,
            INT width,
            INT height,
            INT mipLevel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetSpecularMapMtrl )( 
            IVGS2 * This,
            BSTR mtrlName,
            BSTR specularMapName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateCamera )( 
            IVGS2 * This,
            BSTR name,
            FLOAT fromX,
            FLOAT fromY,
            FLOAT fromZ,
            FLOAT atX,
            FLOAT atY,
            FLOAT atZ,
            FLOAT fov,
            FLOAT nearClip,
            FLOAT farClip);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModel )( 
            IVGS2 * This,
            BSTR modelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCanNavigateByPointer )( 
            IVGS2 * This,
            BSTR navName,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetPlayerCanMove )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetPlayerCanRos )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetPlayerCanZoom )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetPlayerCanLift )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteMovieTexture )( 
            IVGS2 * This,
            BSTR mtrlName,
            BSTR movieTexName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelKfrPlayStatus )( 
            IVGS2 * This,
            BSTR name,
            BOOL kfrPlay);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayModelKfr )( 
            IVGS2 * This,
            BSTR modelName,
            INT startFrame,
            INT endFrame,
            INT repeatCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAllModelsKfrPlayStatus )( 
            IVGS2 * This,
            BOOL bKfrPlay);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelEventOnMouseMove )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetButtonVisibility )( 
            IVGS2 * This,
            BSTR btnName,
            BOOL visible);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAntiAliasLevel )( 
            IVGS2 * This,
            INT level);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMtrlSpecularCubeMap )( 
            IVGS2 * This,
            BSTR mtrlName,
            BSTR specularCubeMapName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetButtonActive )( 
            IVGS2 * This,
            BSTR name,
            BOOL bActive);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateSectionMap )( 
            IVGS2 * This,
            BSTR sectioMapName,
            INT width,
            INT height,
            BSTR sliceModelName,
            FLOAT leftTopX,
            FLOAT leftTopY,
            FLOAT leftTopZ,
            FLOAT rightTopX,
            FLOAT rightTopY,
            FLOAT rightTopZ,
            FLOAT leftBottomX,
            FLOAT leftBottomY,
            FLOAT leftBottomZ,
            FLOAT rightBottomX,
            FLOAT rightBottomY,
            FLOAT rightBottomZ,
            BSTR imageName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExecuteSlice )( 
            IVGS2 * This,
            BSTR sectionMapName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetOverlayVisibility )( 
            IVGS2 * This,
            BSTR overlayName,
            BOOL visible);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateAudio )( 
            IVGS2 * This,
            BSTR name,
            BSTR audioFile,
            INT repeatCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteAudioByName )( 
            IVGS2 * This,
            BSTR name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteAudioByIndex )( 
            IVGS2 * This,
            INT index);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteAllAudio )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAudioFile )( 
            IVGS2 * This,
            BSTR audioName,
            BSTR audioFile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAudioRepeatCount )( 
            IVGS2 * This,
            BSTR audioName,
            INT repeatCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayAudio )( 
            IVGS2 * This,
            BSTR audioName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PauseAudio )( 
            IVGS2 * This,
            BSTR audioName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StopAudio )( 
            IVGS2 * This,
            BSTR audioName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RewindAudio )( 
            IVGS2 * This,
            BSTR audioName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelGroup )( 
            IVGS2 * This,
            BSTR name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelGroupByIndex )( 
            IVGS2 * This,
            INT index);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteAllModelGroup )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelToModelGroup )( 
            IVGS2 * This,
            BSTR modelGroupName,
            BSTR modelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelFromModelGroup )( 
            IVGS2 * This,
            BSTR modelGroupName,
            BSTR modelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearModelGroup )( 
            IVGS2 * This,
            BSTR modelGroupName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelMouseHintType )( 
            IVGS2 * This,
            BSTR modelName,
            INT type);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLightKfrPlayStatus )( 
            IVGS2 * This,
            BSTR lightName,
            BOOL bFlag);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAllLightKfrPlayStatus )( 
            IVGS2 * This,
            BOOL bFlag);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetLightKfrRepeatCnt )( 
            IVGS2 * This,
            BSTR lightName,
            INT repeatCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAllLightKfrRepeatCnt )( 
            IVGS2 * This,
            INT repeatCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAllModelKfrPlayStatus )( 
            IVGS2 * This,
            BOOL kfrPlay);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelPosition )( 
            IVGS2 * This,
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelRotation )( 
            IVGS2 * This,
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddNavigaterPointer )( 
            IVGS2 * This,
            BSTR navigaterName,
            BSTR navPointerName,
            INT navPointerType,
            BSTR camOrModelName,
            BSTR navPointerOriImage,
            BSTR navPointerAlphaImage,
            INT width,
            INT height);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetNavigaterPointerVisibility )( 
            IVGS2 * This,
            BSTR navigaterName,
            BSTR navPointerName,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteNavigaterPointer )( 
            IVGS2 * This,
            BSTR navigaterName,
            BSTR navPointerName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PauseCameraKfr )( 
            IVGS2 * This,
            BSTR camName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResumeCameraKfr )( 
            IVGS2 * This,
            BSTR cameraName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetNavigaterPointerObject )( 
            IVGS2 * This,
            BSTR navName,
            BSTR navPointerName,
            INT objType,
            BSTR objName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetPlayerZoomSpeed )( 
            IVGS2 * This,
            FLOAT speed);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetOverlayFullScreen )( 
            IVGS2 * This,
            BSTR OverlayName,
            BOOL xFull,
            BOOL yFull);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PushOverlayFront )( 
            IVGS2 * This,
            BSTR OverlayName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PushOverlayBack )( 
            IVGS2 * This,
            BSTR overlayName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteAllModel )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayKeyFrame )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PauseKeyFrame )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StopKeyFrame )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetKeyFrameRepeatCount )( 
            IVGS2 * This,
            INT repeatCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GotoFrame )( 
            IVGS2 * This,
            INT frame);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetKeyFramePlaySpeed )( 
            IVGS2 * This,
            INT fps);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateFSGlow )( 
            IVGS2 * This,
            FLOAT BloomValue,
            FLOAT BlurValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFSGlowValue )( 
            IVGS2 * This,
            FLOAT BloomValue,
            FLOAT BlurValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartFSGlow )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EndFSGlow )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMtrlSpecular )( 
            IVGS2 * This,
            BSTR mtrlName,
            INT red,
            INT green,
            INT blue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMtrlAmbient )( 
            IVGS2 * This,
            BSTR mtrlName,
            INT red,
            INT green,
            INT blue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMtrlSpecularLevel )( 
            IVGS2 * This,
            BSTR mtrlName,
            INT level);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMtrlSpecularPower )( 
            IVGS2 * This,
            BSTR mtrlName,
            INT power);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlayKeyFrame1 )( 
            IVGS2 * This,
            INT startFrame,
            INT endFrame);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCameraKfrPlayStatus )( 
            IVGS2 * This,
            BSTR camName,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAllModelVisibility )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAllCameraKfrPlayStatus )( 
            IVGS2 * This,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCameraKfrRepeatCnt )( 
            IVGS2 * This,
            BSTR camName,
            INT repeatCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAllCameraKfrRepeatCnt )( 
            IVGS2 * This,
            INT repeatCnt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetDynamicTexPlaySpeed )( 
            IVGS2 * This,
            BSTR UseDyTexMtrlName,
            INT texLayerIdx,
            INT fps);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetModelPositionByPivot )( 
            IVGS2 * This,
            BSTR modelName,
            FLOAT x,
            FLOAT y,
            FLOAT z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetDynamicUVStatus )( 
            IVGS2 * This,
            BSTR modelName,
            BOOL status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveFlash )( 
            IVGS2 * This,
            BSTR flashName,
            INT x,
            INT y);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetOverlayMtrl )( 
            IVGS2 * This,
            BSTR overlayName,
            BSTR mtrlName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetZoneTriggerModelCallBackFunc )( 
            IVGS2 * This,
            BSTR modelName,
            BSTR funcName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAlphaTestLevel )( 
            IVGS2 * This,
            INT level);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetButtonOpacity )( 
            IVGS2 * This,
            BSTR buttonName,
            INT opacity);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CurCamGotoFrame )( 
            IVGS2 * This,
            INT frame);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetSceneEditMode )( 
            IVGS2 * This,
            INT EditMode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloneModel )( 
            IVGS2 * This,
            BSTR oriModelName,
            BSTR desModelName,
            INT ID1,
            INT ID2,
            FLOAT posX,
            FLOAT posY,
            FLOAT posZ);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theModelPosX )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theModelPosY )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theModelPosZ )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theModelScreenPosX )( 
            IVGS2 * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theModelScreenPosY )( 
            IVGS2 * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetInfoFromModel )( 
            IVGS2 * This,
            BSTR modelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateNewText )( 
            IVGS2 * This,
            BSTR textString);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theTextHandle )( 
            IVGS2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTextPosition )( 
            IVGS2 * This,
            long textHandle,
            UINT x,
            UINT y);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTextColor )( 
            IVGS2 * This,
            long textHandle,
            INT red,
            INT green,
            INT blue,
            INT alpha);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteText )( 
            IVGS2 * This,
            long textHandle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetInfoFromCamera )( 
            IVGS2 * This,
            BSTR cameraName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theCamFromPosX )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theCamFromPosY )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theCamFromPosZ )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theCamAtPosX )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theCamAtPosY )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theCamAtPosZ )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTextString )( 
            IVGS2 * This,
            long textHandle,
            BSTR textString);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadScene )( 
            IVGS2 * This,
            BSTR SceneName,
            BSTR ParentSceneName,
            BSTR N3DFileURL,
            FLOAT m11,
            FLOAT m12,
            FLOAT m13,
            FLOAT m14,
            FLOAT m21,
            FLOAT m22,
            FLOAT m23,
            FLOAT m24,
            FLOAT m31,
            FLOAT m32,
            FLOAT m33,
            FLOAT m34,
            FLOAT m41,
            FLOAT m42,
            FLOAT m43,
            FLOAT m44);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteScene )( 
            IVGS2 * This,
            BSTR SceneName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloneScene )( 
            IVGS2 * This,
            BSTR destSceneName,
            BSTR srcSceneName,
            BSTR ParentSceneName,
            FLOAT m11,
            FLOAT m12,
            FLOAT m13,
            FLOAT m14,
            FLOAT m21,
            FLOAT m22,
            FLOAT m23,
            FLOAT m24,
            FLOAT m31,
            FLOAT m32,
            FLOAT m33,
            FLOAT m34,
            FLOAT m41,
            FLOAT m42,
            FLOAT m43,
            FLOAT m44);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateRTFTexture )( 
            IVGS2 * This,
            BSTR TextureName,
            BSTR refPlaneName,
            INT height,
            INT width,
            FLOAT power);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelToRTFTexture )( 
            IVGS2 * This,
            BSTR RTFTextureName,
            BSTR ModelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateDynamicCubeMap )( 
            IVGS2 * This,
            BSTR cubeMapame,
            BSTR centerModelName,
            INT size,
            INT power);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddDynamicCubeMapReflectionModel )( 
            IVGS2 * This,
            BSTR cubeMapName,
            BSTR ModelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteDynamicCubeMapReflectionModel )( 
            IVGS2 * This,
            BSTR cubeMapName,
            BSTR modelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateRealWater )( 
            IVGS2 * This,
            BSTR name,
            FLOAT x,
            FLOAT y,
            FLOAT z,
            INT gridX,
            INT gridY);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddRealWaterRTTModel )( 
            IVGS2 * This,
            BSTR RealWaterName,
            BSTR ModelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddRealWaterRTFModel )( 
            IVGS2 * This,
            BSTR RealWaterName,
            BSTR ModelName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRealWaterCubeMap )( 
            IVGS2 * This,
            BSTR RealWaterName,
            BSTR CubeMapName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateColorEffect )( 
            IVGS2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetColorEffect )( 
            IVGS2 * This,
            FLOAT brightness,
            FLOAT contrast,
            FLOAT red,
            FLOAT green,
            FLOAT blue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTextureImageFromURL )( 
            IVGS2 * This,
            BSTR mtrlName,
            INT texLayerIdx,
            BSTR imageURL);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetEditCtrlMaxUpDownAngle )( 
            IVGS2 * This,
            FLOAT upAngle,
            FLOAT downAngle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMtrlTexture )( 
            IVGS2 * This,
            BSTR mtrlName,
            INT textureLayerIndex,
            INT textureType,
            BSTR textureName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddFlashCtrlSemaphore )( 
            IVGS2 * This,
            BSTR flashName,
            BSTR SemaphoreName,
            BSTR SemaphoreFunc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteFlashCtrlSemaphore )( 
            IVGS2 * This,
            BSTR flashName,
            BSTR SemaphoreName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theFlashVar )( 
            IVGS2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFlashVaribleInfo )( 
            IVGS2 * This,
            BSTR flashName,
            BSTR varibleName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFlashVarible )( 
            IVGS2 * This,
            BSTR flashName,
            BSTR varibleName,
            BSTR varibleValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadN3DScene )( 
            IVGS2 * This,
            BSTR SceneName,
            BSTR ParentSceneName,
            BSTR N3DFileURL,
            FLOAT posX,
            FLOAT posY,
            FLOAT posZ,
            FLOAT rotX,
            FLOAT rotY,
            FLOAT rotZ,
            FLOAT scaleX,
            FLOAT scaleY,
            FLOAT scaleZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetInfoFromSceneResource )( 
            IVGS2 * This,
            BSTR SceneName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theScenePosX )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theScenePosY )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theScenePosZ )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theSceneRotX )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theSceneRotY )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theSceneRotZ )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theSceneScaleX )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theSceneScaleY )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_theSceneScaleZ )( 
            IVGS2 * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetSceneResColor )( 
            IVGS2 * This,
            BSTR SceneName,
            INT red,
            INT green,
            INT blue,
            INT alpha);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetButtonRect )( 
            IVGS2 * This,
            BSTR buttonName,
            INT x,
            INT y,
            INT width,
            INT height);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAddtionalKeys )( 
            IVGS2 * This,
            /* [in] */ BYTE left,
            /* [in] */ BYTE right,
            /* [in] */ BYTE up,
            /* [in] */ BYTE down);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAddtionalKeysStatus )( 
            IVGS2 * This,
            /* [in] */ BOOL bEnable);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetSubBrowserType )( 
            IVGS2 * This,
            /* [in] */ LONG subType);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_wbType )( 
            IVGS2 * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_wbType )( 
            IVGS2 * This,
            /* [in] */ LONG newVal);
        
        END_INTERFACE
    } IVGS2Vtbl;

    interface IVGS2
    {
        CONST_VTBL struct IVGS2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVGS2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVGS2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVGS2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVGS2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVGS2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVGS2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVGS2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVGS2_SetModelVisibility(This,modelName,visible)	\
    ( (This)->lpVtbl -> SetModelVisibility(This,modelName,visible) ) 

#define IVGS2_SetDynamicUV(This,modelName,u,v,speed)	\
    ( (This)->lpVtbl -> SetDynamicUV(This,modelName,u,v,speed) ) 

#define IVGS2_AddFlashCtrl(This,flashName,flashFile,alignMode,left,top,width,height,loop)	\
    ( (This)->lpVtbl -> AddFlashCtrl(This,flashName,flashFile,alignMode,left,top,width,height,loop) ) 

#define IVGS2_SetFlashRect(This,flashName,alignMode,left,top,width,height)	\
    ( (This)->lpVtbl -> SetFlashRect(This,flashName,alignMode,left,top,width,height) ) 

#define IVGS2_SetFlashCtrlVisibility(This,flashName,status)	\
    ( (This)->lpVtbl -> SetFlashCtrlVisibility(This,flashName,status) ) 

#define IVGS2_SetRenderStatus(This,status)	\
    ( (This)->lpVtbl -> SetRenderStatus(This,status) ) 

#define IVGS2_SetFlashPlayStatus(This,flashName,status)	\
    ( (This)->lpVtbl -> SetFlashPlayStatus(This,flashName,status) ) 

#define IVGS2_DeleteFlashCtrl(This,flashName)	\
    ( (This)->lpVtbl -> DeleteFlashCtrl(This,flashName) ) 

#define IVGS2_MoveCamera(This,cameraName,x,y,z,stepCnt,coordEnum)	\
    ( (This)->lpVtbl -> MoveCamera(This,cameraName,x,y,z,stepCnt,coordEnum) ) 

#define IVGS2_MoveCurrentCamera(This,x,y,z,stepCnt,coordEnum)	\
    ( (This)->lpVtbl -> MoveCurrentCamera(This,x,y,z,stepCnt,coordEnum) ) 

#define IVGS2_ShowDefaultLogo(This,status)	\
    ( (This)->lpVtbl -> ShowDefaultLogo(This,status) ) 

#define IVGS2_SetControlStatus(This,status)	\
    ( (This)->lpVtbl -> SetControlStatus(This,status) ) 

#define IVGS2_SetCameraClip(This,cameraName,nearClip,farClip)	\
    ( (This)->lpVtbl -> SetCameraClip(This,cameraName,nearClip,farClip) ) 

#define IVGS2_SetCurrentCameraClip(This,nearClip,farClip)	\
    ( (This)->lpVtbl -> SetCurrentCameraClip(This,nearClip,farClip) ) 

#define IVGS2_DollyCurrentCamera(This,value,stepCnt)	\
    ( (This)->lpVtbl -> DollyCurrentCamera(This,value,stepCnt) ) 

#define IVGS2_DollyCamera(This,cameraName,value,stepCnt)	\
    ( (This)->lpVtbl -> DollyCamera(This,cameraName,value,stepCnt) ) 

#define IVGS2_SetBackgroundColor(This,red,green,blue)	\
    ( (This)->lpVtbl -> SetBackgroundColor(This,red,green,blue) ) 

#define IVGS2_PreloadN3DFile(This,strN3DFileName,N3DName)	\
    ( (This)->lpVtbl -> PreloadN3DFile(This,strN3DFileName,N3DName) ) 

#define IVGS2_GotoN3D(This,N3DName)	\
    ( (This)->lpVtbl -> GotoN3D(This,N3DName) ) 

#define IVGS2_SetCurrentCameraByName(This,camName)	\
    ( (This)->lpVtbl -> SetCurrentCameraByName(This,camName) ) 

#define IVGS2_PlayCameraKfr(This,camName,repeatCnt)	\
    ( (This)->lpVtbl -> PlayCameraKfr(This,camName,repeatCnt) ) 

#define IVGS2_SetCurrentCameraFrom(This,x,y,z)	\
    ( (This)->lpVtbl -> SetCurrentCameraFrom(This,x,y,z) ) 

#define IVGS2_SetCurrentCameraAt(This,x,y,z)	\
    ( (This)->lpVtbl -> SetCurrentCameraAt(This,x,y,z) ) 

#define IVGS2_StopCameraKfr(This,camName)	\
    ( (This)->lpVtbl -> StopCameraKfr(This,camName) ) 

#define IVGS2_SetServer(This,strServer)	\
    ( (This)->lpVtbl -> SetServer(This,strServer) ) 

#define IVGS2_SetModelClickable(This,modelName,clickable)	\
    ( (This)->lpVtbl -> SetModelClickable(This,modelName,clickable) ) 

#define IVGS2_CreateOverlayFromImage(This,overlayName,oriImageName,alphaImageName,alignMode,left,top,width,height)	\
    ( (This)->lpVtbl -> CreateOverlayFromImage(This,overlayName,oriImageName,alphaImageName,alignMode,left,top,width,height) ) 

#define IVGS2_DeleteOverlay(This,overlayName)	\
    ( (This)->lpVtbl -> DeleteOverlay(This,overlayName) ) 

#define IVGS2_SetOverlayOpacity(This,overlayName,opacity)	\
    ( (This)->lpVtbl -> SetOverlayOpacity(This,overlayName,opacity) ) 

#define IVGS2_CreateButtonFromImage(This,btnName,mouseUpImage,mouseUpImage_alpha,mouseOverImage,mouseOverImage_alpha,mouseDownImage,mouseDownImage_alpha,alignMode,x,y,width,height,callBackFunc)	\
    ( (This)->lpVtbl -> CreateButtonFromImage(This,btnName,mouseUpImage,mouseUpImage_alpha,mouseOverImage,mouseOverImage_alpha,mouseDownImage,mouseDownImage_alpha,alignMode,x,y,width,height,callBackFunc) ) 

#define IVGS2_DeleteButton(This,btnName)	\
    ( (This)->lpVtbl -> DeleteButton(This,btnName) ) 

#define IVGS2_TranslateModel(This,modelName,x,y,z,coord)	\
    ( (This)->lpVtbl -> TranslateModel(This,modelName,x,y,z,coord) ) 

#define IVGS2_SetEditCtrl_DollySpeed(This,dollySpeed)	\
    ( (This)->lpVtbl -> SetEditCtrl_DollySpeed(This,dollySpeed) ) 

#define IVGS2_SetEditCtrl_RosAxis(This,rosFlag)	\
    ( (This)->lpVtbl -> SetEditCtrl_RosAxis(This,rosFlag) ) 

#define IVGS2_SetEditCtrl_DollyDistance(This,nearDis,farDis)	\
    ( (This)->lpVtbl -> SetEditCtrl_DollyDistance(This,nearDis,farDis) ) 

#define IVGS2_SetEditCtrl_AutoPitchRoundRoll(This,status)	\
    ( (This)->lpVtbl -> SetEditCtrl_AutoPitchRoundRoll(This,status) ) 

#define IVGS2_SetEditCtrl_AutoParam(This,PitchSpeedValue,RoundSpeedValue,RollSpeedValue)	\
    ( (This)->lpVtbl -> SetEditCtrl_AutoParam(This,PitchSpeedValue,RoundSpeedValue,RollSpeedValue) ) 

#define IVGS2_SetEditCtrl_ManualParam(This,RoundSpeed,RollSpeed)	\
    ( (This)->lpVtbl -> SetEditCtrl_ManualParam(This,RoundSpeed,RollSpeed) ) 

#define IVGS2_MoveCurrentCameraTo(This,from_x,from_y,from_z,at_x,at_y,at_z,frameCnt,accType)	\
    ( (This)->lpVtbl -> MoveCurrentCameraTo(This,from_x,from_y,from_z,at_x,at_y,at_z,frameCnt,accType) ) 

#define IVGS2_SetFBL(This,fblValue)	\
    ( (This)->lpVtbl -> SetFBL(This,fblValue) ) 

#define IVGS2_SetOverlayWidth(This,overlayName,width)	\
    ( (This)->lpVtbl -> SetOverlayWidth(This,overlayName,width) ) 

#define IVGS2_SetOverlayHeight(This,overlayName,height)	\
    ( (This)->lpVtbl -> SetOverlayHeight(This,overlayName,height) ) 

#define IVGS2_SetOverlayAlignMode(This,overlayName,alignMode)	\
    ( (This)->lpVtbl -> SetOverlayAlignMode(This,overlayName,alignMode) ) 

#define IVGS2_SetOverlayRect(This,overlayName,left,top,width,height)	\
    ( (This)->lpVtbl -> SetOverlayRect(This,overlayName,left,top,width,height) ) 

#define IVGS2_MoveOverlay(This,overlayName,x,y)	\
    ( (This)->lpVtbl -> MoveOverlay(This,overlayName,x,y) ) 

#define IVGS2_MoveCurrentCameraFromTo(This,x,y,z)	\
    ( (This)->lpVtbl -> MoveCurrentCameraFromTo(This,x,y,z) ) 

#define IVGS2_MoveCurrentCameraAtTo(This,x,y,z)	\
    ( (This)->lpVtbl -> MoveCurrentCameraAtTo(This,x,y,z) ) 

#define IVGS2_MoveCurrentCameraFrom(This,x,y,z)	\
    ( (This)->lpVtbl -> MoveCurrentCameraFrom(This,x,y,z) ) 

#define IVGS2_MoveCurrentCameraAt(This,x,y,z)	\
    ( (This)->lpVtbl -> MoveCurrentCameraAt(This,x,y,z) ) 

#define IVGS2_LoadN3DFile(This,strN3DFileName,N3DName)	\
    ( (This)->lpVtbl -> LoadN3DFile(This,strN3DFileName,N3DName) ) 

#define IVGS2_ResetScene(This)	\
    ( (This)->lpVtbl -> ResetScene(This) ) 

#define IVGS2_SetModelMtrl(This,modelName,subMeshID,mtrlName)	\
    ( (This)->lpVtbl -> SetModelMtrl(This,modelName,subMeshID,mtrlName) ) 

#define IVGS2_SetMtrlOpacity(This,mtrlName,opacity)	\
    ( (This)->lpVtbl -> SetMtrlOpacity(This,mtrlName,opacity) ) 

#define IVGS2_SetCurrentCameraByIndex(This,camIndex)	\
    ( (This)->lpVtbl -> SetCurrentCameraByIndex(This,camIndex) ) 

#define IVGS2_SetWalkCtrl_ManualParam(This,moveSpeed,moveAcc,rotateSpeed,rotateAcc,height)	\
    ( (This)->lpVtbl -> SetWalkCtrl_ManualParam(This,moveSpeed,moveAcc,rotateSpeed,rotateAcc,height) ) 

#define IVGS2_SetModelMouseHint(This,modelName,bFlag)	\
    ( (This)->lpVtbl -> SetModelMouseHint(This,modelName,bFlag) ) 

#define IVGS2_EnableMouseDownCallback(This)	\
    ( (This)->lpVtbl -> EnableMouseDownCallback(This) ) 

#define IVGS2_DisableMouseDownCallback(This)	\
    ( (This)->lpVtbl -> DisableMouseDownCallback(This) ) 

#define IVGS2_EnableMouseUpCallback(This)	\
    ( (This)->lpVtbl -> EnableMouseUpCallback(This) ) 

#define IVGS2_DisableMouseUpCallback(This)	\
    ( (This)->lpVtbl -> DisableMouseUpCallback(This) ) 

#define IVGS2_EnableMouseDBClickCallback(This)	\
    ( (This)->lpVtbl -> EnableMouseDBClickCallback(This) ) 

#define IVGS2_DisableMouseDBClickCallback(This)	\
    ( (This)->lpVtbl -> DisableMouseDBClickCallback(This) ) 

#define IVGS2_EnableMouseMoveCallback(This)	\
    ( (This)->lpVtbl -> EnableMouseMoveCallback(This) ) 

#define IVGS2_DisableMouseMoveCallback(This)	\
    ( (This)->lpVtbl -> DisableMouseMoveCallback(This) ) 

#define IVGS2_SetModelMouseMtrl(This,modelName,mouseOverMtrl,mouseDownMtrl)	\
    ( (This)->lpVtbl -> SetModelMouseMtrl(This,modelName,mouseOverMtrl,mouseDownMtrl) ) 

#define IVGS2_SetModelOpacity(This,modelName,opacity)	\
    ( (This)->lpVtbl -> SetModelOpacity(This,modelName,opacity) ) 

#define IVGS2_CreateModelGroup(This,modelGroupName)	\
    ( (This)->lpVtbl -> CreateModelGroup(This,modelGroupName) ) 

#define IVGS2_CreateBumpWaterTexture(This,bumpWaterName,width,height,xSize,ySize,radius,speed,centerX,centerY)	\
    ( (This)->lpVtbl -> CreateBumpWaterTexture(This,bumpWaterName,width,height,xSize,ySize,radius,speed,centerX,centerY) ) 

#define IVGS2_RotateCurrentCamera(This,x,y,z,stepCnt,coordEnum)	\
    ( (This)->lpVtbl -> RotateCurrentCamera(This,x,y,z,stepCnt,coordEnum) ) 

#define IVGS2_RotateCamera(This,cameraName,x,y,z,stepCnt,coordEnum)	\
    ( (This)->lpVtbl -> RotateCamera(This,cameraName,x,y,z,stepCnt,coordEnum) ) 

#define IVGS2_FocusCurrentCameraTo(This,model1Name,model2Name,x,y,z,distance,stepCnt)	\
    ( (This)->lpVtbl -> FocusCurrentCameraTo(This,model1Name,model2Name,x,y,z,distance,stepCnt) ) 

#define IVGS2_HideAllModels(This)	\
    ( (This)->lpVtbl -> HideAllModels(This) ) 

#define IVGS2_ShowAllModels(This)	\
    ( (This)->lpVtbl -> ShowAllModels(This) ) 

#define IVGS2_GetModelHandleByName(This,modelName,modelHandle)	\
    ( (This)->lpVtbl -> GetModelHandleByName(This,modelName,modelHandle) ) 

#define IVGS2_RestoreModelMtrl(This,modelName)	\
    ( (This)->lpVtbl -> RestoreModelMtrl(This,modelName) ) 

#define IVGS2_RestoreAllModelsMtrl(This)	\
    ( (This)->lpVtbl -> RestoreAllModelsMtrl(This) ) 

#define IVGS2_CreateMtrl(This,mtrlName,red,green,blue,alpha,selfIllu)	\
    ( (This)->lpVtbl -> CreateMtrl(This,mtrlName,red,green,blue,alpha,selfIllu) ) 

#define IVGS2_SetMtrlColor(This,mtrlName,red,green,blue,alpha)	\
    ( (This)->lpVtbl -> SetMtrlColor(This,mtrlName,red,green,blue,alpha) ) 

#define IVGS2_DeleteMtrl(This,mtrlName)	\
    ( (This)->lpVtbl -> DeleteMtrl(This,mtrlName) ) 

#define IVGS2_SetMtrlIllumination(This,mtrlName,illumination)	\
    ( (This)->lpVtbl -> SetMtrlIllumination(This,mtrlName,illumination) ) 

#define IVGS2_MoveModel(This,modelName,x,y,z,coordEnum)	\
    ( (This)->lpVtbl -> MoveModel(This,modelName,x,y,z,coordEnum) ) 

#define IVGS2_MoveModelTo(This,modelName,x,y,z,frameCnt,accModel)	\
    ( (This)->lpVtbl -> MoveModelTo(This,modelName,x,y,z,frameCnt,accModel) ) 

#define IVGS2_RestoreModelOpacity(This,modelName)	\
    ( (This)->lpVtbl -> RestoreModelOpacity(This,modelName) ) 

#define IVGS2_RestoreAllModelsOpacity(This)	\
    ( (This)->lpVtbl -> RestoreAllModelsOpacity(This) ) 

#define IVGS2_PlayerGoForward(This)	\
    ( (This)->lpVtbl -> PlayerGoForward(This) ) 

#define IVGS2_PlayerGoBackward(This)	\
    ( (This)->lpVtbl -> PlayerGoBackward(This) ) 

#define IVGS2_PlayerGoLeftward(This)	\
    ( (This)->lpVtbl -> PlayerGoLeftward(This) ) 

#define IVGS2_PlayerGoRightward(This)	\
    ( (This)->lpVtbl -> PlayerGoRightward(This) ) 

#define IVGS2_PlayerStop(This)	\
    ( (This)->lpVtbl -> PlayerStop(This) ) 

#define IVGS2_PlayerTurnLeft(This)	\
    ( (This)->lpVtbl -> PlayerTurnLeft(This) ) 

#define IVGS2_PlayerTurnRight(This)	\
    ( (This)->lpVtbl -> PlayerTurnRight(This) ) 

#define IVGS2_PlayerTurnUp(This)	\
    ( (This)->lpVtbl -> PlayerTurnUp(This) ) 

#define IVGS2_PlayerTurnDown(This)	\
    ( (This)->lpVtbl -> PlayerTurnDown(This) ) 

#define IVGS2_PlayerZoomIn(This)	\
    ( (This)->lpVtbl -> PlayerZoomIn(This) ) 

#define IVGS2_PlayerZoomOut(This)	\
    ( (This)->lpVtbl -> PlayerZoomOut(This) ) 

#define IVGS2_PlayerGoUp(This)	\
    ( (This)->lpVtbl -> PlayerGoUp(This) ) 

#define IVGS2_PlayerGoDown(This)	\
    ( (This)->lpVtbl -> PlayerGoDown(This) ) 

#define IVGS2_SetModelColor(This,modelName,red,green,blue,alpha)	\
    ( (This)->lpVtbl -> SetModelColor(This,modelName,red,green,blue,alpha) ) 

#define IVGS2_SetModelFBLStatus(This,modelName,status)	\
    ( (This)->lpVtbl -> SetModelFBLStatus(This,modelName,status) ) 

#define IVGS2_SetBumpWaterSpeed(This,waterName,speed)	\
    ( (This)->lpVtbl -> SetBumpWaterSpeed(This,waterName,speed) ) 

#define IVGS2_SetBumpWaterRadius(This,waterName,radius)	\
    ( (This)->lpVtbl -> SetBumpWaterRadius(This,waterName,radius) ) 

#define IVGS2_SetSkyBoxModel(This,modelName,status)	\
    ( (This)->lpVtbl -> SetSkyBoxModel(This,modelName,status) ) 

#define IVGS2_SetCameraFrom(This,cameraName,x,y,z)	\
    ( (This)->lpVtbl -> SetCameraFrom(This,cameraName,x,y,z) ) 

#define IVGS2_SetCameraAt(This,cameraName,x,y,z)	\
    ( (This)->lpVtbl -> SetCameraAt(This,cameraName,x,y,z) ) 

#define IVGS2_SetCameraFov(This,cameraName,fov)	\
    ( (This)->lpVtbl -> SetCameraFov(This,cameraName,fov) ) 

#define IVGS2_SetCurrentCameraFov(This,fov)	\
    ( (This)->lpVtbl -> SetCurrentCameraFov(This,fov) ) 

#define IVGS2_CreateFSMotionBlur(This,blurCount)	\
    ( (This)->lpVtbl -> CreateFSMotionBlur(This,blurCount) ) 

#define IVGS2_SetFSMotionBlurPower(This,value)	\
    ( (This)->lpVtbl -> SetFSMotionBlurPower(This,value) ) 

#define IVGS2_StartFSMotionBlur(This)	\
    ( (This)->lpVtbl -> StartFSMotionBlur(This) ) 

#define IVGS2_StopFSMotionBlur(This)	\
    ( (This)->lpVtbl -> StopFSMotionBlur(This) ) 

#define IVGS2_SetCameraKFRFSMotionBlur(This,camName,startFrame,endFrame,power,status)	\
    ( (This)->lpVtbl -> SetCameraKFRFSMotionBlur(This,camName,startFrame,endFrame,power,status) ) 

#define IVGS2_SetFlashCurrentFrame(This,flashName,frameIndex)	\
    ( (This)->lpVtbl -> SetFlashCurrentFrame(This,flashName,frameIndex) ) 

#define IVGS2_SetCameraKfrCallBackFunc(This,camName,frameIndex,funcName)	\
    ( (This)->lpVtbl -> SetCameraKfrCallBackFunc(This,camName,frameIndex,funcName) ) 

#define IVGS2_CreateLensFlare(This,x,y,z,coverFlag)	\
    ( (This)->lpVtbl -> CreateLensFlare(This,x,y,z,coverFlag) ) 

#define IVGS2_CreateFog(This,fogMode,red,green,blue,density,start,end)	\
    ( (This)->lpVtbl -> CreateFog(This,fogMode,red,green,blue,density,start,end) ) 

#define IVGS2_SetFogMode(This,mode)	\
    ( (This)->lpVtbl -> SetFogMode(This,mode) ) 

#define IVGS2_SetFogColor(This,red,blue,green)	\
    ( (This)->lpVtbl -> SetFogColor(This,red,blue,green) ) 

#define IVGS2_SetFogDensity(This,density)	\
    ( (This)->lpVtbl -> SetFogDensity(This,density) ) 

#define IVGS2_SetFogField(This,start,end)	\
    ( (This)->lpVtbl -> SetFogField(This,start,end) ) 

#define IVGS2_SetFogRenderStatus(This,status)	\
    ( (This)->lpVtbl -> SetFogRenderStatus(This,status) ) 

#define IVGS2_SetFilter(This,overlayName,status,opacity)	\
    ( (This)->lpVtbl -> SetFilter(This,overlayName,status,opacity) ) 

#define IVGS2_SetModelMouseColor(This,modelName,mouseOver_Red,mouseOver_Green,mouseOver_Blue,mouseOver_Alpha,mouseClick_Red,mouseClick_Green,mouseClick_Blue,mouseClick_Alpha)	\
    ( (This)->lpVtbl -> SetModelMouseColor(This,modelName,mouseOver_Red,mouseOver_Green,mouseOver_Blue,mouseOver_Alpha,mouseClick_Red,mouseClick_Green,mouseClick_Blue,mouseClick_Alpha) ) 

#define IVGS2_SetMouseMoveEventTimer(This,timeStep)	\
    ( (This)->lpVtbl -> SetMouseMoveEventTimer(This,timeStep) ) 

#define IVGS2_SetNavigaterVisibility(This,navName,visible)	\
    ( (This)->lpVtbl -> SetNavigaterVisibility(This,navName,visible) ) 

#define IVGS2_RestoreModelColor(This,modelName)	\
    ( (This)->lpVtbl -> RestoreModelColor(This,modelName) ) 

#define IVGS2_RestoreAllModelsColor(This)	\
    ( (This)->lpVtbl -> RestoreAllModelsColor(This) ) 

#define IVGS2_SetAlphaTestStatus(This,mtrlName,status)	\
    ( (This)->lpVtbl -> SetAlphaTestStatus(This,mtrlName,status) ) 

#define IVGS2_SetZOneModel(This,modelName,status)	\
    ( (This)->lpVtbl -> SetZOneModel(This,modelName,status) ) 

#define IVGS2_SetLensFlareVisibility(This,status)	\
    ( (This)->lpVtbl -> SetLensFlareVisibility(This,status) ) 

#define IVGS2_SetCollisionDistance(This,distance)	\
    ( (This)->lpVtbl -> SetCollisionDistance(This,distance) ) 

#define IVGS2_CreateStaticCubeMap(This,name,frontImage,backImage,leftImage,rightImage,topImage,bottomImge,size)	\
    ( (This)->lpVtbl -> CreateStaticCubeMap(This,name,frontImage,backImage,leftImage,rightImage,topImage,bottomImge,size) ) 

#define IVGS2_SetMtrlCubeMap(This,mtrlName,cubeMapName,power)	\
    ( (This)->lpVtbl -> SetMtrlCubeMap(This,mtrlName,cubeMapName,power) ) 

#define IVGS2_SetLightColor(This,lightName,red,green,blue)	\
    ( (This)->lpVtbl -> SetLightColor(This,lightName,red,green,blue) ) 

#define IVGS2_SetLightPower(This,lightName,power)	\
    ( (This)->lpVtbl -> SetLightPower(This,lightName,power) ) 

#define IVGS2_SetLightAmbient(This,lightName,red,green,blue)	\
    ( (This)->lpVtbl -> SetLightAmbient(This,lightName,red,green,blue) ) 

#define IVGS2_SetLightPosition(This,lightName,x,y,z)	\
    ( (This)->lpVtbl -> SetLightPosition(This,lightName,x,y,z) ) 

#define IVGS2_SetLightRange(This,lightName,range)	\
    ( (This)->lpVtbl -> SetLightRange(This,lightName,range) ) 

#define IVGS2_SetLightAttenuation(This,lightName,attenuation0,attenuation1,attenuation2)	\
    ( (This)->lpVtbl -> SetLightAttenuation(This,lightName,attenuation0,attenuation1,attenuation2) ) 

#define IVGS2_SetLightStatus(This,lightName,status)	\
    ( (This)->lpVtbl -> SetLightStatus(This,lightName,status) ) 

#define IVGS2_CreateMovieTexture(This,texName,moviePath,movieType,withAudio,repeatCnt,mtrlName)	\
    ( (This)->lpVtbl -> CreateMovieTexture(This,texName,moviePath,movieType,withAudio,repeatCnt,mtrlName) ) 

#define IVGS2_SetMovieMtrl(This,mtrlName,movieTexName)	\
    ( (This)->lpVtbl -> SetMovieMtrl(This,mtrlName,movieTexName) ) 

#define IVGS2_SetModelKfrRepeatCnt(This,modelName,kfrRepeatCnt)	\
    ( (This)->lpVtbl -> SetModelKfrRepeatCnt(This,modelName,kfrRepeatCnt) ) 

#define IVGS2_SetAllModelKfrRepeatCnt(This,kfrRepeatCnt)	\
    ( (This)->lpVtbl -> SetAllModelKfrRepeatCnt(This,kfrRepeatCnt) ) 

#define IVGS2_CreateBackdropFromImage(This,backdropName,oriImageName,alphaImageName,alignMode,left,top,width,height)	\
    ( (This)->lpVtbl -> CreateBackdropFromImage(This,backdropName,oriImageName,alphaImageName,alignMode,left,top,width,height) ) 

#define IVGS2_CreateSpecularMap(This,name,imageName,width,height,mipLevel)	\
    ( (This)->lpVtbl -> CreateSpecularMap(This,name,imageName,width,height,mipLevel) ) 

#define IVGS2_SetSpecularMapMtrl(This,mtrlName,specularMapName)	\
    ( (This)->lpVtbl -> SetSpecularMapMtrl(This,mtrlName,specularMapName) ) 

#define IVGS2_CreateCamera(This,name,fromX,fromY,fromZ,atX,atY,atZ,fov,nearClip,farClip)	\
    ( (This)->lpVtbl -> CreateCamera(This,name,fromX,fromY,fromZ,atX,atY,atZ,fov,nearClip,farClip) ) 

#define IVGS2_DeleteModel(This,modelName)	\
    ( (This)->lpVtbl -> DeleteModel(This,modelName) ) 

#define IVGS2_SetCanNavigateByPointer(This,navName,status)	\
    ( (This)->lpVtbl -> SetCanNavigateByPointer(This,navName,status) ) 

#define IVGS2_SetPlayerCanMove(This,status)	\
    ( (This)->lpVtbl -> SetPlayerCanMove(This,status) ) 

#define IVGS2_SetPlayerCanRos(This,status)	\
    ( (This)->lpVtbl -> SetPlayerCanRos(This,status) ) 

#define IVGS2_SetPlayerCanZoom(This,status)	\
    ( (This)->lpVtbl -> SetPlayerCanZoom(This,status) ) 

#define IVGS2_SetPlayerCanLift(This,status)	\
    ( (This)->lpVtbl -> SetPlayerCanLift(This,status) ) 

#define IVGS2_DeleteMovieTexture(This,mtrlName,movieTexName)	\
    ( (This)->lpVtbl -> DeleteMovieTexture(This,mtrlName,movieTexName) ) 

#define IVGS2_SetModelKfrPlayStatus(This,name,kfrPlay)	\
    ( (This)->lpVtbl -> SetModelKfrPlayStatus(This,name,kfrPlay) ) 

#define IVGS2_PlayModelKfr(This,modelName,startFrame,endFrame,repeatCnt)	\
    ( (This)->lpVtbl -> PlayModelKfr(This,modelName,startFrame,endFrame,repeatCnt) ) 

#define IVGS2_SetAllModelsKfrPlayStatus(This,bKfrPlay)	\
    ( (This)->lpVtbl -> SetAllModelsKfrPlayStatus(This,bKfrPlay) ) 

#define IVGS2_SetModelEventOnMouseMove(This,status)	\
    ( (This)->lpVtbl -> SetModelEventOnMouseMove(This,status) ) 

#define IVGS2_SetButtonVisibility(This,btnName,visible)	\
    ( (This)->lpVtbl -> SetButtonVisibility(This,btnName,visible) ) 

#define IVGS2_SetAntiAliasLevel(This,level)	\
    ( (This)->lpVtbl -> SetAntiAliasLevel(This,level) ) 

#define IVGS2_SetMtrlSpecularCubeMap(This,mtrlName,specularCubeMapName)	\
    ( (This)->lpVtbl -> SetMtrlSpecularCubeMap(This,mtrlName,specularCubeMapName) ) 

#define IVGS2_SetButtonActive(This,name,bActive)	\
    ( (This)->lpVtbl -> SetButtonActive(This,name,bActive) ) 

#define IVGS2_CreateSectionMap(This,sectioMapName,width,height,sliceModelName,leftTopX,leftTopY,leftTopZ,rightTopX,rightTopY,rightTopZ,leftBottomX,leftBottomY,leftBottomZ,rightBottomX,rightBottomY,rightBottomZ,imageName)	\
    ( (This)->lpVtbl -> CreateSectionMap(This,sectioMapName,width,height,sliceModelName,leftTopX,leftTopY,leftTopZ,rightTopX,rightTopY,rightTopZ,leftBottomX,leftBottomY,leftBottomZ,rightBottomX,rightBottomY,rightBottomZ,imageName) ) 

#define IVGS2_ExecuteSlice(This,sectionMapName)	\
    ( (This)->lpVtbl -> ExecuteSlice(This,sectionMapName) ) 

#define IVGS2_SetOverlayVisibility(This,overlayName,visible)	\
    ( (This)->lpVtbl -> SetOverlayVisibility(This,overlayName,visible) ) 

#define IVGS2_CreateAudio(This,name,audioFile,repeatCnt)	\
    ( (This)->lpVtbl -> CreateAudio(This,name,audioFile,repeatCnt) ) 

#define IVGS2_DeleteAudioByName(This,name)	\
    ( (This)->lpVtbl -> DeleteAudioByName(This,name) ) 

#define IVGS2_DeleteAudioByIndex(This,index)	\
    ( (This)->lpVtbl -> DeleteAudioByIndex(This,index) ) 

#define IVGS2_DeleteAllAudio(This)	\
    ( (This)->lpVtbl -> DeleteAllAudio(This) ) 

#define IVGS2_SetAudioFile(This,audioName,audioFile)	\
    ( (This)->lpVtbl -> SetAudioFile(This,audioName,audioFile) ) 

#define IVGS2_SetAudioRepeatCount(This,audioName,repeatCnt)	\
    ( (This)->lpVtbl -> SetAudioRepeatCount(This,audioName,repeatCnt) ) 

#define IVGS2_PlayAudio(This,audioName)	\
    ( (This)->lpVtbl -> PlayAudio(This,audioName) ) 

#define IVGS2_PauseAudio(This,audioName)	\
    ( (This)->lpVtbl -> PauseAudio(This,audioName) ) 

#define IVGS2_StopAudio(This,audioName)	\
    ( (This)->lpVtbl -> StopAudio(This,audioName) ) 

#define IVGS2_RewindAudio(This,audioName)	\
    ( (This)->lpVtbl -> RewindAudio(This,audioName) ) 

#define IVGS2_DeleteModelGroup(This,name)	\
    ( (This)->lpVtbl -> DeleteModelGroup(This,name) ) 

#define IVGS2_DeleteModelGroupByIndex(This,index)	\
    ( (This)->lpVtbl -> DeleteModelGroupByIndex(This,index) ) 

#define IVGS2_DeleteAllModelGroup(This)	\
    ( (This)->lpVtbl -> DeleteAllModelGroup(This) ) 

#define IVGS2_AddModelToModelGroup(This,modelGroupName,modelName)	\
    ( (This)->lpVtbl -> AddModelToModelGroup(This,modelGroupName,modelName) ) 

#define IVGS2_DeleteModelFromModelGroup(This,modelGroupName,modelName)	\
    ( (This)->lpVtbl -> DeleteModelFromModelGroup(This,modelGroupName,modelName) ) 

#define IVGS2_ClearModelGroup(This,modelGroupName)	\
    ( (This)->lpVtbl -> ClearModelGroup(This,modelGroupName) ) 

#define IVGS2_SetModelMouseHintType(This,modelName,type)	\
    ( (This)->lpVtbl -> SetModelMouseHintType(This,modelName,type) ) 

#define IVGS2_SetLightKfrPlayStatus(This,lightName,bFlag)	\
    ( (This)->lpVtbl -> SetLightKfrPlayStatus(This,lightName,bFlag) ) 

#define IVGS2_SetAllLightKfrPlayStatus(This,bFlag)	\
    ( (This)->lpVtbl -> SetAllLightKfrPlayStatus(This,bFlag) ) 

#define IVGS2_SetLightKfrRepeatCnt(This,lightName,repeatCnt)	\
    ( (This)->lpVtbl -> SetLightKfrRepeatCnt(This,lightName,repeatCnt) ) 

#define IVGS2_SetAllLightKfrRepeatCnt(This,repeatCnt)	\
    ( (This)->lpVtbl -> SetAllLightKfrRepeatCnt(This,repeatCnt) ) 

#define IVGS2_SetAllModelKfrPlayStatus(This,kfrPlay)	\
    ( (This)->lpVtbl -> SetAllModelKfrPlayStatus(This,kfrPlay) ) 

#define IVGS2_SetModelPosition(This,modelName,x,y,z)	\
    ( (This)->lpVtbl -> SetModelPosition(This,modelName,x,y,z) ) 

#define IVGS2_SetModelRotation(This,modelName,x,y,z)	\
    ( (This)->lpVtbl -> SetModelRotation(This,modelName,x,y,z) ) 

#define IVGS2_AddNavigaterPointer(This,navigaterName,navPointerName,navPointerType,camOrModelName,navPointerOriImage,navPointerAlphaImage,width,height)	\
    ( (This)->lpVtbl -> AddNavigaterPointer(This,navigaterName,navPointerName,navPointerType,camOrModelName,navPointerOriImage,navPointerAlphaImage,width,height) ) 

#define IVGS2_SetNavigaterPointerVisibility(This,navigaterName,navPointerName,status)	\
    ( (This)->lpVtbl -> SetNavigaterPointerVisibility(This,navigaterName,navPointerName,status) ) 

#define IVGS2_DeleteNavigaterPointer(This,navigaterName,navPointerName)	\
    ( (This)->lpVtbl -> DeleteNavigaterPointer(This,navigaterName,navPointerName) ) 

#define IVGS2_PauseCameraKfr(This,camName)	\
    ( (This)->lpVtbl -> PauseCameraKfr(This,camName) ) 

#define IVGS2_ResumeCameraKfr(This,cameraName)	\
    ( (This)->lpVtbl -> ResumeCameraKfr(This,cameraName) ) 

#define IVGS2_SetNavigaterPointerObject(This,navName,navPointerName,objType,objName)	\
    ( (This)->lpVtbl -> SetNavigaterPointerObject(This,navName,navPointerName,objType,objName) ) 

#define IVGS2_SetPlayerZoomSpeed(This,speed)	\
    ( (This)->lpVtbl -> SetPlayerZoomSpeed(This,speed) ) 

#define IVGS2_SetOverlayFullScreen(This,OverlayName,xFull,yFull)	\
    ( (This)->lpVtbl -> SetOverlayFullScreen(This,OverlayName,xFull,yFull) ) 

#define IVGS2_PushOverlayFront(This,OverlayName)	\
    ( (This)->lpVtbl -> PushOverlayFront(This,OverlayName) ) 

#define IVGS2_PushOverlayBack(This,overlayName)	\
    ( (This)->lpVtbl -> PushOverlayBack(This,overlayName) ) 

#define IVGS2_DeleteAllModel(This)	\
    ( (This)->lpVtbl -> DeleteAllModel(This) ) 

#define IVGS2_PlayKeyFrame(This)	\
    ( (This)->lpVtbl -> PlayKeyFrame(This) ) 

#define IVGS2_PauseKeyFrame(This)	\
    ( (This)->lpVtbl -> PauseKeyFrame(This) ) 

#define IVGS2_StopKeyFrame(This)	\
    ( (This)->lpVtbl -> StopKeyFrame(This) ) 

#define IVGS2_SetKeyFrameRepeatCount(This,repeatCnt)	\
    ( (This)->lpVtbl -> SetKeyFrameRepeatCount(This,repeatCnt) ) 

#define IVGS2_GotoFrame(This,frame)	\
    ( (This)->lpVtbl -> GotoFrame(This,frame) ) 

#define IVGS2_SetKeyFramePlaySpeed(This,fps)	\
    ( (This)->lpVtbl -> SetKeyFramePlaySpeed(This,fps) ) 

#define IVGS2_CreateFSGlow(This,BloomValue,BlurValue)	\
    ( (This)->lpVtbl -> CreateFSGlow(This,BloomValue,BlurValue) ) 

#define IVGS2_SetFSGlowValue(This,BloomValue,BlurValue)	\
    ( (This)->lpVtbl -> SetFSGlowValue(This,BloomValue,BlurValue) ) 

#define IVGS2_StartFSGlow(This)	\
    ( (This)->lpVtbl -> StartFSGlow(This) ) 

#define IVGS2_EndFSGlow(This)	\
    ( (This)->lpVtbl -> EndFSGlow(This) ) 

#define IVGS2_SetMtrlSpecular(This,mtrlName,red,green,blue)	\
    ( (This)->lpVtbl -> SetMtrlSpecular(This,mtrlName,red,green,blue) ) 

#define IVGS2_SetMtrlAmbient(This,mtrlName,red,green,blue)	\
    ( (This)->lpVtbl -> SetMtrlAmbient(This,mtrlName,red,green,blue) ) 

#define IVGS2_SetMtrlSpecularLevel(This,mtrlName,level)	\
    ( (This)->lpVtbl -> SetMtrlSpecularLevel(This,mtrlName,level) ) 

#define IVGS2_SetMtrlSpecularPower(This,mtrlName,power)	\
    ( (This)->lpVtbl -> SetMtrlSpecularPower(This,mtrlName,power) ) 

#define IVGS2_PlayKeyFrame1(This,startFrame,endFrame)	\
    ( (This)->lpVtbl -> PlayKeyFrame1(This,startFrame,endFrame) ) 

#define IVGS2_SetCameraKfrPlayStatus(This,camName,status)	\
    ( (This)->lpVtbl -> SetCameraKfrPlayStatus(This,camName,status) ) 

#define IVGS2_SetAllModelVisibility(This,status)	\
    ( (This)->lpVtbl -> SetAllModelVisibility(This,status) ) 

#define IVGS2_SetAllCameraKfrPlayStatus(This,status)	\
    ( (This)->lpVtbl -> SetAllCameraKfrPlayStatus(This,status) ) 

#define IVGS2_SetCameraKfrRepeatCnt(This,camName,repeatCnt)	\
    ( (This)->lpVtbl -> SetCameraKfrRepeatCnt(This,camName,repeatCnt) ) 

#define IVGS2_SetAllCameraKfrRepeatCnt(This,repeatCnt)	\
    ( (This)->lpVtbl -> SetAllCameraKfrRepeatCnt(This,repeatCnt) ) 

#define IVGS2_SetDynamicTexPlaySpeed(This,UseDyTexMtrlName,texLayerIdx,fps)	\
    ( (This)->lpVtbl -> SetDynamicTexPlaySpeed(This,UseDyTexMtrlName,texLayerIdx,fps) ) 

#define IVGS2_SetModelPositionByPivot(This,modelName,x,y,z)	\
    ( (This)->lpVtbl -> SetModelPositionByPivot(This,modelName,x,y,z) ) 

#define IVGS2_SetDynamicUVStatus(This,modelName,status)	\
    ( (This)->lpVtbl -> SetDynamicUVStatus(This,modelName,status) ) 

#define IVGS2_MoveFlash(This,flashName,x,y)	\
    ( (This)->lpVtbl -> MoveFlash(This,flashName,x,y) ) 

#define IVGS2_SetOverlayMtrl(This,overlayName,mtrlName)	\
    ( (This)->lpVtbl -> SetOverlayMtrl(This,overlayName,mtrlName) ) 

#define IVGS2_SetZoneTriggerModelCallBackFunc(This,modelName,funcName)	\
    ( (This)->lpVtbl -> SetZoneTriggerModelCallBackFunc(This,modelName,funcName) ) 

#define IVGS2_SetAlphaTestLevel(This,level)	\
    ( (This)->lpVtbl -> SetAlphaTestLevel(This,level) ) 

#define IVGS2_SetButtonOpacity(This,buttonName,opacity)	\
    ( (This)->lpVtbl -> SetButtonOpacity(This,buttonName,opacity) ) 

#define IVGS2_CurCamGotoFrame(This,frame)	\
    ( (This)->lpVtbl -> CurCamGotoFrame(This,frame) ) 

#define IVGS2_SetSceneEditMode(This,EditMode)	\
    ( (This)->lpVtbl -> SetSceneEditMode(This,EditMode) ) 

#define IVGS2_CloneModel(This,oriModelName,desModelName,ID1,ID2,posX,posY,posZ)	\
    ( (This)->lpVtbl -> CloneModel(This,oriModelName,desModelName,ID1,ID2,posX,posY,posZ) ) 

#define IVGS2_get_theModelPosX(This,pVal)	\
    ( (This)->lpVtbl -> get_theModelPosX(This,pVal) ) 

#define IVGS2_get_theModelPosY(This,pVal)	\
    ( (This)->lpVtbl -> get_theModelPosY(This,pVal) ) 

#define IVGS2_get_theModelPosZ(This,pVal)	\
    ( (This)->lpVtbl -> get_theModelPosZ(This,pVal) ) 

#define IVGS2_get_theModelScreenPosX(This,pVal)	\
    ( (This)->lpVtbl -> get_theModelScreenPosX(This,pVal) ) 

#define IVGS2_get_theModelScreenPosY(This,pVal)	\
    ( (This)->lpVtbl -> get_theModelScreenPosY(This,pVal) ) 

#define IVGS2_GetInfoFromModel(This,modelName)	\
    ( (This)->lpVtbl -> GetInfoFromModel(This,modelName) ) 

#define IVGS2_CreateNewText(This,textString)	\
    ( (This)->lpVtbl -> CreateNewText(This,textString) ) 

#define IVGS2_get_theTextHandle(This,pVal)	\
    ( (This)->lpVtbl -> get_theTextHandle(This,pVal) ) 

#define IVGS2_SetTextPosition(This,textHandle,x,y)	\
    ( (This)->lpVtbl -> SetTextPosition(This,textHandle,x,y) ) 

#define IVGS2_SetTextColor(This,textHandle,red,green,blue,alpha)	\
    ( (This)->lpVtbl -> SetTextColor(This,textHandle,red,green,blue,alpha) ) 

#define IVGS2_DeleteText(This,textHandle)	\
    ( (This)->lpVtbl -> DeleteText(This,textHandle) ) 

#define IVGS2_GetInfoFromCamera(This,cameraName)	\
    ( (This)->lpVtbl -> GetInfoFromCamera(This,cameraName) ) 

#define IVGS2_get_theCamFromPosX(This,pVal)	\
    ( (This)->lpVtbl -> get_theCamFromPosX(This,pVal) ) 

#define IVGS2_get_theCamFromPosY(This,pVal)	\
    ( (This)->lpVtbl -> get_theCamFromPosY(This,pVal) ) 

#define IVGS2_get_theCamFromPosZ(This,pVal)	\
    ( (This)->lpVtbl -> get_theCamFromPosZ(This,pVal) ) 

#define IVGS2_get_theCamAtPosX(This,pVal)	\
    ( (This)->lpVtbl -> get_theCamAtPosX(This,pVal) ) 

#define IVGS2_get_theCamAtPosY(This,pVal)	\
    ( (This)->lpVtbl -> get_theCamAtPosY(This,pVal) ) 

#define IVGS2_get_theCamAtPosZ(This,pVal)	\
    ( (This)->lpVtbl -> get_theCamAtPosZ(This,pVal) ) 

#define IVGS2_SetTextString(This,textHandle,textString)	\
    ( (This)->lpVtbl -> SetTextString(This,textHandle,textString) ) 

#define IVGS2_LoadScene(This,SceneName,ParentSceneName,N3DFileURL,m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44)	\
    ( (This)->lpVtbl -> LoadScene(This,SceneName,ParentSceneName,N3DFileURL,m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44) ) 

#define IVGS2_DeleteScene(This,SceneName)	\
    ( (This)->lpVtbl -> DeleteScene(This,SceneName) ) 

#define IVGS2_CloneScene(This,destSceneName,srcSceneName,ParentSceneName,m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44)	\
    ( (This)->lpVtbl -> CloneScene(This,destSceneName,srcSceneName,ParentSceneName,m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44) ) 

#define IVGS2_CreateRTFTexture(This,TextureName,refPlaneName,height,width,power)	\
    ( (This)->lpVtbl -> CreateRTFTexture(This,TextureName,refPlaneName,height,width,power) ) 

#define IVGS2_AddModelToRTFTexture(This,RTFTextureName,ModelName)	\
    ( (This)->lpVtbl -> AddModelToRTFTexture(This,RTFTextureName,ModelName) ) 

#define IVGS2_CreateDynamicCubeMap(This,cubeMapame,centerModelName,size,power)	\
    ( (This)->lpVtbl -> CreateDynamicCubeMap(This,cubeMapame,centerModelName,size,power) ) 

#define IVGS2_AddDynamicCubeMapReflectionModel(This,cubeMapName,ModelName)	\
    ( (This)->lpVtbl -> AddDynamicCubeMapReflectionModel(This,cubeMapName,ModelName) ) 

#define IVGS2_DeleteDynamicCubeMapReflectionModel(This,cubeMapName,modelName)	\
    ( (This)->lpVtbl -> DeleteDynamicCubeMapReflectionModel(This,cubeMapName,modelName) ) 

#define IVGS2_CreateRealWater(This,name,x,y,z,gridX,gridY)	\
    ( (This)->lpVtbl -> CreateRealWater(This,name,x,y,z,gridX,gridY) ) 

#define IVGS2_AddRealWaterRTTModel(This,RealWaterName,ModelName)	\
    ( (This)->lpVtbl -> AddRealWaterRTTModel(This,RealWaterName,ModelName) ) 

#define IVGS2_AddRealWaterRTFModel(This,RealWaterName,ModelName)	\
    ( (This)->lpVtbl -> AddRealWaterRTFModel(This,RealWaterName,ModelName) ) 

#define IVGS2_SetRealWaterCubeMap(This,RealWaterName,CubeMapName)	\
    ( (This)->lpVtbl -> SetRealWaterCubeMap(This,RealWaterName,CubeMapName) ) 

#define IVGS2_CreateColorEffect(This)	\
    ( (This)->lpVtbl -> CreateColorEffect(This) ) 

#define IVGS2_SetColorEffect(This,brightness,contrast,red,green,blue)	\
    ( (This)->lpVtbl -> SetColorEffect(This,brightness,contrast,red,green,blue) ) 

#define IVGS2_SetTextureImageFromURL(This,mtrlName,texLayerIdx,imageURL)	\
    ( (This)->lpVtbl -> SetTextureImageFromURL(This,mtrlName,texLayerIdx,imageURL) ) 

#define IVGS2_SetEditCtrlMaxUpDownAngle(This,upAngle,downAngle)	\
    ( (This)->lpVtbl -> SetEditCtrlMaxUpDownAngle(This,upAngle,downAngle) ) 

#define IVGS2_SetMtrlTexture(This,mtrlName,textureLayerIndex,textureType,textureName)	\
    ( (This)->lpVtbl -> SetMtrlTexture(This,mtrlName,textureLayerIndex,textureType,textureName) ) 

#define IVGS2_AddFlashCtrlSemaphore(This,flashName,SemaphoreName,SemaphoreFunc)	\
    ( (This)->lpVtbl -> AddFlashCtrlSemaphore(This,flashName,SemaphoreName,SemaphoreFunc) ) 

#define IVGS2_DeleteFlashCtrlSemaphore(This,flashName,SemaphoreName)	\
    ( (This)->lpVtbl -> DeleteFlashCtrlSemaphore(This,flashName,SemaphoreName) ) 

#define IVGS2_get_theFlashVar(This,pVal)	\
    ( (This)->lpVtbl -> get_theFlashVar(This,pVal) ) 

#define IVGS2_GetFlashVaribleInfo(This,flashName,varibleName)	\
    ( (This)->lpVtbl -> GetFlashVaribleInfo(This,flashName,varibleName) ) 

#define IVGS2_SetFlashVarible(This,flashName,varibleName,varibleValue)	\
    ( (This)->lpVtbl -> SetFlashVarible(This,flashName,varibleName,varibleValue) ) 

#define IVGS2_LoadN3DScene(This,SceneName,ParentSceneName,N3DFileURL,posX,posY,posZ,rotX,rotY,rotZ,scaleX,scaleY,scaleZ)	\
    ( (This)->lpVtbl -> LoadN3DScene(This,SceneName,ParentSceneName,N3DFileURL,posX,posY,posZ,rotX,rotY,rotZ,scaleX,scaleY,scaleZ) ) 

#define IVGS2_GetInfoFromSceneResource(This,SceneName)	\
    ( (This)->lpVtbl -> GetInfoFromSceneResource(This,SceneName) ) 

#define IVGS2_get_theScenePosX(This,pVal)	\
    ( (This)->lpVtbl -> get_theScenePosX(This,pVal) ) 

#define IVGS2_get_theScenePosY(This,pVal)	\
    ( (This)->lpVtbl -> get_theScenePosY(This,pVal) ) 

#define IVGS2_get_theScenePosZ(This,pVal)	\
    ( (This)->lpVtbl -> get_theScenePosZ(This,pVal) ) 

#define IVGS2_get_theSceneRotX(This,pVal)	\
    ( (This)->lpVtbl -> get_theSceneRotX(This,pVal) ) 

#define IVGS2_get_theSceneRotY(This,pVal)	\
    ( (This)->lpVtbl -> get_theSceneRotY(This,pVal) ) 

#define IVGS2_get_theSceneRotZ(This,pVal)	\
    ( (This)->lpVtbl -> get_theSceneRotZ(This,pVal) ) 

#define IVGS2_get_theSceneScaleX(This,pVal)	\
    ( (This)->lpVtbl -> get_theSceneScaleX(This,pVal) ) 

#define IVGS2_get_theSceneScaleY(This,pVal)	\
    ( (This)->lpVtbl -> get_theSceneScaleY(This,pVal) ) 

#define IVGS2_get_theSceneScaleZ(This,pVal)	\
    ( (This)->lpVtbl -> get_theSceneScaleZ(This,pVal) ) 

#define IVGS2_SetSceneResColor(This,SceneName,red,green,blue,alpha)	\
    ( (This)->lpVtbl -> SetSceneResColor(This,SceneName,red,green,blue,alpha) ) 

#define IVGS2_SetButtonRect(This,buttonName,x,y,width,height)	\
    ( (This)->lpVtbl -> SetButtonRect(This,buttonName,x,y,width,height) ) 

#define IVGS2_SetAddtionalKeys(This,left,right,up,down)	\
    ( (This)->lpVtbl -> SetAddtionalKeys(This,left,right,up,down) ) 

#define IVGS2_SetAddtionalKeysStatus(This,bEnable)	\
    ( (This)->lpVtbl -> SetAddtionalKeysStatus(This,bEnable) ) 

#define IVGS2_SetSubBrowserType(This,subType)	\
    ( (This)->lpVtbl -> SetSubBrowserType(This,subType) ) 

#define IVGS2_get_wbType(This,pVal)	\
    ( (This)->lpVtbl -> get_wbType(This,pVal) ) 

#define IVGS2_put_wbType(This,newVal)	\
    ( (This)->lpVtbl -> put_wbType(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelMouseColor_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    INT mouseOver_Red,
    INT mouseOver_Green,
    INT mouseOver_Blue,
    INT mouseOver_Alpha,
    INT mouseClick_Red,
    INT mouseClick_Green,
    INT mouseClick_Blue,
    INT mouseClick_Alpha);


void __RPC_STUB IVGS2_SetModelMouseColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMouseMoveEventTimer_Proxy( 
    IVGS2 * This,
    INT timeStep);


void __RPC_STUB IVGS2_SetMouseMoveEventTimer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetNavigaterVisibility_Proxy( 
    IVGS2 * This,
    BSTR navName,
    BOOL visible);


void __RPC_STUB IVGS2_SetNavigaterVisibility_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_RestoreModelColor_Proxy( 
    IVGS2 * This,
    BSTR modelName);


void __RPC_STUB IVGS2_RestoreModelColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_RestoreAllModelsColor_Proxy( 
    IVGS2 * This);


void __RPC_STUB IVGS2_RestoreAllModelsColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAlphaTestStatus_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BOOL status);


void __RPC_STUB IVGS2_SetAlphaTestStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetZOneModel_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    BOOL status);


void __RPC_STUB IVGS2_SetZOneModel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLensFlareVisibility_Proxy( 
    IVGS2 * This,
    BOOL status);


void __RPC_STUB IVGS2_SetLensFlareVisibility_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetCollisionDistance_Proxy( 
    IVGS2 * This,
    FLOAT distance);


void __RPC_STUB IVGS2_SetCollisionDistance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateStaticCubeMap_Proxy( 
    IVGS2 * This,
    BSTR name,
    BSTR frontImage,
    BSTR backImage,
    BSTR leftImage,
    BSTR rightImage,
    BSTR topImage,
    BSTR bottomImge,
    INT size);


void __RPC_STUB IVGS2_CreateStaticCubeMap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlCubeMap_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BSTR cubeMapName,
    FLOAT power);


void __RPC_STUB IVGS2_SetMtrlCubeMap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightColor_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    INT red,
    INT green,
    INT blue);


void __RPC_STUB IVGS2_SetLightColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightPower_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    FLOAT power);


void __RPC_STUB IVGS2_SetLightPower_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightAmbient_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    INT red,
    INT green,
    INT blue);


void __RPC_STUB IVGS2_SetLightAmbient_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightPosition_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    FLOAT x,
    FLOAT y,
    FLOAT z);


void __RPC_STUB IVGS2_SetLightPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightRange_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    FLOAT range);


void __RPC_STUB IVGS2_SetLightRange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightAttenuation_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    FLOAT attenuation0,
    FLOAT attenuation1,
    FLOAT attenuation2);


void __RPC_STUB IVGS2_SetLightAttenuation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightStatus_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    BOOL status);


void __RPC_STUB IVGS2_SetLightStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateMovieTexture_Proxy( 
    IVGS2 * This,
    BSTR texName,
    BSTR moviePath,
    INT movieType,
    BOOL withAudio,
    INT repeatCnt,
    BSTR mtrlName);


void __RPC_STUB IVGS2_CreateMovieTexture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMovieMtrl_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BSTR movieTexName);


void __RPC_STUB IVGS2_SetMovieMtrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    INT kfrRepeatCnt);


void __RPC_STUB IVGS2_SetModelKfrRepeatCnt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllModelKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    INT kfrRepeatCnt);


void __RPC_STUB IVGS2_SetAllModelKfrRepeatCnt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateBackdropFromImage_Proxy( 
    IVGS2 * This,
    BSTR backdropName,
    BSTR oriImageName,
    BSTR alphaImageName,
    INT alignMode,
    INT left,
    INT top,
    INT width,
    INT height);


void __RPC_STUB IVGS2_CreateBackdropFromImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateSpecularMap_Proxy( 
    IVGS2 * This,
    BSTR name,
    BSTR imageName,
    INT width,
    INT height,
    INT mipLevel);


void __RPC_STUB IVGS2_CreateSpecularMap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetSpecularMapMtrl_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BSTR specularMapName);


void __RPC_STUB IVGS2_SetSpecularMapMtrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateCamera_Proxy( 
    IVGS2 * This,
    BSTR name,
    FLOAT fromX,
    FLOAT fromY,
    FLOAT fromZ,
    FLOAT atX,
    FLOAT atY,
    FLOAT atZ,
    FLOAT fov,
    FLOAT nearClip,
    FLOAT farClip);


void __RPC_STUB IVGS2_CreateCamera_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteModel_Proxy( 
    IVGS2 * This,
    BSTR modelName);


void __RPC_STUB IVGS2_DeleteModel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetCanNavigateByPointer_Proxy( 
    IVGS2 * This,
    BSTR navName,
    BOOL status);


void __RPC_STUB IVGS2_SetCanNavigateByPointer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetPlayerCanMove_Proxy( 
    IVGS2 * This,
    BOOL status);


void __RPC_STUB IVGS2_SetPlayerCanMove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetPlayerCanRos_Proxy( 
    IVGS2 * This,
    BOOL status);


void __RPC_STUB IVGS2_SetPlayerCanRos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetPlayerCanZoom_Proxy( 
    IVGS2 * This,
    BOOL status);


void __RPC_STUB IVGS2_SetPlayerCanZoom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetPlayerCanLift_Proxy( 
    IVGS2 * This,
    BOOL status);


void __RPC_STUB IVGS2_SetPlayerCanLift_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteMovieTexture_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BSTR movieTexName);


void __RPC_STUB IVGS2_DeleteMovieTexture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BSTR name,
    BOOL kfrPlay);


void __RPC_STUB IVGS2_SetModelKfrPlayStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PlayModelKfr_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    INT startFrame,
    INT endFrame,
    INT repeatCnt);


void __RPC_STUB IVGS2_PlayModelKfr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllModelsKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BOOL bKfrPlay);


void __RPC_STUB IVGS2_SetAllModelsKfrPlayStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelEventOnMouseMove_Proxy( 
    IVGS2 * This,
    BOOL status);


void __RPC_STUB IVGS2_SetModelEventOnMouseMove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetButtonVisibility_Proxy( 
    IVGS2 * This,
    BSTR btnName,
    BOOL visible);


void __RPC_STUB IVGS2_SetButtonVisibility_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAntiAliasLevel_Proxy( 
    IVGS2 * This,
    INT level);


void __RPC_STUB IVGS2_SetAntiAliasLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlSpecularCubeMap_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BSTR specularCubeMapName);


void __RPC_STUB IVGS2_SetMtrlSpecularCubeMap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetButtonActive_Proxy( 
    IVGS2 * This,
    BSTR name,
    BOOL bActive);


void __RPC_STUB IVGS2_SetButtonActive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateSectionMap_Proxy( 
    IVGS2 * This,
    BSTR sectioMapName,
    INT width,
    INT height,
    BSTR sliceModelName,
    FLOAT leftTopX,
    FLOAT leftTopY,
    FLOAT leftTopZ,
    FLOAT rightTopX,
    FLOAT rightTopY,
    FLOAT rightTopZ,
    FLOAT leftBottomX,
    FLOAT leftBottomY,
    FLOAT leftBottomZ,
    FLOAT rightBottomX,
    FLOAT rightBottomY,
    FLOAT rightBottomZ,
    BSTR imageName);


void __RPC_STUB IVGS2_CreateSectionMap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_ExecuteSlice_Proxy( 
    IVGS2 * This,
    BSTR sectionMapName);


void __RPC_STUB IVGS2_ExecuteSlice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetOverlayVisibility_Proxy( 
    IVGS2 * This,
    BSTR overlayName,
    BOOL visible);


void __RPC_STUB IVGS2_SetOverlayVisibility_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateAudio_Proxy( 
    IVGS2 * This,
    BSTR name,
    BSTR audioFile,
    INT repeatCnt);


void __RPC_STUB IVGS2_CreateAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteAudioByName_Proxy( 
    IVGS2 * This,
    BSTR name);


void __RPC_STUB IVGS2_DeleteAudioByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteAudioByIndex_Proxy( 
    IVGS2 * This,
    INT index);


void __RPC_STUB IVGS2_DeleteAudioByIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteAllAudio_Proxy( 
    IVGS2 * This);


void __RPC_STUB IVGS2_DeleteAllAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAudioFile_Proxy( 
    IVGS2 * This,
    BSTR audioName,
    BSTR audioFile);


void __RPC_STUB IVGS2_SetAudioFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAudioRepeatCount_Proxy( 
    IVGS2 * This,
    BSTR audioName,
    INT repeatCnt);


void __RPC_STUB IVGS2_SetAudioRepeatCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PlayAudio_Proxy( 
    IVGS2 * This,
    BSTR audioName);


void __RPC_STUB IVGS2_PlayAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PauseAudio_Proxy( 
    IVGS2 * This,
    BSTR audioName);


void __RPC_STUB IVGS2_PauseAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_StopAudio_Proxy( 
    IVGS2 * This,
    BSTR audioName);


void __RPC_STUB IVGS2_StopAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_RewindAudio_Proxy( 
    IVGS2 * This,
    BSTR audioName);


void __RPC_STUB IVGS2_RewindAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteModelGroup_Proxy( 
    IVGS2 * This,
    BSTR name);


void __RPC_STUB IVGS2_DeleteModelGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteModelGroupByIndex_Proxy( 
    IVGS2 * This,
    INT index);


void __RPC_STUB IVGS2_DeleteModelGroupByIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteAllModelGroup_Proxy( 
    IVGS2 * This);


void __RPC_STUB IVGS2_DeleteAllModelGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddModelToModelGroup_Proxy( 
    IVGS2 * This,
    BSTR modelGroupName,
    BSTR modelName);


void __RPC_STUB IVGS2_AddModelToModelGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteModelFromModelGroup_Proxy( 
    IVGS2 * This,
    BSTR modelGroupName,
    BSTR modelName);


void __RPC_STUB IVGS2_DeleteModelFromModelGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_ClearModelGroup_Proxy( 
    IVGS2 * This,
    BSTR modelGroupName);


void __RPC_STUB IVGS2_ClearModelGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelMouseHintType_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    INT type);


void __RPC_STUB IVGS2_SetModelMouseHintType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    BOOL bFlag);


void __RPC_STUB IVGS2_SetLightKfrPlayStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllLightKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BOOL bFlag);


void __RPC_STUB IVGS2_SetAllLightKfrPlayStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    INT repeatCnt);


void __RPC_STUB IVGS2_SetLightKfrRepeatCnt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllLightKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    INT repeatCnt);


void __RPC_STUB IVGS2_SetAllLightKfrRepeatCnt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllModelKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BOOL kfrPlay);


void __RPC_STUB IVGS2_SetAllModelKfrPlayStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelPosition_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    FLOAT x,
    FLOAT y,
    FLOAT z);


void __RPC_STUB IVGS2_SetModelPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelRotation_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    FLOAT x,
    FLOAT y,
    FLOAT z);


void __RPC_STUB IVGS2_SetModelRotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddNavigaterPointer_Proxy( 
    IVGS2 * This,
    BSTR navigaterName,
    BSTR navPointerName,
    INT navPointerType,
    BSTR camOrModelName,
    BSTR navPointerOriImage,
    BSTR navPointerAlphaImage,
    INT width,
    INT height);


void __RPC_STUB IVGS2_AddNavigaterPointer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetNavigaterPointerVisibility_Proxy( 
    IVGS2 * This,
    BSTR navigaterName,
    BSTR navPointerName,
    BOOL status);


void __RPC_STUB IVGS2_SetNavigaterPointerVisibility_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteNavigaterPointer_Proxy( 
    IVGS2 * This,
    BSTR navigaterName,
    BSTR navPointerName);


void __RPC_STUB IVGS2_DeleteNavigaterPointer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PauseCameraKfr_Proxy( 
    IVGS2 * This,
    BSTR camName);


void __RPC_STUB IVGS2_PauseCameraKfr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_ResumeCameraKfr_Proxy( 
    IVGS2 * This,
    BSTR cameraName);


void __RPC_STUB IVGS2_ResumeCameraKfr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetNavigaterPointerObject_Proxy( 
    IVGS2 * This,
    BSTR navName,
    BSTR navPointerName,
    INT objType,
    BSTR objName);


void __RPC_STUB IVGS2_SetNavigaterPointerObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetPlayerZoomSpeed_Proxy( 
    IVGS2 * This,
    FLOAT speed);


void __RPC_STUB IVGS2_SetPlayerZoomSpeed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetOverlayFullScreen_Proxy( 
    IVGS2 * This,
    BSTR OverlayName,
    BOOL xFull,
    BOOL yFull);


void __RPC_STUB IVGS2_SetOverlayFullScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PushOverlayFront_Proxy( 
    IVGS2 * This,
    BSTR OverlayName);


void __RPC_STUB IVGS2_PushOverlayFront_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PushOverlayBack_Proxy( 
    IVGS2 * This,
    BSTR overlayName);


void __RPC_STUB IVGS2_PushOverlayBack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteAllModel_Proxy( 
    IVGS2 * This);


void __RPC_STUB IVGS2_DeleteAllModel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PlayKeyFrame_Proxy( 
    IVGS2 * This);


void __RPC_STUB IVGS2_PlayKeyFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PauseKeyFrame_Proxy( 
    IVGS2 * This);


void __RPC_STUB IVGS2_PauseKeyFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_StopKeyFrame_Proxy( 
    IVGS2 * This);


void __RPC_STUB IVGS2_StopKeyFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetKeyFrameRepeatCount_Proxy( 
    IVGS2 * This,
    INT repeatCnt);


void __RPC_STUB IVGS2_SetKeyFrameRepeatCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_GotoFrame_Proxy( 
    IVGS2 * This,
    INT frame);


void __RPC_STUB IVGS2_GotoFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetKeyFramePlaySpeed_Proxy( 
    IVGS2 * This,
    INT fps);


void __RPC_STUB IVGS2_SetKeyFramePlaySpeed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateFSGlow_Proxy( 
    IVGS2 * This,
    FLOAT BloomValue,
    FLOAT BlurValue);


void __RPC_STUB IVGS2_CreateFSGlow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetFSGlowValue_Proxy( 
    IVGS2 * This,
    FLOAT BloomValue,
    FLOAT BlurValue);


void __RPC_STUB IVGS2_SetFSGlowValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_StartFSGlow_Proxy( 
    IVGS2 * This);


void __RPC_STUB IVGS2_StartFSGlow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_EndFSGlow_Proxy( 
    IVGS2 * This);


void __RPC_STUB IVGS2_EndFSGlow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlSpecular_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT red,
    INT green,
    INT blue);


void __RPC_STUB IVGS2_SetMtrlSpecular_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlAmbient_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT red,
    INT green,
    INT blue);


void __RPC_STUB IVGS2_SetMtrlAmbient_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlSpecularLevel_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT level);


void __RPC_STUB IVGS2_SetMtrlSpecularLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlSpecularPower_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT power);


void __RPC_STUB IVGS2_SetMtrlSpecularPower_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PlayKeyFrame1_Proxy( 
    IVGS2 * This,
    INT startFrame,
    INT endFrame);


void __RPC_STUB IVGS2_PlayKeyFrame1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetCameraKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BSTR camName,
    BOOL status);


void __RPC_STUB IVGS2_SetCameraKfrPlayStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllModelVisibility_Proxy( 
    IVGS2 * This,
    BOOL status);


void __RPC_STUB IVGS2_SetAllModelVisibility_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllCameraKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BOOL status);


void __RPC_STUB IVGS2_SetAllCameraKfrPlayStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetCameraKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    BSTR camName,
    INT repeatCnt);


void __RPC_STUB IVGS2_SetCameraKfrRepeatCnt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllCameraKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    INT repeatCnt);


void __RPC_STUB IVGS2_SetAllCameraKfrRepeatCnt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetDynamicTexPlaySpeed_Proxy( 
    IVGS2 * This,
    BSTR UseDyTexMtrlName,
    INT texLayerIdx,
    INT fps);


void __RPC_STUB IVGS2_SetDynamicTexPlaySpeed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelPositionByPivot_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    FLOAT x,
    FLOAT y,
    FLOAT z);


void __RPC_STUB IVGS2_SetModelPositionByPivot_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetDynamicUVStatus_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    BOOL status);


void __RPC_STUB IVGS2_SetDynamicUVStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_MoveFlash_Proxy( 
    IVGS2 * This,
    BSTR flashName,
    INT x,
    INT y);


void __RPC_STUB IVGS2_MoveFlash_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetOverlayMtrl_Proxy( 
    IVGS2 * This,
    BSTR overlayName,
    BSTR mtrlName);


void __RPC_STUB IVGS2_SetOverlayMtrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetZoneTriggerModelCallBackFunc_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    BSTR funcName);


void __RPC_STUB IVGS2_SetZoneTriggerModelCallBackFunc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAlphaTestLevel_Proxy( 
    IVGS2 * This,
    INT level);


void __RPC_STUB IVGS2_SetAlphaTestLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetButtonOpacity_Proxy( 
    IVGS2 * This,
    BSTR buttonName,
    INT opacity);


void __RPC_STUB IVGS2_SetButtonOpacity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CurCamGotoFrame_Proxy( 
    IVGS2 * This,
    INT frame);


void __RPC_STUB IVGS2_CurCamGotoFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetSceneEditMode_Proxy( 
    IVGS2 * This,
    INT EditMode);


void __RPC_STUB IVGS2_SetSceneEditMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CloneModel_Proxy( 
    IVGS2 * This,
    BSTR oriModelName,
    BSTR desModelName,
    INT ID1,
    INT ID2,
    FLOAT posX,
    FLOAT posY,
    FLOAT posZ);


void __RPC_STUB IVGS2_CloneModel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theModelPosX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theModelPosX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theModelPosY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theModelPosY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theModelPosZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theModelPosZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theModelScreenPosX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ short *pVal);


void __RPC_STUB IVGS2_get_theModelScreenPosX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theModelScreenPosY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ short *pVal);


void __RPC_STUB IVGS2_get_theModelScreenPosY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_GetInfoFromModel_Proxy( 
    IVGS2 * This,
    BSTR modelName);


void __RPC_STUB IVGS2_GetInfoFromModel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateNewText_Proxy( 
    IVGS2 * This,
    BSTR textString);


void __RPC_STUB IVGS2_CreateNewText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theTextHandle_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IVGS2_get_theTextHandle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetTextPosition_Proxy( 
    IVGS2 * This,
    long textHandle,
    UINT x,
    UINT y);


void __RPC_STUB IVGS2_SetTextPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetTextColor_Proxy( 
    IVGS2 * This,
    long textHandle,
    INT red,
    INT green,
    INT blue,
    INT alpha);


void __RPC_STUB IVGS2_SetTextColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteText_Proxy( 
    IVGS2 * This,
    long textHandle);


void __RPC_STUB IVGS2_DeleteText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_GetInfoFromCamera_Proxy( 
    IVGS2 * This,
    BSTR cameraName);


void __RPC_STUB IVGS2_GetInfoFromCamera_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamFromPosX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theCamFromPosX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamFromPosY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theCamFromPosY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamFromPosZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theCamFromPosZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamAtPosX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theCamAtPosX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamAtPosY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theCamAtPosY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamAtPosZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theCamAtPosZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetTextString_Proxy( 
    IVGS2 * This,
    long textHandle,
    BSTR textString);


void __RPC_STUB IVGS2_SetTextString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_LoadScene_Proxy( 
    IVGS2 * This,
    BSTR SceneName,
    BSTR ParentSceneName,
    BSTR N3DFileURL,
    FLOAT m11,
    FLOAT m12,
    FLOAT m13,
    FLOAT m14,
    FLOAT m21,
    FLOAT m22,
    FLOAT m23,
    FLOAT m24,
    FLOAT m31,
    FLOAT m32,
    FLOAT m33,
    FLOAT m34,
    FLOAT m41,
    FLOAT m42,
    FLOAT m43,
    FLOAT m44);


void __RPC_STUB IVGS2_LoadScene_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteScene_Proxy( 
    IVGS2 * This,
    BSTR SceneName);


void __RPC_STUB IVGS2_DeleteScene_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CloneScene_Proxy( 
    IVGS2 * This,
    BSTR destSceneName,
    BSTR srcSceneName,
    BSTR ParentSceneName,
    FLOAT m11,
    FLOAT m12,
    FLOAT m13,
    FLOAT m14,
    FLOAT m21,
    FLOAT m22,
    FLOAT m23,
    FLOAT m24,
    FLOAT m31,
    FLOAT m32,
    FLOAT m33,
    FLOAT m34,
    FLOAT m41,
    FLOAT m42,
    FLOAT m43,
    FLOAT m44);


void __RPC_STUB IVGS2_CloneScene_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateRTFTexture_Proxy( 
    IVGS2 * This,
    BSTR TextureName,
    BSTR refPlaneName,
    INT height,
    INT width,
    FLOAT power);


void __RPC_STUB IVGS2_CreateRTFTexture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddModelToRTFTexture_Proxy( 
    IVGS2 * This,
    BSTR RTFTextureName,
    BSTR ModelName);


void __RPC_STUB IVGS2_AddModelToRTFTexture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateDynamicCubeMap_Proxy( 
    IVGS2 * This,
    BSTR cubeMapame,
    BSTR centerModelName,
    INT size,
    INT power);


void __RPC_STUB IVGS2_CreateDynamicCubeMap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddDynamicCubeMapReflectionModel_Proxy( 
    IVGS2 * This,
    BSTR cubeMapName,
    BSTR ModelName);


void __RPC_STUB IVGS2_AddDynamicCubeMapReflectionModel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteDynamicCubeMapReflectionModel_Proxy( 
    IVGS2 * This,
    BSTR cubeMapName,
    BSTR modelName);


void __RPC_STUB IVGS2_DeleteDynamicCubeMapReflectionModel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateRealWater_Proxy( 
    IVGS2 * This,
    BSTR name,
    FLOAT x,
    FLOAT y,
    FLOAT z,
    INT gridX,
    INT gridY);


void __RPC_STUB IVGS2_CreateRealWater_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddRealWaterRTTModel_Proxy( 
    IVGS2 * This,
    BSTR RealWaterName,
    BSTR ModelName);


void __RPC_STUB IVGS2_AddRealWaterRTTModel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddRealWaterRTFModel_Proxy( 
    IVGS2 * This,
    BSTR RealWaterName,
    BSTR ModelName);


void __RPC_STUB IVGS2_AddRealWaterRTFModel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetRealWaterCubeMap_Proxy( 
    IVGS2 * This,
    BSTR RealWaterName,
    BSTR CubeMapName);


void __RPC_STUB IVGS2_SetRealWaterCubeMap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateColorEffect_Proxy( 
    IVGS2 * This);


void __RPC_STUB IVGS2_CreateColorEffect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetColorEffect_Proxy( 
    IVGS2 * This,
    FLOAT brightness,
    FLOAT contrast,
    FLOAT red,
    FLOAT green,
    FLOAT blue);


void __RPC_STUB IVGS2_SetColorEffect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetTextureImageFromURL_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT texLayerIdx,
    BSTR imageURL);


void __RPC_STUB IVGS2_SetTextureImageFromURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetEditCtrlMaxUpDownAngle_Proxy( 
    IVGS2 * This,
    FLOAT upAngle,
    FLOAT downAngle);


void __RPC_STUB IVGS2_SetEditCtrlMaxUpDownAngle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlTexture_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT textureLayerIndex,
    INT textureType,
    BSTR textureName);


void __RPC_STUB IVGS2_SetMtrlTexture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddFlashCtrlSemaphore_Proxy( 
    IVGS2 * This,
    BSTR flashName,
    BSTR SemaphoreName,
    BSTR SemaphoreFunc);


void __RPC_STUB IVGS2_AddFlashCtrlSemaphore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteFlashCtrlSemaphore_Proxy( 
    IVGS2 * This,
    BSTR flashName,
    BSTR SemaphoreName);


void __RPC_STUB IVGS2_DeleteFlashCtrlSemaphore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theFlashVar_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IVGS2_get_theFlashVar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_GetFlashVaribleInfo_Proxy( 
    IVGS2 * This,
    BSTR flashName,
    BSTR varibleName);


void __RPC_STUB IVGS2_GetFlashVaribleInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetFlashVarible_Proxy( 
    IVGS2 * This,
    BSTR flashName,
    BSTR varibleName,
    BSTR varibleValue);


void __RPC_STUB IVGS2_SetFlashVarible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_LoadN3DScene_Proxy( 
    IVGS2 * This,
    BSTR SceneName,
    BSTR ParentSceneName,
    BSTR N3DFileURL,
    FLOAT posX,
    FLOAT posY,
    FLOAT posZ,
    FLOAT rotX,
    FLOAT rotY,
    FLOAT rotZ,
    FLOAT scaleX,
    FLOAT scaleY,
    FLOAT scaleZ);


void __RPC_STUB IVGS2_LoadN3DScene_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_GetInfoFromSceneResource_Proxy( 
    IVGS2 * This,
    BSTR SceneName);


void __RPC_STUB IVGS2_GetInfoFromSceneResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theScenePosX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theScenePosX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theScenePosY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theScenePosY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theScenePosZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theScenePosZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneRotX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theSceneRotX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneRotY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theSceneRotY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneRotZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theSceneRotZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneScaleX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theSceneScaleX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneScaleY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theSceneScaleY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneScaleZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IVGS2_get_theSceneScaleZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetSceneResColor_Proxy( 
    IVGS2 * This,
    BSTR SceneName,
    INT red,
    INT green,
    INT blue,
    INT alpha);


void __RPC_STUB IVGS2_SetSceneResColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetButtonRect_Proxy( 
    IVGS2 * This,
    BSTR buttonName,
    INT x,
    INT y,
    INT width,
    INT height);


void __RPC_STUB IVGS2_SetButtonRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAddtionalKeys_Proxy( 
    IVGS2 * This,
    /* [in] */ BYTE left,
    /* [in] */ BYTE right,
    /* [in] */ BYTE up,
    /* [in] */ BYTE down);


void __RPC_STUB IVGS2_SetAddtionalKeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAddtionalKeysStatus_Proxy( 
    IVGS2 * This,
    /* [in] */ BOOL bEnable);


void __RPC_STUB IVGS2_SetAddtionalKeysStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetSubBrowserType_Proxy( 
    IVGS2 * This,
    /* [in] */ LONG subType);


void __RPC_STUB IVGS2_SetSubBrowserType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_wbType_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IVGS2_get_wbType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IVGS2_put_wbType_Proxy( 
    IVGS2 * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IVGS2_put_wbType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVGS2_INTERFACE_DEFINED__ */



#ifndef __VGSATL2Lib_LIBRARY_DEFINED__
#define __VGSATL2Lib_LIBRARY_DEFINED__

/* library VGSATL2Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_VGSATL2Lib;

#ifndef ___IVGS2Events_DISPINTERFACE_DEFINED__
#define ___IVGS2Events_DISPINTERFACE_DEFINED__

/* dispinterface _IVGS2Events */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IVGS2Events;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("994E6C81-8A03-4313-9736-F6FAA1DEF95C")
    _IVGS2Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IVGS2EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IVGS2Events * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IVGS2Events * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IVGS2Events * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IVGS2Events * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IVGS2Events * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IVGS2Events * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IVGS2Events * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IVGS2EventsVtbl;

    interface _IVGS2Events
    {
        CONST_VTBL struct _IVGS2EventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IVGS2Events_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IVGS2Events_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IVGS2Events_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IVGS2Events_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IVGS2Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IVGS2Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IVGS2Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IVGS2Events_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_VGS2;

#ifdef __cplusplus

class DECLSPEC_UUID("046216A1-952E-486C-8598-4F2D0D0676F2")
VGS2;
#endif
#endif /* __VGSATL2Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


