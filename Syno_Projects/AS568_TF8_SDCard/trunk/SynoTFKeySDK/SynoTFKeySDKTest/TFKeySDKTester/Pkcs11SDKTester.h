/***************************************************************************
 *	File name:Pkcs11SDKTester.h
 *	Introduce:Syno PKCS11 SDK tester class definition
 *	Author:Syno common
 *	Date:2015/11/07
 *  Version:v1.0
 **************************************************************************/
#ifndef _PKCS11_SDK_TESTER_H_
#define _PKCS11_SDK_TESTER_H_

#include "ISDKTester.h"
#include "../ThirdParty/pkcs11/pkcs11.h"

class CPkcs11SDKTester : public ISDKTester
{
public:
	CPkcs11SDKTester(void);
	virtual ~CPkcs11SDKTester(void);
private:
	HINSTANCE				m_hInstance;
	TCHAR					m_tcDLLFile[256];
	CK_ULONG				m_ulSlotIDCnt;
	CK_SLOT_ID_PTR			m_pSlotIDList;
	CK_FUNCTION_LIST_PTR	m_pFunctionList;
	//CK_SESSION_HANDLE		m_hSession;
	//
	FuncMsgCallback			m_pMsgCallbackFunc;
	LPVOID					m_pCallbackParam;
private:
	ULONG	_FindSlotIDByDevName(LPCTSTR lpcsDevName, ULONG &ulSlotID);
	CK_OBJECT_HANDLE _FindPubKeyObj(CK_SESSION_HANDLE hSession, BOOL bIsExchKey, ULONG ckaType, LPBYTE data, ULONG size);
	CK_OBJECT_HANDLE _FindPriKeyObj(CK_SESSION_HANDLE hSession, BOOL bIsSignKey, ULONG ckaType, LPBYTE data, ULONG size);
	CK_OBJECT_HANDLE _FindDataFileObj(CK_SESSION_HANDLE hSession, LPCSTR lpcsFileName);
	ULONG _GetHashValue(CK_SESSION_HANDLE hSession, ULONG ulCkm, LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpHashValue, ULONG* pHashLen);
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
#endif	//_PKCS11_SDK_TESTER_H_