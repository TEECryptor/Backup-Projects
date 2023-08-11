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
	VOID RednerOneFrame();   // ��Ⱦһ֡ͼ��
	
	CRenderWindow* CreateRenderWindow(HWND hExternalWnd, CHAR* Name = NULL);  // ������Ⱦ����
	VOID RemoveRenderWindow(CRenderWindow* pRenderWnd);	          // ����һ��renderwindow
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


	VOID addFrameListener(CFrameListener* pListener);	    // ���һ��֡������
	VOID removeFrameListener(CFrameListener* pListener);	// ɾ��һ��֡������

	
	// ��Ϣ����
	void ProcessMessage(DWORD sceneHandle, DWORD hRenderWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	// ���úͻ�ȡ��ꡢ���̵Ŀ���״̬
	VOID setMouseControlEnable(BOOL bEnable);
	BOOL getMouseControlEnable();
	VOID setKeyboardControlEnable(BOOL bEnable);
	BOOL getKeyboardControlEnable();

// 	GfxManager* getGfxMgrptr() { return m_ptrGfxMgr; }
private:

	CVGSCore*       m_pVGSCore; // vgs coreָ��
	CAnimationManager m_AnimMgr;       // ����������
	CMaterialManager  m_MaterialMgr;
	CTextureManager   m_TextureMgr;

	vector <CRenderWindow*> m_vRenderWnd;       // ��Ⱦ����
	vector <CFrameListener*> m_vFrameListener;  // ֡������

	DWORD  m_LastFrameTime;              // ��ʼ��ʱ��

	FrameEvent m_Event;   // ֡ʱ����Ϣ

	BOOL m_bKeyboardControlEnable;  // �Ƿ���Ӧ�����¼�
	BOOL m_bMouseControlEnable;     // �Ƿ���Ӧ����¼�

	bool m_bReleased;  // �Ƿ��Ѿ�release

// 	GfxManager*	m_ptrGfxMgr;

};

