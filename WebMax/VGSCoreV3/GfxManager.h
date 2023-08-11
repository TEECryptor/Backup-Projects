#ifndef VGS_GFX_ENGINE_H
#define VGS_GFX_ENGINE_H

// #include "GTimer.h"
#include "GFxEvent.h"
#include "GFxPlayer.h"
#include "GFxFontLib.h"
#include "GRendererD3D9.h"

#include "VGSHeader.h"
#include <map>
#include <set>

#include "VGSGetRoot.h"

typedef GPtr<GFxMovieView>	GfxMovieViewPtr;
typedef GPtr<GFxMovieDef>	GfxMoviePtr;

class MySysAlloc : public GSysAlloc
{
public:
	virtual void  GetInfo(Info* i) const
	{
		i->MinAlign     = 1;
		i->MaxAlign     = 1;
		i->Granularity  = 128*1024;
		i->HasRealloc   = false;
	}

	virtual void* Alloc(UPInt size, UPInt align)
	{
		return malloc(size);
	}

	virtual bool  Free(void* ptr, UPInt size, UPInt align)
	{
		free(ptr);
		return true;
	}
};

/**GfxEngine��˵����
	1.��Scaleform��GFxϵͳ���з�װ���Զ����ṩͳһ����ݵĽӿڣ�

	create date:2010-5-31
	update date: 2010-7-8
	author:Webmax Team. ����ȫ
*/
class GfxManager:public CVGSGetRoot
{
	typedef std::map<std::string, GfxMoviePtr > MovieMap;
	typedef std::map<std::string, GfxMovieViewPtr> MovieViews;

	typedef std::set<GfxMovieViewPtr> MovieViewSet;
	typedef std::map<GfxMoviePtr, MovieViewSet> MovieViewMap;
public:

	// *** Input Constants and 'On' Functions
	// These key codes match Flash-defined and GFxKey::Code values exactly.
	// we have these codes here because we can not (MAC reason) have a dependency
	// on any GFx header files
	enum KeyCode
	{
		VoidSymbol      = 0,

		// A through Z and numbers 0 through 9.
		A               = 65,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Num0            = 48,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,

		// Numeric keypad.
		KP_0            = 96,
		KP_1,
		KP_2,
		KP_3,
		KP_4,
		KP_5,
		KP_6,
		KP_7,
		KP_8,
		KP_9,
		KP_Multiply,
		KP_Add,
		KP_Enter,
		KP_Subtract,
		KP_Decimal,
		KP_Divide,

		// Function keys.
		F1              = 112,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,

		// Other keys.
		Backspace       = 8,
		Tab,
		ClearKey       = 12,
		Return,
		Shift           = 16,
		Control,
		Alt,
		Pause,
		CapsLock        = 20, // Toggle
		Escape          = 27,
		Space           = 32,
		PageUp,
		PageDown,
		End             = 35,
		Home,
		Left,
		Up,
		Right,
		Down,
		Insert          = 45,
		Delete,
		Help,
		NumLock         = 144, // Toggle
		ScrollLock      = 145, // Toggle

		Semicolon       = 186,
		Equal           = 187,
		Comma           = 188, // Platform specific?
		Minus           = 189,
		Period          = 190, // Platform specific?
		Slash           = 191,
		Bar             = 192,
		BracketLeft     = 219,
		Backslash       = 220,
		BracketRight    = 221,
		Quote           = 222,			
		ProcessKey      = 229,

		OEM_AX			= 0xE1,  //  'AX' key on Japanese AX kbd
		OEM_102			= 0xE2,  //  "<>" or "\|" on RT 102-key kbd.
		ICO_HELP		= 0xE3,  //  Help key on ICO
		ICO_00			= 0xE4,  //  00 key on ICO

		// Total number of keys.
		KeyCount
	};

	enum KeyModifiers
	{
		KM_Control = 0x1,
		KM_Alt     = 0x2,
		KM_Shift   = 0x4,
		KM_Num     = 0x8,
		KM_Caps    = 0x10,
		KM_Scroll  = 0x20
	};

public:
	GfxManager();
	~GfxManager();

	void setVGSRender(CVGSRender* render) {m_pDevice = render;}

	/** ��ʼ��.
	*/
	bool initialise(void);
	void setScene(CScene* ptr) { m_pScene = ptr; }
	void OnResetDevice();
	void OnLostDevice();

// 	void advanceAndRender(GfxMovieViewPtr movie, float deltaTime);
	void render(float deltaTime);
	void stopRender() {m_bRenderMovieViews = false;}
	void startRender() {m_bRenderMovieViews = true;}
	/** ����SWF/gfx�ļ�����һ����Ӧ�Ķ���.
	@param szFileName:SWF�ļ���������·������setMediaDir()��������Ŀ¼��.�����ļ�����Ϊ�ֵ�Ĳ��ҹؼ���.
	@param name:movie���ƣ����Ϊ�ս�ʹ���ļ�����Ϊ����.
	@return: 
	*/
	GfxMoviePtr createMovie(const std::string& szFileName, const std::string& name = "");
	/** ͨ�����Ʋ���.
	*/
	GfxMoviePtr getMovie(const std::string& name);

	GfxMovieViewPtr getViewTest(GfxMoviePtr movie);
	/**ͨ��Movie���ƴ���һ��View.
	*/
	GfxMovieViewPtr createMovieView(GfxMoviePtr movie, const std::string& name);
	void destroyMovieView(const std::string& name);
	void destroyAllMovieViews();
	GfxMovieViewPtr getMovieView(const std::string& name);


	/** ��Ϣ����������.
	*/
	void MessageWndProc(UINT message, WPARAM wParam, LPARAM lParam);

	//�����Ϊ����
	LPDIRECT3DTEXTURE9 GetRenderTexture() const { return m_pRenderTexture; }
	void CreateRenderTexture(int bufferWidth, int bufferHeight, bool stencil);
	GfxMovieViewPtr createRTTMovieView(GfxMoviePtr movie);
	void changeRTTMovieView(GfxMovieViewPtr view);
	void renderTexture(float delta);
	void setDeviceTexture(int nIndex = 0);
	bool convertFromScreenToRTT(GPtr<GFxMovieView> pMovieView,int& x, int& y);
	//true:���ţ�flaseֹͣ
	void setRTTMovieViewPlayState(bool bPlaying);
	//true:wireframe, false:solid
	void setRenderWireframe(bool bWireframe)
	{
		m_bMeshWireframe = bWireframe;
	}
	bool isRenderWireframe() const {return m_bMeshWireframe;}

	void setMediaDir(const std::string& dir)
	{
		m_strMediaDir = dir;
	}
	const std::string& getMediaDir() const 
	{
		return m_strMediaDir;
	}
	/**	���ڼ���Lua��Flash�ű��Ĺ���ʵ��.
	*/
	void fsCommand(const char* commands, const char* args);

protected:
	/** ɾ��һ��Movie
	*/
	void destroyMovie(const std::string& filename);
	/**�ݸ�����Movie����һ������View.
	*/
	GfxMovieViewPtr createMovieView(GfxMoviePtr movie);
	/**��ȡMovieView������Movie
	*/
	GfxMoviePtr getMovieOfView(GfxMovieViewPtr view);
protected:
	void OnChar(UInt32 wcharCode, UInt info);
	void OnKey(KeyCode keyCode, unsigned char asciiCode, unsigned int wcharCode, 
		unsigned int mods, bool downFlag) ;
	void KeyEvent(GFxKey::Code keyCode, unsigned char asciiCode, unsigned int wcharCode, unsigned int mods, bool down);
	/** ����Swf����������¼�.
	*/
	void OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
	

protected:
	//D3D�豸����Ⱦ�������
	CVGSRender*	m_pDevice;
	CScene*	m_pScene;

	MySysAlloc myAlloc;
	//ÿ��Ӧ�ó������һ�� GFxSystem ���� 
	GFxSystem* gfxInit;//
// 	GFxSystem gfxInit;//
	// One GFxLoader per application
	GFxLoader*           m_gfxLoader;

	// D3D9 Renderer
	GPtr<GRendererD3D9>		m_pRenderer;
	GPtr<GFxRenderConfig>	m_pRenderConfig;
	GPtr<GFxRenderStats>	m_pRenderStats;

	HWND	m_hWnd;
	/**��Ⱦ���ڵĿ��
	*/
	int		m_nWndWidth, m_nWndHeight;

	//�������л���SWF�ļ�������Gfx����һ���ļ�ֻ��Ӧһ���ö���.
	MovieMap	m_movies;
	//��ÿ��Movie����һ������View�б�.
	MovieViewMap	m_viewsOfMovie;
	//�����Ʊ���View.
	MovieViews	m_movieViews;

	LPDIRECT3DTEXTURE9	m_pRenderTexture;
	LPDIRECT3DSURFACE9  m_pStencilSurface;
	//��ǰ�����ڲ��ԣ��Ժ���Ҫ���뵽Views��ͳһ����.	����ȫ.
	GfxMovieViewPtr	m_pRTTMovieView;
	int	m_nRTTwidth, m_nRTTheight;
	D3DXMATRIX	m_matRTTProj, m_matRTTProjInv;
	D3DXMATRIX	m_matWorld;
	D3DXMATRIX	m_matView, m_matViewInv;
	GFxMovieInfo m_rttMovieInfo;

	//�Ƿ���ȾFlash��������RTT
	bool m_bRenderMovieViews;
	//RTT�Ƿ���ͣ
	bool m_bRTTViewPaused;
	bool m_bMeshWireframe;

	//����Flash��ý��Ŀ¼.
	std::string	m_strMediaDir;
protected://�м���Ʊ���

};
#endif
