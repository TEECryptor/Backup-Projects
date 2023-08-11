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
 *	名称:C_Initialize
 *	介绍:PKCS11接口初始化
 *	参数:pInitArgs:[IN]:初始化参数
 *	返回:成功返回CKR_OK，否则返回错误代码。
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
 *	名称:C_Finalize
 *	介绍:PKCS11接口初始化
 *	参数:pReserved:[IN]:保留参数
 *	返回:成功返回CKR_OK，否则返回错误代码。
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
 *	名称:C_GetInfo
 *	介绍:返回该PKCS11库的基本信息
 *	参数:pInfo:[OUT]:库信息数据
 *	返回:成功返回CKR_OK，否则返回错误代码。
 *************************************************************************/
CK_RV P11API C_GetInfo(CK_INFO_PTR pInfo)
{
	// 参数检查
	if (!pInfo) 
	{
		return CKR_ARGUMENTS_BAD;
	}

	// 版本
	pInfo->cryptokiVersion.major = 2;
	pInfo->cryptokiVersion.minor = 0;

	// 厂商
	strcpy_s(pInfo->manufacturerID, 32, "Syno Chip"); 

	// 标记
	pInfo->flags = 0;

	// 库描述
	memset(pInfo->libraryDescription,' ', 32);

	// 库版本
	pInfo->libraryVersion.major = 2;
	pInfo->libraryVersion.minor = 0;

	return CKR_OK;
}
/**************************************************************************
 *	名称:C_GetFunctionList
 *	介绍:返回该PKCS11库的接口函数地址列表
 *	参数:ppFunctionList:[OUT]:函数地址列表
 *	返回:成功返回CKR_OK，否则返回错误代码。
 *************************************************************************/
CK_RV P11API C_GetFunctionList(CK_FUNCTION_LIST_PTR_PTR ppFunctionList)
{
	// 参数检查
	if (!ppFunctionList) 
	{
		return CKR_ARGUMENTS_BAD;
	}

	// 版本信息
	(*ppFunctionList)->version.major = 2;
	(*ppFunctionList)->version.minor = 0;

	return CKR_OK;
}
/**************************************************************************
 *	名称:C_GetSlotList
 *	介绍:返回槽列表
 *	参数:tokenPresent:[IN]:是否只返回有Token的槽
 *		 pSlotList:[OUT]:返回的槽ID列表
 *		 pulCount:[OUT]:返回的槽的个数
 *	返回:成功返回CKR_OK，否则返回错误代码。
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

	// 参数检查
	if (!pulCount)
	{
		return CKR_ARGUMENTS_BAD;
	}

	// 枚举设备，返回设备名长度
	rv = SKF_EnumDev(tokenPresent, NULL, &ulSize);
	if (SAR_OK != rv || ulSize <= 2)
	{
		*pulCount = 0;
		return CKR_OK;
	}

	lpszNameList = (LPSTR)malloc(ulSize + 1);

	// 枚举设备，返回设备名内容
	rv = SKF_EnumDev(tokenPresent, lpszNameList, &ulSize);
	if (SAR_OK != rv)
	{
		*pulCount = 0;
		rv = CKR_FUNCTION_FAILED;
		goto END;
	}

	// 计算Slot个数
	g_ulSlotCount = 0;
	lpszName = lpszNameList;
	while (!lpszName)
	{
		g_ulSlotCount++;
		lpszName += strlen(lpszName) + 1;
	}

	// 分配内存
	if (g_pSlotList)
	{
		free(g_pSlotList);
		g_pSlotList = NULL;
	}
	g_pSlotList = (LPSLOTINFO)malloc(g_ulSlotCount*sizeof(SlotInfo));

	// 保存每个Slot的ID和设备名
	g_ulSlotCount = 0;
	lpszName = lpszNameList;
	while (!lpszName)
	{
		g_pSlotList[g_ulSlotCount].ulSlotID = g_ulSlotCount + 1;
		strcpy_s(g_pSlotList[g_ulSlotCount].csDevName, 64, lpszName);
		lpszName += strlen(lpszName) + 1;
		g_ulSlotCount++;
	}

	// 返回设备个数
	if (!pSlotList)
	{
		*pulCount = g_ulSlotCount;
		rv = CKR_OK;
		goto END;
	}

	// 返回所有设备序号
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
 *	名称:C_GetSlotInfo
 *	介绍:返回槽信息
 *	参数:slotID:[IN]:槽ID
 *		 pInfo:[OUT]:返回的槽信息数据
 *	返回:成功返回CKR_OK，否则返回错误代码。
 *************************************************************************/
CK_RV P11API C_GetSlotInfo(CK_SLOT_ID slotID, 
						   CK_SLOT_INFO_PTR pInfo)
{
	CK_RV rv = CKR_OK;

	// 参数检查
	if (slotID <= 0 || slotID > g_ulSlotCount)
	{
		return CKR_ARGUMENTS_BAD;
	}

	// 获取Slot信息
	rv = SLT_GetSlotInfo(slotID, pInfo);
	if (CKR_OK != rv)
	{
		return rv;
	}

	return CKR_OK;
}
/**************************************************************************
 *	名称:C_GetTokenInfo
 *	介绍:返回令牌信息
 *	参数:slotID:[IN]:槽ID
 *		 pInfo:[OUT]:返回的令牌信息数据
 *	返回:成功返回CKR_OK，否则返回错误代码。
 *************************************************************************/
CK_RV P11API C_GetTokenInfo(CK_SLOT_ID slotID, 
							CK_TOKEN_INFO_PTR pInfo)
{
	CK_RV rv = CKR_OK;

	// 参数检查
	if (slotID <= 0 || slotID > g_ulSlotCount)
	{
		return CKR_ARGUMENTS_BAD;
	}

	// 获取Token信息
	rv = SLT_GetTokenInfo(slotID, pInfo);
	if (CKR_OK != rv)
	{
		return rv;
	}

	return CKR_OK;
}
/**************************************************************************
 *	名称:C_GetMechanismList
 *	介绍:返回令牌所支持的算法信息
 *	参数:slotID:[IN]:槽ID
 *		 pMechanismList:[OUT]:返回的算法信息
 *		 pulCount:[OUT]:算法个数
 *	返回:成功返回CKR_OK，否则返回错误代码。
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
 *	名称:C_GetMechanismInfo
 *	介绍:返回一个所支持的算法信息
 *	参数:slotID:[IN]:槽ID
 *		 type:[IN]:算法类型
 *		 pInfo:[OUT]:算法信息
 *	返回:成功返回CKR_OK，否则返回错误代码。
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
 *	名称:C_InitToken
 *	介绍:初始化令牌
 *	参数:slotID:[IN]:槽ID
 *		 pPin:[IN]:初始SO PIN
 *		 ulPinLen:[IN]:SO PIN长度
 *		 pLabel:[IN]:标签
 *	返回:成功返回CKR_OK，否则返回错误代码。
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
 *	名称:C_InitPIN
 *	介绍:设置初始用户PIN
 *	参数:hSession:[IN]:会话句柄
 *		 pPin:[IN]:初始User PIN
 *		 ulPinLen:[IN]:User PIN长度
 *	返回:成功返回CKR_OK，否则返回错误代码。
 *************************************************************************/
CK_RV P11API C_InitPIN(CK_SESSION_HANDLE hSession, 
					   CK_UTF8CHAR_PTR pPin, 
					   CK_ULONG ulPinLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	名称:C_SetPIN
 *	介绍:修改当前登录类型的PIN码
 *	参数:hSession:[IN]:会话句柄
 *		 pOldPin:[IN]:老PIN码
 *		 ulOldLen:[IN]:老PIN码长度
 *		 pNewPin:[IN]:新PIN码
 *		 ulNewLen:[IN]:新PIN码长度
 *	返回:成功返回CKR_OK，否则返回错误代码。
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
 *	名称:C_OpenSession
 *	介绍:打开会话句柄
 *	参数:slotID:[IN]:槽ID
 *		 flags:[IN]:CK_SESSION_INFO中的标记
 *		 pApplication:[IN]:回调函数中的参数
 *		 Notify:[IN]:回调函数
 *		 phSession:[OUT]:返回打开的会话句柄
 *	返回:成功返回CKR_OK，否则返回错误代码。
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
 *	名称:C_CloseSession
 *	介绍:关闭一个会话句柄
 *	参数:hSession:[IN]:要关闭的会话句柄
 *	返回:成功返回CKR_OK，否则返回错误代码。
 *************************************************************************/
CK_RV P11API C_CloseSession(CK_SESSION_HANDLE hSession)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	名称:C_CloseAllSessions
 *	介绍:关闭所有打开的会话
 *	参数:slotID:[IN]:槽ID
 *	返回:成功返回CKR_OK，否则返回错误代码。
 *************************************************************************/
CK_RV P11API C_CloseAllSessions(CK_SLOT_ID slotID)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	名称:C_GetSessionInfo
 *	介绍:获取会话信息
 *	参数:hSession:[IN]:会话句柄
 *		 pInfo:[OUT]:返回会话信息
 *	返回:成功返回CKR_OK，否则返回错误代码。
 *************************************************************************/
CK_RV P11API C_GetSessionInfo(CK_SESSION_HANDLE hSession, 
							  CK_SESSION_INFO_PTR pInfo)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	名称:C_GetOperationState
 *	介绍:获取操作状态
 *	参数:hSession:[IN]:会话句柄
 *		 pOperationState:[OUT]:返回操作状态内容
 *		 pulOperationStateLen:[OUT]:返回操作状态内容长度
 *	返回:成功返回CKR_OK，否则返回错误代码。
 *************************************************************************/
CK_RV P11API C_GetOperationState(CK_SESSION_HANDLE hSession, 
								 CK_BYTE_PTR pOperationState, 
								 CK_ULONG_PTR  pulOperationStateLen)
{
	CK_RV rv = CKR_FUNCTION_NOT_SUPPORTED;

	return rv;
}
/**************************************************************************
 *	名称:C_SetOperationState
 *	介绍:设置操作状态
 *	参数:hSession:[IN]:会话句柄
 *		 pOperationState:[IN]:操作状态
 *		 ulOperationStateLen:[IN]:操作状态长度
 *		 hEncryptionKey:[IN]:加/解密Key句柄
 *		 hAuthenticationKey:[IN]:签名/验签Key句柄
 *	返回:成功返回CKR_OK，否则返回错误代码。
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
 *	名称:C_Login
 *	介绍:登录会话
 *	参数:hSession:[IN]:会话句柄
 *		 userType:[IN]:用户类型，SO or User
 *		 pPin:[IN]:PIN码
 *		 ulPinLen:[IN]:PIN码长度
 *	返回:成功返回CKR_OK，否则返回错误代码。
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
 *	名称:C_Logout
 *	介绍:登出会话
 *	参数:hSession:[IN]:会话句柄
 *	返回:成功返回CKR_OK，否则返回错误代码。
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