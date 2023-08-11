/******************************************************
*	File Name: IUSBKeyCSP.h
*	Author: Youfang Yao
*	Date: 13/08/2015
*	Introduce: The interface definition for CSP API
******************************************************/
#ifndef _USBKEY_CSP_H_
#define _USBKEY_CSP_H_

class IUSBKeyCSP
{
public:	
	virtual ULONG Initialize(LPCSTR lpcsCSPName, ULONG ulAsymmAlg, ULONG ulSymmAlg, ULONG ulHashAlg) = 0;
	virtual ULONG Envelop(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen) = 0;
	virtual ULONG Develop(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen) = 0;
	virtual ULONG Encrypt(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen) = 0;
	virtual ULONG Decrypt(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen) = 0;
	virtual ULONG SignP7(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG* pulSignLen) = 0;
	virtual ULONG VerifyP7(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG ulSignLen) = 0;
	virtual ULONG SignP1(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG* pulSignLen) = 0;
	virtual ULONG VerifyP1(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG ulSignLen) = 0;
	virtual ULONG UnInitialize(void) = 0;
};
#endif	//_USBKEY_CSP_H_