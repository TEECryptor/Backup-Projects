//--------------------------------------------
//	FileName:GDIPlusImage.h
//	Introduce:Define a image used GDI+
//	Creator:Youfang Yao
//	Date:10/23/08
//--------------------------------------------
#ifndef _GDIPLUSIMAGE_H_
#define _GDIPLUSIMAGE_H_
//--------------------------------------------
/*
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
*/
//--------------------------------------------
#include <comutil.h>
#include "Gdiplus.h"
using namespace Gdiplus;
//--------------------------------------------
class CDibStreamRead : public IStream
{
public:
	CDibStreamRead( ); 
	ULONG	m_nSpaceLeft, m_nBytesToIgnore;
	BYTE	*m_lpMem;
	ULONG	m_curPos, m_maxPos;

	virtual ~CDibStreamRead();

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef( void);
	ULONG STDMETHODCALLTYPE Release( void);
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
		/* [out] */ void __RPC_FAR *pv,
		/* [in] */ ULONG cb,
		/* [out] */ ULONG __RPC_FAR *pcbRead);
		virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
		/* [size_is][in] */ const void __RPC_FAR *pv,
		/* [in] */ ULONG cb,
		/* [out] */ ULONG __RPC_FAR *pcbWritten);

		virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek( 
		/* [in] */ LARGE_INTEGER dlibMove,
		/* [in] */ DWORD dwOrigin,
		/* [out] */ ULARGE_INTEGER __RPC_FAR *plibNewPosition);

		virtual HRESULT STDMETHODCALLTYPE SetSize( 
		/* [in] */ ULARGE_INTEGER libNewSize) { return E_FAIL; }

		virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo( 
		/* [unique][in] */ IStream __RPC_FAR *pstm,
		/* [in] */ ULARGE_INTEGER cb,
		/* [out] */ ULARGE_INTEGER __RPC_FAR *pcbRead,
		/* [out] */ ULARGE_INTEGER __RPC_FAR *pcbWritten) { return E_FAIL; }

		virtual HRESULT STDMETHODCALLTYPE Commit( 
		/* [in] */ DWORD grfCommitFlags) { return E_FAIL; }

		virtual HRESULT STDMETHODCALLTYPE Revert( void) { return E_FAIL; }

		virtual HRESULT STDMETHODCALLTYPE LockRegion( 
			/* [in] */ ULARGE_INTEGER libOffset,
			/* [in] */ ULARGE_INTEGER cb,
			/* [in] */ DWORD dwLockType) { return E_FAIL; }

			virtual HRESULT STDMETHODCALLTYPE UnlockRegion( 
			/* [in] */ ULARGE_INTEGER libOffset,
			/* [in] */ ULARGE_INTEGER cb,
			/* [in] */ DWORD dwLockType) { return E_FAIL; }

			virtual HRESULT STDMETHODCALLTYPE Stat( 
			/* [out] */ STATSTG __RPC_FAR *pstatstg,
			/* [in] */ DWORD grfStatFlag);

			virtual HRESULT STDMETHODCALLTYPE Clone( 
			/* [out] */ IStream __RPC_FAR *__RPC_FAR *ppstm) { return E_FAIL; }

protected:
	int		m_hFile;
	LONG	_ulRefs;

	ULONGLONG m_BytesWritten, m_BytesRead;
	_bstr_t _Name;
};
//--------------------------------------------
//	Name:CGDIPlusImage
//	Introduce:The image class used GDIPlus
//--------------------------------------------
class CGDIPlusImage
{
public:
	CGDIPlusImage();
	virtual ~CGDIPlusImage();
	//----------------------------------------
public:
	//Load the image from file
	virtual BOOL LoadFromFile(LPCTSTR lpszFile);
	//Load the image from file
	virtual BOOL LoadFromResource(HINSTANCE hInstance, LPCTSTR lpszGroup, int iID);
	//Draw this image to the rect
	virtual void Draw(HDC hdc, int x, int y);
	//Draw this image to the rect
	virtual void Draw(HDC hdc, CRect &rcDes);
	//Draw a part of this image to the rect
	virtual void Draw(HDC hdc, CRect &rcDes, CRect &rcSrc);
	//----------------------------------------
	//Return image width
	DWORD	GetWidth();
	//Return image height
	DWORD	GetHeight();
	//----------------------------------------
private:
	Image*	m_pImage;
	//----------------------------------------
};
//--------------------------------------------
#endif	//_GDIPLUSIMAGE_H_
//--------------------------------------------