/***************************************************************************
 *	File name:SynoTFKeyPkcs11.c
 *	Introduce:The implement file for PKCS11 API functions
 *	Author:Syno common
 *	Date:2015/10/28
 *  Version:v1.0
 **************************************************************************/
#include "SynoTFKeyPkcs11.h"
#include "cryptoki.h"
#include "init.h"
#include "slots.h"


/**************************************************************************
 *	����:C_Initialize
 *	����:PKCS11�ӿڳ�ʼ��
 *	����:pInitArgs:[IN]:��ʼ������
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_Initialize(CK_VOID_PTR pInitArgs)
{
	CK_RV rv = CKR_OK;

	rv = CI_Initialize(pInitArgs);
	if (CKR_OK != rv)
	{
		return rv;
	}

	return CKR_OK;
}
/**************************************************************************
 *	����:C_Finalize
 *	����:PKCS11�ӿڳ�ʼ��
 *	����:pReserved:[IN]:��������
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_Finalize(CK_VOID_PTR pReserved)
{
	if (g_pSlotList)
	{
		free(g_pSlotList);
		g_pSlotList = NULL;
	}

	return CKR_OK;
}
/**************************************************************************
 *	����:C_GetInfo
 *	����:���ظ�PKCS11��Ļ�����Ϣ
 *	����:pInfo:[OUT]:����Ϣ����
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_GetInfo(CK_INFO_PTR pInfo)
{
	// �������
	if (!pInfo) 
	{
		return CKR_ARGUMENTS_BAD;
	}

	// �汾
	pInfo->cryptokiVersion.major = 2;
	pInfo->cryptokiVersion.minor = 0;

	// ����
	strcpy_s(pInfo->manufacturerID, 32, "Syno Chip"); 

	// ���
	pInfo->flags = 0;

	// ������
	memset(pInfo->libraryDescription,' ', 32);

	// ��汾
	pInfo->libraryVersion.major = 2;
	pInfo->libraryVersion.minor = 0;

	return CKR_OK;
}
/**************************************************************************
 *	����:C_GetFunctionList
 *	����:���ظ�PKCS11��Ľӿں�����ַ�б�
 *	����:ppFunctionList:[OUT]:������ַ�б�
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_GetFunctionList(CK_FUNCTION_LIST_PTR_PTR ppFunctionList)
{
	// �������
	if (!ppFunctionList) 
	{
		return CKR_ARGUMENTS_BAD;
	}

	// �汾��Ϣ
	(*ppFunctionList)->version.major = 2;
	(*ppFunctionList)->version.minor = 0;

	return CKR_OK;
}
/**************************************************************************
 *	����:C_GetSlotList
 *	����:���ز��б�
 *	����:tokenPresent:[IN]:�Ƿ�ֻ������Token�Ĳ�
 *		 pSlotList:[OUT]:���صĲ�ID�б�
 *		 pulCount:[OUT]:���صĲ۵ĸ���
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_GetSlotList(CK_BBOOL tokenPresent, 
						   CK_SLOT_ID_PTR pSlotList, 
						   CK_ULONG_PTR pulCount)
{
	CK_RV rv = CKR_OK;
	ULONG ulSize = 0;
	ULONG ulIndex = 0;
	LPSTR lpszNameList = NULL;
	LPSTR lpszName = NULL;
	CK_SLOT_ID_PTR lpSlotIDs = NULL;

	// �������
	if (!pulCount)
	{
		return CKR_ARGUMENTS_BAD;
	}

	// ö���豸�������豸������
	rv = SKF_EnumDev(tokenPresent, NULL, &ulSize);
	if (SAR_OK != rv || ulSize <= 2)
	{
		*pulCount = 0;
		return CKR_OK;
	}

	lpszNameList = (LPSTR)malloc(ulSize + 1);

	// ö���豸�������豸������
	rv = SKF_EnumDev(tokenPresent, lpszNameList, &ulSize);
	if (SAR_OK != rv)
	{
		*pulCount = 0;
		rv = CKR_FUNCTION_FAILED;
		goto END;
	}

	// ����Slot����
	g_ulSlotCount = 0;
	lpszName = lpszNameList;
	while (!lpszName)
	{
		g_ulSlotCount++;
		lpszName += strlen(lpszName) + 1;
	}

	// �����ڴ�
	if (g_pSlotList)
	{
		free(g_pSlotList);
		g_pSlotList = NULL;
	}
	g_pSlotList = (LPSLOTINFO)malloc(g_ulSlotCount*sizeof(SlotInfo));

	// ����ÿ��Slot��ID���豸��
	g_ulSlotCount = 0;
	lpszName = lpszNameList;
	while (!lpszName)
	{
		g_pSlotList[g_ulSlotCount].ulSlotID = g_ulSlotCount + 1;
		strcpy_s(g_pSlotList[g_ulSlotCount].csDevName, 64, lpszName);
		lpszName += strlen(lpszName) + 1;
		g_ulSlotCount++;
	}

	// �����豸����
	if (!pSlotList)
	{
		*pulCount = g_ulSlotCount;
		rv = CKR_OK;
		goto END;
	}

	// ���������豸���
	*pulCount = g_ulSlotCount;
	for (ulIndex = 0; ulIndex < g_ulSlotCount; ulIndex++)
	{
		pSlotList[ulIndex] = g_pSlotList[ulIndex].ulSlotID;
	}

END:
	if (lpszNameList)
	{
		free(lpszNameList);
		lpszNameList = NULL;
	}
	return rv;
}
/**************************************************************************
 *	����:C_GetSlotInfo
 *	����:���ز���Ϣ
 *	����:slotID:[IN]:��ID
 *		 pInfo:[OUT]:���صĲ���Ϣ����
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_GetSlotInfo(CK_SLOT_ID slotID, 
						   CK_SLOT_INFO_PTR pInfo)
{
	CK_RV rv = CKR_OK;

	// �������
	if (slotID <= 0 || slotID > g_ulSlotCount)
	{
		return CKR_ARGUMENTS_BAD;
	}

	// ��ȡSlot��Ϣ
	rv = SLT_GetSlotInfo(slotID, pInfo);
	if (CKR_OK != rv)
	{
		return rv;
	}

	return CKR_OK;
}
/**************************************************************************
 *	����:C_GetTokenInfo
 *	����:����������Ϣ
 *	����:slotID:[IN]:��ID
 *		 pInfo:[OUT]:���ص�������Ϣ����
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_GetTokenInfo(CK_SLOT_ID slotID, 
							CK_TOKEN_INFO_PTR pInfo)
{
	CK_RV rv = CKR_OK;

	// �������
	if (slotID <= 0 || slotID > g_ulSlotCount)
	{
		return CKR_ARGUMENTS_BAD;
	}

	// ��ȡToken��Ϣ
	rv = SLT_GetTokenInfo(slotID, pInfo);
	if (CKR_OK != rv)
	{
		return rv;
	}

	return CKR_OK;
}
/**************************************************************************
 *	����:C_GetMechanismList
 *	����:����������֧�ֵ��㷨��Ϣ
 *	����:slotID:[IN]:��ID
 *		 pMechanismList:[OUT]:���ص��㷨��Ϣ
 *		 pulCount:[OUT]:�㷨����
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_GetMechanismList(CK_SLOT_ID slotID, 
								CK_MECHANISM_TYPE_PTR pMechanismList, 
								CK_ULONG_PTR pulCount)
{
	CK_RV rv = CKR_OK;

	rv = SLT_GetMechanismList(slotID, pMechanismList, pulCount);
	if (CKR_OK != rv)
	{
		return rv;
	}

	return CKR_OK;
}
/**************************************************************************
 *	����:C_GetMechanismInfo
 *	����:����һ����֧�ֵ��㷨��Ϣ
 *	����:slotID:[IN]:��ID
 *		 type:[IN]:�㷨����
 *		 pInfo:[OUT]:�㷨��Ϣ
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_GetMechanismInfo(CK_SLOT_ID slotID, 
								CK_MECHANISM_TYPE type, 
								CK_MECHANISM_INFO_PTR pInfo)
{
	CK_RV rv = CKR_OK;

	rv = SLT_GetMechanismInfo(slotID, type, pInfo);
	if (CKR_OK != rv)
	{
		return rv;
	}

	return CKR_OK;
}
/**************************************************************************
 *	����:C_InitToken
 *	����:��ʼ������
 *	����:slotID:[IN]:��ID
 *		 pPin:[IN]:��ʼSO PIN
 *		 ulPinLen:[IN]:SO PIN����
 *		 pLabel:[IN]:��ǩ
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_InitToken(CK_SLOT_ID slotID, 
						 CK_UTF8CHAR_PTR pPin, 
						 CK_ULONG ulPinLen, 
						 CK_UTF8CHAR_PTR pLabel)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	����:C_InitPIN
 *	����:���ó�ʼ�û�PIN
 *	����:hSession:[IN]:�Ự���
 *		 pPin:[IN]:��ʼUser PIN
 *		 ulPinLen:[IN]:User PIN����
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_InitPIN(CK_SESSION_HANDLE hSession, 
					   CK_UTF8CHAR_PTR pPin, 
					   CK_ULONG ulPinLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	����:C_SetPIN
 *	����:�޸ĵ�ǰ��¼���͵�PIN��
 *	����:hSession:[IN]:�Ự���
 *		 pOldPin:[IN]:��PIN��
 *		 ulOldLen:[IN]:��PIN�볤��
 *		 pNewPin:[IN]:��PIN��
 *		 ulNewLen:[IN]:��PIN�볤��
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_SetPIN(CK_SESSION_HANDLE hSession, 
					  CK_UTF8CHAR_PTR pOldPin, 
					  CK_ULONG ulOldLen, 
					  CK_UTF8CHAR_PTR pNewPin, 
					  CK_ULONG ulNewLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	����:C_OpenSession
 *	����:�򿪻Ự���
 *	����:slotID:[IN]:��ID
 *		 flags:[IN]:CK_SESSION_INFO�еı��
 *		 pApplication:[IN]:�ص������еĲ���
 *		 Notify:[IN]:�ص�����
 *		 phSession:[OUT]:���ش򿪵ĻỰ���
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_OpenSession(CK_SLOT_ID slotID, 
						   CK_FLAGS flags, 
						   CK_VOID_PTR pApplication, 
						   CK_NOTIFY Notify, 
						   CK_SESSION_HANDLE_PTR phSession)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	����:C_CloseSession
 *	����:�ر�һ���Ự���
 *	����:hSession:[IN]:Ҫ�رյĻỰ���
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_CloseSession(CK_SESSION_HANDLE hSession)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	����:C_CloseAllSessions
 *	����:�ر����д򿪵ĻỰ
 *	����:slotID:[IN]:��ID
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_CloseAllSessions(CK_SLOT_ID slotID)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	����:C_GetSessionInfo
 *	����:��ȡ�Ự��Ϣ
 *	����:hSession:[IN]:�Ự���
 *		 pInfo:[OUT]:���ػỰ��Ϣ
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_GetSessionInfo(CK_SESSION_HANDLE hSession, 
							  CK_SESSION_INFO_PTR pInfo)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	����:C_GetOperationState
 *	����:��ȡ����״̬
 *	����:hSession:[IN]:�Ự���
 *		 pOperationState:[OUT]:���ز���״̬����
 *		 pulOperationStateLen:[OUT]:���ز���״̬���ݳ���
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_GetOperationState(CK_SESSION_HANDLE hSession, 
								 CK_BYTE_PTR pOperationState, 
								 CK_ULONG_PTR  pulOperationStateLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	����:C_SetOperationState
 *	����:���ò���״̬
 *	����:hSession:[IN]:�Ự���
 *		 pOperationState:[IN]:����״̬
 *		 ulOperationStateLen:[IN]:����״̬����
 *		 hEncryptionKey:[IN]:��/����Key���
 *		 hAuthenticationKey:[IN]:ǩ��/��ǩKey���
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_SetOperationState(CK_SESSION_HANDLE hSession, 
								 CK_BYTE_PTR pOperationState, 
								 CK_ULONG ulOperationStateLen, 
								 CK_OBJECT_HANDLE hEncryptionKey, 
								 CK_OBJECT_HANDLE hAuthenticationKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	����:C_Login
 *	����:��¼�Ự
 *	����:hSession:[IN]:�Ự���
 *		 userType:[IN]:�û����ͣ�SO or User
 *		 pPin:[IN]:PIN��
 *		 ulPinLen:[IN]:PIN�볤��
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_Login(CK_SESSION_HANDLE hSession, 
					 CK_USER_TYPE userType, 
					 CK_UTF8CHAR_PTR pPin, 
					 CK_ULONG ulPinLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	����:C_Logout
 *	����:�ǳ��Ự
 *	����:hSession:[IN]:�Ự���
 *	����:�ɹ�����CKR_OK�����򷵻ش�����롣
 *************************************************************************/
CK_RV P11API C_Logout(CK_SESSION_HANDLE hSession)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}



CK_RV P11API C_CreateObject(CK_SESSION_HANDLE hSession, 
							CK_ATTRIBUTE_PTR pTemplate, 
							CK_ULONG ulCount, 
							CK_OBJECT_HANDLE_PTR phObject)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_CopyObject(CK_SESSION_HANDLE hSession, 
						  CK_OBJECT_HANDLE hObject, 
						  CK_ATTRIBUTE_PTR pTemplate, 
						  CK_ULONG ulCount, 
						  CK_OBJECT_HANDLE_PTR phNewObject)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_DestroyObject(CK_SESSION_HANDLE hSession, 
							 CK_OBJECT_HANDLE hObject)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_GetObjectSize(CK_SESSION_HANDLE hSession, 
							 CK_OBJECT_HANDLE hObject, 
							 CK_ULONG_PTR pulSize)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_GetAttributeValue(CK_SESSION_HANDLE hSession, 
								 CK_OBJECT_HANDLE hObject, 
								 CK_ATTRIBUTE_PTR pTemplate, 
								 CK_ULONG ulCount)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_SetAttributeValue(CK_SESSION_HANDLE hSession, 
								 CK_OBJECT_HANDLE hObject, 
								 CK_ATTRIBUTE_PTR pTemplate, 
								 CK_ULONG ulCount)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_FindObjectsInit(CK_SESSION_HANDLE hSession, 
							   CK_ATTRIBUTE_PTR pTemplate, 
							   CK_ULONG ulCount)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_FindObjects(CK_SESSION_HANDLE hSession, 
						   CK_OBJECT_HANDLE_PTR phObject, 
						   CK_ULONG ulMaxObjectCount, 
						   CK_ULONG_PTR pulObjectCount)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_FindObjectsFinal(CK_SESSION_HANDLE hSession)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}




CK_RV P11API C_EncryptInit(CK_SESSION_HANDLE hSession, 
						   CK_MECHANISM_PTR pMechanism, 
						   CK_OBJECT_HANDLE hKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_Encrypt(CK_SESSION_HANDLE hSession, 
					   CK_BYTE_PTR pData, 
					   CK_ULONG ulDataLen, 
					   CK_BYTE_PTR pEncryptedData, 
					   CK_ULONG_PTR pulEncryptedDataLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_EncryptUpdate(CK_SESSION_HANDLE hSession, 
							 CK_BYTE_PTR pPart, 
							 CK_ULONG ulPartLen, 
							 CK_BYTE_PTR pEncryptedPart, 
							 CK_ULONG_PTR pulEncryptedPartLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_EncryptFinal(CK_SESSION_HANDLE hSession, 
							CK_BYTE_PTR pLastEncryptedPart, 
							CK_ULONG_PTR pulLastEncryptedPartLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_DecryptInit(CK_SESSION_HANDLE hSession, 
						   CK_MECHANISM_PTR pMechanism, 
						   CK_OBJECT_HANDLE hKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_Decrypt(CK_SESSION_HANDLE hSession, 
					   CK_BYTE_PTR pEncryptedData, 
					   CK_ULONG ulEncryptedDataLen, 
					   CK_BYTE_PTR pData, 
					   CK_ULONG_PTR pulDataLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_DecryptUpdate(CK_SESSION_HANDLE hSession, 
							 CK_BYTE_PTR pEncryptedPart, 
							 CK_ULONG ulEncryptedPartLen, 
							 CK_BYTE_PTR pPart, 
							 CK_ULONG_PTR pulPartLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_DecryptFinal(CK_SESSION_HANDLE hSession, 
							CK_BYTE_PTR pLastPart, 
							CK_ULONG_PTR pulLastPartLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}



CK_RV P11API C_DigestInit(CK_SESSION_HANDLE hSession, 
						  CK_MECHANISM_PTR pMechanism)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_Digest(CK_SESSION_HANDLE hSession, 
					  CK_BYTE_PTR pData, 
					  CK_ULONG ulDataLen, 
					  CK_BYTE_PTR pDigest, 
					  CK_ULONG_PTR pulDigestLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_DigestUpdate(CK_SESSION_HANDLE hSession, 
							CK_BYTE_PTR pPart, 
							CK_ULONG ulPartLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_DigestKey(CK_SESSION_HANDLE hSession, 
						 CK_OBJECT_HANDLE hKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_DigestFinal(CK_SESSION_HANDLE hSession, 
						   CK_BYTE_PTR pDigest, 
						   CK_ULONG_PTR pulDigestLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}


CK_RV P11API C_SignInit(CK_SESSION_HANDLE hSession, 
						CK_MECHANISM_PTR pMechanism, 
						CK_OBJECT_HANDLE hKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_Sign(CK_SESSION_HANDLE hSession, 
					CK_BYTE_PTR pData, 
					CK_ULONG ulDataLen, 
					CK_BYTE_PTR pSignature, 
					CK_ULONG_PTR pulSignatureLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_SignUpdate(CK_SESSION_HANDLE hSession, 
						  CK_BYTE_PTR pPart, 
						  CK_ULONG ulPartLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_SignFinal(CK_SESSION_HANDLE hSession, 
						 CK_BYTE_PTR pSignature, 
						 CK_ULONG_PTR pulSignatureLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_SignRecoverInit(CK_SESSION_HANDLE hSession, 
							   CK_MECHANISM_PTR pMechanism, 
							   CK_OBJECT_HANDLE hKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_SignRecover(CK_SESSION_HANDLE hSession, 
						   CK_BYTE_PTR pData, 
						   CK_ULONG ulDataLen, 
						   CK_BYTE_PTR pSignature, 
						   CK_ULONG_PTR pulSignatureLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}


CK_RV P11API C_VerifyInit(CK_SESSION_HANDLE hSession, 
						  CK_MECHANISM_PTR pMechanism, 
						  CK_OBJECT_HANDLE hKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_Verify(CK_SESSION_HANDLE hSession, 
					  CK_BYTE_PTR pData, 
					  CK_ULONG ulDataLen, 
					  CK_BYTE_PTR pSignature, 
					  CK_ULONG ulSignatureLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_VerifyUpdate(CK_SESSION_HANDLE hSession, 
							CK_BYTE_PTR pPart, 
							CK_ULONG ulPartLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_VerifyFinal(CK_SESSION_HANDLE hSession, 
						   CK_BYTE_PTR pSignature, 
						   CK_ULONG ulSignatureLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_VerifyRecoverInit(CK_SESSION_HANDLE hSession, 
								 CK_MECHANISM_PTR pMechanism, 
								 CK_OBJECT_HANDLE hKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_VerifyRecover(CK_SESSION_HANDLE hSession, 
							 CK_BYTE_PTR pSignature,
							 CK_ULONG ulSignatureLen, 
							 CK_BYTE_PTR pData, 
							 CK_ULONG_PTR pulDataLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}


CK_RV P11API C_DigestEncryptUpdate(CK_SESSION_HANDLE hSession, 
								   CK_BYTE_PTR pPart, 
								   CK_ULONG ulPartLen, 
								   CK_BYTE_PTR pEncryptedPart, 
								   CK_ULONG_PTR pulEncryptedPartLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_DecryptDigestUpdate(CK_SESSION_HANDLE hSession, 
								   CK_BYTE_PTR pEncryptedPart, 
								   CK_ULONG ulEncryptedPartLen, 
								   CK_BYTE_PTR pPart, 
								   CK_ULONG_PTR pulPartLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_SignEncryptUpdate(CK_SESSION_HANDLE hSession, 
								 CK_BYTE_PTR pPart, 
								 CK_ULONG ulPartLen, 
								 CK_BYTE_PTR pEncryptedPart, 
								 CK_ULONG_PTR pulEncryptedPartLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_DecryptVerifyUpdate(CK_SESSION_HANDLE hSession, 
								   CK_BYTE_PTR pEncryptedPart, 
								   CK_ULONG ulEncryptedPartLen, 
								   CK_BYTE_PTR pPart, 
								   CK_ULONG_PTR pulPartLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}


CK_RV P11API C_GenerateKey(CK_SESSION_HANDLE hSession, 
						   CK_MECHANISM_PTR pMechanism, 
						   CK_ATTRIBUTE_PTR pTemplate, 
						   CK_ULONG ulCount, 
						   CK_OBJECT_HANDLE_PTR phKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_GenerateKeyPair(CK_SESSION_HANDLE hSession, 
							   CK_MECHANISM_PTR pMechanism, 
							   CK_ATTRIBUTE_PTR pPublicKeyTemplate, 
							   CK_ULONG ulPublicKeyAttributeCount, 
							   CK_ATTRIBUTE_PTR pPrivateKeyTemplate, 
							   CK_ULONG ulPrivateKeyAttributeCount, 
							   CK_OBJECT_HANDLE_PTR phPublicKey, 
							   CK_OBJECT_HANDLE_PTR phPrivateKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_WrapKey(CK_SESSION_HANDLE hSession, 
					   CK_MECHANISM_PTR pMechanism, 
					   CK_OBJECT_HANDLE hWrappingKey, 
					   CK_OBJECT_HANDLE hKey, 
					   CK_BYTE_PTR pWrappedKey, 
					   CK_ULONG_PTR pulWrappedKeyLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_UnwrapKey(CK_SESSION_HANDLE hSession, 
						 CK_MECHANISM_PTR pMechanism, 
						 CK_OBJECT_HANDLE hUnwrappingKey, 
						 CK_BYTE_PTR pWrappedKey, 
						 CK_ULONG ulWrappedKeyLen, 
						 CK_ATTRIBUTE_PTR pTemplate, 
						 CK_ULONG ulAttributeCount, 
						 CK_OBJECT_HANDLE_PTR phKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_DeriveKey(CK_SESSION_HANDLE hSession, 
						 CK_MECHANISM_PTR pMechanism, 
						 CK_OBJECT_HANDLE hBaseKey, 
						 CK_ATTRIBUTE_PTR pTemplate, 
						 CK_ULONG ulCount, 
						 CK_OBJECT_HANDLE_PTR phKey)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}


CK_RV P11API C_SeedRandom(CK_SESSION_HANDLE hSession, 
						  CK_BYTE_PTR pSeed, 
						  CK_ULONG ulSeedLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_GenerateRandom(CK_SESSION_HANDLE hSession, 
							  CK_BYTE_PTR pRandomData, 
							  CK_ULONG ulRandomLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_GetFunctionStatus(CK_SESSION_HANDLE hSession)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_CancelFunction(CK_SESSION_HANDLE hSession)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}

CK_RV P11API C_WaitForSlotEvent(CK_FLAGS flags, 
								CK_SLOT_ID_PTR pSlot, 
								CK_VOID_PTR pRserved)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}