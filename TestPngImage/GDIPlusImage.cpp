//--------------------------------------------
//	FileName:GDIPlusImage.cpp
//	Introduce:The cpp file for GDIPlusImage class
//	Creator:Youfang Yao
//	Date:10/23/08
//--------------------------------------------
#include "stdafx.h"
#include "GDIPlusImage.h"
//--------------------------------------------

//--------------------------------------------
CDibStreamRead::CDibStreamRead()
{
	m_nBytesToIgnore = m_nSpaceLeft = 0;
	m_lpMem = NULL;
	m_curPos = 0;

	_ulRefs = 1;

	m_maxPos = 0;
	m_BytesWritten = m_BytesRead = 0;

}


CDibStreamRead::~CDibStreamRead()
{
}

HRESULT STDMETHODCALLTYPE CDibStreamRead::Read( 
	/* [out] */ void __RPC_FAR *pv,
	/* [in] */ ULONG cb,
	/* [out] */ ULONG __RPC_FAR *pcbRead)
{	
	int toRead = 0;
	m_nSpaceLeft = m_maxPos - m_curPos;
	if ( cb <= m_nSpaceLeft)
		toRead = cb;
	else
		toRead =  m_nSpaceLeft;

	if ( toRead)
	{
		memcpy( pv, m_lpMem+m_curPos, toRead);
		m_curPos +=  toRead;
		m_nSpaceLeft -=  toRead;

	}
	m_BytesRead += toRead;
	if( pcbRead )
		*pcbRead = toRead;
	return toRead>0 ? S_OK : S_FALSE;
}


HRESULT STDMETHODCALLTYPE CDibStreamRead::Write( /* [size_is][in] */ const void __RPC_FAR *pv,
												/* [in] */ ULONG cb,
												/* [out] */ ULONG __RPC_FAR *pcbWritten)
{

	ULONG BytesIgnored = 0;
	if ( m_nBytesToIgnore > 0)
	{
		if ( m_nBytesToIgnore)
			BytesIgnored = m_nBytesToIgnore > cb ? cb : m_nBytesToIgnore;

		pv = (BYTE*)pv + BytesIgnored;
		cb -= BytesIgnored;
		m_nBytesToIgnore -= BytesIgnored;
	}

	ULONG written = 0;
	LPBITMAPINFOHEADER bih = (LPBITMAPINFOHEADER)pv;
	if ( cb > 0)
	{
		if ( cb <= m_nSpaceLeft)
			written = cb;
		else
			written = m_nSpaceLeft;

		memcpy( m_lpMem, (LPCSTR)pv, written);
		m_lpMem += written;
		m_nSpaceLeft -= written;
	}

	written += BytesIgnored;
	m_BytesWritten += written;
	if( pcbWritten )
		*pcbWritten = written;
	return written>0 ? S_OK : S_FALSE;
}


HRESULT STDMETHODCALLTYPE CDibStreamRead::Seek( 
	/* [in] */ LARGE_INTEGER dlibMove,
	/* [in] */ DWORD dwOrigin,
	/* [out] */ ULARGE_INTEGER __RPC_FAR *plibNewPosition ) 
{ 

	if ( dwOrigin == 0)
		m_curPos = (ULONG)dlibMove.QuadPart;
	else if ( dwOrigin == 2)
	{
		if ( m_maxPos > (ULONG)dlibMove.QuadPart)
			m_curPos = m_maxPos - (ULONG)dlibMove.QuadPart;
		else
			m_curPos = 0;
	}
	else
		m_curPos += (ULONG)dlibMove.QuadPart;

	if ( m_curPos>m_maxPos)
		m_curPos = m_maxPos;

	if( plibNewPosition )
		plibNewPosition->QuadPart = m_curPos;
	return S_OK;
}



HRESULT STDMETHODCALLTYPE CDibStreamRead::QueryInterface(REFIID riid, void ** ppvObject)
{
	if (riid == IID_IUnknown)
	{
		*ppvObject = static_cast<IUnknown*>(this);
	}
	else if (riid == IID_IStream)
	{
		*ppvObject = static_cast<IStream*>(this);
	}
	else {
		ppvObject = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppvObject)->AddRef();
	return S_OK;
}


ULONG STDMETHODCALLTYPE CDibStreamRead::AddRef( void)
{
	return InterlockedIncrement(&_ulRefs);
}


ULONG STDMETHODCALLTYPE CDibStreamRead::Release( void)
{
	if (InterlockedDecrement(&_ulRefs) == 0)
	{
		delete this;
		return 0;
	}
	return _ulRefs;
}



HRESULT STDMETHODCALLTYPE CDibStreamRead::Stat( 
	/* [out] */ STATSTG __RPC_FAR *pstatstg,
	/* [in] */ DWORD grfStatFlag) 
{ 
	pstatstg->cbSize.QuadPart = m_maxPos;
	pstatstg->grfMode = (DWORD)(m_BytesRead/2048);
	pstatstg->grfStateBits = (DWORD)(m_BytesWritten/2048);
	pstatstg->grfLocksSupported = FALSE;
	pstatstg->pwcsName = _Name;
	//pstatstg->pwcsName = NULL;
	pstatstg->reserved = 0;

	return S_OK; 
}
//--------------------------------------------


//--------------------------------------------
//	Name:CGDIPlusImage
//	Introduce:Constructor
//--------------------------------------------
CGDIPlusImage::CGDIPlusImage()
 : m_pImage(NULL)
{
}
//--------------------------------------------
//	Name:~CGDIPlusImage
//	Introduce:Distructor
//--------------------------------------------
CGDIPlusImage::~CGDIPlusImage()
{
	if(NULL != m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}
//--------------------------------------------
//	Name:LoadFromFile
//	Introduce:Load a image file
//--------------------------------------------
BOOL CGDIPlusImage::LoadFromFile(LPCTSTR lpszFile)
{
	if(NULL == lpszFile || _tcslen(lpszFile) == 0)
		return FALSE;

	WCHAR wFileName[MAX_PATH] = {L"\0"};
#ifndef UNICODE
	int iLen = MultiByteToWideChar(CP_ACP, 0, lpszFile, (int)strlen(lpszFile), wFileName, MAX_PATH);
	wFileName[iLen] = L'\0';
#else
	wcscpy(wFileName, lpszFile);
#endif	//UNICODE

	if(NULL != m_pImage)
		delete m_pImage;
	m_pImage = Image::FromFile(wFileName, FALSE);	

	if(NULL == m_pImage)
		return FALSE;

	if(m_pImage->GetLastStatus() != Ok)
		return FALSE;

	return TRUE;
}
//--------------------------------------------
//	Name:IsTransparent
//	Introduce:Load the image from file
//--------------------------------------------
BOOL CGDIPlusImage::LoadFromResource(HINSTANCE hInstance, LPCTSTR lpszGroup, int iID)
{
   	CDibStreamRead *pDibStream = new CDibStreamRead;

	if(NULL == hInstance)
	{
		hInstance = AfxGetResourceHandle();	
	}

	HRSRC hRsrc = FindResource(hInstance, MAKEINTRESOURCE(iID), lpszGroup);
	pDibStream->m_maxPos = SizeofResource(hInstance, hRsrc);
	pDibStream->m_nSpaceLeft = pDibStream->m_maxPos;

	HANDLE hResource = (HRSRC)LoadResource(hInstance, hRsrc);
	pDibStream->m_lpMem = (BYTE *)LockResource(hResource);
	
	m_pImage = Image::FromStream(pDibStream);

	UnlockResource(hResource);
	FreeResource(hRsrc);
	pDibStream->Release();
	pDibStream = NULL;

	if(NULL == m_pImage)
		return FALSE;

	if(m_pImage->GetLastStatus() != Ok)
		return FALSE;

	return TRUE;
}
//--------------------------------------------
//	Name:Draw
//	Introduce:Draw this image to the rect
//--------------------------------------------
void CGDIPlusImage::Draw(HDC hdc, int x, int y)
{
}
//--------------------------------------------
//	Name:Draw
//	Introduce:Render the image all data to the hdc
//--------------------------------------------
void CGDIPlusImage::Draw(HDC hdc, CRect &rcDes)
{
	if(NULL == hdc || NULL == m_pImage)
		return;

	Graphics graphics(hdc);
	
	RectF rcFDes(	(float)rcDes.left, 
					(float)rcDes.top, 
					(float)(rcDes.right-rcDes.left), 
					(float)(rcDes.bottom-rcDes.top));

	graphics.DrawImage(	m_pImage, rcFDes);
}
//--------------------------------------------
//	Name:Draw
//	Introduce:Render a part of the image data to the rcDes
//--------------------------------------------
void CGDIPlusImage::Draw(HDC hdc, CRect &rcDes, CRect &rcSrc)
{
	if(NULL == hdc || NULL == m_pImage)
		return;

	Graphics graphics(hdc);

	RectF rcFDes(	(float)rcDes.left, 
					(float)rcDes.top, 
					(float)(rcDes.right - rcDes.left), 
					(float)(rcDes.bottom - rcDes.top));
	RectF rcFSrc(	(float)rcSrc.left, 
					(float)rcSrc.top, 
					(float)(rcSrc.right - rcSrc.left), 
					(float)(rcSrc.bottom - rcSrc.top));

	graphics.DrawImage(m_pImage, rcFDes, rcFSrc.X, rcFSrc.Y, rcFSrc.Width, rcFSrc.Height, UnitPixel);

}
//--------------------------------------------
//	Name:GetWidth
//	Introduce:Return image width
//--------------------------------------------
DWORD CGDIPlusImage::GetWidth()
{
	return m_pImage ? m_pImage->GetWidth() : 0;
}
//--------------------------------------------
//	Name:GetWidth
//	Introduce:Return image height
//--------------------------------------------
DWORD CGDIPlusImage::GetHeight()
{
	return m_pImage ? m_pImage->GetHeight() : 0;
}
//--------------------------------------------

