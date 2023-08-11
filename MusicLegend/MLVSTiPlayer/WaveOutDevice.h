//--------------------------------------------------
//	File Name:WaveOutDevice.h
//	Introduce:Class CWaveOutDevice definition file
//	Date:2010-09-2
//--------------------------------------------------
#ifndef	_WAVEOUTDEVICE_H_
#define	_WAVEOUTDEVICE_H_
//--------------------------------------------------
#include "WaveBuffer.h"
//--------------------------------------------------
class CWaveOutDevice
{
public:
	CWaveOutDevice(void);
	~CWaveOutDevice(void);
	//----------------------------------------------
public:
	void	SetSystemIndex(int iIndex);
	int		GetSystemIndex();
	void	SetFriendName(LPCTSTR lpszName);
	LPCTSTR	GetFriendName();
	//
	BOOL	Open();
	BOOL	IsOpen();
	void	Close();
	void	AllocateBuffers(int iSize);
	void	FreeBuffers();
	DWORD	SendData(float** pWavData, DWORD dwLenght);
	//----------------------------------------------
public:
	static UINT EventThreadProc(LPVOID pParam);
	void	OnEvent();
	void	OnWomDone(LPWAVEHDR lphdr);
	//----------------------------------------------
private:
	void	_SetupWaveformat(LPWAVEFORMATEX pwf, DWORD dwSamplesPerSec = 44100, WORD wBitsPerSample = 16, WORD nChannels = 2);
	BOOL	_StartEvent();
	void	_KillEvent();
	BOOL	_PrepareHeader(LPWAVEHDR lpHdr);
    BOOL	_UnprepareHeader(LPWAVEHDR lpHdr);
	void	_SetSample(CWaveBuffer &buf, int iSample, int iChannel, short sValue);
	BOOL	_Output(CWaveBuffer &buf, BOOL bSync);
	//----------------------------------------------
public:
	HANDLE			m_hEvent;
	BOOL			m_bEventCancel;
	//----------------------------------------------
private:
	int				m_iSystemIndex;
	TCHAR			m_tcFriendName[128];
	BOOL			m_bIsOpen;
	//
	HWAVEOUT		m_hWavDev;  
	WAVEFORMATEX	m_stuWavFormat;
    WAVEOUTCAPS		m_stuWavCaps;
	HANDLE			m_hEventThread;
	DWORD			m_dwEventThreadID;
	//
	int				m_iAllocBufs;
	int				m_iUsedBufs;
	DWORD			m_dwBufsMask;
	CWaveBuffer**	m_pBuffers;
	//----------------------------------------------
};
//--------------------------------------------------
#endif	//_WAVEOUTDEVICE_H_
//--------------------------------------------------
