//------------------------------------------------
//	File:D3DEngine.h
//	Introduce:
//	Creator:Youfang Yao
//------------------------------------------------
#ifndef	_D3DENGINE_H_
#define	_D3DENGINE_H_
//------------------------------------------------
#include <d3d9.h>
//------------------------------------------------
#define SAF_RELEASE(x) if(x){x->Release(); x = NULL;}
//------------------------------------------------
class CD3DEngine
{
public:
	CD3DEngine(void);
	~CD3DEngine(void);
	//--------------------------------------------
public:
	BOOL	CreateD3DDevice(int iWidth, int iHeight, HWND hRenderWnd, BOOL bFullScreen = TRUE);
	VOID	SetFullScreen(int iWidth, int iHeight, HWND hRenderWnd, BOOL bFullScreen = TRUE);
	VOID	SetViewport(DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, float fMinZ, float fMaxZ);
	VOID	Render();
	//--------------------------------------------
private:	
	LPDIRECT3D9             m_pD3D;					// Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice;			// Our rendering device
	D3DPRESENT_PARAMETERS	m_d3dpp;				// Current device parameters	
	//--------------------------------------------
};
//------------------------------------------------
#endif	//_D3DENGINE_H_
//------------------------------------------------
