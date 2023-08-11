#include "stdafx.h"
#include "USBKeyP11Impl.h"
#include "./LogFile/LogFile.h"

unsigned const char trueValue = 1, falseValue = 0, Rsa_e_f4[ 3 ] = { 0x01, 0x00, 0x01 };
const unsigned ckoPubKey = CKO_PUBLIC_KEY, ckoPrivKey = CKO_PRIVATE_KEY;
const unsigned ckoCertificate = CKO_CERTIFICATE, ckoSecretKey = CKO_SECRET_KEY;
const unsigned ckcX509 = CKC_X_509, ckkRsa = CKK_RSA, ckkEc = CKK_EC;

/***********************************************************************
 *	����:CUSBKeyP11Impl
 *	˵��:���캯��
 *	����:��
 *	����:��
 ***********************************************************************/
CUSBKeyP11Impl::CUSBKeyP11Impl(void)
 : m_hInstance(NULL)
 , m_ulAsymmAlg(USBKEY_ASYMMALG_RSA)
 , m_ulSymmAlg(USBKEY_SYMMALG_3DES)
 , m_ulHashAlg(USBKEY_HASHALG_SHA1)
 , m_ulSymmValueLen(0)
 , m_hSession(NULL)
 , m_pFunctionList(NULL)
{
	memset(m_csDLLFile, 0, 256);
}
/***********************************************************************
 *	����:~CUSBKeyP11Impl
 *	˵��:��������
 *	����:��
 *	����:��
 ***********************************************************************/
CUSBKeyP11Impl::~CUSBKeyP11Impl(void)
{
}
/***********************************************************************
 *	����:Create
 *	˵��:����PKCS11�ӿ�DLL�ļ�
 *	����:lpcsCspOrFile:[IN]:DLL�ļ���
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
 ***********************************************************************/
ULONG CUSBKeyP11Impl::Create(LPCSTR lpcsCspOrFile)
{
	ULONG ulRes = 0;

	FUNC_BEGIN;

	//	�������
	if (!lpcsCspOrFile || strlen(lpcsCspOrFile) == 0)
	{
		USBKey_LogErr("lpcsDll is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (m_hInstance)
	{
		Release();
	}

	//	����	PKCS11�ӿ�SDK
	m_hInstance = LoadLibraryA(lpcsCspOrFile);	
	if (NULL == m_hInstance)
	{
		ulRes = GetLastError();
		USBKey_LogErr("LoadLibrary() failed! dwRes = 0x%x", ulRes);
		return ulRes;
	}

	//	����PKCS11����	
	CK_C_GetFunctionList pCGetFunctionList = (CK_C_GetFunctionList)::GetProcAddress(m_hInstance, "C_GetFunctionList");
	if (NULL == pCGetFunctionList)
	{
		USBKey_LogErr("Interface C_GetFunctionList() is NULL!");
		return USBKEY_ERR_NO_INTERFACE;
	}
	ulRes = pCGetFunctionList(&m_pFunctionList);
	if (CKR_OK != ulRes || !m_pFunctionList)
	{
		USBKey_LogErr("C_GetFunctionList() failed! dwRes = 0x%x", ulRes);
		return USBKEY_ERR_NO_INTERFACE;
	}
	
	//	��ʼ��
	ulRes = m_pFunctionList->C_Initialize(NULL);
	if (CKR_OK != ulRes)
	{
		USBKey_LogErr("C_Initialize() failed! dwRes = 0x%x", ulRes);
		return ulRes;
	}	

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
ULONG CUSBKeyP11Impl::Initialize(ULONG ulAsymmAlg, 
								 ULONG ulSymmAlg, 
								 ULONG ulHashAlg, 
								 LPSTR lpUserPIN)
{
	ULONG ulRes = 0;

	FUNC_BEGIN;
	
	ZJCA_LogI(_T("AsymmAlg:0x%x, SymmAlg:0x%x, HashAlg:0x%x"), ulAsymmAlg, ulSymmAlg, ulHashAlg);

	//	�������
	if (USBKEY_ASYMMALG_RSA == ulAsymmAlg)
	{
		m_ulAsymmAlg = CKK_RSA;
	}
	else
	{
		USBKey_LogErr("ulAsymmAlg is wrong! ulAsymmAlg = 0x%x", ulAsymmAlg);
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (USBKEY_SYMMALG_3DES == ulSymmAlg)
	{
		m_ulSymmAlg = CKK_DES3;
		m_ulSymmValueLen = 24;
	}
	else
	{
		USBKey_LogErr("ulSymmAlg is wrong! ulSymmAlg = 0x%x", ulSymmAlg);
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (USBKEY_HASHALG_SHA1 == ulHashAlg)
	{
		m_ulHashAlg = CKM_SHA_1;
	}
	else if (USBKEY_HASHALG_SHA256 == ulHashAlg)
	{
		m_ulHashAlg = CKM_SHA256;
	}
	else if (USBKEY_HASHALG_MD5 == ulHashAlg)
	{
		m_ulHashAlg = CKM_MD5;
	}
	else
	{
		USBKey_LogErr("ulHashAlg is wrong! ulHashAlg = 0x%x", ulHashAlg);
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
 *	˵��:�ȴ��豸�¼�����
 *	����:bNotBlock:[IN]:�Ƿ��Ƿ�����ģʽ
 *		pulSoltID:[IN]:�����¼���Slot ID
 *		lpDevName:[IN]:P11�ӿں��Ըò���
 *		pulNameLen:[IN]:P11�ӿں��Ըò���
 *		pulEventType:[IN]:P11�ӿں��Ըò���
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
 ***********************************************************************/
ULONG CUSBKeyP11Impl::WaitForDevEvent(BOOL bNotBlock, 
									  ULONG *pulSoltID, 
									  LPSTR lpDevName,
									  ULONG *pulNameLen, 
									  ULONG *pulEventType)
{
	ULONG ulRes = 0;
	
	FUNC_BEGIN;

	if (!m_pFunctionList)
	{
		USBKey_LogErr("m_pFunctionList is NULL!");
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
	if (!pulEventType)
	{
		USBKey_LogErr("pulEventType is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	memset(lpDevName, 0, *pulNameLen);
	*pulNameLen = 0;
	*pulEventType = 0;
	ulRes = m_pFunctionList->C_WaitForSlotEvent(bNotBlock ? CKF_DONT_BLOCK : 0, pulSoltID, NULL_PTR);

	FUNC_END;

	return ulRes;
}
/***********************************************************************
 *	����:CancelWaitForDevEvent
 *	˵��:ȡ���ȴ��豸�¼�������P11�ӿں��Ըò�����
 *	����:��
 *	����:USBKEY_ERR_OK
 ***********************************************************************/
ULONG CUSBKeyP11Impl::CancelWaitForDevEvent()
{
	ZJCA_LogI(_T("This function return OK always!"));
	return USBKEY_ERR_OK;
}
/***********************************************************************
 *	����:ConnectDev
 *	˵��:���ӵ�һ���豸���򿪻Ự��У���û�PIN��
 *	����:��
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
 ***********************************************************************/
ULONG CUSBKeyP11Impl::ConnectDev()
{
	CK_RV rv;
	CK_ULONG ulSoltCnt = 0;
	CK_SLOT_ID_PTR pulSoltIDs = NULL;

	FUNC_BEGIN;

	if (!m_pFunctionList)
	{		
		USBKey_LogErr("m_pFunctionList is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}	

	DisconnectDev();
	
	rv = m_pFunctionList->C_GetSlotList(TRUE, NULL, &ulSoltCnt);	
	if (ulSoltCnt > 0)
	{
		pulSoltIDs = new CK_SLOT_ID[ulSoltCnt];
		rv = m_pFunctionList->C_GetSlotList(TRUE, pulSoltIDs, &ulSoltCnt);
	}
	else
	{
		USBKey_LogErr("No any key is found!");
		return USBKEY_ERR_NO_DEVICE;
	}

	rv = m_pFunctionList->C_OpenSession(pulSoltIDs[0], CKF_SERIAL_SESSION|CKF_RW_SESSION, NULL, NULL, &m_hSession);
	if (CKR_OK != rv || !m_hSession)
	{
		USBKey_LogErr("C_OpenSession() failed! rv=0x%x", rv);
		goto FREE_MEMORY;
	}
	
	rv = m_pFunctionList->C_Login(m_hSession, CKU_USER, (CK_UTF8CHAR_PTR)m_csUserPIN, strlen(m_csUserPIN));
	if (CKR_OK != rv && CKR_USER_ALREADY_LOGGED_IN != rv)
	{
		USBKey_LogErr("C_Login() failed! rv=0x%x", rv);
		goto FREE_MEMORY;
	}
	else
	{
		rv = CKR_OK;
	}

	FUNC_END;

FREE_MEMORY:
	if (pulSoltIDs)
	{
		delete []pulSoltIDs;
		pulSoltIDs = NULL;
	}

	return rv;
}
/***********************************************************************
 *	����:Envelop
 *	˵��:��P7�����ŷ��ʽ��װ���ݣ�P11�ӿڲ�֧�֡�
 *	����:����
 *	����:USBKEY_ERR_NOT_SUPPORT
 ***********************************************************************/
ULONG CUSBKeyP11Impl::Envelop(LPBYTE lpInData, 
							  ULONG ulInLen, 
							  LPBYTE lpOutData, 
							  ULONG* pulOutLen)
{
	USBKey_LogErr("This interface don't support envelop data to P7!");
	return USBKEY_ERR_NOT_SUPPORT;
}
/***********************************************************************
 *	����:Envelop
 *	˵��:����P7��ʽ�������ݣ�P11�ӿڲ�֧�֡�
 *	����:����
 *	����:USBKEY_ERR_NOT_SUPPORT
 ***********************************************************************/
ULONG CUSBKeyP11Impl::Develop(LPBYTE lpInData, 
							  ULONG ulInLen, 
							  LPBYTE lpOutData, 
							  ULONG* pulOutLen)
{
	USBKey_LogErr("This interface don't support Develop data from P7!");
	return USBKEY_ERR_NOT_SUPPORT;
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
ULONG CUSBKeyP11Impl::Encrypt(LPBYTE lpInData, 
							  ULONG ulInLen, 
							  LPBYTE lpOutData, 
							  ULONG* pulOutLen)
{
	CK_RV rv = CKR_OK;
	CK_OBJECT_HANDLE hSymmKey = 0;
	CK_OBJECT_HANDLE hExchPubKey = 0;
	CK_MECHANISM symmGenMech = { 0, 0, 0 };
	CK_MECHANISM symmEncMech = { 0, 0, 0 };
	CK_MECHANISM wrapMech = { 0, 0, 0 };
	BYTE btWrappedData[512] = {0};
	ULONG ulWrappedLen = 512;
	ULONG ulPlainIndex = 0;
	ULONG ulIndex = 0;
	ULONG ulCipherLen = USBKEY_CRYPTLEN_PERTIME + 256;
	BYTE btCipherData[USBKEY_CRYPTLEN_PERTIME + 256] = {0};
	
	FUNC_BEGIN;

	// ���úϷ��Լ��
	if (!m_pFunctionList || !m_hSession)
	{
		USBKey_LogErr("m_pFunctionList or m_hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}

	// ������� 
	if (!lpInData || ulInLen == 0 )
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

	// ���ҹ�Կ
	hExchPubKey = _FindPubKeyObj(m_hSession, TRUE, CKA_KEY_TYPE, (LPBYTE)&m_ulAsymmAlg, 4);
	if (NULL == hExchPubKey)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}
	
	// �Ự��Կ�㷨
	if (CKK_DES3 == m_ulSymmAlg)
	{
		symmGenMech.mechanism = CKM_DES3_KEY_GEN;
		symmEncMech.mechanism = CKM_DES3_ECB;
	}
	else
	{		
		USBKey_LogErr("Symm alg ID is wrong!");
		return USBKEY_ERR_NOT_SUPPORT;
	}

	// �����Ự��Կģ��
	CK_ATTRIBUTE symmAttrs[ 9 ] = 
	{
		{ CKA_CLASS, (unsigned *)&ckoSecretKey, sizeof(unsigned) },
		{ CKA_KEY_TYPE, &m_ulSymmAlg, 4 },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_ENCRYPT, (unsigned char *)&trueValue, 1 },
		{ CKA_DECRYPT, (unsigned char *)&trueValue, 1 },
		{ CKA_UNWRAP, (unsigned char *)&trueValue, 1 },
		{ CKA_WRAP, (unsigned char *)&trueValue, 1 },
		{ CKA_EXTRACTABLE, (unsigned char *)&trueValue, 1 },
		{ CKA_VALUE_LEN, &m_ulSymmValueLen, 4 } 
	};

	//�������Ự��Կ
	rv = m_pFunctionList->C_GenerateKey(m_hSession, &symmGenMech, symmAttrs, 9, &hSymmKey); 
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_GenerateKey() failed! rv = 0x%x", rv);
		goto FREE_OBJS;
	}
	
	// ʹ�ü��ܹ�ԿWrap�Ự��Կ
	wrapMech.mechanism = CKM_RSA_PKCS;
	rv = m_pFunctionList->C_WrapKey(m_hSession, &wrapMech, hExchPubKey, hSymmKey, btWrappedData, &ulWrappedLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_WrapKey() failed! rv = 0x%x", rv);
		goto FREE_OBJS;
	}
	
	// ����ͷ
	memcpy(lpOutData, &ulWrappedLen, 4);
	ulIndex = 4;
	memcpy(lpOutData + ulIndex, btWrappedData, ulWrappedLen);
	ulIndex += ulWrappedLen;

	// ���ܳ�ʼ��
	rv = m_pFunctionList->C_EncryptInit(m_hSession, &symmEncMech, hSymmKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_EncryptInit() failed! rv = 0x%x", rv);
		goto FREE_OBJS;
	}
	
	// �������
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
		ulCipherLen = USBKEY_CRYPTLEN_PERTIME + 256;
		rv = m_pFunctionList->C_EncryptUpdate(m_hSession, bUsePaddedBlob ? btPaddedBlob : lpInData+ulPlainIndex, ulThisTimePlainLen, btCipherData, &ulCipherLen);
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_EncryptUpdate() failed! ulRes=0x%x", rv);
			goto FREE_OBJS;
		}
		ulPlainIndex += ulThisTimePlainLen;
		//
		if (ulCipherLen > 0)
		{
			memcpy_s(lpOutData + ulIndex, *pulOutLen, btCipherData, ulCipherLen);
			ulIndex += ulCipherLen;
		}
	}while(ulPlainIndex < ulInLen);

	//	���ܽ���(���봫��Ч�Ľ������ݵ�Bufferָ��ͳ���)
	ulCipherLen = USBKEY_CRYPTLEN_PERTIME + 256;
	m_pFunctionList->C_EncryptFinal(m_hSession, btCipherData, &ulCipherLen);
	if (ulCipherLen > 0)
	{
		memcpy(lpOutData + ulIndex, btCipherData, ulCipherLen);
		ulIndex += ulCipherLen;
	}
	*pulOutLen = ulIndex;

	FUNC_END;

FREE_OBJS:
	if (hSymmKey)
	{
		m_pFunctionList->C_DestroyObject(m_hSession, hSymmKey);
		hSymmKey = NULL;
	}

	return rv;
}
/***********************************************************************
 *	����:Decrypt
 *	˵��:����"Wrapped Key + Cipher"��ʽ����
 *	����:lpInData:[IN]:��������
 *		 ulInLen:[IN]:�������ݳ���
 *		 lpOutData:[OUT]:���ܺ������
 *		 pulOutLen:[IN/OUT]:���ܺ�����ݳ���
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
 **********************************************************************/
ULONG CUSBKeyP11Impl::Decrypt(LPBYTE lpInData, 
							  ULONG ulInLen, 
							  LPBYTE lpOutData, 
							  ULONG* pulOutLen)
{
	CK_RV rv = CKR_OK;
	CK_OBJECT_HANDLE hSymmKey = 0;
	CK_OBJECT_HANDLE hExchPriKey = 0;
	CK_MECHANISM symmGenMech = { CKM_DES3_KEY_GEN, 0, 0 };
	CK_MECHANISM symmDecMech = { CKM_DES3_ECB, 0, 0 };
	CK_MECHANISM unwrapMech = { 0, 0, 0 };
	BYTE btWrappedData[512] = {0};
	ULONG ulWrappedLen = 512;
	ULONG ulCipherIndex = 0;
	ULONG ulIndex = 0;
	ULONG ulDecryptedLen = USBKEY_CRYPTLEN_PERTIME + 256;
	BYTE btDecryptedData[USBKEY_CRYPTLEN_PERTIME + 256] = {0};
	
	FUNC_BEGIN;

	// ���úϷ��Լ��
	if (!m_pFunctionList || !m_hSession)
	{
		USBKey_LogErr("m_pFunctionList or m_hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}

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
	if (*pulOutLen < ulInLen)
	{
		USBKey_LogErr("*pulOutLen is too small! Reqeusted length:0x%x", ulInLen + 260);
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	// ���ҽ���˽Կ
	hExchPriKey = _FindPriKeyObj(m_hSession, FALSE, CKA_KEY_TYPE, (LPBYTE)&m_ulAsymmAlg, 4);
	if (NULL == hExchPriKey)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// ����Wrapped����
	ulCipherIndex = 0;
	memcpy(&ulWrappedLen, lpInData + ulCipherIndex, 4);
	ulCipherIndex += 4;
	memcpy(btWrappedData, lpInData + ulCipherIndex, ulWrappedLen);
	ulCipherIndex += ulWrappedLen;

	// ����Unwrap�Ự��Կģ��
	CK_ATTRIBUTE unwrapAttrs[ 8 ] = 
	{
		{ CKA_CLASS, (unsigned *)&ckoSecretKey, sizeof(unsigned) },
		{ CKA_KEY_TYPE, &m_ulSymmAlg, sizeof(int) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_DECRYPT, (unsigned char *)&trueValue, 1 },
		{ CKA_UNWRAP, (unsigned char *)&trueValue, 1 },
	};
	
	//�����ܻỰ��Կ
	unwrapMech.mechanism = CKM_RSA_PKCS;
	rv = m_pFunctionList->C_UnwrapKey(m_hSession, &unwrapMech, hExchPriKey, btWrappedData, ulWrappedLen, unwrapAttrs, 5, &hSymmKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_UnwrapKey() failed! rv = 0x%x", rv);
		goto FREE_OBJS;
	}
		
	// ���ܳ�ʼ��
	rv = m_pFunctionList->C_DecryptInit(m_hSession, &symmDecMech, hSymmKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_DecryptInit() failed! rv = 0x%x", rv);
		goto FREE_OBJS;
	}

	//	�������
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
		ulDecryptedLen = USBKEY_CRYPTLEN_PERTIME + 256;
		rv = m_pFunctionList->C_DecryptUpdate(m_hSession, lpInData+ulCipherIndex, ulThisTimeCipherLen, btDecryptedData, &ulDecryptedLen);
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_DecryptUpdate() failed! rv = 0x%x", rv);
			goto FREE_OBJS;
		}
		ulCipherIndex += ulThisTimeCipherLen;
		//
		if (ulDecryptedLen > 0)
		{
			memcpy(lpOutData + ulIndex, btDecryptedData, ulDecryptedLen);
			ulIndex += ulDecryptedLen;
		}
	}while(ulCipherIndex < ulInLen);

	//	���ܽ���(���봫��Ч�Ľ������ݵ�Bufferָ��ͳ���)
	ulDecryptedLen = USBKEY_CRYPTLEN_PERTIME + 256;
	m_pFunctionList->C_DecryptFinal(m_hSession, btDecryptedData, &ulDecryptedLen);
	if (ulDecryptedLen > 0)
	{
		memcpy(lpOutData + ulIndex, btDecryptedData, ulDecryptedLen);
		ulIndex += ulDecryptedLen;
	}
	*pulOutLen = ulIndex;
	
	FUNC_END;

FREE_OBJS:
	if (hSymmKey)
	{
		m_pFunctionList->C_DestroyObject(m_hSession, hSymmKey);
		hSymmKey = NULL;
	}

	return rv;
}
/***********************************************************************
 *	����:SignP7
 *	˵��:��P7��ʽǩ�����ݣ�P11�ӿڲ�֧�֡�
 *	����:����
 *	����:USBKEY_ERR_NOT_SUPPORT
 ***********************************************************************/
ULONG CUSBKeyP11Impl::SignP7(LPBYTE lpOrgData, 
							 ULONG ulOrgLen, 
							 LPBYTE lpSignature, 
							 ULONG* pulSignLen)
{
	USBKey_LogErr("This interface don't support sign data to P7!");
	return USBKEY_ERR_NOT_SUPPORT;
}
/***********************************************************************
 *	����:VerifyP7
 *	˵��:��֤P7��ʽ��ǩ����P11�ӿڲ�֧�֡�
 *	����:����
 *	����:USBKEY_ERR_NOT_SUPPORT
 ***********************************************************************/
ULONG CUSBKeyP11Impl::VerifyP7(LPBYTE lpOrgData, 
							   ULONG ulOrgLen, 
							   LPBYTE lpSignature, 
							   ULONG ulSignLen)
{
	USBKey_LogErr("This interface don't support verify P7 signature!");
	return USBKEY_ERR_NOT_SUPPORT;
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
ULONG CUSBKeyP11Impl::SignP1(LPBYTE lpOrgData, 
							 ULONG ulOrgLen, 
							 LPSTR lpUserID, 
							 LPBYTE lpSignature, 
							 ULONG* pulSignLen)
{
	CK_RV rv = CKR_OK;
	CK_BYTE btHashValue[132] = {0};
	CK_ULONG ulHashLen = sizeof(btHashValue) - 4;
	CK_MECHANISM signMech = {0, 0, 0};
	CK_OBJECT_HANDLE hSignPriKey = 0;
	
	FUNC_BEGIN;

	// ���úϷ��Լ��
	if (!m_pFunctionList || !m_hSession)
	{
		USBKey_LogErr("m_pFunctionList or m_hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	
	//	�������
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

	// ����ǩ��˽Կ
	hSignPriKey = _FindPriKeyObj(m_hSession, TRUE, CKA_KEY_TYPE, (LPBYTE)&m_ulAsymmAlg, 4);
	if (!hSignPriKey)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// �ȶ�ԭʼ������HASH����
	rv = _GetHashValue(m_hSession, m_ulHashAlg, lpOrgData, ulOrgLen, btHashValue, &ulHashLen);
	if (CKR_OK != rv)
	{
		return rv;
	}
	
	// ǩ����ʼ��
	signMech.mechanism = CKM_RSA_PKCS;
	rv = m_pFunctionList->C_SignInit(m_hSession, &signMech, hSignPriKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_SignInit() failed! rv = 0x%x", rv);
		return rv;
	}
	
	// ʹ��ǩ��˽Կ��HASHֵǩ��
	rv = m_pFunctionList->C_Sign(m_hSession, btHashValue, ulHashLen, lpSignature, pulSignLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Sign() failed! rv = 0x%x", rv);
		return rv;
	}

	FUNC_END;
	
	return rv;
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
ULONG CUSBKeyP11Impl::VerifyP1(LPBYTE lpOrgData, 
							   ULONG ulOrgLen, 
							   LPSTR lpUserID, 
							   LPBYTE lpSignature, 
							   ULONG ulSignLen)
{
	CK_RV rv = CKR_OK;
	CK_BYTE btHashValue[132] = {0};
	CK_ULONG ulHashLen = sizeof(btHashValue) - 4;
	CK_MECHANISM signMech = {0, 0, 0};
	CK_OBJECT_HANDLE hVerifyPubKey = 0;
	
	FUNC_BEGIN;

	// ���úϷ��Լ��
	if (!m_pFunctionList || !m_hSession)
	{
		USBKey_LogErr("m_pFunctionList or m_hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	
	//	�������
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

	// ��ȡ��Կ
	hVerifyPubKey = _FindPubKeyObj(m_hSession, FALSE, CKA_KEY_TYPE, (LPBYTE)&m_ulAsymmAlg, 4);
	if (NULL == hVerifyPubKey)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// �ȶ�ԭʼ������HASH����
	rv = _GetHashValue(m_hSession, m_ulHashAlg, lpOrgData, ulOrgLen, btHashValue, &ulHashLen);
	if (CKR_OK != rv)
	{
		return rv;
	}

	// ��ʼ��
	signMech.mechanism = CKM_RSA_PKCS;
	rv = m_pFunctionList->C_VerifyInit(m_hSession, &signMech, hVerifyPubKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_VerifyInit() failed! rv = 0x%x", rv);
		return rv;
	}
	
	// ʹ��ǩ��˽Կ��HASHֵǩ��	
	rv = m_pFunctionList->C_Verify(m_hSession, btHashValue, ulHashLen, lpSignature, ulSignLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_VerifyInit() failed! rv = 0x%x", rv);
		return rv;
	}
	
	FUNC_END;

	return rv;
}
 /***********************************************************************
 *	����:DisconnectDev
 *	˵��:�Ͽ����豸�����ӣ��رջỰ��
 *	����:��
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
 ***********************************************************************/
ULONG CUSBKeyP11Impl::DisconnectDev(void)
{
	ULONG ulRes = 0;
	
	FUNC_BEGIN;

	if (m_hSession)
	{
		m_pFunctionList->C_CloseSession(m_hSession);
		m_hSession = NULL;
	}

	FUNC_END;

	return ulRes;
}
 /***********************************************************************
 *	����:Release
 *	˵��:�ͷŽӿ������ص�DLL�ļ�
 *	����:��
 *	����:USBKEY_ERR_OK
 ***********************************************************************/
ULONG CUSBKeyP11Impl::Release(void)
{
	ULONG ulRes = USBKEY_ERR_OK;
	
	FUNC_BEGIN;

	if (m_pFunctionList)
	{
		m_pFunctionList->C_Finalize(NULL);
	}
	if (m_hInstance)
	{
		FreeLibrary(m_hInstance);
		m_hInstance = NULL;
	}

	FUNC_END;

	return ulRes;
}

//	���ҹ�Կ����
CK_OBJECT_HANDLE CUSBKeyP11Impl::_FindPubKeyObj(CK_SESSION_HANDLE hSession, 
												BOOL bIsExchKey, 
												ULONG ckaType, 
												LPBYTE data, 
												ULONG size)
{
	CK_RV rv = CKR_OK;
	CK_ULONG findCount = 0;
	CK_OBJECT_HANDLE hKey = 0;
	CK_ATTRIBUTE encryptKey_Attrs[ 5 ] = {
		{ CKA_CLASS, (unsigned *)&ckoPubKey, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 },
		{ CKA_ENCRYPT/*CKA_WRAP*/, (unsigned char *)&trueValue, 1 },
		{ ckaType, data, size },
	};
	CK_ATTRIBUTE verifyKey_Attrs[ 5 ] = {
		{ CKA_CLASS, (unsigned *)&ckoPubKey, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 },
		{ CKA_VERIFY, (unsigned char *)&trueValue, 1 },
		{ ckaType, data, size },
	};

	FUNC_BEGIN;
	rv = m_pFunctionList->C_FindObjectsInit( hSession, bIsExchKey ? encryptKey_Attrs : verifyKey_Attrs, data ? 5 : 4);
	rv = m_pFunctionList->C_FindObjects( hSession, &hKey, 1, &findCount);
	rv = m_pFunctionList->C_FindObjectsFinal( hSession );
	FUNC_END;

	return hKey;
}

//	����˽Կ����
CK_OBJECT_HANDLE CUSBKeyP11Impl::_FindPriKeyObj(CK_SESSION_HANDLE hSession, 
												BOOL bIsSignKey, 
												ULONG ckaType, 
												LPBYTE data, 
												ULONG size)
{
	CK_RV rv = CKR_OK;
	CK_ULONG findCount = 0;
	CK_OBJECT_HANDLE hKey = 0;
	CK_ATTRIBUTE decryptKey_Attrs[ 5 ] = {
		{ CKA_CLASS, (unsigned *)&ckoPrivKey, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&trueValue, 1 },
		{ CKA_DECRYPT, (unsigned char *)&trueValue, 1 },
		{ ckaType, data, size },
	};
	CK_ATTRIBUTE signKey_Attrs[ 5 ] = {
		{ CKA_CLASS, (unsigned *)&ckoPrivKey, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&trueValue, 1 },
		{ CKA_SIGN, (unsigned char *)&trueValue, 1 },
		{ ckaType, data, size },
	};
	FUNC_BEGIN;
	rv = m_pFunctionList->C_FindObjectsInit( hSession, bIsSignKey ? signKey_Attrs : decryptKey_Attrs, data ? 5 : 3);
	rv = m_pFunctionList->C_FindObjects( hSession, &hKey, 1, &findCount);
	rv = m_pFunctionList->C_FindObjectsFinal( hSession );
	FUNC_END;

	return hKey;
}

//	����������ϣ����
ULONG CUSBKeyP11Impl::_GetHashValue(CK_SESSION_HANDLE hSession, 
									ULONG ulCkm, 
									LPBYTE lpOrgData, 
									ULONG ulOrgLen, 
									LPBYTE lpHashValue, 
									ULONG* pHashLen)
{
	CK_RV rv = CKR_OK;
	CK_MECHANISM hashMech = {ulCkm, 0, 0};
	CK_ULONG ulIndex = 0;

	FUNC_BEGIN;

	if (!m_pFunctionList || !m_hSession)
	{
		USBKey_LogErr("m_pFunctionList or m_hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	if (!lpOrgData || ulOrgLen == 0)
	{
		USBKey_LogErr("lpOrgData is NULL or ulOrgLen is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpHashValue || !pHashLen)
	{
		USBKey_LogErr("lpHashValue or pHashLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (*pHashLen < 32)
	{
		USBKey_LogErr("Buffer is to small! Request length:0x%x", *pHashLen);
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	// ��ʼ��
	rv = m_pFunctionList->C_DigestInit(m_hSession, &hashMech);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_DigestInit() is failed! rv = 0x%x", rv);
		return rv;
	}

	// ���Hash
	ulIndex = 0;
	do
	{
		ULONG ulLenThisTime = 0;
		if (ulOrgLen - ulIndex > USBKEY_CRYPTLEN_PERTIME)
		{
			ulLenThisTime = USBKEY_CRYPTLEN_PERTIME;
		}
		else
		{
			ulLenThisTime = ulOrgLen - ulIndex;
		}
		rv = m_pFunctionList->C_DigestUpdate(hSession, lpOrgData + ulIndex, ulLenThisTime);
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_DigestUpdate() is failed! rv = 0x%x", rv);
			return rv;
		}
		ulIndex += ulLenThisTime;
	}while(ulIndex < ulOrgLen);

	// ��ȡHash���
	m_pFunctionList->C_DigestFinal(hSession, lpHashValue, pHashLen);

	FUNC_END;

	return rv;
}