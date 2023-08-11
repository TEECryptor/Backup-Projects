
#pragma once


#include <windows.h>
#include <WINDEF.h>


#include <d3d9.h>
#include <d3dx9.h>
#include <d3d9types.h>
#include <d3dx9math.h>
#include <d3dx9tex.h>

#include <vector>

#include "VGSDef.h"
#include "vgsConstDef.h"
#include "CManyFVF.h"
//#include "VGSSingelton.h"
#include "VGSString.h"

#pragma   warning(disable:4786)  // ÔÝÊ±ÆÁ±Îµôwarning
#pragma   warning(disable:4251)  // ÔÝÊ±ÆÁ±Îµôwarning

class CVGSString;
//template <typename T> class Singleton;
class CVGSCore;
class CRoot;
class CVGSGetRoot;
class CMovableObject;
class CSceneNode;
class CAudioPlayer;

class CBumpMap;
class CBumpWater;
class CButton;
class CCamera;
class CCubeMap;
class CD3DDevice;
class CDynamicTex;
class CEditor;
class CFace;
class CFlyer;
class CFont;
class CFSGlow;
class CFullScreenMotionBlur;
class CGetE3DFileInfo;
class CGUIManager;
class CGUIObject;
class CImage;
class CInput;
class CLensFlare;
class CLight;
class CListElement;
class CMaterial;
class CSubModel;
class CModel;
class CModelGroup;
class CMovieTexture;
class CMovieTexureRender;
class CNavigator;
class COverlay;
class CRenderBoundBox;

class CScene;
class CSphere;
class CSwapChain;
class CText;
class CTexture;
class CTextureLayer;
class CTransform;
class CVertex;
class CVGSRender;
class CViewport;
class CWalker;
class CVGSTestWindow;
class CViewport;
class CNode;
class CRenderWindow;
class CFrameListener;
class CMainFrameListener;
class CNodeKeyFrame;
class CNodeAnimation;
class CAnimationManager;
class IController;
class CWalker;
class CEditor;
class CFlyer;
class CResource;
class CResourceManager;
class CMaterialManager;
class CTextureManager;
class CSurface;
class CRealWater;
class CN3DLoader;
class CN3DLoaderImpBase;
class CN3DLoaderImpV2;
class CN3DLoaderImpV3;
class CRTFMap;
class SwfTexture;
class CATLFlashCtrl;
class CGetV3DBase;
class CGetV3DImpV2;
class Axis;
class AxisMove;
class AxisScale;
class AxisRotate;
class CGetV3DImpV3;

// lua
class LuaEngine;