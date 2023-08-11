/***************************************************************************
 *	File name:ISDKTester.h
 *	Introduce:Syno SDK tester interface definition
 *	Author:Syno common
 *	Date:2015/11/07
 *  Version:v1.0
 **************************************************************************/
#ifndef _ISDK_TESTER_H_
#define _ISDK_TESTER_H_

#include "Defs.h"

typedef ULONG (CALLBACK *FuncMsgCallback)(LPCTSTR lpcsResultMsg, LPVOID lpCaller);

class ISDKTester
{
public:	
	virtual ULONG LoadDll(LPCTSTR lpcsCspOrFile, FuncMsgCallback pFunc, LPVOID lpParam) = 0;
	virtual ULONG UnloadDll() = 0;
	virtual ULONG EnumDev(LPTSTR lpsDevName, ULONG *pulSize) = 0;
	virtual ULONG GetDevInfo(LPCTSTR lpcsDevName) = 0;
	virtual ULONG Initialize(LPCTSTR lpcsDevName, LPCTSTR lpcsLabel, LPCTSTR lpcsAppName, LPCTSTR lpcsSOPIN, ULONG ulSOPINRetryTime, LPCTSTR lpcsUserPIN, ULONG ulUserPINRetryTime) = 0;
	virtual ULONG SetLabel(LPCTSTR lpcsDevName, LPCTSTR lpcsLabel) = 0;
	virtual ULONG DevAuth(LPCTSTR lpcsDevName, LPBYTE lpAuthKey, ULONG ulKeyLen) = 0;
	virtual ULONG ChangeAuthKey(LPCTSTR lpcsDevName, LPBYTE lpNewAuthKey, ULONG ulKeyLen) = 0;
	virtual ULONG OpenSession(LPCTSTR lpcsDevName, ULONG *phSession) = 0;
	virtual ULONG GetSessionInfo(ULONG hSession) = 0;
	virtual ULONG CloseSession(ULONG hSession) = 0;
	virtual ULONG LoginSession(ULONG hSession, ULONG ulUserType, LPCTSTR lpcsPIN, ULONG* pulLeftTimes) = 0;
	virtual ULONG LogOutSession(ULONG hSession) = 0;
	virtual ULONG ChangePin(ULONG hSession, ULONG ulUserType, LPCTSTR lpcsOldPIN, LPCTSTR lpcsNewPIN, ULONG* pulLeftTimes) = 0;
	virtual ULONG UnlockPin(ULONG hSession, LPCTSTR lpcsSOPIN, LPCTSTR lpcsNewUserPIN, ULONG* pulLeftTimes) = 0;
	virtual ULONG EnumFiles(ULONG hSession, LPTSTR lpFilesName, ULONG* pulLen) = 0;
	virtual ULONG CreateFile(ULONG hSession, LPTSTR lpFilesName, ULONG ulSize) = 0;
	virtual ULONG DeleteFile(ULONG hSession, LPTSTR lpFilesName) = 0;
	virtual ULONG WriteFile(ULONG hSession, LPTSTR lpFilesName, LPBYTE lpData, ULONG ulLen) = 0;
	virtual ULONG ReadFile(ULONG hSession, LPTSTR lpFilesName, LPBYTE lpData, ULONG* pulLen) = 0;
	virtual ULONG ImportCert(ULONG hSession, LPCTSTR lpContainer, ULONG ulCertAlg, ULONG ulCertUsage, LPBYTE lpCert, ULONG ulLen) = 0;
	virtual ULONG ExportCert(ULONG hSession, ULONG ulCertAlg, ULONG ulCertUsage, LPBYTE lpCert, ULONG* pulLen) = 0;
	virtual ULONG GenerateSessionKey(ULONG hSession, ULONG ulAlgID, ULONG* phSessionKey) = 0;
	virtual ULONG CloseSessionKey(ULONG hSession, ULONG hSessionKey) = 0;
	virtual ULONG GenerateKeyPair(ULONG hSession, ULONG ulAlgID, ULONG ulBits) = 0;
	virtual ULONG ImportKeyPair(ULONG hSession, ULONG ulAlgID, LPBYTE lpKeyPair, ULONG ulKeyPairLen) = 0;
	virtual ULONG ExportPublicKey(ULONG hSession, ULONG ulAlgID, BOOL bSign, LPBYTE lpPubKey, ULONG *pulLen) = 0;
	virtual ULONG TestEncrypt(ULONG hSession, ULONG ulAlgID, LPCTSTR lpcsMsg) = 0;
	virtual ULONG TestEnvelop(ULONG hSession, ULONG ulAlgID, LPCTSTR lpcsMsg) = 0;
	virtual ULONG TestSign(ULONG hSession, ULONG ulAsymID, ULONG ulHashID, LPCTSTR lpcsMsg) = 0;
	virtual ULONG WaitForDevEvent(LPTSTR szDevName, ULONG *pulDevNameLen, ULONG *pulSlotID, ULONG *pulEvent) = 0;
	virtual ULONG CancelWaitForDevEvent() = 0;
};

#endif	//_ISDK_TESTER_H_
