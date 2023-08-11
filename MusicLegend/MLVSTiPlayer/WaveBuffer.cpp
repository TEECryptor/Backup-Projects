//--------------------------------------------------
//	File Name:WaveBuffer.h
//	Introduce:Class CWaveBuffer implement file
//	Date:2010-09-4
//--------------------------------------------------
#include "StdAfx.h"
#include ".\wavebuffer.h"
//--------------------------------------------------
CWaveBuffer::CWaveBuffer(DWORD dwLen)
 : m_bAlloc(FALSE)
 , m_lpHdr(NULL)
 , m_hGlPtr(NULL)
 , m_dwStamp(0)
{
	Init();
	Set(NULL, dwLen);
}
//--------------------------------------------------
CWaveBuffer::~CWaveBuffer(void)
{
}
//--------------------------------------------------



//--------------------------------------------------
int CWaveBuffer::Length()
{
	if (!m_lpHdr)
		return 0;
	if (m_lpHdr->dwBytesRecorded)
		return m_lpHdr->dwBytesRecorded;
	else
		return m_lpHdr->dwBufferLength;
}
//--------------------------------------------------
CWaveBuffer::operator LPWAVEHDR()
{
	return m_lpHdr;
}
//--------------------------------------------------
void CWaveBuffer::Init()
{
	m_bAlloc = FALSE;
	m_lpHdr = NULL;
	m_hGlPtr = NULL;
	m_dwStamp = 0;
}
//--------------------------------------------------
void CWaveBuffer::Set(LPVOID lpBuf, DWORD dwLen)
{
	Empty();

	m_hGlPtr = GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE, sizeof(WAVEHDR) + dwLen);
	if (NULL != m_hGlPtr)
		return;

	m_lpHdr = (LPWAVEHDR)GlobalLock(m_hGlPtr);
	if (NULL != m_lpHdr)
	{
		m_bAlloc = TRUE;
		memset((LPVOID)m_lpHdr, 0, sizeof(WAVEHDR));
		m_lpHdr->lpData = (LPSTR)(m_lpHdr+1);
		m_lpHdr->dwBufferLength = dwLen;
		m_lpHdr->dwBytesRecorded = (lpBuf) ? dwLen : 0;
		if (NULL != lpBuf)
			memcpy(m_lpHdr->lpData, lpBuf, dwLen);
		else
			memset(m_lpHdr->lpData, 0, dwLen);
	}
}
//--------------------------------------------------
void CWaveBuffer::Empty()
{
	if (m_bAlloc)
	{
		GlobalUnlock(m_hGlPtr);
		GlobalFree(m_lpHdr);
	}
	Init();            
}
//--------------------------------------------------
