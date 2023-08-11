# Microsoft Developer Studio Project File - Name="VGSCoreDLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VGSCoreDLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VGSCoreDLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VGSCoreDLL.mak" CFG="VGSCoreDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VGSCoreDLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VGSCoreDLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VGSCoreDLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VGSCOREDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_VGS_DLL" /D "_VGS_CORE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 wininet.lib winmm.lib dxguid.lib d3d9.lib d3dx9.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "VGSCoreDLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug"
# PROP Intermediate_Dir "debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VGSCOREDLL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VGSCOREDLL_EXPORTS" /D "_VGS_DLL" /D "_VGS_CORE" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wininet.lib winmm.lib dxguid.lib d3d9.lib d3dx9.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"D:\StartTeam\0.VGS\VGS3Engine\VGSCoreDLL\debug/VGSCoreDLL.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "VGSCoreDLL - Win32 Release"
# Name "VGSCoreDLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Animation\AnimationManager.cpp
# End Source File
# Begin Source File

SOURCE=..\GUI\background.cpp
# End Source File
# Begin Source File

SOURCE=..\BumpMap.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\CameraKeyFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\CAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\GUI\CButton.cpp
# End Source File
# Begin Source File

SOURCE=..\CCamera.cpp
# End Source File
# Begin Source File

SOURCE=..\CCubeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\CD3DDevice.cpp
# End Source File
# Begin Source File

SOURCE=..\CDynamicTex.cpp
# End Source File
# Begin Source File

SOURCE=..\Controller\CEditor.cpp
# End Source File
# Begin Source File

SOURCE=..\CFace.cpp
# End Source File
# Begin Source File

SOURCE=..\Controller\CFlyer.cpp
# End Source File
# Begin Source File

SOURCE=..\CFont.cpp
# End Source File
# Begin Source File

SOURCE=..\CFSGlow.cpp
# End Source File
# Begin Source File

SOURCE=..\CFullScreenMotionBlur.cpp
# End Source File
# Begin Source File

SOURCE=..\GUI\CGUIManager.cpp
# End Source File
# Begin Source File

SOURCE=..\GUI\CGUIObject.cpp
# End Source File
# Begin Source File

SOURCE=..\CImage.cpp
# End Source File
# Begin Source File

SOURCE=..\CLensFlare.cpp
# End Source File
# Begin Source File

SOURCE=..\CLight.cpp
# End Source File
# Begin Source File

SOURCE=..\CList.cpp
# End Source File
# Begin Source File

SOURCE=..\CListElement.cpp
# End Source File
# Begin Source File

SOURCE=..\CMaterial.cpp
# End Source File
# Begin Source File

SOURCE=..\CModel.cpp
# End Source File
# Begin Source File

SOURCE=..\CModelGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\CMoveAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\GUI\CNavigator.cpp
# End Source File
# Begin Source File

SOURCE=..\Controller\Controller.cpp
# End Source File
# Begin Source File

SOURCE=..\GUI\COverlay.cpp
# End Source File
# Begin Source File

SOURCE=..\CRenderBoundBox.cpp
# End Source File
# Begin Source File

SOURCE=..\CRotateAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\CScaleAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\CScene.cpp
# End Source File
# Begin Source File

SOURCE=..\CSphere.cpp
# End Source File
# Begin Source File

SOURCE=..\CSysFunc.cpp
# End Source File
# Begin Source File

SOURCE=..\CText.cpp
# End Source File
# Begin Source File

SOURCE=..\CTexture.cpp
# End Source File
# Begin Source File

SOURCE=..\CTransform.cpp
# End Source File
# Begin Source File

SOURCE=..\CVertex.cpp
# End Source File
# Begin Source File

SOURCE=..\Controller\CWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\CWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\FileIO\GetS3DFileInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\FileIO\GetV3D.cpp
# End Source File
# Begin Source File

SOURCE=..\FileIO\ImportS3D.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\LightKeyFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\MaterialManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\ModelKeyFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\MovableObject.cpp
# End Source File
# Begin Source File

SOURCE=..\Node.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\NodeAnimation.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\NodeKeyFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\parameterhandler.cpp
# End Source File
# Begin Source File

SOURCE=..\RealWater.cpp
# End Source File
# Begin Source File

SOURCE=..\RenderSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\RenderWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\ResourceManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Root.cpp
# End Source File
# Begin Source File

SOURCE=..\RTFMap.cpp
# End Source File
# Begin Source File

SOURCE=..\FileIO\SaveN3D.cpp
# End Source File
# Begin Source File

SOURCE=..\FileIO\SaveV3D.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\Skeleton.cpp
# End Source File
# Begin Source File

SOURCE=..\software_noisemaker.cpp
# End Source File
# Begin Source File

SOURCE=..\surface.cpp
# End Source File
# Begin Source File

SOURCE=..\SwapChain.cpp
# End Source File
# Begin Source File

SOURCE=..\TextureManager.cpp
# End Source File
# Begin Source File

SOURCE=..\tools.cpp
# End Source File
# Begin Source File

SOURCE=..\VGSCore.cpp
# End Source File
# Begin Source File

SOURCE=..\VGSErrCode.cpp
# End Source File
# Begin Source File

SOURCE=..\Event\VGSMainFrameListener.cpp
# End Source File
# Begin Source File

SOURCE=..\VGSRender.cpp
# End Source File
# Begin Source File

SOURCE=..\VGSString.cpp
# End Source File
# Begin Source File

SOURCE=..\VGSSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\Viewport.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Animation\AnimationManager.h
# End Source File
# Begin Source File

SOURCE=..\GUI\background.h
# End Source File
# Begin Source File

SOURCE=..\BumpMap.h
# End Source File
# Begin Source File

SOURCE=..\Animation\CameraKeyFrame.h
# End Source File
# Begin Source File

SOURCE=..\Animation\CameraParamAnimation.h
# End Source File
# Begin Source File

SOURCE=..\CAxis.h
# End Source File
# Begin Source File

SOURCE=..\GUI\CButton.h
# End Source File
# Begin Source File

SOURCE=..\CCamera.h
# End Source File
# Begin Source File

SOURCE=..\CCubeMap.h
# End Source File
# Begin Source File

SOURCE=..\CD3DDevice.h
# End Source File
# Begin Source File

SOURCE=..\CDynamicTex.h
# End Source File
# Begin Source File

SOURCE=..\Controller\CEditor.h
# End Source File
# Begin Source File

SOURCE=..\CFace.h
# End Source File
# Begin Source File

SOURCE=..\Controller\CFlyer.h
# End Source File
# Begin Source File

SOURCE=..\CFont.h
# End Source File
# Begin Source File

SOURCE=..\CFSGlow.h
# End Source File
# Begin Source File

SOURCE=..\CFullScreenMotionBlur.h
# End Source File
# Begin Source File

SOURCE=..\GUI\CGUIManager.h
# End Source File
# Begin Source File

SOURCE=..\GUI\CGUIObject.h
# End Source File
# Begin Source File

SOURCE=..\CImage.h
# End Source File
# Begin Source File

SOURCE=..\CLensFlare.h
# End Source File
# Begin Source File

SOURCE=..\CLight.h
# End Source File
# Begin Source File

SOURCE=..\CList.h
# End Source File
# Begin Source File

SOURCE=..\CListElement.h
# End Source File
# Begin Source File

SOURCE=..\CManyFVF.h
# End Source File
# Begin Source File

SOURCE=..\CMaterial.h
# End Source File
# Begin Source File

SOURCE=..\CModel.h
# End Source File
# Begin Source File

SOURCE=..\CModelGroup.h
# End Source File
# Begin Source File

SOURCE=..\CMoveAxis.h
# End Source File
# Begin Source File

SOURCE=..\GUI\CNavigator.h
# End Source File
# Begin Source File

SOURCE=..\Controller\Controller.h
# End Source File
# Begin Source File

SOURCE=..\GUI\COverlay.h
# End Source File
# Begin Source File

SOURCE=..\CRenderBoundBox.h
# End Source File
# Begin Source File

SOURCE=..\CRotateAxis.h
# End Source File
# Begin Source File

SOURCE=..\CScaleAxis.h
# End Source File
# Begin Source File

SOURCE=..\CScene.h
# End Source File
# Begin Source File

SOURCE=..\CSphere.h
# End Source File
# Begin Source File

SOURCE=..\CSysFunc.h
# End Source File
# Begin Source File

SOURCE=..\CText.h
# End Source File
# Begin Source File

SOURCE=..\CTexture.h
# End Source File
# Begin Source File

SOURCE=..\CTransform.h
# End Source File
# Begin Source File

SOURCE=..\CVertex.h
# End Source File
# Begin Source File

SOURCE=..\Controller\CWalker.h
# End Source File
# Begin Source File

SOURCE=..\CWindow.h
# End Source File
# Begin Source File

SOURCE=..\FileIO\GetS3DFileInfo.h
# End Source File
# Begin Source File

SOURCE=..\FileIO\GetV3D.h
# End Source File
# Begin Source File

SOURCE=..\FileIO\IMportS3D.h
# End Source File
# Begin Source File

SOURCE=..\InvokeJSFunc.h
# End Source File
# Begin Source File

SOURCE=..\Animation\LightKeyFrame.h
# End Source File
# Begin Source File

SOURCE=..\MaterialManager.h
# End Source File
# Begin Source File

SOURCE=..\Animation\ModelKeyFrame.h
# End Source File
# Begin Source File

SOURCE=..\MovableObject.h
# End Source File
# Begin Source File

SOURCE=..\FileIO\N3D_Def.h
# End Source File
# Begin Source File

SOURCE=..\Node.h
# End Source File
# Begin Source File

SOURCE=..\Animation\NodeAnimation.h
# End Source File
# Begin Source File

SOURCE=..\Animation\NodeKeyFrame.h
# End Source File
# Begin Source File

SOURCE=..\parameterhandler.h
# End Source File
# Begin Source File

SOURCE=..\RealWater.h
# End Source File
# Begin Source File

SOURCE=..\RenderSystem.h
# End Source File
# Begin Source File

SOURCE=..\RenderWindow.h
# End Source File
# Begin Source File

SOURCE=..\resource.h
# End Source File
# Begin Source File

SOURCE=..\ResourceManager.h
# End Source File
# Begin Source File

SOURCE=..\Root.h
# End Source File
# Begin Source File

SOURCE=..\RTFMap.h
# End Source File
# Begin Source File

SOURCE=..\FileIO\S3D_Def.h
# End Source File
# Begin Source File

SOURCE=..\FileIO\SaveN3D.h
# End Source File
# Begin Source File

SOURCE=..\FileIO\SaveV3D.h
# End Source File
# Begin Source File

SOURCE=..\Animation\Skeleton.h
# End Source File
# Begin Source File

SOURCE=..\software_noisemaker.h
# End Source File
# Begin Source File

SOURCE=..\surface.h
# End Source File
# Begin Source File

SOURCE=..\SwapChain.h
# End Source File
# Begin Source File

SOURCE=..\TextureManager.h
# End Source File
# Begin Source File

SOURCE=..\tools.h
# End Source File
# Begin Source File

SOURCE=..\FileIO\V3D_Def.h
# End Source File
# Begin Source File

SOURCE=..\vgsConstDef.h
# End Source File
# Begin Source File

SOURCE=..\VGSCore.h
# End Source File
# Begin Source File

SOURCE=..\VGSDef.h
# End Source File
# Begin Source File

SOURCE=..\VGSErrCode.h
# End Source File
# Begin Source File

SOURCE=..\Event\VGSEvent.h
# End Source File
# Begin Source File

SOURCE=..\Event\VGSFrameListener.h
# End Source File
# Begin Source File

SOURCE=..\VGSHeader.h
# End Source File
# Begin Source File

SOURCE=..\Event\VGSMainFrameListener.h
# End Source File
# Begin Source File

SOURCE=..\VGSRender.h
# End Source File
# Begin Source File

SOURCE=..\VGSSingelton.h
# End Source File
# Begin Source File

SOURCE=..\VGSString.h
# End Source File
# Begin Source File

SOURCE=..\VGSSupport.h
# End Source File
# Begin Source File

SOURCE=..\Viewport.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
