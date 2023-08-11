#include "stdafx.h"
#include "ZJCAUSBKeyEventThread.h"
#include "../Include/CommonDef.h"

CZJCAUSBKeyEventThread::CZJCAUSBKeyEventThread(void)
	: m_hThread(NULL)
	, m_ulThreadID(0)
	, m_bEndThread(FALSE)
	, m_bNotBlock(FALSE)
	, m_pUsbKey(NULL)
	, m_pEventCallback(NULL)
	, m_pCaller(NULL)
{	  
}

CZJCAUSBKeyEventThread::~CZJCAUSBKeyEventThread(void)
{
	Stop();
}

ULONG CZJCAUSBKeyEventThread::Start(IUSBKeyApi* pUsbKey, 
									BOOL bNotBlock,
									LPVOID lpCallback, 
									LPVOID lpCaller)
{
	ULONG ulRes = 0;

	if (!pUsbKey)
	{
		LogErrorMsg(_T("pUsbKey is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpCallback)
	{
		LogErrorMsg(_T("lpCallback is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}

	m_bNotBlock = bNotBlock;
	m_pUsbKey = pUsbKey;
	m_pEventCallback = (USBKeyEventCallback)lpCallback;
	m_pCaller = lpCaller;
		
	m_hThread = CreateThread(NULL, NULL, ThreadProc, this, NULL, NULL);
	if (NULL == m_hThread)
	{
		ulRes = GetLastError();
		LogErrorMsg(_T("Create event thread failed! dwRes = 0x%x"), ulRes);
		return ulRes;		
	}

	m_bEndThread = FALSE;

	return USBKEY_ERR_OK;
}

ULONG CZJCAUSBKeyEventThread::Stop()
{
	ULONG ulWaitRes = 0;

	if (NULL == m_hThread)
	{
		return USBKEY_ERR_OK;
	}

	if (!m_pUsbKey)
	{
		LogErrorMsg(_T("pUsbKey is NULL!"));
		return USBKEY_ERR_INVALIDCALL;
	}

	m_bEndThread = TRUE;
	m_pUsbKey->CancelWaitForDevEvent();	
	ulWaitRes = WaitForSingleObject(m_hThread, 3000);
	if (WAIT_OBJECT_0 != ulWaitRes)
	{
		TerminateThread(m_hThread, 0);
	}

	CloseHandle(m_hThread);
	m_hThread = NULL;

	return USBKEY_ERR_OK;
}

ULONG WINAPI CZJCAUSBKeyEventThread::ThreadProc(LPVOID lpParam)
{
	ULONG ulRes = 0;
	ULONG ulSoltID = 0;
	ULONG ulNameLen = 128;
	ULONG ulEventType = 0;
	CHAR csDevName[128] = {0};

	CZJCAUSBKeyEventThread* pEventThread = (CZJCAUSBKeyEventThread*)lpParam;
	if (NULL == pEventThread)
	{
		return 0;
	}

	USES_CONVERSION;
	while(!pEventThread->m_bEndThread)
	{
		ulNameLen = 128;
		memset(csDevName, 0, 128);
		ulRes = pEventThread->m_pUsbKey->WaitForDevEvent(pEventThread->m_bNotBlock,
														 &ulSoltID,
														 csDevName,
														 &ulNameLen,
														 &ulEventType);
		if (0 == ulRes && (-1 != ulSoltID || ulNameLen > 0) )
		{
#ifdef UNICODE
			pEventThread->m_pEventCallback(ulSoltID, A2W(csDevName), ulEventType, pEventThread->m_pCaller);
#else
			pEventThread->m_pEventCallback(ulSoltID, csDevName, ulEventType, pEventThread->m_pCaller);
#endif	//UNICODE
		}
	}

	return 0;
}