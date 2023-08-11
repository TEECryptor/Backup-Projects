/***************************************************************************
 *	File name:SkfSDKTester.h
 *	Introduce:Syno SKF SDK tester class definition
 *	Author:Syno common
 *	Date:2015/11/07
 *  Version:v1.0
 **************************************************************************/
#ifndef _SKF_SDK_TESTER_H_
#define _SKF_SDK_TESTER_H_

#include "ISDKTester.h"

class CSkfSDKTester : public ISDKTester
{
public:
	CSkfSDKTester(void);
	virtual ~CSkfSDKTester(void);
private:
	HINSTANCE				m_hInstance;
	HANDLE					m_hApplication;
	//
	FuncMsgCallback			m_pMsgCallbackFunc;
	LPVOID					m_pCallbackParam;
public:
	ULONG LoadDll(LPCTSTR lpcsCspOrFile, FuncMsgCallback pMsgFunc, LPVOID lpParam);
	ULONG UnloadDll();
	ULONG EnumDev(LPTSTR lpsDevName, ULONG *pulSize);
	ULONG GetDevInfo(LPCTSTR lpcsDevName);
	ULONG Initialize(LPCTSTR lpcsDevName, LPCTSTR lpcsLabel, LPCTSTR lpcsAppName, LPCTSTR lpcsSOPIN, ULONG ulSOPINRetryTime, LPCTSTR lpcsUserPIN, ULONG ulUserPINRetryTime);
	ULONG SetLabel(LPCTSTR lpcsDevName, LPCTSTR lpcsLabel);
	ULONG DevAuth(LPCTSTR lpcsDevName, LPBYTE lpAuthKey, ULONG ulKeyLen);
	ULONG ChangeAuthKey(LPCTSTR lpcsDevName, LPBYTE lpNewAuthKey, ULONG ulKeyLen);	
	ULONG OpenSession(LPCTSTR lpcsDevName, ULONG *phSession);
	ULONG GetSessionInfo(ULONG hSession);
	ULONG CloseSession(ULONG hSession);
	ULONG LoginSession(ULONG hSession, ULONG ulUserType, LPCTSTR lpcsPIN, ULONG* pulLeftTimes);
	ULONG LogOutSession(ULONG hSession);
	ULONG ChangePin(ULONG hSession, ULONG ulUserType, LPCTSTR lpcsOldPIN, LPCTSTR lpcsNewPIN, ULONG* pulLeftTimes);
	ULONG UnlockPin(ULONG hSession, LPCTSTR lpcsSOPIN, LPCTSTR lpcsNewUserPIN, ULONG* pulLeftTimes);
	ULONG EnumFiles(ULONG hSession, LPTSTR lpFilesName, ULONG* pulLen);
	ULONG CreateFile(ULONG hSession, LPTSTR lpFilesName, ULONG ulSize);
	ULONG DeleteFile(ULONG hSession, LPTSTR lpFilesName);
	ULONG WriteFile(ULONG hSession, LPTSTR lpFilesName, LPBYTE lpData, ULONG ulLen);
	ULONG ReadFile(ULONG hSession, LPTSTR lpFilesName, LPBYTE lpData, ULONG* pulLen);
	ULONG ImportCert(ULONG hSession, LPCTSTR lpContainer, ULONG ulCertAlg, ULONG ulCertUsage, LPBYTE lpCert, ULONG ulLen);
	ULONG ExportCert(ULONG hSession, ULONG ulCertAlg, ULONG ulCertUsage, LPBYTE lpCert, ULONG* pulLen);
	ULONG GenerateSessionKey(ULONG hSession, ULONG ulAlgID, ULONG* phSessionKey);
	ULONG CloseSessionKey(ULONG hSession, ULONG hSessionKey);
	ULONG GenerateKeyPair(ULONG hSession, ULONG ulAlgID, ULONG ulBits);
	ULONG ImportKeyPair(ULONG hSession, ULONG ulAlgID, LPBYTE lpKeyPair, ULONG ulKeyPairLen);
	ULONG ExportPublicKey(ULONG hSession, ULONG ulAlgID, BOOL bSign, LPBYTE lpPubKey, ULONG *pulLen);
	ULONG TestEncrypt(ULONG hSession, ULONG ulAlgID, LPCTSTR lpcsMsg);
	ULONG TestEnvelop(ULONG hSession, ULONG ulAlgID, LPCTSTR lpcsMsg);
	ULONG TestSign(ULONG hSession, ULONG ulAsymID, ULONG ulHashID, LPCTSTR lpcsMsg);
	ULONG WaitForDevEvent(LPTSTR szDevName, ULONG *pulDevNameLen, ULONG *pulSlotID, ULONG *pulEvent);
	ULONG CancelWaitForDevEvent();
};
#endif	//_SKF_SDK_TESTER_H_