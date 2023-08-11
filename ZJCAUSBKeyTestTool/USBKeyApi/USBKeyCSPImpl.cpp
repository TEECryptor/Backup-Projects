#include "stdafx.h"
#include "USBKeyCSPImpl.h"
#include "./LogFile/LogFile.h"

#define USBKEY_DES3_KEYLENGTH		0x00A80000	// 168 for 3DES
#define GLOBAL_ENCODING_TYPE		(PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)

/***********************************************************************
 *	名称:CUSBKeyCSPImpl
 *	说明:构造函数
 *	参数:无
 *	返回:无
 ***********************************************************************/
CUSBKeyCSPImpl::CUSBKeyCSPImpl(void)
 : m_ulSymmAlg(USBKEY_SYMMALG_3DES)
 , m_ulHashAlg(USBKEY_HASHALG_SHA1)
{
	memset(m_csCSPName, 0, 256);
	memset(m_csAsymmAlgOid, 0, 128);
}
/***********************************************************************
 *	名称:~CUSBKeyCSPImpl
 *	说明:析构函数
 *	参数:无
 *	返回:无
 ***********************************************************************/
CUSBKeyCSPImpl::~CUSBKeyCSPImpl(void)
{
}
/***********************************************************************
 *	名称:Create
 *	说明:尝试创建接口内部句柄
 *	参数:lpcsCspOrFile:[IN]:CSP名
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::Create(LPCSTR lpcsCspOrFile)
{
	ULONG ulRes = 0;
	ULONG ulError = 0;
	HCRYPTPROV hProv = NULL;
	
	FUNC_BEGIN;

	//	参数检查
	if (!lpcsCspOrFile || strlen(lpcsCspOrFile) == 0)
	{
		USBKey_LogErr("lpcsCspOrFile is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	//	打开该CSP的缺省容器
	if (!CryptAcquireContextA(&hProv, NULL, lpcsCspOrFile, PROV_RSA_FULL, 0))
	{
		ulError = GetLastError();
		if (0 == ulError) ulRes = ulError ? ulError : USBKEY_ERR_FAILED;
		USBKey_LogErr("CryptAcquireContext() failed! ulError = 0x%x\n", ulError);
		return ulRes;
	}

	//	保存CSP名
	strcpy_s(m_csCSPName, 256, lpcsCspOrFile);

	//	释放掉
	CryptReleaseContext(hProv, 0);

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
ULONG CUSBKeyCSPImpl::Initialize(ULONG ulAsymmAlg, 
								 ULONG ulSymmAlg, 
								 ULONG ulHashAlg, 
								 LPSTR lpUserPIN)
{
	ULONG ulRes = USBKEY_ERR_OK;

	FUNC_BEGIN;

	ZJCA_LogI(_T("AsymmAlg:0x%x, SymmAlg:0x%x, HashAlg:0x%x"), ulAsymmAlg, ulSymmAlg, ulHashAlg);

	if (USBKEY_ASYMMALG_RSA == ulAsymmAlg)
	{
		strcpy_s(m_csAsymmAlgOid, 128, szOID_RSA_RSA);
	}
	else
	{
		USBKey_LogErr("ulAsymmAlg is wrong! ulAsymmAlg = 0x%x\n", ulAsymmAlg);
		return USBKEY_ERR_INVALIDPARAM;
	}

	if (USBKEY_SYMMALG_3DES == ulSymmAlg)
	{
		m_ulSymmAlg = CALG_3DES;
	}
	else
	{
		USBKey_LogErr("ulSymmAlg is wrong! ulSymmAlg = 0x%x\n", ulSymmAlg);
		return USBKEY_ERR_INVALIDPARAM;
	}

	if (USBKEY_HASHALG_SHA1 == ulHashAlg)
	{
		m_ulHashAlg = CALG_SHA1;
	}
	else if (USBKEY_HASHALG_SHA256 == ulHashAlg)
	{
		m_ulHashAlg = CALG_SHA_256;
	}
	else if (USBKEY_HASHALG_MD5 == ulHashAlg)
	{
		m_ulHashAlg = CALG_MD5;
	}
	else
	{
		USBKey_LogErr("ulHashAlg is wrong! ulHashAlg = 0x%x\n", ulHashAlg);
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
 *	说明:等待设备事件函数，CSP不支持。
 *	参数:忽略
 *	返回:USBKEY_ERR_NOT_SUPPORT
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::WaitForDevEvent(BOOL bNotBlock, 
									  ULONG *pulSoltID, 
									  LPSTR lpDevName,
									  ULONG *pulNameLen, 
									  ULONG *pulEventType)
{
	ZJCA_LogE(_T("This function is not supported!"));
	return USBKEY_ERR_NOT_SUPPORT;
}
/***********************************************************************
 *	名称:CancelWaitForDevEvent
 *	说明:取消等待设备事件函数，CSP不支持。
 *	参数:忽略
 *	返回:USBKEY_ERR_OK
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::CancelWaitForDevEvent()
{
	ZJCA_LogI(_T("This function return OK always!"));
	return USBKEY_ERR_OK;
}
/***********************************************************************
 *	名称:ConnectDev
 *	说明:连接设备，CSP不支持。
 *	参数:无
 *	返回:USBKEY_ERR_OK
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::ConnectDev()
{
	ZJCA_LogI(_T("This function return OK always!"));
	return USBKEY_ERR_OK;
}
/***********************************************************************
 *	名称:Envelop
 *	说明:按P7数字信封格式封装数据
 *	参数:lpInData:[IN]:明文数据
 *		 ulInLen:[IN]:明文数据长度
 *		 lpOutData:[OUT]:返回的数字信封数据
 *		 pulOutLen:[IN/OUT]:数字信封数据长度
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::Envelop(LPBYTE lpInData, 
							  ULONG ulInLen,
							  LPBYTE lpOutData,
							  ULONG* pulOutLen)
{
	ULONG ulRes = 0;
	ULONG ulEnvelopedLen = 0;
	HCRYPTPROV hProv = NULL;
	PCCERT_CONTEXT pCert = NULL;
	PCERT_INFO pCertInfo = NULL;
	HCRYPTPROV hMSProv = NULL;
	HCRYPTMSG hMsg = NULL;

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

	// 打开加密密钥所在的容器
	hProv = OpenCSPProvider(AT_KEYEXCHANGE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// 导出加密证书
	pCert = ExportCertificate(hProv, FALSE);
	if (NULL == pCert)
	{
		USBKey_LogErr("No certificate!");
		ulRes = USBKEY_ERR_NO_CERT;
		goto FREE_MEMORY;
	}
	
	// 打开微软RSA CSP
	if (!CryptAcquireContext(&hMSProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptAcquireContext() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	 
	// 数字信封接受者
	PCERT_INFO recipCertArray[] = { pCert->pCertInfo };

	// 算法参数
	CRYPT_ALGORITHM_IDENTIFIER ContentEncryptAlgorithm;
	memset(&ContentEncryptAlgorithm, 0, sizeof(ContentEncryptAlgorithm));
	ContentEncryptAlgorithm.pszObjId = szOID_RSA_SMIMEalgCMS3DESwrap;

	// 加密信息参数
	CMSG_ENVELOPED_ENCODE_INFO encodeInfo;
	memset(&encodeInfo, 0, sizeof(CMSG_ENVELOPED_ENCODE_INFO));
	encodeInfo.cbSize = sizeof(CMSG_ENVELOPED_ENCODE_INFO);
	encodeInfo.hCryptProv = hMSProv;
	encodeInfo.ContentEncryptionAlgorithm = ContentEncryptAlgorithm;
	encodeInfo.pvEncryptionAuxInfo = NULL;
	encodeInfo.cRecipients = 1;
	encodeInfo.rgpRecipients = recipCertArray;

	// 计算数字信封结果长度
	ulEnvelopedLen = CryptMsgCalculateEncodedLength(GLOBAL_ENCODING_TYPE, 0, CMSG_ENVELOPED, &encodeInfo, NULL, ulInLen);
	if (ulEnvelopedLen == 0)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgCalculateEncodedLength() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	if (*pulOutLen < ulEnvelopedLen)
	{
		ulRes = USBKEY_ERR_BUFFER_TOO_SMALL;
		ZJCA_LogE(_T("Buffer is to small! Requested size:0x%x"), ulEnvelopedLen);
		goto FREE_MEMORY;
	}

	// 打开打包消息句柄
	hMsg = CryptMsgOpenToEncode(GLOBAL_ENCODING_TYPE, 0, CMSG_ENVELOPED, &encodeInfo, NULL, NULL);
	if (hMsg == NULL)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgOpenToEncode() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 添加原文
	if (!CryptMsgUpdate(hMsg, lpInData, ulInLen, TRUE))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgUpdate() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 获取数字信封结果
	if (!CryptMsgGetParam(hMsg, CMSG_CONTENT_PARAM, 0, lpOutData, pulOutLen))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgGetParam() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	
	FUNC_END;

FREE_MEMORY:
	if (hMsg)
	{
		CryptMsgClose(hMsg);
		hMsg = NULL;
	}
	if (pCert)
	{
		CertFreeCertificateContext(pCert);
		pCert = NULL;
	}
	if (hMSProv)
	{
		CryptReleaseContext(hMSProv, 0);
		hMSProv = NULL;
	}
	if (hProv)
	{
		CryptReleaseContext(hProv, 0);
		hProv = NULL;
	}

	return ulRes;
}
/***********************************************************************
 *	名称:Develop
 *	说明:解密P7格式的数字信封数据
 *	参数:lpInData:[IN]:P7格式的数字信封数据
 *		 ulInLen:[IN]:数字信封数据长度
 *		 lpOutData:[OUT]:解密后的数据
 *		 pulOutLen:[IN/OUT]:解密后的数据长度
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::Develop(LPBYTE lpInData, 
							  ULONG ulInLen, 
							  LPBYTE lpOutData,
							  ULONG* pulOutLen)
{
	ULONG ulRes = 0;
	ULONG ulRecipInfoLen = 0;
	ULONG ulDecryptedLen = 0;
	LPBYTE lpRecipInfo = NULL;
	HCRYPTPROV hProv = NULL;
	HCRYPTMSG hDecryptMsg = NULL;
	PCERT_INFO pRecipCertInfo = NULL;
	CMSG_CTRL_DECRYPT_PARA decryptParam = {0};
	
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

	// 获取加密密钥所在容器
	hProv = OpenCSPProvider(AT_KEYEXCHANGE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}
			
	// 获取解密消息句柄
	hDecryptMsg = CryptMsgOpenToDecode(GLOBAL_ENCODING_TYPE, 0, 0, NULL, NULL, NULL);
	if (NULL == hDecryptMsg)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgOpenToDecode() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 更新密文
	if (!CryptMsgUpdate(hDecryptMsg, lpInData, ulInLen, TRUE))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgUpdate() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 获取接受者证书信息(目前只支持一个接受者，所以dwIndex = 0)
	if (!CryptMsgGetParam(hDecryptMsg, CMSG_RECIPIENT_INFO_PARAM, 0, NULL, &ulRecipInfoLen) || ulRecipInfoLen == 0)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgGetParam() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	lpRecipInfo = new BYTE[ulRecipInfoLen];
	if (!CryptMsgGetParam(hDecryptMsg, CMSG_RECIPIENT_INFO_PARAM, 0, lpRecipInfo, &ulRecipInfoLen))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgGetParam() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	
	// 解密参数
	decryptParam.cbSize = sizeof(CMSG_CTRL_DECRYPT_PARA);
	decryptParam.dwKeySpec = AT_KEYEXCHANGE;
	decryptParam.hCryptProv = hProv;
	decryptParam.dwRecipientIndex = 0;

	// 设置解密参数
	if (!CryptMsgControl(hDecryptMsg, 0, CMSG_CTRL_DECRYPT, &decryptParam))
	{
		ulRes = GetLastError();	
		USBKey_LogErr("CryptMsgControl() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 获取解密后的结果大小
	if (!CryptMsgGetParam(hDecryptMsg, CMSG_CONTENT_PARAM, 0, NULL, &ulDecryptedLen) || ulDecryptedLen == 0)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgGetParam() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 获取解密结果
	if (*pulOutLen < ulDecryptedLen)
	{
		ulRes = USBKEY_ERR_BUFFER_TOO_SMALL;
		USBKey_LogErr("lpOutData buffer is too small! Requested length = 0x%x", ulDecryptedLen);
		goto FREE_MEMORY;
	}
	if (!CryptMsgGetParam(hDecryptMsg, CMSG_CONTENT_PARAM, 0, lpOutData, pulOutLen))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgGetParam() failed! ulRes = 0x%x\n", ulRes);
		goto FREE_MEMORY;
	}
	
	FUNC_END;

FREE_MEMORY:
	if (lpRecipInfo)
	{
		delete []lpRecipInfo;
		lpRecipInfo = NULL;
	}
	if (hDecryptMsg)
	{
		CryptMsgClose(hDecryptMsg);
		hDecryptMsg = NULL;
	}
	if (hProv)
	{
		CryptReleaseContext(hProv, 0);
		hProv = NULL;
	}

	return ulRes;
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
ULONG CUSBKeyCSPImpl::Encrypt(LPBYTE lpInData, 
							  ULONG ulInLen,
							  LPBYTE lpOutData, 
							  ULONG* pulOutLen)
{
	ULONG ulRes = 0;
	DWORD dwWrappedBlobLen = 0;
	LPBYTE lpWrappedBlob = NULL;
	HCRYPTPROV hProv = NULL;
	HCRYPTKEY hSymmKey = NULL;
	HCRYPTKEY hExchPubKey = NULL;
	DWORD dwPlainIndex = 0;
	DWORD dwEnvelopedIndex = 0;
	DWORD dwCipherBuffLen = USBKEY_CRYPTLEN_PERTIME + 128;
	BYTE btCipherBuff[USBKEY_CRYPTLEN_PERTIME + 128] = {0};
	
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

	// 获取加密密钥所在容器
	hProv = OpenCSPProvider(AT_KEYEXCHANGE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}
	
	// 随机生成会话密钥
	if (!CryptGenKey(hProv, CALG_3DES, USBKEY_DES3_KEYLENGTH|CRYPT_EXPORTABLE, &hSymmKey))
    {
		ulRes = GetLastError();
		USBKey_LogErr("CryptGenKey() failed! ulRes = 0x%x", ulRes);
		return ulRes;
    }

	//	获取密钥对句柄
	if (!CryptGetUserKey(hProv, AT_KEYEXCHANGE, &hExchPubKey))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptGetUserKey() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	
	// 导出会话密钥Blob
	if (!CryptExportKey(hSymmKey, hExchPubKey, SIMPLEBLOB, 0, NULL, &dwWrappedBlobLen) || dwWrappedBlobLen == 0)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptExportKey() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	lpWrappedBlob = new BYTE[dwWrappedBlobLen];
	if (!CryptExportKey(hSymmKey, hExchPubKey, SIMPLEBLOB, 0, lpWrappedBlob, &dwWrappedBlobLen))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptExportKey() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 拷贝被保护的会话密钥数据
	dwEnvelopedIndex = 0;
	memcpy(lpOutData, &dwWrappedBlobLen, 4);
	dwEnvelopedIndex = 4;
	memcpy(lpOutData + dwEnvelopedIndex, lpWrappedBlob, dwWrappedBlobLen);
	dwEnvelopedIndex += dwWrappedBlobLen;

	// 加密数据
	dwPlainIndex = 0;
	do
	{
		BOOL bFinsh = FALSE;
		DWORD dwThisTimeLen = 0;
		if ((ulInLen - dwPlainIndex) > USBKEY_CRYPTLEN_PERTIME)
		{
			dwThisTimeLen = USBKEY_CRYPTLEN_PERTIME;
		}
		else
		{
			bFinsh = TRUE;
			dwThisTimeLen = ulInLen - dwPlainIndex;
		}
		memcpy(btCipherBuff, lpInData+dwPlainIndex, dwThisTimeLen);
		dwPlainIndex += dwThisTimeLen;
		//
		dwCipherBuffLen = USBKEY_CRYPTLEN_PERTIME + 128;
		if (!CryptEncrypt(hSymmKey, NULL, bFinsh, 0, btCipherBuff, &dwThisTimeLen, dwCipherBuffLen))
		{
			ulRes = GetLastError();
			USBKey_LogErr("CryptEncrypt() failed! ulRes = 0x%x\n", ulRes);
			goto FREE_MEMORY;
		}
		memcpy(lpOutData + dwEnvelopedIndex, btCipherBuff, dwThisTimeLen);
		dwEnvelopedIndex += dwThisTimeLen;
	}while(dwPlainIndex < ulInLen);

	*pulOutLen = dwEnvelopedIndex;
	
	FUNC_END;

FREE_MEMORY:
	if (lpWrappedBlob)
	{
		delete []lpWrappedBlob;
		lpWrappedBlob = NULL;
	}
	if (hExchPubKey)
	{
		CryptDestroyKey(hExchPubKey);
		hExchPubKey = NULL;
	}
	if (hSymmKey)
	{
		CryptDestroyKey(hSymmKey);
		hSymmKey = NULL;
	}
	if (hProv)
	{
		CryptReleaseContext(hProv, 0);
		hProv = NULL;
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
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::Decrypt(LPBYTE lpInData, 
							  ULONG ulInLen,
							  LPBYTE lpOutData, 
							  ULONG* pulOutLen)
{
	ULONG ulRes = 0;
	ULONG ulIndex = 0;
	ULONG ulBlobLen = 0;
	ULONG ulEncryptedDataLen = 0;
	LPBYTE lpWrapBlob = NULL;
	LPBYTE lpCipher = NULL;
	HCRYPTPROV hProv = NULL;
	HCRYPTKEY hExchKey = NULL;
	HCRYPTKEY hSymmKey = NULL;
	DWORD dwCipherIndex = 0;
	DWORD dwDecryptedIndex = 0;
	BYTE btDecryptedBuff[USBKEY_CRYPTLEN_PERTIME] = {0};
	
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

	// 获取加密密钥所在容器
	hProv = OpenCSPProvider(AT_KEYEXCHANGE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// 获取交换密钥对句柄，用来解密会话密钥
	if (!CryptGetUserKey(hProv, AT_KEYEXCHANGE, &hExchKey))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptGetUserKey() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	//　获取被交换公钥加密的会话密钥数据
	ulIndex = 0;
	memcpy(&ulBlobLen, lpInData + ulIndex, 4);
	ulIndex += 4;
	if (ulBlobLen > (sizeof(BLOBHEADER) + sizeof(RSAPUBKEY) + 256))
	{
		ulRes = 3;
		USBKey_LogErr("The wrapped key length is wrong! ulBlobLen = 0x%x", ulBlobLen);
		goto FREE_MEMORY;
	}
	lpWrapBlob = new BYTE[ulBlobLen];
	memcpy(lpWrapBlob, lpInData + ulIndex, ulBlobLen);
	ulIndex += ulBlobLen;

	// 导入会话密钥
	if (!CryptImportKey(hProv, lpWrapBlob, ulBlobLen, hExchKey, 0, &hSymmKey))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptImportKey() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 被会话密钥保护的密文
	ulEncryptedDataLen = ulInLen - ulIndex;
	lpCipher = lpInData + ulIndex;

	// 使用会话密钥解密密文	
	dwCipherIndex = 0;
	dwDecryptedIndex = 0;
	do
	{
		BOOL bFinsh = FALSE;
		DWORD dwThisTimeLen = 0;
		if ((ulEncryptedDataLen - dwCipherIndex) > USBKEY_CRYPTLEN_PERTIME)
		{
			dwThisTimeLen = USBKEY_CRYPTLEN_PERTIME;
		}
		else
		{
			bFinsh = TRUE;
			dwThisTimeLen = ulEncryptedDataLen - dwCipherIndex;
		}
		memcpy(btDecryptedBuff, lpCipher+dwCipherIndex, dwThisTimeLen);
		dwCipherIndex += dwThisTimeLen;
		if (!CryptDecrypt(hSymmKey, NULL, bFinsh, 0, btDecryptedBuff, &dwThisTimeLen))
		{
			ulRes = GetLastError();
			if (0 != ulRes)
			{
				USBKey_LogErr("CryptDecrypt() failed! ulRes = 0x%x", ulRes);
				goto FREE_MEMORY;
			}
		}
		memcpy(lpOutData + dwDecryptedIndex, btDecryptedBuff, dwThisTimeLen);
		dwDecryptedIndex += dwThisTimeLen;
	}while(dwCipherIndex < ulEncryptedDataLen);

	*pulOutLen = dwDecryptedIndex;	

	FUNC_END;

FREE_MEMORY:
	if (lpWrapBlob)
	{
		delete []lpWrapBlob;
		lpWrapBlob = NULL;
	}
	if (hSymmKey)
	{
		CryptDestroyKey(hSymmKey);
		hSymmKey = NULL;
	}
	if (hExchKey)
	{
		CryptDestroyKey(hExchKey);
		hExchKey = NULL;
	}
	if (hProv)
	{
		CryptReleaseContext(hProv, 0);
		hProv = NULL;
	}

	return ulRes;
}
/***********************************************************************
 *	名称:SignP7
 *	说明:按P7格式签名数据
 *	参数:lpOrgData:[IN]:原文数据
 *		 ulOrgLen:[IN]:原文数据长度
 *		 lpSignature:[OUT]:签名数据
 *		 pulSignLen:[IN/OUT]:签名长度
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::SignP7(LPBYTE lpOrgData, 
							 ULONG ulOrgLen,
							 LPBYTE lpSignature,
							 ULONG* pulSignLen)
{
	ULONG ulRes = 0;
	ULONG ulSignLen = 0;
	CMSG_SIGNER_ENCODE_INFO sei = {0};
	CMSG_SIGNED_ENCODE_INFO signedEncodeInfo = {0};
	CERT_BLOB signerCertBlob = {0};
	PCCERT_CONTEXT pSignCert = NULL;
	HCRYPTPROV hProv = NULL;
	HCRYPTMSG hSignMsg = NULL;
	
	FUNC_BEGIN;

	// 参数检查
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

	// 获取加密密钥所在容器
	hProv = OpenCSPProvider(AT_SIGNATURE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// 导出签名证书
	pSignCert = ExportCertificate(hProv, TRUE);
	if (!pSignCert)
	{
		USBKey_LogErr("No certificate!");
		ulRes = USBKEY_ERR_NO_CERT;
		goto FREE_MEMORY;
	}

	// 填充签名者信息结构体
	ZeroMemory(&sei, sizeof(CMSG_SIGNER_ENCODE_INFO));
	sei.cbSize = sizeof(CMSG_SIGNER_ENCODE_INFO);
	sei.pCertInfo = pSignCert->pCertInfo;
	sei.hCryptProv = hProv;
	sei.dwKeySpec = AT_SIGNATURE;
	sei.HashAlgorithm.pszObjId = szOID_OIWSEC_sha1;
	sei.pvHashAuxInfo = NULL;
	sei.cAuthAttr = 0;
	sei.rgAuthAttr = NULL;

	// 填充签名编码结构体
	ZeroMemory(&signedEncodeInfo, sizeof(CMSG_SIGNED_ENCODE_INFO));
	signedEncodeInfo.cbSize = sizeof(CMSG_SIGNED_ENCODE_INFO);
	signedEncodeInfo.cSigners = 1;
	signedEncodeInfo.rgSigners = &sei;
	{
		signerCertBlob.cbData = pSignCert->cbCertEncoded;
		signerCertBlob.pbData = pSignCert->pbCertEncoded;

		signedEncodeInfo.cCertEncoded = 1;
		signedEncodeInfo.rgCertEncoded = &signerCertBlob;
	}
		
	// 计算签名后的数字长度
	ulSignLen = CryptMsgCalculateEncodedLength(GLOBAL_ENCODING_TYPE, CMSG_DETACHED_FLAG, CMSG_SIGNED, &signedEncodeInfo, szOID_RSA_signedData, ulOrgLen);
	if (0 == ulSignLen)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgCalculateEncodedLength() failed! ulRes = 0x%x", ulRes);		
		goto FREE_MEMORY;
	}	

	// 返回结果长度
	if (!lpSignature)
	{
		ulRes = USBKEY_ERR_OK;
		*pulSignLen = ulSignLen;
		goto FREE_MEMORY;
	}
	if (*pulSignLen < ulSignLen)
	{
		ulRes = USBKEY_ERR_BUFFER_TOO_SMALL;
		USBKey_LogErr("The buffer is to small!");	
		goto FREE_MEMORY;
	}

	// 打开签名消息句柄
	hSignMsg = CryptMsgOpenToEncode(GLOBAL_ENCODING_TYPE, CMSG_DETACHED_FLAG, CMSG_SIGNED, &signedEncodeInfo, NULL, NULL);
	if (NULL == hSignMsg)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgOpenToEncode() failed! ulRes = 0x%x", ulRes);	
		goto FREE_MEMORY;
	}

	// 签名数据
	if (!CryptMsgUpdate(hSignMsg, lpOrgData, ulOrgLen, TRUE))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgUpdate() failed! ulRes = 0x%x", ulRes);	
		goto FREE_MEMORY;
	}
		
	// 获取签名后的结果
	if (!CryptMsgGetParam(hSignMsg, CMSG_CONTENT_PARAM, 0, lpSignature, &ulSignLen))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgGetParam() failed! ulRes = 0x%x", ulRes);	
		goto FREE_MEMORY;
	} 
	*pulSignLen = ulSignLen;
	
	FUNC_END;

FREE_MEMORY:
	if (hSignMsg)
	{
		CryptMsgClose(hSignMsg);
		hSignMsg = NULL;
	}
	if (pSignCert)
	{
		CertFreeCertificateContext(pSignCert);
		pSignCert = NULL;
	}
	if (hProv)
	{
		CryptReleaseContext(hProv, 0);
		hProv = NULL;
	}

	return ulRes;
}
/***********************************************************************
 *	名称:VerifyP7
 *	说明:验证P7格式的签名
 *	参数:lpOrgData:[IN]:原文数据
 *		 ulOrgLen:[IN]:原文数据长度
 *		 lpSignature:[IN]:签名数据
 *		 ulSignLen:[IN]:签名长度
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::VerifyP7(LPBYTE lpOrgData, 
							   ULONG ulOrgLen, 
							   LPBYTE lpSignature,
							   ULONG ulSignLen)
{
	ULONG ulRes = 0;
	HCRYPTPROV hProv = NULL;
	HCRYPTMSG hVerifyMsg = NULL;
	PCCERT_CONTEXT pSignCert = NULL;	
	
	FUNC_BEGIN;

	// 参数检查
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

	// 获取加密密钥所在容器
	hProv = OpenCSPProvider(AT_SIGNATURE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	//	导出签名证书
	pSignCert = ExportCertificate(hProv, TRUE);
	if (!pSignCert)
	{
		USBKey_LogErr("No certificate!");
		ulRes = USBKEY_ERR_NO_CERT;
		goto FREE_MEMORY;
	}
	
	// 打开消息句柄
	hVerifyMsg = CryptMsgOpenToDecode(GLOBAL_ENCODING_TYPE, CMSG_DETACHED_FLAG, NULL, NULL, NULL, NULL);
	if (NULL == hVerifyMsg)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgOpenToDecode() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 设置签名值			
	if (!CryptMsgUpdate(hVerifyMsg, lpSignature, ulSignLen, TRUE))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgUpdate() failed! ulRes = 0x%x\n", ulRes);
		goto FREE_MEMORY;
	}

	// 设置原文			
	if (!CryptMsgUpdate(hVerifyMsg, lpOrgData, ulOrgLen, TRUE))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgUpdate() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 权限控制				
	if (!CryptMsgControl(hVerifyMsg, 0, CMSG_CTRL_VERIFY_SIGNATURE, pSignCert->pCertInfo))
	{
		ulRes = GetLastError();
		USBKey_LogErr("Verify failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	else
	{
		ulRes = USBKEY_ERR_OK;
	}
	
	FUNC_END;

FREE_MEMORY:
	if (hVerifyMsg)
	{
		CryptMsgClose(hVerifyMsg);
		hVerifyMsg = NULL;
	}
	if (pSignCert)
	{
		CertFreeCertificateContext(pSignCert);
		pSignCert = NULL;
	}
	if (hProv)
	{
		CryptReleaseContext(hProv, 0);
		hProv = NULL;
	}

	return ulRes;
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
ULONG CUSBKeyCSPImpl::SignP1(LPBYTE lpOrgData, 
							 ULONG ulOrgLen, 
							 LPSTR lpUserID, 
							 LPBYTE lpSignature,
							 ULONG* pulSignLen)
{
	ULONG ulRes = 0;
	HCRYPTPROV hProv = NULL;
	HCRYPTHASH hHash = NULL;
	
	FUNC_BEGIN;

	// 参数检查
	if (!lpOrgData || ulOrgLen == 0)
	{
		USBKey_LogErr("lpOrgData is NULL or ulOrgLen is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpSignature || pulSignLen == 0)
	{
		USBKey_LogErr("lpSignature or pulSignLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}

	// 获取加密密钥所在容器
	hProv = OpenCSPProvider(AT_SIGNATURE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// 创建HASH对象
	if (!CryptCreateHash(hProv, m_ulHashAlg, NULL, 0, &hHash))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptCreateHash() failed! ulRes = 0x%x", ulRes);
		return ulRes;
	}

	// 对原文HASH
	if (!CryptHashData(hHash, lpOrgData, ulOrgLen, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptHashData() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 对HASH签名
	if (!CryptSignHashA(hHash, AT_SIGNATURE, (!lpUserID || strlen(lpUserID) == 0) ? NULL : (LPCSTR)lpUserID, 0, lpSignature, pulSignLen))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptSignHashA() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	FUNC_END;
	
FREE_MEMORY:
	if (hHash)
	{
		CryptDestroyHash(hHash);
		hHash = NULL;
	}
	if (hProv)
	{
		CryptReleaseContext(hProv, 0);
		hProv = NULL;
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
ULONG CUSBKeyCSPImpl::VerifyP1(LPBYTE lpOrgData,
							   ULONG ulOrgLen, 
							   LPSTR lpUserID, 
							   LPBYTE lpSignature, 
							   ULONG ulSignLen)
{
	ULONG ulRes = 0;
	HCRYPTPROV hProv = NULL;
	HCRYPTHASH hHash = NULL;
	PCERT_INFO pCertInfo = NULL;
	HCRYPTKEY hPubKey = NULL;
	PCCERT_CONTEXT pSignCert = NULL;
	
	FUNC_BEGIN;

	// 参数检查
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

	// 获取加密密钥所在容器
	hProv = OpenCSPProvider(AT_SIGNATURE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	//	导出签名证书
	pSignCert = ExportCertificate(hProv, TRUE);
	if (!pSignCert)
	{
		USBKey_LogErr("No certificate!");
		ulRes = USBKEY_ERR_NO_CERT;
		goto FREE_MEMORY;
	}

	// 创建HASH对象
	if (!CryptCreateHash(hProv, CALG_SHA1, NULL, 0, &hHash))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptCreateHash() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 对原文HASH
	if (!CryptHashData(hHash, lpOrgData, ulOrgLen, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptHashData() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 导入验签证书的公钥信息
	pCertInfo = pSignCert->pCertInfo;
	if (!pCertInfo)
	{
		ulRes = 3;
		USBKey_LogErr("pSignCert->pCertInfo is NULL! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	if (!CryptImportPublicKeyInfo(hProv, GLOBAL_ENCODING_TYPE, &pCertInfo->SubjectPublicKeyInfo, &hPubKey))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptImportPublicKeyInfo() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// 验签
	if (!CryptVerifySignatureA(hHash, lpSignature, ulSignLen, hPubKey, (!lpUserID || strlen(lpUserID) == 0) ? NULL : (LPCSTR)lpUserID, 0))
	{
		ulRes = GetLastError();
		if (NTE_BAD_SIGNATURE == ulRes)
		{
			ulRes = E_FAIL;
			USBKey_LogErr("Verify failed! ulRes = 0x%x", ulRes);
		}
		else
		{
			USBKey_LogErr("CryptVerifySignatureA() failed! ulRes = 0x%x\n", ulRes);
		}
	}
	else
	{
		ulRes = USBKEY_ERR_OK;
	}
	
	FUNC_END;

FREE_MEMORY:
	if (hPubKey)
	{
		CryptDestroyKey(hPubKey);
		hPubKey = NULL;
	}
	if (hHash)
	{
		CryptDestroyHash(hHash);
		hHash = NULL;
	}
	if (pSignCert)
	{
		CertFreeCertificateContext(pSignCert);
		pSignCert = NULL;
	}
	if (hProv)
	{
		CryptReleaseContext(hProv, 0);
		hProv = NULL;
	}

	return ulRes;
}
 /***********************************************************************
 *	名称:DisconnectDev
 *	说明:断开与设备的连接，CSP无对应的操作。
 *	参数:无
 *	返回:USBKEY_ERR_OK
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::DisconnectDev(void)
{
	ZJCA_LogI(_T("This function return OK always!"));
	return USBKEY_ERR_OK;
}
 /***********************************************************************
 *	名称:Release
 *	说明:释放接口对象
 *	参数:无
 *	返回:USBKEY_ERR_OK
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::Release(void)
{
	memset(m_csCSPName, 0, 256);
	memset(m_csAsymmAlgOid, 0, 128);
	m_ulSymmAlg = CALG_3DES;
	m_ulHashAlg = CALG_SHA1;

	ZJCA_LogI(_T("This function return OK always!"));
	
	return USBKEY_ERR_OK;
}

/*	打开指定的CSP密钥容器 */					 
HCRYPTPROV CUSBKeyCSPImpl::OpenCSPProvider(ULONG ulKeyUsage)
{
	ULONG ulRes = 0;
	ULONG ulEnumFlag = CRYPT_FIRST;
	ULONG ulContainerLen = 512;
	ULONG ulKeyAlg = 0;
	ULONG ulKeyAlgLen = 4;
	CHAR csContainer[512] = {0};
	HCRYPTPROV hDefaultProv = NULL;
	HCRYPTPROV hFoundProv = NULL;
	PCCERT_CONTEXT pCert = NULL;
	
	FUNC_BEGIN;

	//	参数检查
	if (strlen(m_csCSPName) == 0)
	{
		ZJCA_LogE(_T("m_csCSPName is NULL!"));
		return NULL;
	}
	
	//	打开该CSP的缺省容器
	if (!CryptAcquireContextA(&hDefaultProv, NULL, m_csCSPName, PROV_RSA_FULL, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptAcquireContext() failed! ulRes = 0x%x\n", ulRes);
		return NULL;
	}

	//	枚举所有容器
	ulEnumFlag = CRYPT_FIRST;
	while (CryptGetProvParam(hDefaultProv, PP_ENUMCONTAINERS, (LPBYTE)csContainer, &ulContainerLen, ulEnumFlag))
	{
		if (CryptAcquireContextA(&hFoundProv, csContainer, m_csCSPName, PROV_RSA_FULL, 0))
		{			
			pCert = ExportCertificate(hFoundProv, (AT_SIGNATURE == ulKeyUsage) ? TRUE : FALSE);
			if (pCert)
			{
				CertFreeCertificateContext(pCert);
				pCert = NULL;
				break;
			}
		}

		ulContainerLen = 512;
		ulEnumFlag = CRYPT_NEXT;
		CryptReleaseContext(hFoundProv, 0);
		hFoundProv = NULL;
	}

	//	释放句柄
	if (hDefaultProv)
	{
		CryptReleaseContext(hDefaultProv, 0);
		hDefaultProv = NULL;
	}

	FUNC_END;

	return hFoundProv;
}

/*	导出CSP密钥容器中的证书 */		
PCCERT_CONTEXT CUSBKeyCSPImpl::ExportCertificate(HCRYPTPROV hProv, 
												 BOOL bSignCert)
{
	ULONG ulRes = 0;
	ULONG ulCertLen = 0;
	BYTE btUsage[2] = {0};
	LPBYTE lpCertData = NULL;
	HCRYPTKEY hKeyPair = NULL;
	PCCERT_CONTEXT pCertContext = NULL;
	
	FUNC_BEGIN;

	if (!hProv)
	{
		ZJCA_LogE(_T("hProv is NULL!"));
		return NULL;
	}

	//	获取密钥对句柄
	if (!CryptGetUserKey(hProv, bSignCert ? AT_SIGNATURE : AT_KEYEXCHANGE, &hKeyPair))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptGetUserKey() failed! ulRes = 0x%x\n", ulRes);
		goto FREE_MEMORY;
	}

	//	获取对应证书长度
	if (!CryptGetKeyParam(hKeyPair, KP_CERTIFICATE, NULL, &ulCertLen, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptGetKeyParam() failed! ulRes = 0x%x\n", ulRes);
		goto FREE_MEMORY;
	}

	//	获取证书内容
	lpCertData = new BYTE[ulCertLen];
	if (!CryptGetKeyParam(hKeyPair, KP_CERTIFICATE, lpCertData, &ulCertLen, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptGetKeyParam() failed! ulRes = 0x%x\n", ulRes);
		goto FREE_MEMORY;
	}

	//	解析证书
	pCertContext = CertCreateCertificateContext(GLOBAL_ENCODING_TYPE, lpCertData, ulCertLen);
	if (!pCertContext)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CertCreateCertificateContext() failed! ulRes = 0x%x\n", ulRes);
		goto FREE_MEMORY;
	}
	
	//	判断是否是RSA证书					
	if (_stricmp(pCertContext->pCertInfo->SubjectPublicKeyInfo.Algorithm.pszObjId, szOID_RSA_RSA) != 0)
	{
		CertFreeCertificateContext(pCertContext);
		pCertContext = NULL;
		goto FREE_MEMORY;
	}
	
	FUNC_END;

FREE_MEMORY:
	if (lpCertData)
	{
		delete []lpCertData;
		lpCertData = NULL;
	}
	if (hKeyPair)
	{
		CryptDestroyKey(hKeyPair);
		hKeyPair = NULL;
	}

	return pCertContext;
}
