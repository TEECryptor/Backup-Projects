#include "stdafx.h"
#include "USBKeyCSPImpl.h"
#include "./LogFile/LogFile.h"

#define USBKEY_DES3_KEYLENGTH		0x00A80000	// 168 for 3DES
#define GLOBAL_ENCODING_TYPE		(PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)

/***********************************************************************
 *	����:CUSBKeyCSPImpl
 *	˵��:���캯��
 *	����:��
 *	����:��
 ***********************************************************************/
CUSBKeyCSPImpl::CUSBKeyCSPImpl(void)
 : m_ulSymmAlg(USBKEY_SYMMALG_3DES)
 , m_ulHashAlg(USBKEY_HASHALG_SHA1)
{
	memset(m_csCSPName, 0, 256);
	memset(m_csAsymmAlgOid, 0, 128);
}
/***********************************************************************
 *	����:~CUSBKeyCSPImpl
 *	˵��:��������
 *	����:��
 *	����:��
 ***********************************************************************/
CUSBKeyCSPImpl::~CUSBKeyCSPImpl(void)
{
}
/***********************************************************************
 *	����:Create
 *	˵��:���Դ����ӿ��ڲ����
 *	����:lpcsCspOrFile:[IN]:CSP��
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::Create(LPCSTR lpcsCspOrFile)
{
	ULONG ulRes = 0;
	ULONG ulError = 0;
	HCRYPTPROV hProv = NULL;
	
	FUNC_BEGIN;

	//	�������
	if (!lpcsCspOrFile || strlen(lpcsCspOrFile) == 0)
	{
		USBKey_LogErr("lpcsCspOrFile is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	//	�򿪸�CSP��ȱʡ����
	if (!CryptAcquireContextA(&hProv, NULL, lpcsCspOrFile, PROV_RSA_FULL, 0))
	{
		ulError = GetLastError();
		if (0 == ulError) ulRes = ulError ? ulError : USBKEY_ERR_FAILED;
		USBKey_LogErr("CryptAcquireContext() failed! ulError = 0x%x\n", ulError);
		return ulRes;
	}

	//	����CSP��
	strcpy_s(m_csCSPName, 256, lpcsCspOrFile);

	//	�ͷŵ�
	CryptReleaseContext(hProv, 0);

	FUNC_END;

	return ulRes;
}
/***********************************************************************
 *	����:Initialize
 *	˵��:��ʼ���ӿڲ���������ʹ�õ��㷨�ȡ�
 *	����:ulAsymmAlg:[IN]:�ǶԳ��㷨
 *		 ulSymmAlg:[IN]:�Գ��㷨
 *	     ulHashAlg:[IN]:Hash�㷨
 *		 lpUserPIN:[IN]:�û�PIN��
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
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
 *	����:WaitForDevEvent
 *	˵��:�ȴ��豸�¼�������CSP��֧�֡�
 *	����:����
 *	����:USBKEY_ERR_NOT_SUPPORT
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
 *	����:CancelWaitForDevEvent
 *	˵��:ȡ���ȴ��豸�¼�������CSP��֧�֡�
 *	����:����
 *	����:USBKEY_ERR_OK
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::CancelWaitForDevEvent()
{
	ZJCA_LogI(_T("This function return OK always!"));
	return USBKEY_ERR_OK;
}
/***********************************************************************
 *	����:ConnectDev
 *	˵��:�����豸��CSP��֧�֡�
 *	����:��
 *	����:USBKEY_ERR_OK
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::ConnectDev()
{
	ZJCA_LogI(_T("This function return OK always!"));
	return USBKEY_ERR_OK;
}
/***********************************************************************
 *	����:Envelop
 *	˵��:��P7�����ŷ��ʽ��װ����
 *	����:lpInData:[IN]:��������
 *		 ulInLen:[IN]:�������ݳ���
 *		 lpOutData:[OUT]:���ص������ŷ�����
 *		 pulOutLen:[IN/OUT]:�����ŷ����ݳ���
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
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
		
	// �������
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

	// �򿪼�����Կ���ڵ�����
	hProv = OpenCSPProvider(AT_KEYEXCHANGE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// ��������֤��
	pCert = ExportCertificate(hProv, FALSE);
	if (NULL == pCert)
	{
		USBKey_LogErr("No certificate!");
		ulRes = USBKEY_ERR_NO_CERT;
		goto FREE_MEMORY;
	}
	
	// ��΢��RSA CSP
	if (!CryptAcquireContext(&hMSProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptAcquireContext() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	 
	// �����ŷ������
	PCERT_INFO recipCertArray[] = { pCert->pCertInfo };

	// �㷨����
	CRYPT_ALGORITHM_IDENTIFIER ContentEncryptAlgorithm;
	memset(&ContentEncryptAlgorithm, 0, sizeof(ContentEncryptAlgorithm));
	ContentEncryptAlgorithm.pszObjId = szOID_RSA_SMIMEalgCMS3DESwrap;

	// ������Ϣ����
	CMSG_ENVELOPED_ENCODE_INFO encodeInfo;
	memset(&encodeInfo, 0, sizeof(CMSG_ENVELOPED_ENCODE_INFO));
	encodeInfo.cbSize = sizeof(CMSG_ENVELOPED_ENCODE_INFO);
	encodeInfo.hCryptProv = hMSProv;
	encodeInfo.ContentEncryptionAlgorithm = ContentEncryptAlgorithm;
	encodeInfo.pvEncryptionAuxInfo = NULL;
	encodeInfo.cRecipients = 1;
	encodeInfo.rgpRecipients = recipCertArray;

	// ���������ŷ�������
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

	// �򿪴����Ϣ���
	hMsg = CryptMsgOpenToEncode(GLOBAL_ENCODING_TYPE, 0, CMSG_ENVELOPED, &encodeInfo, NULL, NULL);
	if (hMsg == NULL)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgOpenToEncode() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// ���ԭ��
	if (!CryptMsgUpdate(hMsg, lpInData, ulInLen, TRUE))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgUpdate() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// ��ȡ�����ŷ���
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
 *	����:Develop
 *	˵��:����P7��ʽ�������ŷ�����
 *	����:lpInData:[IN]:P7��ʽ�������ŷ�����
 *		 ulInLen:[IN]:�����ŷ����ݳ���
 *		 lpOutData:[OUT]:���ܺ������
 *		 pulOutLen:[IN/OUT]:���ܺ�����ݳ���
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
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

	// �������
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

	// ��ȡ������Կ��������
	hProv = OpenCSPProvider(AT_KEYEXCHANGE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}
			
	// ��ȡ������Ϣ���
	hDecryptMsg = CryptMsgOpenToDecode(GLOBAL_ENCODING_TYPE, 0, 0, NULL, NULL, NULL);
	if (NULL == hDecryptMsg)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgOpenToDecode() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// ��������
	if (!CryptMsgUpdate(hDecryptMsg, lpInData, ulInLen, TRUE))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgUpdate() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// ��ȡ������֤����Ϣ(Ŀǰֻ֧��һ�������ߣ�����dwIndex = 0)
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
	
	// ���ܲ���
	decryptParam.cbSize = sizeof(CMSG_CTRL_DECRYPT_PARA);
	decryptParam.dwKeySpec = AT_KEYEXCHANGE;
	decryptParam.hCryptProv = hProv;
	decryptParam.dwRecipientIndex = 0;

	// ���ý��ܲ���
	if (!CryptMsgControl(hDecryptMsg, 0, CMSG_CTRL_DECRYPT, &decryptParam))
	{
		ulRes = GetLastError();	
		USBKey_LogErr("CryptMsgControl() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// ��ȡ���ܺ�Ľ����С
	if (!CryptMsgGetParam(hDecryptMsg, CMSG_CONTENT_PARAM, 0, NULL, &ulDecryptedLen) || ulDecryptedLen == 0)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgGetParam() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// ��ȡ���ܽ��
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
 *	����:Encrypt
 *	˵��:��"Wrapped Key + Cipher"��ʽ��������
 *	����:lpInData:[IN]:��������
 *		 ulInLen:[IN]:�������ݳ���
 *		 lpOutData:[OUT]:���ص������ŷ�����
 *		 pulOutLen:[IN/OUT]:�����ŷ����ݳ���
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
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

	// �������
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

	// ��ȡ������Կ��������
	hProv = OpenCSPProvider(AT_KEYEXCHANGE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}
	
	// ������ɻỰ��Կ
	if (!CryptGenKey(hProv, CALG_3DES, USBKEY_DES3_KEYLENGTH|CRYPT_EXPORTABLE, &hSymmKey))
    {
		ulRes = GetLastError();
		USBKey_LogErr("CryptGenKey() failed! ulRes = 0x%x", ulRes);
		return ulRes;
    }

	//	��ȡ��Կ�Ծ��
	if (!CryptGetUserKey(hProv, AT_KEYEXCHANGE, &hExchPubKey))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptGetUserKey() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}
	
	// �����Ự��ԿBlob
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

	// �����������ĻỰ��Կ����
	dwEnvelopedIndex = 0;
	memcpy(lpOutData, &dwWrappedBlobLen, 4);
	dwEnvelopedIndex = 4;
	memcpy(lpOutData + dwEnvelopedIndex, lpWrappedBlob, dwWrappedBlobLen);
	dwEnvelopedIndex += dwWrappedBlobLen;

	// ��������
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
 *	����:Decrypt
 *	˵��:����"Wrapped Key + Cipher"��ʽ����
 *	����:lpInData:[IN]:��������
 *		 ulInLen:[IN]:�������ݳ���
 *		 lpOutData:[OUT]:���ܺ������
 *		 pulOutLen:[IN/OUT]:���ܺ�����ݳ���
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
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

	// �������
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

	// ��ȡ������Կ��������
	hProv = OpenCSPProvider(AT_KEYEXCHANGE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// ��ȡ������Կ�Ծ�����������ܻỰ��Կ
	if (!CryptGetUserKey(hProv, AT_KEYEXCHANGE, &hExchKey))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptGetUserKey() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	//����ȡ��������Կ���ܵĻỰ��Կ����
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

	// ����Ự��Կ
	if (!CryptImportKey(hProv, lpWrapBlob, ulBlobLen, hExchKey, 0, &hSymmKey))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptImportKey() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// ���Ự��Կ����������
	ulEncryptedDataLen = ulInLen - ulIndex;
	lpCipher = lpInData + ulIndex;

	// ʹ�ûỰ��Կ��������	
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
 *	����:SignP7
 *	˵��:��P7��ʽǩ������
 *	����:lpOrgData:[IN]:ԭ������
 *		 ulOrgLen:[IN]:ԭ�����ݳ���
 *		 lpSignature:[OUT]:ǩ������
 *		 pulSignLen:[IN/OUT]:ǩ������
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
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

	// �������
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

	// ��ȡ������Կ��������
	hProv = OpenCSPProvider(AT_SIGNATURE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// ����ǩ��֤��
	pSignCert = ExportCertificate(hProv, TRUE);
	if (!pSignCert)
	{
		USBKey_LogErr("No certificate!");
		ulRes = USBKEY_ERR_NO_CERT;
		goto FREE_MEMORY;
	}

	// ���ǩ������Ϣ�ṹ��
	ZeroMemory(&sei, sizeof(CMSG_SIGNER_ENCODE_INFO));
	sei.cbSize = sizeof(CMSG_SIGNER_ENCODE_INFO);
	sei.pCertInfo = pSignCert->pCertInfo;
	sei.hCryptProv = hProv;
	sei.dwKeySpec = AT_SIGNATURE;
	sei.HashAlgorithm.pszObjId = szOID_OIWSEC_sha1;
	sei.pvHashAuxInfo = NULL;
	sei.cAuthAttr = 0;
	sei.rgAuthAttr = NULL;

	// ���ǩ������ṹ��
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
		
	// ����ǩ��������ֳ���
	ulSignLen = CryptMsgCalculateEncodedLength(GLOBAL_ENCODING_TYPE, CMSG_DETACHED_FLAG, CMSG_SIGNED, &signedEncodeInfo, szOID_RSA_signedData, ulOrgLen);
	if (0 == ulSignLen)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgCalculateEncodedLength() failed! ulRes = 0x%x", ulRes);		
		goto FREE_MEMORY;
	}	

	// ���ؽ������
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

	// ��ǩ����Ϣ���
	hSignMsg = CryptMsgOpenToEncode(GLOBAL_ENCODING_TYPE, CMSG_DETACHED_FLAG, CMSG_SIGNED, &signedEncodeInfo, NULL, NULL);
	if (NULL == hSignMsg)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgOpenToEncode() failed! ulRes = 0x%x", ulRes);	
		goto FREE_MEMORY;
	}

	// ǩ������
	if (!CryptMsgUpdate(hSignMsg, lpOrgData, ulOrgLen, TRUE))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgUpdate() failed! ulRes = 0x%x", ulRes);	
		goto FREE_MEMORY;
	}
		
	// ��ȡǩ����Ľ��
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
 *	����:VerifyP7
 *	˵��:��֤P7��ʽ��ǩ��
 *	����:lpOrgData:[IN]:ԭ������
 *		 ulOrgLen:[IN]:ԭ�����ݳ���
 *		 lpSignature:[IN]:ǩ������
 *		 ulSignLen:[IN]:ǩ������
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
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

	// �������
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

	// ��ȡ������Կ��������
	hProv = OpenCSPProvider(AT_SIGNATURE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	//	����ǩ��֤��
	pSignCert = ExportCertificate(hProv, TRUE);
	if (!pSignCert)
	{
		USBKey_LogErr("No certificate!");
		ulRes = USBKEY_ERR_NO_CERT;
		goto FREE_MEMORY;
	}
	
	// ����Ϣ���
	hVerifyMsg = CryptMsgOpenToDecode(GLOBAL_ENCODING_TYPE, CMSG_DETACHED_FLAG, NULL, NULL, NULL, NULL);
	if (NULL == hVerifyMsg)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgOpenToDecode() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// ����ǩ��ֵ			
	if (!CryptMsgUpdate(hVerifyMsg, lpSignature, ulSignLen, TRUE))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgUpdate() failed! ulRes = 0x%x\n", ulRes);
		goto FREE_MEMORY;
	}

	// ����ԭ��			
	if (!CryptMsgUpdate(hVerifyMsg, lpOrgData, ulOrgLen, TRUE))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptMsgUpdate() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// Ȩ�޿���				
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
 *	����:SignP1
 *	˵��:��ͨ��ʽǩ������
 *	����:lpOrgData:[IN]:ԭ������
 *		 ulOrgLen:[IN]:ԭ�����ݳ���
 *		 lpUserID:[IN]:ǩ��ʱ������û�����
 *		 lpSignature:[OUT]:ǩ������
 *		 pulSignLen:[IN/OUT]:ǩ������
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
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

	// �������
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

	// ��ȡ������Կ��������
	hProv = OpenCSPProvider(AT_SIGNATURE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// ����HASH����
	if (!CryptCreateHash(hProv, m_ulHashAlg, NULL, 0, &hHash))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptCreateHash() failed! ulRes = 0x%x", ulRes);
		return ulRes;
	}

	// ��ԭ��HASH
	if (!CryptHashData(hHash, lpOrgData, ulOrgLen, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptHashData() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// ��HASHǩ��
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
 *	����:VerifyP1
 *	˵��:��֤��ͨ��ʽ��ǩ��
 *	����:lpOrgData:[IN]:ԭ������
 *		 ulOrgLen:[IN]:ԭ�����ݳ���
 *		 lpUserID:[IN]:ǩ��ʱ������û�����
 *		 lpSignature:[IN]:ǩ������
 *		 ulSignLen:[IN]:ǩ������
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
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

	// �������
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

	// ��ȡ������Կ��������
	hProv = OpenCSPProvider(AT_SIGNATURE);
	if (NULL == hProv)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	//	����ǩ��֤��
	pSignCert = ExportCertificate(hProv, TRUE);
	if (!pSignCert)
	{
		USBKey_LogErr("No certificate!");
		ulRes = USBKEY_ERR_NO_CERT;
		goto FREE_MEMORY;
	}

	// ����HASH����
	if (!CryptCreateHash(hProv, CALG_SHA1, NULL, 0, &hHash))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptCreateHash() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// ��ԭ��HASH
	if (!CryptHashData(hHash, lpOrgData, ulOrgLen, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptHashData() failed! ulRes = 0x%x", ulRes);
		goto FREE_MEMORY;
	}

	// ������ǩ֤��Ĺ�Կ��Ϣ
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

	// ��ǩ
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
 *	����:DisconnectDev
 *	˵��:�Ͽ����豸�����ӣ�CSP�޶�Ӧ�Ĳ�����
 *	����:��
 *	����:USBKEY_ERR_OK
 ***********************************************************************/
ULONG CUSBKeyCSPImpl::DisconnectDev(void)
{
	ZJCA_LogI(_T("This function return OK always!"));
	return USBKEY_ERR_OK;
}
 /***********************************************************************
 *	����:Release
 *	˵��:�ͷŽӿڶ���
 *	����:��
 *	����:USBKEY_ERR_OK
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

/*	��ָ����CSP��Կ���� */					 
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

	//	�������
	if (strlen(m_csCSPName) == 0)
	{
		ZJCA_LogE(_T("m_csCSPName is NULL!"));
		return NULL;
	}
	
	//	�򿪸�CSP��ȱʡ����
	if (!CryptAcquireContextA(&hDefaultProv, NULL, m_csCSPName, PROV_RSA_FULL, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptAcquireContext() failed! ulRes = 0x%x\n", ulRes);
		return NULL;
	}

	//	ö����������
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

	//	�ͷž��
	if (hDefaultProv)
	{
		CryptReleaseContext(hDefaultProv, 0);
		hDefaultProv = NULL;
	}

	FUNC_END;

	return hFoundProv;
}

/*	����CSP��Կ�����е�֤�� */		
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

	//	��ȡ��Կ�Ծ��
	if (!CryptGetUserKey(hProv, bSignCert ? AT_SIGNATURE : AT_KEYEXCHANGE, &hKeyPair))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptGetUserKey() failed! ulRes = 0x%x\n", ulRes);
		goto FREE_MEMORY;
	}

	//	��ȡ��Ӧ֤�鳤��
	if (!CryptGetKeyParam(hKeyPair, KP_CERTIFICATE, NULL, &ulCertLen, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptGetKeyParam() failed! ulRes = 0x%x\n", ulRes);
		goto FREE_MEMORY;
	}

	//	��ȡ֤������
	lpCertData = new BYTE[ulCertLen];
	if (!CryptGetKeyParam(hKeyPair, KP_CERTIFICATE, lpCertData, &ulCertLen, 0))
	{
		ulRes = GetLastError();
		USBKey_LogErr("CryptGetKeyParam() failed! ulRes = 0x%x\n", ulRes);
		goto FREE_MEMORY;
	}

	//	����֤��
	pCertContext = CertCreateCertificateContext(GLOBAL_ENCODING_TYPE, lpCertData, ulCertLen);
	if (!pCertContext)
	{
		ulRes = GetLastError();
		USBKey_LogErr("CertCreateCertificateContext() failed! ulRes = 0x%x\n", ulRes);
		goto FREE_MEMORY;
	}
	
	//	�ж��Ƿ���RSA֤��					
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
