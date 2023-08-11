#include "stdafx.h"
#include "ZJCAUSBKeyTestThread.h"


DWORD WINAPI TestThreadProc(LPVOID lpParam)
{
	CZJCAUSBKeyTestThread *pCall = (CZJCAUSBKeyTestThread*)lpParam;
	return pCall->ThreadProc();
}

ULONG WaitFor(HANDLE hEvent)
{
	MSG msg;

	while (WAIT_TIMEOUT == WaitForSingleObject(hEvent, 20))
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

CZJCAUSBKeyTestThread::CZJCAUSBKeyTestThread(void)
	: m_hThread(NULL)
	, m_hThreadEvent(NULL)
	, m_ulThreadID(0)
	, m_pUSBKeyApi(NULL)
	, m_lpInData(NULL)
	, m_lpOutData(NULL)
	, m_ulInLen(0)
	, m_pulOutLen(NULL)
{
}

CZJCAUSBKeyTestThread::~CZJCAUSBKeyTestThread(void)
{
	Stop();
}

ULONG CZJCAUSBKeyTestThread::Start()
{
	ULONG ulRes = 0;
	ULONG ulWaitResult = 0;
	
	//	线程已经启动
	if (m_hThread)
	{
		return 0;
	}
	
	//	创建线程事件句柄
	m_hThreadEvent = CreateEvent(NULL, TRUE, FALSE, _T("ThreadEvent") );
	if (!m_hThreadEvent)
	{
		ulRes = GetLastError();
		LogErrorMsg(_T("CreateEvent() failed! dwError=0x%x"), ulRes);
		goto RETURN_FALSE;
	}
		
	//	创建线程
	m_hThread = CreateThread(NULL, NULL, TestThreadProc, this, NULL, &m_ulThreadID);
	if (!m_hThread)
	{
		ulRes = GetLastError();
		LogErrorMsg(_T("CreateThread() failed! dwError=0x%x"), ulRes);
		goto RETURN_FALSE;
	}
	
	//	等待线程启动完成
	ulWaitResult = WaitForSingleObject(m_hThreadEvent, 5000);
	if (WAIT_OBJECT_0 != ulWaitResult)
	{
		ulRes = GetLastError();
		LogErrorMsg(_T("WaitForSingleObject() failed! dwError=0x%x"), ulRes);
		goto RETURN_FALSE;
	}
	goto RETURN_TRUE;

	//	遇到错误，清除对象返回FALSE
RETURN_FALSE:
	if (m_hThreadEvent)
	{
		CloseHandle(m_hThreadEvent);
		m_hThreadEvent = NULL;
	}
	if (m_hThread)
	{
		TerminateThread(m_hThread, 0);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	return ulRes;

	//	创建成功
RETURN_TRUE:	

	return 0;
}

ULONG CZJCAUSBKeyTestThread::Stop()
{
	ULONG ulWaitResult = 0;
	
	if (!m_hThread && !m_hThreadEvent)
	{
		return 0;
	}
	
	//	向线程发送退出命令
	PostThreadMessage(m_ulThreadID, WM_TESTTHREAD_END, 0, 0);

	//	等待线程完全退出
	ulWaitResult = WaitForSingleObject(m_hThread, 5000);

	//	销毁对象
	if (m_hThreadEvent)
	{
		CloseHandle(m_hThreadEvent);
		m_hThreadEvent = NULL;
	}
	if (m_hThread)
	{
		TerminateThread(m_hThread, 0);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	return 0;
}

ULONG CZJCAUSBKeyTestThread::TestEnvelop(IUSBKeyApi* pUsbKey, 
										 LPBYTE lpInData, 
										 ULONG ulInLen, 
										 LPBYTE lpOutData, 
										 ULONG *pulOutLen)
{
	ULONG ulRes = 0;

	if (!pUsbKey)
	{
		LogErrorMsg(_T("pUsbKey is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpInData || ulInLen == 0)
	{
		LogErrorMsg(_T("lpInData is NULL or ulInLen is 0!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpOutData)
	{
		LogErrorMsg(_T("lpOutData is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!pulOutLen || *pulOutLen < (ulInLen + 2048))
	{
		LogErrorMsg(_T("pulOutLen is NULL or *pulOutLen is too small!"));
		return USBKEY_ERR_INVALIDPARAM;
	}

	m_pUSBKeyApi = pUsbKey;
	m_lpInData = lpInData;
	m_lpOutData = lpOutData;
	m_ulInLen = ulInLen;
	m_pulOutLen = pulOutLen;

	ResetEvent(m_hThreadEvent);
	PostThreadMessage(m_ulThreadID, WM_TESTTHREAD_ENVELOP, 0, 0);
	WaitFor(m_hThreadEvent);

	return m_ulTestRes;
}

ULONG CZJCAUSBKeyTestThread::TestDevelop(IUSBKeyApi* pUsbKey, 
										 LPBYTE lpInData, 
										 ULONG ulInLen, 
										 LPBYTE lpOutData,
										 ULONG *pulOutLen)
{
	ULONG ulRes = 0;

	if (!pUsbKey)
	{
		LogErrorMsg(_T("pUsbKey is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpInData || ulInLen == 0)
	{
		LogErrorMsg(_T("lpInData is NULL or ulInLen is 0!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpOutData)
	{
		LogErrorMsg(_T("lpOutData is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!pulOutLen || *pulOutLen < ulInLen)
	{
		LogErrorMsg(_T("pulOutLen is NULL or *pulOutLen is too small!"));
		return USBKEY_ERR_INVALIDPARAM;
	}	

	m_pUSBKeyApi = pUsbKey;
	m_lpInData = lpInData;
	m_lpOutData = lpOutData;
	m_ulInLen = ulInLen;
	m_pulOutLen = pulOutLen;
	
	ResetEvent(m_hThreadEvent);
	PostThreadMessage(m_ulThreadID, WM_TESTTHREAD_DEVELOP, 0, 0);
	WaitFor(m_hThreadEvent);

	return m_ulTestRes;
}

ULONG CZJCAUSBKeyTestThread::TestEncrypt(IUSBKeyApi* pUsbKey, 
										 LPBYTE lpInData, 
										 ULONG ulInLen, 
										 LPBYTE lpOutData, 
										 ULONG* pulOutLen)
{
	ULONG ulRes = 0;
	
	if (!pUsbKey)
	{
		LogErrorMsg(_T("pUsbKey is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpInData || ulInLen == 0)
	{
		LogErrorMsg(_T("lpInData is NULL or ulInLen is 0!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpOutData)
	{
		LogErrorMsg(_T("lpOutData is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!pulOutLen || *pulOutLen < ulInLen)
	{
		LogErrorMsg(_T("pulOutLen is NULL or *pulOutLen is too small!"));
		return USBKEY_ERR_INVALIDPARAM;
	}

	m_pUSBKeyApi = pUsbKey;
	m_lpInData = lpInData;
	m_lpOutData = lpOutData;
	m_ulInLen = ulInLen;
	m_pulOutLen = pulOutLen;
		
	ResetEvent(m_hThreadEvent);
	PostThreadMessage(m_ulThreadID, WM_TESTTHREAD_ENCRYPT, 0, 0);
	WaitFor(m_hThreadEvent);

	return m_ulTestRes;
}

ULONG CZJCAUSBKeyTestThread::TestDecrypt(IUSBKeyApi* pUsbKey, 
										 LPBYTE lpInData, 
										 ULONG ulInLen, 
										 LPBYTE lpOutData, 
										 ULONG* pulOutLen)
{
	ULONG ulRes = 0;
	
	if (!pUsbKey)
	{
		LogErrorMsg(_T("pUsbKey is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpInData || ulInLen == 0)
	{
		LogErrorMsg(_T("lpInData is NULL or ulInLen is 0!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpOutData)
	{
		LogErrorMsg(_T("lpOutData is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!pulOutLen || *pulOutLen < ulInLen)
	{
		LogErrorMsg(_T("pulOutLen is NULL or *pulOutLen is too small!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	m_pUSBKeyApi = pUsbKey;
	m_lpInData = lpInData;
	m_lpOutData = lpOutData;
	m_ulInLen = ulInLen;
	m_pulOutLen = pulOutLen;
		
	ResetEvent(m_hThreadEvent);
	PostThreadMessage(m_ulThreadID, WM_TESTTHREAD_DECRYPT, 0, 0);
	WaitFor(m_hThreadEvent);

	return m_ulTestRes;
}

ULONG CZJCAUSBKeyTestThread::TestSignP7(IUSBKeyApi* pUsbKey, 
										LPBYTE lpOrgData, 
										ULONG ulOrgLen, 
										LPBYTE lpSignature, 
										ULONG* pulSignLen)
{
	ULONG ulRes = 0;
	
	if (!pUsbKey)
	{
		LogErrorMsg(_T("pUsbKey is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpOrgData || ulOrgLen == 0)
	{
		LogErrorMsg(_T("lpOrgData is NULL or ulOrgLen is 0!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpSignature)
	{
		LogErrorMsg(_T("lpSignature is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!pulSignLen || *pulSignLen < (ulOrgLen + 2048))
	{
		LogErrorMsg(_T("pulSignLen is NULL or *pulSignLen is too small!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	m_pUSBKeyApi = pUsbKey;
	m_lpInData = lpOrgData;
	m_lpOutData = lpSignature;
	m_ulInLen = ulOrgLen;
	m_pulOutLen = pulSignLen;
		
	ResetEvent(m_hThreadEvent);
	PostThreadMessage(m_ulThreadID, WM_TESTTHREAD_SIGNP7, 0, 0);
	WaitFor(m_hThreadEvent);

	return m_ulTestRes;
}

ULONG CZJCAUSBKeyTestThread::TestVerifyP7(IUSBKeyApi* pUsbKey, 
										  LPBYTE lpOrgData, 
										  ULONG ulOrgLen, 
										  LPBYTE lpSignature, 
										  ULONG ulSignLen)
{
	ULONG ulRes = 0;
	
	if (!pUsbKey)
	{
		LogErrorMsg(_T("pUsbKey is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpOrgData || ulOrgLen == 0)
	{
		LogErrorMsg(_T("lpOrgData is NULL or ulOrgLen is 0!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpSignature || ulSignLen == 0)
	{
		LogErrorMsg(_T("lpSignature is NULL or ulSignLen is 0!"));
		return USBKEY_ERR_INVALIDPARAM;
	}

	m_pUSBKeyApi = pUsbKey;
	m_lpInData = lpOrgData;
	m_lpOutData = lpSignature;
	m_ulInLen = ulOrgLen;
	m_pulOutLen = &ulSignLen;
	
	ResetEvent(m_hThreadEvent);
	PostThreadMessage(m_ulThreadID, WM_TESTTHREAD_VERIFYP7, 0, 0);
	WaitFor(m_hThreadEvent);

	return m_ulTestRes;
}

ULONG CZJCAUSBKeyTestThread::TestSignP1(IUSBKeyApi* pUsbKey, 
										LPBYTE lpOrgData, 
										ULONG ulOrgLen, 
										LPSTR lpUserID, 
										LPBYTE lpSignature, 
										ULONG* pulSignLen)
{
	ULONG ulRes = 0;
	
	if (!pUsbKey)
	{
		LogErrorMsg(_T("pUsbKey is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpOrgData || ulOrgLen == 0)
	{
		LogErrorMsg(_T("lpOrgData is NULL or ulOrgLen is 0!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpSignature)
	{
		LogErrorMsg(_T("lpSignature is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!pulSignLen || *pulSignLen < 32)
	{
		LogErrorMsg(_T("pulSignLen is NULL or *pulSignLen is too small!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (lpUserID)
	{
		strcpy_s(m_csUserID, 32, lpUserID);
	}
	else
	{
		memset(m_csUserID, 0, 32);
	}

	m_pUSBKeyApi = pUsbKey;
	m_lpInData = lpOrgData;
	m_lpOutData = lpSignature;
	m_ulInLen = ulOrgLen;
	m_pulOutLen = pulSignLen;
		
	ResetEvent(m_hThreadEvent);
	PostThreadMessage(m_ulThreadID, WM_TESTTHREAD_SIGNP1, 0, 0);
	WaitFor(m_hThreadEvent);

	return m_ulTestRes;
}

ULONG CZJCAUSBKeyTestThread::TestVerifyP1(IUSBKeyApi* pUsbKey, 
										  LPBYTE lpOrgData, 
										  ULONG ulOrgLen, 
										  LPSTR lpUserID, 
										  LPBYTE lpSignature, 
										  ULONG ulSignLen)
{
	ULONG ulRes = 0;
	
	if (!pUsbKey)
	{
		LogErrorMsg(_T("pUsbKey is NULL!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpOrgData || ulOrgLen == 0)
	{
		LogErrorMsg(_T("lpOrgData is NULL or ulOrgLen is 0!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpSignature || ulSignLen == 0)
	{
		LogErrorMsg(_T("lpSignature is NULL or ulSignLen is 0!"));
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (lpUserID)
	{
		strcpy_s(m_csUserID, 32, lpUserID);
	}
	else
	{
		memset(m_csUserID, 0, 32);
	}
	
	m_pUSBKeyApi = pUsbKey;
	m_lpInData = lpOrgData;
	m_lpOutData = lpSignature;
	m_ulInLen = ulOrgLen;
	m_pulOutLen = &ulSignLen;

	ResetEvent(m_hThreadEvent);
	PostThreadMessage(m_ulThreadID, WM_TESTTHREAD_VERIFYP1, 0, 0);
	WaitFor(m_hThreadEvent);

	return m_ulTestRes;
}

ULONG CZJCAUSBKeyTestThread::ThreadProc()
{	
	MSG msg;
	BOOL bExit = FALSE;
	DWORD dwExitCode = 0;
	
	PeekMessage(&msg, NULL, 0, 0, 0);
	SetEvent(m_hThreadEvent);
	
	while(!bExit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
			switch(msg.message )
			{
			case WM_TESTTHREAD_START:
				break;
			case WM_TESTTHREAD_ABORT:
				break;		
			case WM_TESTTHREAD_ENVELOP:
				m_ulTestRes = m_pUSBKeyApi->Envelop(m_lpInData, m_ulInLen, m_lpOutData, m_pulOutLen);
				break;	
			case WM_TESTTHREAD_DEVELOP:
				m_ulTestRes = m_pUSBKeyApi->Develop(m_lpInData, m_ulInLen, m_lpOutData, m_pulOutLen);
				break;	
			case WM_TESTTHREAD_ENCRYPT:
				m_ulTestRes = m_pUSBKeyApi->Encrypt(m_lpInData, m_ulInLen, m_lpOutData, m_pulOutLen);
				break;	
			case WM_TESTTHREAD_DECRYPT:
				m_ulTestRes = m_pUSBKeyApi->Decrypt(m_lpInData, m_ulInLen, m_lpOutData, m_pulOutLen);
				break;	
			case WM_TESTTHREAD_SIGNP7:
				m_ulTestRes = m_pUSBKeyApi->SignP7(m_lpInData, m_ulInLen, m_lpOutData, m_pulOutLen);
				break;	
			case WM_TESTTHREAD_VERIFYP7:
				m_ulTestRes = m_pUSBKeyApi->VerifyP7(m_lpInData, m_ulInLen, m_lpOutData, *m_pulOutLen);
				break;	
			case WM_TESTTHREAD_SIGNP1:
				m_ulTestRes = m_pUSBKeyApi->SignP1(m_lpInData, m_ulInLen, m_csUserID, m_lpOutData, m_pulOutLen);
				break;	
			case WM_TESTTHREAD_VERIFYP1:
				m_ulTestRes = m_pUSBKeyApi->VerifyP1(m_lpInData, m_ulInLen, m_csUserID, m_lpOutData, *m_pulOutLen);
				break;	
			case WM_TESTTHREAD_END:
				bExit = TRUE;
				break;		
			default:
				break;
			}
			SetEvent(m_hThreadEvent);
		}
		else
		{
			Sleep(50);
		}
	}
	
	return dwExitCode;
}
