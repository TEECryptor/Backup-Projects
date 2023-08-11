﻿/******************************************************
*	File Name: IUSBKeyP11.h
*	Author: Youfang Yao
*	Date: 13/08/2015
*	Introduce: The interface definition for P11 API
******************************************************/

#ifndef _USBKEY_P11_H_
#define _USBKEY_P11_H_

class IUSBKeyP11
{
public:	
	virtual ULONG Initialize(LPCSTR lpcsDll, ULONG ulAsymmAlg, ULONG ulSymmAlg, ULONG ulHashAlg) = 0;
	virtual ULONG WaitForDevEvent(BOOL bNotBlock, ULONG *pulSoltID, LPVOID lpRserved) = 0;
	virtual ULONG ConnectDev() = 0;
	virtual ULONG Encrypt(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen) = 0;
	virtual ULONG Decrypt(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen) = 0;
	virtual ULONG SignP1(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG* pulSignLen) = 0;
	virtual ULONG VerifyP1(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG ulSignLen) = 0;
	virtual ULONG DisconnectDev(void) = 0;	
	virtual ULONG UnInitialize(void) = 0;
};
#endif	//_USBKEY_P11_H_