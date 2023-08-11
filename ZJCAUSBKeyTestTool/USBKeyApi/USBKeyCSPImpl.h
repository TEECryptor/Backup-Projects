/******************************************************
*	File Name: USBKeyCSPImp.h
*	Author: Youfang Yao
*	Date: 13/08/2015
*	Introduce: The class CUSBKeyCSPImpl definition file
******************************************************/

#ifndef _USBKEY_CSP_IMPL_H_
#define _USBKEY_CSP_IMPL_H_

#include <wincrypt.h>
#include "../Include/IUSBKeyApi.h"
#include "../Include/CommonDef.h"

class CUSBKeyCSPImpl :	public IUSBKeyApi
{
public:
	CUSBKeyCSPImpl(void);
	virtual ~CUSBKeyCSPImpl(void);
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
	HCRYPTPROV OpenCSPProvider(ULONG ulKeyUsage);
	PCCERT_CONTEXT ExportCertificate(HCRYPTPROV hProv, BOOL bSignCert);
	
private:
	CHAR	m_csCSPName[256];
	CHAR	m_csAsymmAlgOid[128];
	CHAR	m_csUserPIN[64];
	ULONG	m_ulSymmAlg;
	ULONG	m_ulHashAlg;
};

#endif	//_USBKEY_CSP_IMPL_H_