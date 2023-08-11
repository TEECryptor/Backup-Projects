/******************************************************
*	File Name: IUSBKeyApi.h
*	Author: Youfang Yao
*	Date: 13/08/2015
*	Introduce: The interface definition for USBKey API
******************************************************/
#ifndef _IUSBKEY_API_H_
#define _IUSBKEY_API_H_

class IUSBKeyApi
{
public:	
	virtual ULONG Create(LPCSTR lpcsCspOrFile) = 0;
	virtual ULONG Initialize(ULONG ulAsymmAlg, ULONG ulSymmAlg, ULONG ulHashAlg, LPSTR lpUserPIN) = 0;
	virtual ULONG WaitForDevEvent(BOOL bNotBlock, ULONG *pulSoltID, LPSTR lpDevName, ULONG *pulNameLen, ULONG *pulEventType) = 0;
	virtual ULONG CancelWaitForDevEvent() = 0;
	virtual ULONG ConnectDev() = 0;
	virtual ULONG Envelop(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen) = 0;
	virtual ULONG Develop(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen) = 0;
	virtual ULONG Encrypt(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen) = 0;
	virtual ULONG Decrypt(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen) = 0;
	virtual ULONG SignP7(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG* pulSignLen) = 0;
	virtual ULONG VerifyP7(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG ulSignLen) = 0;
	virtual ULONG SignP1(LPBYTE lpOrgData, ULONG ulOrgLen, LPSTR lpUserID, LPBYTE lpSignature, ULONG* pulSignLen) = 0;
	virtual ULONG VerifyP1(LPBYTE lpOrgData, ULONG ulOrgLen, LPSTR lpUserID, LPBYTE lpSignature, ULONG ulSignLen) = 0;
	virtual ULONG DisconnectDev(void) = 0;
	virtual ULONG Release(void) = 0;
};
#endif	//_IUSBKEY_API_H_