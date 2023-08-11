#include "GfxManager.h"

#include "Root.h"
#include "VGSCore.h"
#include "VGSRender.h"
#include "Node.h"
#include "Ray.h"

#include <stdlib.h>
#include <assert.h>

#include "CharCoder.h"

//从完整路径中获取带扩展名的文件名.	2010-6-2	林秀全	linxq@live.com
std::string getFileNameFromPath(const std::string& path)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	errno_t err;
	err = _splitpath_s(path.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, fname,
		_MAX_FNAME, ext, _MAX_EXT );
	if (err != 0)
		return "";
	std::string name(fname);
	name += ext;
	return name;
}


#define M(x,r,c) x[(r)*4+(c)]
#define Mt(x,c,r) x[(r)*4+(c)]
static void Translate(Float *P, Float x, Float y, Float z)
{
	for (int i = 0; i < 4; i++)
		M(P, 3,i) += x * M(P, 0,i) + y * M(P, 1,i) + z * M(P, 2,i);
}

static void MakePerspective(Float *P, Float fov, Float aspect, Float z0, Float z1)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M(P,i,j) = 0;

	Float dz = z1-z0;
	Float sinfov = sin(fov*0.5f);
	Float cotfov = cos(fov*0.5f) / sinfov;

	M(P,0,0) = cotfov/aspect;
	M(P,1,1) = cotfov;
	M(P,2,2) = -(z0 + z1) / dz;
	M(P,2,3) = -1;
	M(P,3,2) = -2 * z0 * z1 / dz;
}


static int GetModifiers()
{
	int new_mods = 0;
	if (::GetKeyState(VK_SHIFT) & 0x8000)
		new_mods |= GfxManager::KM_Shift;
	if (::GetKeyState(VK_CONTROL) & 0x8000)
		new_mods |= GfxManager::KM_Control;
	if (::GetKeyState(VK_MENU) & 0x8000)
		new_mods |= GfxManager::KM_Alt;
	if (::GetKeyState(VK_NUMLOCK) & 0x1)
		new_mods |= GfxManager::KM_Num;
	if (::GetKeyState(VK_CAPITAL) & 0x1)
		new_mods |= GfxManager::KM_Caps;
	if (::GetKeyState(VK_SCROLL) & 0x1)
		new_mods |= GfxManager::KM_Scroll;
	return new_mods;
}
static struct 
{
	WPARAM winKey;
	GfxManager::KeyCode appKey;
} KeyCodeMap[] = 
{
	{VK_BACK,    GfxManager::Backspace},
	{VK_TAB,     GfxManager::Tab},
	{VK_CLEAR,   GfxManager::ClearKey},
	{VK_RETURN,  GfxManager::Return},
	{VK_SHIFT,   GfxManager::Shift},
	{VK_CONTROL, GfxManager::Control},
	{VK_MENU,    GfxManager::Alt},
	{VK_PAUSE,   GfxManager::Pause},
	{VK_CAPITAL, GfxManager::CapsLock},
	{VK_ESCAPE,  GfxManager::Escape},
	{VK_SPACE,   GfxManager::Space},
	{VK_PRIOR,   GfxManager::PageUp},
	{VK_NEXT,    GfxManager::PageDown},
	{VK_END,     GfxManager::End},
	{VK_HOME,    GfxManager::Home},
	{VK_LEFT,    GfxManager::Left},
	{VK_UP,      GfxManager::Up},
	{VK_RIGHT,   GfxManager::Right},
	{VK_DOWN,    GfxManager::Down},
	{VK_INSERT,  GfxManager::Insert},
	{VK_DELETE,  GfxManager::Delete},
	{VK_HELP,    GfxManager::Help},
	{VK_NUMLOCK, GfxManager::NumLock},
	{VK_SCROLL,  GfxManager::ScrollLock},

	{VK_OEM_1,     GfxManager::Semicolon},
	{VK_OEM_PLUS,  GfxManager::Equal},
	{VK_OEM_COMMA, GfxManager::Comma},
	{VK_OEM_MINUS, GfxManager::Minus},
	{VK_OEM_PERIOD,GfxManager::Period},
	{VK_OEM_2,     GfxManager::Slash},
	{VK_OEM_3,     GfxManager::Bar},
	{VK_OEM_4,     GfxManager::BracketLeft},
	{VK_OEM_5,     GfxManager::Backslash},
	{VK_OEM_6,     GfxManager::BracketRight},
	{VK_OEM_7,     GfxManager::Quote},
	{VK_PROCESSKEY,GfxManager::ProcessKey},
	{VK_OEM_AX,	   GfxManager::OEM_AX},   //  'AX' key on Japanese AX kbd
	{VK_OEM_102,   GfxManager::OEM_102},  //  "<>" or "\|" on RT 102-key kbd.
	{VK_ICO_HELP,  GfxManager::ICO_HELP}, //  Help key on ICO
	{VK_ICO_00,	   GfxManager::ICO_00}	//  00 key on ICO
};

class OurFSCommandHandler : public GFxFSCommandHandler
{
public:
	virtual void Callback(GFxMovieView* pmovie,
		const char* pcommand, const char* parg)
	{
		GfxManager*   pMgr = (GfxManager*) pmovie->GetUserData();    
		if (!pMgr || !pcommand)
			return;
		//TODO:这里调用Lua的固定脚本.	2010-7-14
// 		// Process commands
		pMgr->fsCommand(pcommand, parg);
// 		if (!strcmp(pcommand, "loadFlashBottom") || !strcmp(pcommand, "loadFlashTop"))
// 		{
// 			// Flash loading
// 			if (!parg)
// 				return;
// 
// 			GfxMoviePtr movie = pMgr->createMovie(parg);
// 			GfxMovieViewPtr view = pMgr->createMovieView(movie, std::string(parg) + "1");
// 			if (!view)
// 			{
// 				// No movie
// 				char msg[MAX_PATH];                 
// 				G_sprintf(msg, MAX_PATH, "Could not load the SWF movie file: %s", parg);
// 				MessageBox(NULL, msg, "GFxPlayer.exe", MB_OK);
// 				return;
// 			}
// 
// // 			view->SetRenderConfig(pMgr->m_pRenderConfig);
// 			// view->GetMovie()->SetFSCommandCallback(&FsCallback);
// 			view->SetUserData((void*)pMgr);
// 
// 			//view->GetMovie()->SetVerboseAction(1);
// 
// 			// No buffers, just viewport.
// 			view->SetViewport(0,0,0, 0,1255, 646);     
// 			// 2x more advances per frame, for smoother animation and UI control.
// // 			view->SetFrameFrequency(0.5f); 
// 		}
// 		else if (!strcmp(pcommand, "unloadFlashTop") )
// 		{
// 			pMgr->destroyMovieView(std::string(parg) + "1");
// 		}
// 		else if (!strcmp(pcommand, "exit"))
// 		{
// 			pMgr->stopRender();
// 		}
// 		else if (!strcmp(pcommand, "modelWireframe"))
// 		{
// 			bool state = true;
// 			if (parg && (!G_stricmp(parg,"false") || !strcmp(parg,"0")))
// 				state = false;
// 			pMgr->setRenderWireframe(state);
// 		}
// 		else if (!strcmp(pcommand, "movieSelect"))//测试	2010-7-8
// 		{
// 			if (!parg)
// 			{
// // 				pMgr->destroyAllMovieViews();
// 				return;         
// 			}
// 			GfxMoviePtr movie = pMgr->createMovie(parg);
// 			pMgr->createRTTMovieView(movie);
// 		}
// 		else
// 		{
// 			// Handle a complex command that has a colon in it
// 			char        commandOp[512];
// 			const char* commandArg = 0;
// 			if (strlen(pcommand) > 512)
// 				return;
// 
// 			// Command with a colon component
// 			UInt i;
// 			for (i=0; pcommand[i] != 0; i++)
// 			{
// 				if (pcommand[i] == ':')
// 				{
// 					memcpy(commandOp, pcommand, i);
// 					commandOp[i] = 0;
// 					commandArg = pcommand + i + 1;
// 					break;
// 				}
// 			}
// 			// Colon not found ? bail
// 			if (pcommand[i] != ':')
// 				return;
// 
// 			// Process complex command
//  			if (!strcmp(commandOp, "gotoAndPlay") && commandArg[0])
//  			{
//  				// Find the clip that this applies to
// 				GfxMoviePtr ptr = pMgr->getMovie(commandArg);
// 				GfxMovieViewPtr pmovie = pMgr->getViewTest(ptr);
//  				if (!pmovie || !parg || !parg[0])
//  					return;
//  
//  				// Separate a function name from frame number
//  				char            argPath[512 + 20];
//  				const char*     argFrame = 0;
//  				if (strlen(parg) > 512)
//  					return;
//  				argPath[0] = 0;
//  
//  				// Command with a comma component
//  				UInt i;
//  				for (i=0; parg[i] != 0; i++)
//  				{
//  					if (parg[i] == ':')
//  					{
//  						memcpy(argPath, parg, i);
//  						argPath[i] = 0;
//  						argFrame = parg + i + 1;
//  						break;
//  					}
//  				}
//  
//  				if (parg[i] == ':')
//  				{
//  					// path was found
//  
//  					// does not work right due to path lookup bug in G-SWF
//  					G_strcat(argPath, 532, ".gotoAndPlay");
//  					pmovie->Invoke(argPath, "%s", argFrame);
//  				}
//  				else
//  				{           
//  					// path not found
//  					G_strcpy(argPath, 532, "gotoAndPlay");
//  					pmovie->Invoke(argPath, "%s", parg);
//  				}
//  
//  				// Done handling gotoAndPlay
//  				return;
//  			}
// 			// Play/pause
// 			else if (!strcmp(commandOp, "buttonPlay") && commandArg[0])
// 			{
// 				bool state = true;
// 
// 				if (parg && (!G_stricmp(parg,"false") || !strcmp(parg,"0")))
// 					state = false;
// 
// 				// Set animation/rotation to a given place
// 				if (!strcmp(commandArg, "modelRotation"))
// 				{
// // 					papp->MeshRotationPaused    = !state;
// 				}
// 				else if (!strcmp(commandArg, "moviePlayback"))
// 				{
// 					pMgr->setRTTMovieViewPlayState(state);
// 				}
// 			}
// 
// 		}
	}
};
class OurExternalInterfaceHandler : public GFxExternalInterface
{
public:
	virtual void Callback(GFxMovieView* pmovieView,
		const char* methodName,
		const GFxValue* args,
		UInt argCount)
	{
	}
};

GfxManager::GfxManager()
	:m_pDevice(0)
{
	gfxInit = new GFxSystem(&myAlloc);
	m_gfxLoader = new GFxLoader;
	m_pRenderTexture = 0;
	m_pStencilSurface = 0;
	m_bRenderMovieViews = true;
	m_bRTTViewPaused = false;
	m_bMeshWireframe = false;

	m_hWnd = 0;
// 	initialise();
}
GfxManager::~GfxManager()
{
	if (m_pRenderTexture)
		m_pRenderTexture->Release();
	if (m_pStencilSurface)
		m_pStencilSurface->Release();

	destroyAllMovieViews();

	SAFE_DELETE(m_gfxLoader);
	GMemory::DetectMemoryLeaks();
	SAFE_DELETE(gfxInit);
}

bool GfxManager::initialise(void)
{
	assert(m_pDevice);
	m_hWnd = m_pDevice->m_hWnd;

	// Give the loader the default file opener
	GPtr<GFxFileOpener> pfileOpener = *new GFxFileOpener;
	m_gfxLoader->SetFileOpener(pfileOpener);

	// Register our FSCommand handler
	GPtr<GFxFSCommandHandler> pcommandHandler = *new OurFSCommandHandler;
	m_gfxLoader->SetFSCommandHandler(pcommandHandler);

	// Register our ExternalInterface handler
	GPtr<GFxExternalInterface> pEIHandler = *new OurExternalInterfaceHandler;
	m_gfxLoader->SetExternalInterface(pEIHandler);


	// Associate the renderer with the GFxLoader -- this is applied to all children,
	// meaning we do not have to register the renderer with individual GFxMovieView
	// objects.
	m_pRenderer = *GRendererD3D9::CreateRenderer();
	m_pRenderConfig = *new GFxRenderConfig(m_pRenderer);
	if(!m_pRenderer || !m_pRenderConfig)
		return false;
	m_gfxLoader->SetRenderConfig(m_pRenderConfig);

	// Use EdgeAA to improve the appearance of the interface without the computational
	// expense of full AA through the video card.
	m_pRenderConfig->SetRenderFlags(GFxRenderConfig::RF_EdgeAA);

	OnResetDevice();

	return true;
}

void GfxManager::OnResetDevice()
{
	m_hWnd = m_pDevice->m_hWnd;
	m_pRenderer->SetDependentVideoMode(m_pDevice->m_pd3dDevice, &(m_pDevice->m_d3dpp), GRendererD3D9::VMConfig_NoSceneCalls /* VMCFlags */, m_hWnd);
// 
// 	// Use the window client rect size as the viewport.
// 	D3DVIEWPORT9 vp;
// 	if(pd3dDevice->GetViewport(&vp) == D3D_OK)
//  		pContainerMovie->SetViewport(vp.Width, vp.Height, 0, 0, vp.Width, vp.Height);

}
void GfxManager::OnLostDevice()
{
	m_pRenderer->ResetVideoMode();
}

GfxMoviePtr GfxManager::createMovie(const std::string& szFileName, const std::string& name)
{
	std::string filename = getFileNameFromPath(szFileName);
	std::string path = m_strMediaDir;
	path += szFileName;
	std::string str = name;
	if (name.empty())
	{
		str = filename;
	}

	MovieMap::iterator it= m_movies.find(str);
	if (it != m_movies.end())
	{
		//已经存在刚直接返回该对象.
		return it->second;
	}
	std::string path2;
	CharCoder::GB2312ToUTF_8(path2, path.c_str(), path.length());
	// Load the movie and crate instance.
	GfxMoviePtr ptr = *(m_gfxLoader->CreateMovie(path2.c_str(),
		GFxLoader::LoadKeepBindData |
		GFxLoader::LoadWaitFrame1));
	if (ptr)
	{
		m_movies.insert(MovieMap::value_type(str, ptr));
	}
	return ptr;
}
void GfxManager::destroyMovie(const std::string& name)
{
	MovieMap::iterator it = m_movies.find(name);
	if (it != m_movies.end())
	{
		it->second = 0;
		m_movies.erase(it);
	}
}
GfxMoviePtr GfxManager::getMovie(const std::string& name)
{
	MovieMap::iterator it = m_movies.find(name);
	if (it != m_movies.end())
	{
		return it->second;
	}
	return NULL;
}

GfxMovieViewPtr GfxManager::createMovieView(GfxMoviePtr movie)
{
	// We pass 'true' argument to make sure that first frame is initialized.
	// We can use 'false' if we need to pass some information to the movie
	// before its first frame 1 ActionScript logic is executed.
	GfxMovieViewPtr ptr = *movie->CreateInstance(GFxMovieDef::MemoryParams(), true);
	if (ptr)
	{
		// Set the scale mode to preserve the aspect ratio of the Flash content
		ptr->SetViewScaleMode(GFxMovieView::SM_ShowAll);
		ptr->SetViewAlignment(GFxMovieView::Align_Center);
		ptr->SetBackgroundAlpha(0.0f);
		// Advance the movie to the first frame
		ptr->Advance(0.0f, 0);

		ptr->SetUserData((void*)this);
		//
		m_viewsOfMovie[movie].insert(MovieViewSet::value_type(ptr));
	}
	return ptr;
}
GfxMoviePtr GfxManager::getMovieOfView(GfxMovieViewPtr view)
{
	MovieViewMap::iterator it = m_viewsOfMovie.begin(), end = m_viewsOfMovie.end();
	for (; it != end; ++it)
	{
		MovieViewSet::iterator it2 = it->second.find(view);
		if (it->second.end() != it2)
			return it->first;
	}
	return NULL;
}

GfxMovieViewPtr GfxManager::createMovieView(GfxMoviePtr movie, const std::string& name)
{
	if (movie && !name.empty())
	{
		MovieViews::iterator it = m_movieViews.find(name);
		if (it != m_movieViews.end())
		{
			it->second->Advance(0.0f, 0);
			return it->second;
		}
		else
		{
			GfxMovieViewPtr ptr = createMovieView(movie);;
			if (ptr)
			{
				m_movieViews.insert(MovieViews::value_type(name, ptr));
			}
			return ptr;
		}
	}
	return NULL;
}
void GfxManager::destroyMovieView(const std::string& name)
{
	stopRender();
	MovieViews::iterator it = m_movieViews.find(name);
	if (it != m_movieViews.end())
	{
		GfxMoviePtr movie = getMovieOfView(it->second);
		if (movie)
		{
			m_viewsOfMovie[movie].erase(it->second);
			if (m_viewsOfMovie[movie].empty())
			{
				m_viewsOfMovie.erase(movie);
				//没有View在使用Movie，则删除。	林秀全.
				MovieMap::iterator m = m_movies.begin(), n = m_movies.end();
				for (; m != n; ++m)
				{
					if (m->second == movie)
					{
						m->second = 0;
						m_movies.erase(m);
						break;
					}
				}
			}
		}
		//
		it->second = 0;
		m_movieViews.erase(it);
	}
	startRender();
}
void GfxManager::destroyAllMovieViews()
{
	//move views
	{
		MovieViews::iterator it = m_movieViews.begin(), end = m_movieViews.end();
		for(; it != end; ++it)
		{
			it->second = 0;
		}
		m_movieViews.clear();
	}
	//movies
	{
		MovieMap::iterator it = m_movies.begin(), end = m_movies.end();
		for(; it != end; ++it)
		{
			it->second = 0;
		}
		m_movies.clear();
	}

	//
	m_viewsOfMovie.clear();
}

GfxMovieViewPtr GfxManager::getMovieView(const std::string& name)
{
	MovieViews::iterator it = m_movieViews.find(name);
	if (it != m_movieViews.end())
	{
		return it->second;
	}
	return NULL;
}
GfxMovieViewPtr GfxManager::getViewTest(GfxMoviePtr movie)
{
	MovieViewMap::iterator it = m_viewsOfMovie.find(movie);
	if (it != m_viewsOfMovie.end())
	{
		return *(it->second.begin());
	}
	return NULL;
}

// void GfxManager::advanceAndRender(GfxMovieViewPtr movie, float deltaTime)
// {
// 	if (!movie)
// 		return;
// 	movie->Advance(deltaTime, 0);
// 	movie->Display();
// }
void GfxManager::render(float deltaTime)
{
	if (!m_bRenderMovieViews)
		return;
//	renderTexture(deltaTime);
	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	m_nWndWidth = rc.right - rc.left;
	m_nWndHeight = rc.bottom - rc.top;

	MovieViews::iterator it = m_movieViews.begin(), end = m_movieViews.end();
	for (; it != end; ++it)
	{
		it->second->Advance(deltaTime, 0);
		it->second->Display();
		it->second->SetViewport(0, 0, 0, 0, m_nWndWidth, m_nWndHeight);
	}

	m_pDevice->m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pDevice->m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}
void GfxManager::MessageWndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MOUSEWHEEL:
		OnMouseEvent(message, wParam, lParam);
		break;
	case WM_CHAR:
		{
			UInt32 wcharCode = (UInt32)wParam;
			OnChar(wcharCode, (UInt)lParam);
		}
		break;
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			KeyCode kc = VoidSymbol;
			if (wParam >= 'A' && wParam <= 'Z')
			{
				kc = (KeyCode) ((wParam - 'A') + A);
			}
			else if (wParam >= VK_F1 && wParam <= VK_F15)
			{
				kc = (KeyCode) ((wParam - VK_F1) + F1);
			}
			else if (wParam >= '0' && wParam <= '9')
			{
				kc = (KeyCode) ((wParam - '0') + Num0);
			}
			else if (wParam >= VK_NUMPAD0 && wParam <= VK_DIVIDE)
			{
				kc = (KeyCode) ((wParam - VK_NUMPAD0) + KP_0);
			}
			else 
			{
				for (int i = 0; KeyCodeMap[i].winKey != 0; i++)
				{
					if (wParam == (UInt)KeyCodeMap[i].winKey)
					{
						kc = KeyCodeMap[i].appKey;
						break;
					}
				}
			}
			unsigned char asciiCode = 0;
			if (kc != VoidSymbol) 
			{
				// do not call ToAscii for WM_SYS... events since this may produce
				// extra characters typed in textfields (for example, when pressing Alt-Arrow)
				if (message != WM_SYSKEYDOWN && message != WM_SYSKEYUP)
				{
					// get the ASCII code, if possible.
					UINT uScanCode = ((UINT)lParam >> 16) & 0xFF; // fetch the scancode
					BYTE ks[256];
					WORD charCode;

					// Get the current keyboard state
					::GetKeyboardState(ks);

					if (::ToAscii((UINT)wParam, uScanCode, ks, &charCode, 0) > 0)
					{
						//!AB, what to do if ToAscii returns > 1?
						asciiCode = LOBYTE (charCode);
					}
				}
			}
			bool downFlag = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN) ? 1 : 0;
			OnKey(kc, asciiCode, 0, GetModifiers(), downFlag);   
			if (message == WM_SYSKEYDOWN || message == WM_SYSKEYUP)
				break; // pass SYSKEY... to DefWndProc to handle commands such as Alt-F4
			return;
		}
	}
	
}
void    GfxManager::OnKey(KeyCode keyCode, unsigned char asciiCode, unsigned int wcharCode, 
						   unsigned int mods, bool downFlag)
{
	KeyEvent((GFxKey::Code)keyCode, asciiCode, wcharCode, mods, downFlag);
}
void    GfxManager::OnChar(UInt32 wcharCode, UInt info)
{
// 	GUNUSED(info);
	if (m_pRTTMovieView && wcharCode)
	{
		GFxCharEvent event(wcharCode);
		m_pRTTMovieView->HandleEvent(event);
	}
}
void GfxManager::KeyEvent(GFxKey::Code keyCode, unsigned char asciiCode,
						   unsigned int wcharCode, unsigned int mods, bool down)
{
	if (keyCode != GFxKey::VoidSymbol) 
	{
		if (m_pRTTMovieView) 
		{
			GFxKeyEvent event(down ? GFxEvent::KeyDown : GFxKeyEvent::KeyUp, keyCode, asciiCode, wcharCode);
			event.SpecialKeysState.SetShiftPressed(mods & GfxManager::KM_Shift ? 1 : 0);
			event.SpecialKeysState.SetCtrlPressed(mods & GfxManager::KM_Control ? 1 : 0);
			event.SpecialKeysState.SetAltPressed(mods & GfxManager::KM_Alt ? 1 : 0);
			event.SpecialKeysState.SetNumToggled(mods & GfxManager::KM_Num ? 1 : 0);
			event.SpecialKeysState.SetCapsToggled(mods & GfxManager::KM_Caps ? 1 : 0);
			event.SpecialKeysState.SetScrollToggled(mods & GfxManager::KM_Scroll ? 1 : 0);
			m_pRTTMovieView->HandleEvent(event);
		}
	}
}
void GfxManager::OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	GFxEvent::EventType evt = GFxEvent::None;
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		evt = GFxEvent::MouseMove;
		break;
	case WM_LBUTTONDOWN:
		::SetCapture(m_hWnd);
		evt = GFxEvent::MouseDown;
		break;
	case WM_LBUTTONUP:
		::ReleaseCapture();
		evt = GFxEvent::MouseUp;
		break;
	case WM_MOUSEWHEEL:
		evt = GFxEvent::MouseWheel;
		break;
	default:
		return;;
	}
	float screenX = LOWORD(lParam);
	float screenY = HIWORD(lParam);

	//测试RTT的交互
	if (m_pRTTMovieView)
	{
		int x = screenX, y = screenY;
		if (convertFromScreenToRTT(m_pRTTMovieView, x, y))
		{
			GFxMouseEvent mevent(evt, 0, x,y);
			m_pRTTMovieView->HandleEvent(mevent);
		}
	}
	//事件处理
	if (evt != GFxEvent::None)
	{
		GFxMouseEvent mevent(evt, 0, screenX,screenY);

		MovieViewMap::iterator it1 = m_viewsOfMovie.begin(), end1 = m_viewsOfMovie.end();
		for (; it1 != end1; ++it1)
		{
			MovieViewSet::iterator it = it1->second.begin(), end = it1->second.end();
			for (; it != end; ++it)
			{
				(*it)->HandleEvent(mevent);
			}
		}
	}

}
void GfxManager::CreateRenderTexture(int bufferWidth, int bufferHeight, bool stencil)
{
	if (!m_pRenderTexture)
	{
		if (FAILED( m_pDevice->m_pd3dDevice->CreateTexture(         
			bufferWidth,bufferHeight,0,
			D3DUSAGE_RENDERTARGET|D3DUSAGE_AUTOGENMIPMAP, D3DFMT_X8R8G8B8, 
			D3DPOOL_DEFAULT, &m_pRenderTexture, 0) ))
			return;
	}

	if (stencil)
	{
		m_pDevice->m_pd3dDevice->CreateDepthStencilSurface( bufferWidth,bufferHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0,
			TRUE, &m_pStencilSurface, 0);
	}

	m_nRTTwidth = bufferWidth;
	m_nRTTheight = bufferHeight;

	MakePerspective((float*)&m_matRTTProj, 3.141592f * 0.25f, Float(m_nRTTwidth)/Float(m_nRTTheight), 0.1f, 100);
	D3DXMatrixInverse(&m_matRTTProjInv, 0, &m_matRTTProj);

	D3DXMatrixIdentity(&m_matWorld);
	
// 	D3DXVECTOR3 pos(0, 0, -1), lookat(0,0,1),up(0,1,0);
// 	D3DXMatrixLookAtLH(&m_matView, &pos, &lookat, &up);
	D3DXMatrixIdentity(&m_matView);
	Translate((Float*)&m_matView, 0, 0, -2.5);
	D3DXMatrixInverse(&m_matViewInv, 0, &m_matView);

}
GfxMovieViewPtr GfxManager::createRTTMovieView(GfxMoviePtr movie)
{
	if (!movie)
		return NULL;
// 	m_gfxLoader->GetMovieInfo(szFileName.c_str(), &m_rttMovieInfo);
	if (m_pRTTMovieView)
	{
		//删除原来的RTTMoview，测试

		//
		m_pRTTMovieView = 0;
	}
	m_pRTTMovieView = *movie->CreateInstance(GFxMovieDef::MemoryParams(), true);
	if (m_pRTTMovieView)
	{
		// Set the scale mode to preserve the aspect ratio of the Flash content
		m_pRTTMovieView->SetViewScaleMode(GFxMovieView::SM_ShowAll);
		m_pRTTMovieView->SetViewAlignment(GFxMovieView::Align_Center);
		m_pRTTMovieView->SetBackgroundAlpha(0.0f);
		// Advance the movie to the first frame
// 		m_pRTTMovieView->Advance(0.0f, 0);

		m_pRTTMovieView->SetViewport(m_nRTTwidth, m_nRTTheight, 0, 0, m_nRTTwidth, m_nRTTheight, GViewport::View_RenderTextureAlpha);
		m_pRTTMovieView->SetUserData((void*)this);
	}
	return m_pRTTMovieView;
}
void GfxManager::changeRTTMovieView(GfxMovieViewPtr view)
{
	if (m_pRTTMovieView == view)
		return;
	if (m_pRTTMovieView)
	{
		m_pRTTMovieView = 0;
	}
	m_pRTTMovieView = view;
}

bool GfxManager::convertFromScreenToRTT(GPtr<GFxMovieView> pMovieView, int& x, int& y)
{
// 	RECT rc;
// 	::GetClientRect(m_hWnd, &rc);
// 	m_nWndWidth = rc.right - rc.left;
// 	m_nWndHeight = rc.bottom - rc.top;
// 
// 	//转换当前窗口坐标到以窗口中心为原点的[-1.0, 1.0]的坐标。
// 	Float fx = (x - m_nWndWidth*0.5f) / (m_nWndWidth*0.5f);
// 	Float fy = (y - m_nWndHeight*0.5f) / (m_nWndHeight*0.5f);
// 
	if (m_pScene->model.size() <= 0)
		return false;
	CSceneNode* pnode = m_pScene->model[0]->m_pNode;
	LPD3DXMESH pMesh = m_pScene->model[0]->getD3DXmesh();
	////
	Ray ray;
	ray.BuildWithSceenPos(*(m_pScene->m_CurViewport->GetD3DViewport()), m_pScene->m_MatProj, x, y);
 	D3DXMatrixInverse(&m_matViewInv, 0, &(m_pScene->m_MatView));
	ray.Transfrom(m_matViewInv);//m_pScene->m_MatViewInverse
	ray.Transfrom(pnode->getWorldInverseMatrix());
	BOOL bHit = FALSE;
	DWORD faceIdx = 0, hitCount = 0;
	FLOAT u,v,dist;
	ID3DXBuffer* ppHits = 0;
	HRESULT hr = D3DXIntersect(pMesh, &(ray.GetOrigin()), &(ray.GetDirection()),&bHit, &faceIdx, &u, &v,&dist,&ppHits,&hitCount);
	if (bHit == TRUE)
	{
		D3DXINTERSECTINFO* info = (D3DXINTERSECTINFO*)ppHits->GetBufferPointer();
		LPDIRECT3DVERTEXBUFFER9 pVB;
		LPDIRECT3DINDEXBUFFER9  pIB;
		pMesh->GetVertexBuffer( &pVB );
		pMesh->GetIndexBuffer( &pIB );
		DWORD*      pIndices;
		VERTEXDECL_POSNORMALCOLORCOORD4* pVertices;
		pIB->Lock( 0, 0, (void**)&pIndices, 0 );
		pVB->Lock( 0, 0, (void**)&pVertices, 0 );

		CVertex_UV uv1, uv2, uv3;
		uv1.u = (pVertices[pIndices[3*faceIdx+0]]).su1;
		uv1.v = (pVertices[pIndices[3*faceIdx+0]]).sv1;
		uv2.u = (pVertices[pIndices[3*faceIdx+1]]).su1;
		uv2.v = (pVertices[pIndices[3*faceIdx+1]]).sv1;
		uv3.u = (pVertices[pIndices[3*faceIdx+2]]).su1;
		uv3.v = (pVertices[pIndices[3*faceIdx+2]]).sv1;
		float uu = uv1.u + u*(uv2.u - uv1.u) + v*(uv3.u - uv1.u);
		float vv = uv1.v + u*(uv2.v - uv1.v) + v*(uv3.v - uv1.v); 

		pVB->Unlock();
		pIB->Unlock();

		x = SInt(uu * m_nRTTwidth);
		y = SInt(vv * m_nRTTheight);
		return true;
	}
	return false;
// 	D3DXVECTOR4 v4,wz1,wz2;
// 	v4.x = 0; v4.y = 0; v4.z = -0.5;v4.w = 1;
// 	D3DXVec4Transform(&wz1, &v4, &(m_pScene->m_MatProj));
// 	v4.x = 0; v4.y = 0; v4.z = -1;v4.w = 1;
// 	D3DXVec4Transform(&wz2, &v4, &(m_pScene->m_MatProj));
// 
// 	D3DXMATRIX invProj;
// 	D3DXMatrixInverse(&invProj, 0, &(m_pScene->m_MatProj));
// 	D3DXVECTOR4 x1p, x2p, x1o,x2o;
// 	v4.x = fx*wz1.w; v4.y = fy*wz1.w; v4.z = wz1.z*wz1.w ;v4.w = wz1.w;
// 	D3DXVec4Transform(&x1p, &v4, &invProj);//m_matRTTProjInv
// 	v4.x = fx*wz2.w; v4.y = fy*wz2.w; v4.z = wz2.z*wz1.w ;v4.w = wz2.w;
// 	D3DXVec4Transform(&x2p, &v4, &invProj);
// 
// 	//把4个分量的齐次空间坐标转换为三维坐标.
// 	VectorInvHomog(&x1p);
// 	VectorInvHomog(&x2p);
// 
// 	//把点还原到3维的世界坐标系中.
// 	D3DXMatrixInverse(&m_matViewInv, 0, &(m_pScene->m_MatView));
// // 	m_matViewInv = m_pScene->m_MatViewInverse;
// 	D3DXVec4Transform(&x1o, &x1p, &m_matViewInv);
// 	D3DXVec4Transform(&x2o, &x2p, &m_matViewInv);
// 
// 	const D3DXMATRIX& invWorld = pnode->getWorldInverseMatrix();
// 	D3DXVec4Transform(&x1o, &x1o, &(invWorld));
// 	D3DXVec4Transform(&x2o, &x2o, &(invWorld));
// 
// 	Float dx = x2o.x-x1o.x;
// 	Float dy = x2o.y-x1o.y;
// 	Float dz = x2o.z-x1o.z;
// 	Float mz = -x1o.z/dz;
// 
// 	fx = x1o.x + mz * dx;
// 	fy = x1o.y + mz * dy;
// 
// 
// 	x = SInt(fx * m_nRTTwidth*0.5f + m_nRTTwidth*0.5f);
// 	y = SInt(fy * m_nRTTheight*0.5f + m_nRTTheight*0.5f);
}

void GfxManager::renderTexture(float delta)
{
	if (!m_pRenderTexture || !m_pRTTMovieView || !m_pDevice->m_pd3dDevice)
		return;
	if (m_bRTTViewPaused)
		return;
	// Clear the backbuffer and the zbuffer
	m_pDevice->m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
		D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	IDirect3DSurface9 *poldSurface      = 0;
	IDirect3DSurface9 *poldDepthSurface = 0;
	IDirect3DSurface9 *psurface         = 0;

	// Draw Flash to the renderer texture       
	if (m_pRenderTexture)
	{           
		// Save both RT and depth-stencil.
		m_pDevice->m_pd3dDevice->GetRenderTarget(0, &poldSurface);
		m_pDevice->m_pd3dDevice->GetDepthStencilSurface(&poldDepthSurface);

		// Get our surface & set it as RT
		m_pRenderTexture->GetSurfaceLevel(0, &psurface);                  

		if (SUCCEEDED(m_pDevice->m_pd3dDevice->SetRenderTarget(0, psurface )))
		{
			// Set stencil; this will disable it if not available.
			m_pDevice->m_pd3dDevice->SetDepthStencilSurface(m_pStencilSurface);           
		}
	}

// 	m_pDevice->m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR/*D3DTEXF_POINT*/);
// 	m_pDevice->m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR/*D3DTEXF_POINT*/);
// 	m_pDevice->m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR/*D3DTEXF_POINT*/);

	// Render.

	// Render flash contents here
	m_pRTTMovieView->SetViewport(m_nRTTwidth, m_nRTTheight, 0, 0, m_nRTTwidth, m_nRTTheight);

	// Must clear after changing viewport.
	m_pRTTMovieView->SetBackgroundAlpha(0.5f);

	m_pRTTMovieView->Advance(delta, 0);
	// Renderer-specific preparation (Disable depth testing)
	m_pRTTMovieView->Display();

	// Restore states.
// 	m_pDevice->m_pd3dDevice->SetVertexShader(0);
	//pDevice->SetPixelShader(0);
// 	m_pDevice->m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
// 	m_pDevice->m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	// Enable depth testing for 3D
// 	m_pDevice->m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
// 	m_pDevice->m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
// 	m_pDevice->m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

// 	D3DXSaveSurfaceToFile("c:\\backbuffer.jpg", D3DXIFF_JPG, psurface, NULL, NULL);

	// Restore RT state.
	if (m_pRenderTexture)
	{
		// Restore the render target
		m_pDevice->m_pd3dDevice->SetRenderTarget(0, poldSurface);
		m_pDevice->m_pd3dDevice->SetDepthStencilSurface(poldDepthSurface);

		if (psurface)
			psurface->Release();
		if (poldSurface)
			poldSurface->Release();
		if (poldDepthSurface)
			poldDepthSurface->Release();

		// Need to do this so that mipmaps are updated.
		m_pRenderTexture->AddDirtyRect(0);    
	}

	m_pDevice->m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pDevice->m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}
void GfxManager::setRTTMovieViewPlayState(bool bPlaying)
{
	if (m_pRTTMovieView)
	{
		m_bRTTViewPaused = !bPlaying;
		m_pRTTMovieView->SetPlayState( bPlaying ? GFxMovie::Playing : GFxMovie::Stopped );
	}
}
void GfxManager::setDeviceTexture(int nIndex)
{
	if (m_pRTTMovieView &&m_pRenderTexture && m_pDevice->m_pd3dDevice)
	{
// 		 	D3DXSaveTextureToFile("c:\\backbuffer.jpg", D3DXIFF_JPG, m_pRenderTexture, NULL);
		m_pDevice->m_pd3dDevice->SetTexture( nIndex, m_pRenderTexture );
	}

}

void GfxManager::fsCommand(const char* commands, const char* args)
{
	m_pRoot->GetVGSCore()->getLuaEnginePtr()->OnFsCommand("", commands, args);
	// Process commands
	if (!strcmp(commands, "loadFlashBottom") || !strcmp(commands, "loadFlashTop"))
	{
		// Flash loading
		if (!args)
			return;

		GfxMoviePtr movie = createMovie(args);
		GfxMovieViewPtr view = createMovieView(movie, std::string(args) + "1");
		if (!view)
		{
			// No movie
			char msg[MAX_PATH];                 
			G_sprintf(msg, MAX_PATH, "Could not load the SWF movie file: %s", args);
			MessageBox(NULL, msg, "GFxPlayer.exe", MB_OK);
			return;
		}

		// 			view->SetRenderConfig(m_pRenderConfig);
		// view->GetMovie()->SetFSCommandCallback(&FsCallback);
		view->SetUserData((void*)this);

		//view->GetMovie()->SetVerboseAction(1);

		// No buffers, just viewport.
		view->SetViewport(0,0,0, 0,1255, 646);     
		// 2x more advances per frame, for smoother animation and UI control.
		// 			view->SetFrameFrequency(0.5f); 
	}
	else if (!strcmp(commands, "unloadFlashTop") )
	{
		destroyMovieView(std::string(args) + "1");
	}
	else if (!strcmp(commands, "exit"))
	{
		stopRender();
	}
	else if (!strcmp(commands, "modelWireframe"))
	{
		bool state = true;
		if (args && (!G_stricmp(args,"false") || !strcmp(args,"0")))
			state = false;
		setRenderWireframe(state);
	}
	else if (!strcmp(commands, "movieSelect"))//测试	2010-7-8
	{
		if (!args)
		{
			// 				destroyAllMovieViews();
			return;         
		}
		GfxMoviePtr movie = createMovie(args);
		createRTTMovieView(movie);
	}
	else
	{
		// Handle a complex command that has a colon in it
		char        commandOp[512];
		const char* commandArg = 0;
		if (strlen(commands) > 512)
			return;

		// Command with a colon component
		UInt i;
		for (i=0; commands[i] != 0; i++)
		{
			if (commands[i] == ':')
			{
				memcpy(commandOp, commands, i);
				commandOp[i] = 0;
				commandArg = commands + i + 1;
				break;
			}
		}
		// Colon not found ? bail
		if (commands[i] != ':')
			return;

		// Process complex command
		if (!strcmp(commandOp, "gotoAndPlay") && commandArg[0])
		{
			// Find the clip that this applies to
			GfxMoviePtr ptr = getMovie(commandArg);
			GfxMovieViewPtr pmovie = getViewTest(ptr);
			if (!pmovie || !args || !args[0])
				return;

			// Separate a function name from frame number
			char            argPath[512 + 20];
			const char*     argFrame = 0;
			if (strlen(args) > 512)
				return;
			argPath[0] = 0;

			// Command with a comma component
			UInt i;
			for (i=0; args[i] != 0; i++)
			{
				if (args[i] == ':')
				{
					memcpy(argPath, args, i);
					argPath[i] = 0;
					argFrame = args + i + 1;
					break;
				}
			}

			if (args[i] == ':')
			{
				// path was found

				// does not work right due to path lookup bug in G-SWF
				G_strcat(argPath, 532, ".gotoAndPlay");
				pmovie->Invoke(argPath, "%s", argFrame);
			}
			else
			{           
				// path not found
				G_strcpy(argPath, 532, "gotoAndPlay");
				pmovie->Invoke(argPath, "%s", args);
			}

			// Done handling gotoAndPlay
			return;
		}
		// Play/pause
		else if (!strcmp(commandOp, "buttonPlay") && commandArg[0])
		{
			bool state = true;

			if (args && (!G_stricmp(args,"false") || !strcmp(args,"0")))
				state = false;

			// Set animation/rotation to a given place
			if (!strcmp(commandArg, "modelRotation"))
			{
				// 					papp->MeshRotationPaused    = !state;
			}
			else if (!strcmp(commandArg, "moviePlayback"))
			{
				setRTTMovieViewPlayState(state);
			}
		}

	}
}