/******************************************************
*	File Name: ZJCAUSBKeyEventThread.h
*	Author: Youfang Yao
*	Date: 14/08/2015
*	Introduce: The class CZJCAUSBKeyEventThread definition file
******************************************************/

#ifndef _ZJCA_USBKEY_EVENT_THREAD_H_
#define _ZJCA_USBKEY_EVENT_THREAD_H_

#include "../Include/IUSBKeyApi.h"

typedef ULONG (CALLBACK *USBKeyEventCallback)(ULONG ulSoltID, LPTSTR lpDevName, ULONG ulEventType, LPVOID lpCaller);

class CZJCAUSBKeyEventThread
{
public:
	CZJCAUSBKeyEventThread(void);
	~CZJCAUSBKeyEventThread(void);
public:
	ULONG	Start(IUSBKeyApi* pUsbKey, BOOL bNotBlock, LPVOID lpCallback, LPVOID lpCaller);
	ULONG	Stop();
private:
	static ULONG WINAPI ThreadProc(LPVOID lpParam);
private:
	HANDLE				m_hThread;
	ULONG				m_ulThreadID;
	BOOL				m_bEndThread;
	BOOL				m_bNotBlock;
	IUSBKeyApi*			m_pUsbKey;
	USBKeyEventCallback	m_pEventCallback;
	LPVOID				m_pCaller;
};
#endif	//_ZJCA_USBKEY_EVENT_THREAD_H_
