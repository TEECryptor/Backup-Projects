//--------------------------------------------------
//	File Name:WaveOutDevice.h
//	Introduce:Class CWaveOutDevice implement file
//	Date:2010-09-2
//--------------------------------------------------
#include "StdAfx.h"
#include <math.h>
#include ".\waveoutdevice.h"
//--------------------------------------------------
inline float Saturate(float input, float fMax)
{
	static const float fGrdDiv = 0.5f;
	float x1 = fabsf(input + fMax);
	float x2 = fabsf(input - fMax);
	return fGrdDiv * (x1 - x2);
}
//--------------------------------------------------
CWaveOutDevice::CWaveOutDevice(void)
 : m_iSystemIndex(-1)
 , m_bIsOpen(FALSE)
 , m_hEvent(NULL)
 , m_bEventCancel(FALSE)
 , m_hWavDev(NULL)
 , m_hEventThread(NULL)
 , m_dwEventThreadID(0)
 , m_iAllocBufs(0)
 , m_iUsedBufs(0)
 , m_dwBufsMask(0)
 , m_pBuffers(NULL)
{
	memset(m_tcFriendName, 0, 128);
}
//--------------------------------------------------
CWaveOutDevice::~CWaveOutDevice(void)
{
	Close();
	FreeBuffers();
}
//--------------------------------------------------


//--------------------------------------------------
void CWaveOutDevice::SetSystemIndex(int iIndex)
{
	m_iSystemIndex = iIndex;
}
//--------------------------------------------------
int	CWaveOutDevice::GetSystemIndex()
{
	return m_iSystemIndex;
}
//--------------------------------------------------
void CWaveOutDevice::SetFriendName(LPCTSTR lpszName)
{
	if (NULL != lpszName)
	{
		_tcscpy(m_tcFriendName, lpszName);
	}
}
//--------------------------------------------------
LPCTSTR	CWaveOutDevice::GetFriendName()
{
	return m_tcFriendName;
}
//--------------------------------------------------
BOOL CWaveOutDevice::Open()
{
	if (TRUE == IsOpen())
		Close();

	if (waveOutGetDevCaps(m_iSystemIndex, &m_stuWavCaps, sizeof(m_stuWavCaps)))
		return FALSE;

	ZeroMemory(&m_stuWavFormat, sizeof(m_stuWavFormat));
	_SetupWaveformat(&m_stuWavFormat); 

	if (!_StartEvent())
		return FALSE;

	DWORD dwCallback = (DWORD)m_hEvent;
	DWORD dwCallbackInst = 0;

	WORD wRtn = waveOutOpen(&m_hWavDev, m_iSystemIndex, &m_stuWavFormat, dwCallback, dwCallbackInst, CALLBACK_EVENT);
	if (MMSYSERR_NOERROR != wRtn)
	{
		m_hWavDev = NULL;
		return FALSE;
	}

	waveOutGetDevCaps(m_iSystemIndex, &m_stuWavCaps, sizeof(m_stuWavCaps));
	m_bIsOpen = TRUE;

	return TRUE;
}
//--------------------------------------------------
BOOL CWaveOutDevice::IsOpen()
{
	return m_bIsOpen;
}
//--------------------------------------------------
void CWaveOutDevice::Close()
{
	if (!IsOpen())
		return;

	waveOutReset(m_hWavDev);

	int iSleeps = 0;
	while (m_iUsedBufs > 0)
	{
		Sleep(50);
		if (++iSleeps > 20)
		{
			break;
		}
	}	

	waveOutClose(m_hWavDev);
	m_hWavDev = NULL;

	_KillEvent();

	memset(m_tcFriendName, 0, 128);
	m_bIsOpen = FALSE;
}
//--------------------------------------------------
void CWaveOutDevice::AllocateBuffers(int iSize)
{
	FreeBuffers();
	if (iSize <= 0)
		return;

	int nSize = ((44100 + iSize - 1) / iSize) * m_stuWavFormat.nBlockAlign;
	m_pBuffers = new CWaveBuffer* [iSize];
	for (int i = 0; i < iSize; i++)
		m_pBuffers[i] = new CWaveBuffer(iSize);
	m_iAllocBufs = iSize;
}
//--------------------------------------------------
void CWaveOutDevice::FreeBuffers()
{
	if (NULL != m_pBuffers)
	{
		for (int i = 0; i < m_iAllocBufs; i++)
		{
			if (NULL != m_pBuffers[i])
			{
				delete []m_pBuffers[i];
				m_pBuffers[i] = NULL;
			}
		}
		delete []m_pBuffers;
		m_pBuffers = NULL;
	}
	m_iAllocBufs = 0;
}
//--------------------------------------------------
DWORD CWaveOutDevice::SendData(float** pWavData, DWORD dwLenght)
{
	if (FALSE == IsOpen())
		return 0;

	int iBufLen = (44100 + m_iAllocBufs - 1) / m_iAllocBufs;

	int iUse = 0;
	int iOff = 0;
	short sValue;
	while(0 != dwLenght)
	{
		iUse = (dwLenght <= iBufLen) ? dwLenght : iBufLen;

		// To find the first unused buffer
		for (int iBuff = 0; iBuff < m_iAllocBufs; iBuff++) 
		{
			if (!(m_dwBufsMask & (1 << iBuff)) && NULL != m_pBuffers[iBuff])
				break;
		}

		// All buffers are used
		if (iBuff >= m_iAllocBufs) 
			return 0;
		
		float fsmpl, fMax = 0.f;

		// Convert all samples to target
		for (int j = 0; j < iUse; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				fsmpl = pWavData[k][j + iOff];
			#if defined(_DEBUG) || defined(_DEBUGFILE)
				float fCur = fabsf(fsmpl);        /* just us diagnostics in here :-)   */
				if (fCur > fMax)
					fMax = fCur;
			#endif
												/* really HARD clipping method...    */
				sValue = (short) ((Saturate(fsmpl, 1.0f) * 32767.505f) - .5f);
				_SetSample(*m_pBuffers[iBuff], j, k, sValue);
			}
		}

		// remember this buffer's in use
		m_dwBufsMask |= (1 << iBuff);

		// increment # used buffers
		m_iUsedBufs++;

		DWORD dwLen = iUse * m_stuWavFormat.nBlockAlign;
		((LPWAVEHDR)*m_pBuffers[iBuff])->dwBufferLength = dwLen;
		((LPWAVEHDR)*m_pBuffers[iBuff])->dwBytesRecorded = dwLen;

		// then send it to output
		_Output(*m_pBuffers[iBuff], FALSE);

		iOff += iUse;
		dwLenght -= iUse;
	}

	return 0;
}
//--------------------------------------------------
UINT CWaveOutDevice::EventThreadProc(LPVOID pParam)
{
	CWaveOutDevice *pDev = (CWaveOutDevice *)pParam;
	if (NULL == pDev)
		return -1;

	UINT uResult = 0;
	while (FALSE == pDev->m_bEventCancel)
	{
		uResult = WaitForSingleObject(pDev->m_hEvent, INFINITE);
		if ((uResult == WAIT_OBJECT_0) && (!pDev->m_bEventCancel))
			pDev->OnEvent();
		else
			break;
	}
	pDev->m_bEventCancel = FALSE;

	return 0;
}
//--------------------------------------------------
void CWaveOutDevice::OnEvent()
{
	for (int iIndex = 0; iIndex < m_iAllocBufs; iIndex++)
	{
		if (m_pBuffers[iIndex])
		{
			LPWAVEHDR lphdr = *m_pBuffers[iIndex];
			if (lphdr->dwFlags & WHDR_DONE)
			OnWomDone(lphdr);
		}
	}
}
//--------------------------------------------------
void CWaveOutDevice::OnWomDone(LPWAVEHDR lphdr)
{
	_UnprepareHeader(lphdr);

	for (int i = 0; i < m_iAllocBufs; i++)
	{
		if ((m_pBuffers[i]) && (lphdr == (LPWAVEHDR)(*m_pBuffers[i])))
		{
			lphdr->dwFlags &= ~WHDR_DONE;
			m_dwBufsMask &= ~(1 << i);
			if (m_iUsedBufs > 0)
				m_iUsedBufs--;
		}
	}
}
//--------------------------------------------------



//--------------------------------------------------
void CWaveOutDevice::_SetupWaveformat(LPWAVEFORMATEX pwf,
									 DWORD dwSamplesPerSec,
									 WORD wBitsPerSample,
									 WORD nChannels)
{
	pwf->wFormatTag = WAVE_FORMAT_PCM;
	pwf->nChannels = nChannels;
	pwf->nSamplesPerSec = dwSamplesPerSec;
	pwf->nAvgBytesPerSec = dwSamplesPerSec * nChannels * wBitsPerSample / 8;
	pwf->nBlockAlign = nChannels * wBitsPerSample / 8;
	pwf->wBitsPerSample = wBitsPerSample;
	pwf->cbSize = 0;
}
//--------------------------------------------------
BOOL CWaveOutDevice::_StartEvent()
{
	_KillEvent();

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_hEvent)
		return FALSE;

	m_hEventThread = CreateThread(NULL, 
								 0,
								 (LPTHREAD_START_ROUTINE)EventThreadProc,
								 this,
								 CREATE_SUSPENDED,
								 &m_dwEventThreadID);
	if (NULL == m_hEventThread)
	{
		_KillEvent();
		return FALSE;
	}

	ResumeThread(m_hEventThread);

	return TRUE;
}
//--------------------------------------------------
void CWaveOutDevice::_KillEvent()
{
	if (NULL != m_hEvent)
	{
		if (NULL != m_hEventThread)
		{
			m_bEventCancel = TRUE;
			SetEvent(m_hEvent);

			int iLoop = 0;
			while(TRUE == m_bEventCancel)
			{
				Sleep(50);
				if (iLoop++ > 50)
					break;
			}
			CloseHandle(m_hEventThread);
			m_hEventThread = NULL;
		}
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
}
//--------------------------------------------------
BOOL CWaveOutDevice::_PrepareHeader(LPWAVEHDR lpHdr)
{
	if (NULL == m_hWavDev)
		return FALSE;

	return !waveOutPrepareHeader(m_hWavDev, lpHdr, sizeof(WAVEHDR));
}
//--------------------------------------------------
BOOL CWaveOutDevice::_UnprepareHeader(LPWAVEHDR lpHdr)
{
	if (NULL == m_hWavDev)
		return FALSE;

	return !waveOutUnprepareHeader(m_hWavDev, lpHdr, sizeof(WAVEHDR));
}
//--------------------------------------------------
void CWaveOutDevice::_SetSample(CWaveBuffer &buf, int iSample, int iChannel, short sValue)
{
	if ((!IsOpen()) ||
		(iChannel >= m_stuWavFormat.nChannels) ||
		(iSample < 0) ||
		(iSample * m_stuWavFormat.nBlockAlign > (int)buf.Length()))
		return;

	LPSTR lpPos = ((LPWAVEHDR)buf)->lpData + (iSample * m_stuWavFormat.nBlockAlign) + (iChannel * m_stuWavFormat.wBitsPerSample  / 8);
	switch (m_stuWavFormat.wBitsPerSample)
	{
	case 8:
		*((BYTE *)lpPos) = (BYTE)((sValue / 256) + 128);
		break;
	case 16 :
		*((short *)lpPos) = sValue;
		break;
	default:
		return;
	}
}
//--------------------------------------------------
BOOL CWaveOutDevice::_Output(CWaveBuffer &buf, BOOL bSync)
{
	if (!IsOpen())
		return FALSE;

	BOOL bRes = _PrepareHeader(buf);
	if (bRes)
	{
		bRes = !waveOutWrite(m_hWavDev, buf, sizeof(WAVEHDR));
		if (bSync)
		{
			LPWAVEHDR lpHdr = (LPWAVEHDR)buf;
			while (bRes && !(lpHdr->dwFlags & WHDR_DONE))
			{
				// Do nothing
			}

			_UnprepareHeader(buf);
		}
	}

	return bRes;
}
//--------------------------------------------------