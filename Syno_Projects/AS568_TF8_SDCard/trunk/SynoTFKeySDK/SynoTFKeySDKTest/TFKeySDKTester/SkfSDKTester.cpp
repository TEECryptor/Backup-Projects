#include "StdAfx.h"
#include "SkfSDKTester.h"
#include "Defs.h"

CSkfSDKTester::CSkfSDKTester(void)
 : m_hInstance(NULL)
 , m_hApplication(NULL)
 , m_pMsgCallbackFunc(NULL)
 , m_pCallbackParam(NULL)
{
}

CSkfSDKTester::~CSkfSDKTester(void)
{
}

// ����SDK DLL�ļ�
ULONG CSkfSDKTester::LoadDll(LPCTSTR lpcsCspOrFile, 
							 FuncMsgCallback pMsgFunc, 
							 LPVOID lpParam)
{
	return 0;
}

// ж��DLL�ļ�
ULONG CSkfSDKTester::UnloadDll()
{
	return USBKEY_ERR_FAILED;
}

// ö���豸
ULONG CSkfSDKTester::EnumDev(LPTSTR lpsDevName, 
							 ULONG *pulSize)
{
	return USBKEY_ERR_FAILED;
}

// ��ȡ�豸��Ϣ
ULONG CSkfSDKTester::GetDevInfo(LPCTSTR lpcsDevName)
{
	return USBKEY_ERR_FAILED;
}

// ��ʼ���豸
ULONG CSkfSDKTester::Initialize(LPCTSTR lpcsDevName,
								LPCTSTR lpcsAppName, 
								LPCTSTR lpcsLabel, 
								LPCTSTR lpcsSOPIN, 
								ULONG ulSOPINRetryTime, 
								LPCTSTR lpcsUserPIN, 
								ULONG ulUserPINRetryTime)
{
	return USBKEY_ERR_FAILED;
}

// ���ñ�ǩ
ULONG CSkfSDKTester::SetLabel(LPCTSTR lpcsDevName,
							  LPCTSTR lpcsLabel)
{
	return USBKEY_ERR_FAILED;
}

// �豸��֤
ULONG CSkfSDKTester::DevAuth(LPCTSTR lpcsDevName, 
							 LPBYTE lpAuthKey, 
							 ULONG ulKeyLen)
{
	return USBKEY_ERR_FAILED;
}

// �޸��豸��֤Key
ULONG CSkfSDKTester::ChangeAuthKey(LPCTSTR lpcsDevName, 
								   LPBYTE lpNewAuthKey, 
								   ULONG ulKeyLen)
{
	return USBKEY_ERR_FAILED;
}

// �򿪻Ự
ULONG CSkfSDKTester::OpenSession(LPCTSTR lpcsDevName, 
								 ULONG *phSession)
{
	return USBKEY_ERR_FAILED;
}

// ��ȡ�Ự��Ϣ
ULONG CSkfSDKTester::GetSessionInfo(ULONG hSession)
{
	return USBKEY_ERR_FAILED;
}

// �رջỰ
ULONG CSkfSDKTester::CloseSession(ULONG hSession)
{
	return USBKEY_ERR_FAILED;
}

// ��¼�Ự
ULONG CSkfSDKTester::LoginSession(ULONG hSession, 
								  ULONG ulUserType, 
								  LPCTSTR lpcsPIN, 
								  ULONG* pulLeftTimes)
{
	return USBKEY_ERR_FAILED;
}

// �ǳ��Ự
ULONG CSkfSDKTester::LogOutSession(ULONG hSession)
{
	return USBKEY_ERR_FAILED;
}

// �޸�PIN��
ULONG CSkfSDKTester::ChangePin(ULONG hSession, 
							   ULONG ulUserType, 
							   LPCTSTR lpcsOldPIN, 
							   LPCTSTR lpcsNewPIN, 
							   ULONG* pulLeftTimes)
{
	return USBKEY_ERR_FAILED;
}

// �����û�PIN
ULONG CSkfSDKTester::UnlockPin(ULONG hSession, 
							   LPCTSTR lpcsSOPIN, 
							   LPCTSTR lpcsNewUserPIN, 
							   ULONG* pulLeftTimes)
{
	return USBKEY_ERR_FAILED;
}

// ö���ļ�
ULONG CSkfSDKTester::EnumFiles(ULONG hSession, 
							   LPTSTR lpFilesName, 
							   ULONG* pulLen)
{
	return USBKEY_ERR_FAILED;
}

// �����ļ�
ULONG CSkfSDKTester::CreateFile(ULONG hSession, 
								LPTSTR lpFilesName, 
								ULONG ulSize)
{
	return USBKEY_ERR_FAILED;
}

// ɾ���ļ�
ULONG CSkfSDKTester::DeleteFile(ULONG hSession, 
								LPTSTR lpFilesName)
{
	return USBKEY_ERR_FAILED;
}

// д�ļ�
ULONG CSkfSDKTester::WriteFile(ULONG hSession, 
							   LPTSTR lpFilesName, 
							   LPBYTE lpData, 
							   ULONG ulLen)
{
	return USBKEY_ERR_FAILED;
}

// ���ļ�
ULONG CSkfSDKTester::ReadFile(ULONG hSession, 
							  LPTSTR lpFilesName, 
							  LPBYTE lpData, 
							  ULONG* pulLen)
{
	return USBKEY_ERR_FAILED;
}

// ����֤��
ULONG CSkfSDKTester::ImportCert(ULONG hSession, 
								LPCTSTR lpContainer, 
								ULONG ulCertAlg, 
								ULONG ulCertUsage, 
								LPBYTE lpCert, 
								ULONG ulLen)
{
	return USBKEY_ERR_FAILED;
}

// ����֤��
ULONG CSkfSDKTester::ExportCert(ULONG hSession, 
								ULONG ulCertAlg, 
								ULONG ulCertUsage, 
								LPBYTE lpCert, 
								ULONG* pulLen)
{
	return USBKEY_ERR_FAILED;
}

// ���ɶԳ���Կ
ULONG CSkfSDKTester::GenerateSessionKey(ULONG hSession, 
										ULONG ulAlgID, 
										ULONG* phSessionKey)
{
	return USBKEY_ERR_FAILED;
}

// ���ٶԳ���Կ
ULONG CSkfSDKTester::CloseSessionKey(ULONG hSession, 
									 ULONG hSessionKey)
{
	return USBKEY_ERR_FAILED;
}

// ����ǩ����Կ��
ULONG CSkfSDKTester::GenerateKeyPair(ULONG hSession,
									 ULONG ulAlgID, 
									 ULONG ulBits)
{
	return USBKEY_ERR_FAILED;
}

// ���������Կ��
ULONG CSkfSDKTester::ImportKeyPair(ULONG hSession,
								   ULONG ulAlgID, 
								   LPBYTE lpKeyPair, 
								   ULONG ulKeyPairLen)
{
	return USBKEY_ERR_FAILED;
}

// ������Կ����
ULONG CSkfSDKTester::ExportPublicKey(ULONG hSession, 
									 ULONG ulAlgID, 
									 BOOL bSign, 
									 LPBYTE lpPubKey, 
									 ULONG *pulLen)
{
	return USBKEY_ERR_FAILED;
}

// ���Լ�������-��������
ULONG CSkfSDKTester::TestEncrypt(ULONG hSession, 
								 ULONG ulAlgID, 
								 LPCTSTR lpcsMsg)
{
	return USBKEY_ERR_OK;
}

// ���Դ�������ŷ�����-���������ŷ�
ULONG CSkfSDKTester::TestEnvelop(ULONG hSession, 
								 ULONG ulAlgID, 
								 LPCTSTR lpcsMsg)
{
	return USBKEY_ERR_OK;
}

// ����ǩ����ǩ
ULONG CSkfSDKTester::TestSign(ULONG hSession, 
							  ULONG ulAsymID, 
							  ULONG ulHashID,  
							  LPCTSTR lpcsMsg)
{
	return USBKEY_ERR_OK;
}


ULONG CSkfSDKTester::WaitForDevEvent(LPTSTR szDevName, 
									 ULONG *pulDevNameLen, 
									 ULONG *pulSlotID, 
									 ULONG *pulEvent)
{
	return USBKEY_ERR_OK;
}

ULONG CSkfSDKTester::CancelWaitForDevEvent()
{
	return USBKEY_ERR_OK;
}
