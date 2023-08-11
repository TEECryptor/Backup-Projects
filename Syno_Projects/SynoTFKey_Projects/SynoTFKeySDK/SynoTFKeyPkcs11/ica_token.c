/******************************************************************************

                  版权所有 (C), 2013-2023, 安徽云盾信息技术有限公司

 ******************************************************************************
  文 件 名   : ica_token.c
  版 本 号   : 初稿
  作    者   : 张晓垒
  生成日期   : 2013年10月12日
  最近修改   : 2013年10月12日
  功能描述   :
  函数列表   :
  修改历史   :
  1.日    期 : 2013年10月12日
    作    者 : 张晓垒
    修改内容 : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
//#include "protocol.h"

#include "ica_token.h"
#include "template.h"
#include "objects.h"
#include "utils.h"

//#include "ica_log.h"
#include "hash.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
CK_BBOOL          s_ucTrue = TRUE;
CK_BBOOL          s_ucFalse = FALSE;

/* RSA密钥对象属性值 */
CK_OBJECT_CLASS   s_ulPubClass = CKO_PUBLIC_KEY;
CK_OBJECT_CLASS   s_ulPrivClass = CKO_PRIVATE_KEY;
CK_KEY_TYPE       s_ulRSAKeyType = CKK_RSA;
CK_CHAR           s_aucRSAPublicLabel[] = "RSAPublicKey";
CK_CHAR           s_aucRSAPrivateLabel[] = "RSAPrivateKey";
CK_ULONG          s_ulModulusBits = 1024;
CK_BYTE           s_aucPublicExponent[4] = {0,1,0,1};

/* RAS公钥对象模板 */
CK_ATTRIBUTE g_astRSAPublicKeyTemplate[10] = {
	{CKA_CLASS, &s_ulPubClass, sizeof(s_ulPubClass)},  /* 对象分类 */
	{CKA_KEY_TYPE, &s_ulRSAKeyType, sizeof(s_ulRSAKeyType)},   /* 密钥类型 */
	{CKA_ID, NULL, 0},  /* 密钥的密钥标识符 */
	{CKA_LABEL, s_aucRSAPublicLabel, sizeof(s_aucRSAPublicLabel)}, /* 对象的说明 */
	{CKA_TOKEN, &s_ucTrue, sizeof(s_ucTrue)}, /* 如果是令牌对象为true,否则为false */
	{CKA_ENCRYPT, &s_ucTrue, sizeof(s_ucTrue)}, /* 如果密钥支持加密,则为TRUE */
	{CKA_WRAP, &s_ucTrue, sizeof(s_ucTrue)}, /* 如果密钥支持打包其它密钥,则为TRUE */
	{CKA_MODULUS, NULL_PTR, 0}, /* 模数n */
	{CKA_MODULUS_BITS, &s_ulModulusBits, sizeof(s_ulModulusBits)}, /* 模数n的位长度 */
	{CKA_PUBLIC_EXPONENT, s_aucPublicExponent, sizeof(s_aucPublicExponent)} /* 公用指数e */
};

/* RAS私钥对象模板 */
CK_ATTRIBUTE g_astRSAPrivateKeyTemplate[8] = {
    {CKA_CLASS, &s_ulPrivClass, sizeof(s_ulPrivClass)},
    {CKA_KEY_TYPE, &s_ulRSAKeyType, sizeof(s_ulRSAKeyType)},
    {CKA_ID, NULL, 0},  /* 密钥的密钥标识符 */
    //{CKA_LABEL, s_aucRSAPrivateLabel, sizeof(s_aucRSAPrivateLabel)},
	{CKA_TOKEN, &s_ucTrue, sizeof(s_ucTrue)},
	{CKA_SENSITIVE, &s_ucFalse, sizeof(s_ucFalse)},
	{CKA_DECRYPT, &s_ucTrue, sizeof(s_ucTrue)},
	{CKA_UNWRAP, &s_ucTrue, sizeof(s_ucTrue)},
    {CKA_EXTRACTABLE, &s_ucTrue, sizeof(s_ucTrue)},
    //{CKA_NEVER_EXTRACTABLE, &s_ucFalse, sizeof(s_ucFalse)}
};

/* DES保密密钥对象属性值 */
CK_OBJECT_CLASS s_ulSecretClass = CKO_SECRET_KEY;
CK_KEY_TYPE s_ulDESKeyType = CKK_DES;
CK_CHAR s_aucDESLabel[] = "DESSecretKey";
CK_ULONG s_ulEight = 8;   /* 密钥值(始终为8字节长) */
CK_BYTE s_aucValue[8] = {0};

/* DES保密秘钥模板 */
CK_ATTRIBUTE g_astDESKeyTemplate[10] = {
    {CKA_CLASS, &s_ulSecretClass, sizeof(s_ulSecretClass)},
    {CKA_KEY_TYPE, &s_ulDESKeyType, sizeof(s_ulDESKeyType)},
    {CKA_LABEL, s_aucDESLabel, sizeof(s_aucDESLabel)},
    {CKA_TOKEN, &s_ucFalse, sizeof(s_ucFalse)},
    {CKA_ENCRYPT, &s_ucTrue, sizeof(s_ucTrue)},
    {CKA_DECRYPT, &s_ucTrue, sizeof(s_ucTrue)},
    {CKA_EXTRACTABLE, &s_ucTrue, sizeof(s_ucTrue)},
    {CKA_NEVER_EXTRACTABLE, &s_ucFalse, sizeof(s_ucFalse)},
    {CKA_VALUE, s_aucValue, sizeof(s_aucValue)},
    {CKA_VALUE_LEN, &s_ulEight, sizeof(s_ulEight)}
};

CK_ATTRIBUTE g_astFindRSATemplate[2] = {
    {CKA_CLASS, NULL, 0},
	{CKA_TOKEN, &s_ucTrue, sizeof(s_ucTrue)},
};

CK_ATTRIBUTE g_astGetRSAKeyIDTemplate[1] = {
    {CKA_ID, NULL, 0},  /* 密钥的密钥标识符 */
};

CK_ATTRIBUTE g_astGetRSAPublicKeyTemplate[1] = {
    {CKA_MODULUS, NULL_PTR, 0},
};

/* 定义RAS机制 */
CK_MECHANISM g_stMechGenRSA = {CKM_RSA_PKCS_KEY_PAIR_GEN, NULL_PTR, 0};
CK_MECHANISM g_stMechRSA = {CKM_RSA_PKCS, NULL_PTR, 0};

/* DES机制 */
CK_MECHANISM g_stMechGenDES = {CKM_DES_KEY_GEN, NULL_PTR, 0};
CK_MECHANISM g_stDESmech = {CKM_DES_ECB, NULL_PTR, 0};

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
extern CK_RV CI_ObjTemplateInit(CK_I_OBJ_PTR CK_PTR ppObjectRef,
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulTemplateLen);

extern CK_RV CI_ObjDestroyObj(CK_I_OBJ_PTR  pObject);

extern CK_RV CI_ContainerAddObj(CK_I_HASHTABLE_PTR container,
    CK_ULONG key, CK_I_OBJ_PTR pObject);

extern CK_RV CI_NewHandle(CK_ULONG_PTR handle);


/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define ICA_TOK_ONCE_RW_LEN             2048

#define ICA_TOK_MAX_SESSION_COUNT       100
#define ICA_TOK_MAX_RW_SESSION_COUNT    100
#define ICA_TOK_MIN_PIN_LEN             8
#define ICA_TOK_MAX_PIN_LEN             255

#define ICA_TOK_SO_PIN_RETRY_COUNT      3
#define ICA_TOK_USER_PIN_RETRY_COUNT    3


/*----------------------------------------------*
 * 内部函数定义                                 *
 *----------------------------------------------*/
static ULONG ICA_CopyObject(CK_I_HASHTABLE_PTR pstDestObjList,
    CK_I_HASHTABLE_PTR pstSrcObjList)
{
    CK_I_HASH_ITERATOR_PTR pIter;
    CK_ULONG key;
    CK_I_OBJ_PTR val;
    CK_RV rv = CKR_OK;

    /* copy all persistent objects into the session object list */
    for(CI_HashIterateInit(pstSrcObjList,&pIter);
        CI_HashIterValid(pIter); CI_HashIterateInc(pIter))
    {
        rv = CI_HashIterateDeRef(pIter,&key,(CK_VOID_PTR_PTR)&val);
        if(rv != CKR_OK)
        {
            return rv;
        }

        rv = CI_ContainerAddObj(pstDestObjList,key,val);
        if(rv != CKR_OK)
        {
            return rv;
        }
    }

    return rv;
}
#if 0
ICA_STATIC ULONG ICA_GetObjectHandle(CK_I_SLOT_DATA_PTR pstSlotData, ULONG ulObjectType,
    CK_OBJECT_HANDLE_PTR phObject, CK_ULONG_PTR pulObjectCount)
{
	int i = 0;
    ULONG ulRet = CKR_OK;
    CK_ATTRIBUTE_PTR pstFindTemplate = NULL;

    if (NULL == pstSlotData)
    {
        return CKR_GENERAL_ERROR;
    }

	/* malloc template buffer */
	pstFindTemplate = (CK_ATTRIBUTE_PTR)ICA_calloc(1, sizeof(g_astFindRSATemplate));
	if (NULL == pstFindTemplate)
	{
        return CKR_HOST_MEMORY;
	}
	memcpy(pstFindTemplate, g_astFindRSATemplate, sizeof(g_astFindRSATemplate));

	/* set template class attribute value based on key type */
	for (i = 0; i < CK_I_RSA_FIND_ATTR_COUNT; i++)
	{
		if (pstFindTemplate[i].type == CKA_CLASS)
		{
			pstFindTemplate[i].ulValueLen = sizeof(CK_OBJECT_CLASS);
			pstFindTemplate[i].pValue = (CK_OBJECT_CLASS_PTR)ICA_calloc(1, sizeof(CK_OBJECT_CLASS));
			*((CK_OBJECT_CLASS_PTR)pstFindTemplate[i].pValue) = (ICA_OBJECT_TYPE_PUBLICE == ulObjectType) ? CKO_PUBLIC_KEY : CKO_PRIVATE_KEY;
			break;
		}
	}

    ulRet = cloudshield_FindObjectsInit(pstSlotData, g_astFindRSATemplate, CK_I_RSA_FIND_ATTR_COUNT);
    if (ulRet != CKR_OK)
	{
		goto End;
	}

	ulRet = cloudshield_FindObjects(pstSlotData, phObject, ICA_FIND_OBJECT_MAX, pulObjectCount);
	if (ulRet != CKR_OK)
	{
		goto End;
	}

	ulRet = cloudshield_FindObjectsFinal(pstSlotData);
	if (ulRet != CKR_OK)
	{
		goto End;
	}

End:
	for (i = 0; i < CK_I_RSA_FIND_ATTR_COUNT; i++)
	{
		if (pstFindTemplate[i].type == CKA_CLASS)
		{
			ICA_FREE(pstFindTemplate[i].pValue);
			pstFindTemplate[i].pValue = NULL;
		}
	}
	ICA_FREE(pstFindTemplate);
	pstFindTemplate = NULL;
    return ulRet;
}



ICA_STATIC ULONG ICA_FillPublicKeyTemplate(CK_I_SLOT_DATA_PTR pstSlotData, CK_I_OBJ_PTR pstPublicObject)
{
	int i = 0;
	ULONG ulRet = CKR_OK;
	ULONG ulTemplateCnt = 0;
    CK_ATTRIBUTE_PTR pstPublicKeyTemplate = NULL;

    if ((NULL == pstSlotData) || (NULL == pstPublicObject))
    {
		return CKR_ARGUMENTS_BAD;
    }

	/* malloc attributes buffer */
	pstPublicKeyTemplate = (CK_ATTRIBUTE_PTR)ICA_calloc(1, sizeof(g_astGetRSAPublicKeyTemplate));
	if (NULL == pstPublicKeyTemplate)
	{
		return CKR_HOST_MEMORY;
	}
	memcpy(pstPublicKeyTemplate, g_astGetRSAPublicKeyTemplate, sizeof(g_astGetRSAPublicKeyTemplate));
	ulTemplateCnt = sizeof(g_astGetRSAPublicKeyTemplate)/sizeof(g_astGetRSAPublicKeyTemplate[0]);

	/* get attributes value length */
	ulRet = cloudshield_GetAttributeValue(pstSlotData, pstPublicObject->obj_id,	pstPublicKeyTemplate, ulTemplateCnt);
	if (ulRet != CKR_OK)
    {
    	return ulRet;
    }

	/* malloc buffer for attributes value */
	for (i = 0; i < ulTemplateCnt; i++)
	{
		if (NULL == pstPublicKeyTemplate[i].pValue)
		{
			pstPublicKeyTemplate[i].pValue = (CK_BYTE_PTR)ICA_calloc(1, pstPublicKeyTemplate[i].ulValueLen);
		}
	}

	/* get attributes value now */
	ulRet = cloudshield_GetAttributeValue(pstSlotData, pstPublicObject->obj_id,	pstPublicKeyTemplate, ulTemplateCnt);
	if (ulRet != CKR_OK)
    {
    	return ulRet;
    }

	/* fill attributes to object */
	for (i = 0; i < ulTemplateCnt; i++)
	{
		if (NULL != pstPublicKeyTemplate[i].pValue)
		{
			CI_ObjSetAttributeValue(pstPublicObject, pstPublicKeyTemplate[i].type, pstPublicKeyTemplate[i].pValue, pstPublicKeyTemplate[i].ulValueLen);

			if (pstPublicKeyTemplate[i].type == CKA_MODULUS)
			{
				ICA_FREE(pstPublicKeyTemplate[i].pValue);
				pstPublicKeyTemplate->pValue = NULL;
				pstPublicKeyTemplate->ulValueLen = 0;
			}
		}
	}

	/* free attributes buffer */
	ICA_FREE(pstPublicKeyTemplate);
	pstPublicKeyTemplate = NULL;

	return CKR_OK;
}

ICA_STATIC ULONG ICA_GetTokenObject(CK_I_SESSION_DATA_PTR pstSessionData, ULONG ulObjectType)
{
	ULONG ulRet = CKR_OK;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    ULONG aulHObject[ICA_FIND_OBJECT_MAX] = {0};
    CK_ULONG ulObjectCount = 0;
    CK_I_OBJ_PTR pstPublicObject = NULL_PTR;
    CK_ATTRIBUTE_PTR pstKeyTemplate = NULL;
    ULONG ulKeyTemplateCount = 0;

    ULONG ulKey = 0;
    int i, j;

    if ((NULL == pstSessionData) || (NULL == pstSessionData->slot_data))
    {
        return CKR_ARGUMENTS_BAD;
    }

    pstSlotData = pstSessionData->slot_data;

    ulRet = ICA_GetObjectHandle(pstSlotData, ulObjectType, aulHObject, &ulObjectCount);
    if (ulRet != CKR_OK)
	{
		return ulRet;
	}

	if (0 == ulObjectCount)
	{
		return CKR_GENERAL_ERROR;
	}

    switch(ulObjectType)
    {
        case ICA_OBJECT_TYPE_PUBLICE:
        {
            pstKeyTemplate = (CK_ATTRIBUTE_PTR)ICA_calloc(1, sizeof(g_astRSAPublicKeyTemplate));
            if (NULL == pstKeyTemplate)
        	{
                return CKR_HOST_MEMORY;
        	}
            memcpy(pstKeyTemplate, g_astRSAPublicKeyTemplate, sizeof(g_astRSAPublicKeyTemplate));
            ulKeyTemplateCount = CK_I_RSA_PUBLIC_KEY_ATTR_COUNT;
            break;
        }
        case ICA_OBJECT_TYPE_PRIVATE:
        {
            pstKeyTemplate = (CK_ATTRIBUTE_PTR)ICA_calloc(1, sizeof(g_astRSAPrivateKeyTemplate));
            if (NULL == pstKeyTemplate)
        	{
				return CKR_HOST_MEMORY;
        	}
            memcpy(pstKeyTemplate, g_astRSAPrivateKeyTemplate, sizeof(g_astRSAPrivateKeyTemplate));
            ulKeyTemplateCount = CK_I_RSA_PRIVATE_KEY_ATTR_COUNT;
            break;
        }
        default:
        {
            return CKR_GENERAL_ERROR;
        }
    }

    for (i = 0; i < ulObjectCount; i++)
    {
		/* create and initialize object */
		pstPublicObject = NULL_PTR;
        ulRet = CI_ObjTemplateInit(&pstPublicObject, pstKeyTemplate, ulKeyTemplateCount);
        if (ulRet != CKR_OK)
    	{
    		goto End;
    	}

        pstPublicObject->obj_id = aulHObject[i];
		pstPublicObject->session = pstSessionData;

		/* for public key, get its attributes (MODULUS, BITS and EXPONENT) */
		if (ICA_OBJECT_TYPE_PUBLICE == ulObjectType)
		{
			//ICA_FillPublicKeyTemplate(pstSlotData, pstPublicObject);
		}

        ulRet = CI_NewHandle(&ulKey);
        if(ulRet != CKR_OK)
        {
            goto End;
        }

        ulRet = CI_ContainerAddObj(pstSlotData->token_data->object_list, ulKey, pstPublicObject);
        if(ulRet != CKR_OK)
        {
			CI_ObjDestroyObj(pstPublicObject);
			pstPublicObject = NULL;
            goto End;
        }
    }

End:
	ICA_FREE(pstKeyTemplate);
	pstKeyTemplate = NULL;
    return ulRet;
}
#endif
ULONG ICA_CreatePubKeyObj(CK_I_SESSION_DATA_PTR pstSessionData,
    CHAR *pcContainerName, HCONTAINER hContainer)
{
    ULONG ulRet = CKR_OK;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    CK_I_OBJ_PTR pstPublicObject = NULL_PTR;
    RSAPUBLICKEYBLOB stRSAPubBlob = {0};
    ULONG ulBlobLen = 0;
    CK_ATTRIBUTE_PTR pstTemplate = NULL;
    HANDLE hKey = NULL;
    ULONG ulKey = 0;
    CHAR *pcLabel = NULL;

    /* 参数检查 */
    if ((NULL == pstSessionData)
        || (NULL == hContainer))
    {
        return CKR_GENERAL_ERROR;
    }

    pstSlotData = pstSessionData->slot_data;
    if (NULL == pstSlotData)
    {
        return CKR_GENERAL_ERROR;
    }

    ulRet = CI_ObjTemplateInit(&pstPublicObject,
        g_astRSAPublicKeyTemplate, CK_I_RSA_PUBLIC_KEY_ATTR_COUNT);
    if (ulRet != CKR_OK)
	{
		goto End;
	}

    ulBlobLen = sizeof(stRSAPubBlob);
    ulRet = SKF_ExportPublicKey(hContainer, FALSE, (BYTE *)&stRSAPubBlob, &ulBlobLen);
    if (ulRet != CKR_OK)
	{
		goto End;
	}

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPublicObject, CK_IA_ID));
    pstTemplate->pValue = ICA_calloc(1, strlen((char *)pcContainerName) + 1);
    strncpy((char*)pstTemplate->pValue, pcContainerName, strlen((char *)pcContainerName));
    pstTemplate->ulValueLen = strlen((char *)pcContainerName) + 1;

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPublicObject, CK_IA_MODULUS));
    pstTemplate->pValue = ICA_calloc(1, MAX_RSA_MODULUS_LEN);
    memcpy(pstTemplate->pValue, stRSAPubBlob.Modulus, MAX_RSA_MODULUS_LEN);
    pstTemplate->ulValueLen = MAX_RSA_MODULUS_LEN;

    //ulRet = SKF_GetRSAKeyHandle(hContainer, AT_KEYEXCHANGE, &hKey);
    //if (ulRet != CKR_OK)
	//{
	//	goto End;
	//}

    pstPublicObject->ObjHandle = hKey;
    pstPublicObject->CtrHandle = hContainer;
	pstPublicObject->session = pstSessionData;

    ulRet = CI_NewHandle(&ulKey);
    if(ulRet != CKR_OK)
    {
        goto End;
    }

    ulRet = CI_ContainerAddObj(pstSlotData->token_data->object_list, ulKey, pstPublicObject);
    if(ulRet != CKR_OK)
    {
		CI_ObjDestroyObj(pstPublicObject);
		pstPublicObject = NULL;
        goto End;
    }

End:
    return ulRet;
}

ULONG ICA_CreatePrivKeyObj(CK_I_SESSION_DATA_PTR pstSessionData,
    CHAR *pcContainerName, HCONTAINER hContainer)
{
    ULONG ulRet = CKR_OK;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    CK_I_OBJ_PTR pstPrivObject = NULL_PTR;
    RSAPUBLICKEYBLOB stRSAPubBlob = {0};
    ULONG ulBlobLen = 0;
    CK_ATTRIBUTE_PTR pstTemplate = NULL;
    HANDLE hKey = NULL;
    ULONG ulKey = 0;
    CHAR *pcLabel = NULL;

    /* 参数检查 */
    if ((NULL == pstSessionData)
        || (NULL == hContainer))
    {
        return CKR_GENERAL_ERROR;
    }

    pstSlotData = pstSessionData->slot_data;
    if (NULL == pstSlotData)
    {
        return CKR_GENERAL_ERROR;
    }

    ulRet = CI_ObjTemplateInit(&pstPrivObject,
        g_astRSAPrivateKeyTemplate, CK_I_RSA_PRIVATE_KEY_ATTR_COUNT);
    if (ulRet != CKR_OK)
	{
		goto End;
	}

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPrivObject, CK_IA_ID));
    pstTemplate->pValue = ICA_calloc(1, strlen((char *)pcContainerName) + 1);
    strncpy((char*)pstTemplate->pValue, (char *)pcContainerName, strlen((char *)pcContainerName));
    pstTemplate->ulValueLen = strlen((char *)pcContainerName) + 1;

    pstPrivObject->ObjHandle = hKey;
    pstPrivObject->CtrHandle = hContainer;
	pstPrivObject->session = pstSessionData;

    ulRet = CI_NewHandle(&ulKey);
    if(ulRet != CKR_OK)
    {
        goto End;
    }

    ulRet = CI_ContainerAddObj(pstSlotData->token_data->object_list, ulKey, pstPrivObject);
    if(ulRet != CKR_OK)
    {
		CI_ObjDestroyObj(pstPrivObject);
		pstPrivObject = NULL;
        goto End;
    }

End:
    return ulRet;
}



void ICA_StrFmtList(CHAR *pcDst, CHAR *pcSrc)
{
    UCHAR *pucTmp = NULL;
	ULONG ulTmpLen = 0;
	ULONG ulLen = 0;

    if ((NULL == pcDst) || (NULL == pcSrc))
    {
        return;
    }

    pucTmp = (UCHAR *)pcSrc;
	while (pucTmp[0] != '\0')
	{
		strncpy((pcDst + ulLen), (char *)pucTmp, strlen((char *)pucTmp));
		ulTmpLen = strlen(((char *)pcDst + ulLen));
		pucTmp += (ulTmpLen + 1);
		ulLen += ulTmpLen;
		if (pucTmp[0] != '\0')
		{
			//sprintf(pcDst + ulLen, 2, "%s", ", ");
			memcpy(pcDst + ulLen, ",", 1);
			ulLen += 1;
		}
	}

    return;
}

ULONG ICA_CheckName(CHAR *pcNameList, CHAR *pcName)
{
    ULONG ulRet = CKR_OK;
    CHAR acList[PATH_MAX] = {0};
    CHAR *pcCheckList = NULL;
    CHAR *pcRemainder = NULL;
    CHAR *pcCheckName = NULL;

    ICA_StrFmtList(acList, pcNameList);

    pcCheckList = (CHAR *)ICA_malloc(strlen((char *)acList) + 1);
	strcpy(pcCheckList, acList);

    /* 删除所有应用 */
    pcCheckName = (CHAR *)TC_strtok_r(pcCheckList, ",", &pcRemainder);
    while(pcCheckName != NULL)
    {
        ulRet = strcmp(pcCheckName, pcName);
        if (ulRet == 0)
        {
            break;
        }

        pcCheckName = TC_strtok_r(NULL, ",", &pcRemainder);
    }

    ICA_free(pcCheckList);

    return ulRet;
}


/*----------------------------------------------*
 * 外部函数定义                                 *
 *----------------------------------------------*/
ULONG ICA_TOK_EnumToken(CK_CHAR_PTR pucTokenList, ULONG *pulSize)
{
    ULONG ulRet = CKR_OK;
    UCHAR aucNameList[PATH_MAX] = {0};
	ULONG ulSize = PATH_MAX;
	UCHAR *pucTmp = NULL;
	ULONG ulTmpLen = 0;
	ULONG ulLen = 0;

    FUNC_BEGIN;

    /* 参数检查 */
    if ((NULL == pucTokenList) || (NULL == pulSize))
    {
        return CKR_GENERAL_ERROR;
    }

    /* 枚举设备 */
    ulRet = SKF_EnumDev(TRUE, (LPSTR)aucNameList, &ulSize);
    if (CKR_OK != ulRet)
    {
        goto End;
    }

    ICA_LogD("SKF_EnumDev OK!");

    ICA_StrFmtList((CHAR *)pucTokenList, (CHAR *)aucNameList);

End:
    FUNC_END;
    return ulRet;
}


ULONG ICA_TOK_TokenInitialize(CK_CHAR_PTR pucTokenName,
    CK_I_SLOT_DATA_PTR CK_PTR ppSlotData)
{
    ULONG ulRet = CKR_GENERAL_ERROR;
    CK_SLOT_INFO_PTR pstSlotInfo = NULL;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    CK_TOKEN_INFO_PTR pstTokenInfo = NULL;
    CK_I_TOKEN_DATA_PTR pstTokenData = NULL;
    DEVHANDLE hDev = NULL;
    DEVINFO DevInfo = {0};

    FUNC_BEGIN;

    pstSlotInfo = (CK_SLOT_INFO_PTR)ICA_calloc(1, sizeof(CK_SLOT_INFO));
    if (NULL == pstSlotInfo)
    {
        goto Err;
    }

    strcpy((char*)pstSlotInfo->slotDescription, "GNU PKCS #11 Wrapper for YunDun");
    strcpy((char*)pstSlotInfo->manufacturerID, "SN201311071000000000001");
    pstSlotInfo->hardwareVersion.major = 0;
    pstSlotInfo->hardwareVersion.minor = 1;
    pstSlotInfo->firmwareVersion.major = 0;
    pstSlotInfo->firmwareVersion.minor = 1;

    pstSlotData = (CK_I_SLOT_DATA_PTR)ICA_calloc(1, sizeof(CK_I_SLOT_DATA));
    if (NULL == pstSlotData)
    {
        goto FreeSlotInfo;
    }

    pstSlotData->flags = 0;
    pstSlotData->slot_info = pstSlotInfo;
    pstSlotData->config_section_name = (UCHAR *)ICA_calloc(1,strlen((char *)pucTokenName) + 1);
    if (NULL == pstSlotData->config_section_name)
    {
        goto FreeSlotData;
    }

    strcpy((CHAR *)pstSlotData->config_section_name, (CHAR *)pucTokenName);

    pstTokenInfo = (CK_TOKEN_INFO_PTR)ICA_calloc(1, sizeof(CK_TOKEN_INFO));
    if (NULL == pstTokenInfo)
    {
        goto FreeSectionName;
    }

    /* 连接设备 */
    ulRet = SKF_ConnectDev((CHAR *)pucTokenName, &hDev);
    if (CKR_OK != ulRet)
    {
        goto FreeTokenInfo;
    }

	//if (TRUE != ExterAuth(hDev, NULL))
	//{
	//	goto FreeTokenInfo;
	//}

    /* 获取设备信息 */
    ulRet = SKF_GetDevInfo(hDev, &DevInfo);
    if (CKR_OK != ulRet)
    {
        goto FreeTokenInfo;
    }

    strcpy((char*)pstTokenInfo->label, DevInfo.Label);
    strcpy((char*)pstTokenInfo->serialNumber, (char*)DevInfo.Label);
    pstTokenInfo->ulMinPinLen = ICA_TOK_MIN_PIN_LEN;
    pstTokenInfo->ulMaxPinLen = ICA_TOK_MAX_PIN_LEN;
    pstTokenInfo->ulMaxSessionCount = ICA_TOK_MAX_SESSION_COUNT;
    pstTokenInfo->ulMaxRwSessionCount = ICA_TOK_MAX_RW_SESSION_COUNT;

    pstSlotData->pDevHandle = hDev;

    pstSlotInfo->flags = CKF_TOKEN_PRESENT;

    pstTokenData = (CK_I_TOKEN_DATA_PTR)ICA_calloc(1,sizeof(CK_I_TOKEN_DATA));
    if (NULL == pstTokenData)
    {
        goto FreeTokenInfo;
    }

    pstTokenData->token_info = pstTokenInfo;
    pstSlotData->token_data = pstTokenData;
    *ppSlotData = pstSlotData;

    FUNC_END;
    return CKR_OK;

FreeTokenInfo:
    ICA_free(pstTokenInfo);
FreeSectionName:
    ICA_free(pstSlotData->config_section_name);
FreeSlotData:
    ICA_free(pstSlotData);
FreeSlotInfo:
    ICA_free(pstSlotInfo);
Err:
    FUNC_END;
    return ulRet;
}

ULONG ICA_TOK_FinalizeToken(CK_I_SLOT_DATA_PTR pstSlotData)
{
    if (NULL != pstSlotData)
    {
        if (NULL != pstSlotData->config_section_name)
        {
            ICA_free(pstSlotData->config_section_name);
        }

        if (NULL != pstSlotData->pDevHandle)
        {
            (void)SKF_DisConnectDev(pstSlotData->pDevHandle);
			pstSlotData->pDevHandle = NULL;
        }

        if (NULL != pstSlotData->slot_info)
        {
            ICA_free(pstSlotData->slot_info);
        }

        if (NULL != pstSlotData->token_data)
        {
            if (NULL != pstSlotData->token_data->token_info)
            {
                ICA_free(pstSlotData->token_data->token_info);
            }

            ICA_free(pstSlotData->token_data);
        }

        ICA_free(pstSlotData);
    }

	return CKR_OK;
}


ULONG ICA_TOK_InitToken(CK_I_SLOT_DATA_PTR SlotData,
    UCHAR *pucPin, ULONG ulPinLen, UCHAR *pucLabel)
{
    ULONG ulRet = SAR_OK;
    CK_TOKEN_INFO_PTR pstTokenInfo = NULL;
    DEVHANDLE hDev = NULL;
    CHAR acApp[PATH_MAX] = {0};
    CHAR acAppList[PATH_MAX] = {0};
    CHAR acName[PATH_MAX] = {0};
    CHAR *pcAppNameList = NULL;
    CHAR *pcRemainder = NULL;
    CHAR *pcAppName = NULL;
    ULONG ulSize = PATH_MAX;
    HAPPLICATION hApp = NULL;
    HCONTAINER hContainer = NULL;
    ULONG ulRetryCount = 0;

    /* 参数检查 */
    if ((NULL == SlotData)
        || (NULL == SlotData->token_data)
        || (NULL == pucPin)
        || (NULL == pucLabel))
    {
        return CKR_GENERAL_ERROR;
    }

    /* 检查令牌 */
    pstTokenInfo = SlotData->token_data->token_info;
    if (NULL == pstTokenInfo)
    {
        return CKR_GENERAL_ERROR;
    }

    /* 检查PIN */
    if ((ulPinLen < pstTokenInfo->ulMinPinLen)
        || (ulPinLen > pstTokenInfo->ulMaxPinLen))
    {
        return CKR_GENERAL_ERROR;
    }

    hDev = SlotData->pDevHandle;

	ulRet = SKF_SetLabel(hDev, (LPSTR)pucLabel);
	if (CKR_OK != ulRet)
	{
		goto End;
	}

#if 0
    /* 枚举应用 */
    ulRet = SKF_EnumApplication(hDev, acApp, &ulSize);
    if(ulRet != CKR_OK)
    {
        return CKR_GENERAL_ERROR;
    }

	if (*acApp != '\0')
	{
		ICA_StrFmtList(acAppList, acApp);

		pcAppNameList = ICA_malloc(strlen((char *)acAppList) + 1);
		strcpy(pcAppNameList, acAppList);

		/* 删除所有应用 */
		pcAppName = TC_strtok_r((char *)pcAppNameList, ",", &pcRemainder);
		while(pcAppName != NULL)
		{
			/* 打开应用 */
			ulRet = SKF_OpenApplication(hDev, pcAppName, &hApp);
            if(ulRet != CKR_OK)
			{
				goto FreeAppNameList;
			}

            /* 校验PIN码 */
            ulRet = SKF_VerifyPIN(hApp, KT_USERPIN,
                pucPin, &ulRetryCount);
            if (CKR_OK != ulRet)
            {
                goto FreeAppNameList;
            }

            /* 枚举容器 */
            ulRet = SKF_EnumContainer(hApp, acName, &ulSize);
            if(ulRet != CKR_OK)
            {
                goto FreeAppNameList;
            }

            ulRet = SKF_DeleteContainer(hApp, acName, &hContainer);
            if(ulRet != CKR_OK)
            {
                goto FreeAppNameList;
            }

			/* 删除指定应用 */
			ulRet = SKF_DeleteApplication(hDev, pcAppName);
			if(ulRet != CKR_OK)
			{
				goto FreeAppNameList;
			}

			pcAppName = TC_strtok_r(NULL, ",", &pcRemainder);
		}
	}
#endif

	/* 枚举应用 */
	ulRet = SKF_EnumApplication(hDev, acApp, &ulSize);
	if(ulRet != CKR_OK)
	{
		return CKR_GENERAL_ERROR;
	}

	if (acApp[0] == '\0')
	{
		/* 创建应用 */
		ulRet = SKF_CreateApplication(hDev, (LPSTR)pucLabel,
			(LPSTR)pucPin, ICA_TOK_SO_PIN_RETRY_COUNT,
			(LPSTR)pucPin, ICA_TOK_USER_PIN_RETRY_COUNT,
			SECURE_ANYONE_ACCOUNT, &hApp);
		if(ulRet != CKR_OK)
		{
			goto FreeAppNameList;
		}
	}
	else
	{
		/* 打开应用 */
		ulRet = SKF_OpenApplication(hDev, pucLabel, &hApp);
		if(ulRet != CKR_OK)
		{
			goto FreeAppNameList;
		}
	}

    (void)SKF_CloseApplication(hApp);
	hApp = NULL;

FreeAppNameList:
    ICA_free(pcAppNameList);
End:
    return ulRet;
}

ULONG ICA_TOK_OpenSession(CK_I_SESSION_DATA_PTR pstSessionData)
{
    ULONG ulRet = CKR_OK;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    CHAR acName[PATH_MAX] = {0};
    ULONG ulSize = PATH_MAX;
    CHAR acContainerList[PATH_MAX] = {0};
    CHAR *pcList = NULL;
    CHAR *pcContainerName = NULL;
    CHAR *pcRemainder = NULL;
    DEVHANDLE hDev = NULL;
    HAPPLICATION hApp = NULL;
    HCONTAINER hContainer = NULL;
    ULONG ulContainerCount = 0;

    /* 参数检查 */
    if (NULL == pstSessionData)
    {
        return CKR_GENERAL_ERROR;
    }

    pstSlotData = pstSessionData->slot_data;
    if (NULL == pstSlotData)
    {
        return CKR_GENERAL_ERROR;
    }

    hDev = pstSlotData->pDevHandle;

    /* 枚举应用 */
    ulRet = SKF_EnumApplication(hDev, acName, &ulSize);
    if(ulRet != CKR_OK)
    {
        return CKR_GENERAL_ERROR;
    }

    if (acName[0] == '\0')
    {
        return CKR_GENERAL_ERROR;
    }

    ulRet = ICA_CheckName((CHAR *)acName, (CHAR *)pstSlotData->config_section_name);
    if(ulRet != CKR_OK)
    {
        return CKR_GENERAL_ERROR;
    }

    /* 假设只有一个应用,打开应用 */
    ulRet = SKF_OpenApplication(hDev, acName, &hApp);
    if(ulRet != CKR_OK)
    {
        return CKR_GENERAL_ERROR;
    }

	pstSessionData->AppHandle = hApp;

    /* 枚举容器 */
    ulRet = SKF_EnumContainer(hApp, acName, &ulSize);
    if(ulRet != CKR_OK)
    {
        goto End;
    }

    if (acName[0] == '\0')
    {
        return CKR_OK;
    }

    ICA_StrFmtList(acContainerList, acName);

    pcList = (CHAR *)ICA_malloc(strlen((char *)acContainerList) + 1);
	strcpy(pcList, acContainerList);

    /* 打开所有容器 */
    pstSessionData->CtrCount = 0;
    pcContainerName = TC_strtok_r(pcList, ",", &pcRemainder);
    while(pcContainerName != NULL)
    {
        /* 打开指定容器 */
        ulRet = SKF_OpenContainer(hApp, pcContainerName, &hContainer);
        if(ulRet != CKR_OK)
        {
            goto FreeList;
        }

        /* 假设容器内存在公钥 */
        ulRet = ICA_CreatePubKeyObj(pstSessionData, pcContainerName, hContainer);
        if(ulRet == CKR_OK)
        {
			ulContainerCount = pstSessionData->CtrCount;
			pstSessionData->aCtrHandle[ulContainerCount] = hContainer;
			pstSessionData->CtrCount++;

			if(strcmp(pcContainerName, pstSlotData->config_section_name) == 0)
			{
				pstSessionData->MainCtrHandle = hContainer;
			}
        }

        pcContainerName = TC_strtok_r(NULL, ",", &pcRemainder);
    }

    ulRet = ICA_CopyObject(pstSessionData->object_list,
        pstSlotData->token_data->object_list);
    if(ulRet != CKR_OK)
    {
        return CKR_GENERAL_ERROR;
    }

FreeList:
    ICA_free(pcList);
End:
    return ulRet;
}

ULONG ICA_TOK_CloseSession(CK_I_SESSION_DATA_PTR pstSessionData)
{
    ULONG ulRet = CKR_OK;
    ULONG ulLen = ICA_TOK_ONCE_RW_LEN;
    UCHAR pucBuff[ICA_TOK_ONCE_RW_LEN] = {0};
    HCONTAINER hContainer = NULL;
    int i;

    if(NULL != pstSessionData->encrypt_state)
    {
        ulRet = ICA_TOK_EncryptFinal(pstSessionData, pucBuff, &ulLen);
        if(ulRet != CKR_OK)
        {
            return ulRet;
        }
    }

    if(NULL != pstSessionData->decrypt_state)
    {
        ulRet = ICA_TOK_DecryptFinal(pstSessionData, pucBuff, &ulLen);
        if(ulRet != CKR_OK)
        {
            return ulRet;
        }
    }

    for (i = 0; i < pstSessionData->CtrCount; i++)
    {
        hContainer = (HCONTAINER)pstSessionData->aCtrHandle[i];
        (void)SKF_CloseContainer(hContainer);
		pstSessionData->aCtrHandle[i] = NULL;
    }

    /* 关闭应用 */
    ulRet = SKF_CloseApplication(pstSessionData->AppHandle);
    if (CKR_OK != ulRet)
    {
        return ulRet;
    }

    return CKR_OK;
}

ULONG ICA_TOK_Login(CK_I_SESSION_DATA_PTR pstSessionData,
    ULONG ulUserType, UCHAR *pucPin, ULONG ulPinLen)
{
    ULONG ulRet = CKR_OK;
	CHAR acName[PATH_MAX] = {0};
	ULONG ulSize = PATH_MAX;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    ULONG ulPINType = 0;
    ULONG ulRetryCount = 0;

    FUNC_BEGIN;

    /* 参数检查 */
    if ((NULL == pstSessionData) || (NULL == pucPin))
    {
        return CKR_GENERAL_ERROR;
    }

    pstSlotData = pstSessionData->slot_data;
    if (NULL == pstSlotData)
    {
        ICA_LogE("pstSlotData is null!");
        return CKR_GENERAL_ERROR;
    }

    /* 转换登录类型 */
    if (CKU_SO == ulUserType)
    {
        ulPINType = KT_SOPIN;
    }
    else
    {
        ulPINType = KT_USERPIN;
    }

    /* 校验PIN码 */
    ulRet = SKF_VerifyPIN(pstSessionData->AppHandle, ulPINType,
        (CHAR *)pucPin, &ulRetryCount);
    if (CKR_OK != ulRet)
    {
        /* 检查PIN码是否已经锁死 */
        if (ulRetryCount == 0)
        {
            return CKR_PIN_LOCKED;
        }

        return ulRet;
    }

	/* 枚举容器 */
	ulRet = SKF_EnumContainer(pstSessionData->AppHandle, acName, &ulSize);
	if(ulRet != CKR_OK)
	{
		return ulRet;
	}

	if (acName[0] == '\0')
	{
		return CKR_OK;
	}

    ulRet = ICA_CreatePrivKeyObj(pstSessionData,
        (CHAR *)pstSlotData->token_data->token_info->label, (HCONTAINER)pstSessionData->MainCtrHandle);
    if(ulRet != CKR_OK)
    {
        return CKR_GENERAL_ERROR;
    }

    ulRet = ICA_CopyObject(pstSessionData->object_list,
        pstSlotData->token_data->object_list);
    if(ulRet != CKR_OK)
    {
        return CKR_GENERAL_ERROR;
    }

    return ulRet;
}

ULONG ICA_TOK_Logout(CK_I_SESSION_DATA_PTR pstSessionData)
{
    ULONG ulRet = CKR_OK;

    /* 参数检查 */
    if (NULL == pstSessionData)
    {
        return CKR_GENERAL_ERROR;
    }

    /* 清除应用当前的安全状态 */
    ulRet = SKF_ClearSecureState(pstSessionData->AppHandle);
    if (CKR_OK != ulRet)
    {
        ulRet = CKR_GENERAL_ERROR;
    }

    return ulRet;
}

ULONG ICA_TOK_InitPIN(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucPin, ULONG ulPinLen)
{
    CK_RV ulRet = CKR_OK;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;

    /* 参数检查 */
    if ((NULL == pstSessionData) || (NULL == pucPin))
    {
        return CKR_GENERAL_ERROR;
    }

    /* 不支持,直接返回OK */
    return ulRet;
}

ULONG ICA_TOK_SetPIN(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucOldPin, ULONG ulOldLen, UCHAR *pucNewPin, ULONG ulNewLen)
{
    CK_RV ulRet = CKR_OK;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    ULONG ulRetryCount = 0;

    if ((NULL == pstSessionData) || (NULL == pucOldPin) || (NULL == pucNewPin))
    {
        return CKR_GENERAL_ERROR;
    }

    /* 修改用户PIN的值 */
    ulRet = SKF_ChangePIN (pstSessionData->AppHandle, KT_USERPIN,
        (CHAR *)pucOldPin, (CHAR *)pucNewPin, &ulRetryCount);
    if (CKR_OK != ulRet)
    {
        /* 检查PIN码是否已经锁死 */
        if (ulRetryCount == 0)
        {
            return CKR_PIN_LOCKED;
        }

        return ulRet;
    }

    return ulRet;
}

ULONG ICA_TOK_CreateObject(CK_I_SESSION_DATA_PTR pstSessionData,
	CK_I_OBJ_PTR pstObj)
{
    ULONG ulRet = CKR_OK;
    CK_CHAR  aucLabel[PATH_MAX] = {0};
    CK_ATTRIBUTE_PTR pstTemplate = NULL;
    HCONTAINER hContainer = NULL;
    RSAPUBLICKEYBLOB stRSAPubBlob = {0};
    ULONG ulBlobLen = 0;
    HANDLE hKey = NULL;
    ULONG ulContainerCount = 0;

    /* 参数检查 */
    if (NULL == pstSessionData || NULL == pstObj)
    {
        return CKR_GENERAL_ERROR;
    }

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstObj, CK_IA_ID));
    if (NULL == pstTemplate)
    {
        return CKR_OK;
    }

    strncpy((char*)aucLabel, (char*)pstTemplate->pValue, pstTemplate->ulValueLen);

    /* 创建容器 */
    ulRet = SKF_CreateContainer(pstSessionData->AppHandle, (CHAR *)aucLabel, &hContainer);
	if (SAR_COS_FILE_ALREADY_EXIST == ulRet)
	{
		ulRet = SKF_OpenContainer(pstSessionData->AppHandle, (CHAR *)aucLabel, &hContainer);
	}
    if (CKR_OK != ulRet)
    {
        return ulRet;
    }

    if(*((CK_OBJECT_CLASS CK_PTR)CI_ObjLookup(pstObj,CK_IA_CLASS)->pValue) == CKO_PUBLIC_KEY)
    {
        //stRSAPubBlob.AlgID = SGD_RSA;
    }

    stRSAPubBlob.BitLen = *((CK_ULONG_PTR)CI_ObjLookup(pstObj, CK_IA_MODULUS_BITS)->pValue);

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstObj, CK_IA_MODULUS));
    memcpy(stRSAPubBlob.Modulus, pstTemplate->pValue, pstTemplate->ulValueLen);

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstObj, CK_IA_PUBLIC_EXPONENT));
    memcpy(stRSAPubBlob.PublicExponent, pstTemplate->pValue, pstTemplate->ulValueLen);

    ulBlobLen = sizeof(stRSAPubBlob);
    ulRet = SKF_ImportPublicKey(hContainer, FALSE, &stRSAPubBlob, &ulBlobLen, &hKey);
    if (CKR_OK != ulRet)
    {
        return ulRet;
    }

    pstObj->ObjHandle = hKey;

    ulContainerCount = pstSessionData->CtrCount;
    pstSessionData->aCtrHandle[ulContainerCount] = hContainer;
    pstSessionData->CtrCount++;

    return ulRet;
}

ULONG ICA_TOK_GenerateKey(CK_I_SESSION_DATA_PTR pstSessionData,
    CK_MECHANISM_PTR pstMechanism, CK_I_OBJ_PTR pstKeyObj)
{
    ULONG ulRet = CKR_OK;
	ULONG ulRsaPubKeyBlobLen = 0;
	ULONG ulSessionKebBlobLen = 256;
    ULONG ulAlgId = 0;
	BYTE btSessionKey[256] = {0}; 
	RSAPUBLICKEYBLOB stRSAPubBlob = {0};
    HANDLE hSessionKey = NULL;
    HCONTAINER hContainer = NULL;

    /* 参数检查 */
    if ((NULL == pstSessionData)
        || (NULL == pstMechanism)
        || (NULL == pstKeyObj))
    {
        return CKR_GENERAL_ERROR;
    }

	ulAlgId = SGD_AES128_ECB;

	/* 导出公钥 */
	ulRsaPubKeyBlobLen = sizeof(RSAPUBLICKEYBLOB);
	ulRet = SKF_ExportPublicKey((HCONTAINER)pstSessionData->MainCtrHandle,
		FALSE, &stRSAPubBlob, &ulRsaPubKeyBlobLen);
    if (CKR_OK != ulRet)
    {
        goto End;
    }

    /* 生成会话密钥 */
	SKF_RSAExportSessionKey((HCONTAINER)pstSessionData->MainCtrHandle, ulAlgId,
		&stRSAPubBlob, btSessionKey, &ulSessionKebBlobLen, &hSessionKey);
    if (CKR_OK != ulRet)
    {
        goto End;
    }

    pstKeyObj->ObjHandle = hSessionKey;

End:
    return ulRet;
}


ULONG ICA_TOK_GenerateKeyPair(CK_I_SESSION_DATA_PTR pstSessionData,
    CK_MECHANISM_PTR pstMechanism,
    CK_I_OBJ_PTR pstPublicKeyObj, CK_I_OBJ_PTR pstPrivateKeyObj)
{
    CK_RV ulRet = CKR_OK;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    CK_ATTRIBUTE_PTR pstTemplate = NULL;
    CK_CHAR  aucLabel[PATH_MAX] = {0};
    HCONTAINER hContainer = NULL;
    CK_ULONG_PTR pulModulusBits = 0;
    RSAPUBLICKEYBLOB stBlob = {0};
    ULONG ulContainerCount = 0;
    CHAR acName[PATH_MAX] = {0};
    ULONG ulSize = PATH_MAX;

    /* 参数检查 */
    if ((NULL == pstSessionData)
        || (NULL == pstMechanism)
        || (NULL == pstPublicKeyObj)
        || (NULL == pstPrivateKeyObj))
    {
        return CKR_GENERAL_ERROR;
    }

    pstSlotData = pstSessionData->slot_data;
    if ((NULL == pstSlotData)
        || (NULL == pstSlotData->token_data)
        || (NULL == pstSlotData->token_data->token_info))
    {
        return CKR_GENERAL_ERROR;
    }

    strcpy((char*)aucLabel, (char*)pstSlotData->token_data->token_info->serialNumber);

    /* 枚举容器 */
    ulRet = SKF_EnumContainer(pstSessionData->AppHandle, acName, &ulSize);
    if(ulRet != CKR_OK)
    {
        return CKR_GENERAL_ERROR;
    }

    if (acName[0] == '\0')
    {
        /* 创建容器 */
        ulRet = SKF_CreateContainer(pstSessionData->AppHandle, (CHAR *)aucLabel, &hContainer);
        if (CKR_OK != ulRet)
        {
            return ulRet;
        }

        pstSessionData->MainCtrHandle = hContainer;

        ulContainerCount = pstSessionData->CtrCount;
        pstSessionData->aCtrHandle[ulContainerCount] = hContainer;
        pstSessionData->CtrCount++;
    }
    else
    {
        ulRet = SKF_OpenContainer(pstSessionData->AppHandle, acName, &hContainer);
        if(ulRet != CKR_OK)
        {
            return ulRet;
        }
    }

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPublicKeyObj, CK_IA_MODULUS_BITS));
    pulModulusBits = (CK_ULONG_PTR)(pstTemplate->pValue);

    /* 生成RSA密钥对 */
    ulRet = SKF_GenRSAKeyPair(hContainer, *pulModulusBits, &stBlob);
    if (CKR_OK != ulRet)
    {
        return ulRet;
    }

    return ulRet;
}

ULONG ICA_TOK_WrapKey(CK_I_SESSION_DATA_PTR pstSessionData,
    CK_MECHANISM_PTR pstMechanism,
    CK_I_OBJ_PTR pstWrapKeyObj, CK_I_OBJ_PTR pstKeyObj,
    CK_BYTE_PTR pucWrappedKey, CK_ULONG_PTR pulWrappedKeyLen)
{
    CK_RV ulRet = CKR_OK;
	RSAPUBLICKEYBLOB stRSAPubBlob = {0};
	ULONG ulBlobLen = 0;
	BYTE EncSessionkey[128] = {0};
	HANDLE pSessionKey = NULL;
	ULONG ulRetLen = 0;

    /* 参数检查 */
    if ((NULL == pstSessionData)
        || (NULL == pstMechanism)
        || (NULL == pstWrapKeyObj)
        || (NULL == pstKeyObj)
        || (NULL == pucWrappedKey))
    {
        return CKR_GENERAL_ERROR;
    }

	ulBlobLen = sizeof(stRSAPubBlob);
	ulRet = SKF_ExportPublicKey((HCONTAINER)pstWrapKeyObj->CtrHandle, FALSE, (BYTE *)&stRSAPubBlob, &ulBlobLen);
	if (ulRet != CKR_OK)
	{
		goto End;
	}

	ulRet = SKF_RSAExportSessionKey((HCONTAINER)pstSessionData->MainCtrHandle, SGD_AES128_ECB,
		&stRSAPubBlob, pucWrappedKey, pulWrappedKeyLen, (void ** )pstKeyObj->ObjHandle);
	if (ulRet!=CKR_OK)
	{
		goto End;
	}


#if 0
    /* 导出会话密钥 */
    ulRet = SKF_ExportSessionKey(pstSessionData->MainCtrHandle, pstKeyObj->ObjHandle,
        pstWrapKeyObj->ObjHandle, pucWrappedKey, pulWrappedKeyLen);
    if (CKR_OK != ulRet)
    {
        goto End;
    }
#endif
End:
    return ulRet;
}

ULONG ICA_TOK_UnwrapKey(CK_I_SESSION_DATA_PTR  pstSessionData,
    CK_MECHANISM_PTR pstMechanism, CK_I_OBJ_PTR pstUnwrapKeyObj,
    CK_BYTE_PTR pucWrappedKey, ULONG ulWrappedKeyLen,
    CK_I_OBJ_PTR pstKeyObj)
{
    ULONG ulRet = CKR_OK;
    HCONTAINER hContainer = NULL;
    ULONG ulAlgId = 0;
    HANDLE hKey = NULL;

    /* 参数检查 */
    if ((NULL == pstSessionData)
        || (NULL == pstMechanism)
        || (NULL == pstUnwrapKeyObj)
        || (NULL == pucWrappedKey)
        || (NULL == pstKeyObj))
    {
        return CKR_GENERAL_ERROR;
    }

    /* 导入会话密钥 */
    ulRet = SKF_ImportSessionKey((HCONTAINER)pstSessionData->MainCtrHandle, SGD_AES128_ECB,
        pucWrappedKey, ulWrappedKeyLen, &hKey);
    if (CKR_OK != ulRet)
    {
        goto End;
    }

    pstKeyObj->ObjHandle = hKey;

End:
    return ulRet;
}

ULONG ICA_TOK_EncryptInit(CK_I_SESSION_DATA_PTR pstSessionData,
    CK_MECHANISM_PTR pstMechanism, CK_I_OBJ_PTR pstKeyObj)
{
    ULONG ulRet = CKR_OK;
    BLOCKCIPHERPARAM stEncryptParam = {0};

    /* 参数检查 */
    if ((NULL == pstSessionData)
        || (NULL == pstMechanism)
        || (NULL == pstKeyObj))
    {
        return CKR_GENERAL_ERROR;
    }

    /* 数据加密初始化,设置数据加密的算法相关参数 */
    ulRet = SKF_EncryptInit(pstKeyObj->ObjHandle, stEncryptParam);
    if (CKR_OK != ulRet)
    {
        goto End;
    }

    pstSessionData->encrypt_state = pstKeyObj->ObjHandle;

End:
    return ulRet;
}

ULONG ICA_TOK_EncryptUpdate(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucPart, ULONG ulPartLen,
    UCHAR *pucEncryptedPart, ULONG *pulEncryptedPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;

    /* 参数检查 */
    if ((NULL == pstSessionData)
        || (NULL == pucPart)
        || (NULL == pulEncryptedPartLen))
    {
        return CKR_GENERAL_ERROR;
    }

    /* 如果为NULL,返回加密结果的长度 */
	if (NULL == pucEncryptedPart)
	{
		*pulEncryptedPartLen = ICA_TOK_ONCE_RW_LEN;
		return CKR_OK;
	}

    /* 得到加密句柄 */
    hKey = pstSessionData->encrypt_state;
    if (NULL == hKey)
    {
        return CKR_GENERAL_ERROR;
    }

    /* 多组数据加密 */
    ulRet = SKF_EncryptUpdate(hKey, pucPart, ulPartLen,
        pucEncryptedPart, pulEncryptedPartLen);

    return ulRet;
}


ULONG ICA_TOK_EncryptFinal(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucLastEncryptedPart, ULONG *pulLastEncryptedPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;

    /* 参数检查 */
    if ((NULL == pstSessionData) || (NULL == pucLastEncryptedPart))
    {
        return CKR_GENERAL_ERROR;
    }

    /* 得到加密句柄 */
    hKey = pstSessionData->encrypt_state;
    if (NULL == hKey)
    {
        return CKR_GENERAL_ERROR;
    }

    /* 结束多个分组数据的加密,返回剩余加密结果 */
    ulRet = SKF_EncryptFinal(hKey, pucLastEncryptedPart,
        pulLastEncryptedPartLen);

    (void)SKF_CloseHandle(hKey);
    pstSessionData->encrypt_state = NULL;

End:
    return ulRet;
}

ULONG ICA_TOK_DecryptInit(CK_I_SESSION_DATA_PTR pstSessionData,
    CK_MECHANISM_PTR  pstMechanism, CK_I_OBJ_PTR pstKeyObj)
{
    ULONG ulRet = CKR_OK;
    BLOCKCIPHERPARAM stDecryptParam = {0};

    /* 参数检查 */
    if ((NULL == pstSessionData)
        || (NULL == pstMechanism)
        || (NULL == pstKeyObj))
    {
        return CKR_GENERAL_ERROR;
    }

    ulRet = SKF_DecryptInit(pstKeyObj->ObjHandle, stDecryptParam);
    if (CKR_OK != ulRet)
    {
        goto End;
    }

    pstSessionData->decrypt_state = pstKeyObj->ObjHandle;

End:
    return ulRet;

}

ULONG ICA_TOK_DecryptUpdate(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucEncryptedPart, ULONG ulEncryptedPartLen,
    UCHAR *pucPart, ULONG *pulPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;
    UCHAR *pulHDecrypt = NULL;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;

    /* 参数检查 */
    if ((NULL == pstSessionData)
        || (NULL == pucEncryptedPart)
        || (NULL == pulPartLen))
    {
        return CKR_GENERAL_ERROR;
    }

    /* 如果为NULL,返回解密结果的长度 */
    if (NULL == pucPart)
	{
		*pulPartLen = ICA_TOK_ONCE_RW_LEN;
		return CKR_OK;
	}

    /* 得到加密句柄 */
    hKey = pstSessionData->decrypt_state;
    if (NULL == hKey)
    {
        return CKR_GENERAL_ERROR;
    }

    /* 多组数据解密 */
    ulRet = SKF_DecryptUpdate(hKey, pucEncryptedPart,
        ulEncryptedPartLen, pucPart, pulPartLen);

    return ulRet;
}

ULONG ICA_TOK_DecryptFinal(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucLastPart, ULONG *pulLastPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;
    ULONG *pulHDecrypt = NULL;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;

    if ((NULL == pstSessionData) || (NULL == pucLastPart))
    {
        return CKR_GENERAL_ERROR;
    }

    /* 得到加密句柄 */
    hKey = pstSessionData->decrypt_state;
    if (NULL == hKey)
    {
        return CKR_GENERAL_ERROR;
    }

    /* 结束多个分组数据的解密,返回剩余解密结果 */
    ulRet = SKF_DecryptFinal(hKey, pucLastPart, pulLastPartLen);

    (void)SKF_CloseHandle(hKey);
    pstSessionData->decrypt_state = NULL;

    return ulRet;
}


