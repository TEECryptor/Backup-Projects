#include "stdafx.h"
#include <string.h>
#include "USBKeySKFImpl.h"
#include "./LogFile/LogFile.h"

/***********************************************************************
 *	名称:CUSBKeySKFImpl
 *	说明:构造函数
 *	参数:无
 *	返回:无
 ***********************************************************************/
CUSBKeySKFImpl::CUSBKeySKFImpl(void)
 : m_hInstance(NULL)
 , m_ulAsymmAlg(USBKEY_ASYMMALG_SM2)
 , m_ulSymmAlg(USBKEY_SYMMALG_SM1_ECB)
 , m_ulHashAlg(USBKEY_HASHALG_SM3)
 , m_bSDKLoaded(FALSE)
 , m_hDev(NULL)
 , m_hApplication(NULL)
{
	memset(m_csDLLFile, 0, 256);
}
/***********************************************************************
 *	名称:~CUSBKeySKFImpl
 *	说明:析构函数
 *	参数:无
 *	返回:无
 ***********************************************************************/
CUSBKeySKFImpl::~CUSBKeySKFImpl(void)
{
}
/***********************************************************************
 *	名称:Create
 *	说明:加载国密SKF接口DLL文件
 *	参数:lpcsCspOrFile:[IN]:DLL文件名
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeySKFImpl::Create(LPCSTR lpcsCspOrFile)
{
	ULONG ulRes = USBKEY_ERR_OK;

	FUNC_BEGIN;

	strcpy_s(m_csDLLFile, lpcsCspOrFile);
	if (strlen(m_csDLLFile) == 0)
	{
		USBKey_LogErr("m_csDLLFile is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	if (m_hInstance)
	{
		Release();
	}
	
	//	加载DLL
	m_hInstance = LoadLibraryA(m_csDLLFile);
	if (!m_hInstance)
	{
		ulRes = GetLastError();
		USBKey_LogErr("LoadLibraryA() failed! ulRes=0x%x", ulRes);
		return ulRes;
	}	
	
	//	获取接口函数
	SKF_WaitForDevEvent = (SKF_WaitForDevEventProc)GetProcAddress(m_hInstance, "SKF_WaitForDevEvent");
	SKF_CancelWaitForDevEvent = (SKF_CancelWaitForDevEventProc)GetProcAddress(m_hInstance, "SKF_CancelWaitForDevEvent");
	SKF_EnumDev = (SKF_EnumDevProc)GetProcAddress(m_hInstance, "SKF_EnumDev");
	SKF_ConnectDev = (SKF_ConnectDevProc)GetProcAddress(m_hInstance, "SKF_ConnectDev");
	SKF_DisConnectDev = (SKF_DisConnectDevProc)GetProcAddress(m_hInstance, "SKF_DisConnectDev");
	SKF_GetDevState = (SKF_GetDevStateProc)GetProcAddress(m_hInstance, "SKF_GetDevState");
	SKF_SetLabel = (SKF_SetLabelProc)GetProcAddress(m_hInstance, "SKF_SetLabel");
	SKF_GetDevInfo = (SKF_GetDevInfoProc)GetProcAddress(m_hInstance, "SKF_GetDevInfo");
	SKF_LockDev = (SKF_LockDevProc)GetProcAddress(m_hInstance, "SKF_LockDev");
	SKF_UnlockDev = (SKF_UnlockDevProc)GetProcAddress(m_hInstance, "SKF_UnlockDev");
	SKF_ChangeDevAuthKey = (SKF_ChangeDevAuthKeyProc)GetProcAddress(m_hInstance, "SKF_ChangeDevAuthKey");
	SKF_DevAuth = (SKF_ChangeDevAuthKeyProc)GetProcAddress(m_hInstance, "SKF_DevAuth");
	//
	SKF_CreateApplication = (SKF_CreateApplicationProc)GetProcAddress(m_hInstance, "SKF_CreateApplication");
	SKF_EnumApplication = (SKF_EnumApplicationProc)GetProcAddress(m_hInstance, "SKF_EnumApplication");
	SKF_DeleteApplication = (SKF_DeleteApplicationProc)GetProcAddress(m_hInstance, "SKF_DeleteApplication");
	SKF_OpenApplication = (SKF_OpenApplicationProc)GetProcAddress(m_hInstance, "SKF_OpenApplication");
	SKF_CloseApplication = (SKF_CloseApplicationProc)GetProcAddress(m_hInstance, "SKF_CloseApplication");	
	//
	SKF_CreateFile = (SKF_CreateFileProc)GetProcAddress(m_hInstance, "SKF_CreateFile");
	SKF_DeleteFile = (SKF_DeleteFileProc)GetProcAddress(m_hInstance, "SKF_DeleteFile");
	SKF_EnumFiles = (SKF_EnumFilesProc)GetProcAddress(m_hInstance, "SKF_EnumFiles");
	SKF_GetFileInfo = (SKF_GetFileInfoProc)GetProcAddress(m_hInstance, "SKF_GetFileInfo");
	SKF_ReadFile = (SKF_ReadFileProc)GetProcAddress(m_hInstance, "SKF_ReadFile");
	SKF_WriteFile = (SKF_WriteFileProc)GetProcAddress(m_hInstance, "SKF_WriteFile");
	//
	SKF_GenRandom = (SKF_GenRandomProc)GetProcAddress(m_hInstance, "SKF_GenRandom");
	SKF_SetSymmKey = (SKF_SetSymmKeyProc)GetProcAddress(m_hInstance, "SKF_SetSymmKey");
	SKF_EncryptInit = (SKF_EncryptInitProc)GetProcAddress(m_hInstance, "SKF_EncryptInit");
	SKF_Encrypt = (SKF_EncryptProc)GetProcAddress(m_hInstance, "SKF_Encrypt");
	SKF_EncryptUpdate = (SKF_EncryptUpdateProc)GetProcAddress(m_hInstance, "SKF_EncryptUpdate");
	SKF_EncryptFinal = (SKF_EncryptFinalProc)GetProcAddress(m_hInstance, "SKF_EncryptFinal");	
	SKF_DecryptInit = (SKF_DecryptInitProc)GetProcAddress(m_hInstance, "SKF_DecryptInit");
	SKF_Decrypt = (SKF_DecryptProc)GetProcAddress(m_hInstance, "SKF_Decrypt");
	SKF_DecryptUpdate = (SKF_DecryptUpdateProc)GetProcAddress(m_hInstance, "SKF_DecryptUpdate");
	SKF_DecryptFinal = (SKF_DecryptFinalProc)GetProcAddress(m_hInstance, "SKF_DecryptFinal");
	SKF_ExtECCEncrypt = (SKF_ExtECCEncryptProc)GetProcAddress(m_hInstance, "SKF_ExtECCEncrypt");
	SKF_CloseHandle = (SKF_CloseHandleProc)GetProcAddress(m_hInstance, "SKF_CloseHandle");
	//
	SKF_ChangePIN = (SKF_ChangePINProc)GetProcAddress(m_hInstance, "SKF_ChangePIN");
	SKF_GetPINInfo = (SKF_GetPINInfoProc)GetProcAddress(m_hInstance, "SKF_GetPINInfo");
	SKF_VerifyPIN = (SKF_VerifyPINProc)GetProcAddress(m_hInstance, "SKF_VerifyPIN");
	SKF_UnblockPIN = (SKF_UnblockPINProc)GetProcAddress(m_hInstance, "SKF_UnblockPIN");
	SKF_ClearSecureState = (SKF_ClearSecureStateProc)GetProcAddress(m_hInstance, "SKF_ClearSecureState");
	//	
	SKF_CreateContainer = (SKF_CreateContainerProc)GetProcAddress(m_hInstance, "SKF_CreateContainer");
	SKF_DeleteContainer = (SKF_DeleteContainerProc)GetProcAddress(m_hInstance, "SKF_DeleteContainer");
	SKF_OpenContainer = (SKF_OpenContainerProc)GetProcAddress(m_hInstance, "SKF_OpenContainer");
	SKF_CloseContainer = (SKF_CloseContainerProc)GetProcAddress(m_hInstance, "SKF_CloseContainer");
	SKF_EnumContainer = (SKF_EnumContainerProc)GetProcAddress(m_hInstance, "SKF_EnumContainer");
	SKF_GetContainerType = (SKF_GetContainerTypeProc)GetProcAddress(m_hInstance, "SKF_GetContainerType");
	if (!SKF_GetContainerType) SKF_GetContainerType = (SKF_GetContainerTypeProc)GetProcAddress(m_hInstance, "SKF_GetConProperty");
	//	
	SKF_ImportCertificate = (SKF_ImportCertificateProc)GetProcAddress(m_hInstance, "SKF_ImportCertificate");
	SKF_ExportCertificate = (SKF_ExportCertificateProc)GetProcAddress(m_hInstance, "SKF_ExportCertificate");
	//
	SKF_GenRSAKeyPair = (SKF_GenRSAKeyPairProc)GetProcAddress(m_hInstance, "SKF_GenRSAKeyPair");
	SKF_ImportRSAKeyPair = (SKF_ImportRSAKeyPairProc)GetProcAddress(m_hInstance, "SKF_ImportRSAKeyPair");
	SKF_RSASignData = (SKF_RSASignDataProc)GetProcAddress(m_hInstance, "SKF_RSASignData");
	SKF_RSAVerify = (SKF_RSAVerifyProc)GetProcAddress(m_hInstance, "SKF_RSAVerify");
	SKF_RSAExportSessionKey = (SKF_RSAExportSessionKeyProc)GetProcAddress(m_hInstance, "SKF_RSAExportSessionKey");
	//
	SKF_GenECCKeyPair = (SKF_GenECCKeyPairProc)GetProcAddress(m_hInstance, "SKF_GenECCKeyPair");
	SKF_ImportECCKeyPair = (SKF_ImportECCKeyPairProc)GetProcAddress(m_hInstance, "SKF_ImportECCKeyPair");
	SKF_ECCSignData = (SKF_ECCSignDataProc)GetProcAddress(m_hInstance, "SKF_ECCSignData");
	SKF_ECCVerify = (SKF_ECCVerifyProc)GetProcAddress(m_hInstance, "SKF_ECCVerify");
	SKF_ECCExportSessionKey = (SKF_ECCExportSessionKeyProc)GetProcAddress(m_hInstance, "SKF_ECCExportSessionKey");
	//	
	SKF_ExportPublicKey = (SKF_ExportPublicKeyProc)GetProcAddress(m_hInstance, "SKF_ExportPublicKey");
	SKF_ImportSessionKey = (SKF_ImportSessionKeyProc)GetProcAddress(m_hInstance, "SKF_ImportSessionKey");
	//
	SKF_DigestInit = (SKF_DigestInitProc)GetProcAddress(m_hInstance, "SKF_DigestInit");
	SKF_Digest = (SKF_DigestProc)GetProcAddress(m_hInstance, "SKF_Digest");
	SKF_DigestUpdate = (SKF_DigestUpdateProc)GetProcAddress(m_hInstance, "SKF_DigestUpdate");
	SKF_DigestFinal = (SKF_DigestFinalProc)GetProcAddress(m_hInstance, "SKF_DigestFinal");
	//
	SKF_MacInit = (SKF_MacInitProc)GetProcAddress(m_hInstance, "SKF_MacInit");
	SKF_Mac = (SKF_MacProc)GetProcAddress(m_hInstance, "SKF_Mac");
	SKF_MacUpdate = (SKF_MacUpdateProc)GetProcAddress(m_hInstance, "SKF_MacUpdate");
	SKF_MacFinal = (SKF_MacFinalProc)GetProcAddress(m_hInstance, "SKF_MacFinal");
	SKF_Transmit = (SKF_TransmitProc)GetProcAddress(m_hInstance, "SKF_Transmit");

	m_bSDKLoaded = TRUE;

	FUNC_END;

	return ulRes;
}
/***********************************************************************
 *	名称:Initialize
 *	说明:初始化接口参数，设置使用的算法等。
 *	参数:ulAsymmAlg:[IN]:非对称算法
 *		 ulSymmAlg:[IN]:对称算法
 *	     ulHashAlg:[IN]:Hash算法
 *		 lpUserPIN:[IN]:用户PIN码
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeySKFImpl::Initialize(ULONG ulAsymmAlg, 
								 ULONG ulSymmAlg, 
								 ULONG ulHashAlg, 
								 LPSTR lpUserPIN)
{
	ULONG ulRes = USBKEY_ERR_OK;

	FUNC_BEGIN;

	ZJCA_LogI(_T("AsymmAlg:0x%x, SymmAlg:0x%x, HashAlg:0x%x"), ulAsymmAlg, ulSymmAlg, ulHashAlg);

	//	保存参数
	if (USBKEY_ASYMMALG_RSA == ulAsymmAlg)
	{
		m_ulAsymmAlg = SGD_RSA;
	}
	else if (USBKEY_ASYMMALG_SM2 == ulAsymmAlg)
	{
		m_ulAsymmAlg = SGD_SM2_1;
	}
	else
	{		
		USBKey_LogErr("ulAsymmAlg is wrong! ulAsymmAlg = 0x%x", ulAsymmAlg);
		return USBKEY_ERR_INVALIDPARAM;
	}

	if (USBKEY_SYMMALG_SM1_ECB == ulSymmAlg)
	{
		m_ulSymmAlg = SGD_SM1_ECB;
	}
	else if (USBKEY_SYMMALG_SMS4_ECB == ulSymmAlg)
	{
		m_ulSymmAlg = SGD_SMS4_ECB;
	}
	else if (USBKEY_SYMMALG_SSF33_ECB == ulSymmAlg)
	{
		m_ulSymmAlg = SGD_SSF33_ECB;
	}
	else
	{
		USBKey_LogErr("ulSymmAlg is wrong! ulSymmAlg = 0x%x", ulSymmAlg);
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	if (USBKEY_HASHALG_SHA1 == ulHashAlg)
	{
		m_ulHashAlg = SGD_SHA1;
	}
	else if (USBKEY_HASHALG_SHA256 == ulHashAlg)
	{
		m_ulHashAlg = SGD_SHA256;
	}
	else if (USBKEY_HASHALG_SM3 == ulHashAlg)
	{
		m_ulHashAlg = SGD_SM3;
	}
	else if (USBKEY_HASHALG_MD5 == ulHashAlg)
	{
		m_ulHashAlg = SGD_MD5;
	}
	else
	{
		USBKey_LogErr("ulHashAlg is wrong! ulHashAlg = 0x%x", ulHashAlg);
		return USBKEY_ERR_INVALIDPARAM;
	}

	if ((SGD_SM3 == m_ulHashAlg) && (SGD_SM2_1 != m_ulAsymmAlg))
	{
		USBKey_LogErr("SM3 is only valid for SM2!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	if (lpUserPIN && strlen(lpUserPIN) > 0)
	{
		strcpy_s(m_csUserPIN, 64, lpUserPIN);
	}

	FUNC_END;

	return ulRes;
}
/***********************************************************************
 *	名称:WaitForDevEvent
 *	说明:等待设备事件函数
 *	参数:bNotBlock:[IN]:是否是非阻塞模式
 *		pulSoltID:[IN]:SKF接口忽略该参数
 *		lpDevName:[IN]:发生事件的设备名
 *		pulNameLen:[IN]:设备名长度
 *		pulEventType:[IN]:事件类型
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeySKFImpl::WaitForDevEvent(BOOL bNotBlock, 
									  ULONG *pulSoltID, 
									  LPSTR lpDevName,
									  ULONG *pulNameLen, 
									  ULONG *pulEventType)
{
	ULONG ulRes = 0;

	FUNC_BEGIN;

	if (!SKF_WaitForDevEvent)
	{
		USBKey_LogErr("SKF_WaitForDevEvent is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	if (!pulSoltID)
	{
		USBKey_LogErr("pulSoltID is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpDevName)
	{
		USBKey_LogErr("lpDevName is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!pulNameLen)
	{
		USBKey_LogErr("pulNameLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (*pulNameLen < 33)
	{
		USBKey_LogErr("*pulNameLen is too small!");
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}
	if (!pulEventType)
	{
		USBKey_LogErr("pulEventType is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}

	*pulSoltID = 0;
	ulRes = SKF_WaitForDevEvent(lpDevName, pulNameLen, pulEventType);

	FUNC_END;

	return ulRes;
}
/***********************************************************************
 *	名称:CancelWaitForDevEvent
 *	说明:取消等待设备事件函数。
 *	参数:无
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeySKFImpl::CancelWaitForDevEvent()
{
	ULONG ulRes = 0;
	
	FUNC_BEGIN;

	if (!SKF_CancelWaitForDevEvent)
	{
		USBKey_LogErr("SKF_CancelWaitForDevEvent is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}

	ulRes = SKF_CancelWaitForDevEvent();
	if (SAR_OK != ulRes)		
	{
		USBKey_LogErr("SKF_CancelWaitForDevEvent failed! ulRes = 0x%x");
		return ulRes;
	}

	FUNC_END;

	return ulRes;
}
/***********************************************************************
 *	名称:ConnectDev
 *	说明:连接第一个设备，打开应用并校验用户PIN。
 *	参数:无
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeySKFImpl::ConnectDev()
{
	ULONG ulRes = SAR_OK;
	ULONG ulPos = 0;
	ULONG ulNamesLen = 1024;
	ULONG ulRetryTimes = 0;
	CHAR csNames[1024] = {0};
	CHAR csSelDevName[48] = {0};
	LPSTR lpcsAppName = NULL;
	
	FUNC_BEGIN;

	if (!m_bSDKLoaded)
	{
		USBKey_LogErr("SKF DLL has not loaded, please call Create() at first!");
		return USBKEY_ERR_INVALIDCALL;
	}

	ulRes = SKF_EnumDev(TRUE, csNames, &ulNamesLen);
	if (0 != ulRes)
	{
		USBKey_LogErr("SKF_EnumDev() failed! ulRes=0x%x", ulRes);
		return ulRes;
	}	
	if (ulNamesLen <= 2)
	{
		USBKey_LogErr("No any key is found!");
		return USBKEY_ERR_NO_DEVICE;
	}

	DisconnectDev();
	
	strcpy_s(csSelDevName, 48, csNames);	
	ulRes = SKF_ConnectDev(csSelDevName, &m_hDev);
	if (ulRes != SAR_OK)
	{
		USBKey_LogErr("SKF_ConnectDev() failed! ulRes=0x%x", ulRes);
		return ulRes;
	}
		
	lpcsAppName = _GetApplitionName();
	if (!lpcsAppName || strlen(lpcsAppName) == 0)
	{
		USBKey_LogErr("No application!");
		return USBKEY_ERR_NO_APP;
	}

	ulRes = SKF_OpenApplication(m_hDev, lpcsAppName, &m_hApplication);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_OpenApplication() failed! ulRes=0x%x", ulRes);
		return ulRes;
	}

	ulRes = SKF_VerifyPIN(m_hApplication, USER_TYPE, m_csUserPIN, &ulRetryTimes);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_VerifyPIN() failed! ulRes=0x%x", ulRes);
		return ulRes;
	}

	FUNC_END;

	return USBKEY_ERR_OK;
}
/***********************************************************************
 *	名称:Envelop
 *	说明:按P7数字信封格式封装数据，SKF接口不支持。
 *	参数:忽略
 *	返回:USBKEY_ERR_NOT_SUPPORT
 ***********************************************************************/
ULONG CUSBKeySKFImpl::Envelop(LPBYTE lpInData, 
							  ULONG ulInLen, 
							  LPBYTE lpOutData, 
							  ULONG* pulOutLen)
{
	USBKey_LogErr("This interface don't support envelop data to P7!");
	return USBKEY_ERR_NOT_SUPPORT;
}
/***********************************************************************
 *	名称:Envelop
 *	说明:解码P7格式密文数据，SKF接口不支持。
 *	参数:忽略
 *	返回:USBKEY_ERR_NOT_SUPPORT
 ***********************************************************************/
ULONG CUSBKeySKFImpl::Develop(LPBYTE lpInData, 
							  ULONG ulInLen, 
							  LPBYTE lpOutData, 
							  ULONG* pulOutLen)
{
	USBKey_LogErr("This interface don't support Develop data from P7!");
	return USBKEY_ERR_NOT_SUPPORT;
}
/***********************************************************************
 *	名称:Encrypt
 *	说明:按"Wrapped Key + Cipher"格式加密数据
 *	参数:lpInData:[IN]:明文数据
 *		 ulInLen:[IN]:明文数据长度
 *		 lpOutData:[OUT]:返回的数字信封数据
 *		 pulOutLen:[IN/OUT]:数字信封数据长度
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeySKFImpl::Encrypt(LPBYTE lpInData, 
							  ULONG ulInLen, 
							  LPBYTE lpOutData, 
							  ULONG* pulOutLen)
{
	ULONG ulRes = 0;
	ULONG ulIndex = 0;
	ULONG ulPlainIndex = 0;
	ULONG ulPubKeyLen = 0;
	ULONG ulSessionKeyLen = 256;
	LPBYTE lpPubKey = NULL;
	BYTE btSessionKey[256] = {0};
	ULONG ulCipherLen = USBKEY_CRYPTLEN_PERTIME + 256;
	BYTE btCipherData[USBKEY_CRYPTLEN_PERTIME + 256] = {0};
	BLOCKCIPHERPARAM cipherParam = {0};
	HCONTAINER hCon = NULL;
	HANDLE hSessionKey = NULL;
	
	FUNC_BEGIN;

	// 参数检查
	if (!lpInData || ulInLen == 0)
	{
		USBKey_LogErr("lpInData is NULL or ulInLen is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpOutData || !pulOutLen)
	{
		USBKey_LogErr("lpOutData or pulOutLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (*pulOutLen < (ulInLen + 260))
	{
		USBKey_LogErr("*pulOutLen is too small! Reqeusted length:0x%x", ulInLen + 260);
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	// 打开加密密钥所在的容器
	hCon = _OpenContainer(m_ulAsymmAlg, FALSE);
	if (NULL == hCon)
	{
		return USBKEY_ERR_NO_KEYSET;
	}

	// 导出加密公钥
	ulRes = SKF_ExportPublicKey(hCon, FALSE, NULL, &ulPubKeyLen);
	if (SAR_OK != ulRes || ulPubKeyLen == 0)
	{
		USBKey_LogErr("SKF_ExportPublicKey() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	lpPubKey = new BYTE[ulPubKeyLen];
	ulRes = SKF_ExportPublicKey(hCon, FALSE, lpPubKey, &ulPubKeyLen);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_ExportPublicKey() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 生成并导出会话密钥
	if (SGD_RSA == m_ulAsymmAlg)
	{
		ulRes = SKF_RSAExportSessionKey(hCon, m_ulSymmAlg, (PRSAPUBLICKEYBLOB)lpPubKey, btSessionKey, &ulSessionKeyLen, &hSessionKey);
	}
	else
	{
		PECCCIPHERBLOB lpEccCipher = (PECCCIPHERBLOB)btSessionKey;
		lpEccCipher->CipherLen = 32;
		ulRes = SKF_ECCExportSessionKey(hCon, m_ulSymmAlg, (PECCPUBLICKEYBLOB)lpPubKey, (PECCCIPHERBLOB)btSessionKey, &hSessionKey);
		ulSessionKeyLen = sizeof(ECCCIPHERBLOB) + ((PECCCIPHERBLOB)btSessionKey)->CipherLen - 1;
	}
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("Export session key failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 拷贝Session Key数据
	memcpy_s(lpOutData, *pulOutLen, &ulSessionKeyLen, 4);
	ulIndex = 4;
	memcpy_s(lpOutData + ulIndex, *pulOutLen, btSessionKey, ulSessionKeyLen);
	ulIndex += ulSessionKeyLen;
	
	//	加密初始化
	ulRes = SKF_EncryptInit(hSessionKey, cipherParam);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_EncryptInit() failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}

	//	使用对称密钥对数据加密(采用多次)
	ulPlainIndex = 0;
	do
	{
		BOOL bUsePaddedBlob = FALSE;
		ULONG ulThisTimePlainLen = 0;
		BYTE btPaddedBlob[USBKEY_CRYPTLEN_PERTIME + 16] = {0};
		if ((ulInLen - ulPlainIndex) > USBKEY_CRYPTLEN_PERTIME)
		{
			ulThisTimePlainLen = USBKEY_CRYPTLEN_PERTIME;
		}
		else
		{
			ulThisTimePlainLen = ulInLen - ulPlainIndex;
			if (ulThisTimePlainLen % 16 != 0)
			{
				memcpy(btPaddedBlob, lpInData+ulPlainIndex, ulThisTimePlainLen);
				ulThisTimePlainLen += (16 - ulThisTimePlainLen % 16);
				bUsePaddedBlob = TRUE;
			}
		}
		//
		ulCipherLen = USBKEY_CRYPTLEN_PERTIME + 256;
		ulRes = SKF_EncryptUpdate(hSessionKey, bUsePaddedBlob ? btPaddedBlob : lpInData+ulPlainIndex, ulThisTimePlainLen, btCipherData, &ulCipherLen);
		if (SAR_OK != ulRes)
		{
			USBKey_LogErr("SKF_EncryptUpdate() failed! ulRes=0x%x", ulRes);
			goto FREE_MEMORY;
		}
		ulPlainIndex += ulThisTimePlainLen;
		//
		if (ulCipherLen > 0)
		{
			memcpy_s(lpOutData + ulIndex, *pulOutLen, btCipherData, ulCipherLen);
			ulIndex += ulCipherLen;
		}
	}while(ulPlainIndex < ulInLen);
	
	//	加密结束(必须传有效的接受数据的Buffer指针和长度)
	ulCipherLen = USBKEY_CRYPTLEN_PERTIME + 256;
	ulRes = SKF_EncryptFinal(hSessionKey, btCipherData, &ulCipherLen);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_EncryptFinal() failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}
	if (ulCipherLen > 0)
	{
		memcpy_s(lpOutData + ulIndex, *pulOutLen, btCipherData, ulCipherLen);
		ulIndex += ulCipherLen;
	}
	*pulOutLen = ulIndex;

	FUNC_END;

FREE_MEMORY: 
	if (lpPubKey)
	{
		delete []lpPubKey;
		lpPubKey = NULL;
	}
	if (hSessionKey)
	{
		SKF_CloseHandle(hSessionKey);
		hSessionKey = NULL;
	}
	if (hCon)
	{
		SKF_CloseContainer(hCon);
		hCon = NULL;
	}

	return ulRes;
}
/***********************************************************************
 *	名称:Decrypt
 *	说明:解密"Wrapped Key + Cipher"格式密文
 *	参数:lpInData:[IN]:密文数据
 *		 ulInLen:[IN]:密文数据长度
 *		 lpOutData:[OUT]:解密后的数据
 *		 pulOutLen:[IN/OUT]:解密后的数据长度
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 **********************************************************************/
ULONG CUSBKeySKFImpl::Decrypt(LPBYTE lpInData, 
							  ULONG ulInLen, 
							  LPBYTE lpOutData, 
							  ULONG* pulOutLen)
{
	ULONG ulRes = 0;
	ULONG ulCipherIndex = 0;
	ULONG ulIndex = 0;
	ULONG ulWrappedLen = 0;
	BLOCKCIPHERPARAM cipherParam = {0};
	HCONTAINER hCon = NULL;
	HANDLE hSessionKey = NULL;
	ULONG ulDecryptedLen = USBKEY_CRYPTLEN_PERTIME + 256;
	BYTE btDecryptedData[USBKEY_CRYPTLEN_PERTIME + 256] = {0};
	
	FUNC_BEGIN;

	// 参数检查
	if (!lpInData || ulInLen == 0)
	{
		USBKey_LogErr("lpInData is NULL or ulInLen is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpOutData || !pulOutLen)
	{
		USBKey_LogErr("lpOutData or pulOutLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (*pulOutLen < ulInLen)
	{
		USBKey_LogErr("*pulOutLen is too small! Reqeusted length:0x%x", ulInLen + 260);
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	// 打开加密密钥所在的容器
	hCon = _OpenContainer(m_ulAsymmAlg, FALSE);
	if (NULL == hCon)
	{
		return USBKEY_ERR_NO_KEYSET;
	}

	// Wrapped Key长度
	memcpy(&ulWrappedLen, lpInData, 4);	
	ulCipherIndex = 4;
	if (SGD_RSA == m_ulAsymmAlg)
	{
		if (128 != ulWrappedLen && 256 != ulWrappedLen)
		{
			USBKey_LogErr("Wrapped key blob lenght is wrong! The length should be 128 or 256. ulWrappedLen = 0x%x", ulWrappedLen);
			ulRes = USBKEY_ERR_INDATAERROR;
			goto FREE_MEMORY;
		}
	}
	else
	{
		PECCCIPHERBLOB pECCBlob = (PECCCIPHERBLOB)(lpInData + ulCipherIndex);
		if (ulWrappedLen != (sizeof(ECCCIPHERBLOB) - 1 + pECCBlob->CipherLen))
		{
			USBKey_LogErr("Wrapped key blob lenght is wrong! The length should be 0x%x. ulWrappedLen = 0x%x", (sizeof(ECCCIPHERBLOB) - 1 + pECCBlob->CipherLen), ulWrappedLen);
			ulRes = USBKEY_ERR_INDATAERROR;
			goto FREE_MEMORY;
		}
	}

	//	导入对称密钥
	ulRes = SKF_ImportSessionKey(hCon, m_ulSymmAlg, lpInData + ulCipherIndex, ulWrappedLen, &hSessionKey);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_ImportSessionKey() failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}
	ulCipherIndex += ulWrappedLen;

	//	解密初始化
	ulRes = SKF_DecryptInit(hSessionKey, cipherParam);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_DecryptInit() failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}
		
	//	使用对称密钥对数据加密(采用多次)
	ulIndex = 0;
	do
	{
		ULONG ulThisTimeCipherLen = 0;
		if ((ulInLen - ulCipherIndex) > USBKEY_CRYPTLEN_PERTIME)
		{
			ulThisTimeCipherLen = USBKEY_CRYPTLEN_PERTIME;
		}
		else
		{
			ulThisTimeCipherLen = ulInLen - ulCipherIndex;
		}
		//
		ulDecryptedLen = USBKEY_CRYPTLEN_PERTIME + 256;
		ulRes = SKF_DecryptUpdate(hSessionKey, lpInData+ulCipherIndex, ulThisTimeCipherLen, btDecryptedData, &ulDecryptedLen);
		if (SAR_OK != ulRes)
		{
			USBKey_LogErr("SKF_DecryptUpdate() failed! ulRes = 0x%x", ulRes);
			goto FREE_MEMORY;
		}
		ulCipherIndex += ulThisTimeCipherLen;
		//
		if (ulDecryptedLen > 0)
		{
			memcpy(lpOutData + ulIndex, btDecryptedData, ulDecryptedLen);
			ulIndex += ulDecryptedLen;
		}
	}while(ulCipherIndex < ulInLen);
		
	//	解密结束(必须传有效的接受数据的Buffer指针和长度)
	ulDecryptedLen = USBKEY_CRYPTLEN_PERTIME + 256;
	ulRes = SKF_DecryptFinal(hSessionKey, btDecryptedData, &ulDecryptedLen);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_EncryptFinal() failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}
	if (ulDecryptedLen > 0)
	{
		memcpy(lpOutData + ulIndex, btDecryptedData, ulDecryptedLen);
		ulIndex += ulDecryptedLen;
	}
	*pulOutLen = ulIndex;
	
	FUNC_END;

FREE_MEMORY: 
	if (hSessionKey)
	{
		SKF_CloseHandle(hSessionKey);
		hSessionKey = NULL;
	}
	if (hCon)
	{
		SKF_CloseContainer(hCon);
		hCon = NULL;
	}

	return ulRes;
}
/***********************************************************************
 *	名称:SignP7
 *	说明:按P7格式签名数据，SKF接口不支持。
 *	参数:忽略
 *	返回:USBKEY_ERR_NOT_SUPPORT
 ***********************************************************************/
ULONG CUSBKeySKFImpl::SignP7(LPBYTE lpOrgData, 
							 ULONG ulOrgLen, 
							 LPBYTE lpSignature, 
							 ULONG* pulSignLen)
{
	USBKey_LogErr("This interface don't support sign data to P7!");
	return USBKEY_ERR_NOT_SUPPORT;
}
/***********************************************************************
 *	名称:VerifyP7
 *	说明:验证P7格式的签名，SKF接口不支持。
 *	参数:忽略
 *	返回:USBKEY_ERR_NOT_SUPPORT
 ***********************************************************************/
ULONG CUSBKeySKFImpl::VerifyP7(LPBYTE lpOrgData, 
							   ULONG ulOrgLen, 
							   LPBYTE lpSignature, 
							   ULONG ulSignLen)
{
	USBKey_LogErr("This interface don't support verify P7 signature!");
	return USBKEY_ERR_NOT_SUPPORT;
}
 /***********************************************************************
 *	名称:SignP1
 *	说明:普通格式签名数据
 *	参数:lpOrgData:[IN]:原文数据
 *		 ulOrgLen:[IN]:原文数据长度
 *		 lpUserID:[IN]:签名时额外的用户描述
 *		 lpSignature:[OUT]:签名数据
 *		 pulSignLen:[IN/OUT]:签名长度
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeySKFImpl::SignP1(LPBYTE lpOrgData, 
							 ULONG ulOrgLen, 
							 LPSTR lpUserID, 
							 LPBYTE lpSignature, 
							 ULONG* pulSignLen)
{
	ULONG ulRes = 0;
	ULONG ulOrgIndex = 0;
	ULONG ulHashLen = 64;
	BYTE btHashValue[64] = {0};
	ECCPUBLICKEYBLOB eccPubKeyBlob = {0};
	HCONTAINER hCon = NULL;
	HANDLE hHashKey = NULL;
	
	FUNC_BEGIN;

	//	参数检查
	if (!lpOrgData || ulOrgLen == 0)
	{
		USBKey_LogErr("lpOrgData is NULL or ulOrgLen is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpSignature || !pulSignLen)
	{
		USBKey_LogErr("lpSignature or pulSignLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (*pulSignLen < 256)
	{
		USBKey_LogErr("*pulOutLen is too small! Reqeusted length:0x%x", 256);
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	//	打开加密密钥所在的容器
	hCon = _OpenContainer(m_ulAsymmAlg, TRUE);
	if (NULL == hCon)
	{
		return USBKEY_ERR_NO_KEYSET;
	}

	//	如果是SM3，则需要导出签名公钥
	if (SGD_SM3 == m_ulHashAlg)
	{
		ULONG ulConType = 0;
		ULONG ulBlobLen = sizeof(ECCPUBLICKEYBLOB);

		SKF_GetContainerType(hCon, &ulConType);
		if (2 != ulConType)
		{
			USBKey_LogErr("SM3 is valid for SM2!");
			ulRes = USBKEY_ERR_INVALIDPARAM;
			goto FREE_MEMORY;
		}
		ulRes = SKF_ExportPublicKey(hCon, TRUE, (LPBYTE)&eccPubKeyBlob, &ulBlobLen);
		if (SAR_OK != ulRes)
		{
			USBKey_LogErr("SKF_ExportPublicKey() failed! ulRes=0x%x", ulRes);
			goto FREE_MEMORY;
		}
	}
	
	//	创建HASH句柄
	ulRes = SKF_DigestInit(m_hDev, m_ulHashAlg, &eccPubKeyBlob, (LPBYTE)lpUserID, strlen(lpUserID), &hHashKey);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_DigestInit() failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}

	//	对原数据做HASH
	ulOrgIndex = 0;
	do
	{
		ULONG ulThisTimeLen = 0;
		if (ulOrgLen - ulOrgIndex > USBKEY_CRYPTLEN_PERTIME)
		{
			ulThisTimeLen = USBKEY_CRYPTLEN_PERTIME;
		}
		else
		{
			ulThisTimeLen = ulOrgLen - ulOrgIndex;
		}
		ulRes = SKF_DigestUpdate(hHashKey, lpOrgData+ulOrgIndex, ulThisTimeLen);
		if (SAR_OK != ulRes)
		{
			USBKey_LogErr("SKF_DigestUpdate() failed! ulRes=0x%x", ulRes);
			goto FREE_MEMORY;
		}
		ulOrgIndex += ulThisTimeLen;
	}while(ulOrgIndex < ulOrgLen);
	
	//	返回HASH结果
	ulRes = SKF_DigestFinal(hHashKey, btHashValue, &ulHashLen);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_DigestFinal() failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}
		
	//	签名
	if (SGD_RSA == m_ulAsymmAlg)
	{
		ulRes = SKF_RSASignData(hCon, btHashValue, ulHashLen, lpSignature, pulSignLen);
	}
	else
	{
		ECCSIGNATUREBLOB eccSignBlob = {0};
		ulRes = SKF_ECCSignData(hCon, btHashValue, ulHashLen, &eccSignBlob);
		memcpy(lpSignature, &eccSignBlob, sizeof(ECCSIGNATUREBLOB));
		*pulSignLen = sizeof(ECCSIGNATUREBLOB);
	}
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_RSASignData() or SKF_ECCSignData() failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}

	FUNC_END;

FREE_MEMORY:
	if (hHashKey)
	{
		SKF_CloseHandle(hHashKey);
		hHashKey = NULL;
	}
	if (hCon)
	{
		SKF_CloseContainer(hCon);
		hCon = NULL;
	}

	return ulRes;
}
 /***********************************************************************
 *	名称:VerifyP1
 *	说明:验证普通格式的签名
 *	参数:lpOrgData:[IN]:原文数据
 *		 ulOrgLen:[IN]:原文数据长度
 *		 lpUserID:[IN]:签名时额外的用户描述
 *		 lpSignature:[IN]:签名数据
 *		 ulSignLen:[IN]:签名长度
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeySKFImpl::VerifyP1(LPBYTE lpOrgData, 
							   ULONG ulOrgLen, 
							   LPSTR lpUserID, 
							   LPBYTE lpSignature, 
							   ULONG ulSignLen)
{
	ULONG ulRes = 0;
	ULONG ulOrgIndex = 0;
	ULONG ulPubKeyLen = 256;
	ULONG ulHashLen = 64;
	BYTE btPubKey[256] = {0};
	BYTE btHashValue[64] = {0};
	HCONTAINER hCon = NULL;
	HANDLE hHashKey = NULL;
	
	FUNC_BEGIN;

	//	参数检查
	if (!lpOrgData || ulOrgLen == 0)
	{
		USBKey_LogErr("lpOrgData is NULL or ulOrgLen is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpSignature || ulSignLen == 0)
	{
		USBKey_LogErr("lpSignature is NULL or ulSignLen is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}

	//	打开加密密钥所在的容器
	hCon = _OpenContainer(m_ulAsymmAlg, TRUE);
	if (NULL == hCon)
	{
		return USBKEY_ERR_NO_KEYSET;
	}
	
	//	导出公钥
	ulRes = SKF_ExportPublicKey(hCon, TRUE, btPubKey, &ulPubKeyLen);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_ExportPublicKey() failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}
	
	//	创建HASH句柄
	if (SGD_SM3 == m_ulHashAlg)
	{
		ulRes = SKF_DigestInit(m_hDev, m_ulHashAlg, (PECCPUBLICKEYBLOB)btPubKey, (LPBYTE)lpUserID, strlen(lpUserID), &hHashKey);
	}
	else
	{
		ulRes = SKF_DigestInit(m_hDev, m_ulHashAlg, NULL, NULL, 0, &hHashKey);
	}
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_DigestInit() failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}

	//	对原数据做HASH
	ulOrgIndex = 0;
	do
	{
		ULONG ulThisTimeLen = 0;
		if (ulOrgLen - ulOrgIndex > USBKEY_CRYPTLEN_PERTIME)
		{
			ulThisTimeLen = USBKEY_CRYPTLEN_PERTIME;
		}
		else
		{
			ulThisTimeLen = ulOrgLen - ulOrgIndex;
		}
		ulRes = SKF_DigestUpdate(hHashKey, lpOrgData+ulOrgIndex, ulThisTimeLen);
		if (SAR_OK != ulRes)
		{
			USBKey_LogErr("SKF_DigestUpdate() failed! ulRes=0x%x", ulRes);
			goto FREE_MEMORY;
		}
		ulOrgIndex += ulThisTimeLen;
	}while(ulOrgIndex < ulOrgLen);
	
	//	返回HASH结果
	ulRes = SKF_DigestFinal(hHashKey, btHashValue, &ulHashLen);
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("SKF_DigestFinal() failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}

	//	验证签名
	if (SGD_RSA == m_ulAsymmAlg)
	{
		ulRes = SKF_RSAVerify(m_hDev, (PRSAPUBLICKEYBLOB)btPubKey, btHashValue, ulHashLen, lpSignature, ulSignLen);
	}
	else
	{
		if (sizeof(ECCSIGNATUREBLOB) != ulSignLen)
		{
			USBKey_LogErr("Signature length (dwSignLen=0x%x) is wrong! The SM2 signature length should be 0x%x.", ulSignLen, sizeof(ECCSIGNATUREBLOB));
			ulRes = USBKEY_ERR_INDATAERROR;
			goto FREE_MEMORY;
		}
		ulRes = SKF_ECCVerify(m_hDev, (PECCPUBLICKEYBLOB)btPubKey, btHashValue, ulHashLen, (PECCSIGNATUREBLOB)lpSignature);
	}
	if (SAR_OK != ulRes)
	{
		USBKey_LogErr("Verify failed! ulRes=0x%x", ulRes);
		goto FREE_MEMORY;
	}
	
	FUNC_END;

FREE_MEMORY:
	if (hHashKey)
	{
		SKF_CloseHandle(hHashKey);
		hHashKey = NULL;
	}
	if (hCon)
	{
		SKF_CloseContainer(hCon);
		hCon = NULL;
	}

	return ulRes;
}
 /***********************************************************************
 *	名称:DisconnectDev
 *	说明:断开与设备的连接，关闭应用。
 *	参数:无
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeySKFImpl::DisconnectDev(void)
{
	ULONG ulRes = 0;

	FUNC_BEGIN;

	if (m_hApplication)
	{
		SKF_CloseApplication(m_hApplication);
		m_hApplication = NULL;
	}
	if (m_hDev)
	{
		SKF_DisConnectDev(m_hDev);
		m_hDev = NULL;
	}

	FUNC_END;

	return ulRes;
}
 /***********************************************************************
 *	名称:Release
 *	说明:释放接口所加载的DLL文件
 *	参数:无
 *	返回:USBKEY_ERR_OK
 ***********************************************************************/
ULONG CUSBKeySKFImpl::Release(void)
{
	ULONG ulRes = 0;

	FUNC_BEGIN;

	if (m_hInstance)
	{
		FreeLibrary(m_hInstance);
		m_hInstance = NULL;
	}

	m_bSDKLoaded = FALSE;

	FUNC_END;

	return ulRes;
}

// 由于只支持创建一个应用，故返回第一个应用名
LPSTR CUSBKeySKFImpl::_GetApplitionName()
{	
	ULONG ulRes = 0;
	ULONG ulAppNamesLen = 0;
	CHAR* lpsAppNames = NULL;
	static CHAR  csAppName[64] = {0};
	
	FUNC_BEGIN;

	memset(csAppName, 0, 64);
	if (!m_hDev || !m_bSDKLoaded)
	{
		ZJCA_LogE(_T("m_hDev is NULL or m_bSDKLoaded is FALSE!"));
		return NULL;
	}

	ulRes = SKF_EnumApplication(m_hDev, NULL, &ulAppNamesLen);
	if (SAR_OK != ulRes || ulAppNamesLen <= 2)
	{
		ZJCA_LogE(_T("SKF_EnumApplication() failed! ulRes = 0x%x"), ulRes);
		return NULL;
	}

	lpsAppNames = new CHAR[ulAppNamesLen + 1];
	ulRes = SKF_EnumApplication(m_hDev, lpsAppNames, &ulAppNamesLen);
	if (SAR_OK != ulRes)
	{
		ZJCA_LogE(_T("SKF_EnumApplication() failed! ulRes = 0x%x"), ulRes);
		delete []lpsAppNames;
		lpsAppNames = NULL;
		return NULL;
	}
	strcpy_s(csAppName, 64, lpsAppNames);
	delete []lpsAppNames;
	lpsAppNames = NULL;

	FUNC_END;

	return csAppName;
}

HCONTAINER CUSBKeySKFImpl::_OpenContainer(ULONG ulAlgID, BOOL bSign)
{
	ULONG ulRes = USBKEY_ERR_OK;
	ULONG ulPos = 0;
	ULONG ulConType = 0;
	ULONG ulContainerNameLen = 1024;
	CHAR csContainerNames[1024] = {0};
	HCONTAINER hCon = NULL;

	FUNC_BEGIN;

	if (!m_hApplication)
	{
		USBKey_LogErr("Application is not open!");
		return NULL;
	}

	if (SGD_RSA == ulAlgID)
	{
		ulConType = 1;
	}
	else
	{
		ulConType = 2;
	}

	// 枚举所有的容器
	ulRes = SKF_EnumContainer(m_hApplication, csContainerNames, &ulContainerNameLen);
	if (ulRes != SAR_OK)
	{
		USBKey_LogErr("SKF_EnumContainer() failed! ulRes=0x%x", ulRes);
		return NULL;
	}

	// 依次打开所有容器
	while ((ulContainerNameLen > 2) && (ulPos < ulContainerNameLen) && 
		(csContainerNames[ulPos] != '\0' || csContainerNames[ulPos + 1] != '\0'))
	{
		LPSTR lpszContainer = NULL;
		ULONG ulContainerType = 0;
		ULONG ulPubKeyLen = 0;

		lpszContainer = csContainerNames + ulPos;
		ulPos += strlen(lpszContainer) + 1;
		if (strlen(lpszContainer) == 0)
		{
			continue;
		}		

		// 打开容器
		ulRes = SKF_OpenContainer(m_hApplication, lpszContainer, &hCon);
		if (SAR_OK != ulRes || !hCon)
		{
			USBKey_LogErr("SKF_OpenContainer() failed! ulRes=0x%x", ulRes);
			continue;
		}

		// 获取容器类型
		ulRes = SKF_GetContainerType(hCon, &ulContainerType);
		if (SAR_OK != ulRes || ulContainerType != ulConType)
		{
			USBKey_LogErr("Container type is incorrect, ulContainerType=0x%x", ulContainerType);
			goto CLOSE_CONTAINER;
		}

		// 检查对应的证书是否存在
		ulRes = SKF_ExportCertificate(hCon, bSign, NULL, &ulPubKeyLen);
		if (SAR_OK != ulRes || ulPubKeyLen <= 0)
		{
			USBKey_LogErr("SKF_ExportCertificate() failed! ulRes=0x%x", ulRes);
			goto CLOSE_CONTAINER;
		}
		else
		{
			break;
		}

CLOSE_CONTAINER:
		SKF_CloseContainer(hCon);
		hCon = NULL;
	}

	FUNC_END;

	return hCon;
}