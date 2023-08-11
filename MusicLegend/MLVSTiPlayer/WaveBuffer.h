//--------------------------------------------------
//	File Name:WaveBuffer.h
//	Introduce:Class CWaveBuffer definition file
//	Date:2010-09-4
//--------------------------------------------------
#ifndef	_WAVEBUFFER_H_
#define	_WAVEBUFFER_H_
//--------------------------------------------------
#include <mmsystem.h> 
//--------------------------------------------------
class CWaveBuffer
{
public:
	CWaveBuffer(DWORD dwLen = 0);
	~CWaveBuffer(void);
	//----------------------------------------------
public:
	int Length();
	operator LPWAVEHDR();
	//----------------------------------------------
private:
	void	Init();
	void	Set(LPVOID lpBuf, DWORD dwLen);
	void	Empty();
	//----------------------------------------------
private:
    BOOL		m_bAlloc;
    LPWAVEHDR	m_lpHdr;
    HGLOBAL		m_hGlPtr;
    DWORD		m_dwStamp;
	//----------------------------------------------
};
//--------------------------------------------------
#endif	//_WAVEBUFFER_H_
//--------------------------------------------------
