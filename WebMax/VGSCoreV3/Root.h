// Root.h: interface for the CRoot class.
//
//////////////////////////////////////////////////////////////////////

#pragma  once

#include "VGSHeader.h"
#include "RenderWindow.h"
#include <vector>
// #include "VGSSingelton.h"
// #include "..\\VGSCoreV3\VGSCore.h"

#include ".\\Event\VGSFrameListener.h"

#include "Animation/AnimationManager.h"
#include "MaterialManager.h"
#include "TextureManager.h"

using namespace std;

class VGS_EXPORT CRoot //: public Singleton<CRoot>
{
public:
	VOID RednerOneFrame();   // 渲染一帧图像
	
	CRenderWindow* CreateRenderWindow(HWND hExternalWnd, CHAR* Name = NULL);  // 创建渲染窗口
	VOID RemoveRenderWindow(CRenderWindow* pRenderWnd);	          // 销毁一个renderwindow
	VOID RemoveAllRenderWindow();

	UINT GetRenderWindowCount();
	CRenderWindow* GetRenderWindow(int index);
	CVGSRender& getVGSRender();

public:
	CRoot();
	virtual ~CRoot();

	VOID        SetVGSCore(CVGSCore* pCore);
	CVGSCore*   GetVGSCore(){return m_pVGSCore;}
	//static CRoot* getSingletonPtr();
	//static CRoot& getSingleton();

	CAnimationManager& GetAnimManager(){return m_AnimMgr;}
	CMaterialManager&  GetMaterialManager(){return m_MaterialMgr;}
	CTextureManager& GetTextureManager(){return m_TextureMgr;}
	
	VOID Release();   // release


	VOID addFrameListener(CFrameListener* pListener);	    // 添加一个帧监听器
	VOID removeFrameListener(CFrameListener* pListener);	// 删除一个帧监听器

	
	// 消息处理
	void ProcessMessage(DWORD sceneHandle, DWORD hRenderWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	// 设置和获取鼠标、键盘的控制状态
	VOID setMouseControlEnable(BOOL bEnable);
	BOOL getMouseControlEnable();
	VOID setKeyboardControlEnable(BOOL bEnable);
	BOOL getKeyboardControlEnable();

// 	GfxManager* getGfxMgrptr() { return m_ptrGfxMgr; }
private:

	CVGSCore*       m_pVGSCore; // vgs core指针
	CAnimationManager m_AnimMgr;       // 动画管理器
	CMaterialManager  m_MaterialMgr;
	CTextureManager   m_TextureMgr;

	vector <CRenderWindow*> m_vRenderWnd;       // 渲染窗口
	vector <CFrameListener*> m_vFrameListener;  // 帧监听器

	DWORD  m_LastFrameTime;              // 开始的时间

	FrameEvent m_Event;   // 帧时间信息

	BOOL m_bKeyboardControlEnable;  // 是否响应键盘事件
	BOOL m_bMouseControlEnable;     // 是否响应鼠标事件

	bool m_bReleased;  // 是否已经release

// 	GfxManager*	m_ptrGfxMgr;

};

