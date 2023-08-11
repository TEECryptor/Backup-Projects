

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "VGSATL2_i.h"

#define TYPE_FORMAT_STRING_SIZE   1079                              
#define PROC_FORMAT_STRING_SIZE   12811                             
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _VGSATL2_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } VGSATL2_MIDL_TYPE_FORMAT_STRING;

typedef struct _VGSATL2_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } VGSATL2_MIDL_PROC_FORMAT_STRING;

typedef struct _VGSATL2_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } VGSATL2_MIDL_EXPR_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const VGSATL2_MIDL_TYPE_FORMAT_STRING VGSATL2__MIDL_TypeFormatString;
extern const VGSATL2_MIDL_PROC_FORMAT_STRING VGSATL2__MIDL_ProcFormatString;
extern const VGSATL2_MIDL_EXPR_FORMAT_STRING VGSATL2__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IVGS2_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IVGS2_ProxyInfo;

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
    INT mouseClick_Alpha)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5298],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMouseMoveEventTimer_Proxy( 
    IVGS2 * This,
    INT timeStep)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5382],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetNavigaterVisibility_Proxy( 
    IVGS2 * This,
    BSTR navName,
    BOOL visible)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5418],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_RestoreModelColor_Proxy( 
    IVGS2 * This,
    BSTR modelName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5460],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_RestoreAllModelsColor_Proxy( 
    IVGS2 * This)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5496],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAlphaTestStatus_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5526],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetZOneModel_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5568],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLensFlareVisibility_Proxy( 
    IVGS2 * This,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5610],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetCollisionDistance_Proxy( 
    IVGS2 * This,
    FLOAT distance)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5646],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateStaticCubeMap_Proxy( 
    IVGS2 * This,
    BSTR name,
    BSTR frontImage,
    BSTR backImage,
    BSTR leftImage,
    BSTR rightImage,
    BSTR topImage,
    BSTR bottomImge,
    INT size)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5682],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlCubeMap_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BSTR cubeMapName,
    FLOAT power)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5760],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightColor_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    INT red,
    INT green,
    INT blue)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5808],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightPower_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    FLOAT power)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5862],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightAmbient_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    INT red,
    INT green,
    INT blue)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5904],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightPosition_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    FLOAT x,
    FLOAT y,
    FLOAT z)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[5958],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightRange_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    FLOAT range)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6012],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightAttenuation_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    FLOAT attenuation0,
    FLOAT attenuation1,
    FLOAT attenuation2)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6054],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightStatus_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6108],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateMovieTexture_Proxy( 
    IVGS2 * This,
    BSTR texName,
    BSTR moviePath,
    INT movieType,
    BOOL withAudio,
    INT repeatCnt,
    BSTR mtrlName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6150],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMovieMtrl_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BSTR movieTexName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6216],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    INT kfrRepeatCnt)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6258],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllModelKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    INT kfrRepeatCnt)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6300],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateBackdropFromImage_Proxy( 
    IVGS2 * This,
    BSTR backdropName,
    BSTR oriImageName,
    BSTR alphaImageName,
    INT alignMode,
    INT left,
    INT top,
    INT width,
    INT height)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6336],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateSpecularMap_Proxy( 
    IVGS2 * This,
    BSTR name,
    BSTR imageName,
    INT width,
    INT height,
    INT mipLevel)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6414],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetSpecularMapMtrl_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BSTR specularMapName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6474],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

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
    FLOAT farClip)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6516],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteModel_Proxy( 
    IVGS2 * This,
    BSTR modelName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6606],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetCanNavigateByPointer_Proxy( 
    IVGS2 * This,
    BSTR navName,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6642],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetPlayerCanMove_Proxy( 
    IVGS2 * This,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6684],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetPlayerCanRos_Proxy( 
    IVGS2 * This,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6720],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetPlayerCanZoom_Proxy( 
    IVGS2 * This,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6756],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetPlayerCanLift_Proxy( 
    IVGS2 * This,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6792],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteMovieTexture_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BSTR movieTexName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6828],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BSTR name,
    BOOL kfrPlay)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6870],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PlayModelKfr_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    INT startFrame,
    INT endFrame,
    INT repeatCnt)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6912],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllModelsKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BOOL bKfrPlay)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[6966],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelEventOnMouseMove_Proxy( 
    IVGS2 * This,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7002],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetButtonVisibility_Proxy( 
    IVGS2 * This,
    BSTR btnName,
    BOOL visible)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7038],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAntiAliasLevel_Proxy( 
    IVGS2 * This,
    INT level)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7080],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlSpecularCubeMap_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    BSTR specularCubeMapName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7116],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetButtonActive_Proxy( 
    IVGS2 * This,
    BSTR name,
    BOOL bActive)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7158],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

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
    BSTR imageName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7200],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_ExecuteSlice_Proxy( 
    IVGS2 * This,
    BSTR sectionMapName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7332],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetOverlayVisibility_Proxy( 
    IVGS2 * This,
    BSTR overlayName,
    BOOL visible)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7368],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateAudio_Proxy( 
    IVGS2 * This,
    BSTR name,
    BSTR audioFile,
    INT repeatCnt)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7410],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteAudioByName_Proxy( 
    IVGS2 * This,
    BSTR name)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7458],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteAudioByIndex_Proxy( 
    IVGS2 * This,
    INT index)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7494],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteAllAudio_Proxy( 
    IVGS2 * This)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7530],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAudioFile_Proxy( 
    IVGS2 * This,
    BSTR audioName,
    BSTR audioFile)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7560],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAudioRepeatCount_Proxy( 
    IVGS2 * This,
    BSTR audioName,
    INT repeatCnt)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7602],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PlayAudio_Proxy( 
    IVGS2 * This,
    BSTR audioName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7644],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PauseAudio_Proxy( 
    IVGS2 * This,
    BSTR audioName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7680],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_StopAudio_Proxy( 
    IVGS2 * This,
    BSTR audioName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7716],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_RewindAudio_Proxy( 
    IVGS2 * This,
    BSTR audioName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7752],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteModelGroup_Proxy( 
    IVGS2 * This,
    BSTR name)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7788],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteModelGroupByIndex_Proxy( 
    IVGS2 * This,
    INT index)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7824],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteAllModelGroup_Proxy( 
    IVGS2 * This)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7860],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddModelToModelGroup_Proxy( 
    IVGS2 * This,
    BSTR modelGroupName,
    BSTR modelName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7890],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteModelFromModelGroup_Proxy( 
    IVGS2 * This,
    BSTR modelGroupName,
    BSTR modelName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7932],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_ClearModelGroup_Proxy( 
    IVGS2 * This,
    BSTR modelGroupName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[7974],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelMouseHintType_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    INT type)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8010],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    BOOL bFlag)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8052],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllLightKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BOOL bFlag)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8094],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetLightKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    BSTR lightName,
    INT repeatCnt)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8130],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllLightKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    INT repeatCnt)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8172],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllModelKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BOOL kfrPlay)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8208],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelPosition_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    FLOAT x,
    FLOAT y,
    FLOAT z)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8244],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelRotation_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    FLOAT x,
    FLOAT y,
    FLOAT z)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8298],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddNavigaterPointer_Proxy( 
    IVGS2 * This,
    BSTR navigaterName,
    BSTR navPointerName,
    INT navPointerType,
    BSTR camOrModelName,
    BSTR navPointerOriImage,
    BSTR navPointerAlphaImage,
    INT width,
    INT height)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8352],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetNavigaterPointerVisibility_Proxy( 
    IVGS2 * This,
    BSTR navigaterName,
    BSTR navPointerName,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8430],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteNavigaterPointer_Proxy( 
    IVGS2 * This,
    BSTR navigaterName,
    BSTR navPointerName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8478],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PauseCameraKfr_Proxy( 
    IVGS2 * This,
    BSTR camName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8520],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_ResumeCameraKfr_Proxy( 
    IVGS2 * This,
    BSTR cameraName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8556],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetNavigaterPointerObject_Proxy( 
    IVGS2 * This,
    BSTR navName,
    BSTR navPointerName,
    INT objType,
    BSTR objName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8592],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetPlayerZoomSpeed_Proxy( 
    IVGS2 * This,
    FLOAT speed)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8646],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetOverlayFullScreen_Proxy( 
    IVGS2 * This,
    BSTR OverlayName,
    BOOL xFull,
    BOOL yFull)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8682],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PushOverlayFront_Proxy( 
    IVGS2 * This,
    BSTR OverlayName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8730],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PushOverlayBack_Proxy( 
    IVGS2 * This,
    BSTR overlayName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8766],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteAllModel_Proxy( 
    IVGS2 * This)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8802],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PlayKeyFrame_Proxy( 
    IVGS2 * This)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8832],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PauseKeyFrame_Proxy( 
    IVGS2 * This)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8862],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_StopKeyFrame_Proxy( 
    IVGS2 * This)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8892],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetKeyFrameRepeatCount_Proxy( 
    IVGS2 * This,
    INT repeatCnt)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8922],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_GotoFrame_Proxy( 
    IVGS2 * This,
    INT frame)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8958],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetKeyFramePlaySpeed_Proxy( 
    IVGS2 * This,
    INT fps)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[8994],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateFSGlow_Proxy( 
    IVGS2 * This,
    FLOAT BloomValue,
    FLOAT BlurValue)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9030],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetFSGlowValue_Proxy( 
    IVGS2 * This,
    FLOAT BloomValue,
    FLOAT BlurValue)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9072],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_StartFSGlow_Proxy( 
    IVGS2 * This)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9114],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_EndFSGlow_Proxy( 
    IVGS2 * This)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9144],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlSpecular_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT red,
    INT green,
    INT blue)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9174],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlAmbient_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT red,
    INT green,
    INT blue)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9228],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlSpecularLevel_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT level)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9282],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlSpecularPower_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT power)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9324],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_PlayKeyFrame1_Proxy( 
    IVGS2 * This,
    INT startFrame,
    INT endFrame)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9366],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetCameraKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BSTR camName,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9408],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllModelVisibility_Proxy( 
    IVGS2 * This,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9450],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllCameraKfrPlayStatus_Proxy( 
    IVGS2 * This,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9486],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetCameraKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    BSTR camName,
    INT repeatCnt)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9522],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAllCameraKfrRepeatCnt_Proxy( 
    IVGS2 * This,
    INT repeatCnt)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9564],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetDynamicTexPlaySpeed_Proxy( 
    IVGS2 * This,
    BSTR UseDyTexMtrlName,
    INT texLayerIdx,
    INT fps)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9600],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetModelPositionByPivot_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    FLOAT x,
    FLOAT y,
    FLOAT z)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9648],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetDynamicUVStatus_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    BOOL status)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9702],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_MoveFlash_Proxy( 
    IVGS2 * This,
    BSTR flashName,
    INT x,
    INT y)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9744],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetOverlayMtrl_Proxy( 
    IVGS2 * This,
    BSTR overlayName,
    BSTR mtrlName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9792],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetZoneTriggerModelCallBackFunc_Proxy( 
    IVGS2 * This,
    BSTR modelName,
    BSTR funcName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9834],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAlphaTestLevel_Proxy( 
    IVGS2 * This,
    INT level)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9876],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetButtonOpacity_Proxy( 
    IVGS2 * This,
    BSTR buttonName,
    INT opacity)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9912],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CurCamGotoFrame_Proxy( 
    IVGS2 * This,
    INT frame)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9954],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetSceneEditMode_Proxy( 
    IVGS2 * This,
    INT EditMode)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[9990],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CloneModel_Proxy( 
    IVGS2 * This,
    BSTR oriModelName,
    BSTR desModelName,
    INT ID1,
    INT ID2,
    FLOAT posX,
    FLOAT posY,
    FLOAT posZ)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10026],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theModelPosX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10098],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theModelPosY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10134],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theModelPosZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10170],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theModelScreenPosX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ short *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10206],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theModelScreenPosY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ short *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10242],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_GetInfoFromModel_Proxy( 
    IVGS2 * This,
    BSTR modelName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10278],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateNewText_Proxy( 
    IVGS2 * This,
    BSTR textString)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10314],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theTextHandle_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ long *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10350],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetTextPosition_Proxy( 
    IVGS2 * This,
    long textHandle,
    UINT x,
    UINT y)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10386],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetTextColor_Proxy( 
    IVGS2 * This,
    long textHandle,
    INT red,
    INT green,
    INT blue,
    INT alpha)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10434],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteText_Proxy( 
    IVGS2 * This,
    long textHandle)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10494],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_GetInfoFromCamera_Proxy( 
    IVGS2 * This,
    BSTR cameraName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10530],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamFromPosX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10566],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamFromPosY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10602],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamFromPosZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10638],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamAtPosX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10674],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamAtPosY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10710],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theCamAtPosZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10746],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetTextString_Proxy( 
    IVGS2 * This,
    long textHandle,
    BSTR textString)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10782],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

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
    FLOAT m44)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10824],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteScene_Proxy( 
    IVGS2 * This,
    BSTR SceneName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[10968],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

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
    FLOAT m44)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11004],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateRTFTexture_Proxy( 
    IVGS2 * This,
    BSTR TextureName,
    BSTR refPlaneName,
    INT height,
    INT width,
    FLOAT power)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11148],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddModelToRTFTexture_Proxy( 
    IVGS2 * This,
    BSTR RTFTextureName,
    BSTR ModelName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11208],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateDynamicCubeMap_Proxy( 
    IVGS2 * This,
    BSTR cubeMapame,
    BSTR centerModelName,
    INT size,
    INT power)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11250],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddDynamicCubeMapReflectionModel_Proxy( 
    IVGS2 * This,
    BSTR cubeMapName,
    BSTR ModelName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11304],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteDynamicCubeMapReflectionModel_Proxy( 
    IVGS2 * This,
    BSTR cubeMapName,
    BSTR modelName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11346],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateRealWater_Proxy( 
    IVGS2 * This,
    BSTR name,
    FLOAT x,
    FLOAT y,
    FLOAT z,
    INT gridX,
    INT gridY)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11388],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddRealWaterRTTModel_Proxy( 
    IVGS2 * This,
    BSTR RealWaterName,
    BSTR ModelName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11454],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddRealWaterRTFModel_Proxy( 
    IVGS2 * This,
    BSTR RealWaterName,
    BSTR ModelName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11496],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetRealWaterCubeMap_Proxy( 
    IVGS2 * This,
    BSTR RealWaterName,
    BSTR CubeMapName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11538],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_CreateColorEffect_Proxy( 
    IVGS2 * This)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11580],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetColorEffect_Proxy( 
    IVGS2 * This,
    FLOAT brightness,
    FLOAT contrast,
    FLOAT red,
    FLOAT green,
    FLOAT blue)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11610],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetTextureImageFromURL_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT texLayerIdx,
    BSTR imageURL)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11670],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetEditCtrlMaxUpDownAngle_Proxy( 
    IVGS2 * This,
    FLOAT upAngle,
    FLOAT downAngle)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11718],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetMtrlTexture_Proxy( 
    IVGS2 * This,
    BSTR mtrlName,
    INT textureLayerIndex,
    INT textureType,
    BSTR textureName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11760],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_AddFlashCtrlSemaphore_Proxy( 
    IVGS2 * This,
    BSTR flashName,
    BSTR SemaphoreName,
    BSTR SemaphoreFunc)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11814],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_DeleteFlashCtrlSemaphore_Proxy( 
    IVGS2 * This,
    BSTR flashName,
    BSTR SemaphoreName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11862],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theFlashVar_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ BSTR *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11904],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_GetFlashVaribleInfo_Proxy( 
    IVGS2 * This,
    BSTR flashName,
    BSTR varibleName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11940],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetFlashVarible_Proxy( 
    IVGS2 * This,
    BSTR flashName,
    BSTR varibleName,
    BSTR varibleValue)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[11982],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

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
    FLOAT scaleZ)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12030],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_GetInfoFromSceneResource_Proxy( 
    IVGS2 * This,
    BSTR SceneName)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12132],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theScenePosX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12168],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theScenePosY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12204],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theScenePosZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12240],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneRotX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12276],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneRotY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12312],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneRotZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12348],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneScaleX_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12384],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneScaleY_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12420],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_theSceneScaleZ_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ float *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12456],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetSceneResColor_Proxy( 
    IVGS2 * This,
    BSTR SceneName,
    INT red,
    INT green,
    INT blue,
    INT alpha)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12492],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetButtonRect_Proxy( 
    IVGS2 * This,
    BSTR buttonName,
    INT x,
    INT y,
    INT width,
    INT height)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12552],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAddtionalKeys_Proxy( 
    IVGS2 * This,
    /* [in] */ BYTE left,
    /* [in] */ BYTE right,
    /* [in] */ BYTE up,
    /* [in] */ BYTE down)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12612],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetAddtionalKeysStatus_Proxy( 
    IVGS2 * This,
    /* [in] */ BOOL bEnable)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12666],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVGS2_SetSubBrowserType_Proxy( 
    IVGS2 * This,
    /* [in] */ LONG subType)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12702],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVGS2_get_wbType_Proxy( 
    IVGS2 * This,
    /* [retval][out] */ LONG *pVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12738],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}

/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IVGS2_put_wbType_Proxy( 
    IVGS2 * This,
    /* [in] */ LONG newVal)
{
CLIENT_CALL_RETURN _RetVal;

_RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &VGSATL2__MIDL_ProcFormatString.Format[12774],
                  ( unsigned char * )&This);
return ( HRESULT  )_RetVal.Simple;

}


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const VGSATL2_MIDL_PROC_FORMAT_STRING VGSATL2__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure SetModelVisibility */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 16 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x1 ),	/* 1 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 24 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter visible */

/* 30 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 36 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 38 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetDynamicUV */

/* 42 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 44 */	NdrFcLong( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 52 */	NdrFcShort( 0x18 ),	/* 24 */
/* 54 */	NdrFcShort( 0x8 ),	/* 8 */
/* 56 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 58 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x4 ),	/* 4 */
/* 64 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 66 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 68 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 70 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter u */

/* 72 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 74 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 76 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter v */

/* 78 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter speed */

/* 84 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 86 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 88 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 90 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 92 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 94 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddFlashCtrl */

/* 96 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 98 */	NdrFcLong( 0x0 ),	/* 0 */
/* 102 */	NdrFcShort( 0x9 ),	/* 9 */
/* 104 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 106 */	NdrFcShort( 0x30 ),	/* 48 */
/* 108 */	NdrFcShort( 0x8 ),	/* 8 */
/* 110 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x9,		/* 9 */
/* 112 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 114 */	NdrFcShort( 0x0 ),	/* 0 */
/* 116 */	NdrFcShort( 0x8 ),	/* 8 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter flashName */

/* 120 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 122 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 124 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter flashFile */

/* 126 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 128 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 130 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter alignMode */

/* 132 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 134 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 136 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter left */

/* 138 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 140 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter top */

/* 144 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 146 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 148 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter width */

/* 150 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 152 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 154 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 156 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 158 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 160 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter loop */

/* 162 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 164 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 166 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 168 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 170 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFlashRect */

/* 174 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 176 */	NdrFcLong( 0x0 ),	/* 0 */
/* 180 */	NdrFcShort( 0xa ),	/* 10 */
/* 182 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 184 */	NdrFcShort( 0x28 ),	/* 40 */
/* 186 */	NdrFcShort( 0x8 ),	/* 8 */
/* 188 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 190 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 194 */	NdrFcShort( 0x4 ),	/* 4 */
/* 196 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter flashName */

/* 198 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 200 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 202 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter alignMode */

/* 204 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 206 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter left */

/* 210 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 212 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter top */

/* 216 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 218 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 220 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter width */

/* 222 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 224 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 228 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 230 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 232 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 234 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 236 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 238 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFlashCtrlVisibility */

/* 240 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 242 */	NdrFcLong( 0x0 ),	/* 0 */
/* 246 */	NdrFcShort( 0xb ),	/* 11 */
/* 248 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 250 */	NdrFcShort( 0x8 ),	/* 8 */
/* 252 */	NdrFcShort( 0x8 ),	/* 8 */
/* 254 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 256 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 258 */	NdrFcShort( 0x0 ),	/* 0 */
/* 260 */	NdrFcShort( 0x4 ),	/* 4 */
/* 262 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter flashName */

/* 264 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 266 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 268 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 270 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 272 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 274 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 276 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 278 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetRenderStatus */

/* 282 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 284 */	NdrFcLong( 0x0 ),	/* 0 */
/* 288 */	NdrFcShort( 0xc ),	/* 12 */
/* 290 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 292 */	NdrFcShort( 0x8 ),	/* 8 */
/* 294 */	NdrFcShort( 0x8 ),	/* 8 */
/* 296 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 298 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 300 */	NdrFcShort( 0x0 ),	/* 0 */
/* 302 */	NdrFcShort( 0x0 ),	/* 0 */
/* 304 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 306 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 308 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 310 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 312 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 314 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 316 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFlashPlayStatus */

/* 318 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 320 */	NdrFcLong( 0x0 ),	/* 0 */
/* 324 */	NdrFcShort( 0xd ),	/* 13 */
/* 326 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 328 */	NdrFcShort( 0x8 ),	/* 8 */
/* 330 */	NdrFcShort( 0x8 ),	/* 8 */
/* 332 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 334 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 336 */	NdrFcShort( 0x0 ),	/* 0 */
/* 338 */	NdrFcShort( 0x4 ),	/* 4 */
/* 340 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter flashName */

/* 342 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 344 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 346 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 348 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 350 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 352 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 354 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 356 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 358 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteFlashCtrl */

/* 360 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 362 */	NdrFcLong( 0x0 ),	/* 0 */
/* 366 */	NdrFcShort( 0xe ),	/* 14 */
/* 368 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x8 ),	/* 8 */
/* 374 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 376 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 378 */	NdrFcShort( 0x0 ),	/* 0 */
/* 380 */	NdrFcShort( 0x4 ),	/* 4 */
/* 382 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter flashName */

/* 384 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 386 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 388 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 390 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 392 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveCamera */

/* 396 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 398 */	NdrFcLong( 0x0 ),	/* 0 */
/* 402 */	NdrFcShort( 0xf ),	/* 15 */
/* 404 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 406 */	NdrFcShort( 0x28 ),	/* 40 */
/* 408 */	NdrFcShort( 0x8 ),	/* 8 */
/* 410 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 412 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 414 */	NdrFcShort( 0x0 ),	/* 0 */
/* 416 */	NdrFcShort( 0x4 ),	/* 4 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cameraName */

/* 420 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 422 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 424 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 426 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 428 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 430 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 432 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 434 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 436 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 438 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 440 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 442 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter stepCnt */

/* 444 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 446 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 448 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter coordEnum */

/* 450 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 452 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 454 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 456 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 458 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 460 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveCurrentCamera */

/* 462 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 464 */	NdrFcLong( 0x0 ),	/* 0 */
/* 468 */	NdrFcShort( 0x10 ),	/* 16 */
/* 470 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 472 */	NdrFcShort( 0x28 ),	/* 40 */
/* 474 */	NdrFcShort( 0x8 ),	/* 8 */
/* 476 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x6,		/* 6 */
/* 478 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 480 */	NdrFcShort( 0x0 ),	/* 0 */
/* 482 */	NdrFcShort( 0x0 ),	/* 0 */
/* 484 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter x */

/* 486 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 488 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 490 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 492 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 494 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 496 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 498 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 500 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 502 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter stepCnt */

/* 504 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 506 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 508 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter coordEnum */

/* 510 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 512 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 514 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 516 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 518 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 520 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ShowDefaultLogo */

/* 522 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 524 */	NdrFcLong( 0x0 ),	/* 0 */
/* 528 */	NdrFcShort( 0x11 ),	/* 17 */
/* 530 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 532 */	NdrFcShort( 0x8 ),	/* 8 */
/* 534 */	NdrFcShort( 0x8 ),	/* 8 */
/* 536 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 538 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 540 */	NdrFcShort( 0x0 ),	/* 0 */
/* 542 */	NdrFcShort( 0x0 ),	/* 0 */
/* 544 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 546 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 548 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 552 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 554 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 556 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetControlStatus */

/* 558 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 560 */	NdrFcLong( 0x0 ),	/* 0 */
/* 564 */	NdrFcShort( 0x12 ),	/* 18 */
/* 566 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 568 */	NdrFcShort( 0x8 ),	/* 8 */
/* 570 */	NdrFcShort( 0x8 ),	/* 8 */
/* 572 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 574 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 580 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 582 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 584 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 586 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 588 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 590 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 592 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCameraClip */

/* 594 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 596 */	NdrFcLong( 0x0 ),	/* 0 */
/* 600 */	NdrFcShort( 0x13 ),	/* 19 */
/* 602 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 604 */	NdrFcShort( 0x10 ),	/* 16 */
/* 606 */	NdrFcShort( 0x8 ),	/* 8 */
/* 608 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 610 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0x4 ),	/* 4 */
/* 616 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cameraName */

/* 618 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 620 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 622 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter nearClip */

/* 624 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 626 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 628 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter farClip */

/* 630 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 632 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 634 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 636 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 638 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 640 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCurrentCameraClip */

/* 642 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 644 */	NdrFcLong( 0x0 ),	/* 0 */
/* 648 */	NdrFcShort( 0x14 ),	/* 20 */
/* 650 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 652 */	NdrFcShort( 0x10 ),	/* 16 */
/* 654 */	NdrFcShort( 0x8 ),	/* 8 */
/* 656 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 658 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 660 */	NdrFcShort( 0x0 ),	/* 0 */
/* 662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 664 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter nearClip */

/* 666 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 668 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 670 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter farClip */

/* 672 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 674 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 676 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 678 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 680 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 682 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DollyCurrentCamera */

/* 684 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 686 */	NdrFcLong( 0x0 ),	/* 0 */
/* 690 */	NdrFcShort( 0x15 ),	/* 21 */
/* 692 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 694 */	NdrFcShort( 0x10 ),	/* 16 */
/* 696 */	NdrFcShort( 0x8 ),	/* 8 */
/* 698 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 700 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 708 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 710 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 712 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter stepCnt */

/* 714 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 716 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 718 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 720 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 722 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 724 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DollyCamera */

/* 726 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 728 */	NdrFcLong( 0x0 ),	/* 0 */
/* 732 */	NdrFcShort( 0x16 ),	/* 22 */
/* 734 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 736 */	NdrFcShort( 0x10 ),	/* 16 */
/* 738 */	NdrFcShort( 0x8 ),	/* 8 */
/* 740 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 742 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 744 */	NdrFcShort( 0x0 ),	/* 0 */
/* 746 */	NdrFcShort( 0x4 ),	/* 4 */
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cameraName */

/* 750 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 752 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 754 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter value */

/* 756 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 758 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 760 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter stepCnt */

/* 762 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 764 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 766 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 768 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 770 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 772 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBackgroundColor */

/* 774 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 776 */	NdrFcLong( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x17 ),	/* 23 */
/* 782 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 784 */	NdrFcShort( 0x18 ),	/* 24 */
/* 786 */	NdrFcShort( 0x8 ),	/* 8 */
/* 788 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 790 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 792 */	NdrFcShort( 0x0 ),	/* 0 */
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 796 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter red */

/* 798 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 800 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 802 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 804 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 806 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 808 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 810 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 812 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 814 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 816 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 818 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 820 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PreloadN3DFile */

/* 822 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 824 */	NdrFcLong( 0x0 ),	/* 0 */
/* 828 */	NdrFcShort( 0x18 ),	/* 24 */
/* 830 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 832 */	NdrFcShort( 0x0 ),	/* 0 */
/* 834 */	NdrFcShort( 0x8 ),	/* 8 */
/* 836 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 838 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 842 */	NdrFcShort( 0x8 ),	/* 8 */
/* 844 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strN3DFileName */

/* 846 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 848 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 850 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter N3DName */

/* 852 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 854 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 856 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 858 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 860 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 862 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GotoN3D */

/* 864 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 866 */	NdrFcLong( 0x0 ),	/* 0 */
/* 870 */	NdrFcShort( 0x19 ),	/* 25 */
/* 872 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x8 ),	/* 8 */
/* 878 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 880 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 882 */	NdrFcShort( 0x0 ),	/* 0 */
/* 884 */	NdrFcShort( 0x4 ),	/* 4 */
/* 886 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter N3DName */

/* 888 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 890 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 892 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 894 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 896 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 898 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCurrentCameraByName */

/* 900 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 902 */	NdrFcLong( 0x0 ),	/* 0 */
/* 906 */	NdrFcShort( 0x1a ),	/* 26 */
/* 908 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 910 */	NdrFcShort( 0x0 ),	/* 0 */
/* 912 */	NdrFcShort( 0x8 ),	/* 8 */
/* 914 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 916 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 918 */	NdrFcShort( 0x0 ),	/* 0 */
/* 920 */	NdrFcShort( 0x4 ),	/* 4 */
/* 922 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter camName */

/* 924 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 926 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 928 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 930 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 932 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 934 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayCameraKfr */

/* 936 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 938 */	NdrFcLong( 0x0 ),	/* 0 */
/* 942 */	NdrFcShort( 0x1b ),	/* 27 */
/* 944 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 946 */	NdrFcShort( 0x8 ),	/* 8 */
/* 948 */	NdrFcShort( 0x8 ),	/* 8 */
/* 950 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 952 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 954 */	NdrFcShort( 0x0 ),	/* 0 */
/* 956 */	NdrFcShort( 0x4 ),	/* 4 */
/* 958 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter camName */

/* 960 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 962 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 964 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter repeatCnt */

/* 966 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 968 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 970 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 972 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 974 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 976 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCurrentCameraFrom */

/* 978 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 980 */	NdrFcLong( 0x0 ),	/* 0 */
/* 984 */	NdrFcShort( 0x1c ),	/* 28 */
/* 986 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 988 */	NdrFcShort( 0x18 ),	/* 24 */
/* 990 */	NdrFcShort( 0x8 ),	/* 8 */
/* 992 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 994 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 996 */	NdrFcShort( 0x0 ),	/* 0 */
/* 998 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1000 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter x */

/* 1002 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1004 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1006 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 1008 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1010 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1012 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 1014 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1016 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1018 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 1020 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1022 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1024 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCurrentCameraAt */

/* 1026 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1028 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1032 */	NdrFcShort( 0x1d ),	/* 29 */
/* 1034 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1036 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1038 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1040 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 1042 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1044 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1046 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1048 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter x */

/* 1050 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1052 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1054 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 1056 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1058 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1060 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 1062 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1064 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1066 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 1068 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1070 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1072 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StopCameraKfr */

/* 1074 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1076 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1080 */	NdrFcShort( 0x1e ),	/* 30 */
/* 1082 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1084 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1086 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1088 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1090 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1092 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1094 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1096 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter camName */

/* 1098 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1100 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1102 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 1104 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1106 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1108 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetServer */

/* 1110 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1112 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1116 */	NdrFcShort( 0x1f ),	/* 31 */
/* 1118 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1122 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1124 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1126 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1130 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1132 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strServer */

/* 1134 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1136 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1138 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 1140 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1142 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1144 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelClickable */

/* 1146 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1148 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1152 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1154 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1156 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1158 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1160 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1162 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1166 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1168 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 1170 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1172 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1174 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter clickable */

/* 1176 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1178 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1180 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1182 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1184 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1186 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateOverlayFromImage */

/* 1188 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1190 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1194 */	NdrFcShort( 0x21 ),	/* 33 */
/* 1196 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 1198 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1200 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1202 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x9,		/* 9 */
/* 1204 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1206 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1208 */	NdrFcShort( 0xc ),	/* 12 */
/* 1210 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 1212 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1214 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1216 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter oriImageName */

/* 1218 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1220 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1222 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter alphaImageName */

/* 1224 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1226 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1228 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter alignMode */

/* 1230 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1232 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1234 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter left */

/* 1236 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1238 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1240 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter top */

/* 1242 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1244 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1246 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter width */

/* 1248 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1250 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1252 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 1254 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1256 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1260 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1262 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1264 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteOverlay */

/* 1266 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1268 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1272 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1274 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1278 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1280 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1282 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1284 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1286 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1288 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 1290 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1292 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1294 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 1296 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1298 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1300 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetOverlayOpacity */

/* 1302 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1304 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1308 */	NdrFcShort( 0x23 ),	/* 35 */
/* 1310 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1312 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1314 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1316 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1318 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1322 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1324 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 1326 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1328 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1330 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter opacity */

/* 1332 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1334 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1336 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1338 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1340 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1342 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateButtonFromImage */

/* 1344 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1346 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1350 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1352 */	NdrFcShort( 0x3c ),	/* x86 Stack size/offset = 60 */
/* 1354 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1356 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1358 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0xe,		/* 14 */
/* 1360 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1362 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1364 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1366 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter btnName */

/* 1368 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1370 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1372 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter mouseUpImage */

/* 1374 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1376 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1378 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter mouseUpImage_alpha */

/* 1380 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1382 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1384 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter mouseOverImage */

/* 1386 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1388 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1390 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter mouseOverImage_alpha */

/* 1392 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1394 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1396 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter mouseDownImage */

/* 1398 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1400 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1402 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter mouseDownImage_alpha */

/* 1404 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1406 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1408 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter alignMode */

/* 1410 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1412 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1414 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter x */

/* 1416 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1418 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1420 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter y */

/* 1422 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1424 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 1426 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter width */

/* 1428 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1430 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 1432 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 1434 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1436 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 1438 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter callBackFunc */

/* 1440 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1442 */	NdrFcShort( 0x34 ),	/* x86 Stack size/offset = 52 */
/* 1444 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 1446 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1448 */	NdrFcShort( 0x38 ),	/* x86 Stack size/offset = 56 */
/* 1450 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteButton */

/* 1452 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1454 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1458 */	NdrFcShort( 0x25 ),	/* 37 */
/* 1460 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1464 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1466 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1468 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1472 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1474 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter btnName */

/* 1476 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1478 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1480 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 1482 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1484 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1486 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TranslateModel */

/* 1488 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1490 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1494 */	NdrFcShort( 0x26 ),	/* 38 */
/* 1496 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1498 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1500 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1502 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1504 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1506 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1508 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1510 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 1512 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1514 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1516 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 1518 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1520 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1522 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 1524 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1526 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1528 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 1530 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1532 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1534 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter coord */

/* 1536 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1538 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1540 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1542 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1544 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1546 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetEditCtrl_DollySpeed */

/* 1548 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1550 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1554 */	NdrFcShort( 0x27 ),	/* 39 */
/* 1556 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1558 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1560 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1562 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1564 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1566 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1568 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1570 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dollySpeed */

/* 1572 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1574 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1576 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 1578 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1580 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1582 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetEditCtrl_RosAxis */

/* 1584 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1586 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1590 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1592 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1594 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1596 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1598 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1600 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1602 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1606 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter rosFlag */

/* 1608 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1610 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1612 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1614 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1616 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1618 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetEditCtrl_DollyDistance */

/* 1620 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1622 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1626 */	NdrFcShort( 0x29 ),	/* 41 */
/* 1628 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1630 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1632 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1634 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1636 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1638 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1642 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter nearDis */

/* 1644 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1646 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1648 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter farDis */

/* 1650 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1652 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1654 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 1656 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1658 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1660 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetEditCtrl_AutoPitchRoundRoll */

/* 1662 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1664 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1668 */	NdrFcShort( 0x2a ),	/* 42 */
/* 1670 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1672 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1674 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1676 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1678 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1680 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1682 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1684 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 1686 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1688 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1690 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1692 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1694 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1696 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetEditCtrl_AutoParam */

/* 1698 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1700 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1704 */	NdrFcShort( 0x2b ),	/* 43 */
/* 1706 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1708 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1710 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1712 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 1714 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1716 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1720 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter PitchSpeedValue */

/* 1722 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1724 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1726 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter RoundSpeedValue */

/* 1728 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1730 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1732 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter RollSpeedValue */

/* 1734 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1736 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1738 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 1740 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1742 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1744 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetEditCtrl_ManualParam */

/* 1746 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1748 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1752 */	NdrFcShort( 0x2c ),	/* 44 */
/* 1754 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1756 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1758 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1760 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1762 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1766 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1768 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter RoundSpeed */

/* 1770 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1772 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1774 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter RollSpeed */

/* 1776 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1778 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1780 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 1782 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1784 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1786 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveCurrentCameraTo */

/* 1788 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1790 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1794 */	NdrFcShort( 0x2d ),	/* 45 */
/* 1796 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 1798 */	NdrFcShort( 0x40 ),	/* 64 */
/* 1800 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1802 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x9,		/* 9 */
/* 1804 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1806 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1808 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1810 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter from_x */

/* 1812 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1814 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1816 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter from_y */

/* 1818 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1820 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1822 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter from_z */

/* 1824 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1826 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1828 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter at_x */

/* 1830 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1832 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1834 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter at_y */

/* 1836 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1838 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1840 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter at_z */

/* 1842 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1844 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1846 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter frameCnt */

/* 1848 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1850 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1852 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter accType */

/* 1854 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1856 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1858 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1860 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1862 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 1864 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFBL */

/* 1866 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1868 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1872 */	NdrFcShort( 0x2e ),	/* 46 */
/* 1874 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1876 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1878 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1880 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1882 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1884 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1886 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1888 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter fblValue */

/* 1890 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1892 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1894 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 1896 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1898 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1900 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetOverlayWidth */

/* 1902 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1904 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1908 */	NdrFcShort( 0x2f ),	/* 47 */
/* 1910 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1912 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1914 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1916 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1918 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1922 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1924 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 1926 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1928 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1930 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter width */

/* 1932 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1934 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1936 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1938 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1940 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1942 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetOverlayHeight */

/* 1944 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1946 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1950 */	NdrFcShort( 0x30 ),	/* 48 */
/* 1952 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1954 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1956 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1958 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1960 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1962 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1964 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1966 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 1968 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1970 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1972 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter height */

/* 1974 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1976 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1978 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1980 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1982 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1984 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetOverlayAlignMode */

/* 1986 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1988 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1992 */	NdrFcShort( 0x31 ),	/* 49 */
/* 1994 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1996 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1998 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2000 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2002 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2004 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2006 */	NdrFcShort( 0x4 ),	/* 4 */
/* 2008 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 2010 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2012 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2014 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter alignMode */

/* 2016 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2018 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2020 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2022 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2024 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2026 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetOverlayRect */

/* 2028 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2030 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2034 */	NdrFcShort( 0x32 ),	/* 50 */
/* 2036 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2038 */	NdrFcShort( 0x20 ),	/* 32 */
/* 2040 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2042 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 2044 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2046 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2048 */	NdrFcShort( 0x4 ),	/* 4 */
/* 2050 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 2052 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2054 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2056 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter left */

/* 2058 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2060 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2062 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter top */

/* 2064 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2066 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2068 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter width */

/* 2070 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2072 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2074 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 2076 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2078 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2080 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2082 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2084 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2086 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveOverlay */

/* 2088 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2090 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2094 */	NdrFcShort( 0x33 ),	/* 51 */
/* 2096 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2098 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2100 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2102 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2104 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2108 */	NdrFcShort( 0x4 ),	/* 4 */
/* 2110 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 2112 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2114 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2116 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 2118 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2120 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2122 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter y */

/* 2124 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2126 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2130 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2132 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2134 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveCurrentCameraFromTo */

/* 2136 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2138 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2142 */	NdrFcShort( 0x34 ),	/* 52 */
/* 2144 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2146 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2148 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2150 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 2152 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2158 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter x */

/* 2160 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2162 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2164 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 2166 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2168 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2170 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 2172 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2174 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2176 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 2178 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2180 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2182 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveCurrentCameraAtTo */

/* 2184 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2186 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2190 */	NdrFcShort( 0x35 ),	/* 53 */
/* 2192 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2194 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2196 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2198 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 2200 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2202 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2206 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter x */

/* 2208 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2210 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2212 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 2214 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2216 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2218 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 2220 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2222 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2224 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 2226 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2228 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2230 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveCurrentCameraFrom */

/* 2232 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2234 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2238 */	NdrFcShort( 0x36 ),	/* 54 */
/* 2240 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2242 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2244 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2246 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 2248 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2250 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2252 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2254 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter x */

/* 2256 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2258 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2260 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 2262 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2264 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2266 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 2268 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2270 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2272 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 2274 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2276 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2278 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveCurrentCameraAt */

/* 2280 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2282 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2286 */	NdrFcShort( 0x37 ),	/* 55 */
/* 2288 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2290 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2292 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2294 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 2296 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2300 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2302 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter x */

/* 2304 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2306 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2308 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 2310 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2312 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2314 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 2316 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2318 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2320 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 2322 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2324 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2326 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure LoadN3DFile */

/* 2328 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2330 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2334 */	NdrFcShort( 0x38 ),	/* 56 */
/* 2336 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2338 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2340 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2342 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2344 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2346 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2348 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2350 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strN3DFileName */

/* 2352 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2354 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2356 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter N3DName */

/* 2358 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2360 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2362 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 2364 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2366 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2368 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ResetScene */

/* 2370 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2372 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2376 */	NdrFcShort( 0x39 ),	/* 57 */
/* 2378 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2382 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2384 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2386 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2392 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2394 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2396 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2398 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelMtrl */

/* 2400 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2402 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2406 */	NdrFcShort( 0x3a ),	/* 58 */
/* 2408 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2410 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2412 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2414 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2416 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2418 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2420 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2422 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 2424 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2426 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2428 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter subMeshID */

/* 2430 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2432 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2434 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter mtrlName */

/* 2436 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2438 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2440 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 2442 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2444 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2446 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMtrlOpacity */

/* 2448 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2450 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2454 */	NdrFcShort( 0x3b ),	/* 59 */
/* 2456 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2458 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2460 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2462 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2464 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2468 */	NdrFcShort( 0x4 ),	/* 4 */
/* 2470 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 2472 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2474 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2476 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter opacity */

/* 2478 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2480 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2482 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2484 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2486 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2488 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCurrentCameraByIndex */

/* 2490 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2492 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2496 */	NdrFcShort( 0x3c ),	/* 60 */
/* 2498 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2500 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2502 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2504 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2506 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2508 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2512 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter camIndex */

/* 2514 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2516 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2518 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2520 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2522 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2524 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetWalkCtrl_ManualParam */

/* 2526 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2528 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2532 */	NdrFcShort( 0x3d ),	/* 61 */
/* 2534 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2536 */	NdrFcShort( 0x28 ),	/* 40 */
/* 2538 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2540 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x6,		/* 6 */
/* 2542 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2544 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2546 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2548 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter moveSpeed */

/* 2550 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2552 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2554 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter moveAcc */

/* 2556 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2558 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2560 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter rotateSpeed */

/* 2562 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2564 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2566 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter rotateAcc */

/* 2568 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2570 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2572 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter height */

/* 2574 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2576 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2578 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 2580 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2582 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2584 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelMouseHint */

/* 2586 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2588 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2592 */	NdrFcShort( 0x3e ),	/* 62 */
/* 2594 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2596 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2598 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2600 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2602 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2606 */	NdrFcShort( 0x4 ),	/* 4 */
/* 2608 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 2610 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2612 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2614 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter bFlag */

/* 2616 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2618 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2620 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2622 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2624 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2626 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EnableMouseDownCallback */

/* 2628 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2630 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2634 */	NdrFcShort( 0x3f ),	/* 63 */
/* 2636 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2638 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2640 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2642 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2644 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2650 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2652 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2654 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2656 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DisableMouseDownCallback */

/* 2658 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2660 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2664 */	NdrFcShort( 0x40 ),	/* 64 */
/* 2666 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2668 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2670 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2672 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2674 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2678 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2680 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2682 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2684 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2686 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EnableMouseUpCallback */

/* 2688 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2690 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2694 */	NdrFcShort( 0x41 ),	/* 65 */
/* 2696 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2700 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2702 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2704 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2706 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2708 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2710 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2712 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2714 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2716 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DisableMouseUpCallback */

/* 2718 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2720 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2724 */	NdrFcShort( 0x42 ),	/* 66 */
/* 2726 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2730 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2732 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2734 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2736 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2740 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2742 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2744 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2746 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EnableMouseDBClickCallback */

/* 2748 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2750 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2754 */	NdrFcShort( 0x43 ),	/* 67 */
/* 2756 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2758 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2760 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2762 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2764 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2766 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2768 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2770 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2772 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2774 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2776 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DisableMouseDBClickCallback */

/* 2778 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2780 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2784 */	NdrFcShort( 0x44 ),	/* 68 */
/* 2786 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2790 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2792 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2794 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2796 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2798 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2800 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2802 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2804 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2806 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EnableMouseMoveCallback */

/* 2808 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2810 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2814 */	NdrFcShort( 0x45 ),	/* 69 */
/* 2816 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2818 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2820 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2822 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2824 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2826 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2828 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2830 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2832 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2834 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2836 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DisableMouseMoveCallback */

/* 2838 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2840 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2844 */	NdrFcShort( 0x46 ),	/* 70 */
/* 2846 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2848 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2850 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2852 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2854 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2856 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2858 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2860 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2862 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2864 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2866 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelMouseMtrl */

/* 2868 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2870 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2874 */	NdrFcShort( 0x47 ),	/* 71 */
/* 2876 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2878 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2880 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2882 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2884 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2886 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2888 */	NdrFcShort( 0xc ),	/* 12 */
/* 2890 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 2892 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2894 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2896 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter mouseOverMtrl */

/* 2898 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2900 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2902 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter mouseDownMtrl */

/* 2904 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2906 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2908 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 2910 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2912 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2914 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelOpacity */

/* 2916 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2918 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2922 */	NdrFcShort( 0x48 ),	/* 72 */
/* 2924 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2926 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2928 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2930 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2932 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2934 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2936 */	NdrFcShort( 0x4 ),	/* 4 */
/* 2938 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 2940 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2942 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2944 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter opacity */

/* 2946 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2948 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2950 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2952 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2954 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2956 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateModelGroup */

/* 2958 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2960 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2964 */	NdrFcShort( 0x49 ),	/* 73 */
/* 2966 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2968 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2970 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2972 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2974 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2976 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2978 */	NdrFcShort( 0x4 ),	/* 4 */
/* 2980 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelGroupName */

/* 2982 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2984 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2986 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 2988 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2990 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2992 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateBumpWaterTexture */

/* 2994 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2996 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3000 */	NdrFcShort( 0x4a ),	/* 74 */
/* 3002 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 3004 */	NdrFcShort( 0x40 ),	/* 64 */
/* 3006 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3008 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0xa,		/* 10 */
/* 3010 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3012 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3014 */	NdrFcShort( 0x4 ),	/* 4 */
/* 3016 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bumpWaterName */

/* 3018 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3020 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3022 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter width */

/* 3024 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3026 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3028 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 3030 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3032 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3034 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter xSize */

/* 3036 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3038 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3040 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ySize */

/* 3042 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3044 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3046 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter radius */

/* 3048 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3050 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3052 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter speed */

/* 3054 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3056 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3058 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter centerX */

/* 3060 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3062 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3064 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter centerY */

/* 3066 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3068 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 3070 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 3072 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3074 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 3076 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RotateCurrentCamera */

/* 3078 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3080 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3084 */	NdrFcShort( 0x4b ),	/* 75 */
/* 3086 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3088 */	NdrFcShort( 0x28 ),	/* 40 */
/* 3090 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3092 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x6,		/* 6 */
/* 3094 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3096 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3098 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3100 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter x */

/* 3102 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3104 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3106 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 3108 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3110 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3112 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 3114 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3118 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter stepCnt */

/* 3120 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3122 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter coordEnum */

/* 3126 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3128 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3132 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3134 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3136 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RotateCamera */

/* 3138 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3140 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3144 */	NdrFcShort( 0x4c ),	/* 76 */
/* 3146 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3148 */	NdrFcShort( 0x28 ),	/* 40 */
/* 3150 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3152 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 3154 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3158 */	NdrFcShort( 0x4 ),	/* 4 */
/* 3160 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cameraName */

/* 3162 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3164 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3166 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 3168 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3170 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3172 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 3174 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3176 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3178 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 3180 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3182 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3184 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter stepCnt */

/* 3186 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3188 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3190 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter coordEnum */

/* 3192 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3194 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3196 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3198 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3200 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3202 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure FocusCurrentCameraTo */

/* 3204 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3206 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3210 */	NdrFcShort( 0x4d ),	/* 77 */
/* 3212 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 3214 */	NdrFcShort( 0x28 ),	/* 40 */
/* 3216 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3218 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 3220 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3222 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3224 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3226 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter model1Name */

/* 3228 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3230 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3232 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter model2Name */

/* 3234 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3236 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3238 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 3240 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3242 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3244 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 3246 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3248 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3250 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 3252 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3254 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3256 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter distance */

/* 3258 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3260 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3262 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter stepCnt */

/* 3264 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3266 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3268 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3270 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3272 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3274 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure HideAllModels */

/* 3276 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3278 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3282 */	NdrFcShort( 0x4e ),	/* 78 */
/* 3284 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3288 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3290 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3292 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3294 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3296 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3298 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3300 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3302 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3304 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ShowAllModels */

/* 3306 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3308 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3312 */	NdrFcShort( 0x4f ),	/* 79 */
/* 3314 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3318 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3320 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3322 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3326 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3328 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3330 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3332 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetModelHandleByName */

/* 3336 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3338 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3342 */	NdrFcShort( 0x50 ),	/* 80 */
/* 3344 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3346 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3348 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3350 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3352 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3354 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3356 */	NdrFcShort( 0xa6 ),	/* 166 */
/* 3358 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 3360 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3362 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3364 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter modelHandle */

/* 3366 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3368 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3370 */	NdrFcShort( 0x40e ),	/* Type Offset=1038 */

	/* Return value */

/* 3372 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3374 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3376 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RestoreModelMtrl */

/* 3378 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3380 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3384 */	NdrFcShort( 0x51 ),	/* 81 */
/* 3386 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3390 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3392 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 3394 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3396 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3398 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3400 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 3402 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3404 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3406 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 3408 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3410 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3412 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RestoreAllModelsMtrl */

/* 3414 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3416 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3420 */	NdrFcShort( 0x52 ),	/* 82 */
/* 3422 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3424 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3426 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3428 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3430 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3432 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3434 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3436 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3438 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3440 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateMtrl */

/* 3444 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3446 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3450 */	NdrFcShort( 0x53 ),	/* 83 */
/* 3452 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3454 */	NdrFcShort( 0x28 ),	/* 40 */
/* 3456 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3458 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 3460 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3464 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3466 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 3468 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3470 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3472 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter red */

/* 3474 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3476 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3478 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 3480 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3482 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3484 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 3486 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3488 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3490 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter alpha */

/* 3492 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3494 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3496 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter selfIllu */

/* 3498 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3500 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3502 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3504 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3506 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3508 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMtrlColor */

/* 3510 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3512 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3516 */	NdrFcShort( 0x54 ),	/* 84 */
/* 3518 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3520 */	NdrFcShort( 0x20 ),	/* 32 */
/* 3522 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3524 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 3526 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3528 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3530 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3532 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 3534 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3536 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3538 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter red */

/* 3540 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3542 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3544 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 3546 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3548 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 3552 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3554 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3556 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter alpha */

/* 3558 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3560 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3562 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3564 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3566 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3568 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteMtrl */

/* 3570 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3572 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3576 */	NdrFcShort( 0x55 ),	/* 85 */
/* 3578 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3580 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3582 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3584 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 3586 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3588 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3590 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3592 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 3594 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3596 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3598 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 3600 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3602 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3604 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMtrlIllumination */

/* 3606 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3608 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3612 */	NdrFcShort( 0x56 ),	/* 86 */
/* 3614 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3616 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3618 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3620 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3622 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3624 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3626 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3628 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 3630 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3632 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3634 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter illumination */

/* 3636 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3638 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3640 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3642 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3644 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3646 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveModel */

/* 3648 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3650 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3654 */	NdrFcShort( 0x57 ),	/* 87 */
/* 3656 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3658 */	NdrFcShort( 0x20 ),	/* 32 */
/* 3660 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3662 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 3664 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3668 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3670 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 3672 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3674 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3676 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 3678 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3680 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3682 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 3684 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3686 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3688 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 3690 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3692 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3694 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter coordEnum */

/* 3696 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3698 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3700 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3702 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3704 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3706 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveModelTo */

/* 3708 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3710 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3714 */	NdrFcShort( 0x58 ),	/* 88 */
/* 3716 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 3718 */	NdrFcShort( 0x28 ),	/* 40 */
/* 3720 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3722 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 3724 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3728 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3730 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 3732 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3734 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3736 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 3738 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3740 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3742 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 3744 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3746 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3748 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 3750 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3752 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3754 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter frameCnt */

/* 3756 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3758 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3760 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter accModel */

/* 3762 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3764 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3766 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3768 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3770 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3772 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RestoreModelOpacity */

/* 3774 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3776 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3780 */	NdrFcShort( 0x59 ),	/* 89 */
/* 3782 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3786 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3788 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 3790 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3792 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3794 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3796 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 3798 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3800 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3802 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 3804 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3806 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3808 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RestoreAllModelsOpacity */

/* 3810 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3812 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3816 */	NdrFcShort( 0x5a ),	/* 90 */
/* 3818 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3820 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3822 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3824 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3826 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3828 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3830 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3832 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3834 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3836 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3838 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerGoForward */

/* 3840 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3842 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3846 */	NdrFcShort( 0x5b ),	/* 91 */
/* 3848 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3850 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3852 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3854 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3856 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3858 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3860 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3862 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3864 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3866 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3868 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerGoBackward */

/* 3870 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3872 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3876 */	NdrFcShort( 0x5c ),	/* 92 */
/* 3878 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3880 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3882 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3884 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3886 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3888 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3890 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3892 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3894 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3896 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3898 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerGoLeftward */

/* 3900 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3902 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3906 */	NdrFcShort( 0x5d ),	/* 93 */
/* 3908 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3910 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3912 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3914 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3916 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3918 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3922 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3924 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3926 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3928 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerGoRightward */

/* 3930 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3932 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3936 */	NdrFcShort( 0x5e ),	/* 94 */
/* 3938 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3940 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3942 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3944 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3946 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3948 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3950 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3952 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3954 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3956 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3958 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerStop */

/* 3960 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3962 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3966 */	NdrFcShort( 0x5f ),	/* 95 */
/* 3968 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3970 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3972 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3974 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3976 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3978 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3980 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3982 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3984 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3986 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3988 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerTurnLeft */

/* 3990 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3992 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3996 */	NdrFcShort( 0x60 ),	/* 96 */
/* 3998 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4000 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4002 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4004 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 4006 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4008 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4010 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4012 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 4014 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4016 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4018 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerTurnRight */

/* 4020 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4022 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4026 */	NdrFcShort( 0x61 ),	/* 97 */
/* 4028 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4030 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4032 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4034 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 4036 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4038 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4040 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4042 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 4044 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4046 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4048 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerTurnUp */

/* 4050 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4052 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4056 */	NdrFcShort( 0x62 ),	/* 98 */
/* 4058 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4060 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4062 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4064 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 4066 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4068 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4070 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4072 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 4074 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4076 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4078 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerTurnDown */

/* 4080 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4082 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4086 */	NdrFcShort( 0x63 ),	/* 99 */
/* 4088 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4090 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4092 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4094 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 4096 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4098 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4102 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 4104 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4106 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4108 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerZoomIn */

/* 4110 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4112 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4116 */	NdrFcShort( 0x64 ),	/* 100 */
/* 4118 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4122 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4124 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 4126 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4130 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4132 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 4134 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4136 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4138 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerZoomOut */

/* 4140 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4142 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4146 */	NdrFcShort( 0x65 ),	/* 101 */
/* 4148 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4152 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4154 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 4156 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4158 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4162 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 4164 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4166 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4168 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerGoUp */

/* 4170 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4172 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4176 */	NdrFcShort( 0x66 ),	/* 102 */
/* 4178 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4180 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4182 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4184 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 4186 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4188 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4192 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 4194 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4196 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4198 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayerGoDown */

/* 4200 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4202 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4206 */	NdrFcShort( 0x67 ),	/* 103 */
/* 4208 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4212 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4214 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 4216 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4218 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4222 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 4224 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4226 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4228 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelColor */

/* 4230 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4232 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4236 */	NdrFcShort( 0x68 ),	/* 104 */
/* 4238 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 4240 */	NdrFcShort( 0x20 ),	/* 32 */
/* 4242 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4244 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 4246 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4250 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4252 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 4254 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4256 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4258 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter red */

/* 4260 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4262 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4264 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 4266 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4268 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4270 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 4272 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4274 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4276 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter alpha */

/* 4278 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4280 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4282 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 4284 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4286 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4288 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelFBLStatus */

/* 4290 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4292 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4296 */	NdrFcShort( 0x69 ),	/* 105 */
/* 4298 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4300 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4302 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4304 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 4306 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4310 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4312 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 4314 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4316 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4318 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 4320 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4322 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4324 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 4326 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4328 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4330 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBumpWaterSpeed */

/* 4332 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4334 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4338 */	NdrFcShort( 0x6a ),	/* 106 */
/* 4340 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4342 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4344 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4346 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 4348 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4352 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4354 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter waterName */

/* 4356 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4358 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4360 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter speed */

/* 4362 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4364 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4366 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 4368 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4370 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4372 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBumpWaterRadius */

/* 4374 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4376 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4380 */	NdrFcShort( 0x6b ),	/* 107 */
/* 4382 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4384 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4386 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4388 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 4390 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4394 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4396 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter waterName */

/* 4398 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4400 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4402 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter radius */

/* 4404 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4406 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4408 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 4410 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4412 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4414 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetSkyBoxModel */

/* 4416 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4418 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4422 */	NdrFcShort( 0x6c ),	/* 108 */
/* 4424 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4426 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4428 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4430 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 4432 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4434 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4436 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4438 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 4440 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4442 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4444 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 4446 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4448 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4450 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 4452 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4454 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4456 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCameraFrom */

/* 4458 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4460 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4464 */	NdrFcShort( 0x6d ),	/* 109 */
/* 4466 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4468 */	NdrFcShort( 0x18 ),	/* 24 */
/* 4470 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4472 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 4474 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4478 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4480 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cameraName */

/* 4482 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4484 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4486 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 4488 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4490 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4492 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 4494 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4496 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4498 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 4500 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4502 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4504 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 4506 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4508 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4510 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCameraAt */

/* 4512 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4514 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4518 */	NdrFcShort( 0x6e ),	/* 110 */
/* 4520 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4522 */	NdrFcShort( 0x18 ),	/* 24 */
/* 4524 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4526 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 4528 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4530 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4532 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4534 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cameraName */

/* 4536 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4538 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4540 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 4542 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4544 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4546 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 4548 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4550 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4552 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 4554 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4556 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4558 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 4560 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4562 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4564 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCameraFov */

/* 4566 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4568 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4572 */	NdrFcShort( 0x6f ),	/* 111 */
/* 4574 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4576 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4578 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4580 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 4582 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4586 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4588 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cameraName */

/* 4590 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4592 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4594 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter fov */

/* 4596 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4598 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4600 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 4602 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4604 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4606 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCurrentCameraFov */

/* 4608 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4610 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4614 */	NdrFcShort( 0x70 ),	/* 112 */
/* 4616 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4618 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4620 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4622 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 4624 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4626 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4628 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4630 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter fov */

/* 4632 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4634 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4636 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 4638 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4640 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4642 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateFSMotionBlur */

/* 4644 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4646 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4650 */	NdrFcShort( 0x71 ),	/* 113 */
/* 4652 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4654 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4656 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4658 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 4660 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4666 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter blurCount */

/* 4668 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4670 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4672 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 4674 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4676 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4678 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFSMotionBlurPower */

/* 4680 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4682 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4686 */	NdrFcShort( 0x72 ),	/* 114 */
/* 4688 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4690 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4692 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4694 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 4696 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4702 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 4704 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4706 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4708 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 4710 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4712 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4714 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StartFSMotionBlur */

/* 4716 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4718 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4722 */	NdrFcShort( 0x73 ),	/* 115 */
/* 4724 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4728 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4730 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 4732 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4734 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4736 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4738 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 4740 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4742 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4744 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StopFSMotionBlur */

/* 4746 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4748 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4752 */	NdrFcShort( 0x74 ),	/* 116 */
/* 4754 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4758 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4760 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 4762 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4766 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4768 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 4770 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4772 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4774 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCameraKFRFSMotionBlur */

/* 4776 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4778 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4782 */	NdrFcShort( 0x75 ),	/* 117 */
/* 4784 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 4786 */	NdrFcShort( 0x20 ),	/* 32 */
/* 4788 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4790 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 4792 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4796 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4798 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter camName */

/* 4800 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4802 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4804 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter startFrame */

/* 4806 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4808 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4810 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter endFrame */

/* 4812 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4814 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4816 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter power */

/* 4818 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4820 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter status */

/* 4824 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4826 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4828 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 4830 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4832 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4834 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFlashCurrentFrame */

/* 4836 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4838 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4842 */	NdrFcShort( 0x76 ),	/* 118 */
/* 4844 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4846 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4848 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4850 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 4852 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4854 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4856 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4858 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter flashName */

/* 4860 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4862 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4864 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter frameIndex */

/* 4866 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4868 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4870 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 4872 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4874 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4876 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCameraKfrCallBackFunc */

/* 4878 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4880 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4884 */	NdrFcShort( 0x77 ),	/* 119 */
/* 4886 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4888 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4890 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4892 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 4894 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 4896 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4898 */	NdrFcShort( 0xc ),	/* 12 */
/* 4900 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter camName */

/* 4902 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4904 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4906 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter frameIndex */

/* 4908 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4910 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4912 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter funcName */

/* 4914 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 4916 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4918 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 4920 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4922 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4924 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateLensFlare */

/* 4926 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4928 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4932 */	NdrFcShort( 0x78 ),	/* 120 */
/* 4934 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 4936 */	NdrFcShort( 0x20 ),	/* 32 */
/* 4938 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4940 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x5,		/* 5 */
/* 4942 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4944 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4946 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4948 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter x */

/* 4950 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4952 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4954 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 4956 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4958 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4960 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 4962 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4964 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4966 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter coverFlag */

/* 4968 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4970 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4972 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 4974 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4976 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4978 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateFog */

/* 4980 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4982 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4986 */	NdrFcShort( 0x79 ),	/* 121 */
/* 4988 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 4990 */	NdrFcShort( 0x38 ),	/* 56 */
/* 4992 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4994 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x8,		/* 8 */
/* 4996 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4998 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5000 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5002 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter fogMode */

/* 5004 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5006 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5008 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter red */

/* 5010 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5012 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5014 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 5016 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5018 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5020 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 5022 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5024 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5026 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter density */

/* 5028 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5030 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 5032 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter start */

/* 5034 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5036 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 5038 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter end */

/* 5040 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5042 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 5044 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 5046 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5048 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 5050 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFogMode */

/* 5052 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5054 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5058 */	NdrFcShort( 0x7a ),	/* 122 */
/* 5060 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5062 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5064 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5066 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 5068 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 5070 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5072 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5074 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mode */

/* 5076 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5078 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5080 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5082 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5084 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5086 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFogColor */

/* 5088 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5090 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5094 */	NdrFcShort( 0x7b ),	/* 123 */
/* 5096 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 5098 */	NdrFcShort( 0x18 ),	/* 24 */
/* 5100 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5102 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 5104 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 5106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5108 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5110 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter red */

/* 5112 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5114 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5116 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 5118 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5120 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5122 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 5124 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5126 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5130 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5132 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5134 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFogDensity */

/* 5136 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5138 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5142 */	NdrFcShort( 0x7c ),	/* 124 */
/* 5144 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5146 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5148 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5150 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 5152 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 5154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5158 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter density */

/* 5160 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5162 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5164 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 5166 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5168 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5170 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFogField */

/* 5172 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5174 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5178 */	NdrFcShort( 0x7d ),	/* 125 */
/* 5180 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5182 */	NdrFcShort( 0x10 ),	/* 16 */
/* 5184 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5186 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 5188 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 5190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5194 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter start */

/* 5196 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5198 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5200 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter end */

/* 5202 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5204 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5206 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 5208 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5210 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5212 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFogRenderStatus */

/* 5214 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5216 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5220 */	NdrFcShort( 0x7e ),	/* 126 */
/* 5222 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5224 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5226 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5228 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 5230 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 5232 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5236 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 5238 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5240 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5242 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5244 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5246 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5248 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFilter */

/* 5250 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5252 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5256 */	NdrFcShort( 0x7f ),	/* 127 */
/* 5258 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 5260 */	NdrFcShort( 0x10 ),	/* 16 */
/* 5262 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5264 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 5266 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5268 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5270 */	NdrFcShort( 0x6 ),	/* 6 */
/* 5272 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 5274 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5276 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5278 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 5280 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5282 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5284 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter opacity */

/* 5286 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5288 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5290 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5292 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5294 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5296 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelMouseColor */

/* 5298 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5300 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5304 */	NdrFcShort( 0x80 ),	/* 128 */
/* 5306 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 5308 */	NdrFcShort( 0x40 ),	/* 64 */
/* 5310 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5312 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0xa,		/* 10 */
/* 5314 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5318 */	NdrFcShort( 0x6 ),	/* 6 */
/* 5320 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 5322 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5324 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5326 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter mouseOver_Red */

/* 5328 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5330 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5332 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter mouseOver_Green */

/* 5334 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5336 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5338 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter mouseOver_Blue */

/* 5340 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5342 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5344 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter mouseOver_Alpha */

/* 5346 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5348 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 5350 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter mouseClick_Red */

/* 5352 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5354 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 5356 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter mouseClick_Green */

/* 5358 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5360 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 5362 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter mouseClick_Blue */

/* 5364 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5366 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 5368 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter mouseClick_Alpha */

/* 5370 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5372 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 5374 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5376 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5378 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 5380 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMouseMoveEventTimer */

/* 5382 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5384 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5388 */	NdrFcShort( 0x81 ),	/* 129 */
/* 5390 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5392 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5394 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5396 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 5398 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 5400 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5402 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5404 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter timeStep */

/* 5406 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5408 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5410 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5412 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5414 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5416 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetNavigaterVisibility */

/* 5418 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5420 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5424 */	NdrFcShort( 0x82 ),	/* 130 */
/* 5426 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5428 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5430 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5432 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 5434 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5438 */	NdrFcShort( 0x6 ),	/* 6 */
/* 5440 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter navName */

/* 5442 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5444 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5446 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter visible */

/* 5448 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5450 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5452 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5454 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5456 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5458 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RestoreModelColor */

/* 5460 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5462 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5466 */	NdrFcShort( 0x83 ),	/* 131 */
/* 5468 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5472 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5474 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 5476 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5480 */	NdrFcShort( 0x6 ),	/* 6 */
/* 5482 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 5484 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5486 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5488 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 5490 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5492 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5494 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RestoreAllModelsColor */

/* 5496 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5498 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5502 */	NdrFcShort( 0x84 ),	/* 132 */
/* 5504 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5506 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5508 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5510 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 5512 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 5514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5516 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5518 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 5520 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5522 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5524 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAlphaTestStatus */

/* 5526 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5528 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5532 */	NdrFcShort( 0x85 ),	/* 133 */
/* 5534 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5536 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5538 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5540 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 5542 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5544 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5546 */	NdrFcShort( 0x6 ),	/* 6 */
/* 5548 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 5550 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5552 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5554 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 5556 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5558 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5560 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5562 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5564 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5566 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetZOneModel */

/* 5568 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5570 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5574 */	NdrFcShort( 0x86 ),	/* 134 */
/* 5576 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5578 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5580 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5582 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 5584 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5588 */	NdrFcShort( 0x6 ),	/* 6 */
/* 5590 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 5592 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5594 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5596 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 5598 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5600 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5602 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5604 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5606 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5608 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetLensFlareVisibility */

/* 5610 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5612 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5616 */	NdrFcShort( 0x87 ),	/* 135 */
/* 5618 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5620 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5622 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5624 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 5626 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 5628 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5632 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 5634 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5636 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5638 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5640 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5642 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5644 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCollisionDistance */

/* 5646 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5648 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5652 */	NdrFcShort( 0x88 ),	/* 136 */
/* 5654 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5656 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5658 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5660 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 5662 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 5664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5668 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter distance */

/* 5670 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5672 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5674 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 5676 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5678 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5680 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateStaticCubeMap */

/* 5682 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5684 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5688 */	NdrFcShort( 0x89 ),	/* 137 */
/* 5690 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 5692 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5694 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5696 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x9,		/* 9 */
/* 5698 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5702 */	NdrFcShort( 0x2a ),	/* 42 */
/* 5704 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 5706 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5708 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5710 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter frontImage */

/* 5712 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5714 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5716 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter backImage */

/* 5718 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5720 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5722 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter leftImage */

/* 5724 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5726 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5728 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter rightImage */

/* 5730 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5732 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 5734 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter topImage */

/* 5736 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5738 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 5740 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter bottomImge */

/* 5742 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5744 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 5746 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter size */

/* 5748 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5750 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 5752 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5754 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5756 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 5758 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMtrlCubeMap */

/* 5760 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5762 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5766 */	NdrFcShort( 0x8a ),	/* 138 */
/* 5768 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 5770 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5772 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5774 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 5776 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5780 */	NdrFcShort( 0xc ),	/* 12 */
/* 5782 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 5784 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5786 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5788 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter cubeMapName */

/* 5790 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5792 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5794 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter power */

/* 5796 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5798 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5800 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 5802 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5804 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5806 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetLightColor */

/* 5808 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5810 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5814 */	NdrFcShort( 0x8b ),	/* 139 */
/* 5816 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 5818 */	NdrFcShort( 0x18 ),	/* 24 */
/* 5820 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5822 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 5824 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5826 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5828 */	NdrFcShort( 0x6 ),	/* 6 */
/* 5830 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lightName */

/* 5832 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5834 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5836 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter red */

/* 5838 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5840 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5842 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 5844 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5846 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5848 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 5850 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5852 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5854 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5856 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5858 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 5860 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetLightPower */

/* 5862 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5864 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5868 */	NdrFcShort( 0x8c ),	/* 140 */
/* 5870 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5872 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5874 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5876 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 5878 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5880 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5882 */	NdrFcShort( 0x6 ),	/* 6 */
/* 5884 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lightName */

/* 5886 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5888 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5890 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter power */

/* 5892 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5894 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5896 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 5898 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5900 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5902 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetLightAmbient */

/* 5904 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5906 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5910 */	NdrFcShort( 0x8d ),	/* 141 */
/* 5912 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 5914 */	NdrFcShort( 0x18 ),	/* 24 */
/* 5916 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5918 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 5920 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5922 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5924 */	NdrFcShort( 0x6 ),	/* 6 */
/* 5926 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lightName */

/* 5928 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5930 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5932 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter red */

/* 5934 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5936 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5938 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 5940 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5942 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5944 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 5946 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5948 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 5950 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5952 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 5954 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 5956 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetLightPosition */

/* 5958 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5960 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5964 */	NdrFcShort( 0x8e ),	/* 142 */
/* 5966 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 5968 */	NdrFcShort( 0x18 ),	/* 24 */
/* 5970 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5972 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 5974 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 5976 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5978 */	NdrFcShort( 0x6 ),	/* 6 */
/* 5980 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lightName */

/* 5982 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 5984 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 5986 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 5988 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5990 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 5992 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 5994 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 5996 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 5998 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 6000 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6002 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6004 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 6006 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6008 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 6010 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetLightRange */

/* 6012 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6014 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6018 */	NdrFcShort( 0x8f ),	/* 143 */
/* 6020 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6022 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6024 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6026 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 6028 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6030 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6032 */	NdrFcShort( 0x6 ),	/* 6 */
/* 6034 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lightName */

/* 6036 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6038 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6040 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter range */

/* 6042 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6044 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6046 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 6048 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6050 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6052 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetLightAttenuation */

/* 6054 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6056 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6060 */	NdrFcShort( 0x90 ),	/* 144 */
/* 6062 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 6064 */	NdrFcShort( 0x18 ),	/* 24 */
/* 6066 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6068 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 6070 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6072 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6074 */	NdrFcShort( 0x6 ),	/* 6 */
/* 6076 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lightName */

/* 6078 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6080 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6082 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter attenuation0 */

/* 6084 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6086 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6088 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter attenuation1 */

/* 6090 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6092 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6094 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter attenuation2 */

/* 6096 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6098 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6100 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 6102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6104 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 6106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetLightStatus */

/* 6108 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6114 */	NdrFcShort( 0x91 ),	/* 145 */
/* 6116 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6118 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6122 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 6124 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6128 */	NdrFcShort( 0x6 ),	/* 6 */
/* 6130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lightName */

/* 6132 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6134 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6136 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 6138 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6140 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6144 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6146 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6148 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateMovieTexture */

/* 6150 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6152 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6156 */	NdrFcShort( 0x92 ),	/* 146 */
/* 6158 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 6160 */	NdrFcShort( 0x18 ),	/* 24 */
/* 6162 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6164 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 6166 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6168 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6170 */	NdrFcShort( 0x12 ),	/* 18 */
/* 6172 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter texName */

/* 6174 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6176 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6178 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter moviePath */

/* 6180 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6182 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6184 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter movieType */

/* 6186 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6188 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6190 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter withAudio */

/* 6192 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6194 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6196 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter repeatCnt */

/* 6198 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6200 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 6202 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter mtrlName */

/* 6204 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6206 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 6208 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 6210 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6212 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 6214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMovieMtrl */

/* 6216 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6222 */	NdrFcShort( 0x93 ),	/* 147 */
/* 6224 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6228 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6230 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 6232 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6236 */	NdrFcShort( 0xc ),	/* 12 */
/* 6238 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 6240 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6242 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6244 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter movieTexName */

/* 6246 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6248 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6250 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 6252 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6254 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6256 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelKfrRepeatCnt */

/* 6258 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6260 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6264 */	NdrFcShort( 0x94 ),	/* 148 */
/* 6266 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6268 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6270 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6272 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 6274 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6278 */	NdrFcShort( 0x6 ),	/* 6 */
/* 6280 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 6282 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6284 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6286 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter kfrRepeatCnt */

/* 6288 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6290 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6292 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6294 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6296 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6298 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAllModelKfrRepeatCnt */

/* 6300 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6302 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6306 */	NdrFcShort( 0x95 ),	/* 149 */
/* 6308 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6310 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6312 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6314 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 6316 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 6318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6322 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter kfrRepeatCnt */

/* 6324 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6326 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6328 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6330 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6332 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateBackdropFromImage */

/* 6336 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6338 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6342 */	NdrFcShort( 0x96 ),	/* 150 */
/* 6344 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 6346 */	NdrFcShort( 0x28 ),	/* 40 */
/* 6348 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6350 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x9,		/* 9 */
/* 6352 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6354 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6356 */	NdrFcShort( 0x12 ),	/* 18 */
/* 6358 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter backdropName */

/* 6360 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6362 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6364 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter oriImageName */

/* 6366 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6368 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6370 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter alphaImageName */

/* 6372 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6374 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6376 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter alignMode */

/* 6378 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6380 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6382 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter left */

/* 6384 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6386 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 6388 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter top */

/* 6390 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6392 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 6394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter width */

/* 6396 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6398 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 6400 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 6402 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6404 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 6406 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6408 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6410 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 6412 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateSpecularMap */

/* 6414 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6416 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6420 */	NdrFcShort( 0x97 ),	/* 151 */
/* 6422 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 6424 */	NdrFcShort( 0x18 ),	/* 24 */
/* 6426 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6428 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 6430 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6432 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6434 */	NdrFcShort( 0xc ),	/* 12 */
/* 6436 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 6438 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6440 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6442 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter imageName */

/* 6444 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6446 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6448 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter width */

/* 6450 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6452 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6454 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 6456 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6458 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6460 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter mipLevel */

/* 6462 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6464 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 6466 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6468 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6470 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 6472 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetSpecularMapMtrl */

/* 6474 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6476 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6480 */	NdrFcShort( 0x98 ),	/* 152 */
/* 6482 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6484 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6486 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6488 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 6490 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6492 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6494 */	NdrFcShort( 0xc ),	/* 12 */
/* 6496 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 6498 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6500 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6502 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter specularMapName */

/* 6504 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6506 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6508 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 6510 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6512 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6514 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateCamera */

/* 6516 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6518 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6522 */	NdrFcShort( 0x99 ),	/* 153 */
/* 6524 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 6526 */	NdrFcShort( 0x48 ),	/* 72 */
/* 6528 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6530 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0xb,		/* 11 */
/* 6532 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6534 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6536 */	NdrFcShort( 0x6 ),	/* 6 */
/* 6538 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 6540 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6542 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6544 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter fromX */

/* 6546 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6548 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6550 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter fromY */

/* 6552 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6554 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6556 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter fromZ */

/* 6558 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6560 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6562 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter atX */

/* 6564 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6566 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 6568 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter atY */

/* 6570 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6572 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 6574 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter atZ */

/* 6576 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6578 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 6580 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter fov */

/* 6582 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6584 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 6586 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter nearClip */

/* 6588 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6590 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 6592 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter farClip */

/* 6594 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6596 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 6598 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 6600 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6602 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 6604 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteModel */

/* 6606 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6608 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6612 */	NdrFcShort( 0x9a ),	/* 154 */
/* 6614 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6616 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6618 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6620 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 6622 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6624 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6626 */	NdrFcShort( 0x6 ),	/* 6 */
/* 6628 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 6630 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6632 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6634 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 6636 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6638 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6640 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCanNavigateByPointer */

/* 6642 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6644 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6648 */	NdrFcShort( 0x9b ),	/* 155 */
/* 6650 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6652 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6654 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6656 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 6658 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6660 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6662 */	NdrFcShort( 0x6 ),	/* 6 */
/* 6664 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter navName */

/* 6666 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6668 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6670 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 6672 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6674 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6676 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6678 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6680 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6682 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPlayerCanMove */

/* 6684 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6686 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6690 */	NdrFcShort( 0x9c ),	/* 156 */
/* 6692 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6694 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6696 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6698 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 6700 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 6702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6706 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 6708 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6710 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6712 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6714 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6716 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6718 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPlayerCanRos */

/* 6720 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6722 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6726 */	NdrFcShort( 0x9d ),	/* 157 */
/* 6728 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6730 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6732 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6734 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 6736 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 6738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6740 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6742 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 6744 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6746 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6748 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6750 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6752 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6754 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPlayerCanZoom */

/* 6756 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6758 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6762 */	NdrFcShort( 0x9e ),	/* 158 */
/* 6764 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6766 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6768 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6770 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 6772 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 6774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6778 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 6780 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6782 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6784 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6786 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6788 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6790 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPlayerCanLift */

/* 6792 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6794 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6798 */	NdrFcShort( 0x9f ),	/* 159 */
/* 6800 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6802 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6804 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6806 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 6808 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 6810 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6814 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 6816 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6818 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6820 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6822 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6824 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6826 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteMovieTexture */

/* 6828 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6830 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6834 */	NdrFcShort( 0xa0 ),	/* 160 */
/* 6836 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6840 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6842 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 6844 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6846 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6848 */	NdrFcShort( 0xc ),	/* 12 */
/* 6850 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 6852 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6854 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6856 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter movieTexName */

/* 6858 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6860 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6862 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 6864 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6866 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6868 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelKfrPlayStatus */

/* 6870 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6872 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6876 */	NdrFcShort( 0xa1 ),	/* 161 */
/* 6878 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6880 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6882 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6884 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 6886 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6888 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6890 */	NdrFcShort( 0x6 ),	/* 6 */
/* 6892 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 6894 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6896 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6898 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter kfrPlay */

/* 6900 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6902 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6904 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6906 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6908 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6910 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayModelKfr */

/* 6912 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6914 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6918 */	NdrFcShort( 0xa2 ),	/* 162 */
/* 6920 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 6922 */	NdrFcShort( 0x18 ),	/* 24 */
/* 6924 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6926 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 6928 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 6930 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6932 */	NdrFcShort( 0x6 ),	/* 6 */
/* 6934 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 6936 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 6938 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6940 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter startFrame */

/* 6942 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6944 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 6946 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter endFrame */

/* 6948 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6950 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6952 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter repeatCnt */

/* 6954 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6956 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 6958 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6960 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6962 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 6964 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAllModelsKfrPlayStatus */

/* 6966 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6968 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6972 */	NdrFcShort( 0xa3 ),	/* 163 */
/* 6974 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 6976 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6978 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6980 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 6982 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 6984 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6986 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6988 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bKfrPlay */

/* 6990 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 6992 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 6994 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6996 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 6998 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7000 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelEventOnMouseMove */

/* 7002 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7004 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7008 */	NdrFcShort( 0xa4 ),	/* 164 */
/* 7010 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7012 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7014 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7016 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 7018 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 7020 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7022 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7024 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 7026 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7028 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7030 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 7032 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7034 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7036 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetButtonVisibility */

/* 7038 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7040 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7044 */	NdrFcShort( 0xa5 ),	/* 165 */
/* 7046 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 7048 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7050 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7052 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 7054 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7056 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7058 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7060 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter btnName */

/* 7062 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7064 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7066 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter visible */

/* 7068 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7070 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7072 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 7074 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7076 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7078 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAntiAliasLevel */

/* 7080 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7082 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7086 */	NdrFcShort( 0xa6 ),	/* 166 */
/* 7088 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7090 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7092 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7094 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 7096 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 7098 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7102 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter level */

/* 7104 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7106 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7108 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 7110 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7112 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7114 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMtrlSpecularCubeMap */

/* 7116 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7118 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7122 */	NdrFcShort( 0xa7 ),	/* 167 */
/* 7124 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 7126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7128 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7130 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 7132 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7136 */	NdrFcShort( 0xc ),	/* 12 */
/* 7138 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 7140 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7142 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7144 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter specularCubeMapName */

/* 7146 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7148 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7150 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7152 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7154 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7156 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetButtonActive */

/* 7158 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7160 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7164 */	NdrFcShort( 0xa8 ),	/* 168 */
/* 7166 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 7168 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7170 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7172 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 7174 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7176 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7178 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7180 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 7182 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7184 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7186 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter bActive */

/* 7188 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7190 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7192 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 7194 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7196 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7198 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateSectionMap */

/* 7200 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7202 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7206 */	NdrFcShort( 0xa9 ),	/* 169 */
/* 7208 */	NdrFcShort( 0x4c ),	/* x86 Stack size/offset = 76 */
/* 7210 */	NdrFcShort( 0x70 ),	/* 112 */
/* 7212 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7214 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x12,		/* 18 */
/* 7216 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7218 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7220 */	NdrFcShort( 0x12 ),	/* 18 */
/* 7222 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sectioMapName */

/* 7224 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7226 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7228 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter width */

/* 7230 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7232 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7234 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 7236 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7238 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7240 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter sliceModelName */

/* 7242 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7244 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 7246 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter leftTopX */

/* 7248 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7250 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 7252 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter leftTopY */

/* 7254 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7256 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 7258 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter leftTopZ */

/* 7260 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7262 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 7264 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter rightTopX */

/* 7266 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7268 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 7270 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter rightTopY */

/* 7272 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7274 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 7276 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter rightTopZ */

/* 7278 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7280 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 7282 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter leftBottomX */

/* 7284 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7286 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 7288 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter leftBottomY */

/* 7290 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7292 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 7294 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter leftBottomZ */

/* 7296 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7298 */	NdrFcShort( 0x34 ),	/* x86 Stack size/offset = 52 */
/* 7300 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter rightBottomX */

/* 7302 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7304 */	NdrFcShort( 0x38 ),	/* x86 Stack size/offset = 56 */
/* 7306 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter rightBottomY */

/* 7308 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7310 */	NdrFcShort( 0x3c ),	/* x86 Stack size/offset = 60 */
/* 7312 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter rightBottomZ */

/* 7314 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7316 */	NdrFcShort( 0x40 ),	/* x86 Stack size/offset = 64 */
/* 7318 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter imageName */

/* 7320 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7322 */	NdrFcShort( 0x44 ),	/* x86 Stack size/offset = 68 */
/* 7324 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7326 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7328 */	NdrFcShort( 0x48 ),	/* x86 Stack size/offset = 72 */
/* 7330 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ExecuteSlice */

/* 7332 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7334 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7338 */	NdrFcShort( 0xaa ),	/* 170 */
/* 7340 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7344 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7346 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 7348 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7352 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7354 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sectionMapName */

/* 7356 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7358 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7360 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7362 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7364 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7366 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetOverlayVisibility */

/* 7368 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7370 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7374 */	NdrFcShort( 0xab ),	/* 171 */
/* 7376 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 7378 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7380 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7382 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 7384 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7388 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7390 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 7392 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7394 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7396 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter visible */

/* 7398 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7400 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7402 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 7404 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7406 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7408 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateAudio */

/* 7410 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7412 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7416 */	NdrFcShort( 0xac ),	/* 172 */
/* 7418 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 7420 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7422 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7424 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 7426 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7430 */	NdrFcShort( 0xc ),	/* 12 */
/* 7432 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 7434 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7436 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7438 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter audioFile */

/* 7440 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7442 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7444 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter repeatCnt */

/* 7446 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7448 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7450 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 7452 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7454 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 7456 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteAudioByName */

/* 7458 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7460 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7464 */	NdrFcShort( 0xad ),	/* 173 */
/* 7466 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7470 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7472 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 7474 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7478 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7480 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 7482 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7484 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7486 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7488 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7490 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7492 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteAudioByIndex */

/* 7494 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7496 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7500 */	NdrFcShort( 0xae ),	/* 174 */
/* 7502 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7504 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7506 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7508 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 7510 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 7512 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7516 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 7518 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7520 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7522 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 7524 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7526 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7528 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteAllAudio */

/* 7530 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7532 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7536 */	NdrFcShort( 0xaf ),	/* 175 */
/* 7538 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7540 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7542 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7544 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 7546 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 7548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7550 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7552 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 7554 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7556 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7558 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAudioFile */

/* 7560 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7562 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7566 */	NdrFcShort( 0xb0 ),	/* 176 */
/* 7568 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 7570 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7572 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7574 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 7576 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7580 */	NdrFcShort( 0xc ),	/* 12 */
/* 7582 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter audioName */

/* 7584 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7586 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7588 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter audioFile */

/* 7590 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7592 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7594 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7596 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7598 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7600 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAudioRepeatCount */

/* 7602 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7604 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7608 */	NdrFcShort( 0xb1 ),	/* 177 */
/* 7610 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 7612 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7614 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7616 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 7618 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7622 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7624 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter audioName */

/* 7626 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7628 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7630 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter repeatCnt */

/* 7632 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7634 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7636 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 7638 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7640 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7642 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayAudio */

/* 7644 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7646 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7650 */	NdrFcShort( 0xb2 ),	/* 178 */
/* 7652 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7654 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7656 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7658 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 7660 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7664 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7666 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter audioName */

/* 7668 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7670 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7672 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7674 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7676 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7678 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PauseAudio */

/* 7680 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7682 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7686 */	NdrFcShort( 0xb3 ),	/* 179 */
/* 7688 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7690 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7692 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7694 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 7696 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7700 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7702 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter audioName */

/* 7704 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7706 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7708 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7710 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7712 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7714 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StopAudio */

/* 7716 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7718 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7722 */	NdrFcShort( 0xb4 ),	/* 180 */
/* 7724 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7728 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7730 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 7732 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7734 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7736 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7738 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter audioName */

/* 7740 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7742 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7744 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7746 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7748 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7750 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RewindAudio */

/* 7752 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7754 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7758 */	NdrFcShort( 0xb5 ),	/* 181 */
/* 7760 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7762 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7764 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7766 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 7768 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7770 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7772 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7774 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter audioName */

/* 7776 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7778 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7780 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7782 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7784 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7786 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteModelGroup */

/* 7788 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7790 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7794 */	NdrFcShort( 0xb6 ),	/* 182 */
/* 7796 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7798 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7800 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7802 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 7804 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7806 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7808 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7810 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 7812 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7814 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7816 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7818 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7820 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteModelGroupByIndex */

/* 7824 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7826 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7830 */	NdrFcShort( 0xb7 ),	/* 183 */
/* 7832 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7834 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7836 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7838 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 7840 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 7842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7846 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 7848 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 7850 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7852 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 7854 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7856 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7858 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteAllModelGroup */

/* 7860 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7862 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7866 */	NdrFcShort( 0xb8 ),	/* 184 */
/* 7868 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7870 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7872 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7874 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 7876 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 7878 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7880 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7882 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 7884 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7886 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7888 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddModelToModelGroup */

/* 7890 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7892 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7896 */	NdrFcShort( 0xb9 ),	/* 185 */
/* 7898 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 7900 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7902 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7904 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 7906 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7908 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7910 */	NdrFcShort( 0xc ),	/* 12 */
/* 7912 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelGroupName */

/* 7914 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7916 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7918 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter modelName */

/* 7920 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7922 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7924 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7926 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7928 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7930 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteModelFromModelGroup */

/* 7932 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7934 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7938 */	NdrFcShort( 0xba ),	/* 186 */
/* 7940 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 7942 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7944 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7946 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 7948 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7950 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7952 */	NdrFcShort( 0xc ),	/* 12 */
/* 7954 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelGroupName */

/* 7956 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7958 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 7960 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter modelName */

/* 7962 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 7964 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 7966 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 7968 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 7970 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7972 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ClearModelGroup */

/* 7974 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 7976 */	NdrFcLong( 0x0 ),	/* 0 */
/* 7980 */	NdrFcShort( 0xbb ),	/* 187 */
/* 7982 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 7984 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7986 */	NdrFcShort( 0x8 ),	/* 8 */
/* 7988 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 7990 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 7992 */	NdrFcShort( 0x0 ),	/* 0 */
/* 7994 */	NdrFcShort( 0x6 ),	/* 6 */
/* 7996 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelGroupName */

/* 7998 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8000 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8002 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 8004 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8006 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8008 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelMouseHintType */

/* 8010 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8012 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8016 */	NdrFcShort( 0xbc ),	/* 188 */
/* 8018 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 8020 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8022 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8024 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 8026 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8028 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8030 */	NdrFcShort( 0x6 ),	/* 6 */
/* 8032 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 8034 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8036 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8038 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter type */

/* 8040 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8042 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8044 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 8046 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8048 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8050 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetLightKfrPlayStatus */

/* 8052 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8054 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8058 */	NdrFcShort( 0xbd ),	/* 189 */
/* 8060 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 8062 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8064 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8066 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 8068 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8070 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8072 */	NdrFcShort( 0x6 ),	/* 6 */
/* 8074 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lightName */

/* 8076 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8078 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8080 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter bFlag */

/* 8082 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8084 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8086 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 8088 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8090 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8092 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAllLightKfrPlayStatus */

/* 8094 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8096 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8100 */	NdrFcShort( 0xbe ),	/* 190 */
/* 8102 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8104 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8106 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8108 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 8110 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 8112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8114 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8116 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bFlag */

/* 8118 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8120 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8122 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 8124 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8126 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetLightKfrRepeatCnt */

/* 8130 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8132 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8136 */	NdrFcShort( 0xbf ),	/* 191 */
/* 8138 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 8140 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8142 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8144 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 8146 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8148 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8150 */	NdrFcShort( 0x6 ),	/* 6 */
/* 8152 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lightName */

/* 8154 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8156 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8158 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter repeatCnt */

/* 8160 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8162 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8164 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 8166 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8168 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8170 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAllLightKfrRepeatCnt */

/* 8172 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8174 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8178 */	NdrFcShort( 0xc0 ),	/* 192 */
/* 8180 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8182 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8184 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8186 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 8188 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 8190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8194 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter repeatCnt */

/* 8196 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8198 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8200 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 8202 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8204 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8206 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAllModelKfrPlayStatus */

/* 8208 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8210 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8214 */	NdrFcShort( 0xc1 ),	/* 193 */
/* 8216 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8218 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8220 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8222 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 8224 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 8226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8228 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8230 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter kfrPlay */

/* 8232 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8234 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8236 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 8238 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8240 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8242 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelPosition */

/* 8244 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8246 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8250 */	NdrFcShort( 0xc2 ),	/* 194 */
/* 8252 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 8254 */	NdrFcShort( 0x18 ),	/* 24 */
/* 8256 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8258 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 8260 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8264 */	NdrFcShort( 0x6 ),	/* 6 */
/* 8266 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 8268 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8270 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8272 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 8274 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8276 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8278 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 8280 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8282 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8284 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 8286 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8288 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 8290 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 8292 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8294 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 8296 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelRotation */

/* 8298 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8300 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8304 */	NdrFcShort( 0xc3 ),	/* 195 */
/* 8306 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 8308 */	NdrFcShort( 0x18 ),	/* 24 */
/* 8310 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8312 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 8314 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8318 */	NdrFcShort( 0x6 ),	/* 6 */
/* 8320 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 8322 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8324 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8326 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 8328 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8330 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8332 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 8334 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8336 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8338 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 8340 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8342 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 8344 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 8346 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8348 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 8350 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddNavigaterPointer */

/* 8352 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8354 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8358 */	NdrFcShort( 0xc4 ),	/* 196 */
/* 8360 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 8362 */	NdrFcShort( 0x18 ),	/* 24 */
/* 8364 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8366 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x9,		/* 9 */
/* 8368 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8372 */	NdrFcShort( 0x1e ),	/* 30 */
/* 8374 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter navigaterName */

/* 8376 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8378 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8380 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter navPointerName */

/* 8382 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8384 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8386 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter navPointerType */

/* 8388 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8390 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8392 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter camOrModelName */

/* 8394 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8396 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 8398 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter navPointerOriImage */

/* 8400 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8402 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 8404 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter navPointerAlphaImage */

/* 8406 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8408 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 8410 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter width */

/* 8412 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8414 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 8416 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 8418 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8420 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 8422 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 8424 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8426 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 8428 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetNavigaterPointerVisibility */

/* 8430 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8432 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8436 */	NdrFcShort( 0xc5 ),	/* 197 */
/* 8438 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 8440 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8442 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8444 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 8446 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8448 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8450 */	NdrFcShort( 0xc ),	/* 12 */
/* 8452 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter navigaterName */

/* 8454 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8456 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8458 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter navPointerName */

/* 8460 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8462 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8464 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 8466 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8468 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8470 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 8472 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8474 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 8476 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteNavigaterPointer */

/* 8478 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8480 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8484 */	NdrFcShort( 0xc6 ),	/* 198 */
/* 8486 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 8488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8490 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8492 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 8494 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8496 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8498 */	NdrFcShort( 0xc ),	/* 12 */
/* 8500 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter navigaterName */

/* 8502 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8504 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8506 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter navPointerName */

/* 8508 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8510 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8512 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 8514 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8516 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8518 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PauseCameraKfr */

/* 8520 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8522 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8526 */	NdrFcShort( 0xc7 ),	/* 199 */
/* 8528 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8530 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8532 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8534 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 8536 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8538 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8540 */	NdrFcShort( 0x6 ),	/* 6 */
/* 8542 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter camName */

/* 8544 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8546 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8548 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 8550 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8552 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8554 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ResumeCameraKfr */

/* 8556 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8558 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8562 */	NdrFcShort( 0xc8 ),	/* 200 */
/* 8564 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8566 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8568 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8570 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 8572 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8574 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8576 */	NdrFcShort( 0x6 ),	/* 6 */
/* 8578 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cameraName */

/* 8580 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8582 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8584 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 8586 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8588 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8590 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetNavigaterPointerObject */

/* 8592 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8594 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8598 */	NdrFcShort( 0xc9 ),	/* 201 */
/* 8600 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 8602 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8604 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8606 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 8608 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8612 */	NdrFcShort( 0x12 ),	/* 18 */
/* 8614 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter navName */

/* 8616 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8618 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8620 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter navPointerName */

/* 8622 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8624 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8626 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter objType */

/* 8628 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8630 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8632 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter objName */

/* 8634 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8636 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 8638 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 8640 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8642 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 8644 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPlayerZoomSpeed */

/* 8646 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8648 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8652 */	NdrFcShort( 0xca ),	/* 202 */
/* 8654 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8656 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8658 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8660 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 8662 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 8664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8668 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter speed */

/* 8670 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8672 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8674 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 8676 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8678 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8680 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetOverlayFullScreen */

/* 8682 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8684 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8688 */	NdrFcShort( 0xcb ),	/* 203 */
/* 8690 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 8692 */	NdrFcShort( 0x10 ),	/* 16 */
/* 8694 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8696 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 8698 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8702 */	NdrFcShort( 0x6 ),	/* 6 */
/* 8704 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter OverlayName */

/* 8706 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8708 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8710 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter xFull */

/* 8712 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8714 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8716 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter yFull */

/* 8718 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8720 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8722 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 8724 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8726 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 8728 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PushOverlayFront */

/* 8730 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8732 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8736 */	NdrFcShort( 0xcc ),	/* 204 */
/* 8738 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8740 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8742 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8744 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 8746 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8750 */	NdrFcShort( 0x6 ),	/* 6 */
/* 8752 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter OverlayName */

/* 8754 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8756 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8758 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 8760 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8762 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8764 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PushOverlayBack */

/* 8766 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8768 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8772 */	NdrFcShort( 0xcd ),	/* 205 */
/* 8774 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8778 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8780 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 8782 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 8784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8786 */	NdrFcShort( 0x6 ),	/* 6 */
/* 8788 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 8790 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 8792 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8794 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 8796 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8798 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8800 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteAllModel */

/* 8802 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8804 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8808 */	NdrFcShort( 0xce ),	/* 206 */
/* 8810 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8814 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8816 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 8818 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 8820 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8824 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 8826 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8828 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8830 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayKeyFrame */

/* 8832 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8834 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8838 */	NdrFcShort( 0xcf ),	/* 207 */
/* 8840 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8844 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8846 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 8848 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 8850 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8852 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8854 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 8856 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8858 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8860 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PauseKeyFrame */

/* 8862 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8864 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8868 */	NdrFcShort( 0xd0 ),	/* 208 */
/* 8870 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8872 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8874 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8876 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 8878 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 8880 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8882 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8884 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 8886 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8888 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8890 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StopKeyFrame */

/* 8892 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8894 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8898 */	NdrFcShort( 0xd1 ),	/* 209 */
/* 8900 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8902 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8904 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8906 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 8908 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 8910 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8912 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8914 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 8916 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8918 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8920 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetKeyFrameRepeatCount */

/* 8922 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8924 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8928 */	NdrFcShort( 0xd2 ),	/* 210 */
/* 8930 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8932 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8934 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8936 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 8938 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 8940 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8942 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8944 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter repeatCnt */

/* 8946 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8948 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8950 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 8952 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8954 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8956 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GotoFrame */

/* 8958 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8960 */	NdrFcLong( 0x0 ),	/* 0 */
/* 8964 */	NdrFcShort( 0xd3 ),	/* 211 */
/* 8966 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 8968 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8970 */	NdrFcShort( 0x8 ),	/* 8 */
/* 8972 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 8974 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 8976 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8978 */	NdrFcShort( 0x0 ),	/* 0 */
/* 8980 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frame */

/* 8982 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 8984 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 8986 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 8988 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 8990 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 8992 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetKeyFramePlaySpeed */

/* 8994 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 8996 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9000 */	NdrFcShort( 0xd4 ),	/* 212 */
/* 9002 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9004 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9006 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9008 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 9010 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 9012 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9014 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9016 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter fps */

/* 9018 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9020 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9022 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9024 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9026 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9028 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateFSGlow */

/* 9030 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9032 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9036 */	NdrFcShort( 0xd5 ),	/* 213 */
/* 9038 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9040 */	NdrFcShort( 0x10 ),	/* 16 */
/* 9042 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9044 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 9046 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 9048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9050 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9052 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter BloomValue */

/* 9054 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9056 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9058 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter BlurValue */

/* 9060 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9062 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9064 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 9066 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9068 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9070 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFSGlowValue */

/* 9072 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9074 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9078 */	NdrFcShort( 0xd6 ),	/* 214 */
/* 9080 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9082 */	NdrFcShort( 0x10 ),	/* 16 */
/* 9084 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9086 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 9088 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 9090 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9092 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9094 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter BloomValue */

/* 9096 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9098 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9100 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter BlurValue */

/* 9102 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9104 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9106 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 9108 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9110 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StartFSGlow */

/* 9114 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9116 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9120 */	NdrFcShort( 0xd7 ),	/* 215 */
/* 9122 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9126 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9128 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 9130 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 9132 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9136 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 9138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9140 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EndFSGlow */

/* 9144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9150 */	NdrFcShort( 0xd8 ),	/* 216 */
/* 9152 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9156 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9158 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 9160 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 9162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 9168 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMtrlSpecular */

/* 9174 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9176 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9180 */	NdrFcShort( 0xd9 ),	/* 217 */
/* 9182 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 9184 */	NdrFcShort( 0x18 ),	/* 24 */
/* 9186 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9188 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 9190 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9194 */	NdrFcShort( 0x6 ),	/* 6 */
/* 9196 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 9198 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9200 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9202 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter red */

/* 9204 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9206 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 9210 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9212 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 9216 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9218 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9220 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9222 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9224 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 9226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMtrlAmbient */

/* 9228 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9230 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9234 */	NdrFcShort( 0xda ),	/* 218 */
/* 9236 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 9238 */	NdrFcShort( 0x18 ),	/* 24 */
/* 9240 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9242 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 9244 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9248 */	NdrFcShort( 0x6 ),	/* 6 */
/* 9250 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 9252 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9254 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9256 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter red */

/* 9258 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9260 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9262 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 9264 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9266 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9268 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 9270 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9272 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9274 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9276 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9278 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 9280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMtrlSpecularLevel */

/* 9282 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9284 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9288 */	NdrFcShort( 0xdb ),	/* 219 */
/* 9290 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9292 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9294 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9296 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 9298 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9300 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9302 */	NdrFcShort( 0x6 ),	/* 6 */
/* 9304 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 9306 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9308 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9310 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter level */

/* 9312 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9314 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9316 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9318 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9320 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMtrlSpecularPower */

/* 9324 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9326 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9330 */	NdrFcShort( 0xdc ),	/* 220 */
/* 9332 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9334 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9336 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9338 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 9340 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9344 */	NdrFcShort( 0x6 ),	/* 6 */
/* 9346 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 9348 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9350 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9352 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter power */

/* 9354 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9356 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9358 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9360 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9362 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayKeyFrame1 */

/* 9366 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9368 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9372 */	NdrFcShort( 0xdd ),	/* 221 */
/* 9374 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9376 */	NdrFcShort( 0x10 ),	/* 16 */
/* 9378 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9380 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 9382 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 9384 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9388 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter startFrame */

/* 9390 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9392 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter endFrame */

/* 9396 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9398 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9400 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9402 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9404 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9406 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCameraKfrPlayStatus */

/* 9408 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9410 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9414 */	NdrFcShort( 0xde ),	/* 222 */
/* 9416 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9418 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9420 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9422 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 9424 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9426 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9428 */	NdrFcShort( 0x6 ),	/* 6 */
/* 9430 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter camName */

/* 9432 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9434 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9436 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 9438 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9440 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9444 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9446 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9448 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAllModelVisibility */

/* 9450 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9452 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9456 */	NdrFcShort( 0xdf ),	/* 223 */
/* 9458 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9460 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9462 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9464 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 9466 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 9468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9472 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 9474 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9476 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9478 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9480 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9482 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9484 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAllCameraKfrPlayStatus */

/* 9486 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9488 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9492 */	NdrFcShort( 0xe0 ),	/* 224 */
/* 9494 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9496 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9498 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9500 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 9502 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 9504 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9506 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9508 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter status */

/* 9510 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9512 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9514 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9516 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9518 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9520 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCameraKfrRepeatCnt */

/* 9522 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9524 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9528 */	NdrFcShort( 0xe1 ),	/* 225 */
/* 9530 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9532 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9534 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9536 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 9538 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9540 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9542 */	NdrFcShort( 0x6 ),	/* 6 */
/* 9544 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter camName */

/* 9546 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9548 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9550 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter repeatCnt */

/* 9552 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9554 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9556 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9558 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9560 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9562 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAllCameraKfrRepeatCnt */

/* 9564 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9566 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9570 */	NdrFcShort( 0xe2 ),	/* 226 */
/* 9572 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9574 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9576 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9578 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 9580 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 9582 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9586 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter repeatCnt */

/* 9588 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9590 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9592 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9594 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9596 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9598 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetDynamicTexPlaySpeed */

/* 9600 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9602 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9606 */	NdrFcShort( 0xe3 ),	/* 227 */
/* 9608 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 9610 */	NdrFcShort( 0x10 ),	/* 16 */
/* 9612 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9614 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 9616 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9620 */	NdrFcShort( 0x6 ),	/* 6 */
/* 9622 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter UseDyTexMtrlName */

/* 9624 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9626 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9628 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter texLayerIdx */

/* 9630 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9632 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9634 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter fps */

/* 9636 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9638 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9640 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9642 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9644 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9646 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModelPositionByPivot */

/* 9648 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9650 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9654 */	NdrFcShort( 0xe4 ),	/* 228 */
/* 9656 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 9658 */	NdrFcShort( 0x18 ),	/* 24 */
/* 9660 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9662 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 9664 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9668 */	NdrFcShort( 0x6 ),	/* 6 */
/* 9670 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 9672 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9674 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9676 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 9678 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9680 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9682 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 9684 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9686 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9688 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 9690 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9692 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9694 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 9696 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9698 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 9700 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetDynamicUVStatus */

/* 9702 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9704 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9708 */	NdrFcShort( 0xe5 ),	/* 229 */
/* 9710 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9712 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9714 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9716 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 9718 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9720 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9722 */	NdrFcShort( 0x6 ),	/* 6 */
/* 9724 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 9726 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9728 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9730 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter status */

/* 9732 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9734 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9736 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9738 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9740 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9742 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveFlash */

/* 9744 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9746 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9750 */	NdrFcShort( 0xe6 ),	/* 230 */
/* 9752 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 9754 */	NdrFcShort( 0x10 ),	/* 16 */
/* 9756 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9758 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 9760 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9762 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9764 */	NdrFcShort( 0x6 ),	/* 6 */
/* 9766 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter flashName */

/* 9768 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9770 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9772 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 9774 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9776 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9778 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter y */

/* 9780 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9782 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9784 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9786 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9788 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9790 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetOverlayMtrl */

/* 9792 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9794 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9798 */	NdrFcShort( 0xe7 ),	/* 231 */
/* 9800 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9804 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9806 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 9808 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9810 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9812 */	NdrFcShort( 0xc ),	/* 12 */
/* 9814 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter overlayName */

/* 9816 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9818 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9820 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter mtrlName */

/* 9822 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9824 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9826 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 9828 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9830 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9832 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetZoneTriggerModelCallBackFunc */

/* 9834 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9836 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9840 */	NdrFcShort( 0xe8 ),	/* 232 */
/* 9842 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9846 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9848 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 9850 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9852 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9854 */	NdrFcShort( 0xc ),	/* 12 */
/* 9856 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 9858 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9860 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9862 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter funcName */

/* 9864 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9866 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9868 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 9870 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9872 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9874 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAlphaTestLevel */

/* 9876 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9878 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9882 */	NdrFcShort( 0xe9 ),	/* 233 */
/* 9884 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9886 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9888 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9890 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 9892 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 9894 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9896 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9898 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter level */

/* 9900 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9902 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9904 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9906 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9908 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9910 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetButtonOpacity */

/* 9912 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9914 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9918 */	NdrFcShort( 0xea ),	/* 234 */
/* 9920 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 9922 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9924 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9926 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 9928 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 9930 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9932 */	NdrFcShort( 0x6 ),	/* 6 */
/* 9934 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter buttonName */

/* 9936 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 9938 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9940 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter opacity */

/* 9942 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9944 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9946 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9948 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9950 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9952 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CurCamGotoFrame */

/* 9954 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9956 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9960 */	NdrFcShort( 0xeb ),	/* 235 */
/* 9962 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 9964 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9966 */	NdrFcShort( 0x8 ),	/* 8 */
/* 9968 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 9970 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 9972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9974 */	NdrFcShort( 0x0 ),	/* 0 */
/* 9976 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter frame */

/* 9978 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 9980 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 9982 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 9984 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 9986 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 9988 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetSceneEditMode */

/* 9990 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 9992 */	NdrFcLong( 0x0 ),	/* 0 */
/* 9996 */	NdrFcShort( 0xec ),	/* 236 */
/* 9998 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10000 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10002 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10004 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10006 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10008 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10010 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10012 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter EditMode */

/* 10014 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10016 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10018 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 10020 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10022 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10024 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CloneModel */

/* 10026 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10028 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10032 */	NdrFcShort( 0xed ),	/* 237 */
/* 10034 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 10036 */	NdrFcShort( 0x28 ),	/* 40 */
/* 10038 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10040 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 10042 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 10044 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10046 */	NdrFcShort( 0xc ),	/* 12 */
/* 10048 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter oriModelName */

/* 10050 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 10052 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10054 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter desModelName */

/* 10056 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 10058 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10060 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ID1 */

/* 10062 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10064 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10066 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ID2 */

/* 10068 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10070 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10072 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter posX */

/* 10074 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10076 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 10078 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter posY */

/* 10080 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10082 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 10084 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter posZ */

/* 10086 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10088 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 10090 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 10092 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10094 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 10096 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theModelPosX */

/* 10098 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10100 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10104 */	NdrFcShort( 0xee ),	/* 238 */
/* 10106 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10108 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10110 */	NdrFcShort( 0x24 ),	/* 36 */
/* 10112 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10114 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10120 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10122 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10124 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10126 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 10128 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10130 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10132 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theModelPosY */

/* 10134 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10136 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10140 */	NdrFcShort( 0xef ),	/* 239 */
/* 10142 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10144 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10146 */	NdrFcShort( 0x24 ),	/* 36 */
/* 10148 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10150 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10156 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10158 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10160 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10162 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 10164 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10166 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10168 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theModelPosZ */

/* 10170 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10172 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10176 */	NdrFcShort( 0xf0 ),	/* 240 */
/* 10178 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10180 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10182 */	NdrFcShort( 0x24 ),	/* 36 */
/* 10184 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10186 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10188 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10192 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10194 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10196 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10198 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 10200 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10202 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10204 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theModelScreenPosX */

/* 10206 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10208 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10212 */	NdrFcShort( 0xf1 ),	/* 241 */
/* 10214 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10216 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10218 */	NdrFcShort( 0x22 ),	/* 34 */
/* 10220 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10222 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10224 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10228 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10230 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10232 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10234 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 10236 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10238 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10240 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theModelScreenPosY */

/* 10242 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10244 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10248 */	NdrFcShort( 0xf2 ),	/* 242 */
/* 10250 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10252 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10254 */	NdrFcShort( 0x22 ),	/* 34 */
/* 10256 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10258 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10264 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10266 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10268 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10270 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 10272 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10274 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10276 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetInfoFromModel */

/* 10278 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10280 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10284 */	NdrFcShort( 0xf3 ),	/* 243 */
/* 10286 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10290 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10292 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 10294 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 10296 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10298 */	NdrFcShort( 0x6 ),	/* 6 */
/* 10300 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter modelName */

/* 10302 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 10304 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10306 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 10308 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10310 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateNewText */

/* 10314 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10316 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10320 */	NdrFcShort( 0xf4 ),	/* 244 */
/* 10322 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10326 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10328 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 10330 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 10332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10334 */	NdrFcShort( 0x6 ),	/* 6 */
/* 10336 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter textString */

/* 10338 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 10340 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10342 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 10344 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10346 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10348 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theTextHandle */

/* 10350 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10352 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10356 */	NdrFcShort( 0xf5 ),	/* 245 */
/* 10358 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10362 */	NdrFcShort( 0x24 ),	/* 36 */
/* 10364 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10366 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10372 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10374 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10376 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10378 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 10380 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10382 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10384 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetTextPosition */

/* 10386 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10388 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10392 */	NdrFcShort( 0xf6 ),	/* 246 */
/* 10394 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 10396 */	NdrFcShort( 0x18 ),	/* 24 */
/* 10398 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10400 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 10402 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10404 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10408 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter textHandle */

/* 10410 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10412 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10414 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter x */

/* 10416 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10418 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10420 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter y */

/* 10422 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10424 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10426 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 10428 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10430 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10432 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetTextColor */

/* 10434 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10436 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10440 */	NdrFcShort( 0xf7 ),	/* 247 */
/* 10442 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 10444 */	NdrFcShort( 0x28 ),	/* 40 */
/* 10446 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10448 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x6,		/* 6 */
/* 10450 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10452 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10454 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10456 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter textHandle */

/* 10458 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10460 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10462 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter red */

/* 10464 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10466 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10468 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 10470 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10472 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10474 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 10476 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10478 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10480 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter alpha */

/* 10482 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10484 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 10486 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 10488 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10490 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 10492 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteText */

/* 10494 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10496 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10500 */	NdrFcShort( 0xf8 ),	/* 248 */
/* 10502 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10504 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10506 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10508 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10510 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10512 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10516 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter textHandle */

/* 10518 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10520 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10522 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 10524 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10526 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10528 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetInfoFromCamera */

/* 10530 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10532 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10536 */	NdrFcShort( 0xf9 ),	/* 249 */
/* 10538 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10540 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10542 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10544 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 10546 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 10548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10550 */	NdrFcShort( 0x6 ),	/* 6 */
/* 10552 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cameraName */

/* 10554 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 10556 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10558 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 10560 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10562 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10564 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theCamFromPosX */

/* 10566 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10568 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10572 */	NdrFcShort( 0xfa ),	/* 250 */
/* 10574 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10578 */	NdrFcShort( 0x24 ),	/* 36 */
/* 10580 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10582 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10588 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10590 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10592 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10594 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 10596 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10598 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10600 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theCamFromPosY */

/* 10602 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10604 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10608 */	NdrFcShort( 0xfb ),	/* 251 */
/* 10610 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10614 */	NdrFcShort( 0x24 ),	/* 36 */
/* 10616 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10618 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10622 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10624 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10626 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10628 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10630 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 10632 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10634 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10636 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theCamFromPosZ */

/* 10638 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10640 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10644 */	NdrFcShort( 0xfc ),	/* 252 */
/* 10646 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10650 */	NdrFcShort( 0x24 ),	/* 36 */
/* 10652 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10654 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10656 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10658 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10660 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10662 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10664 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10666 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 10668 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10670 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10672 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theCamAtPosX */

/* 10674 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10676 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10680 */	NdrFcShort( 0xfd ),	/* 253 */
/* 10682 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10684 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10686 */	NdrFcShort( 0x24 ),	/* 36 */
/* 10688 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10690 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10692 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10694 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10696 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10698 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10700 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10702 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 10704 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10706 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10708 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theCamAtPosY */

/* 10710 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10712 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10716 */	NdrFcShort( 0xfe ),	/* 254 */
/* 10718 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10720 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10722 */	NdrFcShort( 0x24 ),	/* 36 */
/* 10724 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10726 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10730 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10732 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10734 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10736 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10738 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 10740 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10742 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10744 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theCamAtPosZ */

/* 10746 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10748 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10752 */	NdrFcShort( 0xff ),	/* 255 */
/* 10754 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10758 */	NdrFcShort( 0x24 ),	/* 36 */
/* 10760 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 10762 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 10764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10766 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10768 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 10770 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 10772 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10774 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 10776 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10778 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10780 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetTextString */

/* 10782 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10784 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10788 */	NdrFcShort( 0x100 ),	/* 256 */
/* 10790 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10792 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10794 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10796 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 10798 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 10800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10802 */	NdrFcShort( 0x6 ),	/* 6 */
/* 10804 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter textHandle */

/* 10806 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10808 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10810 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter textString */

/* 10812 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 10814 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10816 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 10818 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10820 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure LoadScene */

/* 10824 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10826 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10830 */	NdrFcShort( 0x101 ),	/* 257 */
/* 10832 */	NdrFcShort( 0x54 ),	/* x86 Stack size/offset = 84 */
/* 10834 */	NdrFcShort( 0x80 ),	/* 128 */
/* 10836 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10838 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x14,		/* 20 */
/* 10840 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 10842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10844 */	NdrFcShort( 0x12 ),	/* 18 */
/* 10846 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter SceneName */

/* 10848 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 10850 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10852 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ParentSceneName */

/* 10854 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 10856 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10858 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter N3DFileURL */

/* 10860 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 10862 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10864 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter m11 */

/* 10866 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10868 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10870 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m12 */

/* 10872 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10874 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 10876 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m13 */

/* 10878 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10880 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 10882 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m14 */

/* 10884 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10886 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 10888 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m21 */

/* 10890 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10892 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 10894 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m22 */

/* 10896 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10898 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 10900 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m23 */

/* 10902 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10904 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 10906 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m24 */

/* 10908 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10910 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 10912 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m31 */

/* 10914 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10916 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 10918 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m32 */

/* 10920 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10922 */	NdrFcShort( 0x34 ),	/* x86 Stack size/offset = 52 */
/* 10924 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m33 */

/* 10926 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10928 */	NdrFcShort( 0x38 ),	/* x86 Stack size/offset = 56 */
/* 10930 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m34 */

/* 10932 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10934 */	NdrFcShort( 0x3c ),	/* x86 Stack size/offset = 60 */
/* 10936 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m41 */

/* 10938 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10940 */	NdrFcShort( 0x40 ),	/* x86 Stack size/offset = 64 */
/* 10942 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m42 */

/* 10944 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10946 */	NdrFcShort( 0x44 ),	/* x86 Stack size/offset = 68 */
/* 10948 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m43 */

/* 10950 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10952 */	NdrFcShort( 0x48 ),	/* x86 Stack size/offset = 72 */
/* 10954 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m44 */

/* 10956 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 10958 */	NdrFcShort( 0x4c ),	/* x86 Stack size/offset = 76 */
/* 10960 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 10962 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 10964 */	NdrFcShort( 0x50 ),	/* x86 Stack size/offset = 80 */
/* 10966 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteScene */

/* 10968 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 10970 */	NdrFcLong( 0x0 ),	/* 0 */
/* 10974 */	NdrFcShort( 0x102 ),	/* 258 */
/* 10976 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10978 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10980 */	NdrFcShort( 0x8 ),	/* 8 */
/* 10982 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 10984 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 10986 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10988 */	NdrFcShort( 0x6 ),	/* 6 */
/* 10990 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter SceneName */

/* 10992 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 10994 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10996 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 10998 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11000 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11002 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CloneScene */

/* 11004 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11006 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11010 */	NdrFcShort( 0x103 ),	/* 259 */
/* 11012 */	NdrFcShort( 0x54 ),	/* x86 Stack size/offset = 84 */
/* 11014 */	NdrFcShort( 0x80 ),	/* 128 */
/* 11016 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11018 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x14,		/* 20 */
/* 11020 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11022 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11024 */	NdrFcShort( 0x12 ),	/* 18 */
/* 11026 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter destSceneName */

/* 11028 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11030 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11032 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter srcSceneName */

/* 11034 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11036 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11038 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ParentSceneName */

/* 11040 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11042 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11044 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter m11 */

/* 11046 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11048 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11050 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m12 */

/* 11052 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11054 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 11056 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m13 */

/* 11058 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11060 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 11062 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m14 */

/* 11064 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11066 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 11068 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m21 */

/* 11070 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11072 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 11074 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m22 */

/* 11076 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11078 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 11080 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m23 */

/* 11082 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11084 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 11086 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m24 */

/* 11088 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11090 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 11092 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m31 */

/* 11094 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11096 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 11098 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m32 */

/* 11100 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11102 */	NdrFcShort( 0x34 ),	/* x86 Stack size/offset = 52 */
/* 11104 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m33 */

/* 11106 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11108 */	NdrFcShort( 0x38 ),	/* x86 Stack size/offset = 56 */
/* 11110 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m34 */

/* 11112 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11114 */	NdrFcShort( 0x3c ),	/* x86 Stack size/offset = 60 */
/* 11116 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m41 */

/* 11118 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11120 */	NdrFcShort( 0x40 ),	/* x86 Stack size/offset = 64 */
/* 11122 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m42 */

/* 11124 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11126 */	NdrFcShort( 0x44 ),	/* x86 Stack size/offset = 68 */
/* 11128 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m43 */

/* 11130 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11132 */	NdrFcShort( 0x48 ),	/* x86 Stack size/offset = 72 */
/* 11134 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter m44 */

/* 11136 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11138 */	NdrFcShort( 0x4c ),	/* x86 Stack size/offset = 76 */
/* 11140 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 11142 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11144 */	NdrFcShort( 0x50 ),	/* x86 Stack size/offset = 80 */
/* 11146 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateRTFTexture */

/* 11148 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11150 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11154 */	NdrFcShort( 0x104 ),	/* 260 */
/* 11156 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 11158 */	NdrFcShort( 0x18 ),	/* 24 */
/* 11160 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11162 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 11164 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11168 */	NdrFcShort( 0xc ),	/* 12 */
/* 11170 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter TextureName */

/* 11172 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11174 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11176 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter refPlaneName */

/* 11178 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11180 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11182 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter height */

/* 11184 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11186 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11188 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter width */

/* 11190 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11192 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11194 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter power */

/* 11196 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11198 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 11200 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 11202 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11204 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 11206 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddModelToRTFTexture */

/* 11208 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11210 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11214 */	NdrFcShort( 0x105 ),	/* 261 */
/* 11216 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11218 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11220 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11222 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 11224 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11228 */	NdrFcShort( 0xc ),	/* 12 */
/* 11230 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter RTFTextureName */

/* 11232 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11234 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11236 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ModelName */

/* 11238 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11240 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11242 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 11244 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11246 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11248 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateDynamicCubeMap */

/* 11250 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11252 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11256 */	NdrFcShort( 0x106 ),	/* 262 */
/* 11258 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 11260 */	NdrFcShort( 0x10 ),	/* 16 */
/* 11262 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11264 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 11266 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11268 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11270 */	NdrFcShort( 0xc ),	/* 12 */
/* 11272 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cubeMapame */

/* 11274 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11276 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11278 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter centerModelName */

/* 11280 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11282 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11284 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter size */

/* 11286 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11288 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11290 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter power */

/* 11292 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11294 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11296 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 11298 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11300 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 11302 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddDynamicCubeMapReflectionModel */

/* 11304 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11306 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11310 */	NdrFcShort( 0x107 ),	/* 263 */
/* 11312 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11316 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11318 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 11320 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11322 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11324 */	NdrFcShort( 0xc ),	/* 12 */
/* 11326 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cubeMapName */

/* 11328 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11330 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11332 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ModelName */

/* 11334 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11336 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11338 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 11340 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11342 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11344 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteDynamicCubeMapReflectionModel */

/* 11346 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11348 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11352 */	NdrFcShort( 0x108 ),	/* 264 */
/* 11354 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11356 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11358 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11360 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 11362 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11364 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11366 */	NdrFcShort( 0xc ),	/* 12 */
/* 11368 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cubeMapName */

/* 11370 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11372 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11374 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter modelName */

/* 11376 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11378 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11380 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 11382 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11384 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11386 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateRealWater */

/* 11388 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11390 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11394 */	NdrFcShort( 0x109 ),	/* 265 */
/* 11396 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 11398 */	NdrFcShort( 0x28 ),	/* 40 */
/* 11400 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11402 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 11404 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11408 */	NdrFcShort( 0x6 ),	/* 6 */
/* 11410 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 11412 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11414 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11416 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 11418 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11420 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11422 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter y */

/* 11424 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11426 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11428 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter z */

/* 11430 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11432 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11434 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter gridX */

/* 11436 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11438 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 11440 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter gridY */

/* 11442 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11444 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 11446 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 11448 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11450 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 11452 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddRealWaterRTTModel */

/* 11454 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11456 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11460 */	NdrFcShort( 0x10a ),	/* 266 */
/* 11462 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11466 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11468 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 11470 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11472 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11474 */	NdrFcShort( 0xc ),	/* 12 */
/* 11476 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter RealWaterName */

/* 11478 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11480 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11482 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ModelName */

/* 11484 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11486 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11488 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 11490 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11492 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11494 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddRealWaterRTFModel */

/* 11496 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11498 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11502 */	NdrFcShort( 0x10b ),	/* 267 */
/* 11504 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11506 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11508 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11510 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 11512 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11516 */	NdrFcShort( 0xc ),	/* 12 */
/* 11518 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter RealWaterName */

/* 11520 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11522 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11524 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ModelName */

/* 11526 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11528 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11530 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 11532 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11534 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11536 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetRealWaterCubeMap */

/* 11538 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11540 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11544 */	NdrFcShort( 0x10c ),	/* 268 */
/* 11546 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11550 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11552 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 11554 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11556 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11558 */	NdrFcShort( 0xc ),	/* 12 */
/* 11560 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter RealWaterName */

/* 11562 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11564 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11566 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter CubeMapName */

/* 11568 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11570 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11572 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 11574 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11576 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11578 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateColorEffect */

/* 11580 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11582 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11586 */	NdrFcShort( 0x10d ),	/* 269 */
/* 11588 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11590 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11592 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11594 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 11596 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 11598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11600 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11602 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 11604 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11606 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11608 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetColorEffect */

/* 11610 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11612 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11616 */	NdrFcShort( 0x10e ),	/* 270 */
/* 11618 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 11620 */	NdrFcShort( 0x28 ),	/* 40 */
/* 11622 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11624 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x6,		/* 6 */
/* 11626 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 11628 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11632 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter brightness */

/* 11634 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11636 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11638 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter contrast */

/* 11640 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11642 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11644 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter red */

/* 11646 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11648 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11650 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter green */

/* 11652 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11654 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11656 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter blue */

/* 11658 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11660 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 11662 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 11664 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11666 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 11668 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetTextureImageFromURL */

/* 11670 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11672 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11676 */	NdrFcShort( 0x10f ),	/* 271 */
/* 11678 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 11680 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11682 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11684 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 11686 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11688 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11690 */	NdrFcShort( 0xc ),	/* 12 */
/* 11692 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 11694 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11696 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11698 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter texLayerIdx */

/* 11700 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11702 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11704 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter imageURL */

/* 11706 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11708 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11710 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 11712 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11714 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11716 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetEditCtrlMaxUpDownAngle */

/* 11718 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11720 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11724 */	NdrFcShort( 0x110 ),	/* 272 */
/* 11726 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11728 */	NdrFcShort( 0x10 ),	/* 16 */
/* 11730 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11732 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 11734 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 11736 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11740 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter upAngle */

/* 11742 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11744 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11746 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter downAngle */

/* 11748 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11750 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11752 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 11754 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11756 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11758 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMtrlTexture */

/* 11760 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11762 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11766 */	NdrFcShort( 0x111 ),	/* 273 */
/* 11768 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 11770 */	NdrFcShort( 0x10 ),	/* 16 */
/* 11772 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11774 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 11776 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11780 */	NdrFcShort( 0xc ),	/* 12 */
/* 11782 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter mtrlName */

/* 11784 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11786 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11788 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter textureLayerIndex */

/* 11790 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11792 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11794 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter textureType */

/* 11796 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 11798 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11800 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter textureName */

/* 11802 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11804 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11806 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 11808 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11810 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 11812 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddFlashCtrlSemaphore */

/* 11814 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11816 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11820 */	NdrFcShort( 0x112 ),	/* 274 */
/* 11822 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 11824 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11826 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11828 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 11830 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11832 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11834 */	NdrFcShort( 0x12 ),	/* 18 */
/* 11836 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter flashName */

/* 11838 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11840 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11842 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter SemaphoreName */

/* 11844 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11846 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11848 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter SemaphoreFunc */

/* 11850 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11852 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11854 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 11856 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11858 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11860 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteFlashCtrlSemaphore */

/* 11862 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11864 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11868 */	NdrFcShort( 0x113 ),	/* 275 */
/* 11870 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11872 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11874 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11876 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 11878 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11880 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11882 */	NdrFcShort( 0xc ),	/* 12 */
/* 11884 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter flashName */

/* 11886 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11888 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11890 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter SemaphoreName */

/* 11892 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11894 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11896 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 11898 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11900 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11902 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theFlashVar */

/* 11904 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11906 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11910 */	NdrFcShort( 0x114 ),	/* 276 */
/* 11912 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11914 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11916 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11918 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 11920 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 11922 */	NdrFcShort( 0x1 ),	/* 1 */
/* 11924 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11926 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 11928 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 11930 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11932 */	NdrFcShort( 0x42c ),	/* Type Offset=1068 */

	/* Return value */

/* 11934 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11936 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11938 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFlashVaribleInfo */

/* 11940 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11942 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11946 */	NdrFcShort( 0x115 ),	/* 277 */
/* 11948 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 11950 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11952 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11954 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 11956 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 11958 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11960 */	NdrFcShort( 0x9 ),	/* 9 */
/* 11962 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter flashName */

/* 11964 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11966 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 11968 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter varibleName */

/* 11970 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 11972 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 11974 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 11976 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 11978 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 11980 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetFlashVarible */

/* 11982 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 11984 */	NdrFcLong( 0x0 ),	/* 0 */
/* 11988 */	NdrFcShort( 0x116 ),	/* 278 */
/* 11990 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 11992 */	NdrFcShort( 0x0 ),	/* 0 */
/* 11994 */	NdrFcShort( 0x8 ),	/* 8 */
/* 11996 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 11998 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 12000 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12002 */	NdrFcShort( 0x18 ),	/* 24 */
/* 12004 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter flashName */

/* 12006 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 12008 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12010 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter varibleName */

/* 12012 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 12014 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12016 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter varibleValue */

/* 12018 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 12020 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12022 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 12024 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12026 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 12028 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure LoadN3DScene */

/* 12030 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12032 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12036 */	NdrFcShort( 0x117 ),	/* 279 */
/* 12038 */	NdrFcShort( 0x38 ),	/* x86 Stack size/offset = 56 */
/* 12040 */	NdrFcShort( 0x48 ),	/* 72 */
/* 12042 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12044 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0xd,		/* 13 */
/* 12046 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 12048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12050 */	NdrFcShort( 0x18 ),	/* 24 */
/* 12052 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter SceneName */

/* 12054 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 12056 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12058 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter ParentSceneName */

/* 12060 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 12062 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12064 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter N3DFileURL */

/* 12066 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 12068 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12070 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter posX */

/* 12072 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12074 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 12076 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter posY */

/* 12078 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12080 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 12082 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter posZ */

/* 12084 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12086 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 12088 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter rotX */

/* 12090 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12092 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 12094 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter rotY */

/* 12096 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12098 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 12100 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter rotZ */

/* 12102 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12104 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 12106 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter scaleX */

/* 12108 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12110 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 12112 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter scaleY */

/* 12114 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12116 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 12118 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Parameter scaleZ */

/* 12120 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12122 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 12124 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 12126 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12128 */	NdrFcShort( 0x34 ),	/* x86 Stack size/offset = 52 */
/* 12130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetInfoFromSceneResource */

/* 12132 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12134 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12138 */	NdrFcShort( 0x118 ),	/* 280 */
/* 12140 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12142 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12144 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12146 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 12148 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 12150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12152 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12154 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter SceneName */

/* 12156 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 12158 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12160 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 12162 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12164 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12166 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theScenePosX */

/* 12168 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12170 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12174 */	NdrFcShort( 0x119 ),	/* 281 */
/* 12176 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12178 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12180 */	NdrFcShort( 0x24 ),	/* 36 */
/* 12182 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12184 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12186 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12188 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12190 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 12192 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 12194 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12196 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 12198 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12200 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12202 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theScenePosY */

/* 12204 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12206 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12210 */	NdrFcShort( 0x11a ),	/* 282 */
/* 12212 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12214 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12216 */	NdrFcShort( 0x24 ),	/* 36 */
/* 12218 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12220 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12222 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12224 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12226 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 12228 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 12230 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12232 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 12234 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12236 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12238 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theScenePosZ */

/* 12240 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12242 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12246 */	NdrFcShort( 0x11b ),	/* 283 */
/* 12248 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12250 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12252 */	NdrFcShort( 0x24 ),	/* 36 */
/* 12254 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12256 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12258 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12262 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 12264 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 12266 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12268 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 12270 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12272 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12274 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theSceneRotX */

/* 12276 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12278 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12282 */	NdrFcShort( 0x11c ),	/* 284 */
/* 12284 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12288 */	NdrFcShort( 0x24 ),	/* 36 */
/* 12290 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12292 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12294 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12296 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12298 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 12300 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 12302 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12304 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 12306 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12308 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12310 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theSceneRotY */

/* 12312 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12314 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12318 */	NdrFcShort( 0x11d ),	/* 285 */
/* 12320 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12322 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12324 */	NdrFcShort( 0x24 ),	/* 36 */
/* 12326 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12328 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12330 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12334 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 12336 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 12338 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12340 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 12342 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12344 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12346 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theSceneRotZ */

/* 12348 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12350 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12354 */	NdrFcShort( 0x11e ),	/* 286 */
/* 12356 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12358 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12360 */	NdrFcShort( 0x24 ),	/* 36 */
/* 12362 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12364 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12366 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12370 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 12372 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 12374 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12376 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 12378 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12380 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12382 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theSceneScaleX */

/* 12384 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12386 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12390 */	NdrFcShort( 0x11f ),	/* 287 */
/* 12392 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12394 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12396 */	NdrFcShort( 0x24 ),	/* 36 */
/* 12398 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12400 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12402 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12404 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12406 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 12408 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 12410 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12412 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 12414 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12416 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12418 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theSceneScaleY */

/* 12420 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12422 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12426 */	NdrFcShort( 0x120 ),	/* 288 */
/* 12428 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12430 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12432 */	NdrFcShort( 0x24 ),	/* 36 */
/* 12434 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12436 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12438 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12440 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12442 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 12444 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 12446 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12448 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 12450 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12452 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12454 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_theSceneScaleZ */

/* 12456 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12458 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12462 */	NdrFcShort( 0x121 ),	/* 289 */
/* 12464 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12468 */	NdrFcShort( 0x24 ),	/* 36 */
/* 12470 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12472 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12478 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 12480 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 12482 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12484 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 12486 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12488 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12490 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetSceneResColor */

/* 12492 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12494 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12498 */	NdrFcShort( 0x122 ),	/* 290 */
/* 12500 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 12502 */	NdrFcShort( 0x20 ),	/* 32 */
/* 12504 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12506 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 12508 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 12510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12512 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12514 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter SceneName */

/* 12516 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 12518 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12520 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter red */

/* 12522 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12524 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12526 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter green */

/* 12528 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12530 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12532 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter blue */

/* 12534 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12536 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 12538 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter alpha */

/* 12540 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12542 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 12544 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 12546 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12548 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 12550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetButtonRect */

/* 12552 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12554 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12558 */	NdrFcShort( 0x123 ),	/* 291 */
/* 12560 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 12562 */	NdrFcShort( 0x20 ),	/* 32 */
/* 12564 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12566 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 12568 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 12570 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12572 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12574 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter buttonName */

/* 12576 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 12578 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12580 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter x */

/* 12582 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12584 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12586 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter y */

/* 12588 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12590 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12592 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter width */

/* 12594 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12596 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 12598 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 12600 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12602 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 12604 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 12606 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12608 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 12610 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAddtionalKeys */

/* 12612 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12614 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12618 */	NdrFcShort( 0x124 ),	/* 292 */
/* 12620 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 12622 */	NdrFcShort( 0x14 ),	/* 20 */
/* 12624 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12626 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x5,		/* 5 */
/* 12628 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12634 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter left */

/* 12636 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12638 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12640 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Parameter right */

/* 12642 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12644 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12646 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Parameter up */

/* 12648 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12650 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12652 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Parameter down */

/* 12654 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12656 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 12658 */	0x1,		/* FC_BYTE */
			0x0,		/* 0 */

	/* Return value */

/* 12660 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12662 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 12664 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAddtionalKeysStatus */

/* 12666 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12668 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12672 */	NdrFcShort( 0x125 ),	/* 293 */
/* 12674 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12676 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12678 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12680 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12682 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12684 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12686 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12688 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bEnable */

/* 12690 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12692 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12694 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 12696 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12698 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12700 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetSubBrowserType */

/* 12702 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12704 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12708 */	NdrFcShort( 0x126 ),	/* 294 */
/* 12710 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12712 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12714 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12716 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12718 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12720 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12724 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter subType */

/* 12726 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12728 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12730 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 12732 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12734 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12736 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_wbType */

/* 12738 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12740 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12744 */	NdrFcShort( 0x127 ),	/* 295 */
/* 12746 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12750 */	NdrFcShort( 0x24 ),	/* 36 */
/* 12752 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12754 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12758 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12760 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 12762 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 12764 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12766 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 12768 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12770 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12772 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_wbType */

/* 12774 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 12776 */	NdrFcLong( 0x0 ),	/* 0 */
/* 12780 */	NdrFcShort( 0x128 ),	/* 296 */
/* 12782 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 12784 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12786 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12788 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 12790 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 12792 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12796 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 12798 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 12800 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 12802 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 12804 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 12806 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 12808 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const VGSATL2_MIDL_TYPE_FORMAT_STRING VGSATL2__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0xe ),	/* Offset= 14 (18) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 14 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 16 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 18 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 20 */	NdrFcShort( 0x8 ),	/* 8 */
/* 22 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (6) */
/* 24 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 26 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 28 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 30 */	NdrFcShort( 0x0 ),	/* 0 */
/* 32 */	NdrFcShort( 0x4 ),	/* 4 */
/* 34 */	NdrFcShort( 0x0 ),	/* 0 */
/* 36 */	NdrFcShort( 0xffde ),	/* Offset= -34 (2) */
/* 38 */	
			0x11, 0x0,	/* FC_RP */
/* 40 */	NdrFcShort( 0x3e6 ),	/* Offset= 998 (1038) */
/* 42 */	
			0x12, 0x0,	/* FC_UP */
/* 44 */	NdrFcShort( 0x3ce ),	/* Offset= 974 (1018) */
/* 46 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 48 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 50 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 52 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 54 */	NdrFcShort( 0x2 ),	/* Offset= 2 (56) */
/* 56 */	NdrFcShort( 0x10 ),	/* 16 */
/* 58 */	NdrFcShort( 0x2f ),	/* 47 */
/* 60 */	NdrFcLong( 0x14 ),	/* 20 */
/* 64 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 66 */	NdrFcLong( 0x3 ),	/* 3 */
/* 70 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 72 */	NdrFcLong( 0x11 ),	/* 17 */
/* 76 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 78 */	NdrFcLong( 0x2 ),	/* 2 */
/* 82 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 84 */	NdrFcLong( 0x4 ),	/* 4 */
/* 88 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 90 */	NdrFcLong( 0x5 ),	/* 5 */
/* 94 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 96 */	NdrFcLong( 0xb ),	/* 11 */
/* 100 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 102 */	NdrFcLong( 0xa ),	/* 10 */
/* 106 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 108 */	NdrFcLong( 0x6 ),	/* 6 */
/* 112 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (344) */
/* 114 */	NdrFcLong( 0x7 ),	/* 7 */
/* 118 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 120 */	NdrFcLong( 0x8 ),	/* 8 */
/* 124 */	NdrFcShort( 0xff86 ),	/* Offset= -122 (2) */
/* 126 */	NdrFcLong( 0xd ),	/* 13 */
/* 130 */	NdrFcShort( 0xdc ),	/* Offset= 220 (350) */
/* 132 */	NdrFcLong( 0x9 ),	/* 9 */
/* 136 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (368) */
/* 138 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 142 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (386) */
/* 144 */	NdrFcLong( 0x24 ),	/* 36 */
/* 148 */	NdrFcShort( 0x31c ),	/* Offset= 796 (944) */
/* 150 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 154 */	NdrFcShort( 0x316 ),	/* Offset= 790 (944) */
/* 156 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 160 */	NdrFcShort( 0x314 ),	/* Offset= 788 (948) */
/* 162 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 166 */	NdrFcShort( 0x312 ),	/* Offset= 786 (952) */
/* 168 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 172 */	NdrFcShort( 0x310 ),	/* Offset= 784 (956) */
/* 174 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 178 */	NdrFcShort( 0x30e ),	/* Offset= 782 (960) */
/* 180 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 184 */	NdrFcShort( 0x30c ),	/* Offset= 780 (964) */
/* 186 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 190 */	NdrFcShort( 0x30a ),	/* Offset= 778 (968) */
/* 192 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 196 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (952) */
/* 198 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 202 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (956) */
/* 204 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 208 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (972) */
/* 210 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 214 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (968) */
/* 216 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 220 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (976) */
/* 222 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 226 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (980) */
/* 228 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 232 */	NdrFcShort( 0x2f0 ),	/* Offset= 752 (984) */
/* 234 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 238 */	NdrFcShort( 0x2ee ),	/* Offset= 750 (988) */
/* 240 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 244 */	NdrFcShort( 0x2ec ),	/* Offset= 748 (992) */
/* 246 */	NdrFcLong( 0x10 ),	/* 16 */
/* 250 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 252 */	NdrFcLong( 0x12 ),	/* 18 */
/* 256 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 258 */	NdrFcLong( 0x13 ),	/* 19 */
/* 262 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 264 */	NdrFcLong( 0x15 ),	/* 21 */
/* 268 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 270 */	NdrFcLong( 0x16 ),	/* 22 */
/* 274 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 276 */	NdrFcLong( 0x17 ),	/* 23 */
/* 280 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 282 */	NdrFcLong( 0xe ),	/* 14 */
/* 286 */	NdrFcShort( 0x2ca ),	/* Offset= 714 (1000) */
/* 288 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 292 */	NdrFcShort( 0x2ce ),	/* Offset= 718 (1010) */
/* 294 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 298 */	NdrFcShort( 0x2cc ),	/* Offset= 716 (1014) */
/* 300 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 304 */	NdrFcShort( 0x288 ),	/* Offset= 648 (952) */
/* 306 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 310 */	NdrFcShort( 0x286 ),	/* Offset= 646 (956) */
/* 312 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 316 */	NdrFcShort( 0x284 ),	/* Offset= 644 (960) */
/* 318 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 322 */	NdrFcShort( 0x27a ),	/* Offset= 634 (956) */
/* 324 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 328 */	NdrFcShort( 0x274 ),	/* Offset= 628 (956) */
/* 330 */	NdrFcLong( 0x0 ),	/* 0 */
/* 334 */	NdrFcShort( 0x0 ),	/* Offset= 0 (334) */
/* 336 */	NdrFcLong( 0x1 ),	/* 1 */
/* 340 */	NdrFcShort( 0x0 ),	/* Offset= 0 (340) */
/* 342 */	NdrFcShort( 0xffff ),	/* Offset= -1 (341) */
/* 344 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 346 */	NdrFcShort( 0x8 ),	/* 8 */
/* 348 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 350 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 352 */	NdrFcLong( 0x0 ),	/* 0 */
/* 356 */	NdrFcShort( 0x0 ),	/* 0 */
/* 358 */	NdrFcShort( 0x0 ),	/* 0 */
/* 360 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 362 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 364 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 366 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 368 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 370 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 374 */	NdrFcShort( 0x0 ),	/* 0 */
/* 376 */	NdrFcShort( 0x0 ),	/* 0 */
/* 378 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 380 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 382 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 384 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 386 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 388 */	NdrFcShort( 0x2 ),	/* Offset= 2 (390) */
/* 390 */	
			0x12, 0x0,	/* FC_UP */
/* 392 */	NdrFcShort( 0x216 ),	/* Offset= 534 (926) */
/* 394 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 396 */	NdrFcShort( 0x18 ),	/* 24 */
/* 398 */	NdrFcShort( 0xa ),	/* 10 */
/* 400 */	NdrFcLong( 0x8 ),	/* 8 */
/* 404 */	NdrFcShort( 0x5a ),	/* Offset= 90 (494) */
/* 406 */	NdrFcLong( 0xd ),	/* 13 */
/* 410 */	NdrFcShort( 0x7e ),	/* Offset= 126 (536) */
/* 412 */	NdrFcLong( 0x9 ),	/* 9 */
/* 416 */	NdrFcShort( 0x9e ),	/* Offset= 158 (574) */
/* 418 */	NdrFcLong( 0xc ),	/* 12 */
/* 422 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (622) */
/* 424 */	NdrFcLong( 0x24 ),	/* 36 */
/* 428 */	NdrFcShort( 0x124 ),	/* Offset= 292 (720) */
/* 430 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 434 */	NdrFcShort( 0x140 ),	/* Offset= 320 (754) */
/* 436 */	NdrFcLong( 0x10 ),	/* 16 */
/* 440 */	NdrFcShort( 0x15a ),	/* Offset= 346 (786) */
/* 442 */	NdrFcLong( 0x2 ),	/* 2 */
/* 446 */	NdrFcShort( 0x174 ),	/* Offset= 372 (818) */
/* 448 */	NdrFcLong( 0x3 ),	/* 3 */
/* 452 */	NdrFcShort( 0x18e ),	/* Offset= 398 (850) */
/* 454 */	NdrFcLong( 0x14 ),	/* 20 */
/* 458 */	NdrFcShort( 0x1a8 ),	/* Offset= 424 (882) */
/* 460 */	NdrFcShort( 0xffff ),	/* Offset= -1 (459) */
/* 462 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 464 */	NdrFcShort( 0x4 ),	/* 4 */
/* 466 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 470 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 472 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 474 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 476 */	NdrFcShort( 0x4 ),	/* 4 */
/* 478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 480 */	NdrFcShort( 0x1 ),	/* 1 */
/* 482 */	NdrFcShort( 0x0 ),	/* 0 */
/* 484 */	NdrFcShort( 0x0 ),	/* 0 */
/* 486 */	0x12, 0x0,	/* FC_UP */
/* 488 */	NdrFcShort( 0xfe2a ),	/* Offset= -470 (18) */
/* 490 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 492 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 494 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 496 */	NdrFcShort( 0x8 ),	/* 8 */
/* 498 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 500 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 502 */	NdrFcShort( 0x4 ),	/* 4 */
/* 504 */	NdrFcShort( 0x4 ),	/* 4 */
/* 506 */	0x11, 0x0,	/* FC_RP */
/* 508 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (462) */
/* 510 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 512 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 514 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 516 */	NdrFcShort( 0x0 ),	/* 0 */
/* 518 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 522 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 524 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 528 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 530 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 532 */	NdrFcShort( 0xff4a ),	/* Offset= -182 (350) */
/* 534 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 536 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 538 */	NdrFcShort( 0x8 ),	/* 8 */
/* 540 */	NdrFcShort( 0x0 ),	/* 0 */
/* 542 */	NdrFcShort( 0x6 ),	/* Offset= 6 (548) */
/* 544 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 546 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 548 */	
			0x11, 0x0,	/* FC_RP */
/* 550 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (514) */
/* 552 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 560 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 562 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 566 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 568 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 570 */	NdrFcShort( 0xff36 ),	/* Offset= -202 (368) */
/* 572 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 574 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 576 */	NdrFcShort( 0x8 ),	/* 8 */
/* 578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 580 */	NdrFcShort( 0x6 ),	/* Offset= 6 (586) */
/* 582 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 584 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 586 */	
			0x11, 0x0,	/* FC_RP */
/* 588 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (552) */
/* 590 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 592 */	NdrFcShort( 0x4 ),	/* 4 */
/* 594 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 598 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 600 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 602 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 604 */	NdrFcShort( 0x4 ),	/* 4 */
/* 606 */	NdrFcShort( 0x0 ),	/* 0 */
/* 608 */	NdrFcShort( 0x1 ),	/* 1 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	0x12, 0x0,	/* FC_UP */
/* 616 */	NdrFcShort( 0x192 ),	/* Offset= 402 (1018) */
/* 618 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 620 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 622 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 624 */	NdrFcShort( 0x8 ),	/* 8 */
/* 626 */	NdrFcShort( 0x0 ),	/* 0 */
/* 628 */	NdrFcShort( 0x6 ),	/* Offset= 6 (634) */
/* 630 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 632 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 634 */	
			0x11, 0x0,	/* FC_RP */
/* 636 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (590) */
/* 638 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 640 */	NdrFcLong( 0x2f ),	/* 47 */
/* 644 */	NdrFcShort( 0x0 ),	/* 0 */
/* 646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 648 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 650 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 652 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 654 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 656 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 658 */	NdrFcShort( 0x1 ),	/* 1 */
/* 660 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 662 */	NdrFcShort( 0x4 ),	/* 4 */
/* 664 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 666 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 668 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 670 */	NdrFcShort( 0x10 ),	/* 16 */
/* 672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 674 */	NdrFcShort( 0xa ),	/* Offset= 10 (684) */
/* 676 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 678 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 680 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (638) */
/* 682 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 684 */	
			0x12, 0x0,	/* FC_UP */
/* 686 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (656) */
/* 688 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 690 */	NdrFcShort( 0x4 ),	/* 4 */
/* 692 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 694 */	NdrFcShort( 0x0 ),	/* 0 */
/* 696 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 698 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 700 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 702 */	NdrFcShort( 0x4 ),	/* 4 */
/* 704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x1 ),	/* 1 */
/* 708 */	NdrFcShort( 0x0 ),	/* 0 */
/* 710 */	NdrFcShort( 0x0 ),	/* 0 */
/* 712 */	0x12, 0x0,	/* FC_UP */
/* 714 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (668) */
/* 716 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 718 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 720 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 722 */	NdrFcShort( 0x8 ),	/* 8 */
/* 724 */	NdrFcShort( 0x0 ),	/* 0 */
/* 726 */	NdrFcShort( 0x6 ),	/* Offset= 6 (732) */
/* 728 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 730 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 732 */	
			0x11, 0x0,	/* FC_RP */
/* 734 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (688) */
/* 736 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 738 */	NdrFcShort( 0x8 ),	/* 8 */
/* 740 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 742 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 744 */	NdrFcShort( 0x10 ),	/* 16 */
/* 746 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 748 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 750 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (736) */
			0x5b,		/* FC_END */
/* 754 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 756 */	NdrFcShort( 0x18 ),	/* 24 */
/* 758 */	NdrFcShort( 0x0 ),	/* 0 */
/* 760 */	NdrFcShort( 0xa ),	/* Offset= 10 (770) */
/* 762 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 764 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 766 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (742) */
/* 768 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 770 */	
			0x11, 0x0,	/* FC_RP */
/* 772 */	NdrFcShort( 0xfefe ),	/* Offset= -258 (514) */
/* 774 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 776 */	NdrFcShort( 0x1 ),	/* 1 */
/* 778 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 780 */	NdrFcShort( 0x0 ),	/* 0 */
/* 782 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 784 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 786 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 788 */	NdrFcShort( 0x8 ),	/* 8 */
/* 790 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 792 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 794 */	NdrFcShort( 0x4 ),	/* 4 */
/* 796 */	NdrFcShort( 0x4 ),	/* 4 */
/* 798 */	0x12, 0x0,	/* FC_UP */
/* 800 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (774) */
/* 802 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 804 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 806 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 808 */	NdrFcShort( 0x2 ),	/* 2 */
/* 810 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 814 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 816 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 818 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 820 */	NdrFcShort( 0x8 ),	/* 8 */
/* 822 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 824 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 826 */	NdrFcShort( 0x4 ),	/* 4 */
/* 828 */	NdrFcShort( 0x4 ),	/* 4 */
/* 830 */	0x12, 0x0,	/* FC_UP */
/* 832 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (806) */
/* 834 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 836 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 838 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 840 */	NdrFcShort( 0x4 ),	/* 4 */
/* 842 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 846 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 848 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 850 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 852 */	NdrFcShort( 0x8 ),	/* 8 */
/* 854 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 856 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 858 */	NdrFcShort( 0x4 ),	/* 4 */
/* 860 */	NdrFcShort( 0x4 ),	/* 4 */
/* 862 */	0x12, 0x0,	/* FC_UP */
/* 864 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (838) */
/* 866 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 868 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 870 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 872 */	NdrFcShort( 0x8 ),	/* 8 */
/* 874 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 876 */	NdrFcShort( 0x0 ),	/* 0 */
/* 878 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 880 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 882 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 884 */	NdrFcShort( 0x8 ),	/* 8 */
/* 886 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 888 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 890 */	NdrFcShort( 0x4 ),	/* 4 */
/* 892 */	NdrFcShort( 0x4 ),	/* 4 */
/* 894 */	0x12, 0x0,	/* FC_UP */
/* 896 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (870) */
/* 898 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 900 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 902 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 904 */	NdrFcShort( 0x8 ),	/* 8 */
/* 906 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 908 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 910 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 912 */	NdrFcShort( 0x8 ),	/* 8 */
/* 914 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 916 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 918 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 920 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 922 */	NdrFcShort( 0xffec ),	/* Offset= -20 (902) */
/* 924 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 926 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 928 */	NdrFcShort( 0x28 ),	/* 40 */
/* 930 */	NdrFcShort( 0xffec ),	/* Offset= -20 (910) */
/* 932 */	NdrFcShort( 0x0 ),	/* Offset= 0 (932) */
/* 934 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 936 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 938 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 940 */	NdrFcShort( 0xfdde ),	/* Offset= -546 (394) */
/* 942 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 944 */	
			0x12, 0x0,	/* FC_UP */
/* 946 */	NdrFcShort( 0xfeea ),	/* Offset= -278 (668) */
/* 948 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 950 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 952 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 954 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 956 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 958 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 960 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 962 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 964 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 966 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 968 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 970 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 972 */	
			0x12, 0x0,	/* FC_UP */
/* 974 */	NdrFcShort( 0xfd8a ),	/* Offset= -630 (344) */
/* 976 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 978 */	NdrFcShort( 0xfc30 ),	/* Offset= -976 (2) */
/* 980 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 982 */	NdrFcShort( 0xfd88 ),	/* Offset= -632 (350) */
/* 984 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 986 */	NdrFcShort( 0xfd96 ),	/* Offset= -618 (368) */
/* 988 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 990 */	NdrFcShort( 0xfda4 ),	/* Offset= -604 (386) */
/* 992 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 994 */	NdrFcShort( 0x2 ),	/* Offset= 2 (996) */
/* 996 */	
			0x12, 0x0,	/* FC_UP */
/* 998 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1018) */
/* 1000 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1002 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1004 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1006 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1008 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1010 */	
			0x12, 0x0,	/* FC_UP */
/* 1012 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1000) */
/* 1014 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1016 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1018 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1020 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1022 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1024 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1024) */
/* 1026 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1028 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1030 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1032 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1034 */	NdrFcShort( 0xfc24 ),	/* Offset= -988 (46) */
/* 1036 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1038 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1040 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1042 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1044 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1046 */	NdrFcShort( 0xfc14 ),	/* Offset= -1004 (42) */
/* 1048 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1050 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1052 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1054 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1056 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1058 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1060 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1062 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1068) */
/* 1064 */	
			0x13, 0x0,	/* FC_OP */
/* 1066 */	NdrFcShort( 0xfbe8 ),	/* Offset= -1048 (18) */
/* 1068 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1070 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1072 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1074 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1076 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1064) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IVGS2, ver. 0.0,
   GUID={0xF66246AF,0xD0D7,0x46D3,{0x89,0x1F,0x9E,0xFB,0xC9,0xFA,0xC2,0x57}} */

#pragma code_seg(".orpc")
static const unsigned short IVGS2_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    42,
    96,
    174,
    240,
    282,
    318,
    360,
    396,
    462,
    522,
    558,
    594,
    642,
    684,
    726,
    774,
    822,
    864,
    900,
    936,
    978,
    1026,
    1074,
    1110,
    1146,
    1188,
    1266,
    1302,
    1344,
    1452,
    1488,
    1548,
    1584,
    1620,
    1662,
    1698,
    1746,
    1788,
    1866,
    1902,
    1944,
    1986,
    2028,
    2088,
    2136,
    2184,
    2232,
    2280,
    2328,
    2370,
    2400,
    2448,
    2490,
    2526,
    2586,
    2628,
    2658,
    2688,
    2718,
    2748,
    2778,
    2808,
    2838,
    2868,
    2916,
    2958,
    2994,
    3078,
    3138,
    3204,
    3276,
    3306,
    3336,
    3378,
    3414,
    3444,
    3510,
    3570,
    3606,
    3648,
    3708,
    3774,
    3810,
    3840,
    3870,
    3900,
    3930,
    3960,
    3990,
    4020,
    4050,
    4080,
    4110,
    4140,
    4170,
    4200,
    4230,
    4290,
    4332,
    4374,
    4416,
    4458,
    4512,
    4566,
    4608,
    4644,
    4680,
    4716,
    4746,
    4776,
    4836,
    4878,
    4926,
    4980,
    5052,
    5088,
    5136,
    5172,
    5214,
    5250,
    5298,
    5382,
    5418,
    5460,
    5496,
    5526,
    5568,
    5610,
    5646,
    5682,
    5760,
    5808,
    5862,
    5904,
    5958,
    6012,
    6054,
    6108,
    6150,
    6216,
    6258,
    6300,
    6336,
    6414,
    6474,
    6516,
    6606,
    6642,
    6684,
    6720,
    6756,
    6792,
    6828,
    6870,
    6912,
    6966,
    7002,
    7038,
    7080,
    7116,
    7158,
    7200,
    7332,
    7368,
    7410,
    7458,
    7494,
    7530,
    7560,
    7602,
    7644,
    7680,
    7716,
    7752,
    7788,
    7824,
    7860,
    7890,
    7932,
    7974,
    8010,
    8052,
    8094,
    8130,
    8172,
    8208,
    8244,
    8298,
    8352,
    8430,
    8478,
    8520,
    8556,
    8592,
    8646,
    8682,
    8730,
    8766,
    8802,
    8832,
    8862,
    8892,
    8922,
    8958,
    8994,
    9030,
    9072,
    9114,
    9144,
    9174,
    9228,
    9282,
    9324,
    9366,
    9408,
    9450,
    9486,
    9522,
    9564,
    9600,
    9648,
    9702,
    9744,
    9792,
    9834,
    9876,
    9912,
    9954,
    9990,
    10026,
    10098,
    10134,
    10170,
    10206,
    10242,
    10278,
    10314,
    10350,
    10386,
    10434,
    10494,
    10530,
    10566,
    10602,
    10638,
    10674,
    10710,
    10746,
    10782,
    10824,
    10968,
    11004,
    11148,
    11208,
    11250,
    11304,
    11346,
    11388,
    11454,
    11496,
    11538,
    11580,
    11610,
    11670,
    11718,
    11760,
    11814,
    11862,
    11904,
    11940,
    11982,
    12030,
    12132,
    12168,
    12204,
    12240,
    12276,
    12312,
    12348,
    12384,
    12420,
    12456,
    12492,
    12552,
    12612,
    12666,
    12702,
    12738,
    12774
    };

static const MIDL_STUBLESS_PROXY_INFO IVGS2_ProxyInfo =
    {
    &Object_StubDesc,
    VGSATL2__MIDL_ProcFormatString.Format,
    &IVGS2_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IVGS2_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    VGSATL2__MIDL_ProcFormatString.Format,
    &IVGS2_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(297) _IVGS2ProxyVtbl = 
{
    &IVGS2_ProxyInfo,
    &IID_IVGS2,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetModelVisibility */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetDynamicUV */ ,
    (void *) (INT_PTR) -1 /* IVGS2::AddFlashCtrl */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFlashRect */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFlashCtrlVisibility */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetRenderStatus */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFlashPlayStatus */ ,
    (void *) (INT_PTR) -1 /* IVGS2::DeleteFlashCtrl */ ,
    (void *) (INT_PTR) -1 /* IVGS2::MoveCamera */ ,
    (void *) (INT_PTR) -1 /* IVGS2::MoveCurrentCamera */ ,
    (void *) (INT_PTR) -1 /* IVGS2::ShowDefaultLogo */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetControlStatus */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCameraClip */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCurrentCameraClip */ ,
    (void *) (INT_PTR) -1 /* IVGS2::DollyCurrentCamera */ ,
    (void *) (INT_PTR) -1 /* IVGS2::DollyCamera */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetBackgroundColor */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PreloadN3DFile */ ,
    (void *) (INT_PTR) -1 /* IVGS2::GotoN3D */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCurrentCameraByName */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayCameraKfr */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCurrentCameraFrom */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCurrentCameraAt */ ,
    (void *) (INT_PTR) -1 /* IVGS2::StopCameraKfr */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetServer */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetModelClickable */ ,
    (void *) (INT_PTR) -1 /* IVGS2::CreateOverlayFromImage */ ,
    (void *) (INT_PTR) -1 /* IVGS2::DeleteOverlay */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetOverlayOpacity */ ,
    (void *) (INT_PTR) -1 /* IVGS2::CreateButtonFromImage */ ,
    (void *) (INT_PTR) -1 /* IVGS2::DeleteButton */ ,
    (void *) (INT_PTR) -1 /* IVGS2::TranslateModel */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetEditCtrl_DollySpeed */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetEditCtrl_RosAxis */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetEditCtrl_DollyDistance */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetEditCtrl_AutoPitchRoundRoll */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetEditCtrl_AutoParam */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetEditCtrl_ManualParam */ ,
    (void *) (INT_PTR) -1 /* IVGS2::MoveCurrentCameraTo */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFBL */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetOverlayWidth */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetOverlayHeight */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetOverlayAlignMode */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetOverlayRect */ ,
    (void *) (INT_PTR) -1 /* IVGS2::MoveOverlay */ ,
    (void *) (INT_PTR) -1 /* IVGS2::MoveCurrentCameraFromTo */ ,
    (void *) (INT_PTR) -1 /* IVGS2::MoveCurrentCameraAtTo */ ,
    (void *) (INT_PTR) -1 /* IVGS2::MoveCurrentCameraFrom */ ,
    (void *) (INT_PTR) -1 /* IVGS2::MoveCurrentCameraAt */ ,
    (void *) (INT_PTR) -1 /* IVGS2::LoadN3DFile */ ,
    (void *) (INT_PTR) -1 /* IVGS2::ResetScene */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetModelMtrl */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetMtrlOpacity */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCurrentCameraByIndex */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetWalkCtrl_ManualParam */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetModelMouseHint */ ,
    (void *) (INT_PTR) -1 /* IVGS2::EnableMouseDownCallback */ ,
    (void *) (INT_PTR) -1 /* IVGS2::DisableMouseDownCallback */ ,
    (void *) (INT_PTR) -1 /* IVGS2::EnableMouseUpCallback */ ,
    (void *) (INT_PTR) -1 /* IVGS2::DisableMouseUpCallback */ ,
    (void *) (INT_PTR) -1 /* IVGS2::EnableMouseDBClickCallback */ ,
    (void *) (INT_PTR) -1 /* IVGS2::DisableMouseDBClickCallback */ ,
    (void *) (INT_PTR) -1 /* IVGS2::EnableMouseMoveCallback */ ,
    (void *) (INT_PTR) -1 /* IVGS2::DisableMouseMoveCallback */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetModelMouseMtrl */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetModelOpacity */ ,
    (void *) (INT_PTR) -1 /* IVGS2::CreateModelGroup */ ,
    (void *) (INT_PTR) -1 /* IVGS2::CreateBumpWaterTexture */ ,
    (void *) (INT_PTR) -1 /* IVGS2::RotateCurrentCamera */ ,
    (void *) (INT_PTR) -1 /* IVGS2::RotateCamera */ ,
    (void *) (INT_PTR) -1 /* IVGS2::FocusCurrentCameraTo */ ,
    (void *) (INT_PTR) -1 /* IVGS2::HideAllModels */ ,
    (void *) (INT_PTR) -1 /* IVGS2::ShowAllModels */ ,
    (void *) (INT_PTR) -1 /* IVGS2::GetModelHandleByName */ ,
    (void *) (INT_PTR) -1 /* IVGS2::RestoreModelMtrl */ ,
    (void *) (INT_PTR) -1 /* IVGS2::RestoreAllModelsMtrl */ ,
    (void *) (INT_PTR) -1 /* IVGS2::CreateMtrl */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetMtrlColor */ ,
    (void *) (INT_PTR) -1 /* IVGS2::DeleteMtrl */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetMtrlIllumination */ ,
    (void *) (INT_PTR) -1 /* IVGS2::MoveModel */ ,
    (void *) (INT_PTR) -1 /* IVGS2::MoveModelTo */ ,
    (void *) (INT_PTR) -1 /* IVGS2::RestoreModelOpacity */ ,
    (void *) (INT_PTR) -1 /* IVGS2::RestoreAllModelsOpacity */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerGoForward */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerGoBackward */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerGoLeftward */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerGoRightward */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerStop */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerTurnLeft */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerTurnRight */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerTurnUp */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerTurnDown */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerZoomIn */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerZoomOut */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerGoUp */ ,
    (void *) (INT_PTR) -1 /* IVGS2::PlayerGoDown */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetModelColor */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetModelFBLStatus */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetBumpWaterSpeed */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetBumpWaterRadius */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetSkyBoxModel */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCameraFrom */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCameraAt */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCameraFov */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCurrentCameraFov */ ,
    (void *) (INT_PTR) -1 /* IVGS2::CreateFSMotionBlur */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFSMotionBlurPower */ ,
    (void *) (INT_PTR) -1 /* IVGS2::StartFSMotionBlur */ ,
    (void *) (INT_PTR) -1 /* IVGS2::StopFSMotionBlur */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCameraKFRFSMotionBlur */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFlashCurrentFrame */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetCameraKfrCallBackFunc */ ,
    (void *) (INT_PTR) -1 /* IVGS2::CreateLensFlare */ ,
    (void *) (INT_PTR) -1 /* IVGS2::CreateFog */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFogMode */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFogColor */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFogDensity */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFogField */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFogRenderStatus */ ,
    (void *) (INT_PTR) -1 /* IVGS2::SetFilter */ ,
    IVGS2_SetModelMouseColor_Proxy ,
    IVGS2_SetMouseMoveEventTimer_Proxy ,
    IVGS2_SetNavigaterVisibility_Proxy ,
    IVGS2_RestoreModelColor_Proxy ,
    IVGS2_RestoreAllModelsColor_Proxy ,
    IVGS2_SetAlphaTestStatus_Proxy ,
    IVGS2_SetZOneModel_Proxy ,
    IVGS2_SetLensFlareVisibility_Proxy ,
    IVGS2_SetCollisionDistance_Proxy ,
    IVGS2_CreateStaticCubeMap_Proxy ,
    IVGS2_SetMtrlCubeMap_Proxy ,
    IVGS2_SetLightColor_Proxy ,
    IVGS2_SetLightPower_Proxy ,
    IVGS2_SetLightAmbient_Proxy ,
    IVGS2_SetLightPosition_Proxy ,
    IVGS2_SetLightRange_Proxy ,
    IVGS2_SetLightAttenuation_Proxy ,
    IVGS2_SetLightStatus_Proxy ,
    IVGS2_CreateMovieTexture_Proxy ,
    IVGS2_SetMovieMtrl_Proxy ,
    IVGS2_SetModelKfrRepeatCnt_Proxy ,
    IVGS2_SetAllModelKfrRepeatCnt_Proxy ,
    IVGS2_CreateBackdropFromImage_Proxy ,
    IVGS2_CreateSpecularMap_Proxy ,
    IVGS2_SetSpecularMapMtrl_Proxy ,
    IVGS2_CreateCamera_Proxy ,
    IVGS2_DeleteModel_Proxy ,
    IVGS2_SetCanNavigateByPointer_Proxy ,
    IVGS2_SetPlayerCanMove_Proxy ,
    IVGS2_SetPlayerCanRos_Proxy ,
    IVGS2_SetPlayerCanZoom_Proxy ,
    IVGS2_SetPlayerCanLift_Proxy ,
    IVGS2_DeleteMovieTexture_Proxy ,
    IVGS2_SetModelKfrPlayStatus_Proxy ,
    IVGS2_PlayModelKfr_Proxy ,
    IVGS2_SetAllModelsKfrPlayStatus_Proxy ,
    IVGS2_SetModelEventOnMouseMove_Proxy ,
    IVGS2_SetButtonVisibility_Proxy ,
    IVGS2_SetAntiAliasLevel_Proxy ,
    IVGS2_SetMtrlSpecularCubeMap_Proxy ,
    IVGS2_SetButtonActive_Proxy ,
    IVGS2_CreateSectionMap_Proxy ,
    IVGS2_ExecuteSlice_Proxy ,
    IVGS2_SetOverlayVisibility_Proxy ,
    IVGS2_CreateAudio_Proxy ,
    IVGS2_DeleteAudioByName_Proxy ,
    IVGS2_DeleteAudioByIndex_Proxy ,
    IVGS2_DeleteAllAudio_Proxy ,
    IVGS2_SetAudioFile_Proxy ,
    IVGS2_SetAudioRepeatCount_Proxy ,
    IVGS2_PlayAudio_Proxy ,
    IVGS2_PauseAudio_Proxy ,
    IVGS2_StopAudio_Proxy ,
    IVGS2_RewindAudio_Proxy ,
    IVGS2_DeleteModelGroup_Proxy ,
    IVGS2_DeleteModelGroupByIndex_Proxy ,
    IVGS2_DeleteAllModelGroup_Proxy ,
    IVGS2_AddModelToModelGroup_Proxy ,
    IVGS2_DeleteModelFromModelGroup_Proxy ,
    IVGS2_ClearModelGroup_Proxy ,
    IVGS2_SetModelMouseHintType_Proxy ,
    IVGS2_SetLightKfrPlayStatus_Proxy ,
    IVGS2_SetAllLightKfrPlayStatus_Proxy ,
    IVGS2_SetLightKfrRepeatCnt_Proxy ,
    IVGS2_SetAllLightKfrRepeatCnt_Proxy ,
    IVGS2_SetAllModelKfrPlayStatus_Proxy ,
    IVGS2_SetModelPosition_Proxy ,
    IVGS2_SetModelRotation_Proxy ,
    IVGS2_AddNavigaterPointer_Proxy ,
    IVGS2_SetNavigaterPointerVisibility_Proxy ,
    IVGS2_DeleteNavigaterPointer_Proxy ,
    IVGS2_PauseCameraKfr_Proxy ,
    IVGS2_ResumeCameraKfr_Proxy ,
    IVGS2_SetNavigaterPointerObject_Proxy ,
    IVGS2_SetPlayerZoomSpeed_Proxy ,
    IVGS2_SetOverlayFullScreen_Proxy ,
    IVGS2_PushOverlayFront_Proxy ,
    IVGS2_PushOverlayBack_Proxy ,
    IVGS2_DeleteAllModel_Proxy ,
    IVGS2_PlayKeyFrame_Proxy ,
    IVGS2_PauseKeyFrame_Proxy ,
    IVGS2_StopKeyFrame_Proxy ,
    IVGS2_SetKeyFrameRepeatCount_Proxy ,
    IVGS2_GotoFrame_Proxy ,
    IVGS2_SetKeyFramePlaySpeed_Proxy ,
    IVGS2_CreateFSGlow_Proxy ,
    IVGS2_SetFSGlowValue_Proxy ,
    IVGS2_StartFSGlow_Proxy ,
    IVGS2_EndFSGlow_Proxy ,
    IVGS2_SetMtrlSpecular_Proxy ,
    IVGS2_SetMtrlAmbient_Proxy ,
    IVGS2_SetMtrlSpecularLevel_Proxy ,
    IVGS2_SetMtrlSpecularPower_Proxy ,
    IVGS2_PlayKeyFrame1_Proxy ,
    IVGS2_SetCameraKfrPlayStatus_Proxy ,
    IVGS2_SetAllModelVisibility_Proxy ,
    IVGS2_SetAllCameraKfrPlayStatus_Proxy ,
    IVGS2_SetCameraKfrRepeatCnt_Proxy ,
    IVGS2_SetAllCameraKfrRepeatCnt_Proxy ,
    IVGS2_SetDynamicTexPlaySpeed_Proxy ,
    IVGS2_SetModelPositionByPivot_Proxy ,
    IVGS2_SetDynamicUVStatus_Proxy ,
    IVGS2_MoveFlash_Proxy ,
    IVGS2_SetOverlayMtrl_Proxy ,
    IVGS2_SetZoneTriggerModelCallBackFunc_Proxy ,
    IVGS2_SetAlphaTestLevel_Proxy ,
    IVGS2_SetButtonOpacity_Proxy ,
    IVGS2_CurCamGotoFrame_Proxy ,
    IVGS2_SetSceneEditMode_Proxy ,
    IVGS2_CloneModel_Proxy ,
    IVGS2_get_theModelPosX_Proxy ,
    IVGS2_get_theModelPosY_Proxy ,
    IVGS2_get_theModelPosZ_Proxy ,
    IVGS2_get_theModelScreenPosX_Proxy ,
    IVGS2_get_theModelScreenPosY_Proxy ,
    IVGS2_GetInfoFromModel_Proxy ,
    IVGS2_CreateNewText_Proxy ,
    IVGS2_get_theTextHandle_Proxy ,
    IVGS2_SetTextPosition_Proxy ,
    IVGS2_SetTextColor_Proxy ,
    IVGS2_DeleteText_Proxy ,
    IVGS2_GetInfoFromCamera_Proxy ,
    IVGS2_get_theCamFromPosX_Proxy ,
    IVGS2_get_theCamFromPosY_Proxy ,
    IVGS2_get_theCamFromPosZ_Proxy ,
    IVGS2_get_theCamAtPosX_Proxy ,
    IVGS2_get_theCamAtPosY_Proxy ,
    IVGS2_get_theCamAtPosZ_Proxy ,
    IVGS2_SetTextString_Proxy ,
    IVGS2_LoadScene_Proxy ,
    IVGS2_DeleteScene_Proxy ,
    IVGS2_CloneScene_Proxy ,
    IVGS2_CreateRTFTexture_Proxy ,
    IVGS2_AddModelToRTFTexture_Proxy ,
    IVGS2_CreateDynamicCubeMap_Proxy ,
    IVGS2_AddDynamicCubeMapReflectionModel_Proxy ,
    IVGS2_DeleteDynamicCubeMapReflectionModel_Proxy ,
    IVGS2_CreateRealWater_Proxy ,
    IVGS2_AddRealWaterRTTModel_Proxy ,
    IVGS2_AddRealWaterRTFModel_Proxy ,
    IVGS2_SetRealWaterCubeMap_Proxy ,
    IVGS2_CreateColorEffect_Proxy ,
    IVGS2_SetColorEffect_Proxy ,
    IVGS2_SetTextureImageFromURL_Proxy ,
    IVGS2_SetEditCtrlMaxUpDownAngle_Proxy ,
    IVGS2_SetMtrlTexture_Proxy ,
    IVGS2_AddFlashCtrlSemaphore_Proxy ,
    IVGS2_DeleteFlashCtrlSemaphore_Proxy ,
    IVGS2_get_theFlashVar_Proxy ,
    IVGS2_GetFlashVaribleInfo_Proxy ,
    IVGS2_SetFlashVarible_Proxy ,
    IVGS2_LoadN3DScene_Proxy ,
    IVGS2_GetInfoFromSceneResource_Proxy ,
    IVGS2_get_theScenePosX_Proxy ,
    IVGS2_get_theScenePosY_Proxy ,
    IVGS2_get_theScenePosZ_Proxy ,
    IVGS2_get_theSceneRotX_Proxy ,
    IVGS2_get_theSceneRotY_Proxy ,
    IVGS2_get_theSceneRotZ_Proxy ,
    IVGS2_get_theSceneScaleX_Proxy ,
    IVGS2_get_theSceneScaleY_Proxy ,
    IVGS2_get_theSceneScaleZ_Proxy ,
    IVGS2_SetSceneResColor_Proxy ,
    IVGS2_SetButtonRect_Proxy ,
    IVGS2_SetAddtionalKeys_Proxy ,
    IVGS2_SetAddtionalKeysStatus_Proxy ,
    IVGS2_SetSubBrowserType_Proxy ,
    IVGS2_get_wbType_Proxy ,
    IVGS2_put_wbType_Proxy
};


static const PRPC_STUB_FUNCTION IVGS2_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IVGS2StubVtbl =
{
    &IID_IVGS2,
    &IVGS2_ServerInfo,
    297,
    &IVGS2_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    VGSATL2__MIDL_TypeFormatString.Format,
    0, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x70001f4, /* MIDL Version 7.0.500 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * _VGSATL2_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IVGS2ProxyVtbl,
    0
};

const CInterfaceStubVtbl * _VGSATL2_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IVGS2StubVtbl,
    0
};

PCInterfaceName const _VGSATL2_InterfaceNamesList[] = 
{
    "IVGS2",
    0
};

const IID *  _VGSATL2_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _VGSATL2_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _VGSATL2, pIID, n)

int __stdcall _VGSATL2_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_VGSATL2_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo VGSATL2_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _VGSATL2_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _VGSATL2_StubVtblList,
    (const PCInterfaceName * ) & _VGSATL2_InterfaceNamesList,
    (const IID ** ) & _VGSATL2_BaseIIDList,
    & _VGSATL2_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

