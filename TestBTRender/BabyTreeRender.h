//-----------------------------------------
//	FileName:BabyTreeRender.h
//	Introduce:To render 2D graphic useing HGE engine
//	Creator:Youfang Yao
//	Date:11/08/07
//-----------------------------------------
#ifndef	_BABYTREE_RENDER_H_
#define _BABYTREE_RENDER_H_
//-----------------------------------------
#include <windows.h>
//-----------------------------------------
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BABYTREERENDER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BABYTREERENDER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//-----------------------------------------
#define BABYTREERENDER_EXPORTS

#ifdef BABYTREERENDER_EXPORTS
#define BABYTREERENDER_API __declspec(dllexport)
#else
#define BABYTREERENDER_API __declspec(dllimport)
#endif
//-----------------------------------------

//-----------------------------------------
#define	MAX_NAME_LEN	64		
#define DEGREE_TO_RAD	0.017453292f
//-----------------------------------------
/*
** BabyTree Render System handle
*/
typedef	DWORD BTHANDLE;
//-----------------------------------------

//-----------------------------------------
/*
** BabyTree Render Virtual-key codes
*/
//-----------------------------------------
#define BTKEY_LBUTTON		0x01
#define BTKEY_RBUTTON		0x02
#define BTKEY_MBUTTON		0x04

#define BTKEY_ESCAPE		0x1B
#define BTKEY_BACKSPACE		0x08
#define BTKEY_TAB			0x09
#define BTKEY_ENTER			0x0D
#define BTKEY_SPACE			0x20

#define BTKEY_SHIFT			0x10
#define BTKEY_CTRL			0x11
#define BTKEY_ALT			0x12

#define BTKEY_LWIN			0x5B
#define BTKEY_RWIN			0x5C
#define BTKEY_APPS			0x5D

#define BTKEY_PAUSE			0x13
#define BTKEY_CAPSLOCK		0x14
#define BTKEY_NUMLOCK		0x90
#define BTKEY_SCROLLLOCK	0x91

#define BTKEY_PGUP			0x21
#define BTKEY_PGDN			0x22
#define BTKEY_HOME			0x24
#define BTKEY_END			0x23
#define BTKEY_INSERT		0x2D
#define BTKEY_DELETE		0x2E

#define BTKEY_LEFT			0x25
#define BTKEY_UP			0x26
#define BTKEY_RIGHT			0x27
#define BTKEY_DOWN			0x28

#define BTKEY_0				0x30
#define BTKEY_1				0x31
#define BTKEY_2				0x32
#define BTKEY_3				0x33
#define BTKEY_4				0x34
#define BTKEY_5				0x35
#define BTKEY_6				0x36
#define BTKEY_7				0x37
#define BTKEY_8				0x38
#define BTKEY_9				0x39

#define BTKEY_A				0x41
#define BTKEY_B				0x42
#define BTKEY_C				0x43
#define BTKEY_D				0x44
#define BTKEY_E				0x45
#define BTKEY_F				0x46
#define BTKEY_G				0x47
#define BTKEY_H				0x48
#define BTKEY_I				0x49
#define BTKEY_J				0x4A
#define BTKEY_K				0x4B
#define BTKEY_L				0x4C
#define BTKEY_M				0x4D
#define BTKEY_N				0x4E
#define BTKEY_O				0x4F
#define BTKEY_P				0x50
#define BTKEY_Q				0x51
#define BTKEY_R				0x52
#define BTKEY_S				0x53
#define BTKEY_T				0x54
#define BTKEY_U				0x55
#define BTKEY_V				0x56
#define BTKEY_W				0x57
#define BTKEY_X				0x58
#define BTKEY_Y				0x59
#define BTKEY_Z				0x5A

#define BTKEY_GRAVE			0xC0
#define BTKEY_MINUS			0xBD
#define BTKEY_EQUALS		0xBB
#define BTKEY_BACKSLASH		0xDC
#define BTKEY_LBRACKET		0xDB
#define BTKEY_RBRACKET		0xDD
#define BTKEY_SEMICOLON		0xBA
#define BTKEY_APOSTROPHE	0xDE
#define BTKEY_COMMA			0xBC
#define BTKEY_PERIOD		0xBE
#define BTKEY_SLASH			0xBF

#define BTKEY_NUMPAD0		0x60
#define BTKEY_NUMPAD1		0x61
#define BTKEY_NUMPAD2		0x62
#define BTKEY_NUMPAD3		0x63
#define BTKEY_NUMPAD4		0x64
#define BTKEY_NUMPAD5		0x65
#define BTKEY_NUMPAD6		0x66
#define BTKEY_NUMPAD7		0x67
#define BTKEY_NUMPAD8		0x68
#define BTKEY_NUMPAD9		0x69

#define BTKEY_MULTIPLY		0x6A
#define BTKEY_DIVIDE		0x6F
#define BTKEY_ADD			0x6B
#define BTKEY_SUBTRACT		0x6D
#define BTKEY_DECIMAL		0x6E

#define BTKEY_F1			0x70
#define BTKEY_F2			0x71
#define BTKEY_F3			0x72
#define BTKEY_F4			0x73
#define BTKEY_F5			0x74
#define BTKEY_F6			0x75
#define BTKEY_F7			0x76
#define BTKEY_F8			0x77
#define BTKEY_F9			0x78
#define BTKEY_F10			0x79
#define BTKEY_F11			0x7A
#define BTKEY_F12			0x7B
//-----------------------------------------


//-----------------------------------------
/*
** BabyTree Render System state constants
*/
//-----------------------------------------
enum BTBoolState
{
	BT_WINDOWED			= 12,   // bool		run in window?		(default: false)
	BT_ZBUFFER			= 13,   // bool		use z-buffer?		(default: false)
	BT_TEXTUREFILTER	= 28,   // bool		texture filtering?	(default: true)
	
	BT_USESOUND			= 18,   // bool		use BASS for sound?	(default: true)
	
	BT_DONTSUSPEND		= 24,	// bool		focus lost:suspend?	(default: false)
	BT_HIDEMOUSE		= 25,	// bool		hide system cursor?	(default: true)

	BT_SHOWSPLASH		= 27,	// bool		hide system cursor?	(default: true)

	BTBOOLSTATE_FORCE_DWORD = 0x7FFFFFFF
};
//-----------------------------------------
enum BTFuncState
{
	BT_FRAMEFUNC		= 1,    // bool*()	frame function		(default: NULL) (you MUST set this)
	BT_RENDERFUNC		= 2,    // bool*()	render function		(default: NULL)
	BT_FOCUSLOSTFUNC	= 3,    // bool*()	focus lost function	(default: NULL)
	BT_FOCUSGAINFUNC	= 4,    // bool*()	focus gain function	(default: NULL)
	BT_GFXRESTOREFUNC	= 5,    // bool*()	exit function		(default: NULL)
	BT_EXITFUNC			= 6,    // bool*()	exit function		(default: NULL)
	
	BTFUNCSTATE_FORCE_DWORD = 0x7FFFFFFF
};
//-----------------------------------------
enum BTHwndState
{
	BT_HWND				= 26,	// int		window handle: read only
	BT_HWNDPARENT		= 27,	// int		parent win handle	(default: 0)
	
	BTHWNDSTATE_FORCE_DWORD = 0x7FFFFFFF
};
//-----------------------------------------
enum BTIntState
{
	BT_SCREENWIDTH		= 9,    // int		screen width		(default: 800)
	BT_SCREENHEIGHT		= 10,   // int		screen height		(default: 600)
	BT_SCREENBPP		= 11,   // int		screen bitdepth		(default: 32) (desktop bpp in windowed mode)
	
	BT_SAMPLERATE		= 19,   // int		sample rate			(default: 44100)
	BT_FXVOLUME			= 20,   // int		global fx volume	(default: 100)
	BT_MUSVOLUME		= 21,   // int		global music volume	(default: 100)
	
	BT_FPS				= 23,	// int		fixed fps			(default: HGEFPS_UNLIMITED)
	
	BTINTSTATE_FORCE_DWORD = 0x7FFFFFF
};
//-----------------------------------------
enum BTStringState
{
	BT_ICON				= 7,    // char*	icon resource		(default: NULL)
	BT_TITLE			= 8,    // char*	window title		(default: "HGE")
	
	BT_INIFILE			= 15,   // char*	ini file			(default: NULL) (meaning no file)
	BT_LOGFILE			= 16,   // char*	log file			(default: NULL) (meaning no file)

	BTSTRINGSTATE_FORCE_DWORD = 0x7FFFFFFF
};
//-----------------------------------------


//-----------------------------------------
/*
** BabyTree Render Input Event type constants
*/
//-----------------------------------------
#define BTINP_KEYDOWN		1
#define BTINP_KEYUP			2
#define BTINP_MBUTTONDOWN	3
#define BTINP_MBUTTONUP		4
#define BTINP_MOUSEMOVE		5
#define BTINP_MOUSEWHEEL	6
//-----------------------------------------
/*
** BabyTree Render Input Event flags
*/
//-----------------------------------------
#define BTINP_SHIFT			1
#define BTINP_CTRL			2
#define BTINP_ALT			4
#define BTINP_CAPSLOCK		8
#define BTINP_SCROLLLOCK	16
#define BTINP_NUMLOCK		32
#define BTINP_REPEAT		64
//-----------------------------------------
/*
** BabyTree Render Input Event structure
*/
//-----------------------------------------
struct BTInputEvent
{
	int		type;			// event type
	int		key;			// key code
	int		flags;			// event flags
	int		chr;			// character code
	int		wheel;			// wheel shift
	float	x;				// mouse cursor x-coordinate
	float	y;				// mouse cursor y-coordinate
};
//-----------------------------------------

//-----------------------------------------
/*
** BabyTree Render RECT define
*/
//-----------------------------------------
struct BTRect
{
	float fLeft;
	float fTop;
	float fWidth;
	float fHeight;
};
//-----------------------------------------
//	Name:eBTObjState
//	Introduce:BabyTree object state definition
//-----------------------------------------
enum eBTObjState
{
	BTOS_NORMAL		= 0,		// normal state: mouse is not in it and is not selected
	BTOS_MOUSEOVER	= 1,		// mouse is in this object
	BTOS_SELECTED	= 2,		// is selected by mouse click
	BTOS_COUNT
};
//-----------------------------------------
//	Name:eBTObjEventCode
//	Introduce:BabyTree object event code definition
//-----------------------------------------
enum eBTObjEventCode
{
	BTOE_STATE_CHANGED	= 0,	// object state has been changed
	BTOE_ANIMATE_START	= 1,	// a animate object is playing
	BTOE_ANIMATE_END	= 2,	// a animate object is end, only for NOT loop playing
	BTOE_COUNT
};
//-----------------------------------------
//	Name:eBTObjEventCode
//	Introduce:BabyTree object event code definition
//-----------------------------------------
enum eBTObjAnimateType
{
	BTOAT_NONE		= 0x0,		// object has not any animate
	BTOAT_TEXCHANGE	= 0x1,		// object has texture change animate
	BTOAT_TEXMOVE	= 0x2,		// object has texture moving animate
	BTOAT_OBJMOVE	= 0x4,		// object has animate for rect moving
	BTOAT_OBJROTATE	= 0x8,		// object has animate for rect rotating
	BTOAT_COUNT		= 5
};
//-----------------------------------------

//-----------------------------------------
/*
** Callback protoype used by BabyTree Render
*/
//-----------------------------------------
typedef bool (*BTRenderCallback)();
typedef void (*BTObjectCallback)(LPCTSTR lpszObjName, DWORD dwCode, DWORD dwParam);
//-----------------------------------------

//-----------------------------------------
//	Name:CBTRender
//	Introduce:The render for BabyTree 2D graphic
//-----------------------------------------
class CBTRender
{
public:
	CBTRender(){};
	virtual ~CBTRender(){};
	//-------------------------------------
public:
	virtual BOOL Create() = 0;
	virtual void SetState(BTBoolState eState, BOOL bValue) = 0;
	virtual void SetState(BTFuncState eState, BTRenderCallback pValue) = 0;
	virtual void SetState(BTHwndState eState, HWND hValue) = 0;
	virtual void SetState(BTIntState eState, int iValue) = 0;
	virtual void SetState(BTStringState eState, LPCTSTR lpszValue) = 0;
	virtual void SetWindowRect(int iLeft, int iTop, int iWidth, int iHeight) = 0;
	virtual BOOL Initiate() = 0;
	//-------------------------------------
	virtual DWORD GetRenderHandle() = 0;
	virtual float GetTimeDelta() = 0;
	virtual void GetInputEvent(BTInputEvent *event) = 0;
	//-------------------------------------
	virtual BOOL RenderStart() = 0;
	virtual BOOL BeginScene() = 0;
	virtual void Clear(DWORD dwColor) = 0;
	virtual BOOL EndScene() = 0;
	//-------------------------------------
	virtual void Shutdown() = 0;
	virtual void Release() = 0;
	//-------------------------------------
};
//-----------------------------------------
//	Name:CBTTexture
//	Introduce:The texture object for BTRender
//-----------------------------------------
class CBTTexture
{
public:
	CBTTexture(){};
	virtual ~CBTTexture(){};
	//-------------------------------------
public:
	virtual void SetTexFilePath(LPCTSTR lpszTexPath) = 0;
	virtual BOOL LoadFromFile(LPCTSTR lpszName, DWORD dwRenderHandle, LPCTSTR lpszTexFile, BOOL bAnimate = FALSE, DWORD dwFrameCnt = 1) = 0;
	virtual BOOL IsAnimateTexture() = 0;
	virtual DWORD GetTotalFrameCnt() = 0;
	virtual void GetName(LPTSTR lpName) = 0;
	virtual BTHANDLE GetTexture(DWORD dwFrame = 0) = 0;
	virtual DWORD GetTexOrgWidth() = 0;
	virtual DWORD GetTexOrgHeight() = 0;
	virtual void SetTexRect(BTRect &texRect, DWORD dwFrame) = 0;
	virtual void GetTexRect(BTRect &texRect, DWORD dwFrame = 0) = 0;
	virtual void SetTimeLen(DWORD dwTimeLen, DWORD dwFrame) = 0;
	virtual DWORD GetTimeLen(DWORD dwFrame = 0) = 0;
	//-------------------------------------
};
//-----------------------------------------
//	Name:CBTObject
//	Introduce:The 2D rect object for BTRender
//-----------------------------------------
class CBTObject
{
public:
	CBTObject(){};
	virtual ~CBTObject(){};
	//-------------------------------------
public:
	virtual BOOL Create(LPCTSTR lpszName, float fLeft, float fTop, float fWidth, float fHeight) = 0;
	virtual void SetTexture(CBTTexture* pTexture, float fTexLeft = 0, float fTexTop = 0, float fTexWidth = 0, float fTexHeight = 0) = 0;
	virtual void SetTexMoveAnimate(BTRect &rcTexStart, BTRect &rcTexEnd, DWORD dwTime) = 0;
	virtual void SetObjMoveAnimate(BTRect &rcStart, BTRect &rcEnd, DWORD dwTime) = 0;
	virtual void SetObjRotateAnimate(float fStartDegree, float fEndDegree, DWORD dwTime) = 0;
	virtual void AddChildObject(CBTObject* pChildObj) = 0;
	virtual void DelChildObject(LPCTSTR lpszChild) = 0;
	virtual void SetOrgParentRect(BTRect rcOrgParent) = 0;
	virtual void SetCurParentRect(BTRect rcCurParent) = 0;
	virtual void UpdateTimeDelta(float fTimeDelta, BOOL bUpdateChild = FALSE) = 0;
	virtual void Play(DWORD dwType, DWORD dwSetLoop = BTOAT_NONE, DWORD dwSetCallback = BTOAT_NONE) = 0;
	virtual void Render() = 0;
	virtual void Render(float fX, float fY) = 0;
	//-------------------------------------
	virtual void SetCallbackFunc(BTObjectCallback pCallbackFunc) = 0;
	virtual void GetName(LPTSTR lpName) = 0;
	virtual void SetCurRect(float fLeft, float fTop, float fWidth, float fHeight) = 0;
	virtual void GetCurRect(BTRect &rcObj) = 0;
	virtual void SetVisible(BOOL bVisible = TRUE) = 0;
	virtual BOOL PntInObject(float fX, float fY) = 0;
	virtual void ChangeState(DWORD dwState) = 0;
	//-------------------------------------
	virtual void Release() = 0;
	//-------------------------------------
};
//-----------------------------------------


//-----------------------------------------
/*The export functions of the dll*/
//-----------------------------------------
//To create and return a CBTRender object pointer
BABYTREERENDER_API CBTRender* BTRender_CreateRender();
//To create and return a CBTTexture object pointer
BABYTREERENDER_API CBTTexture* BTRender_CreateTexture();
//To create and return a CBTObject object pointer
BABYTREERENDER_API CBTObject* BTRender_CreateObject();
//To destroy a CBTRender object pointer
BABYTREERENDER_API void	BTRender_DestroyRender(CBTRender* pBTRender);
//To destroy a CBTTexture object pointer
BABYTREERENDER_API void BTRender_DestroyTexture(CBTTexture* pBTTexture);
//To destroy a CBTObject object pointer
BABYTREERENDER_API void	BTRender_DestroyObject(CBTObject* pBTObject);
//-----------------------------------------
#endif	//_BABYTREE_RENDER_H_
//-----------------------------------------

