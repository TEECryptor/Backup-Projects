/******************************************************
*	File Name: ZJCAUSBKeyTestThread.h
*	Author: Youfang Yao
*	Date: 14/08/2015
*	Introduce: The class CZJCAUSBKeyTestThread definition file
******************************************************/

#ifndef _ZJCA_USBKEY_TEST_THREAD_H_
#define _ZJCA_USBKEY_TEST_THREAD_H_

#include "../Include/IUSBKeyApi.h"
#include "../Include/Commondef.h"


#define	WM_TESTTHREAD_START		0x01
#define	WM_TESTTHREAD_ABORT		0x02
#define	WM_TESTTHREAD_ENVELOP	0x03
#define	WM_TESTTHREAD_DEVELOP	0x04
#define	WM_TESTTHREAD_ENCRYPT	0x05
#define	WM_TESTTHREAD_DECRYPT	0x06
#define	WM_TESTTHREAD_SIGNP7	0x07
#define	WM_TESTTHREAD_VERIFYP7	0x08
#define	WM_TESTTHREAD_SIGNP1	0x09
#define	WM_TESTTHREAD_VERIFYP1	0x0A
#define	WM_TESTTHREAD_END		0xFF


class CZJCAUSBKeyTestThread
{
public:
	CZJCAUSBKeyTestThread(void);
	~CZJCAUSBKeyTestThread(void);

public:
	ULONG	Start();
	ULONG	Stop();
	ULONG	TestEnvelop(IUSBKeyApi* pUsbKey, LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG *pulOutLen);
	ULONG	TestDevelop(IUSBKeyApi* pUsbKey, LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG *pulOutLen);
	ULONG	TestEncrypt(IUSBKeyApi* pUsbKey, LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen);
	ULONG	TestDecrypt(IUSBKeyApi* pUsbKey, LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen);
	ULONG	TestSignP7(IUSBKeyApi* pUsbKey, LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG* pulSignLen);
	ULONG	TestVerifyP7(IUSBKeyApi* pUsbKey, LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG ulSignLen);
	ULONG	TestSignP1(IUSBKeyApi* pUsbKey, LPBYTE lpOrgData, ULONG ulOrgLen, LPSTR lpUserID, LPBYTE lpSignature, ULONG* pulSignLen);
	ULONG	TestVerifyP1(IUSBKeyApi* pUsbKey, LPBYTE lpOrgData, ULONG ulOrgLen, LPSTR lpUserID, LPBYTE lpSignature, ULONG ulSignLen);
public:
	ULONG	ThreadProc();
private:
	HANDLE	m_hThread;
	HANDLE	m_hThreadEvent;
	ULONG	m_ulThreadID;
	//
	IUSBKeyApi*	m_pUSBKeyApi;
	LPBYTE		m_lpInData;
	LPBYTE		m_lpOutData;
	ULONG		m_ulInLen;
	ULONG*		m_pulOutLen;
	ULONG		m_ulTestRes;
	CHAR		m_csUserID[32];
};

#endif	//_ZJCA_USBKEY_TEST_THREAD_H_

