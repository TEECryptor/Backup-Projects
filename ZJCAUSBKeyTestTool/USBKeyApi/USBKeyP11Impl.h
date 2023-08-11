/******************************************************
*	File Name: USBKeyP11Imp.h
*	Author: Youfang Yao
*	Date: 13/08/2015
*	Introduce: The class CUSBKeyP11Impl definition file
******************************************************/

#ifndef _USBKEY_P11_IMPL_H_
#define _USBKEY_P11_IMPL_H_

#include "../Include/IUSBKeyApi.h"
#include "../Include/CommonDef.h"
#include "./ThirdParty/pkcs11/pkcs11.h"

class CUSBKeyP11Impl :	public IUSBKeyApi
{
public:
	CUSBKeyP11Impl(void);
	virtual ~CUSBKeyP11Impl(void);

public:	
	virtual ULONG Create(LPCSTR lpcsCspOrFile);
	virtual ULONG Initialize(ULONG ulAsymmAlg, ULONG ulSymmAlg, ULONG ulHashAlg, LPSTR lpUserPIN);
	virtual ULONG WaitForDevEvent(BOOL bNotBlock, ULONG *pulSoltID, LPSTR lpDevName, ULONG *pulNameLen, ULONG *pulEventType);
	virtual ULONG CancelWaitForDevEvent();
	virtual ULONG ConnectDev();
	virtual ULONG Envelop(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen);
	virtual ULONG Develop(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen);
	virtual ULONG Encrypt(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen);
	virtual ULONG Decrypt(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen);
	virtual ULONG SignP7(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG* pulSignLen);
	virtual ULONG VerifyP7(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG ulSignLen);
	virtual ULONG SignP1(LPBYTE lpOrgData, ULONG ulOrgLen, LPSTR lpUserID, LPBYTE lpSignature, ULONG* pulSignLen);
	virtual ULONG VerifyP1(LPBYTE lpOrgData, ULONG ulOrgLen, LPSTR lpUserID, LPBYTE lpSignature, ULONG ulSignLen);
	virtual ULONG DisconnectDev(void);
	virtual ULONG Release(void);

private:
	CK_OBJECT_HANDLE _FindPubKeyObj(CK_SESSION_HANDLE hSession, BOOL bIsExchKey, ULONG ckaType, LPBYTE data, ULONG size);
	CK_OBJECT_HANDLE _FindPriKeyObj(CK_SESSION_HANDLE hSession, BOOL bIsSignKey, ULONG ckaType, LPBYTE data, ULONG size);
	ULONG _GetHashValue(CK_SESSION_HANDLE hSession, ULONG ulCkm, LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpHashValue, ULONG* pHashLen);
	
private:
	HINSTANCE	m_hInstance;
	CHAR		m_csDLLFile[256];	
	CHAR		m_csUserPIN[64];
	ULONG		m_ulAsymmAlg;
	ULONG		m_ulSymmAlg;
	ULONG		m_ulHashAlg;
	ULONG		m_ulSymmValueLen;
	CK_SESSION_HANDLE		m_hSession;
	CK_FUNCTION_LIST_PTR	m_pFunctionList;
};
#endif	//_USBKEY_P11_IMPL_H_
