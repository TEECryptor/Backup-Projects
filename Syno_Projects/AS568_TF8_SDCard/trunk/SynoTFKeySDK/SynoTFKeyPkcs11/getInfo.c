
 
static char RCSID[]="$Id: getInfo.c,v 1.5 2000/01/31 18:09:02 lbe Exp $";
const char* Version_getInfo_c(){return RCSID;}

/* Needed for Win32-isms in cryptoki.h */
#ifndef CK_I_library_build
#define CK_I_library_build
#endif

#include "internal.h"
#include "error.h"

#include <string.h>
#include <stdio.h>
#include <signal.h>

#if defined(CK_Win32) 
#include "windows.h"
#include "winuser.h"
#endif 

CK_FUNCTION_LIST ck_function_list;

void set_function_list(void)
{
	ck_function_list.version.major=         LIBRARY_VERSION_MAJOR;
	ck_function_list.version.minor=         LIBRARY_VERSION_MINOR;
	ck_function_list.C_Initialize= 	  &C_Initialize;
	ck_function_list.C_Finalize= 		  &C_Finalize;
	ck_function_list.C_GetInfo= 		  &C_GetInfo;
	ck_function_list.C_GetFunctionList= 	  &C_GetFunctionList;
	ck_function_list.C_GetSlotList= 	  &C_GetSlotList;
	ck_function_list.C_GetSlotInfo= 	  &C_GetSlotInfo;
	ck_function_list.C_GetTokenInfo= 	  &C_GetTokenInfo;
	ck_function_list.C_GetMechanismList= 	  &C_GetMechanismList;
	ck_function_list.C_GetMechanismInfo= 	  &C_GetMechanismInfo;
	ck_function_list.C_InitToken= 	  &C_InitToken;
	ck_function_list.C_InitPIN= 		  &C_InitPIN;
	ck_function_list.C_SetPIN= 		  &C_SetPIN;
	ck_function_list.C_OpenSession= 	  &C_OpenSession;
	ck_function_list.C_CloseSession= 	  &C_CloseSession;
	ck_function_list.C_CloseAllSessions= 	  &C_CloseAllSessions;
	ck_function_list.C_GetSessionInfo= 	  &C_GetSessionInfo;
	ck_function_list.C_GetOperationState=   &C_GetOperationState;
	ck_function_list.C_SetOperationState=   &C_SetOperationState;
	ck_function_list.C_Login= 		  &C_Login;
	ck_function_list.C_Logout= 		  &C_Logout;
	ck_function_list.C_CreateObject= 	  &C_CreateObject;
	ck_function_list.C_CopyObject= 	  &C_CopyObject;
	ck_function_list.C_DestroyObject= 	  &C_DestroyObject;
	ck_function_list.C_GetObjectSize= 	  &C_GetObjectSize;
	ck_function_list.C_GetAttributeValue=   &C_GetAttributeValue;
	ck_function_list.C_SetAttributeValue=   &C_SetAttributeValue;
	ck_function_list.C_FindObjectsInit= 	  &C_FindObjectsInit;
	ck_function_list.C_FindObjects= 	  &C_FindObjects;
	ck_function_list.C_FindObjectsFinal= 	  &C_FindObjectsFinal;
	ck_function_list.C_EncryptInit= 	  &C_EncryptInit;
	ck_function_list.C_Encrypt= 		  &C_Encrypt;
	ck_function_list.C_EncryptUpdate= 	  &C_EncryptUpdate;
	ck_function_list.C_EncryptFinal= 	  &C_EncryptFinal;
	ck_function_list.C_DecryptInit= 	  &C_DecryptInit;
	ck_function_list.C_Decrypt= 		  &C_Decrypt;
	ck_function_list.C_DecryptUpdate= 	  &C_DecryptUpdate;
	ck_function_list.C_DecryptFinal= 	  &C_DecryptFinal;
	ck_function_list.C_DigestInit= 	  &C_DigestInit;
	ck_function_list.C_Digest= 		  &C_Digest;
	ck_function_list.C_DigestUpdate= 	  &C_DigestUpdate;
	ck_function_list.C_DigestKey= 	  &C_DigestKey;
	ck_function_list.C_DigestFinal= 	  &C_DigestFinal;
	ck_function_list.C_SignInit= 		  &C_SignInit;
	ck_function_list.C_Sign= 		  &C_Sign;
	ck_function_list.C_SignUpdate= 	  &C_SignUpdate;
	ck_function_list.C_SignFinal= 	  &C_SignFinal;
	ck_function_list.C_SignRecoverInit= 	  &C_SignRecoverInit;
	ck_function_list.C_SignRecover= 	  &C_SignRecover;
	ck_function_list.C_VerifyInit= 	  &C_VerifyInit;
	ck_function_list.C_Verify= 		  &C_Verify;
	ck_function_list.C_VerifyUpdate= 	  &C_VerifyUpdate;
	ck_function_list.C_VerifyFinal= 	  &C_VerifyFinal;
	ck_function_list.C_VerifyRecoverInit=   &C_VerifyRecoverInit;
	ck_function_list.C_VerifyRecover= 	  &C_VerifyRecover;
	ck_function_list.C_DigestEncryptUpdate= &C_DigestEncryptUpdate;
	ck_function_list.C_DecryptDigestUpdate= &C_DecryptDigestUpdate;
	ck_function_list.C_SignEncryptUpdate=   &C_SignEncryptUpdate;
	ck_function_list.C_DecryptVerifyUpdate= &C_DecryptVerifyUpdate;
	ck_function_list.C_GenerateKey= 	  &C_GenerateKey;
	ck_function_list.C_GenerateKeyPair= 	  &C_GenerateKeyPair;
	ck_function_list.C_WrapKey= 		  &C_WrapKey;
	ck_function_list.C_UnwrapKey= 	  &C_UnwrapKey;
	ck_function_list.C_DeriveKey= 	  &C_DeriveKey;
	ck_function_list.C_SeedRandom= 	  &C_SeedRandom;
	ck_function_list.C_GenerateRandom= 	  &C_GenerateRandom;
	ck_function_list.C_GetFunctionStatus=   &C_GetFunctionStatus;
	ck_function_list.C_CancelFunction= 	  &C_CancelFunction;
	ck_function_list.C_WaitForSlotEvent= 	  &C_WaitForSlotEvent;
}

CK_RV C_GetInfo(
	CK_INFO_PTR pInfo
	)
{
	CK_RV rv = CKR_OK;

	FUNC_BEGIN;

	/* make sure we are initialized */
	if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
	{
        SYNO_LogE("Cryptoki has not initialized!");
		return CKR_CRYPTOKI_NOT_INITIALIZED;
	}
	if (pInfo == NULL_PTR)
	{
        SYNO_LogE("pInfo is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	pInfo->cryptokiVersion.major = 0x2;
	pInfo->cryptokiVersion.minor = 0x1;

	strncpy_s((char*)pInfo->manufacturerID, 32, "Syno Chip CO.       ", 32);

	pInfo->flags = 0;

	#if defined(BUILD_MAMOOTH)
	strncpy_s((char*) pInfo->libraryDescription, 32, "TrustCenter PKCS#11 Library(M)  ", 32);
	#else
	strncpy_s((char*) pInfo->libraryDescription, 32, "Syno PKCS#11 Library     ", 32);
	#endif

	pInfo->libraryVersion.major = LIBRARY_VERSION_MAJOR;
	pInfo->libraryVersion.minor = LIBRARY_VERSION_MINOR;

	FUNC_END;

	return rv;
}

CK_RV C_GetFunctionList(
	CK_FUNCTION_LIST_PTR_PTR ppFunctionList
	)
{
	CK_RV rv = CKR_OK;

	FUNC_BEGIN;
	
	if (ppFunctionList == NULL_PTR)
	{
        SYNO_LogE("ppFunctionList is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	
	if (!(CK_I_global_flags & CK_IGF_FUNCTIONLIST_SET))
	{
		set_function_list();
		CK_I_global_flags |= CK_IGF_FUNCTIONLIST_SET;
	}

	*ppFunctionList = &ck_function_list;

	FUNC_END;

	return rv;
}



