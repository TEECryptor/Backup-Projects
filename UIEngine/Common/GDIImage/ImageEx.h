//--------------------------------------------
//	Name:ImageEx
//	Introduce:Use GDI+ to draw GIF image
//--------------------------------------------
#ifndef	_IMAGEEX_H_
#define	_IMAGEEX_H_
//--------------------------------------------
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
//--------------------------------------------
#include "Gdiplus.h"
using namespace Gdiplus;
//--------------------------------------------
class ImageEx : public Image
{
public:
	ImageEx(IN LPCTSTR  sResourceType, IN LPCTSTR  sResource);
	ImageEx(const WCHAR* filename, BOOL useEmbeddedColorManagement = FALSE);	
	~ImageEx();
	//----------------------------------------
public:
	void	Draw(CDC* pDC);
	CSize	GetSize();

	bool	IsAnimatedGIF() { return m_nFrameCount > 1; }
	void	SetPause(bool bPause);
	bool	IsPaused() { return m_bPause; }
	bool	InitAnimation(HWND hWnd, RECT rc);
	void	Destroy();
	//----------------------------------------
protected:

	bool				TestForAnimatedGIF();
	void				Initialize();
	bool				DrawFrameGIF();

	IStream*			m_pStream;
	
	bool LoadFromBuffer(BYTE* pBuff, int nSize);
	bool GetResource(LPCTSTR lpName, LPCTSTR lpType, void* pResource, int& nBufSize);
	bool Load(CString sResourceType, CString sResource);

	void ThreadAnimation();

	static UINT WINAPI _ThreadAnimationProc(LPVOID pParam);

	HANDLE			m_hThread;
	HANDLE			m_hPause;
	HANDLE			m_hExitEvent;
	HINSTANCE		m_hInst;
	HWND			m_hWnd;
	UINT			m_nFrameCount;
	UINT			m_nFramePosition;
	bool			m_bIsInitialized;
	bool			m_bPause;
	PropertyItem*	m_pPropertyItem;
	CPoint			m_pt;
	//
	HDC				m_hdc;
	RECT			m_stuRect;
	//----------------------------------------
};
//--------------------------------------------
#endif // _IMAGEEX_H_
//--------------------------------------------
