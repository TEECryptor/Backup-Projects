/***************************************************************************
 *	File name:token.c
 *	Introduce:The implement file for Syno TF key functions
 *	Author:Syno common
 *	Date:2015/11/05
 *  Version:v1.0
 **************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

#include "token.h"
#include "slot.h"
#include "template.h"
#include "objects.h"
#include "utils.h"
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
CK_OBJECT_CLASS	  s_ulDataClass = CKO_DATA;
CK_OBJECT_CLASS   s_ulPubClass = CKO_PUBLIC_KEY;
CK_OBJECT_CLASS   s_ulPrivClass = CKO_PRIVATE_KEY;
CK_OBJECT_CLASS   s_ulCertClass = CKO_PRIVATE_KEY;
CK_KEY_TYPE       s_ulRSAKeyType = CKK_RSA;
CK_CHAR           s_aucDataObjLabel[] = "A data file object";
CK_CHAR           s_aucPublicKeyObjLabel[] = "A public key object";
CK_CHAR           s_aucPrivateKeyObjLabel[] = "A private key object";
CK_CHAR           s_aucCertObjLabel[] = "A certificate object";
CK_ULONG          s_ulModulusBits = 1024;
CK_BYTE           s_aucPublicExponent[4] = {0, 1, 0, 1};

/* Data File对象属性模板 */
static CK_ATTRIBUTE s_DataObjTemplate[ 7 ] = {
	{ CKA_CLASS, &s_ulDataClass, sizeof(int) },						//对象类别
	{ CKA_TOKEN, (unsigned char *)&s_ucTrue, sizeof(char) },		//是Token对象
	{ CKA_PRIVATE, (unsigned char *)&s_ucFalse, sizeof(char) },		//是公有对象
	{ CKA_LABEL, s_aucDataObjLabel, sizeof(s_aucDataObjLabel) },	//对象标示符
	{ CKA_APPLICATION, NULL, 0 },									//所在应用名
	{ CKA_ID, NULL, 0 },											//文件名 
	{ CKA_VALUE, NULL, 0 }											//文件内容
};

/* 公钥对象模板 */
static CK_ATTRIBUTE s_PublicKeyObjTemplate[ 12 ] = {
	{ CKA_CLASS, &s_ulPubClass, sizeof(s_ulPubClass) },						//对象类别
	{ CKA_TOKEN, &s_ucTrue, sizeof(s_ucTrue) },								//是Token对象
	{ CKA_PRIVATE, (unsigned char *)&s_ucFalse, sizeof(char) },				//是公有对象
	{ CKA_KEY_TYPE, &s_ulRSAKeyType, sizeof(s_ulRSAKeyType) },				//密钥算法类型
	{ CKA_LABEL, s_aucPublicKeyObjLabel, sizeof(s_aucPublicKeyObjLabel) },	//对象标示符
	{ CKA_ID, NULL, 0 },													//密钥所在容器名
	{ CKA_VERIFY, &s_ucTrue, sizeof(s_ucTrue) },							//是否可以用来验证签名
	{ CKA_ENCRYPT, &s_ucTrue, sizeof(s_ucTrue) },							//是否可以用来加密数据
	{ CKA_WRAP, &s_ucTrue, sizeof(s_ucTrue) },								//是否可以用来Wrap Key
	{ CKA_MODULUS, NULL_PTR, 0 },											//模数n
	{ CKA_MODULUS_BITS, &s_ulModulusBits, sizeof(s_ulModulusBits) },		//模数n的位长度
	{ CKA_PUBLIC_EXPONENT, s_aucPublicExponent, sizeof(s_aucPublicExponent) }//公用指数e
};

/* 私钥对象模板 */
CK_ATTRIBUTE s_PrivateKeyObjTemplate[ 11 ] = {
    { CKA_CLASS, &s_ulPrivClass, sizeof(s_ulPrivClass) },					//对象类别
	{ CKA_TOKEN, &s_ucTrue, sizeof(s_ucTrue) },								//是Token对象
	{ CKA_PRIVATE, (unsigned char *)&s_ucTrue, sizeof(char) },				//是私有对象
    { CKA_KEY_TYPE, &s_ulRSAKeyType, sizeof(s_ulRSAKeyType) },				//密钥算法类型
    { CKA_LABEL, s_aucPrivateKeyObjLabel, sizeof(s_aucPrivateKeyObjLabel) },//对象标示符
    { CKA_ID, NULL, 0 },													//密钥所在容器名
	{ CKA_SENSITIVE, &s_ucFalse, sizeof(s_ucFalse) },						//是否是敏感数据
	{ CKA_SIGN, &s_ucTrue, sizeof(s_ucTrue) },								//是否可以用来签名
	{ CKA_DECRYPT, &s_ucTrue, sizeof(s_ucTrue) },							//是否可以用来解密数据
	{ CKA_UNWRAP, &s_ucTrue, sizeof(s_ucTrue) },							//是否可以用来Unwrap Key
    { CKA_EXTRACTABLE, &s_ucTrue, sizeof(s_ucTrue) },						//是否可以导出(无论该属性为何值，私钥数据都不能导出)
};

/* 证书对象模板 */
CK_ATTRIBUTE s_CertObjTemplate[ 13 ] = {
	{ CKA_CLASS, &s_ulPrivClass, sizeof(s_ulCertClass) },					//对象类别
	{ CKA_TOKEN, &s_ucTrue, sizeof(s_ucTrue) },								//是Token对象
	{ CKA_PRIVATE, (unsigned char *)&s_ucFalse, sizeof(char) },				//是私有对象
    { CKA_KEY_TYPE, &s_ulRSAKeyType, sizeof(s_ulRSAKeyType) },				//密钥算法类型
	{ CKA_LABEL, s_aucCertObjLabel, sizeof(s_aucCertObjLabel) },			//对象标示符
    { CKA_ID, NULL, 0 },													//密钥所在容器名
	{ CKA_SIGN, &s_ucTrue, sizeof(s_ucTrue) },								//是否是签名证书
	{ CKA_VERIFY, &s_ucTrue, sizeof(s_ucTrue) },							//是否是签名证书
	{ CKA_ENCRYPT, &s_ucTrue, sizeof(s_ucTrue) },							//是否是加密证书
	{ CKA_VALUE, NULL, 0 },													//证书内容
	{ CKA_SUBJECT, NULL, 0 },												//证书主题
	{ CKA_ISSUER, NULL, 0 },												//证书颁发者
	{ CKA_SERIAL_NUMBER, NULL, 0 }											//证书序列号
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
#define TOK_ONCE_RW_LEN             2048

#define TOK_MAX_SESSION_COUNT       8
#define TOK_MAX_RW_SESSION_COUNT    8
#define TOK_MIN_PIN_LEN             6
#define TOK_MAX_PIN_LEN             15

#define TOK_SO_PIN_RETRY_COUNT      10
#define TOK_USER_PIN_RETRY_COUNT    10

#define TOK_INIT_APPNAME			"SynoInitApp"
#define TOK_INIT_CONTAINER			"SynoRSACon"
#define	TOK_INIT_USERPIN			"111111"

static CK_I_TOKEN_METHODS s_ck_i_token_methods = {0};
static CK_MECHANISM_TYPE s_ck_i_token_mechanism_types[] = {CKM_RSA_PKCS_KEY_PAIR_GEN, CKM_RSA_PKCS, CKM_MD5_RSA_PKCS, CKM_SHA1_RSA_PKCS, CKM_SHA256_RSA_PKCS,
														   CKM_DES_KEY_GEN, CKM_DES_ECB, CKM_DES_CBC, CKM_DES3_KEY_GEN, CKM_DES3_ECB, CKM_DES3_CBC,
														   CKM_AES_KEY_GEN, CKM_AES_ECB, CKM_AES_CBC, CKM_MD5, CKM_SHA_1, CKM_SHA256};

/***********************************************************************
 *	名称:TOK_I_SetFunctionList
 *	说明:设置Token相关的函数地址。
 *	参数:无
 *	返回:无
 ***********************************************************************/
static CK_RV TOK_I_SetFunctionList()
{
	FUNC_BEGIN;

	s_ck_i_token_methods.GetTokenInfo = TOK_GetTokenInfo;
	s_ck_i_token_methods.GetMechanismList = TOK_GetMechanismList;
	s_ck_i_token_methods.GetMechanismInfo = TOK_GetMechanismInfo;
	s_ck_i_token_methods.InitToken = TOK_InitToken;
	s_ck_i_token_methods.FinalizeToken = TOK_FinalizeToken;
	s_ck_i_token_methods.InitPIN = TOK_InitPIN;
	s_ck_i_token_methods.SetPIN = TOK_SetPIN;
	s_ck_i_token_methods.OpenSession = TOK_OpenSession;
	s_ck_i_token_methods.CloseSession = TOK_CloseSession;
	s_ck_i_token_methods.GetOperationState = TOK_GetOperationState;
	s_ck_i_token_methods.SetOperationState = TOK_SetOperationState;
	s_ck_i_token_methods.Login = TOK_Login;
	s_ck_i_token_methods.Logout = TOK_Logout;
	s_ck_i_token_methods.EncryptInit = TOK_EncryptInit;
	s_ck_i_token_methods.Encrypt = TOK_Encrypt;
	s_ck_i_token_methods.EncryptUpdate = TOK_EncryptUpdate;
	s_ck_i_token_methods.EncryptFinal = TOK_EncryptFinal;
	s_ck_i_token_methods.DecryptInit = TOK_DecryptInit;
	s_ck_i_token_methods.Decrypt = TOK_Decrypt;
	s_ck_i_token_methods.DecryptUpdate = TOK_DecryptUpdate;
	s_ck_i_token_methods.DecryptFinal = TOK_DecryptFinal;
	s_ck_i_token_methods.DigestInit = TOK_DigestInit;
	s_ck_i_token_methods.Digest = TOK_Digest;
	s_ck_i_token_methods.DigestUpdate = TOK_DigestUpdate;
	s_ck_i_token_methods.DigestKey = TOK_DigestKey;
	s_ck_i_token_methods.DigestFinal = TOK_DigestFinal;
	s_ck_i_token_methods.SignInit = TOK_SignInit;
	s_ck_i_token_methods.Sign = TOK_Sign;
	s_ck_i_token_methods.SignUpdate = TOK_SignUpdate;
	s_ck_i_token_methods.SignFinal = TOK_SignFinal;
	s_ck_i_token_methods.SignRecoverInit = TOK_SignRecoverInit;
	s_ck_i_token_methods.SignRecover = TOK_SignRecover;
	s_ck_i_token_methods.VerifyInit = TOK_VerifyInit;
	s_ck_i_token_methods.Verify = TOK_Verify;
	s_ck_i_token_methods.VerifyUpdate = TOK_VerifyUpdate;
	s_ck_i_token_methods.VerifyFinal = TOK_VerifyFinal;
	s_ck_i_token_methods.VerifyRecoverInit = TOK_VerifyRecoverInit;
	s_ck_i_token_methods.VerifyRecover = TOK_VerifyRecover;
	s_ck_i_token_methods.DigestEncryptUpdate = TOK_DigestEncryptUpdate;
	s_ck_i_token_methods.DecryptDigestUpdate = TOK_DecryptDigestUpdate;
	s_ck_i_token_methods.SignEncryptUpdate = TOK_SignEncryptUpdate;
	s_ck_i_token_methods.DecryptVerifyUpdate = TOK_DecryptVerifyUpdate;
	s_ck_i_token_methods.GenerateKey = TOK_GenerateKey;
	s_ck_i_token_methods.GenerateKeyPair = TOK_GenerateKeyPair;
	s_ck_i_token_methods.WrapKey = TOK_WrapKey;
	s_ck_i_token_methods.UnwrapKey = TOK_UnwrapKey;
	s_ck_i_token_methods.DeriveKey = TOK_DeriveKey;
	s_ck_i_token_methods.SeedRandom = TOK_SeedRandom;
	s_ck_i_token_methods.GenerateRandom = TOK_GenerateRandom;
	s_ck_i_token_methods.GetFunctionStatus = TOK_GetFunctionStatus;
	s_ck_i_token_methods.CancelFunction = TOK_CancelFunction;
	s_ck_i_token_methods.WaitForSlotEvent = TOK_WaitForSlotEvent;
	s_ck_i_token_methods.TokenObjAdd = TOK_TokenObjAdd;
	s_ck_i_token_methods.TokenObjCommit = TOK_TokenObjCommit;
	s_ck_i_token_methods.TokenObjDelete = TOK_TokenObjDelete;

	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	名称:TOK_I_StrFmtList
 *	说明:将国密格式的字符串序列转化为用字符','分隔的字符串List
 *	参数:pcSrc:[IN]:传入的字符串序列
 *       pcDst:[OUT]:返回的字符串List，用','分隔。
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
void TOK_I_StrFmtList(CHAR *pcDst, CHAR *pcSrc)
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
/***********************************************************************
 *	名称:TOK_I_GenUniqeContainerId
 *	说明:使用随机数生成一个唯一的容器名
 *	参数:pPubKeyObj:[IN]:内部公钥对象指针
 *       pstuRsaPubKey:[OUT]:返回的国标公钥结构体指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static LPSTR TOK_I_GenUniqeContainerId()
{
	ULONG ulIndex = 0;
	CHAR csBaseChar[26] = {0};
	CHAR csRandom[10 + 1] = {0};
	const ULONG ulRandomLen = 10;
	static CHAR csResult[32] = {0};
	
	for (ulIndex = 0; ulIndex < 26; ulIndex++)
	{
		csBaseChar[ulIndex] = (CHAR)('A' + ulIndex);
	}

    srand(time(NULL));

    for (ulIndex = 0; ulIndex < ulRandomLen; ulIndex++)
    {
		csRandom[ulIndex] = csBaseChar[rand() % sizeof(csBaseChar)];
    }
	csRandom[ulRandomLen] = '\0';

	memset(csResult, 0, sizeof(csResult));
	strcpy(csResult, "P11_");
	strcat(csResult, csRandom);

	return csResult;
}
/***********************************************************************
 *	名称:TOK_I_ConvertPubKeyObj
 *	说明:将公钥对象转化为国标中的结构体形式
 *	参数:pPubKeyObj:[IN]:内部公钥对象指针
 *       pstuRsaPubKey:[OUT]:返回的国标公钥结构体指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static CK_RV TOK_I_ConvertPubKeyObj(CK_I_OBJ_PTR pPubKeyObj, 
									PRSAPUBLICKEYBLOB pstuRsaPubKey)
{
	ULONG ulRet = CKR_OK;
    CK_ATTRIBUTE_PTR pObjAttr = NULL;

	FUNC_BEGIN;

	// 参数检查
	if (NULL == pPubKeyObj)
	{
		SYNO_LogE("pPubKeyObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL == pstuRsaPubKey)
	{
		SYNO_LogE("pstuRsaPubKey is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	// 算法标记
	pstuRsaPubKey->AlgID = SGD_RSA;

	// 模Bits
	pObjAttr = CI_ObjLookup(pPubKeyObj, CK_IA_MODULUS_BITS);
	if (NULL == pObjAttr || NULL == pObjAttr->pValue)
	{
		SYNO_LogE("CK_IA_MODULUS_BITS attribyte is NULL!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	pstuRsaPubKey->BitLen = *((CK_ULONG_PTR)pObjAttr->pValue);

	// 模数据
	pObjAttr = CI_ObjLookup(pPubKeyObj, CK_IA_MODULUS);
	if (NULL == pObjAttr || NULL == pObjAttr->pValue)
	{
		SYNO_LogE("CK_IA_MODULUS attribyte is NULL!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	if (pObjAttr->ulValueLen != pstuRsaPubKey->BitLen / 8)
	{
		SYNO_LogE("CK_IA_MODULUS attribyte value length is wrong! pObjAttr->ulValueLen = 0x%x", pObjAttr->ulValueLen);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	memcpy(pstuRsaPubKey->Modulus + (MAX_RSA_MODULUS_LEN - pstuRsaPubKey->BitLen/8), pObjAttr->pValue, pObjAttr->ulValueLen);

	// 指数
	pObjAttr = CI_ObjLookup(pPubKeyObj, CK_IA_PUBLIC_EXPONENT);
	if (NULL == pObjAttr || NULL == pObjAttr->pValue)
	{
		SYNO_LogE("CK_IA_PUBLIC_EXPONENT attribyte is NULL!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	if (pObjAttr->ulValueLen != 4)
	{
		SYNO_LogE("CK_IA_PUBLIC_EXPONENT attribyte value length is wrong! pObjAttr->ulValueLen = 0x%x", pObjAttr->ulValueLen);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	memcpy(pstuRsaPubKey->PublicExponent, pObjAttr->pValue, pObjAttr->ulValueLen);

	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	名称:TOK_I_EnumDataFileObj
 *	说明:枚举指定应用下的所有Data File，并创建对应的对象。
 *	参数:session_data:[IN]:当前会话数据
 *	     hApp:[IN]:应用对象句柄
 *		 lpAppName:[IN]:应用对象句柄hApp对应的应用名
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static ULONG TOK_I_EnumDataFileObj(CK_I_SESSION_DATA_PTR session_data, 
								   HAPPLICATION hApp,
								   LPSTR lpAppName)
{
	ULONG ulRet = SAR_OK;
	ULONG ulCount = 0;
	ULONG ulFilesLen = 0;
	CHAR* lpFile = NULL;
	CHAR* lpFileNames = NULL;
	CHAR* lpFileNameList = NULL;
	CHAR* lpRemainder = NULL;
	FILEATTRIBUTE fileInfo = {0};
	CK_I_OBJ_PTR new_obj = NULL_PTR;
	CK_ATTRIBUTE_PTR pAttribute = NULL_PTR;
	CK_OBJECT_HANDLE hDataObject = NULL_PTR;

	// 参数检查
	if (NULL == session_data)
	{
		SYNO_LogE("session_data is NULL!");
		return 0;
	}
	if (NULL == hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return 0;
	}
	if (NULL == lpAppName || strlen(lpAppName) == 0)
	{
		SYNO_LogE("lpAppName is NULL!");
		return 0;
	}

	// 获取文件名列表大小
	ulRet = P11_EnumFiles(hApp, NULL, &ulFilesLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_EnumFiles() failed! ulRet = 0x%x", ulRet);
		return 0;
	}
	if (ulFilesLen <= 2)
	{
		SYNO_LogD("No any data files!");
		return 0;
	}

	// 枚举文件名
	lpFileNames = (CHAR*)SYNO_malloc(ulFilesLen + 2);
	memset(lpFileNames, 0, ulFilesLen + 2);
	ulRet = P11_EnumFiles(hApp, lpFileNames, &ulFilesLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_EnumFiles() failed! ulRet = 0x%x", ulRet);
		ulRet = 0;
		goto END;
	}

	// 将返回的文件名序列转化为字符串列表
	lpFileNameList = (CHAR*)SYNO_malloc(ulFilesLen + 2);
	memset(lpFileNameList, 0, ulFilesLen + 2);
	TOK_I_StrFmtList(lpFileNameList, lpFileNames);

	// 对每个文件创建一个CKO_DATA对象	
	lpFile = TC_strtok_r(lpFileNameList, ",", &lpRemainder);
	while (lpFile && strlen(lpFile) > 0)
	{
		SYNO_LogD("Found data file:%s", lpFile);

		// 获取文件属性
		ulRet = P11_GetFileInfo(hApp, lpFile, &fileInfo);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("P11_GetFileInfo() failed! ulRet = 0x%x", ulRet);
			goto NEXT;
		}
		if (fileInfo.FileSize == 0)
		{
			SYNO_LogE("fileInfo.FileSize = 0!");
			goto NEXT;
		}
		
		// 创建对象
		ulRet = CI_ObjCreateObj(&new_obj);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjCreateObj() failed! ulRet = 0x%x", ulRet);
			goto NEXT;
		}
		
		// 设置对象属性模板
		ulRet = CI_ObjReadTemplate(new_obj, s_DataObjTemplate, sizeof(s_DataObjTemplate)/sizeof(s_DataObjTemplate[0]));
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjReadTemplate() failed! ulRet = 0x%x", ulRet);
			CI_ObjDestroyObj(new_obj);
			new_obj = NULL;
			goto NEXT;
		}
		
		// 设置文件名属性
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ID));
		pAttribute->pValue = SYNO_calloc(1, strlen(fileInfo.FileName));
		strncpy((char*)pAttribute->pValue, fileInfo.FileName, strlen(fileInfo.FileName));
		pAttribute->ulValueLen = strlen(fileInfo.FileName);

		// 设置文件内容属性
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_VALUE));
		pAttribute->pValue = SYNO_calloc(1, fileInfo.FileSize);
		pAttribute->ulValueLen = fileInfo.FileSize;
		ulRet = P11_ReadFile(hApp, lpFile, 0, fileInfo.FileSize, (BYTE*)pAttribute->pValue, &pAttribute->ulValueLen);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("P11_ReadFile() failed! ulRet = 0x%x", ulRet);
			CI_ObjDestroyObj(new_obj);
			new_obj = NULL;
			goto NEXT;
		}
		
		// 将该对象加入对象列表
		ulRet = CI_InternalAddObject(session_data, new_obj, &hDataObject);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_InternalAddObject() failed! ulRet = 0x%x", ulRet);
			CI_ObjDestroyObj(new_obj);
			new_obj = NULL;
			goto NEXT;
		}

		ulCount++;

NEXT:
		lpFile = TC_strtok_r(NULL, ",", &lpRemainder);
	}
	
	FUNC_END;

END:
	if (lpFileNameList)
	{
		SYNO_free(lpFileNameList);
		lpFileNameList = NULL;
	}
	if (lpFileNames)
	{
		SYNO_free(lpFileNames);
		lpFileNames = NULL;
	}
	
	return ulCount;
}
/***********************************************************************
 *	名称:TOK_I_EnumPubKeyObj
 *	说明:枚举指定容器下的所有公钥，并创建对应的对象。
 *	参数:session_data:[IN]:当前会话数据
 *	     hContainer:[IN]:容器对象句柄
 *		 lpContainerName:[IN]:容器对象句柄hContainer对应的容器名
 *		 lpContainerInfoEx:[IN]:容器对象句柄hContainer对应的容器属性
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static ULONG TOK_I_EnumPubKeyObj(CK_I_SESSION_DATA_PTR session_data, 
								 HCONTAINER hContainer,
								 LPSTR lpContainerName,
								 LPCONTAINERINFOEX lpContainerInfoEx)
{
	ULONG ulRet = SAR_OK;
	ULONG ulCount = 0;
	BOOL bHasPubKey[2] = {FALSE, FALSE};
	ULONG ulIndex = 0;
	ULONG ulRsaPubKeyLen = 0;	
	RSAPUBLICKEYBLOB stRsaPubKey = {0};
	CK_I_OBJ_PTR new_obj = NULL_PTR;
	CK_ATTRIBUTE_PTR pAttribute = NULL_PTR;
	CK_OBJECT_HANDLE hPubKeyObject = NULL_PTR;

	// 参数检查
	if (NULL == session_data)
	{
		SYNO_LogE("session_data is NULL!");
		return 0;
	}
	if (NULL == hContainer)
	{
		SYNO_LogE("hContainer is NULL!");
		return 0;
	}
	if (NULL == lpContainerName || strlen(lpContainerName) == 0)
	{
		SYNO_LogE("lpContainerName is NULL!");
		return 0;
	}
	if (NULL == lpContainerInfoEx)
	{
		SYNO_LogE("lpContainerInfoEx is NULL!");
		return 0;
	}

	// 判断是否有公钥文件存在
	if (lpContainerInfoEx->signKeyPair)
	{
		bHasPubKey[0] = TRUE;
		SYNO_LogD("Has sign key pair!");
	}
	if (lpContainerInfoEx->exchKeyPair)
	{
		bHasPubKey[1] = TRUE;
		SYNO_LogD("Has exch key pair!");
	}
	if (lpContainerInfoEx->hasVerfiyPubKey)
	{
		bHasPubKey[0] = TRUE;
		SYNO_LogD("Has a verfiy public key!");
	}
	if (lpContainerInfoEx->hasEncryptPubKey)
	{
		bHasPubKey[1] = TRUE;
		SYNO_LogD("Has a encrypt public key!");
	}
	if (!bHasPubKey[0] && !bHasPubKey[1])
	{
		SYNO_LogD("No any public key!");
		return 0;
	}

	// 依次导出公钥并创建对象
	for (ulIndex = 0; ulIndex < 2; ulIndex++)
	{	
		if (!bHasPubKey[ulIndex])
		{
			continue;
		}

		// 导出公钥对象
		ulRsaPubKeyLen = sizeof(stRsaPubKey);
		ulRet = P11_ExportPublicKey(hContainer, (0 == ulIndex), (BYTE*)&stRsaPubKey, &ulRsaPubKeyLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_ExportPublicKey() failed! ulRet = 0x%x", ulRet);
			continue;
		}
	
		// 创建对象
		ulRet = CI_ObjCreateObj(&new_obj);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjCreateObj() failed! ulRet = 0x%x", ulRet);
			continue;
		}
		
		// 设置对象属性模板
		ulRet = CI_ObjReadTemplate(new_obj, s_PublicKeyObjTemplate, sizeof(s_PublicKeyObjTemplate)/sizeof(s_PublicKeyObjTemplate[0]));
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjReadTemplate() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
	
		// 设置容器名
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ID));
		pAttribute->pValue = SYNO_calloc(1, strlen(lpContainerName));
		strncpy((char*)pAttribute->pValue, lpContainerName, strlen(lpContainerName));
		pAttribute->ulValueLen = strlen(lpContainerName);
	
		// 设置公钥用途
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_VERIFY));
		*(CK_BYTE_PTR)pAttribute->pValue = (0 == ulIndex) ? s_ucTrue : s_ucFalse;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_WRAP));
		*(CK_BYTE_PTR)pAttribute->pValue = (0 == ulIndex) ? s_ucFalse : s_ucTrue;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ENCRYPT));
		*(CK_BYTE_PTR)pAttribute->pValue = (0 == ulIndex) ? s_ucFalse : s_ucTrue;
	
		// 设置公钥bits属性
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_MODULUS_BITS));
		*(CK_ULONG_PTR)pAttribute->pValue = stRsaPubKey.BitLen;

		// 设置公钥指数
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_PUBLIC_EXPONENT));
		memcpy(pAttribute->pValue, stRsaPubKey.PublicExponent, 4);

		// 设置公钥模
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_MODULUS));
		pAttribute->pValue = SYNO_calloc(1, stRsaPubKey.BitLen / 8);
		memcpy(pAttribute->pValue, stRsaPubKey.Modulus + MAX_RSA_MODULUS_BITS/8-stRsaPubKey.BitLen/8, stRsaPubKey.BitLen / 8);
		pAttribute->ulValueLen = stRsaPubKey.BitLen / 8;

		// 设置容器句柄
		new_obj->CtrHandle = hContainer;

		// 将该对象加入对象列表
		ulRet = CI_InternalAddObject(session_data, new_obj, &hPubKeyObject);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_InternalAddObject() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
		else
		{
			// 下一个
			ulCount++;
			continue;
		}

FREE_OBJ:
		CI_ObjDestroyObj(new_obj);
		new_obj = NULL;
	}
	
	FUNC_END;

	return ulCount;
}
/***********************************************************************
 *	名称:TOK_I_EnumPriKeyObj
 *	说明:枚举指定容器下的所有私钥，并创建对应的对象。
 *	参数:session_data:[IN]:当前会话数据
 *	     hContainer:[IN]:容器对象句柄
 *		 lpContainerName:[IN]:容器对象句柄hContainer对应的容器名
 *		 lpContainerInfoEx:[IN]:容器对象句柄hContainer对应的容器属性
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static ULONG TOK_I_EnumPriKeyObj(CK_I_SESSION_DATA_PTR session_data, 
								 HCONTAINER hContainer,
								 LPSTR lpContainerName,
								 LPCONTAINERINFOEX lpContainerInfoEx)
{
	ULONG ulRet = SAR_OK;
	ULONG ulCount = 0;
	ULONG ulIndex = 0;
	BOOL bHasPriKey[2] = {FALSE, FALSE};	
	CK_I_OBJ_PTR new_obj = NULL_PTR;
	CK_ATTRIBUTE_PTR pAttribute = NULL_PTR;
	CK_OBJECT_HANDLE hPriKeyObject = NULL_PTR;

	// 参数检查
	if (NULL == session_data)
	{
		SYNO_LogE("session_data is NULL!");
		return 0;
	}
	if (NULL == hContainer)
	{
		SYNO_LogE("hContainer is NULL!");
		return 0;
	}
	if (NULL == lpContainerName || strlen(lpContainerName) == 0)
	{
		SYNO_LogE("lpContainerName is NULL!");
		return 0;
	}
	if (NULL == lpContainerInfoEx)
	{
		SYNO_LogE("lpContainerInfoEx is NULL!");
		return 0;
	}

	// 判断是否有公钥文件存在
	if (lpContainerInfoEx->signKeyPair)
	{
		bHasPriKey[0] = TRUE;
		SYNO_LogD("Has sign key pair!");
	}
	if (lpContainerInfoEx->exchKeyPair)
	{
		bHasPriKey[1] = TRUE;
		SYNO_LogD("Has exch key pair!");
	}
	if (lpContainerInfoEx->hasSignPriKey)
	{
		bHasPriKey[0] = TRUE;
		SYNO_LogD("Has a sign private key!");
	}
	if (lpContainerInfoEx->hasDecryptPriKey)
	{
		bHasPriKey[1] = TRUE;
		SYNO_LogD("Has a decrypt private key!");
	}
	if (!bHasPriKey[0] && !bHasPriKey[1])
	{
		SYNO_LogD("No any private key!");
		return 0;
	}
		
	// 依次创建私钥对象
	for (ulIndex = 0; ulIndex < 2; ulIndex++)
	{
		if (!bHasPriKey[ulIndex])
		{
			continue;
		}

		// 创建对象
		ulRet = CI_ObjCreateObj(&new_obj);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjCreateObj() failed! ulRet = 0x%x", ulRet);
			continue;
		}
		
		// 设置对象属性模板
		ulRet = CI_ObjReadTemplate(new_obj, s_PrivateKeyObjTemplate, sizeof(s_PrivateKeyObjTemplate)/sizeof(s_PrivateKeyObjTemplate[0]));
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjReadTemplate() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
	
		// 设置容器名
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ID));
		pAttribute->pValue = SYNO_calloc(1, strlen(lpContainerName));
		strncpy((char*)pAttribute->pValue, lpContainerName, strlen(lpContainerName));
		pAttribute->ulValueLen = strlen(lpContainerName);
	
		// 设置私钥用途
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_SIGN));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucTrue : s_ucFalse;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_UNWRAP));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucFalse : s_ucTrue;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_DECRYPT));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucFalse : s_ucTrue;
	
		// 设置容器句柄
		new_obj->CtrHandle = hContainer;

		// 将该对象加入对象列表
		ulRet = CI_InternalAddObject(session_data, new_obj, &hPriKeyObject);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_InternalAddObject() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
		else
		{
			// 下一个
			ulCount++;
			continue;
		}

FREE_OBJ:
		CI_ObjDestroyObj(new_obj);
		new_obj = NULL;
	}

	FUNC_END;
	
	return ulCount;
}
/***********************************************************************
 *	名称:TOK_I_EnumCertObj
 *	说明:枚举指定容器下的所有证书，并创建对应的对象。
 *	参数:session_data:[IN]:当前会话数据
 *	     hContainer:[IN]:容器对象句柄
 *		 lpContainerName:[IN]:容器对象句柄hContainer对应的容器名
 *		 lpContainerInfoEx:[IN]:容器对象句柄hContainer对应的容器属性
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static ULONG TOK_I_EnumCertObj(CK_I_SESSION_DATA_PTR session_data, 
							   HCONTAINER hContainer,
								 LPSTR lpContainerName,
								 LPCONTAINERINFOEX lpContainerInfoEx)
{
	ULONG ulRet = SAR_OK;
	ULONG ulCount = 0;
	ULONG ulIndex = 0;
	ULONG ulCertDataLen = 0;
	BYTE* lpCertData = NULL;
	BOOL bHasCertificate[2] = {FALSE, FALSE};
	CK_I_OBJ_PTR new_obj = NULL_PTR;
	CK_ATTRIBUTE_PTR pAttribute = NULL_PTR;
	CK_OBJECT_HANDLE hCertObject = NULL_PTR;

	// 参数检查
	if (NULL == session_data)
	{
		SYNO_LogE("session_data is NULL!");
		return 0;
	}
	if (NULL == hContainer)
	{
		SYNO_LogE("hContainer is NULL!");
		return 0;
	}
	if (NULL == lpContainerName || strlen(lpContainerName) == 0)
	{
		SYNO_LogE("lpContainerName is NULL!");
		return 0;
	}
	if (NULL == lpContainerInfoEx)
	{
		SYNO_LogE("lpContainerInfoEx is NULL!");
		return 0;
	}

	// 判断是否有公钥文件存在
	if (lpContainerInfoEx->hasSignCert)
	{
		bHasCertificate[0] = TRUE;
		SYNO_LogD("Has sign certificate!");
	}
	if (lpContainerInfoEx->hasExchCert)
	{
		bHasCertificate[1] = TRUE;
		SYNO_LogD("Has sign certificate!");
	}
	if (!bHasCertificate[0] && !bHasCertificate[0])
	{
		SYNO_LogD("No any certificate!");
		return CKR_OK;
	}
		
	// 依次导出证书并创建证书对象
	for (ulIndex = 0; ulIndex < 2; ulIndex++)
	{
		if (!bHasCertificate[ulIndex])
		{
			continue;
		}

		// 导出证书
		ulRet = P11_ExportCertificate(hContainer, (0 == ulIndex), NULL, &ulCertDataLen);
		if (ulRet != CKR_OK || 0 == ulCertDataLen)
		{
			SYNO_LogE("P11_ExportCertificate() failed! ulRet = 0x%x", ulRet);
			continue;
		}
		lpCertData = (BYTE*)SYNO_calloc(1, ulCertDataLen);
		ulRet = P11_ExportCertificate(hContainer, (0 == ulIndex), lpCertData, &ulCertDataLen);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("P11_ExportCertificate() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}

		// 创建对象
		ulRet = CI_ObjCreateObj(&new_obj);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjCreateObj() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
		
		// 设置对象属性模板
		ulRet = CI_ObjReadTemplate(new_obj, s_CertObjTemplate, sizeof(s_CertObjTemplate)/sizeof(s_CertObjTemplate[0]));
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjReadTemplate() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}

		// 设置证书类型
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_CERTIFICATE_TYPE));
		*(CK_ULONG_PTR)pAttribute->pValue = CKC_X_509;
	
		// 设置容器名
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ID));
		pAttribute->pValue = SYNO_calloc(1, strlen(lpContainerName));
		strncpy((char*)pAttribute->pValue, lpContainerName, strlen(lpContainerName));
		pAttribute->ulValueLen = strlen(lpContainerName);
	
		// 设置证书用途
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_SIGN));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucTrue : s_ucFalse;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_VERIFY));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucTrue : s_ucFalse;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ENCRYPT));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucFalse : s_ucTrue;

		// 设置证书内容
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_VALUE));
		pAttribute->pValue = SYNO_calloc(1, ulCertDataLen);
		memcpy(pAttribute->pValue, lpCertData, ulCertDataLen);
		pAttribute->ulValueLen = ulCertDataLen;
	
		// 设置容器句柄
		new_obj->CtrHandle = hContainer;

		// 将该对象加入对象列表
		ulRet = CI_InternalAddObject(session_data, new_obj, &hCertObject);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_InternalAddObject() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
		else
		{
			// 下一个
			ulCount++;
			continue;
		}

FREE_OBJ:
		SYNO_free(lpCertData);
		lpCertData = NULL;
		CI_ObjDestroyObj(new_obj);
		new_obj = NULL;
	}

	FUNC_END;
	
	return ulCount;
}

/***********************************************************************
 *	名称:TOK_I_ImportPublicKey
 *	说明:将公钥对象数据导入到Key中
 *	参数:hApp:[IN]:应用对象句柄
 *	     pPubKeyObj:[IN]:公钥对象指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static CK_RV TOK_I_ImportPublicKey(HAPPLICATION hApp, 
								   CK_I_OBJ_PTR pPubKeyObj)
{
	ULONG ulRet = SAR_OK;
	ULONG ulValueLen = 0;
	BYTE bIsSign = FALSE;
	BYTE bIsExch = FALSE;
	BYTE bIsEncrypt = FALSE;
	BYTE bIsDecrypt = FALSE;
	BYTE btModulus[MAX_RSA_MODULUS_LEN] = {0};
	CHAR csContainer[64] = {0};
	LPSTR lpNewContainer = NULL;
	RSAPUBLICKEYBLOB stRsaPubKey = {0};
	HCONTAINER hContainer = NULL;

	// 参数检查
	if (NULL_PTR == hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL_PTR == pPubKeyObj)
	{
		SYNO_LogE("pPubKeyObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// 设置算法标示
	stRsaPubKey.AlgID = SGD_RSA;

	// 获取公钥模
	ulValueLen = MAX_RSA_MODULUS_LEN;
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_MODULUS, btModulus, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_MODULUS failed! ulRet = 0x%x", ulRet);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	if (128 != ulValueLen && 256 != ulValueLen)
	{
		SYNO_LogE("CKA_MODULUS attribute data length is wrong! ulValueLen = 0x%x", ulValueLen);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	memcpy(stRsaPubKey.Modulus + (MAX_RSA_MODULUS_LEN - ulValueLen), btModulus, ulValueLen);

	// 计算公钥bits(不直接获取CKA_MODULUS_BITS属性，因为该参数为可选)
	stRsaPubKey.BitLen = ulValueLen * 8;

	ulValueLen = sizeof(stRsaPubKey.BitLen);
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_MODULUS_BITS, (CK_BYTE_PTR)&stRsaPubKey.BitLen, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_MODULUS_BITS failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}	
		
	// 获取公钥指数
	ulValueLen = sizeof(stRsaPubKey.PublicExponent);
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_PUBLIC_EXPONENT, stRsaPubKey.PublicExponent, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_MODULUS failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	// 获取公钥用途
	ulValueLen = sizeof(BYTE);
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_VERIFY, (CK_BYTE_PTR)&bIsSign, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VERIFY failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_WRAP, (CK_BYTE_PTR)&bIsExch, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_WRAP failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_ENCRYPT, (CK_BYTE_PTR)&bIsEncrypt, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ENCRYPT failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_DECRYPT, (CK_BYTE_PTR)&bIsDecrypt, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_DECRYPT failed! ulRet = 0x%x", ulRet);
	}
	if (!bIsSign && !bIsExch && !bIsEncrypt && !bIsDecrypt)
	{
		SYNO_LogE("All of CKA_VERIFY, CKA_WRAP, CKA_ENCRYPT and CKA_DECRYPT attribute is FALSE!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// 输出公钥用途信息
	if (bIsSign) SYNO_LogD("This public key can be use to verify signature.");
	if (bIsExch) SYNO_LogD("This public key can be use to wrap session key.");
	if (bIsEncrypt) SYNO_LogD("This public key can be use to encrypt data.");
	if (bIsDecrypt) SYNO_LogD("This public key can be use to decrypt data.");
	
	// 打开容器,先获取CKA_ID属性值,如果没有该属性则创建一个新容器
	ulValueLen = 64;
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_ID, (CK_BYTE_PTR)csContainer, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ID failed!ulRet = 0x%x", ulRet);
	
		// 创建容器
		do
		{
			lpNewContainer = TOK_I_GenUniqeContainerId();
			ulRet = P11_CreateContainer(hApp, lpNewContainer, &hContainer);
		}while (SAR_COS_CONTAINER_ALREADY_EXISTS == ulRet);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
		SYNO_LogD("New container: %s is created！", lpNewContainer);
		CI_ObjSetAttributeValue(pPubKeyObj, CKA_ID, (CK_BYTE_PTR)lpNewContainer, strlen(lpNewContainer));
	}
	else
	{
		SYNO_LogD("Try to open and create the set container: %s！", csContainer);
		ulRet = P11_OpenContainer(hApp, csContainer, &hContainer);
		if (SAR_OK != ulRet)
		{
			ulRet = P11_CreateContainer(hApp, csContainer, &hContainer);
			if (SAR_OK != ulRet)
			{
				SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
				return CKR_GENERAL_ERROR;
			}
		}
	}

	// 导入公钥
	ulRet = P11_ImportPlainRSAPubKey(hContainer, bIsSign ? 1 : 0,  &stRsaPubKey);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_ImportPlainRSAKeyPair() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
	}
	pPubKeyObj->CtrHandle = hContainer;

	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	名称:TOK_I_ImportPrivateKey
 *	说明:将私钥对象数据导入到Key中
 *	参数:hApp:[IN]:应用对象句柄
 *       pPriKeyObj:[IN]:私钥对象指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static CK_RV TOK_I_ImportPrivateKey(HAPPLICATION hApp,
									CK_I_OBJ_PTR pPriKeyObj)
{
	ULONG ulRet = SAR_OK;
	ULONG ulValueLen = 0;
	BYTE bIsSign = FALSE;
	BYTE bIsExch = FALSE;
	BYTE bIsEncrypt = FALSE;
	BYTE bIsDecrypt = FALSE;
	CHAR csContainer[64] = {0};
	LPSTR lpNewContainer = NULL;
	RSAPRIVATEKEYBLOB stRsaPriKey = {0};
	HCONTAINER hContainer = NULL;
	BYTE btData[256] = {0};

	// 参数检查
	if (NULL_PTR == hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL_PTR == pPriKeyObj)
	{
		SYNO_LogE("pPubKeyObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// 设置算法标示
	stRsaPriKey.AlgID = SGD_RSA;

	// 获取私钥指数PrivateExponent
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_PRIVATE_EXPONENT, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_PRIVATE_EXPONENT failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (128 != ulValueLen && 256 != ulValueLen)
	{
		SYNO_LogE("CKA_PRIVATE_EXPONENT attribute data length is wrong! ulValueLen = 0x%x", ulValueLen);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// 计算公钥bits(不直接获取CKA_MODULUS_BITS属性，因为该参数为可选)
	stRsaPriKey.BitLen = ulValueLen * 8;
	memcpy(stRsaPriKey.PrivateExponent + (MAX_RSA_MODULUS_LEN - stRsaPriKey.BitLen/8), btData, ulValueLen);
	
	// 获取私钥指数Prime1
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_PRIME_1, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_PRIME_1 failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	memcpy(stRsaPriKey.Prime1 + (MAX_RSA_PRIME_LEN - stRsaPriKey.BitLen/16), btData, ulValueLen);
	
	// 获取私钥指数Prime2
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_PRIME_2, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_PRIME_2 failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	memcpy(stRsaPriKey.Prime2 + (MAX_RSA_PRIME_LEN - stRsaPriKey.BitLen/16), btData, ulValueLen);
	
	// 获取私钥指数Prime1Exponent
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_EXPONENT_1, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_EXPONENT_1 failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	memcpy(stRsaPriKey.Prime1Exponent + (MAX_RSA_PRIME_LEN - stRsaPriKey.BitLen/16), btData, ulValueLen);
	
	// 获取私钥指数Prime2Exponent
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_EXPONENT_2, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_EXPONENT_2 failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	memcpy(stRsaPriKey.Prime2Exponent + (MAX_RSA_PRIME_LEN - stRsaPriKey.BitLen/16), btData, ulValueLen);

	// 获取私钥指数Coefficient
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_COEFFICIENT, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_COEFFICIENT failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	memcpy(stRsaPriKey.Coefficient + (MAX_RSA_PRIME_LEN - stRsaPriKey.BitLen/16), btData, ulValueLen);

	// 获取私钥用途
	ulValueLen = sizeof(BYTE);
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_SIGN, (CK_BYTE_PTR)&bIsSign, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_SIGN failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_UNWRAP, (CK_BYTE_PTR)&bIsExch, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_UNWRAP failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_ENCRYPT, (CK_BYTE_PTR)&bIsEncrypt, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ENCRYPT failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_DECRYPT, (CK_BYTE_PTR)&bIsDecrypt, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_DECRYPT failed! ulRet = 0x%x", ulRet);
	}
	if (!bIsSign && !bIsExch && !bIsEncrypt && !bIsDecrypt)
	{
		SYNO_LogE("All of CKA_VERIFY, CKA_UNWRAP, CKA_ENCRYPT and CKA_DECRYPT attribute is FALSE!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// 输出私钥用途信息
	if (bIsSign) SYNO_LogD("This private key can be use to sign data.");
	if (bIsExch) SYNO_LogD("This private key can be use to unwrap session key.");
	if (bIsEncrypt) SYNO_LogD("This private key can be use to encrypt data.");
	if (bIsDecrypt) SYNO_LogD("This private key can be use to decrypt data.");

	// 打开容器,先获取CKA_ID属性值,如果没有该属性则创建一个新容器
	ulValueLen = 64;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_ID, (CK_BYTE_PTR)csContainer, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ID failed!ulRet = 0x%x", ulRet);
	
		// 创建容器
		do
		{
			lpNewContainer = TOK_I_GenUniqeContainerId();
			ulRet = P11_CreateContainer(hApp, lpNewContainer, &hContainer);
		}while (SAR_COS_CONTAINER_ALREADY_EXISTS == ulRet);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
		SYNO_LogD("New container: %s is created！", lpNewContainer);
		CI_ObjSetAttributeValue(pPriKeyObj, CKA_ID, (CK_BYTE_PTR)lpNewContainer, strlen(lpNewContainer));
	}
	else
	{
		SYNO_LogD("Try to open and create the set container: %s！", csContainer);
		ulRet = P11_OpenContainer(hApp, csContainer, &hContainer);
		if (SAR_OK != ulRet)
		{
			ulRet = P11_CreateContainer(hApp, csContainer, &hContainer);
			if (SAR_OK != ulRet)
			{
				SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
				return CKR_GENERAL_ERROR;
			}
		}
	}

	// 导入私钥
	ulRet = P11_ImportPlainRSAPriKey(hContainer, bIsSign ? 1 : 0, &stRsaPriKey);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_ImportPlainRSAKeyPair() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
	}
	pPriKeyObj->CtrHandle = hContainer;

	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	名称:TOK_I_ImportSessionKey
 *	说明:将对称密钥数据导入到Key中
 *	参数:hDev:[IN]:设备对象句柄
 *		 pKeyObj:[IN]:对称密钥对象指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static CK_RV TOK_I_ImportSessionKey(DEVHANDLE hDev,
									CK_I_OBJ_PTR pKeyObj)
{
	ULONG ulRet = 0;
	ULONG ulAlgID = 0;
	ULONG ulKeyLen = 0;
	ULONG ulValueLen = 0;
	BYTE* lpKeyData = NULL;	
	HANDLE hKey = NULL;

	FUNC_BEGIN;
		
	// 参数检查
	if (NULL_PTR == hDev)
	{
		SYNO_LogE("hDev is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL_PTR == pKeyObj)
	{
		SYNO_LogE("pKeyObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	// 获取Key的算法类型
	ulValueLen = 4;
	ulRet = CI_ObjGetAttributeValue(pKeyObj, CKA_KEY_TYPE, (CK_BYTE_PTR)&ulAlgID, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_KEY_TYPE failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	switch (ulAlgID)
	{
	case CKK_AES:
		ulAlgID = SGD_AES128;
		break;
	case CKK_DES:
		ulAlgID = SGD_DES;
		break;
	case CKK_DES3:
		ulAlgID = SGD_DES3;
		break;
	default:
		SYNO_LogE("CKA_KEY_TYPE is not suppored! ulAlgID = 0x%x", ulAlgID);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	SYNO_LogD("Coverted alg id:0x%x", ulAlgID);
	
	// 获取Key长度
	ulRet = CI_ObjGetAttributeValue(pKeyObj, CKA_VALUE_LEN, (CK_BYTE_PTR)&ulKeyLen, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VALUE_LEN failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (ulKeyLen == 0)
	{
		SYNO_LogE("CKA_VALUE_LEN value is 0!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	
	// 分配空间
	lpKeyData = (BYTE*)SYNO_malloc(ulKeyLen);
	if (NULL == lpKeyData)
	{
		SYNO_LogE("SYNO_malloc() return failed!");
		return CKR_HOST_MEMORY;
	}

	// 获取Key数据
	ulValueLen = ulKeyLen;
	ulRet = CI_ObjGetAttributeValue(pKeyObj, CKA_VALUE, lpKeyData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VALUE failed! ulRet = 0x%x", ulRet);
		ulRet = ulRet;
		goto FAILED;
	}

	// 导入会话密钥
	ulRet = P11_SetSymmKey(hDev, lpKeyData, ulAlgID, &hKey);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_SetSymmKey() failed! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto FAILED;
	}
	pKeyObj->ObjHandle = hKey;

	FUNC_END;

FAILED:
	if (lpKeyData)
	{
		SYNO_free(lpKeyData);
		lpKeyData = NULL;
	}

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_I_ImportCertificate
 *	说明:将证书对象数据导入到Key中
 *	参数:hApp:[IN]:应用句柄
 *       pObj:[IN]:对象指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static CK_RV TOK_I_ImportCertificate(HAPPLICATION hApp, 
									 CK_I_OBJ_PTR pCertObj)
{
	ULONG ulRet = 0;
	ULONG ulValueLen = 0;
	ULONG ulCertDataLen = 0;
	BYTE* pCertData = NULL;
	CHAR csContainer[64] = {0};
	BOOL bIsSign = FALSE;
	BOOL bIsExch = FALSE;
	LPSTR lpNewContainer = NULL;
	HCONTAINER hContainer = NULL;

	FUNC_BEGIN;
		
	// 参数检查
	if (NULL_PTR == hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL_PTR == pCertObj)
	{
		SYNO_LogE("pCertObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// 获取证书用途
	ulValueLen = sizeof(BOOL);
	ulRet = CI_ObjGetAttributeValue(pCertObj, CKA_SIGN, (CK_BYTE_PTR)&bIsSign, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_SIGN failed! ulRet = 0x%x", ulRet);
	}
	ulValueLen = sizeof(BOOL);
	ulRet = CI_ObjGetAttributeValue(pCertObj, CKA_ENCRYPT, (CK_BYTE_PTR)&bIsExch, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ENCRYPT failed! ulRet = 0x%x", ulRet);
	}
	if ((!bIsSign && !bIsExch) || (bIsSign && bIsExch))
	{
		SYNO_LogE("Certificate usage type is wrong! bIsSign = %d, bIsExch = %d", bIsSign, bIsExch);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	else
	{
		if (bIsSign) SYNO_LogD("This is a sign certificate!");
		if (bIsExch) SYNO_LogD("This is a exchange certificate!");
	}

	// 获取证书数据长度
	ulValueLen = 64;
	ulRet = CI_ObjGetAttributeValue(pCertObj, CKA_VALUE_LEN, (CK_BYTE_PTR)&ulCertDataLen, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VALUE_LEN failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (ulCertDataLen == 0)
	{
		SYNO_LogE("CKA_VALUE_LEN value is 0!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	
	// 分配空间
	pCertData = (BYTE*)SYNO_malloc(ulValueLen);
	if (NULL == pCertData)
	{
		SYNO_LogE("SYNO_malloc() return failed!");
		return CKR_HOST_MEMORY;
	}

	// 获取Key数据
	ulValueLen = ulCertDataLen;
	ulRet = CI_ObjGetAttributeValue(pCertObj, CKA_VALUE, pCertData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VALUE failed! ulRet = 0x%x", ulRet);
		goto FAILED;
	}
	
	// 打开容器,先获取CKA_ID属性值,如果没有该属性则创建一个新容器
	ulValueLen = 64;
	ulRet = CI_ObjGetAttributeValue(pCertObj, CKA_ID, (CK_BYTE_PTR)csContainer, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ID failed!ulRet = 0x%x", ulRet);
	
		// 创建容器
		do
		{
			lpNewContainer = TOK_I_GenUniqeContainerId();
			ulRet = P11_CreateContainer(hApp, lpNewContainer, &hContainer);
		}while (SAR_COS_CONTAINER_ALREADY_EXISTS == ulRet);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
		SYNO_LogD("New container: %s is created！", lpNewContainer);
		CI_ObjSetAttributeValue(pCertObj, CKA_ID, (CK_BYTE_PTR)lpNewContainer, strlen(lpNewContainer));
	}
	else
	{
		SYNO_LogD("The set container: %s is created！", csContainer);
		ulRet = P11_OpenContainer(hApp, csContainer, &hContainer);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_OpenContainer() failed! ulRet = 0x%x", ulRet);			
			return CKR_ATTRIBUTE_VALUE_INVALID;
		}
	}

	// 导入证书
	ulRet = P11_ImportCertificate(hContainer, bIsSign, pCertData, ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_ImportCertificate() failed! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto FAILED;
	}
	pCertObj->CtrHandle = hContainer;

	FUNC_END;

FAILED:
	if (pCertData)
	{
		SYNO_free(pCertData);
		pCertData = NULL;
	}
	
	//CKA_ID:容器名
	//CKA_LABEL:使用者
	//CKA_SUBJECT:Subject
	//CKA_ISSUER:Issuer
	//CKA_SERIAL_NUMBER:SN

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_I_ImportDataFile
 *	说明:将证书对象数据导入到Key中
 *	参数:hApp:[IN]:应用句柄
 *       pObj:[IN]:对象指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static CK_RV TOK_I_ImportDataFile(HAPPLICATION hApp, 
								  CK_I_OBJ_PTR pDataObj)
{
	ULONG ulRet = 0;
	ULONG ulFileSize = 0;
	ULONG ulValueLen = 256;
	CHAR csFileName[256] = {0};
	
	FUNC_BEGIN;

	// 参数检查
	if (NULL_PTR == hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL_PTR == pDataObj)
	{
		SYNO_LogE("pDataObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
		
	// 文件名
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pDataObj, CKA_ID, (CK_BYTE_PTR)csFileName, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ID failed! ulRet = 0x%x", ulRet);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// 获取文件大小
	ulValueLen = sizeof(ULONG);
	ulRet = CI_ObjGetAttributeValue(pDataObj, CKA_VALUE_LEN, (CK_BYTE_PTR)&ulFileSize, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VALUE_LEN failed! ulRet = 0x%x", ulRet);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	
	// 创建数据文件
	ulRet = P11_CreateFile(hApp, csFileName, ulFileSize, SECURE_USER_ACCOUNT, SECURE_USER_ACCOUNT);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_CreateFile() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
	}

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_I_DeleteObjContainer
 *	说明:删除对象所在的容器
 *	参数:hApp:[IN]:应用句柄
 *       pObj:[IN]:对象指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
static CK_RV TOK_I_DeleteObjContainer(HAPPLICATION hApp, 
									  CK_I_OBJ_PTR pObj)
{
	ULONG ulRet = 0;
	CK_BYTE csObjContainer[64] = {0};
	CK_ULONG ulContainerLen = 64;
	
	// 参数检查
	if (!hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (!pObj)
	{
		SYNO_LogE("pObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	// 关闭容器
	if (pObj->CtrHandle)
	{
		ulRet = P11_CloseContainer(pObj->CtrHandle);
		if (CKR_OK != ulRet)
		{
			SYNO_LogE("P11_CloseContainer() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}

	// 获取容器名
	ulRet = CI_ObjGetAttributeValue(pObj, CKA_ID, csObjContainer, &ulContainerLen); 
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("CI_ObjGetIntAttributeValue() to get CKA_ID value failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}

	// 删除容器
	ulRet = P11_DeleteContainer(hApp, (LPSTR)csObjContainer);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("P11_DeleteContainer() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
	}

	return CKR_OK;
}


#if 0
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
	pstFindTemplate = (CK_ATTRIBUTE_PTR)SYNO_calloc(1, sizeof(g_astFindRSATemplate));
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
			pstFindTemplate[i].pValue = (CK_OBJECT_CLASS_PTR)SYNO_calloc(1, sizeof(CK_OBJECT_CLASS));
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
	pstPublicKeyTemplate = (CK_ATTRIBUTE_PTR)SYNO_calloc(1, sizeof(g_astGetRSAPublicKeyTemplate));
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
			pstPublicKeyTemplate[i].pValue = (CK_BYTE_PTR)SYNO_calloc(1, pstPublicKeyTemplate[i].ulValueLen);
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
            pstKeyTemplate = (CK_ATTRIBUTE_PTR)SYNO_calloc(1, sizeof(g_astRSAPublicKeyTemplate));
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
            pstKeyTemplate = (CK_ATTRIBUTE_PTR)SYNO_calloc(1, sizeof(g_astRSAPrivateKeyTemplate));
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
    ulRet = P11_ExportPublicKey(hContainer, FALSE, (BYTE *)&stRSAPubBlob, &ulBlobLen);
    if (ulRet != CKR_OK)
	{
		goto End;
	}

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPublicObject, CK_IA_ID));
    pstTemplate->pValue = SYNO_calloc(1, strlen((char *)pcContainerName) + 1);
    strncpy((char*)pstTemplate->pValue, pcContainerName, strlen((char *)pcContainerName));
    pstTemplate->ulValueLen = strlen((char *)pcContainerName) + 1;

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPublicObject, CK_IA_MODULUS));
    pstTemplate->pValue = SYNO_calloc(1, MAX_RSA_MODULUS_LEN);
    memcpy(pstTemplate->pValue, stRSAPubBlob.Modulus, MAX_RSA_MODULUS_LEN);
    pstTemplate->ulValueLen = MAX_RSA_MODULUS_LEN;

    //ulRet = P11_GetRSAKeyHandle(hContainer, AT_KEYEXCHANGE, &hKey);
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
    pstTemplate->pValue = SYNO_calloc(1, strlen((char *)pcContainerName) + 1);
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

ULONG ICA_CheckName(CHAR *pcNameList, CHAR *pcName)
{
    ULONG ulRet = CKR_OK;
    CHAR acList[PATH_MAX] = {0};
    CHAR *pcCheckList = NULL;
    CHAR *pcRemainder = NULL;
    CHAR *pcCheckName = NULL;

    TOK_I_StrFmtList(acList, pcNameList);

    pcCheckList = (CHAR *)SYNO_malloc(strlen((char *)acList) + 1);
	strcpy(pcCheckList, acList);

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

    SYNO_free(pcCheckList);

    return ulRet;
}



/***********************************************************************
 *	名称:TOK_EnumToken
 *	说明:返回找到的Token名列表
 *	参数:pucTokenList:[OUT]:存放Token名的字符指针，每个Token名之间用'\0'分割，以2个'\0'表示结束。
 *       pulSize:[OUT]:返回整个列表buffer的长度。
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_EnumToken(CK_CHAR_PTR pucTokenList, 
					ULONG *pulSize)
{
    ULONG ulRet = CKR_OK;
    UCHAR aucNameList[PATH_MAX] = {0};
	ULONG ulSize = PATH_MAX;
	UCHAR *pucTmp = NULL;
	ULONG ulTmpLen = 0;
	ULONG ulLen = 0;

    FUNC_BEGIN;

    // 参数检查
    if ((NULL == pucTokenList) || (NULL == pulSize))
    {
		SYNO_LogE("pucTokenList or pulSize is NULL!");
        return CKR_GENERAL_ERROR;
    }

    // 枚举设备
    ulRet = P11_EnumDev(TRUE, (LPSTR)aucNameList, &ulSize);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_EnumDev() failed! ulRet = 0x%x", ulRet);
        goto End;
    }
	
    TOK_I_StrFmtList((CHAR *)pucTokenList, (CHAR *)aucNameList);

End:
    FUNC_END;
    return ulRet;
}
/***********************************************************************
 *	名称:TOK_TokenInitialize
 *	说明:初始化一个Token的相关数据
 *	参数:pucTokenName:[IN]:Token名
 *       ppSlotData:[OUT]:返回初始化后的相关数据
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_TokenInitialize(CK_CHAR_PTR pucTokenName,
						  CK_I_SLOT_DATA_PTR CK_PTR ppSlotData)
{
    ULONG ulRet = CKR_GENERAL_ERROR;
    CK_SLOT_INFO_PTR pstSlotInfo = NULL;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    CK_TOKEN_INFO_PTR pstTokenInfo = NULL;
    CK_I_TOKEN_DATA_PTR pstTokenData = NULL;
	CK_UTF8CHAR_PTR pUTF8 = NULL;
    DEVHANDLE hDev = NULL;
    DEVINFO DevInfo = {0};

    FUNC_BEGIN;
	
    // 连接设备
    ulRet = P11_ConnectDev((CHAR *)pucTokenName, &hDev);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_ConnectDev() failed! ulRet = 0x%x", ulRet);
        return ulRet;
    }

    // 获取设备信息
    ulRet = P11_GetDevInfo(hDev, &DevInfo);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_GetDevInfo() failed! ulRet = 0x%x", ulRet);
        return ulRet;
    }

	// 设置Slot Info
    pstSlotInfo = (CK_SLOT_INFO_PTR)SYNO_calloc(1, sizeof(CK_SLOT_INFO));
    if (NULL == pstSlotInfo)
    {
		SYNO_LogE("SYNO_calloc() failed!");
        goto Err;
    }
	memset(pstSlotInfo, 0, sizeof(CK_SLOT_INFO));

    strcpy((char*)pstSlotInfo->slotDescription, "Syno TF Key");
	pUTF8 = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(DevInfo.Manufacturer);
	memcpy(pstSlotInfo->manufacturerID, pUTF8, strlen((char*)pUTF8));
	SYNO_free(pUTF8);
	//
	pstSlotInfo->hardwareVersion.major = DevInfo.HWVersion.major;
	pstSlotInfo->hardwareVersion.minor = DevInfo.HWVersion.minor;
	pstSlotInfo->firmwareVersion.major = DevInfo.FirmwareVersion.major;
    pstSlotInfo->firmwareVersion.minor = DevInfo.FirmwareVersion.minor;

    pstSlotData = (CK_I_SLOT_DATA_PTR)SYNO_calloc(1, sizeof(CK_I_SLOT_DATA));
    if (NULL == pstSlotData)
    {
		SYNO_LogE("SYNO_calloc() failed!");
        goto FreeSlotInfo;
    }
	memset(pstSlotData, 0, sizeof(CK_I_SLOT_DATA));

    pstSlotData->flags = 0;
    pstSlotData->slot_info = pstSlotInfo;
    pstSlotData->config_section_name = (UCHAR *)SYNO_calloc(1, strlen((char *)pucTokenName) + 1);
    if (NULL == pstSlotData->config_section_name)
    {
		SYNO_LogE("SYNO_calloc() failed!");
        goto FreeSlotData;
    }
	memset(pstSlotData->config_section_name, 0, strlen((char *)pucTokenName) + 1);
    strcpy((CHAR *)pstSlotData->config_section_name, (CHAR *)pucTokenName);

    pstTokenInfo = (CK_TOKEN_INFO_PTR)SYNO_calloc(1, sizeof(CK_TOKEN_INFO));
    if (NULL == pstTokenInfo)
    {
		SYNO_LogE("SYNO_calloc() failed!");
        goto FreeSectionName;
    }
	memset(pstTokenInfo, 0, sizeof(CK_TOKEN_INFO));
	
	pUTF8 = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(DevInfo.Label);
	memcpy(pstTokenInfo->label, pUTF8, strlen((char*)pUTF8));
	SYNO_free(pUTF8);
	pUTF8 = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(DevInfo.Manufacturer);
	memcpy(pstTokenInfo->manufacturerID, pUTF8, strlen((char*)pUTF8));
	SYNO_free(pUTF8);
	pUTF8 = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(DevInfo.SerialNumber);
	memcpy(pstTokenInfo->serialNumber, pUTF8, strlen((char*)pUTF8));
	SYNO_free(pUTF8);
	//
    pstTokenInfo->ulMinPinLen = TOK_MIN_PIN_LEN;
    pstTokenInfo->ulMaxPinLen = TOK_MAX_PIN_LEN;
    pstTokenInfo->ulMaxSessionCount = TOK_MAX_SESSION_COUNT;
    pstTokenInfo->ulMaxRwSessionCount = TOK_MAX_RW_SESSION_COUNT;
	pstTokenInfo->ulTotalPublicMemory = DevInfo.TotalSpace;
	pstTokenInfo->ulFreePublicMemory = DevInfo.FreeSpace;
	pstTokenInfo->hardwareVersion.major = DevInfo.HWVersion.major;
	pstTokenInfo->hardwareVersion.minor = DevInfo.HWVersion.minor;
	pstTokenInfo->firmwareVersion.major = DevInfo.FirmwareVersion.major;
    pstTokenInfo->firmwareVersion.minor = DevInfo.FirmwareVersion.minor;

    pstSlotData->pDevHandle = hDev;

    pstSlotInfo->flags = CKF_TOKEN_PRESENT;

    pstTokenData = (CK_I_TOKEN_DATA_PTR)SYNO_calloc(1, sizeof(CK_I_TOKEN_DATA));
    if (NULL == pstTokenData)
    {
		SYNO_LogE("SYNO_calloc() failed!");
        goto FreeTokenInfo;
    }
	memset(pstTokenData, 0, sizeof(CK_I_TOKEN_DATA));

	// 保存算法标示
	pstTokenData->asymalg_flags = DevInfo.AlgAsymCap;
	pstTokenData->symalg_flags = DevInfo.AlgSymCap;
	pstTokenData->hashalg_flags = DevInfo.AlgHashCap;

	// 设置Token函数地址
	if (s_ck_i_token_methods.GetTokenInfo == NULL)
	{
		TOK_I_SetFunctionList();
	}
	pstSlotData->methods = &s_ck_i_token_methods;

    pstTokenData->token_info = pstTokenInfo;
    pstSlotData->token_data = pstTokenData;
    *ppSlotData = pstSlotData;

    FUNC_END;
    return CKR_OK;

FreeTokenInfo:
    SYNO_free(pstTokenInfo);
FreeSectionName:
    SYNO_free(pstSlotData->config_section_name);
FreeSlotData:
    SYNO_free(pstSlotData);
FreeSlotInfo:
    SYNO_free(pstSlotInfo);
Err:
    FUNC_END;
    return ulRet;
}
/***********************************************************************
 *	名称:TOK_GetTokenInfo
 *	说明:获取一个Token的信息
 *	参数:slot_data:[IN]:该Token所对应的Slot数据
 *       pInfo:[OUT]:存放返回Token信息的指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_GetTokenInfo(CK_I_SLOT_DATA_PTR  slot_data, 
					   CK_TOKEN_INFO_PTR pInfo)
{
	ULONG ulRet = CKR_OK;
    CK_I_TOKEN_DATA_PTR token_data = NULL_PTR;

	FUNC_BEGIN;

	// 检查参数
	if (slot_data == NULL_PTR)
	{
		SYNO_LogE("slot_data is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (pInfo == NULL_PTR)
	{
		SYNO_LogE("pInfo is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	// 检查token data
    token_data = slot_data->token_data;
	if (token_data == NULL_PTR)
	{
		SYNO_LogE("slot_data->token_data is NULL!");
		return CKR_GENERAL_ERROR;
	}

	// 拷贝数据
    strncpy((char*)pInfo->label,           (char*)token_data->token_info->label, 32);
    strncpy((char*)pInfo->manufacturerID,  (char*)token_data->token_info->manufacturerID, 32);
    strncpy((char*)pInfo->model,           (char*)token_data->token_info->model, 16);
    strncpy((char*)pInfo->serialNumber,    (char*)token_data->token_info->serialNumber, 16);

    pInfo->flags                 = token_data->token_info->flags;
    pInfo->ulMaxSessionCount     = token_data->token_info->ulMaxSessionCount;
    pInfo->ulSessionCount        = token_data->token_info->ulSessionCount;
    pInfo->ulMaxRwSessionCount   = token_data->token_info->ulMaxRwSessionCount;
    pInfo->ulRwSessionCount      = token_data->token_info->ulRwSessionCount;
    pInfo->ulMaxPinLen           = token_data->token_info->ulMaxPinLen;
    pInfo->ulMinPinLen           = token_data->token_info->ulMinPinLen;
    pInfo->ulTotalPublicMemory   = token_data->token_info->ulTotalPublicMemory;
    pInfo->ulFreePublicMemory    = token_data->token_info->ulFreePublicMemory;
    pInfo->ulTotalPrivateMemory  = token_data->token_info->ulTotalPrivateMemory;
    pInfo->ulFreePrivateMemory   = token_data->token_info->ulFreePrivateMemory;
    pInfo->hardwareVersion.major = token_data->token_info->hardwareVersion.major;
    pInfo->hardwareVersion.minor = token_data->token_info->hardwareVersion.minor;
    pInfo->firmwareVersion.major = token_data->token_info->firmwareVersion.major;
    pInfo->firmwareVersion.minor = token_data->token_info->firmwareVersion.minor;

	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	名称:TOK_GetMechanismList
 *	说明:获取一个Token所支持的算法类型列表
 *	参数:pMechanismList:[OUT]:存放算法类型的指针
 *       pulCount:[OUT]:返回所支持的算法类型个数
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_GetMechanismList(CK_MECHANISM_TYPE_PTR pMechanismList, 
						   CK_ULONG_PTR pulCount)
{
	CK_ULONG types_count = 0;

	FUNC_BEGIN;

	// 参数检查
	if (NULL_PTR == pulCount)
	{
		SYNO_LogE("pulCount is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	types_count = sizeof(s_ck_i_token_mechanism_types) / sizeof(s_ck_i_token_mechanism_types[0]);

	// 返回个数
	if (pMechanismList)
	{
		*pulCount = types_count;
		SYNO_LogI("Return mechanism count:0x%x", types_count);
		return CKR_OK;
	}

	// 检查Buffer空间大小
	if (*pulCount < types_count)
	{
		SYNO_LogI("Buffer is too small! Requested mechanism count:0x%x", types_count);
		return CKR_BUFFER_TOO_SMALL;
	}

	// 返回算法列表
	memcpy(pMechanismList, s_ck_i_token_mechanism_types, sizeof(s_ck_i_token_mechanism_types));
	*pulCount = types_count;

	SYNO_LogI("Returned mechanism counts:0x%x", *pulCount);
	
	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	名称:TOK_GetMechanismInfo
 *	说明:获取某个算法类型的具体信息
 *	参数:type:[IN]:指定的算法类型
 *       pInfo:[OUT]:返回该算法的相关信息
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_GetMechanismInfo(CK_MECHANISM_TYPE type, 
						   CK_MECHANISM_INFO_PTR pInfo)
{
	FUNC_BEGIN;

	// 参数检查
	if (NULL_PTR == pInfo)
	{
		SYNO_LogE("pInfo is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// 返回各个算法属性
	switch (type)
	{
	case CKM_RSA_PKCS_KEY_PAIR_GEN:
		pInfo->ulMinKeySize = 0x400;
		pInfo->ulMaxKeySize = 0x800;
		pInfo->flags = CKF_HW | CKF_GENERATE_KEY_PAIR;
		break;
	case CKM_RSA_PKCS:
		pInfo->ulMinKeySize = 0x400;
		pInfo->ulMaxKeySize = 0x800;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_MD5_RSA_PKCS:
		pInfo->ulMinKeySize = 0x400;
		pInfo->ulMaxKeySize = 0x800;
		pInfo->flags = CKF_HW | CKF_SIGN | CKF_VERIFY;
		break;
	case CKM_SHA1_RSA_PKCS:
		pInfo->ulMinKeySize = 0x400;
		pInfo->ulMaxKeySize = 0x800;
		pInfo->flags = CKF_HW | CKF_SIGN | CKF_VERIFY;
		break;
	case CKM_SHA256_RSA_PKCS:
		pInfo->ulMinKeySize = 0x400;
		pInfo->ulMaxKeySize = 0x800;
		pInfo->flags = CKF_HW | CKF_SIGN | CKF_VERIFY;
		break;		
	case CKM_AES_KEY_GEN:
		pInfo->ulMinKeySize = 0x80;
		pInfo->ulMaxKeySize = 0x80;
		pInfo->flags = CKF_HW | CKF_GENERATE;
		break;
	case CKM_AES_ECB:
		pInfo->ulMinKeySize = 0x80;
		pInfo->ulMaxKeySize = 0x80;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_AES_CBC:
		pInfo->ulMinKeySize = 0x40;
		pInfo->ulMaxKeySize = 0x40;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_DES_KEY_GEN:
		pInfo->ulMinKeySize = 0x40;
		pInfo->ulMaxKeySize = 0x40;
		pInfo->flags = CKF_HW | CKF_GENERATE;
		break;
	case CKM_DES_ECB:
		pInfo->ulMinKeySize = 0x40;
		pInfo->ulMaxKeySize = 0x40;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_DES_CBC:
		pInfo->ulMinKeySize = 0x40;
		pInfo->ulMaxKeySize = 0x40;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_DES3_KEY_GEN:
		pInfo->ulMinKeySize = 0xC0;
		pInfo->ulMaxKeySize = 0xC0;
		pInfo->flags = CKF_HW | CKF_GENERATE;
		break;
	case CKM_DES3_ECB:
		pInfo->ulMinKeySize = 0xC0;
		pInfo->ulMaxKeySize = 0xC0;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_DES3_CBC:
		pInfo->ulMinKeySize = 0xC0;
		pInfo->ulMaxKeySize = 0xC0;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_MD5:
		pInfo->ulMinKeySize = 0x80;
		pInfo->ulMaxKeySize = 0x80;
		pInfo->flags = CKF_HW | CKF_DIGEST;
		break;
	case CKM_SHA_1:
		pInfo->ulMinKeySize = 0xA0;
		pInfo->ulMaxKeySize = 0xA0;
		pInfo->flags = CKF_HW | CKF_DIGEST;
		break;
	case CKM_SHA256:
		pInfo->ulMinKeySize = 0x100;
		pInfo->ulMaxKeySize = 0x100;
		pInfo->flags = CKF_HW | CKF_DIGEST;
		break;
	default:
		SYNO_LogE("This mechanism (0x%x) is not supported!");
		return CKR_MECHANISM_INVALID;
	};
	
	FUNC_END;

	return CKR_OK;
}
 /***********************************************************************
 *	名称:TOK_InitToken
 *	说明:初始化一个Token，删除所有用户数据，并新建一个初始化应用。
 *	参数:slot_data:[IN]:该Token对应的Slot数据
 *       pPin:[IN]:初始SO PIN
 *       ulPinLen:[IN]:SO PIN长度
 *       pLabel:[IN]:Token标签
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_InitToken(CK_I_SLOT_DATA_PTR slot_data,
					CK_CHAR_PTR pPin, 
					CK_ULONG ulPinLen, 
					CK_CHAR_PTR pLabel)
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

	FUNC_BEGIN;

    // 参数检查
    if ((NULL == slot_data)
        || (NULL == slot_data->token_data)
        || (NULL == pPin)
        || (NULL == pLabel))
    {
		SYNO_LogE("slot_data or slot_data->token_data or pPin or pLabel is NULL!");
        return CKR_GENERAL_ERROR;
    }

    // 检查令牌
    pstTokenInfo = slot_data->token_data->token_info;
    if (NULL == pstTokenInfo)
    {
		SYNO_LogE("pstTokenInfo is NULL!");
        return CKR_GENERAL_ERROR;
    }

    // 检查PIN
    if ((ulPinLen < pstTokenInfo->ulMinPinLen)
        || (ulPinLen > pstTokenInfo->ulMaxPinLen))
    {
		SYNO_LogE("ulPinLen is wrong! ulSOPinLen = 0x%x", ulPinLen);
        return CKR_GENERAL_ERROR;
    }

    hDev = slot_data->pDevHandle;
	
	// 设备认证(这步将来必须省略掉)
    if (!P11_ExterAuth(hDev, NULL, 0))
    {
		SYNO_LogE("P11_DevAuth() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

    // 枚举应用
    ulRet = P11_EnumApplication(hDev, acApp, &ulSize);
    if (ulRet != CKR_OK)
    {
		SYNO_LogE("P11_EnumApplication() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

	// 删除所有应用
	TOK_I_StrFmtList(acAppList, acApp);
	pcAppNameList = (char*)SYNO_malloc(strlen((char *)acAppList) + 1);
	strcpy(pcAppNameList, acAppList);
	pcAppName = TC_strtok_r((char *)pcAppNameList, ",", &pcRemainder);
	while (pcAppName && strlen(pcAppName) > 0)
	{
		SYNO_LogD("Try to delete application:%s", pcAppName);
		ulRet = P11_DeleteApplication(hDev, pcAppName);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("P11_DeleteApplication() failed! ulRet = 0x%x", ulRet);
			goto End;
		}
		pcAppName = TC_strtok_r(NULL, ",", &pcRemainder);
	}

	// 创建新应用
	ulRet = P11_CreateApplication(hDev, TOK_INIT_APPNAME, (char*)pPin, TOK_SO_PIN_RETRY_COUNT, 
		TOK_INIT_USERPIN, TOK_USER_PIN_RETRY_COUNT, SECURE_ANYONE_ACCOUNT, &hApp);
	if (ulRet != CKR_OK)
	{
		SYNO_LogE("P11_CreateApplication() failed! ulRet = 0x%x", ulRet);
		goto End;
	}

	// 设置标签
	ulRet = P11_SetLabel(hDev, (char*)pLabel);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("P11_SetLabel() failed! ulRet = 0x%x", ulRet);
		goto End;
	}

	FUNC_END;

End:
	if (pcAppNameList)
	{
		SYNO_free(pcAppNameList);
		pcAppNameList = NULL;
	}
	if (hApp)
	{
		P11_CloseApplication(hApp);
		hApp = NULL;
	}

    return ulRet;
}
 /***********************************************************************
 *	名称:TOK_FinalizeToken
 *	说明:释放一个Token，并且释放所有相关的内存数据。
 *	参数:slot_data:[IN]:该Token对应的Slot数据
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_FinalizeToken(CK_I_SLOT_DATA_PTR slot_data)
{
	FUNC_BEGIN;

    if (NULL == slot_data)
	{		
		SYNO_LogE("pstSlotData is NULL!");
		return CKR_GENERAL_ERROR;
	}

    if (NULL != slot_data->config_section_name)
    {
        SYNO_free(slot_data->config_section_name);
    }

    if (NULL != slot_data->pDevHandle)
    {
        (void)P11_DisConnectDev(slot_data->pDevHandle);
		slot_data->pDevHandle = NULL;
    }

    if (NULL != slot_data->slot_info)
    {
        SYNO_free(slot_data->slot_info);
    }

    if (NULL != slot_data->token_data)
    {
        if (NULL != slot_data->token_data->token_info)
        {
            SYNO_free(slot_data->token_data->token_info);
        }

        SYNO_free(slot_data->token_data);
    }

    SYNO_free(slot_data);

	FUNC_END;

	return CKR_OK;
}
 /***********************************************************************
 *	名称:TOK_InitPIN
 *	说明:设置出示用户PIN码。
 *	参数:session_data:[IN]:会话数据
 *       pPin:[IN]:初始User PIN
 *       ulPinLen:[IN]:User PIN长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_InitPIN(CK_I_SESSION_DATA_PTR session_data,
				  CK_CHAR_PTR pPin, 
				  CK_ULONG ulPinLen)
{
    CK_RV ulRet = CKR_OK;
    CK_ULONG ulRetryCount = 0;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;

	FUNC_BEGIN;

    // 参数检查
    if ((NULL == session_data) || (NULL == pPin))
    {
        SYNO_LogE("session_data or pPin is null!");
        return CKR_GENERAL_ERROR;
    }

	// 修改User PIN。由于TOK_InitPIN()是设置初始User PIN，所以这个时候的Old User PIN只能是TOK_INIT_USERPIN
    ulRet = P11_ChangePIN (session_data->AppHandle, KT_USERPIN, TOK_INIT_USERPIN, (char*)pPin, &ulRetryCount);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_ChangePIN() failed! ulRet = 0x%x", ulRet);
		if (SAR_COS_PIN_LOCKED == ulRet || 0 == ulRetryCount)
		{
            return CKR_PIN_LOCKED;
		}

		return CKR_PIN_INCORRECT;
    }
    
	FUNC_END;

	return CKR_OK;
}
 /***********************************************************************
 *	名称:TOK_SetPIN
 *	说明:修改当前登录用户的PIN码
 *	参数:session_data:[IN]:会话数据
 *       pOldPin:[IN]:老PIN码
 *       ulOldLen:[IN]:老PIN码长度
 *       pNewPin:[IN]:新PIN码
 *       ulNewLen:[IN]:新PIN码长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_SetPIN(CK_I_SESSION_DATA_PTR session_data,
				 CK_CHAR_PTR pOldPin, 
				 CK_ULONG ulOldLen, 
				 CK_CHAR_PTR pNewPin, 
				 CK_ULONG ulNewLen)
{
    CK_RV ulRet = CKR_OK;
	CK_ULONG ulUserType = 0;
    CK_ULONG ulRetryCount = 0;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
	
	FUNC_BEGIN;
	
    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
        return CKR_GENERAL_ERROR;
    }
    if ((NULL == pOldPin) || (NULL == pNewPin))
    {
        SYNO_LogE("pucOldPin or pucNewPin is null!");
        return CKR_GENERAL_ERROR;
    }

	// 判断当前登录的类型
	switch (session_data->session_info->state)
	{
	case CKS_RW_USER_FUNCTIONS:
		ulUserType = KT_USERPIN;
		break;
	case CKS_RW_SO_FUNCTIONS:
		ulUserType = KT_SOPIN;
		break;
	default:
        SYNO_LogE("Current user is not login!");
		return CKR_USER_NOT_LOGGED_IN;
	}

    // 修改用户PIN的值
    ulRet = P11_ChangePIN(session_data->AppHandle, ulUserType, (char*)pOldPin, (char*)pNewPin, &ulRetryCount);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_ChangePIN() failed! ulRet = 0x%x", ulRet);
		if (SAR_COS_PIN_LOCKED == ulRet || 0 == ulRetryCount)
		{
            return CKR_PIN_LOCKED;
		}

        return CKR_PIN_INCORRECT;
    }

	FUNC_END;

    return CKR_OK;
}
 /***********************************************************************
 *	名称:TOK_OpenSession
 *	说明:打开会话
 *	参数:slot_data:[IN]:会话数据指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_OpenSession(CK_I_SESSION_DATA_PTR session_data)
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
	ContainerInfoEx stContainerInfo = {0};
    ULONG ulObjCount = 0;

	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
		SYNO_LogE("session_data is NULL!");
        return CKR_GENERAL_ERROR;
    }
    pstSlotData = session_data->slot_data;
    if (NULL == pstSlotData)
    {
		SYNO_LogE("pstSlotData is NULL!");
        return CKR_GENERAL_ERROR;
    }

    hDev = pstSlotData->pDevHandle;

    // 枚举应用
    ulRet = P11_EnumApplication(hDev, acName, &ulSize);
    if (ulRet != CKR_OK)
    {
		SYNO_LogE("P11_EnumApplication() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }
    if (acName[0] == '\0')
    {
		SYNO_LogE("No any application!");
        return CKR_GENERAL_ERROR;
    }
	
    // 假设只有一个应用,打开应用
	SYNO_LogD("Use applicaiton: %s", acName);
    ulRet = P11_OpenApplication(hDev, acName, &hApp);
    if(ulRet != CKR_OK)
    {
		SYNO_LogE("P11_OpenApplication() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }
	session_data->AppHandle = hApp;

	// 枚举应用下的Data file
	TOK_I_EnumDataFileObj(session_data, hApp, acName);
		
    // 枚举容器
	ulSize = 512;
    ulRet = P11_EnumContainer(hApp, acContainerList, &ulSize);
    if (ulRet != CKR_OK)
    {
		SYNO_LogE("P11_OpenApplication() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }
    if (acName[0] == '\0' || ulSize <= 2)
    {
		SYNO_LogD("Not any containers!");
        return CKR_OK;
    }

	// 字符串序列转换
	pcList = (CHAR*)SYNO_calloc(1, ulSize);
	memset(pcList, 0, ulSize);
	TOK_I_StrFmtList(pcList, acContainerList);

	// 枚举每个容器中的对象	
    session_data->CtrCount = 0;
    pcContainerName = TC_strtok_r(pcList, ",", &pcRemainder);
    while (pcContainerName && strlen(pcContainerName) > 0)
    {
		SYNO_LogD("Try to open container: %s", pcContainerName);

		// 获取容器属性
		ulRet = P11_GetContainerInfoEx(hApp, pcContainerName, &stContainerInfo);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_GetContainerInfoEx() failed! ulRet = 0x%x", ulRet);
			goto NEXT;
		}

		// 忽略ECC容器
		if (2 == stContainerInfo.type)
		{
			SYNO_LogD("This is an ECC container, skip it!");
			goto NEXT;
		}

        // 打开指定容器
        ulRet = P11_OpenContainer(hApp, pcContainerName, &hContainer);
        if (SAR_OK == ulRet)
        {
			ulObjCount = 0;
			ulObjCount = TOK_I_EnumPubKeyObj(session_data, hContainer, pcContainerName, &stContainerInfo);
			ulObjCount += TOK_I_EnumPriKeyObj(session_data, hContainer, pcContainerName, &stContainerInfo);
			ulObjCount += TOK_I_EnumCertObj(session_data, hContainer, pcContainerName, &stContainerInfo);
			if (0 == ulObjCount) 
			{
				SYNO_LogD("Not any objects in this container, close it!");
				P11_CloseContainer(hContainer);
				ulObjCount = NULL;
			}
			else
			{
				session_data->CtrCount++;
				SYNO_LogD("There are %d objects in this container!", ulObjCount);
			}
        }
		else
		{
			SYNO_LogE("P11_OpenContainer() failed! ulRet = 0x%x", ulRet);
		}

NEXT:
		pcContainerName = TC_strtok_r(NULL, ",", &pcRemainder);
	}
	
	FUNC_END;

	SYNO_free(pcList);
	pcList = NULL;

    return ulRet;
}
 /***********************************************************************
 *	名称:TOK_CloseSession
 *	说明:关闭会话
 *	参数:slot_data:[IN]:会话数据
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_CloseSession(CK_I_SESSION_DATA_PTR session_data)
{
    ULONG ulRet = CKR_OK;
    ULONG ulLen = TOK_ONCE_RW_LEN;
    UCHAR pucBuff[TOK_ONCE_RW_LEN] = {0};
    HCONTAINER hContainer = NULL;
    ULONG i = 0;

	FUNC_BEGIN;

	// 检查是否还在进行加密操作
    if (NULL != session_data->encrypt_state)
    {
        ulRet = TOK_EncryptFinal(session_data, pucBuff, &ulLen);
        if (ulRet != CKR_OK)
        {
			SYNO_LogE("TOK_EncryptFinal() failed! ulRet = 0x%x", ulRet);
            return ulRet;
        }
    }
	
	// 检查是否还在进行解密操作
    if (NULL != session_data->decrypt_state)
    {
        ulRet = TOK_DecryptFinal(session_data, pucBuff, &ulLen);
        if (ulRet != CKR_OK)
        {
			SYNO_LogE("TOK_DecryptFinal() failed! ulRet = 0x%x", ulRet);
            return ulRet;
        }
    }

	// 关闭容器句柄
    for (i = 0; i < session_data->CtrCount; i++)
    {
        hContainer = (HCONTAINER)session_data->aCtrHandle[i];
        (void)P11_CloseContainer(hContainer);
		session_data->aCtrHandle[i] = NULL;
    }

    // 关闭应用
    ulRet = P11_CloseApplication(session_data->AppHandle);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_CloseApplication() failed! ulRet = 0x%x", ulRet);
        return ulRet;
    }

    return CKR_OK;
}
 /***********************************************************************
 *	名称:TOK_GetOperationState
 *	说明:返回操作状态
 *	参数:session_data:[IN]:会话数据
 ＊　　　pOperationState:[OUT]:存放返回状态的指针
 ＊　　　pulOperationStateLen:[OUT]:存放返回状态数据长度的指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_GetOperationState(CK_I_SESSION_DATA_PTR session_data, 
							CK_BYTE_PTR pOperationState, 
							CK_ULONG_PTR pulOperationStateLen)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	名称:TOK_SetOperationState
 *	说明:设置操作状态
 *	参数:session_data:[IN]:会话数据
 ＊　　　pOperationState:[IN]:存放状态的Buffer
 ＊　　　ulOperationStateLen:[IN]:Buffer长度
 ＊　　　encrypt_key_obj:[IN]:加密Key对象指针
 ＊　　　auth_key_obj:[IN]:验证Key对象指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_SetOperationState(CK_I_SESSION_DATA_PTR session_data, 
							CK_BYTE_PTR pOperationState, 
							CK_ULONG ulOperationStateLen, 
							CK_I_OBJ_PTR encrypt_key_obj, 
							CK_I_OBJ_PTR auth_key_obj)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	名称:TOK_Login
 *	说明:登录会话
 *	参数:session_data:[IN]:会话数据
 ＊　　　userType:[IN]:用户类型，CKU_SO或者CKU_USER。
 ＊　　　pPin:[IN]:PIN码
 ＊　　　ulPinLen:[IN]:PIN码长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_Login(CK_I_SESSION_DATA_PTR session_data,
				CK_USER_TYPE userType, 
				CK_CHAR_PTR pPin,
				CK_ULONG ulPinLen)
{
    ULONG ulRet = CKR_OK;
	CHAR acName[PATH_MAX] = {0};
	ULONG ulSize = PATH_MAX;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    ULONG ulPINType = 0;
    ULONG ulRetryCount = 0;

    FUNC_BEGIN;

    // 参数检查
    if ((NULL == session_data) || (NULL == pPin))
    {
		SYNO_LogE("session_data or pPin is NULL!");
        return CKR_GENERAL_ERROR;
    }

    pstSlotData = session_data->slot_data;
    if (NULL == pstSlotData)
    {
        SYNO_LogE("pstSlotData is null!");
        return CKR_GENERAL_ERROR;
    }

    // 转换登录类型
    if (CKU_SO == userType)
    {
        ulPINType = KT_SOPIN;
    }
    else
    {
        ulPINType = KT_USERPIN;
    }

    // 校验PIN码
    ulRet = P11_VerifyPIN(session_data->AppHandle, ulPINType, (char*)pPin, &ulRetryCount);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_VerifyPIN() failed! ulRet = 0x%x", ulRet);
		if (SAR_COS_PIN_LOCKED == ulRet || 0 == ulRetryCount)
		{
            return CKR_PIN_LOCKED;
		}

		return CKR_PIN_INCORRECT;
    }

	FUNC_END;

    return ulRet;
}
 /***********************************************************************
 *	名称:TOK_Logout
 *	说明:登出会话
 *	参数:session_data:[IN]:会话数据
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_Logout(CK_I_SESSION_DATA_PTR session_data)
{
    ULONG ulRet = CKR_OK;

	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
        return CKR_GENERAL_ERROR;
    }

    // 清除应用当前的安全状态
    ulRet = P11_ClearSecureState(session_data->AppHandle);
    if (CKR_OK != ulRet)
    {
        SYNO_LogE("P11_ClearSecureState() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

	FUNC_END;

    return CKR_OK;
}
 /***********************************************************************
 *	名称:TOK_EncryptInit
 *	说明:加密初始化
 *	参数:session_data:[IN]:会话数据
 *       pMechanism:[IN]:密钥算法
 *       key_obj:[IN]:加密用的密钥对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_EncryptInit(CK_I_SESSION_DATA_PTR session_data,
					  CK_MECHANISM_PTR pMechanism, 
					  CK_I_OBJ_PTR key_obj)
{
    ULONG ulRet = CKR_OK;
	ULONG ulAlgId = 0;
    BLOCKCIPHERPARAM stEncryptParam = {0};

	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == key_obj)
    {
        SYNO_LogE("key_obj is null!");
        return CKR_ARGUMENTS_BAD;
    }

	// 解析加密算法
	switch (pMechanism->mechanism)
	{
	case CKM_DES_ECB:
		ulAlgId = SGD_DES_ECB;
		break;
	case CKM_DES_CBC:
		ulAlgId = SGD_DES_CBC;
		break;
	case CKM_DES3_ECB:
		ulAlgId = SGD_DES3_ECB;
		break;
	case CKM_DES3_CBC:
		ulAlgId = SGD_DES3_CBC;
		break;
	case CKM_AES_ECB:
		ulAlgId = SGD_AES128_ECB;
		break;
	case CKM_AES_CBC:
		ulAlgId = SGD_AES128_CBC;
		break;
	default:
	case CKM_RSA_PKCS:
		ulAlgId = SGD_RSA;
		break;
	}
	if (0 == ulAlgId)
	{
        SYNO_LogE("Key algId in mechanism is wrong! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	SYNO_LogD("pMechanism->mechanism = 0x%x, the according SKF alg id: 0x%x", pMechanism->mechanism, ulAlgId);

	// 解析加密参数，如果有
	if (SGD_AES128_CBC == ulAlgId || CKM_DES_CBC == ulAlgId || CKM_DES3_CBC == ulAlgId)
	{
		if (!pMechanism->pParameter)
		{
			SYNO_LogE("pMechanism->pParameter is NULL! It should be IV!");
			return CKR_MECHANISM_PARAM_INVALID;
		}
		if (0 == pMechanism->ulParameterLen || pMechanism->ulParameterLen > 32)
		{
			SYNO_LogE("pMechanism->ulParameterLen is wrong! pMechanism->ulParameterLen = 0x%x", pMechanism->ulParameterLen);
			return CKR_MECHANISM_PARAM_INVALID;
		}
		memcpy(stEncryptParam.IV, pMechanism->pParameter, pMechanism->ulParameterLen);
	}
	
    // 数据加密初始化,设置数据加密的算法相关参数
	if (SGD_RSA != ulAlgId)
	{
		stEncryptParam.PaddingType = 1;		//自动补齐
		ulRet = P11_EncryptInit(key_obj->ObjHandle, ulAlgId, stEncryptParam);
		if (CKR_OK != ulRet)
		{
			SYNO_LogE("P11_EncryptInit() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
		session_data->encrypt_state = key_obj->ObjHandle;
	}
	else
	{
		session_data->encrypt_state = key_obj;
	}

	session_data->encrypt_mechanism = pMechanism->mechanism;

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	名称:TOK_Encrypt
 *	说明:单组加密数据
 *	参数:session_data:[IN]:会话数据
 *       pData:[IN]:明文数据
 *       ulDataLen:[IN]:明文数据长度
 *       pEncryptedData:[OUT]:返回的密文数据
 *       pulEncryptedDataLen:[OUT]:返回的密文数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_Encrypt(CK_I_SESSION_DATA_PTR session_data,
				  CK_BYTE_PTR pData, 
				  CK_ULONG ulDataLen, 
				  CK_BYTE_PTR pEncryptedData, 
				  CK_ULONG_PTR pulEncryptedDataLen)
{   
	ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;

	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pData || 0 == ulDataLen)
    {
        SYNO_LogE("pucData is null or ulDataLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulEncryptedDataLen)
    {
        SYNO_LogE("pulEncryptedPartLen is null!");
		return CKR_ARGUMENTS_BAD;
    }
	
	// 不支持补齐
	//if (ulDataLen % 16)
	//{
 //       SYNO_LogE("ulDataLen is not Nx16!");
	//	return CKR_ARGUMENTS_BAD;
	//}

    // 得到加密句柄
    hKey = session_data->encrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("hKey is null!");
        return CKR_GENERAL_ERROR;
    }

	if (CKM_RSA_PKCS == session_data->encrypt_mechanism)
	{
		// RSA公钥加密数据		
		ulRet = P11_ExtRSAEnDecrypt(((CK_I_OBJ_PTR)hKey)->CtrHandle, FALSE, TRUE, TRUE, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_ExtRSAEnDecrypt() failed! ulRet = 0x%x", ulRet);
			ulRet = CKR_GENERAL_ERROR;
			goto ERR;
		}
	}
	else
	{
		// 对称密钥加密数据
		ulRet = P11_Encrypt(hKey, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_Encrypt() failed! ulRet = 0x%x", ulRet);
			ulRet = CKR_GENERAL_ERROR;
			goto ERR;
		}
	}
	// 直接返回长度
	if (NULL == pEncryptedData)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulEncryptedDataLen);
		return CKR_OK;
	}
	

	FUNC_END;

	// 加密结束
ERR:
	session_data->encrypt_state = NULL;
	session_data->encrypt_mechanism = 0;

    return ulRet;
}
/***********************************************************************
 *	名称:TOK_EncryptUpdate
 *	说明:多组加密数据
 *	参数:session_data:[IN]:会话数据
 *       pPart:[IN]:明文分组数据
 *       ulPartLen:[IN]:明文分组数据长度
 *       pEncryptedPart:[OUT]:返回的密文数据
 *       pulEncryptedPartLen:[OUT]:返回的密文数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_EncryptUpdate(CK_I_SESSION_DATA_PTR session_data,
						CK_BYTE_PTR pPart, 
						CK_ULONG ulPartLen, 
						CK_BYTE_PTR pEncryptedPart, 
						CK_ULONG_PTR pulEncryptedPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;

	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pPart || 0 == ulPartLen)
    {
        SYNO_LogE("pPart is null or ulPartLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulEncryptedPartLen)
    {
        SYNO_LogE("pulEncryptedPartLen is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// 不支持补齐
	//if (ulPartLen % 16)
	//{
 //       SYNO_LogE("ulPartLen is not Nx16!");
	//	return CKR_ARGUMENTS_BAD;
	//}

    // 得到加密句柄
    hKey = session_data->encrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("hKey is null!");
        return CKR_GENERAL_ERROR;
    }
	
	if (CKM_RSA_PKCS == session_data->encrypt_mechanism)
	{
		// RSA公钥加密数据		
		ulRet = P11_ExtRSAEnDecrypt(((CK_I_OBJ_PTR)hKey)->CtrHandle, FALSE, TRUE, TRUE, pPart, ulPartLen, pEncryptedPart, pulEncryptedPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_ExtRSAEnDecrypt() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	else
	{
		// 对称密钥多组加密数据
		ulRet = P11_EncryptUpdate(hKey, pPart, ulPartLen, pEncryptedPart, pulEncryptedPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_EncryptUpdate() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	// 直接返回长度
	if (NULL == pEncryptedPart)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulEncryptedPartLen);
		return CKR_OK;
	}

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	名称:TOK_EncryptFinal
 *	说明:多组加密结束
 *	参数:session_data:[IN]:会话数据
 *       pLastEncryptedPart:[OUT]:返回余留的密文数据
 *       pulLastEncryptedPartLen:[OUT]:返回余留的密文数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_EncryptFinal(CK_I_SESSION_DATA_PTR session_data,
					   CK_BYTE_PTR pLastEncryptedPart, 
					   CK_ULONG_PTR pulLastEncryptedPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;
	
	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulLastEncryptedPartLen)
    {
        SYNO_LogE("pulLastEncryptedPartLen is null!");
		return CKR_ARGUMENTS_BAD;
    }

    // 得到加密句柄
    hKey = session_data->encrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("slot_data->encrypt_state is null!");
        return CKR_GENERAL_ERROR;
    }
	
	if (CKM_RSA_PKCS == session_data->encrypt_mechanism)
	{
		//TBD: ...
	}
	else
	{
		// 结束多个分组数据的加密,返回剩余加密结果
		ulRet = P11_EncryptFinal(hKey, pLastEncryptedPart, pulLastEncryptedPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_EncryptFinal() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	// 直接返回长度
	if (NULL == pLastEncryptedPart)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulLastEncryptedPartLen);
		return CKR_OK;
	}
	
	// 加密结束
    session_data->encrypt_state = NULL;
	session_data->encrypt_mechanism = 0;

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	名称:TOK_DecryptInit
 *	说明:解密初始化
 *	参数:session_data:[IN]:会话数据
 *       pMechanism:[IN]:密钥算法
 *       key_obj:[IN]:解密用的密钥对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_DecryptInit(CK_I_SESSION_DATA_PTR session_data,
					  CK_MECHANISM_PTR pMechanism,
					  CK_I_OBJ_PTR pstKeyObj)
{
    ULONG ulRet = CKR_OK;
	ULONG ulAlgId = 0;
    BLOCKCIPHERPARAM stDecryptParam = {0};
	
	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pstKeyObj)
    {
        SYNO_LogE("pstKeyObj is null!");
        return CKR_ARGUMENTS_BAD;
    }

	// 解析解密算法
	switch (pMechanism->mechanism)
	{
	case CKM_DES_ECB:
		ulAlgId = SGD_DES_ECB;
		break;
	case CKM_DES_CBC:
		ulAlgId = SGD_DES_CBC;
		break;
	case CKM_DES3_ECB:
		ulAlgId = SGD_DES3_ECB;
		break;
	case CKM_DES3_CBC:
		ulAlgId = SGD_DES3_CBC;
		break;
	case CKM_AES_ECB:
		ulAlgId = SGD_AES128_ECB;
		break;
	case CKM_AES_CBC:
		ulAlgId = SGD_AES128_CBC;
		break;
	case CKM_RSA_PKCS:
		ulAlgId = SGD_RSA;
		break;
	default:
		break;
	}
	if (0 == ulAlgId)
	{
        SYNO_LogE("Key algId in mechanism is wrong! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	SYNO_LogD("pMechanism->mechanism = 0x%x, the according SKF alg id: 0x%x", pMechanism->mechanism, ulAlgId);

	// 解析解密参数，如果有
	if (SGD_AES128_CBC == ulAlgId || CKM_DES_CBC == ulAlgId || CKM_DES3_CBC == ulAlgId)
	{
		if (!pMechanism->pParameter)
		{
			SYNO_LogE("pMechanism->pParameter is NULL! It should be IV!");
			return CKR_MECHANISM_PARAM_INVALID;
		}
		if (0 == pMechanism->ulParameterLen || pMechanism->ulParameterLen > 32)
		{
			SYNO_LogE("pMechanism->ulParameterLen is wrong! pMechanism->ulParameterLen = 0x%x", pMechanism->ulParameterLen);
			return CKR_MECHANISM_PARAM_INVALID;
		}
		memcpy(stDecryptParam.IV, pMechanism->pParameter, pMechanism->ulParameterLen);
	}
	
	// 解密初始化
	if (SGD_RSA != ulAlgId)
	{
		stDecryptParam.PaddingType = 1;		//自动去掉补码
		ulRet = P11_DecryptInit(pstKeyObj->ObjHandle, ulAlgId, stDecryptParam);
		if (CKR_OK != ulRet)
		{
			SYNO_LogE("P11_DecryptInit() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
		session_data->decrypt_state = pstKeyObj->ObjHandle;
	}
	else
	{
		session_data->decrypt_state = pstKeyObj;
	}

	session_data->decrypt_mechanism = pMechanism->mechanism;

	FUNC_END;

    return ulRet;

}
/***********************************************************************
 *	名称:TOK_Decrypt
 *	说明:单组解密数据
 *	参数:session_data:[IN]:会话数据
 *       pEncryptedData:[IN]:密文数据
 *       ulEncryptedDataLen:[IN]:密文数据长度
 *       pData:[OUT]:返回的明文数据
 *       pulDataLen:[OUT]:返回的明文数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_Decrypt(CK_I_SESSION_DATA_PTR session_data,
				  CK_BYTE_PTR pEncryptedData, 
				  CK_ULONG ulEncryptedDataLen, 
				  CK_BYTE_PTR pData, 
				  CK_ULONG_PTR pulDataLen)
{
	ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;
	
	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pEncryptedData || 0 == ulEncryptedDataLen)
    {
        SYNO_LogE("pEncryptedData is null or ulEncryptedDataLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulDataLen)
    {
        SYNO_LogE("pulDataLen is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// 不支持补齐
	//if (ulEncryptedDataLen % 16)
	//{
 //       SYNO_LogE("ulEncryptedDataLen is not Nx16!");
	//	return CKR_ARGUMENTS_BAD;
	//}
	
    // 得到加密句柄
    hKey = session_data->decrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("hKey is null!");
        return CKR_GENERAL_ERROR;
    }
	
	if (CKM_RSA_PKCS == session_data->decrypt_mechanism)
	{
		// RSA私钥解密数据		
		ulRet = P11_ExtRSAEnDecrypt(((CK_I_OBJ_PTR)hKey)->CtrHandle, FALSE, FALSE, FALSE, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_ExtRSAEnDecrypt() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	else
	{
		// 单组数据解密
		ulRet = P11_Decrypt(hKey, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_Decrypt() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	// 直接返回长度
	if (NULL == pData)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulDataLen);
		return CKR_OK;
	}

	// 解密结束
    session_data->decrypt_state = NULL;
	session_data->decrypt_mechanism = 0;

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	名称:TOK_DecryptUpdate
 *	说明:多组解密数据
 *	参数:session_data:[IN]:会话数据
 *       pEncryptedPart:[IN]:密文分组数据
 *       ulEncryptedPartLen:[IN]:密文分组数据长度
 *       pPart:[OUT]:返回的明文数据
 *       pulPartLen:[OUT]:返回的明文数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_DecryptUpdate(CK_I_SESSION_DATA_PTR session_data,
						CK_BYTE_PTR pEncryptedPart, 
						CK_ULONG ulEncryptedPartLen, 
						CK_BYTE_PTR pPart, 
						CK_ULONG_PTR pulPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;
	
	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pEncryptedPart || 0 == ulEncryptedPartLen)
    {
        SYNO_LogE("pEncryptedPart is null or ulEncryptedPartLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulPartLen)
    {
        SYNO_LogE("pulPartLen is null!");
		return CKR_ARGUMENTS_BAD;
    }
	
	// 不支持补齐
	//if (ulEncryptedPartLen % 16)
	//{
 //       SYNO_LogE("ulEncryptedDataLen is not Nx16!");
	//	return CKR_ARGUMENTS_BAD;
	//}

    // 得到加密句柄
    hKey = session_data->decrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("hKey is null!");
        return CKR_GENERAL_ERROR;
    }
	
	if (CKM_RSA_PKCS == session_data->decrypt_mechanism)
	{
		// RSA私钥解密数据		
		ulRet = P11_ExtRSAEnDecrypt(((CK_I_OBJ_PTR)hKey)->CtrHandle, FALSE, FALSE, FALSE, pEncryptedPart, ulEncryptedPartLen, pPart, pulPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_ExtRSAEnDecrypt() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	else
	{
		// 多组数据解密
		ulRet = P11_DecryptUpdate(hKey, pEncryptedPart, ulEncryptedPartLen, pPart, pulPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_DecryptUpdate() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	// 直接返回长度
	if (NULL == pPart)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulPartLen);
		return CKR_OK;
	}

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	名称:TOK_DecryptFinal
 *	说明:解密结束
 *	参数:session_data:[IN]:会话数据
 *       pLastEncryptedPart:[OUT]:返回余留的明文数据
 *       pulLastEncryptedPartLen:[OUT]:返回余留的明文数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_DecryptFinal(CK_I_SESSION_DATA_PTR session_data,
					   CK_BYTE_PTR pLastPart, 
					   CK_ULONG_PTR pulLastPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;

	FUNC_BEGIN;

	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulLastPartLen)
    {
        SYNO_LogE("pulLastPartLen is null!");
		return CKR_ARGUMENTS_BAD;
    }

    // 得到加密句柄
    hKey = session_data->decrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("hKey is null!");
        return CKR_GENERAL_ERROR;
    }
	
	if (CKM_RSA_PKCS == session_data->encrypt_mechanism)
	{
	}
	else
	{
		// 结束多个分组数据的解密,返回剩余解密结果
		ulRet = P11_DecryptFinal(hKey, pLastPart, pulLastPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_DecryptFinal() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	// 直接返回长度
	if (NULL == pLastPart)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulLastPartLen);
		return CKR_OK;
	}

	// 解密结束
    session_data->decrypt_state = NULL;
	session_data->decrypt_mechanism = 0;
	
	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	名称:TOK_DigestInit
 *	说明:杂凑初始化
 *	参数:session_data:[IN]:会话数据
 *       pMechanism:[IN]:杂凑算法
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_DigestInit(CK_I_SESSION_DATA_PTR session_data, 
					 CK_MECHANISM_PTR pMechanism)
{
	ULONG ulRet = CKR_OK;
	ULONG ulAlgId = 0;
	DEVHANDLE hDev = NULL;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// 设备句柄
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
        return CKR_GENERAL_ERROR;
	}

	// 检查算法
	switch(pMechanism->mechanism)
	{
	case CKM_MD5:
		ulAlgId = SGD_MD5;
		break;
	case CKM_SHA_1:
		ulAlgId = SGD_SHA1;
		break;
	case CKM_SHA256:
		ulAlgId = SGD_SHA256;
		break;
	}
	if (0 == ulAlgId)
	{
        SYNO_LogE("Digest algId in mechanism is wrong! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	SYNO_LogD("pMechanism->mechanism = 0x%x, the according SKF alg id: 0x%x", pMechanism->mechanism, ulAlgId);

	// 杂凑初始化
	ulRet = P11_DigestInit(hDev, ulAlgId, NULL, NULL, 0, &hDigestKey);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestInit() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

	// 保存句柄
	session_data->digest_state = hDigestKey;
	session_data->digest_mechanism = pMechanism->mechanism;

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_Digest
 *	说明:单组数据杂凑运算
 *	参数:session_data:[IN]:会话数据
 *       pData:[IN]:原始数据
 *       ulDataLen:[IN]:原始数据长度
 *       pDigest:[IN]:返回的杂凑数据
 *       pulDigestLen:[IN]:返回的杂凑数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_Digest(CK_I_SESSION_DATA_PTR session_data, 
				 CK_BYTE_PTR pData, 
				 CK_ULONG ulDataLen, 
				 CK_BYTE_PTR pDigest, 
				 CK_ULONG_PTR pulDigestLen)
{
	ULONG ulRet = CKR_OK;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pData || 0 == ulDataLen)
    {
        SYNO_LogE("pData is null or ulDataLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
	if (pulDigestLen)
	{
        SYNO_LogE("pulDigestLen is null!");
		return CKR_ARGUMENTS_BAD;
	}

	// 杂凑句柄
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
        return CKR_GENERAL_ERROR;
	}

	// 杂凑运算
	ulRet = P11_Digest(hDigestKey, pData, ulDataLen, pDigest, pulDigestLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_Digest() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

	// 返回长度
	if (NULL == pDigest)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulDigestLen);
		return CKR_OK;		
	}

	// 杂凑结束
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	P11_CloseHandle(hDigestKey);
	hDigestKey = NULL;

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_DigestUpdate
 *	说明:多组数据杂凑运算
 *	参数:session_data:[IN]:会话数据
 *       pPart:[IN]:分组原始数据
 *       ulPartLen:[IN]:分组原始数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_DigestUpdate(CK_I_SESSION_DATA_PTR session_data, 
					   CK_C_BYTE_PTR pPart, 
					   CK_ULONG ulPartLen)
{
	ULONG ulRet = CKR_OK;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pPart || 0 == ulPartLen)
    {
        SYNO_LogE("pPart is null or ulPartLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }

	// 杂凑句柄
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
        return CKR_GENERAL_ERROR;
	}

	// 杂凑运算
	ulRet = P11_DigestUpdate(hDigestKey, (BYTE*)pPart, ulPartLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestUpdate() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }
	
	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_DigestKey
 *	说明:杂凑密钥运算
 *	参数:session_data:[IN]:会话数据
 *       key_obj:[IN]:密钥对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_DigestKey(CK_I_SESSION_DATA_PTR  session_data, 
					CK_I_OBJ_PTR key_obj)
{
	return CKR_FUNCTION_FAILED;
}
/***********************************************************************
 *	名称:TOK_DigestUpdate
 *	说明:杂凑结束
 *	参数:session_data:[IN]:会话对象
 *       pDigest:[OUT]:返回的杂凑值
 *       pulDigestLen:[OUT]:杂凑值长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_DigestFinal(CK_I_SESSION_DATA_PTR session_data,  
					  BYTE* pDigest, 
					  ULONG* pulDigestLen)
{
	ULONG ulRet = CKR_OK;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulDigestLen)
    {
        SYNO_LogE("pulDigestLen is NULL!");
		return CKR_ARGUMENTS_BAD;
    }

	// 杂凑句柄
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
        return CKR_GENERAL_ERROR;
	}

	// 杂凑运算
	ulRet = P11_DigestFinal(hDigestKey, pDigest, pulDigestLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestFinal() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }
	
	// 返回长度
	if (NULL == pDigest)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulDigestLen);
		return CKR_OK;		
	}

	// 杂凑结束
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	P11_CloseHandle(hDigestKey);
	hDigestKey = NULL;

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_SignInit
 *	说明:签名初始化
 *	参数:session_data:[IN]:会话对象
 *       pMechanism:[OUT]:签名算法
 *       key_obj:[OUT]:签名密钥对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_SignInit(CK_I_SESSION_DATA_PTR session_data, 
				   CK_MECHANISM_PTR pMechanism, 
				   CK_I_OBJ_PTR key_obj)
{
	ULONG ulRet = CKR_OK;
	ULONG ulAlgId = 0;
	DEVHANDLE hDev = NULL;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == key_obj)
    {
        SYNO_LogE("key_obj is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// 设备句柄
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
        return CKR_GENERAL_ERROR;
	}

	// 检查算法
	switch(pMechanism->mechanism)
	{
	case CKM_RSA_PKCS:
		break;
	case CKM_MD5_RSA_PKCS:
		ulAlgId = SGD_MD5;
		break;
	case CKM_SHA1_RSA_PKCS:
		ulAlgId = SGD_SHA1;
		break;
	case CKM_SHA256_RSA_PKCS:
		ulAlgId = SGD_SHA256;
		break;
	default:
        SYNO_LogE("Digest algId in mechanism is wrong! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	// 判断是否需要做杂凑运算
	if (CKM_RSA_PKCS == pMechanism->mechanism)
	{
		SYNO_LogD("pMechanism->mechanism = 0x%x, don't need to do digest!");
		session_data->digest_state = NULL;
	}
	else
	{
		SYNO_LogD("pMechanism->mechanism = 0x%x, the according SKF digest alg id: 0x%x", pMechanism->mechanism, ulAlgId);

		// 杂凑初始化
		ulRet = P11_DigestInit(hDev, ulAlgId, NULL, NULL, 0, &hDigestKey);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_DigestInit() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}

		// 保存句柄
		session_data->digest_state = hDigestKey;
	}

	session_data->sign_mechanism = pMechanism->mechanism;
	session_data->sign_state = key_obj;

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_Sign
 *	说明:签名单组数据
 *	参数:session_data:[IN]:会话对象
 *       pData:[IN]:待签名的原文数据
 *       ulDataLen:[IN]:原文数据长度
 *       pSignature:[OUT]:返回的签名值
 *       pulSignatureLen:[OUT]:签名值长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_Sign(CK_I_SESSION_DATA_PTR session_data, 
			    CK_BYTE_PTR pData, 
				CK_ULONG ulDataLen, 
				CK_BYTE_PTR pSignature, 
				CK_ULONG_PTR pulSignatureLen)
{
	ULONG ulRet = CKR_OK;
	ULONG ulModulusBits = 0;
	ULONG ulDigestLen = 256;
	BYTE btDigestData[256] = {0};
	HANDLE hDigestKey = NULL;
	HCONTAINER hContainer = NULL;
	CK_I_OBJ_PTR pstPriKeyObj = NULL;
	CK_ATTRIBUTE_PTR pstPriAttr = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pData || 0 == ulDataLen)
    {
        SYNO_LogE("pData is null or ulDataLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
	if (!pulSignatureLen)
	{
        SYNO_LogE("pulSignatureLen is null!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// 获取签名私钥所在容器
	pstPriKeyObj = (CK_I_OBJ_PTR)session_data->sign_state;
	if (NULL == pstPriKeyObj)
	{
        SYNO_LogE("pstPriKeyObj is null!");
		return CKR_GENERAL_ERROR;
	}
	hContainer = pstPriKeyObj->CtrHandle;
	if (NULL == hContainer)
	{
        SYNO_LogE("hContainer is null!");
		return CKR_GENERAL_ERROR;
	}

	// 判断是否需要做杂凑运算
	if (CKM_RSA_PKCS == session_data->sign_mechanism)
	{		
		// 判断数据长度
		pstPriAttr = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPriKeyObj, CK_IA_PRIVATE_EXPONENT));
		if (pstPriAttr)
		{
			ulModulusBits = pstPriAttr->ulValueLen * 8;
			if (ulDataLen > (ulModulusBits - 11))
			{
				SYNO_LogE("ulDataLen is wrong! ulDataLen = 0x%x", ulDataLen);
				ulRet = CKR_DATA_LEN_RANGE;
				goto ERR;
			}
		}
		else if (ulDataLen > (256-11))
		{
			SYNO_LogE("ulDataLen is wrong! ulDataLen = 0x%x", ulDataLen);
			ulRet = CKR_DATA_LEN_RANGE;
			goto ERR;
		}

		// 拷贝数据
		memcpy(btDigestData, pData, ulDataLen);
		ulDigestLen = ulDataLen;
	}
	else
	{
		// 杂凑句柄
		hDigestKey = session_data->digest_state;
		if (NULL == hDigestKey)
		{
			SYNO_LogE("hDigestKey is null!");
			ulRet = CKR_GENERAL_ERROR;
			goto ERR;
		}

		// 杂凑运算
		ulRet = P11_Digest(hDigestKey, pData, ulDataLen, btDigestData, &ulDigestLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_Digest() failed! ulRet = 0x%x", ulRet);
			ulRet = CKR_GENERAL_ERROR;
			goto ERR;
		}
	}
	
	// 签名运算
	ulRet = P11_RSASignData(hContainer, btDigestData, ulDigestLen, pSignature, pulSignatureLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_RSASignData() failed! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto ERR;
    }

	// 返回长度
	if (NULL == pSignature)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulSignatureLen);
		return CKR_OK;		
	}
		
	FUNC_END;

	// 签名结束
ERR:
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	session_data->sign_state = NULL;
	session_data->sign_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_SignUpdate
 *	说明:签名分组数据
 *	参数:session_data:[IN]:会话对象
 *       pPart:[IN]:待签名的分组原文数据
 *       ulPartLen:[IN]:分组原文数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_SignUpdate(CK_I_SESSION_DATA_PTR session_data, 
					 CK_BYTE_PTR pPart, 
					 CK_ULONG ulPartLen)
{
	ULONG ulRet = CKR_OK;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pPart || 0 == ulPartLen)
    {
        SYNO_LogE("pPart is null or ulPartLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }

	// 判断算法
	if (CKM_RSA_PKCS == session_data->sign_mechanism)
	{
        SYNO_LogE("This function doesn't support the sign mechanism! session_data->sign_mechanism = 0x%x", session_data->sign_mechanism);
        ulRet = CKR_MECHANISM_INVALID;
		goto ERROR;
	}

	// 杂凑句柄
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	
	// 杂凑运算
	ulRet = P11_DigestUpdate(hDigestKey, pPart, ulPartLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestUpdate() failed! ulRet = 0x%x", ulRet);
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
    }
	
	FUNC_END;	

	// 签名结束
ERROR:
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	session_data->sign_state = NULL;
	session_data->sign_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_SignFinal
 *	说明:签名结束
 *	参数:session_data:[IN]:会话对象
 *       pSignature:[OUT]:返回的签名值
 *       pulSignatureLen:[OUT]:签名值长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_SignFinal(CK_I_SESSION_DATA_PTR session_data, 
					CK_BYTE_PTR pSignature, 
					CK_ULONG_PTR pulSignatureLen)
{
	ULONG ulRet = CKR_OK;
	ULONG ulDigestLen = 256;
	BYTE btDigestData[256] = {0};
	HANDLE hDigestKey = NULL;
	HCONTAINER hContainer = NULL;
	CK_I_OBJ_PTR pstPriKeyObj = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulSignatureLen)
    {
        SYNO_LogE("pulSignatureLen is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// 判断算法
	if (CKM_RSA_PKCS == session_data->sign_mechanism)
	{
        SYNO_LogE("This function doesn't support the sign mechanism! session_data->sign_mechanism = 0x%x", session_data->sign_mechanism);
		ulRet = CKR_MECHANISM_INVALID;
		goto ERROR;
	}

	// 杂凑句柄
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}

	// 获取杂凑结果
	ulRet = P11_DigestFinal(hDigestKey, btDigestData, &ulDigestLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestFinal() failed! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
    }
	
	// 获取签名私钥所在容器
	pstPriKeyObj = (CK_I_OBJ_PTR)session_data->sign_state;
	if (NULL == pstPriKeyObj)
	{
        SYNO_LogE("pstPriKeyObj is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	hContainer = pstPriKeyObj->CtrHandle;
	if (NULL == hContainer)
	{
        SYNO_LogE("hContainer is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}

	// 对杂凑结果签名
	ulRet = P11_RSASignData(hContainer, btDigestData, ulDigestLen, pSignature, pulSignatureLen);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("P11_RSASignData() failed()! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	
	// 返回长度
	if (NULL == pSignature)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulSignatureLen);
		return CKR_OK;		
	}
	
	FUNC_END;

ERROR:
	// 杂凑结束
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	
	// 签名结束
	session_data->sign_state = NULL;
	session_data->sign_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_SignRecoverInit
 *	说明:可恢复签名运算初始化
 *	参数:session_data:[IN]:会话对象
 *       pMechanism:[OUT]:签名算法
 *       key_obj:[OUT]:签名密钥对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_SignRecoverInit(CK_I_SESSION_DATA_PTR session_data, 
						  CK_MECHANISM_PTR pMechanism, 
						  CK_I_OBJ_PTR key_obj)
{
	return CKR_FUNCTION_FAILED;
}
/***********************************************************************
 *	名称:TOK_SignRecover
 *	说明:可恢复签名运算
 *	参数:session_data:[IN]:会话对象
 *       pData:[IN]:待签名的原文数据
 *       ulDataLen:[IN]:原文数据长度
 *       pSignature:[OUT]:返回的签名值
 *       pulSignatureLen:[OUT]:签名值长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_SignRecover(CK_I_SESSION_DATA_PTR session_data,
					  CK_BYTE_PTR pData, 
					  CK_ULONG ulDataLen, 
					  CK_BYTE_PTR pSignature, 
					  CK_ULONG_PTR pulSignatureLen)
{
	return CKR_FUNCTION_FAILED;
}
/***********************************************************************
 *	名称:TOK_VerifyInit
 *	说明:验签初始化
 *	参数:session_data:[IN]:会话对象
 *       pMechanism:[OUT]:签名算法
 *       key_obj:[OUT]:签名密钥对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_VerifyInit(CK_I_SESSION_DATA_PTR session_data,
					 CK_MECHANISM_PTR pMechanism, 
					 CK_I_OBJ_PTR key_obj)
{
	ULONG ulRet = CKR_OK;
	ULONG ulAlgId = 0;
	DEVHANDLE hDev = NULL;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == key_obj)
    {
        SYNO_LogE("key_obj is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// 设备句柄
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
        return CKR_GENERAL_ERROR;
	}

	// 检查算法
	switch(pMechanism->mechanism)
	{
	case CKM_RSA_PKCS:
		break;
	case CKM_MD5_RSA_PKCS:
		ulAlgId = SGD_MD5;
		break;
	case CKM_SHA1_RSA_PKCS:
		ulAlgId = SGD_SHA1;
		break;
	case CKM_SHA256_RSA_PKCS:
		ulAlgId = SGD_SHA256;
		break;
	default:
        SYNO_LogE("Digest algId in mechanism is wrong! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	// 判断是否需要做杂凑运算
	if (CKM_RSA_PKCS == pMechanism->mechanism)
	{
		SYNO_LogD("pMechanism->mechanism = 0x%x, don't need to do digest!");
		session_data->digest_state = NULL;
	}
	else
	{
		SYNO_LogD("pMechanism->mechanism = 0x%x, the according SKF digest alg id: 0x%x", pMechanism->mechanism, ulAlgId);

		// 杂凑初始化
		ulRet = P11_DigestInit(hDev, ulAlgId, NULL, NULL, 0, &hDigestKey);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_DigestInit() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}

		// 保存句柄
		session_data->digest_state = hDigestKey;
	}

	session_data->verify_mechanism = pMechanism->mechanism;
	session_data->verify_state = key_obj;

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_Verify
 *	说明:验签单组数据
 *	参数:session_data:[IN]:会话对象
 *       pData:[IN]:签名的原文数据
 *       ulDataLen:[IN]:原文数据长度
 *       pSignature:[IN]:签名值
 *       pulSignatureLen:[IN]:签名值长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_Verify(CK_I_SESSION_DATA_PTR session_data, 
				 CK_BYTE_PTR pData, 
				 CK_ULONG ulDataLen, 
				 CK_BYTE_PTR pSignature, 
				 CK_ULONG ulSignatureLen)
{
	ULONG ulRet = CKR_OK;
	ULONG ulKeyBytes = 0;
	ULONG ulDigestLen = 256;
	BYTE btDigestData[256] = {0};
	HANDLE hDigestKey = NULL;
	DEVHANDLE hDev = NULL;
	CK_I_OBJ_PTR pstPubKeyObj = NULL;
	CK_ATTRIBUTE_PTR pstPubAttr = NULL;
	RSAPUBLICKEYBLOB stuRsaPubKey = {0};

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pData || 0 == ulDataLen)
    {
        SYNO_LogE("pData is null or ulDataLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
	if (NULL == pSignature || 0 == ulSignatureLen)
	{
        SYNO_LogE("pSignature is null or ulSignatureLen is 0!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// 设备句柄
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	// 获取签名公钥
	pstPubKeyObj = (CK_I_OBJ_PTR)session_data->verify_state;
	if (NULL == pstPubKeyObj)
	{
        SYNO_LogE("pstPubKeyObj is null!");
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}

	// 判断是否需要做杂凑运算
	if (CKM_RSA_PKCS == session_data->verify_mechanism)
	{		
		// 判断数据长度
		pstPubAttr = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPubKeyObj, CK_IA_MODULUS_BITS));
		ulKeyBytes = *(CK_ULONG_PTR)(pstPubAttr->pValue) / 8;
		if (ulDataLen > (ulKeyBytes - 11))
		{
			SYNO_LogE("ulDataLen is wrong! ulDataLen = 0x%x", ulDataLen);
			ulRet = CKR_DATA_LEN_RANGE;
			goto ERROR;
		}
		if (ulSignatureLen != ulKeyBytes)
		{
			SYNO_LogE("ulSignatureLen is wrong! ulSignatureLen = 0x%x", ulSignatureLen);
			ulRet = CKR_DATA_LEN_RANGE;
			goto ERROR;
		}

		// 拷贝数据
		memcpy(btDigestData, pData, ulDataLen);
		ulDigestLen = ulDataLen;
	}
	else
	{
		// 杂凑句柄
		hDigestKey = session_data->digest_state;
		if (NULL == hDigestKey)
		{
			SYNO_LogE("hDigestKey is null!");
			ulRet = CKR_GENERAL_ERROR;
			goto ERROR;
		}

		// 杂凑运算
		ulRet = P11_Digest(hDigestKey, pData, ulDataLen, btDigestData, &ulDigestLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_Digest() failed! ulRet = 0x%x", ulRet);
			ulRet = CKR_GENERAL_ERROR;
			goto ERROR;
		}
	}
		
	// 构造验签公钥
	ulRet = TOK_I_ConvertPubKeyObj(pstPubKeyObj, &stuRsaPubKey);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("TOK_I_ConvertPubKeyObj() failed! ulRet = 0x%x", ulRet);
		goto ERROR;
	}
	
	// 验签运算
	ulRet = P11_RSAVerify(hDev, &stuRsaPubKey, btDigestData, ulDigestLen, pSignature, ulSignatureLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_RSAVerify() failed! ulRet = 0x%x", ulRet);
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
    }
	
	FUNC_END;

	// 验签结束
ERROR:
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	session_data->verify_state = NULL;
	session_data->verify_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_VerifyUpdate
 *	说明:验签分组数据
 *	参数:session_data:[IN]:会话对象
 *       pPart:[IN]:签名的分组原文数据
 *       ulPartLen:[IN]:分组原文数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_VerifyUpdate(CK_I_SESSION_DATA_PTR session_data, 
					   CK_BYTE_PTR pPart, 
					   CK_ULONG ulPartLen)
{
	ULONG ulRet = CKR_OK;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pPart || 0 == ulPartLen)
    {
        SYNO_LogE("pPart is null or ulPartLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }

	// 判断算法
	if (CKM_RSA_PKCS == session_data->sign_mechanism)
	{
        SYNO_LogE("This function doesn't support the sign mechanism! session_data->sign_mechanism = 0x%x", session_data->sign_mechanism);
		ulRet = CKR_MECHANISM_INVALID;
		goto ERROR;
	}
	
	// 杂凑句柄
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	
	// 杂凑运算
	ulRet = P11_DigestUpdate(hDigestKey, pPart, ulPartLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestUpdate() failed! ulRet = 0x%x", ulRet);
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
    }
	
	FUNC_END;
	
	// 验签结束
ERROR:
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	session_data->verify_state = NULL;
	session_data->verify_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_VerifyFinal
 *	说明:验签结束
 *	参数:session_data:[IN]:会话对象
 *       pSignature:[IN]:签名值
 *       pulSignatureLen:[IN]:签名值长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_VerifyFinal(CK_I_SESSION_DATA_PTR session_data, 
					  CK_BYTE_PTR pSignature, 
					  CK_ULONG ulSignatureLen)
{
	ULONG ulRet = CKR_OK;
	ULONG ulDigestLen = 256;
	BYTE btDigestData[256] = {0};
	HANDLE hDigestKey = NULL;
	HCONTAINER hContainer = NULL;
	CK_I_OBJ_PTR pstPubKeyObj = NULL;
	RSAPUBLICKEYBLOB stuRsaPubKey = {0};

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pSignature || 0 == ulSignatureLen)
    {
        SYNO_LogE("pSignature is null or ulSignatureLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }

	// 判断算法
	if (CKM_RSA_PKCS == session_data->sign_mechanism)
	{
        SYNO_LogE("This function doesn't support the sign mechanism! session_data->sign_mechanism = 0x%x", session_data->sign_mechanism);
		ulRet = CKR_MECHANISM_INVALID;
		goto ERROR;
	}

	// 杂凑句柄
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}

	// 获取杂凑结果
	ulRet = P11_DigestFinal(hDigestKey, btDigestData, &ulDigestLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestFinal() failed! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
    }
	
	// 获取验签共钥所在容器
	pstPubKeyObj = (CK_I_OBJ_PTR)session_data->sign_state;
	if (NULL == pstPubKeyObj)
	{
        SYNO_LogE("pstPubKeyObj is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	hContainer = pstPubKeyObj->CtrHandle;
	if (NULL == hContainer)
	{
        SYNO_LogE("hContainer is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
			
	// 构造验签公钥
	ulRet = TOK_I_ConvertPubKeyObj(pstPubKeyObj, &stuRsaPubKey);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("TOK_I_ConvertPubKeyObj() failed! ulRet = 0x%x", ulRet);
		goto ERROR;
	}

	// 验签运算
	ulRet = P11_RSAVerify(hContainer, &stuRsaPubKey, btDigestData, ulDigestLen, pSignature, ulSignatureLen);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("P11_RSAVerify() failed()! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}

	FUNC_END;	
	
	// 验签结束
ERROR:
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	session_data->verify_state = NULL;
	session_data->verify_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	名称:TOK_VerifyRecoverInit
 *	说明:恢复签名运算初始化
 *	参数:session_data:[IN]:会话对象
 *       pMechanism:[OUT]:签名算法
 *       key_obj:[OUT]:签名密钥对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_VerifyRecoverInit(CK_I_SESSION_DATA_PTR session_data, 
							CK_MECHANISM_PTR pMechanism, 
							CK_I_OBJ_PTR key_obj)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	名称:TOK_VerifyRecover
 *	说明:恢复签名运算
 *	参数:session_data:[IN]:会话对象
 *       pSignature:[IN]:签名值
 *       ulSignatureLen:[IN]:签名值长度
 *       pData:[OUT]:返回的原文数据
 *       pulDataLen:[OUT]:原文数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_VerifyRecover(CK_I_SESSION_DATA_PTR session_data, 
						CK_BYTE_PTR pSignature,
						CK_ULONG ulSignatureLen,
						CK_BYTE_PTR pData, 
						CK_ULONG_PTR pulDataLen)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	名称:TOK_DigestEncryptUpdate
 *	说明:杂凑加密双运算
 *	参数:session_data:[IN]:会话对象
 *       pPart:[IN]:分组数据
 *       ulPartLen:[IN]:分组数据长度
 *       pEncryptedPart:[OUT]:返回的密文
 *       pulEncryptedPartLen:[OUT]:密文长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_DigestEncryptUpdate(CK_I_SESSION_DATA_PTR session_data, 
							  CK_BYTE_PTR pPart, 
							  CK_ULONG ulPartLen, 
							  CK_BYTE_PTR pEncryptedPart, 
							  CK_ULONG_PTR pulEncryptedPartLen)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	名称:TOK_DecryptDigestUpdate
 *	说明:解密杂凑双运算
 *	参数:session_data:[IN]:会话对象
 *       pEncryptedPart:[IN]:密文
 *       ulEncryptedPartLen:[IN]:密文长度
 *       pPart:[OUT]:解密后的数据
 *       pulPartLen:[OUT]:解密后的数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_DecryptDigestUpdate(CK_I_SESSION_DATA_PTR session_data, 
							  CK_BYTE_PTR pEncryptedPart, 
							  CK_ULONG ulEncryptedPartLen, 
							  CK_BYTE_PTR pPart, 
							  CK_ULONG_PTR pulPartLen)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	名称:TOK_SignEncryptUpdate
 *	说明:签名加密双运算
 *	参数:session_data:[IN]:会话对象
 *       pPart:[IN]:分组数据
 *       ulPartLen:[IN]:分组数据长度
 *       pEncryptedPart:[OUT]:返回的密文
 *       pulEncryptedPartLen:[OUT]:密文长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_SignEncryptUpdate(CK_I_SESSION_DATA_PTR session_data, 
							CK_BYTE_PTR pPart, 
							CK_ULONG ulPartLen, 
							CK_BYTE_PTR pEncryptedPart, 
							CK_ULONG_PTR pulEncryptedPartLen)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	名称:TOK_DecryptVerifyUpdate
 *	说明:解密验签双运算
 *	参数:session_data:[IN]:会话对象
 *       pEncryptedPart:[IN]:密文
 *       ulEncryptedPartLen:[IN]:密文长度
 *       pPart:[OUT]:解密后的数据
 *       pulPartLen:[OUT]:解密后的数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_DecryptVerifyUpdate(CK_I_SESSION_DATA_PTR session_data, 
							  CK_BYTE_PTR pEncryptedPart, 
							  CK_ULONG ulEncryptedPartLen, 
							  CK_BYTE_PTR pPart, 
							  CK_ULONG_PTR pulPartLen)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	名称:TOK_GenerateKey
 *	说明:生成会话密钥
 *	参数:session_data:[IN]:会话对象
 *       pMechanism:[IN]:会话密钥算法
 *       key:[IN]:密钥对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_GenerateKey(CK_I_SESSION_DATA_PTR session_data,
					  CK_MECHANISM_PTR pMechanism, 
					  CK_I_OBJ_PTR key)
{
    ULONG ulRet = CKR_OK;
    ULONG ulAlgId = 0;
	DEVHANDLE hDev = NULL;
    HANDLE hSessionKey = NULL;

	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
        return CKR_GENERAL_ERROR;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_GENERAL_ERROR;
    }
    if (NULL == key)
    {
        SYNO_LogE("key is null!");
        return CKR_GENERAL_ERROR;
    }

	// 获取设备句柄
	if (!session_data->slot_data)
	{
        SYNO_LogE("session_data->slot_data is null!");
        return CKR_GENERAL_ERROR;
	}
	hDev = session_data->slot_data->pDevHandle;
	if (!hDev)
	{
        SYNO_LogE("hDev is null!");
        return CKR_GENERAL_ERROR;
	}

	// 算法
	switch (pMechanism->mechanism)
	{
	case CKM_DES_KEY_GEN:
		ulAlgId = SGD_DES;
		break;
	case CKM_DES3_KEY_GEN:
		ulAlgId = SGD_DES3;
		break;
	case CKM_AES_KEY_GEN:
		ulAlgId = SGD_AES128;
		break;
	default:
        SYNO_LogE("pMechanism->mechanism is invalid! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}
	
    // 生成会话密钥
	ulRet = P11_GenSymmKey(hDev, ulAlgId, &hSessionKey);
    if (CKR_OK != ulRet)
    {
        SYNO_LogE("P11_GenSymmKey() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

	// 保存对象
	key->ObjHandle = hSessionKey;

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	名称:TOK_GenerateKeyPair
 *	说明:生成密钥对
 *	参数:session_data:[IN]:会话对象
 *       pMechanism:[IN]:密钥算法
 *       public_key:[IN]:公钥对象
 *       private_key:[IN]:私钥对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_GenerateKeyPair(CK_I_SESSION_DATA_PTR session_data,
						  CK_MECHANISM_PTR pMechanism,
						  CK_I_OBJ_PTR public_key,
						  CK_I_OBJ_PTR private_key)
{
    CK_RV ulRet = CKR_OK;
	ULONG ulValueLen = 0;
	CK_BBOOL bIsToken = FALSE;
	CK_BBOOL bIsSign = FALSE;
	CK_BBOOL bIsExch = FALSE;
	CK_BBOOL bIsEncrypt = FALSE;
	ULONG ulKeyType = 0;
	ULONG ulBits = 0;
	LPSTR lpNewContainer = NULL;
	HAPPLICATION hApp = NULL;
    HCONTAINER hContainer = NULL;
    RSAPUBLICKEYBLOB stBlob = {0};
	ULONG ulRsaPubKeyLen = 0;	
    ULONG ulContainerCount = 0;
    CHAR acName[PATH_MAX] = {0};
    ULONG ulSize = PATH_MAX;
	CK_ATTRIBUTE rsaPubKeyAttr = {CKA_MODULUS, NULL, 0};

	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == public_key)
    {
        SYNO_LogE("public_key is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == private_key)
    {
        SYNO_LogE("private_key is null!");
        return CKR_ARGUMENTS_BAD;
    }
	
	// 获取应用句柄
	hApp = session_data->AppHandle;
	if (NULL == hApp)
	{
        SYNO_LogE("hApp is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}

	// 检查公钥的CKA_TOKEN属性
	ulValueLen = 1;
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_TOKEN, (CK_BYTE_PTR)&bIsToken, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_TOKEN from public_key failed!ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (!bIsToken)
	{
		SYNO_LogE("CKA_TOKEN value of public_key is FALSE!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// 检查私钥的CKA_TOKEN属性
	bIsToken = FALSE;
	ulRet = CI_ObjGetAttributeValue(private_key, CKA_TOKEN, (CK_BYTE_PTR)&bIsToken, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_TOKEN from private_key failed!ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (!bIsToken)
	{
		SYNO_LogE("CKA_TOKEN value of private_key is FALSE!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	
	// 检查公钥的CKA_KEY_TYPE属性
	ulValueLen = 4;
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_KEY_TYPE, (CK_BYTE_PTR)&ulKeyType, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_KEY_TYPE from public_key failed!ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (CKK_RSA != ulKeyType)
	{
		SYNO_LogE("CKA_TOKEN value of public_key is wrong! ulKeyType = 0x%x", ulKeyType);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// 检查私钥的CKA_KEY_TYPE属性
	ulKeyType = 0;
	ulRet = CI_ObjGetAttributeValue(private_key, CKA_KEY_TYPE, (CK_BYTE_PTR)&ulKeyType, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_KEY_TYPE from private_key failed!ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (CKK_RSA != ulKeyType)
	{
		SYNO_LogE("CKA_TOKEN value of private_key is wrong! ulKeyType = 0x%x", ulKeyType);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	
	// 获取公钥模的bits
	ulKeyType = 4;
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_MODULUS_BITS, (CK_BYTE_PTR)&ulBits, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_MODULUS_BITS from public_key failed!ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (1024 != ulBits && 2048 != ulBits)
	{
		SYNO_LogE("CKA_MODULUS_BITS value of public_key is wrong! ulBits = 0x%x", ulBits);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
		
	// 获取私钥用途
	ulValueLen = sizeof(CK_BBOOL);
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_VERIFY, (CK_BYTE_PTR)&bIsSign, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VERIFY from public_key failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_WRAP, (CK_BYTE_PTR)&bIsExch, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_WRAP from public_key failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_ENCRYPT, (CK_BYTE_PTR)&bIsEncrypt, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ENCRYPT from public_key failed! ulRet = 0x%x", ulRet);
	}
	if (!bIsSign && !bIsExch && !bIsEncrypt)
	{
		SYNO_LogE("All of CKA_VERIFY, CKA_WRAP and CKA_ENCRYPT attribute is FALSE!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// 输出私钥用途信息
	if (bIsSign) SYNO_LogD("This key pair can be use to sign/verify data.");
	if (bIsExch) SYNO_LogD("This key pair can be use to wrap/unwrap session key.");
	if (bIsEncrypt) SYNO_LogD("This key pair can be use to encrypt/decrypt data.");
		
	// 创建容器
	do
	{
		lpNewContainer = TOK_I_GenUniqeContainerId();
		ulRet = P11_CreateContainer(hApp, lpNewContainer, &hContainer);
	}while (SAR_COS_CONTAINER_ALREADY_EXISTS == ulRet);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
	}
	SYNO_LogD("New container: %s is created！", lpNewContainer);

	// 生成RSA密钥对
    ulRet = P11_GenRSAKeyPair(hContainer, bIsSign ? 1 : 0, ulBits, &stBlob);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_GenRSAKeyPair() failed! ulRet = 0x%x", ulRet);
        return ulRet;
    }

	// 设置公钥模属性	
	rsaPubKeyAttr.ulValueLen = stBlob.BitLen / 8;
	rsaPubKeyAttr.pValue = SYNO_calloc(1, stBlob.BitLen / 8);
	memcpy(rsaPubKeyAttr.pValue, stBlob.Modulus + MAX_RSA_MODULUS_BITS/8-stBlob.BitLen/8, stBlob.BitLen / 8);
	ulRet = CI_ObjSetAttribute(public_key, &rsaPubKeyAttr);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("CI_ObjSetAttribute() failed! ulRet = 0x%x", ulRet);
		SYNO_free(rsaPubKeyAttr.pValue);
        return ulRet;
    }

	// 保存容器句柄
	public_key->CtrHandle = hContainer;
	private_key->CtrHandle = hContainer;

	FUNC_END;

    return CKR_OK;
}
 /***********************************************************************
 *	名称:TOK_WrapKey
 *	说明:打包会话密钥
 *	参数:session_data:[IN]:会话对象
 *       pMechanism:[IN]:密钥算法
 *       wrap_key_obj:[IN]:用来打包会话密钥的公钥对象
 *       key_obj:[IN]:被打包的会话密钥对象
 *       pWrappedKey:[OUT]:返回的打包结果数据
 *       pulWrappedKeyLen:[OUT]:结果数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_WrapKey(CK_I_SESSION_DATA_PTR session_data,
				  CK_MECHANISM_PTR pMechanism,
				  CK_I_OBJ_PTR wrap_key_obj, 
				  CK_I_OBJ_PTR key_obj, 
				  BYTE* pWrappedKey, 
				  CK_RV* pulWrappedKeyLen)
{
    CK_RV ulRet = CKR_OK;
	RSAPUBLICKEYBLOB stRSAPubBlob = {0};
	BYTE EncSessionkey[128] = {0};
	HANDLE pSessionKey = NULL;
	ULONG ulRetLen = 0;

	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == wrap_key_obj)
    {
        SYNO_LogE("wrap_key_obj is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == key_obj)
    {
        SYNO_LogE("key_obj is null!");
        return CKR_ARGUMENTS_BAD;
	}
    if (NULL == pWrappedKey)
    {
        SYNO_LogE("pucWrappedKey is null!");
        return CKR_ARGUMENTS_BAD;
	}

	// Wrap格式检查
	if (pMechanism->mechanism != CKM_RSA_PKCS)
	{
        SYNO_LogE("pMechanism->mechanism is wrong, only support CKM_RSA_PKCS! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}
	
	// 构造Wrap Key的公钥
	ulRet = TOK_I_ConvertPubKeyObj(wrap_key_obj, &stRSAPubBlob);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("TOK_I_ConvertPubKeyObj() failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}

	// 导出指定的Key数据
	ulRet = P11_RSAExportSessionKeyEx(key_obj->ObjHandle, &stRSAPubBlob, pWrappedKey, pulWrappedKeyLen);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("P11_RSAExportSessionKeyEx() failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}

	FUNC_END;

    return CKR_OK;
}
 /***********************************************************************
 *	名称:TOK_UnwrapKey
 *	说明:解包会话密钥
 *	参数:session_data:[IN]:会话对象
 *       pMechanism:[IN]:密钥算法
 *       unwrap_key_obj:[IN]:用来解包会话密钥的私钥对象
 *       pWrappedKey:[IN]:打包后的回话密钥数据
 *       pulWrappedKeyLen:[IN]:数据长度
 *       key_obj:[OUT]:返回的会话密钥对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_UnwrapKey(CK_I_SESSION_DATA_PTR  session_data,
					CK_MECHANISM_PTR pMechanism,
					CK_I_OBJ_PTR unwrap_key_obj, 
					BYTE* pWrappedKey, 
					ULONG ulWrappedKeyLen, 
					CK_I_OBJ_PTR key_obj)
{
    ULONG ulRet = CKR_OK;
    HCONTAINER hContainer = NULL;
    ULONG ulAlgId = 0;
    HANDLE hKey = NULL;
	CK_MECHANISM_TYPE_PTR pMechanismType = NULL;

	FUNC_BEGIN;

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == unwrap_key_obj)
    {
        SYNO_LogE("unwrap_key_obj is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pWrappedKey)
    {
        SYNO_LogE("pWrappedKey is null!");
        return CKR_ARGUMENTS_BAD;
	}
    if (NULL == key_obj)
    {
        SYNO_LogE("pstKeyObj is null!");
        return CKR_ARGUMENTS_BAD;
	}
	
	// Unwrap格式检查
	if (pMechanism->mechanism != CKM_RSA_PKCS)
	{
        SYNO_LogE("pMechanism->mechanism is wrong, only support CKM_RSA_PKCS! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	// 获取Key算法
	pMechanismType = (CK_MECHANISM_TYPE_PTR)CI_ObjLookup(key_obj, CK_IA_KEY_TYPE)->pValue;
	switch (*pMechanismType)
	{
	case CKK_DES:
		ulAlgId = SGD_DES;
		break;
	case CKK_DES3:
		ulAlgId = SGD_DES3;
		break;
	case CKK_AES:
		ulAlgId = SGD_AES128;
		break;
	default:
        SYNO_LogE("CKA_KEY_TYPE value in Unwrap key mechanism is wrong! Value = 0x%x", *pMechanism);
		return CKR_MECHANISM_PARAM_INVALID;
	}
	
    // 导入会话密钥
    ulRet = P11_ImportSessionKey(unwrap_key_obj->CtrHandle, ulAlgId, pWrappedKey, ulWrappedKeyLen, &hKey);
    if (CKR_OK != ulRet)
    {
        SYNO_LogE("P11_ImportSessionKey() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
    }

    key_obj->ObjHandle = hKey;
	key_obj->CtrHandle = unwrap_key_obj->CtrHandle;
	
	FUNC_END;

    return CKR_OK;
}
 /***********************************************************************
 *	名称:TOK_DeriveKey
 *	说明:衍生一个会话密钥
 *	参数:session_data:[IN]:会话对象
 *       pMechanism:[IN]:密钥算法
 *       base_key:[IN]:基本密钥对象
 *       derived_ky:[OUT]:返回的会话密钥对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_DeriveKey(CK_I_SESSION_DATA_PTR session_data, 
					CK_MECHANISM_PTR pMechanism, 
					CK_I_OBJ_PTR base_key, 
					CK_I_OBJ_PTR derived_ky)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	名称:TOK_SeedRandom
 *	说明:设置随机数的种子
 *	参数:session_data:[IN]:会话对象
 *       pSeed:[IN]:种子数据
 *       ulSeedLen:[IN]:种子数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_SeedRandom(CK_I_SESSION_DATA_PTR session_data, 
					 CK_BYTE_PTR pSeed, 
					 CK_ULONG ulSeedLen)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	名称:TOK_GenerateRandom
 *	说明:生成随机数
 *	参数:session_data:[IN]:会话对象
 *       pRandomData:[OUT]:返回随机数数据
 *       ulRandomLen:[IN]:数据长度
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_GenerateRandom(CK_I_SESSION_DATA_PTR session_data,
						 CK_BYTE_PTR pRandomData, 
						 CK_ULONG ulRandomLen)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	名称:TOK_GetFunctionStatus
 *	说明:返回函数状态
 *	参数:session_data:[IN]:会话对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_GetFunctionStatus(CK_I_SESSION_DATA_PTR session_data)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	名称:TOK_GetFunctionStatus
 *	说明:取消函数
 *	参数:session_data:[IN]:会话对象
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_CancelFunction(CK_I_SESSION_DATA_PTR session_data)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	名称:TOK_WaitForSlotEvent
 *	说明:等待Slot实践
 *	参数:flags:[IN]:等待标记
 *      pSlot:[OUT]:发生事件的Slot ID
 *      pRserved:[OUT]:保留参数
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_WaitForSlotEvent(CK_FLAGS flags, 
						   CK_SLOT_ID_PTR pSlot, 
						   CK_VOID_PTR pRserved)
{
	CK_RV rv = CKR_OK;
	CK_BYTE devName[512] = {0};
	CK_ULONG nameLen = 512;
	CK_ULONG eventType = 0;
	CK_ULONG eventSlotID = 0;

	FUNC_BEGIN;
	
	//	暂不支持非阻塞模式，因为调用的国密接口是阻塞的
	if (CKF_DONT_BLOCK == flags)
	{
        SYNO_LogE("flags is wrong! Only support block mode! flags = 0x%x", flags);
		return CKR_FUNCTION_NOT_SUPPORTED;
	}
	if (NULL == pSlot)
	{
        SYNO_LogE("pSlot is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	//	等待设备事件
	rv = P11_WaitForDevEvent((LPSTR)devName, &nameLen, &eventType);
	if (SAR_OK != rv)
	{
        SYNO_LogE("P11_WaitForDevEvent() failed! rv = 0x%x", rv);
		return CKR_FUNCTION_FAILED;
	}
	if (strlen((LPSTR)devName) == 0 || nameLen <= 2)
	{
        SYNO_LogE("P11_WaitForDevEvent() return with devName is NULL!");
		return CKR_GENERAL_ERROR;
	}

	//	是新设备接入消息
	if (1 == eventType)
	{
		CK_I_SLOT_DATA_PTR pSlotData = NULL_PTR;
        rv = TOK_TokenInitialize(devName, &pSlotData);
        if (CKR_OK != rv)
        {
            SYNO_LogE("TOK_TokenInitialize err!");
            return rv;
        }

		eventSlotID = CK_I_SLOT_ANZ;
        rv = CI_RegisterSlot(eventSlotID, pSlotData);
        if (CKR_OK != rv)
        {
			SYNO_free(pSlotData);
            SYNO_LogE("CI_RegisterSlot err!");
            return rv;
        }
	}
	//	是设备移除事件
	else if (2 == eventType)
	{	
		CK_SLOT_ID_PTR slot_list = NULL_PTR;
		CK_ULONG slot_num = 0;
		CK_ULONG i = 0;
		
		rv = C_GetSlotList(TRUE, NULL_PTR, &slot_num);
		if (rv != CKR_OK)
		{
			SYNO_LogE("C_GetSlotList() failed! rv = 0x%x", rv);
			return rv;
		}

		slot_list = (CK_SLOT_ID_PTR)SYNO_malloc(slot_num * sizeof(CK_SLOT_ID_PTR));
		if (slot_list == NULL_PTR)
		{
			SYNO_LogE("Getting space for slot list failed! rv = 0x%x", rv);
			return CKR_HOST_MEMORY;
		}
		rv = C_GetSlotList(TRUE, slot_list, &slot_num);
		if (rv != CKR_OK)
		{
			SYNO_free(slot_list);
			SYNO_LogE("C_GetSlotList() failed! rv = 0x%x", rv);
			return rv;
		}

		rv = CKR_NO_EVENT;
		for (i = 0 ; i < slot_num; i++)
		{
			CK_I_SLOT_DATA_PTR slot_data = NULL_PTR;
			CI_GetSlotData(slot_list[i], &slot_data);
			if (slot_data && (strcmp((CHAR*)slot_data->config_section_name, (CHAR*)devName) == 0))
			{
				rv = CI_RemoveToken(slot_list[i]);
				if (rv != CKR_OK)
				{
					SYNO_LogE("CI_RemoveToken() failed in slot %d! rv = 0x%x", slot_list[i], rv);
					//return rv;
				}
				eventSlotID = slot_list[i];
				SYNO_free(slot_list);
				rv = CKR_OK;
				break;
			}
		}
	}
	else
	{
        SYNO_LogE("P11_WaitForDevEvent() return a wrong event type! eventType = 0x%x", eventType);
		return CKR_GENERAL_ERROR;
	}

	*pSlot = eventSlotID;
	if (pRserved)
	{
		*((ULONG*)pRserved) = eventType;
	}

	FUNC_END;
	
	return rv;
}
 /***********************************************************************
 *	名称:TOK_TokenObjRetrieve
 *	说明:恢复一个对象的属性??
 *	参数:session_data:[IN]:会话对象
 *      phObject:[IN]:对象句柄
 *      ppNewObject:[OUT]:返回的对象指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_TokenObjRetrieve(CK_I_SESSION_DATA_PTR session_data, 
						   CK_OBJECT_HANDLE phObject, 
						   CK_I_OBJ_PTR CK_PTR ppNewObject)
{
	return CKR_FUNCTION_FAILED;
}
/***********************************************************************
 *	名称:TOK_TokenObjCommit
 *	说明:设置对象属性，仅限于设置CKO_DATA的值。
 *	参数:session_data:[IN]:会话对象
 *      phObject:[IN]:对象句柄
 *      ppNewObject:[OUT]:返回的对象指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_TokenObjCommit(CK_I_SESSION_DATA_PTR session_data, 
						 CK_OBJECT_HANDLE hObject, 
						 CK_I_OBJ_PTR pObject)
{
	CK_RV rv = CKR_OK;
	CK_BBOOL bIsToken = CK_FALSE;
	CK_ULONG ulIsTokenLen = sizeof(CK_BBOOL);
	CK_OBJECT_CLASS ulObjClass = CKO_VENDOR_DEFINED;  
	CK_ULONG ulObjClassLen = sizeof(CK_OBJECT_CLASS);
	CK_ATTRIBUTE_PTR pAttribute = NULL;
	HAPPLICATION hApp = NULL;
	LPSTR lpFileName = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == hObject)
    {
        SYNO_LogE("hObject is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pObject)
    {
        SYNO_LogE("pObject is null!");
        return CKR_ARGUMENTS_BAD;
    }
	
	// 获取应用句柄
	hApp = session_data->AppHandle;
	if (NULL == hApp)
	{
        SYNO_LogE("hApp is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}
	
	// 检查CKA_TOKEN属性是否为TRUE
	rv = CI_ObjGetIntAttributeValue(pObject, CK_IA_TOKEN, &bIsToken, &ulIsTokenLen);
	if (CKR_OK != rv || CK_FALSE == bIsToken)
	{
        SYNO_LogE("CI_ObjGetIntAttributeValue() with CK_IA_TOKEN attribute value is FALSE! rv = 0x%x, bIsToken = %d", rv, bIsToken);
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}

	// 获取对象类别
	rv = CI_ObjGetIntAttributeValue(pObject, CK_IA_CLASS, (CK_BYTE_PTR)&ulObjClass, &ulObjClassLen);
	if (CKR_OK != rv)
	{
        SYNO_LogE("CI_ObjGetIntAttributeValue() with CK_IA_CLASS failed! rv = 0x%x", rv);
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}
	if (CKO_DATA != ulObjClass)
	{
        SYNO_LogE("Object class is wrong! ulObjClass = 0x%x", ulObjClass);
		return CKR_FUNCTION_NOT_SUPPORTED;
	}

	// 获取文件名	
	pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pObject, CK_IA_ID));
	if (NULL == pAttribute)
	{
        SYNO_LogE("Attribute CK_IA_ID is not find!");
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}
	lpFileName = (LPSTR)SYNO_malloc(pAttribute->ulValueLen + 1);
	memcpy(lpFileName, pAttribute->pValue, pAttribute->ulValueLen);
	lpFileName[pAttribute->ulValueLen] = '\0';

	// 获取文件内容	
	pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pObject, CK_IA_VALUE));
	if (NULL == pAttribute)
	{
        SYNO_LogE("Attribute CK_IA_VALUE is not find!");
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}

	// 写文件
	rv = P11_WriteFile(hApp, lpFileName, 0, (BYTE*)pAttribute->pValue, pAttribute->ulValueLen);
	if (SAR_OK != rv)
	{
        SYNO_LogE("P11_WriteFile() failed! rv = 0x%x", rv);
		rv = CKR_FUNCTION_FAILED;
		goto FREE;
	}

	FUNC_END;

FREE:
	if (lpFileName)
	{
		SYNO_free(lpFileName);
		lpFileName = NULL;
	}

	return rv;
}
/***********************************************************************
 *	名称:TOK_TokenObjAdd
 *	说明:添加一个对象
 *	参数:session_data:[IN]:会话对象
 *      phObject:[IN]:对象句柄
 *      ppNewObject:[OUT]:返回的对象指针
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_TokenObjAdd(CK_I_SESSION_DATA_PTR session_data, 
					  CK_OBJECT_HANDLE phObject, 
					  CK_I_OBJ_PTR pNewObject)
{
	CK_RV rv = CKR_OK;
	CK_BBOOL bIsToken = CK_FALSE;
	CK_ULONG ulIsTokenLen = sizeof(CK_BBOOL);
	CK_OBJECT_CLASS ulObjClass = CKO_VENDOR_DEFINED;  
	CK_ULONG ulObjClassLen = sizeof(CK_OBJECT_CLASS);
	DEVHANDLE hDev = NULL;
	HAPPLICATION hApp = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == phObject)
    {
        SYNO_LogE("phObject is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pNewObject)
    {
        SYNO_LogE("pNewObject is null!");
        return CKR_ARGUMENTS_BAD;
    }
	
	// 获取设备句柄
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}

	// 获取应用句柄
	hApp = session_data->AppHandle;
	if (NULL == hApp)
	{
        SYNO_LogE("hApp is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}

	// 检查CKA_TOKEN属性是否为TRUE
	rv = CI_ObjGetIntAttributeValue(pNewObject, CK_IA_TOKEN, &bIsToken, &ulIsTokenLen);
	if (CKR_OK != rv || CK_FALSE == bIsToken)
	{
        SYNO_LogE("CI_ObjGetIntAttributeValue() or CK_IA_TOKEN attribute value is FALSE! rv = 0x%x, bIsToken = %d", rv, bIsToken);
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}

	// 获取对象类别
	rv = CI_ObjGetIntAttributeValue(pNewObject, CK_IA_CLASS, (CK_BYTE_PTR)&ulObjClass, &ulObjClassLen);
	if (CKR_OK != rv)
	{
        SYNO_LogE("CI_ObjGetIntAttributeValue() failed! rv = 0x%x", rv);
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}
	SYNO_LogD("Object class is: 0x%x", ulObjClass);

	// 根据不同的对象类型，将对象数据导入Token中
	switch (ulObjClass)
	{
	case CKO_PUBLIC_KEY:
		rv = TOK_I_ImportPublicKey(hApp, pNewObject);
		break;
	case CKO_PRIVATE_KEY:
		rv = TOK_I_ImportPrivateKey(hApp, pNewObject);
		break;
	case CKO_SECRET_KEY:
		rv = TOK_I_ImportSessionKey(hDev, pNewObject);
		break;
	case CKO_CERTIFICATE:
		rv = TOK_I_ImportCertificate(hApp, pNewObject);
		break;
	case CKO_DATA:
		rv = TOK_I_ImportDataFile(hApp, pNewObject);
		break;
	default:
		SYNO_LogE("This object class is can't add to token!");
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}

	FUNC_END;

	return rv;
}
/***********************************************************************
 *	名称:TOK_TokenObjDelete
 *	说明:删除一个对象
 *	参数:session_data:[IN]:会话对象
 *      phObject:[IN]:对象句柄
 *	返回:成功返回CR_OK，否则返回错误代码。
 ***********************************************************************/
CK_RV TOK_TokenObjDelete(CK_I_SESSION_DATA_PTR session_data, 
						 CK_OBJECT_HANDLE phObject)
{
	CK_RV rv = CKR_OK;
	CK_I_OBJ_PTR obj = NULL_PTR;
	CK_OBJECT_CLASS ulObjClass = CKO_VENDOR_DEFINED;  
	CK_ULONG ulObjClassLen = sizeof(CK_OBJECT_CLASS);
	DEVHANDLE hDev = NULL;
	HAPPLICATION hApp = NULL;

	FUNC_BEGIN;
	
	// 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == phObject)
    {
        SYNO_LogE("phObject is null!");
        return CKR_ARGUMENTS_BAD;
    }
	
	// 获取设备句柄
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}

	// 获取应用句柄
	hApp = session_data->AppHandle;
	if (NULL == hApp)
	{
        SYNO_LogE("hApp is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}
	
	// 获取Object对象
	rv = CI_ReturnObj(session_data, phObject, &obj);
    if (CKR_OK != rv || NULL == obj)
    {
        SYNO_LogE("CI_ReturnObj() failed or obj is null! rv = 0x%x", rv);
        return rv;
    }

	// 获取对象类别
	rv = CI_ObjGetIntAttributeValue(obj, CK_IA_CLASS, (CK_BYTE_PTR)&ulObjClass, &ulObjClassLen);
	if (CKR_OK != rv)
	{
        SYNO_LogE("CI_ObjGetIntAttributeValue() failed! rv = 0x%x", rv);
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}
	SYNO_LogD("Object class is: 0x%x", ulObjClass);

	// 根据不同的对象类型，将对象数据导入Token中
	switch (ulObjClass)
	{
	case CKO_PUBLIC_KEY:
	case CKO_PRIVATE_KEY:
	case CKO_CERTIFICATE:
		// 直接删除对象所在容器
		rv = TOK_I_DeleteObjContainer(hApp, obj);
		if (CKR_OK != rv)
		{
			SYNO_LogE("TOK_I_DeleteObjContainer() failed! rv = 0x%x", rv);
			return rv;
		}
		break;
	case CKO_SECRET_KEY:
		rv = P11_CloseHandle(obj->ObjHandle);
		if (CKR_OK != rv)
		{
			SYNO_LogE("P11_CloseHandle() failed! rv = 0x%x", rv);
			return rv;
		}
		obj->ObjHandle = NULL;
		break;
	case CKO_DATA:
		{
			ULONG ulLen = 256;
			CHAR csFileName[256] = {0};
			rv = CI_ObjGetIntAttributeValue(obj, CK_IA_ID, (CK_BYTE_PTR)csFileName, &ulLen);
			if (CKR_OK != rv)
			{
				SYNO_LogE("CI_ObjGetIntAttributeValue() get CK_IA_ID failed! rv = 0x%x", rv);
				return rv;
			}
			rv = P11_DeleteFile(hApp, csFileName);
			if (CKR_OK != rv)
			{
				SYNO_LogE("P11_DeleteFile() file %s failed! rv = 0x%x", csFileName, rv);
				return rv;
			}
		}
		break;
	default:
		SYNO_LogE("This object class is can't add to token!");
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}

	FUNC_END;

	return rv;
}

CK_RV TOK_CreateObject(CK_I_SESSION_DATA_PTR session_data,
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

    // 参数检查
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
        return CKR_GENERAL_ERROR;
    }
    if (NULL == pstObj)
    {
        SYNO_LogE("pstObj is null!");
        return CKR_GENERAL_ERROR;
    }

	// 查找模板
    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstObj, CK_IA_ID));
    if (NULL == pstTemplate)
    {
        return CKR_OK;
    }

    strncpy((char*)aucLabel, (char*)pstTemplate->pValue, pstTemplate->ulValueLen);

    /* 创建容器 */
    ulRet = P11_CreateContainer(session_data->AppHandle, (CHAR *)aucLabel, &hContainer);
	if (SAR_COS_FILE_ALREADY_EXIST == ulRet)
	{
		ulRet = P11_OpenContainer(session_data->AppHandle, (CHAR *)aucLabel, &hContainer);
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
	ulRet = P11_ImportPublicKey(hContainer, FALSE, (CK_BYTE_PTR)&stRSAPubBlob, ulBlobLen, &hKey);
    if (CKR_OK != ulRet)
    {
        return ulRet;
    }

    pstObj->ObjHandle = hKey;

    ulContainerCount = session_data->CtrCount;
    session_data->aCtrHandle[ulContainerCount] = hContainer;
    session_data->CtrCount++;

	FUNC_END;

    return ulRet;
}
