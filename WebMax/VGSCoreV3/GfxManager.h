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

/**GfxEngine类说明：
	1.对Scaleform的GFx系统进行封装，以对外提供统一、便捷的接口；

	create date:2010-5-31
	update date: 2010-7-8
	author:Webmax Team. 林秀全
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

	/** 初始化.
	*/
	bool initialise(void);
	void setScene(CScene* ptr) { m_pScene = ptr; }
	void OnResetDevice();
	void OnLostDevice();

// 	void advanceAndRender(GfxMovieViewPtr movie, float deltaTime);
	void render(float deltaTime);
	void stopRender() {m_bRenderMovieViews = false;}
	void startRender() {m_bRenderMovieViews = true;}
	/** 根据SWF/gfx文件创建一个对应的对象.
	@param szFileName:SWF文件名（不带路径，由setMediaDir()设置所在目录）.并该文件名作为字典的查找关键字.
	@param name:movie名称，如果为空将使用文件名作为名称.
	@return: 
	*/
	GfxMoviePtr createMovie(const std::string& szFileName, const std::string& name = "");
	/** 通过名称查找.
	*/
	GfxMoviePtr getMovie(const std::string& name);

	GfxMovieViewPtr getViewTest(GfxMoviePtr movie);
	/**通过Movie名称创建一个View.
	*/
	GfxMovieViewPtr createMovieView(GfxMoviePtr movie, const std::string& name);
	void destroyMovieView(const std::string& name);
	void destroyAllMovieViews();
	GfxMovieViewPtr getMovieView(const std::string& name);


	/** 消息处理主函数.
	*/
	void MessageWndProc(UINT message, WPARAM wParam, LPARAM lParam);

	//下面均为测试
	LPDIRECT3DTEXTURE9 GetRenderTexture() const { return m_pRenderTexture; }
	void CreateRenderTexture(int bufferWidth, int bufferHeight, bool stencil);
	GfxMovieViewPtr createRTTMovieView(GfxMoviePtr movie);
	void changeRTTMovieView(GfxMovieViewPtr view);
	void renderTexture(float delta);
	void setDeviceTexture(int nIndex = 0);
	bool convertFromScreenToRTT(GPtr<GFxMovieView> pMovieView,int& x, int& y);
	//true:播放，flase停止
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
	/**	用于集成Lua与Flash脚本的功能实现.
	*/
	void fsCommand(const char* commands, const char* args);

protected:
	/** 删除一个Movie
	*/
	void destroyMovie(const std::string& filename);
	/**据给定的Movie创建一个它的View.
	*/
	GfxMovieViewPtr createMovieView(GfxMoviePtr movie);
	/**获取MovieView所属的Movie
	*/
	GfxMoviePtr getMovieOfView(GfxMovieViewPtr view);
protected:
	void OnChar(UInt32 wcharCode, UInt info);
	void OnKey(KeyCode keyCode, unsigned char asciiCode, unsigned int wcharCode, 
		unsigned int mods, bool downFlag) ;
	void KeyEvent(GFxKey::Code keyCode, unsigned char asciiCode, unsigned int wcharCode, unsigned int mods, bool down);
	/** 处理Swf的鼠标输入事件.
	*/
	void OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
	

protected:
	//D3D设备及渲染窗口相关
	CVGSRender*	m_pDevice;
	CScene*	m_pScene;

	MySysAlloc myAlloc;
	//每个应用程序包含一个 GFxSystem 对象 
	GFxSystem* gfxInit;//
// 	GFxSystem gfxInit;//
	// One GFxLoader per application
	GFxLoader*           m_gfxLoader;

	// D3D9 Renderer
	GPtr<GRendererD3D9>		m_pRenderer;
	GPtr<GFxRenderConfig>	m_pRenderConfig;
	GPtr<GFxRenderStats>	m_pRenderStats;

	HWND	m_hWnd;
	/**渲染窗口的宽高
	*/
	int		m_nWndWidth, m_nWndHeight;

	//保存所有基于SWF文件创建的Gfx对象，一个文件只对应一个该对象.
	MovieMap	m_movies;
	//对每个Movie保存一个它的View列表.
	MovieViewMap	m_viewsOfMovie;
	//按名称保存View.
	MovieViews	m_movieViews;

	LPDIRECT3DTEXTURE9	m_pRenderTexture;
	LPDIRECT3DSURFACE9  m_pStencilSurface;
	//当前仅用于测试，以后需要加入到Views中统一管理.	林秀全.
	GfxMovieViewPtr	m_pRTTMovieView;
	int	m_nRTTwidth, m_nRTTheight;
	D3DXMATRIX	m_matRTTProj, m_matRTTProjInv;
	D3DXMATRIX	m_matWorld;
	D3DXMATRIX	m_matView, m_matViewInv;
	GFxMovieInfo m_rttMovieInfo;

	//是否渲染Flash，不包括RTT
	bool m_bRenderMovieViews;
	//RTT是否暂停
	bool m_bRTTViewPaused;
	bool m_bMeshWireframe;

	//保存Flash的媒体目录.
	std::string	m_strMediaDir;
protected://中间控制变量

};
#endif
