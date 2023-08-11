# Microsoft Developer Studio Project File - Name="VGSCoreLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=VGSCoreLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VGSCoreLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VGSCoreLib.mak" CFG="VGSCoreLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VGSCoreLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "VGSCoreLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VGSCoreLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_VGS_LIB" /D "_VGS_CORE" /FR /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\VGS_ATL\VGSCore\VGSCoreLib.lib"

!ELSEIF  "$(CFG)" == "VGSCoreLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_VGS_LIB" /D "_VGS_CORE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\VGS_ATL\VGSCore\VGSCoreLibD.lib"

!ENDIF 

# Begin Target

# Name "VGSCoreLib - Win32 Release"
# Name "VGSCoreLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Animation\AnimationManager.cpp
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

SOURCE=..\CSectionMap.cpp
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

SOURCE=..\FileIO\N3DLoader.cpp
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

SOURCE=..\SceneResource.cpp
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

SOURCE=..\BumpMap.h
# End Source File
# Begin Source File

SOURCE=..\Animation\CameraKeyFrame.h
# End Source File
# Begin Source File

SOURCE=..\CAxis.h
# End Source File
# Begin Source File

SOURCE=..\CBumpWater.h
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

SOURCE=..\CScene.h
# End Source File
# Begin Source File

SOURCE=..\CSectionMap.h
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

SOURCE=..\FileIO\N3DLoader.h
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

SOURCE=..\SceneResource.h
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

SOURCE=..\VGSHeader.h
# End Source File
# Begin Source File

SOURCE=..\VGSRender.h
# End Source File
# Begin Source File

SOURCE=..\VGSSingelton.h
# End Source File
# Begin Source File

SOURCE=..\VGSSupport.h
# End Source File
# Begin Source File

SOURCE=..\Viewport.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
