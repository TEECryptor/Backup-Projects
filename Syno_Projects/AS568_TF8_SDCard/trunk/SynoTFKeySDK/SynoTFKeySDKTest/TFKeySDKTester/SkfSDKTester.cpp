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

// 加载SDK DLL文件
ULONG CSkfSDKTester::LoadDll(LPCTSTR lpcsCspOrFile, 
							 FuncMsgCallback pMsgFunc, 
							 LPVOID lpParam)
{
	return 0;
}

// 卸载DLL文件
ULONG CSkfSDKTester::UnloadDll()
{
	return USBKEY_ERR_FAILED;
}

// 枚举设备
ULONG CSkfSDKTester::EnumDev(LPTSTR lpsDevName, 
							 ULONG *pulSize)
{
	return USBKEY_ERR_FAILED;
}

// 获取设备信息
ULONG CSkfSDKTester::GetDevInfo(LPCTSTR lpcsDevName)
{
	return USBKEY_ERR_FAILED;
}

// 初始化设备
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

// 设置标签
ULONG CSkfSDKTester::SetLabel(LPCTSTR lpcsDevName,
							  LPCTSTR lpcsLabel)
{
	return USBKEY_ERR_FAILED;
}

// 设备认证
ULONG CSkfSDKTester::DevAuth(LPCTSTR lpcsDevName, 
							 LPBYTE lpAuthKey, 
							 ULONG ulKeyLen)
{
	return USBKEY_ERR_FAILED;
}

// 修改设备认证Key
ULONG CSkfSDKTester::ChangeAuthKey(LPCTSTR lpcsDevName, 
								   LPBYTE lpNewAuthKey, 
								   ULONG ulKeyLen)
{
	return USBKEY_ERR_FAILED;
}

// 打开会话
ULONG CSkfSDKTester::OpenSession(LPCTSTR lpcsDevName, 
								 ULONG *phSession)
{
	return USBKEY_ERR_FAILED;
}

// 获取会话信息
ULONG CSkfSDKTester::GetSessionInfo(ULONG hSession)
{
	return USBKEY_ERR_FAILED;
}

// 关闭会话
ULONG CSkfSDKTester::CloseSession(ULONG hSession)
{
	return USBKEY_ERR_FAILED;
}

// 登录会话
ULONG CSkfSDKTester::LoginSession(ULONG hSession, 
								  ULONG ulUserType, 
								  LPCTSTR lpcsPIN, 
								  ULONG* pulLeftTimes)
{
	return USBKEY_ERR_FAILED;
}

// 登出会话
ULONG CSkfSDKTester::LogOutSession(ULONG hSession)
{
	return USBKEY_ERR_FAILED;
}

// 修改PIN码
ULONG CSkfSDKTester::ChangePin(ULONG hSession, 
							   ULONG ulUserType, 
							   LPCTSTR lpcsOldPIN, 
							   LPCTSTR lpcsNewPIN, 
							   ULONG* pulLeftTimes)
{
	return USBKEY_ERR_FAILED;
}

// 解锁用户PIN
ULONG CSkfSDKTester::UnlockPin(ULONG hSession, 
							   LPCTSTR lpcsSOPIN, 
							   LPCTSTR lpcsNewUserPIN, 
							   ULONG* pulLeftTimes)
{
	return USBKEY_ERR_FAILED;
}

// 枚举文件
ULONG CSkfSDKTester::EnumFiles(ULONG hSession, 
							   LPTSTR lpFilesName, 
							   ULONG* pulLen)
{
	return USBKEY_ERR_FAILED;
}

// 创建文件
ULONG CSkfSDKTester::CreateFile(ULONG hSession, 
								LPTSTR lpFilesName, 
								ULONG ulSize)
{
	return USBKEY_ERR_FAILED;
}

// 删除文件
ULONG CSkfSDKTester::DeleteFile(ULONG hSession, 
								LPTSTR lpFilesName)
{
	return USBKEY_ERR_FAILED;
}

// 写文件
ULONG CSkfSDKTester::WriteFile(ULONG hSession, 
							   LPTSTR lpFilesName, 
							   LPBYTE lpData, 
							   ULONG ulLen)
{
	return USBKEY_ERR_FAILED;
}

// 读文件
ULONG CSkfSDKTester::ReadFile(ULONG hSession, 
							  LPTSTR lpFilesName, 
							  LPBYTE lpData, 
							  ULONG* pulLen)
{
	return USBKEY_ERR_FAILED;
}

// 导入证书
ULONG CSkfSDKTester::ImportCert(ULONG hSession, 
								LPCTSTR lpContainer, 
								ULONG ulCertAlg, 
								ULONG ulCertUsage, 
								LPBYTE lpCert, 
								ULONG ulLen)
{
	return USBKEY_ERR_FAILED;
}

// 导出证书
ULONG CSkfSDKTester::ExportCert(ULONG hSession, 
								ULONG ulCertAlg, 
								ULONG ulCertUsage, 
								LPBYTE lpCert, 
								ULONG* pulLen)
{
	return USBKEY_ERR_FAILED;
}

// 生成对称密钥
ULONG CSkfSDKTester::GenerateSessionKey(ULONG hSession, 
										ULONG ulAlgID, 
										ULONG* phSessionKey)
{
	return USBKEY_ERR_FAILED;
}

// 销毁对称密钥
ULONG CSkfSDKTester::CloseSessionKey(ULONG hSession, 
									 ULONG hSessionKey)
{
	return USBKEY_ERR_FAILED;
}

// 生成签名密钥对
ULONG CSkfSDKTester::GenerateKeyPair(ULONG hSession,
									 ULONG ulAlgID, 
									 ULONG ulBits)
{
	return USBKEY_ERR_FAILED;
}

// 导入加密密钥对
ULONG CSkfSDKTester::ImportKeyPair(ULONG hSession,
								   ULONG ulAlgID, 
								   LPBYTE lpKeyPair, 
								   ULONG ulKeyPairLen)
{
	return USBKEY_ERR_FAILED;
}

// 导出公钥数据
ULONG CSkfSDKTester::ExportPublicKey(ULONG hSession, 
									 ULONG ulAlgID, 
									 BOOL bSign, 
									 LPBYTE lpPubKey, 
									 ULONG *pulLen)
{
	return USBKEY_ERR_FAILED;
}

// 测试加密数据-解密数据
ULONG CSkfSDKTester::TestEncrypt(ULONG hSession, 
								 ULONG ulAlgID, 
								 LPCTSTR lpcsMsg)
{
	return USBKEY_ERR_OK;
}

// 测试打包数字信封数据-解密数字信封
ULONG CSkfSDKTester::TestEnvelop(ULONG hSession, 
								 ULONG ulAlgID, 
								 LPCTSTR lpcsMsg)
{
	return USBKEY_ERR_OK;
}

// 测试签名验签
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
