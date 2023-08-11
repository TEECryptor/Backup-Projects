/***************************************************************************
 *	File name:SynoTFKeySKF.h
 *	Introduce:SKF API functions definition
 *	Author:Syno common
 *	Date:2015/07/18
 *  Version:v1.0
 **************************************************************************/

#ifndef _SYNO_TFKEY_SKF_H_
#define	_SYNO_TFKEY_SKF_H_

#include "Common.h"

/*----------------------------------------------*
 * 分组密码算法标识                             *
 *----------------------------------------------*/
#define SGD_SM1_ECB	                  0x00000101 /* SM1算法ECB加密模式 */
#define SGD_SM1_CBC	                  0x00000102 /* SM1算法CBC加密模式 */
#define SGD_SM1_CFB	                  0x00000104 /* SM1算法CFB加密模式 */
#define SGD_SM1_OFB	                  0x00000108 /* SM1算法OFB加密模式 */
#define SGD_SM1_MAC	                  0x00000110 /* SM1算法MAC运算 */

#define SGD_SSF33_ECB	              0x00000201 /* SSF33算法ECB加密模式 */
#define SGD_SSF33_CBC	              0x00000202 /* SSF33算法CBC加密模式 */
#define SGD_SSF33_CFB	              0x00000204 /* SSF33算法CFB加密模式 */
#define SGD_SSF33_OFB	              0x00000208 /* SSF33算法OFB加密模式 */
#define SGD_SSF33_MAC	              0x00000210 /* SSF33算法MAC运算 */

#define SGD_SMS4_ECB	              0x00000401 /* SMS4算法ECB加密模式 */
#define SGD_SMS4_CBC	              0x00000402 /* SMS4算法CBC加密模式 */
#define SGD_SMS4_CFB	              0x00000404 /* SMS4算法CFB加密模式 */
#define SGD_SMS4_OFB	              0x00000408 /* SMS4算法OFB加密模式 */
#define SGD_SMS4_MAC	              0x00000410 /* SMS4算法MAC运算 */

#define	SGD_AES128_CBC	              0x00000501 /* AES 128位(16bytes) CBC加密算法 */
#define	SGD_AES128_ECB	              0x00000502 /* AES 128位(16bytes) ECB加密算法 */
#define	SGD_AES192_CBC	              0x00000504 /* AES 192位(24bytes) CBC加密算法 */
#define	SGD_AES192_ECB	              0x00000508 /* AES 192位(24bytes) ECB加密算法 */
#define	SGD_AES256_CBC	              0x00000510 /* AES 256位(32bytes) CBC加密算法 */
#define	SGD_AES256_ECB	              0x00000511 /* AES 256位(32bytes) ECB加密算法 */

/*----------------------------------------------*
 * 非对称密码算法标识                           *
 *----------------------------------------------*/
#define SGD_RSA                       0x00010000 /* RSA算法 */
#define SGD_SM2_1                     0x00020100 /* 椭圆曲线签名算法 */
#define SGD_SM2_2                     0x00020200 /* 椭圆曲线密钥交换协议 */
#define SGD_SM2_3                     0x00020400 /* 椭圆曲线加密算法 */

/*----------------------------------------------*
 * 杂凑密码算法标识                             *
 *----------------------------------------------*/
#define SGD_SM3                       0x00000001 /* SM3密码杂凑算法 */
#define SGD_SHA1                      0x00000002 /* SHA1密码杂凑算法 */
#define SGD_SHA256                    0x00000004 /* SHA256密码杂凑算法 */
#if 0
/*----------------------------------------------*
 * 基本数据类型                                 *
 *----------------------------------------------*/
#ifndef INT8
typedef signed char                        INT8; /* 有符号8位整数 */
#endif

#ifndef INT16
typedef signed short                      INT16; /* 有符号16位整数 */
#endif

#ifndef INT32
typedef signed int                        INT32; /* 有符号32位整数 */
#endif

#ifndef UINT8
typedef unsigned char                     UINT8; /* 无符号8位整数 */
#endif

#ifndef UINT16
typedef unsigned short                   UINT16; /* 无符号16位整数 */
#endif

#ifndef UINT32
typedef unsigned int                     UINT32; /* 无符号32位整数 */
#endif

#ifndef BOOL
typedef signed int                         BOOL; /* 布尔类型,取值为TRUE或FALSE */
#endif

#ifndef BYTE
typedef UINT8                              BYTE; /* 字节类型,无符号8位整数 */
#endif

#ifndef CHAR
typedef char                        CHAR; /* 字符类型,有符号8位整数 */
#endif

#ifndef SHORT
typedef INT16                             SHORT; /* 短整数,有符号16位 */
#endif

#ifndef SHORT
//typedef INT16                             SHORT; /* 短整数,有符号16位 */
#endif

#ifndef USHORT
typedef UINT16                           USHORT; /* 短整数,无符号16位整数 */
#endif

#ifndef LONG
//typedef INT32                              LONG; /* 长整数,有符号32位整数 */
#endif

#ifndef ULONG
#ifdef _X64
typedef unsigned int                      ULONG; /* 长整数,无符号32位整数 */
#else
typedef unsigned long                     ULONG; /* 长整数,无符号32位整数 */
#endif
#endif

#ifndef UINT
typedef UINT32                             UINT; /* 无符号32位整数 */
#endif

#ifndef WORD
//typedef UINT16                             WORD; /* 字类型,无符号16位整数 */
#endif

#ifndef DWORD
//typedef UINT32                            DWORD; /* 双字类型,无符号32位整数 */
#endif

#ifndef FLAGS
typedef UINT32                            FLAGS; /* 标志类型,无符号32位整数 */
#endif

#ifndef LPSTR
typedef CHAR *                            LPSTR; /* 8位字符串指针,按照UTF8格式存储及交换 */
#endif
#endif//0
#ifndef HANDLE
typedef void *                           HANDLE; /* 句柄,指向任意数据对象的起始地址 */
#endif

#ifndef DEVHANDLE
typedef HANDLE                        DEVHANDLE; /* 设备句柄 */
#endif

#ifndef HAPPLICATION
typedef HANDLE                     HAPPLICATION; /* 应用句柄 */
#endif

#ifndef HCONTAINER
typedef HANDLE                       HCONTAINER; /* 容器句柄 */
#endif

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
#ifndef TRUE
#define TRUE		                  0x00000001 /* 布尔值为真 */
#endif

#ifndef FALSE
#define FALSE		                  0x00000000 /* 布尔值为假 */
#endif

#if defined(WIN32) && defined(SYNOTFKEYSKF_EXPORTS)
#define DEVAPI						   __declspec(dllexport) /* 输出函数 */
#else
#define DEVAPI
#endif

#define ADMIN_TYPE	                        0x00 /* 管理员PIN类型 */
#define USER_TYPE	                        0x01 /* 用户PIN类型 */
#pragma pack(push)
#pragma pack(1)		                             /* 设定为1字节对齐 */

/*----------------------------------------------*
 * 版本                                         *
 *----------------------------------------------*/
//typedef struct tagVersion
//{
//    //BYTE major;
//    //BYTE minor;
//    unsigned char major;
//    unsigned char minor;
//} VERSION;

typedef struct Struct_Version
{
    BYTE major;
    BYTE minor;
} VER_NUM;

/*----------------------------------------------*
 * 设备信息                                     *
 *----------------------------------------------*/

typedef struct Struct_DEVINFO
{
/*
    VERSION		Version;
    CHAR		Manufacturer[64];
    CHAR		Issuer[64];
    CHAR		Label[32];
    CHAR		SerialNumber[32];
    VERSION		HWVersion;
    VERSION		FirmwareVersion;
    UINT32		AlgSymCap;
    UINT32		AlgAsymCap;
    UINT32		AlgHashCap;
    UINT32		DevAuthAlgId;
    UINT32		TotalSpace;
    UINT32		FreeSpace;
    BYTE  		Reserved[64];
*/
	VER_NUM		StructVersion;
	VER_NUM    SpecificationVersion;
	CHAR       Manufacturer[64];
	CHAR       Issuer[64];
	CHAR       Label[32];
	CHAR       SerialNumber[32];
	VER_NUM    HWVersion;
	VER_NUM    FirmwareVersion;
	ULONG      AlgSymCap;
	ULONG	   AlgAsymCap;
	ULONG	   AlgHashCap;
	ULONG      DevAuthAlgId;
	ULONG      TotalSpace;
	ULONG      FreeSpace;
    WORD       MaxApduDataLen;
    WORD       UserAuthMethod;
    WORD       DeviceType;
    BYTE       MaxContainerNum;
    BYTE       MaxCertNum;
    WORD       MaxFileNum;
	BYTE	   Rev[54];
} DEVINFO, *PDEVINFO;
#pragma pack(pop)

/*----------------------------------------------*
 * RSA 公钥数据结构                             *
 *----------------------------------------------*/
#define MAX_RSA_MODULUS_BITS                2048
#define MAX_RSA_MODULUS_LEN  ((MAX_RSA_MODULUS_BITS + 7)/8)
#define MAX_RSA_PRIME_BITS   ((MAX_RSA_MODULUS_BITS + 1)/2)
#define MAX_RSA_PRIME_LEN    ((MAX_RSA_PRIME_BITS + 7)/8)
#define MAX_RSA_EXPONENT_LEN                   4

typedef struct Struct_RSAPUBLICKEYBLOB
{
    ULONG	AlgID;
    ULONG	BitLen;
    BYTE	Modulus[MAX_RSA_MODULUS_LEN];
    BYTE	PublicExponent[MAX_RSA_EXPONENT_LEN];
} RSAPUBLICKEYBLOB, *PRSAPUBLICKEYBLOB;

/*----------------------------------------------*
 * RSA 私钥数据结构                             *
 *----------------------------------------------*/
typedef struct Struct_RSAPRIVATEKEYBLOB
{
    ULONG	AlgID;
    ULONG	BitLen;
    BYTE	Modulus[MAX_RSA_MODULUS_LEN];
    BYTE	PublicExponent[MAX_RSA_EXPONENT_LEN];
    BYTE	PrivateExponent[MAX_RSA_MODULUS_LEN];
    BYTE	Prime1[MAX_RSA_MODULUS_LEN/2];
    BYTE	Prime2[MAX_RSA_MODULUS_LEN/2];
    BYTE	Prime1Exponent[MAX_RSA_MODULUS_LEN/2];
    BYTE	Prime2Exponent[MAX_RSA_MODULUS_LEN/2];
    BYTE	Coefficient[MAX_RSA_MODULUS_LEN/2];
} RSAPRIVATEKEYBLOB, *PRSAPRIVATEKEYBLOB;

/*----------------------------------------------*
 * ECC公钥数据结构                              *
 *----------------------------------------------*/
#define ECC_MAX_XCOORDINATE_BITS_LEN         512
#define ECC_MAX_YCOORDINATE_BITS_LEN         512

typedef struct Struct_ECCPUBLICKEYBLOB
{
	ULONG  BitLen;
	BYTE   XCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE   YCoordinate[ECC_MAX_YCOORDINATE_BITS_LEN/8];
} ECCPUBLICKEYBLOB, *PECCPUBLICKEYBLOB;

/*----------------------------------------------*
 * ECC私钥数据结构                              *
 *----------------------------------------------*/
#define ECC_MAX_MODULUS_BITS_LEN             512

typedef struct Struct_ECCPRIVATEKEYBLOB
{
    ULONG	BitLen;
    BYTE	PrivateKey[ECC_MAX_MODULUS_BITS_LEN/8];
} ECCPRIVATEKEYBLOB, *PECCPRIVATEKEYBLOB;

/*----------------------------------------------*
 * ECC密文数据结构                              *
 *----------------------------------------------*/
typedef struct Struct_ECCCIPHERBLOB
{
	BYTE  XCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE  YCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE  HASH[32];
	ULONG CipherLen;
	BYTE  Cipher[2048];
} ECCCIPHERBLOB, *PECCCIPHERBLOB;

/*----------------------------------------------*
 * ECC签名数据结构                              *
 *----------------------------------------------*/
typedef struct Struct_ECCSIGNATUREBLOB
{
    BYTE r[ECC_MAX_XCOORDINATE_BITS_LEN/8];
    BYTE s[ECC_MAX_XCOORDINATE_BITS_LEN/8];
} ECCSIGNATUREBLOB, *PECCSIGNATUREBLOB;

/*----------------------------------------------*
 * 分组密码参数                                 *
 *----------------------------------------------*/
#define MAX_IV_LEN 32

typedef struct Struct_BLOCKCIPHERPARAM
{
    BYTE	IV[MAX_IV_LEN];
    ULONG	IVLen;
    ULONG	PaddingType;
    ULONG	FeedBitLen;
} BLOCKCIPHERPARAM, *PBLOCKCIPHERPARAM;

/*----------------------------------------------*
 * ECC加密密钥对保护结构                        *
 *----------------------------------------------*/
typedef struct SKF_ENVELOPEDKEYBLOB
{
	ULONG Version;
	ULONG ulSymmAlgID;
	ULONG ulBits;
	BYTE cbEncryptedPriKey[64];
	ECCPUBLICKEYBLOB PubKey;
	ECCCIPHERBLOB ECCCipherBlob;
} ENVELOPEDKEYBLOB, *PENVELOPEDKEYBLOB;

/*----------------------------------------------*
 * 文件属性                                     *
 *----------------------------------------------*/
typedef struct Struct_FILEATTRIBUTE
{
	CHAR	FileName[48];                        /* 囯密规范为32 */
	ULONG	FileSize;
	ULONG	ReadRights;
	ULONG	WriteRights;
} FILEATTRIBUTE, *PFILEATTRIBUTE;

/*----------------------------------------------*
 * 权限类型                                     *
 *----------------------------------------------*/
#define SECURE_NEVER_ACCOUNT          0x00000000 /* 不允许 */
#define SECURE_ADM_ACCOUNT            0x00000001 /* 管理员权限 */
#define SECURE_USER_ACCOUNT           0x00000010 /* 用户权限 */
#define SECURE_ANYONE_ACCOUNT         0x000000FF /* 任何人 */

/*----------------------------------------------*
 * 设备状态                                     *
 *----------------------------------------------*/
#define DEV_ABSENT_STATE              0x00000000 /* 设备不存在 */
#define DEV_PRESENT_STATE             0x00000001 /* 设备存在 */
#define DEV_UNKNOW_STATE              0x00000002 /* 设备状态未知 */

/*----------------------------------------------*
 * PIN信息结构定义                              *
 *----------------------------------------------*/
#define KT_SOPIN	                        0x00 /* 管理员 */
#define KT_USERPIN	                        0x01 /* 用户 */
#define KT_SMARTPIN	                        0x02 /* 智能 */
#define KT_SAFE_USERPIN	                    0x03 /* 高安用户 */
#define KT_SAFE_SMARTPIN                    0x04 /* 高安智能 */
#define MIN_PIN_LEN                   0x00000006
#define MAX_PIN_LEN                   0x00000010

typedef struct tagApplicationHandle
{
	CHAR szName[48];
	USHORT usAppID;
	DEVHANDLE phDev;
} APPHANDLE_S;

typedef struct tagContainerHandle
{
	CHAR szName[48];
	USHORT ContainerID;
	USHORT AppID;
	DEVHANDLE phDev;
} CONTAINERHANDLE_S;

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------*
 * 设备管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_WaitForDevEvent(LPSTR szDevName, ULONG *pulDevNameLen, ULONG *pulEvent);

ULONG DEVAPI SKF_CancelWaitForDevEvent();

ULONG DEVAPI SKF_EnumDev(BOOL bPresent, LPSTR szNameList, ULONG *pulSize);

ULONG DEVAPI SKF_ConnectDev(LPSTR szName, DEVHANDLE *phDev);

ULONG DEVAPI SKF_DisConnectDev(DEVHANDLE hDev);

ULONG DEVAPI SKF_GetDevState(LPSTR szDevName, ULONG *pulDevState);

ULONG DEVAPI SKF_SetLabel(DEVHANDLE hDev, CHAR *pcLabel);

ULONG DEVAPI SKF_GetDevInfo(DEVHANDLE hDev, DEVINFO *pDevInfo);

ULONG DEVAPI SKF_LockDev(DEVHANDLE hDev, ULONG ulTimeOut);

ULONG DEVAPI SKF_UnlockDev (DEVHANDLE hDev);

ULONG DEVAPI SKF_Transmit(DEVHANDLE hDev, BYTE* pbCommand, ULONG ulCommandLen,BYTE* pbData, ULONG* pulDataLen);

/*----------------------------------------------*
 * 访问控制                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_ChangeDevAuthKey(DEVHANDLE hDev,
    BYTE *pbKeyValue, ULONG ulKeyLen);

ULONG DEVAPI SKF_DevAuth(DEVHANDLE hDev, BYTE *pbAuthData, ULONG ulLen);

ULONG DEVAPI SKF_ChangePIN(HAPPLICATION hApplication,
    ULONG ulPinType, CHAR *pcOldPin, CHAR *pcNewPin, ULONG *pulRetryCount);

ULONG DEVAPI SKF_GetPINInfo(HAPPLICATION hApplication,
    ULONG ulPINType, ULONG *pulMaxRetryCount,
    ULONG *pulRemainRetryCount, BOOL *pbDefaultPin);

ULONG DEVAPI SKF_VerifyPIN(HAPPLICATION hApplication,
    ULONG ulPinType, CHAR *pcPin, ULONG *pulRetryCount);

ULONG DEVAPI SKF_UnblockPIN(HAPPLICATION hApplication,
    CHAR *pcAdminPIN, CHAR *pcNewUserPIN, ULONG *pulRetryCount);

ULONG DEVAPI SKF_ClearSecureState(HAPPLICATION hApplication);

/*----------------------------------------------*
 * 应用管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_CreateApplication(DEVHANDLE hDev, CHAR *pcAppName,
    CHAR *pcAdminPin, ULONG ulAdminPinRetryCount,
    CHAR *pcUserPin, ULONG ulUserPinRetryCount,
    ULONG ulCreateFileRights, HAPPLICATION *phApplication);

ULONG DEVAPI SKF_OpenApplication(DEVHANDLE hDev,
    CHAR *pcAppName, HAPPLICATION *phApplication);

ULONG DEVAPI SKF_DeleteApplication(DEVHANDLE hDev, LPSTR szAppName);

ULONG DEVAPI SKF_EnumApplication(DEVHANDLE hDev, CHAR *pcAppName, ULONG *pulSize);

ULONG DEVAPI SKF_CloseApplication(HAPPLICATION hApplication);

/*----------------------------------------------*
 * 文件管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_CreateFile(HAPPLICATION hApplication, LPSTR szFileName,
    ULONG ulFileSize, ULONG ulReadRights, ULONG ulWriteRights);

ULONG DEVAPI SKF_DeleteFile(HAPPLICATION hApplication, CHAR *pcFileName);

ULONG DEVAPI SKF_EnumFiles(HAPPLICATION hApplication,
    CHAR *pcFileList, ULONG *pulSize);

ULONG DEVAPI SKF_GetFileInfo(HAPPLICATION hApplication,
    CHAR *pcFileName, FILEATTRIBUTE *pstFileInfo);

ULONG DEVAPI SKF_ReadFile(HAPPLICATION hApplication,
    LPSTR szFileName, ULONG ulOffset, ULONG ulSize,
    BYTE *pbOutData, ULONG *pulOutLen);

ULONG DEVAPI SKF_WriteFile (HAPPLICATION hApplication,
	LPSTR szFileName, ULONG  ulOffset, BYTE *pbData, ULONG ulSize);

/*----------------------------------------------*
 * 容器管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_CreateContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, HCONTAINER *phContainer);

ULONG DEVAPI SKF_DeleteContainer(HAPPLICATION hApplication, LPSTR szContainerName);

ULONG DEVAPI SKF_OpenContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, HCONTAINER *phContainer);

ULONG DEVAPI SKF_CloseContainer(HCONTAINER hContainer);

ULONG DEVAPI SKF_EnumContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, ULONG *pulSize);

ULONG DEVAPI SKF_GetContainerType(HCONTAINER hContainer, ULONG *pulContainerType);

ULONG DEVAPI SKF_ImportCertificate(HCONTAINER hContainer,
    BOOL bSignFlag, BYTE *pbCert, ULONG ulCertLen);

ULONG DEVAPI SKF_ExportCertificate(HCONTAINER hContainer,
    BOOL bSignFlag, BYTE *pbCert, ULONG *pulCertLen);

/*----------------------------------------------*
 * 密码管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_GenRandom(DEVHANDLE hDev, BYTE *pbRandom, ULONG ulRandomLen);

ULONG DEVAPI SKF_GenExtRSAKey(DEVHANDLE hDev, ULONG ulBitsLen, PRSAPRIVATEKEYBLOB pBlob);

ULONG DEVAPI SKF_GenRSAKeyPair(HCONTAINER hContainer, ULONG ulBitsLen, PRSAPUBLICKEYBLOB pBlob);

ULONG DEVAPI SKF_ImportPlainRSAKeyPair(HCONTAINER hContainer, PRSAPRIVATEKEYBLOB pBlob, BOOL bSignKeyPair);

ULONG DEVAPI SKF_ImportRSAKeyPair(HCONTAINER hContainer, ULONG ulSymAlgId,
    BYTE *pbWrappedKey, ULONG ulWrappedKeyLen,
    BYTE *pbEncryptedData, ULONG ulEncryptedDataLen);

ULONG DEVAPI SKF_RSASignData(HCONTAINER hContainer,
    BYTE *pbData, ULONG ulDataLen,
    BYTE *pbSignature, ULONG *pulSignLen);

ULONG DEVAPI SKF_RSAVerify(DEVHANDLE hDev,
    RSAPUBLICKEYBLOB *pRSAPubKeyBlob,
    BYTE *pbData, ULONG  ulDataLen,
    BYTE *pbSignature, ULONG ulSignLen);

ULONG DEVAPI SKF_RSAExportSessionKey(HCONTAINER hContainer, ULONG ulAlgId,
    RSAPUBLICKEYBLOB *pPubKey, BYTE *pbData, ULONG *pulDataLen, HANDLE *phSessionKey);

ULONG DEVAPI SKF_ExtRSAPubKeyOperation(DEVHANDLE hDev,
    PRSAPUBLICKEYBLOB pRSAPubKeyBlob, BYTE *pbInput, ULONG ulInputLen,
    BYTE *pbOutput, ULONG *pulOutputLen);

ULONG DEVAPI SKF_ExtRSAPriKeyOperation(DEVHANDLE hDev,
    PRSAPRIVATEKEYBLOB pRSAPriKeyBlob, BYTE *pbInput, ULONG ulInputLen,
    BYTE *pbOutput, ULONG *pulOutputLen);

ULONG DEVAPI SKF_GenECCKeyPair(HCONTAINER hContainer, ULONG ulAlgId, PECCPUBLICKEYBLOB pBlob);

ULONG DEVAPI SKF_ImportECCKeyPair(HCONTAINER hContainer, PENVELOPEDKEYBLOB pEnvelopedKeyBlob);

ULONG DEVAPI SKF_ECCSignData(HCONTAINER hContainer,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG DEVAPI SKF_ECCVerify(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG DEVAPI SKF_ECCExportSessionKey(HCONTAINER hContainer, ULONG ulAlgId,
    PECCPUBLICKEYBLOB pPubKey, PECCCIPHERBLOB pData, HANDLE *phSessionKey);

ULONG DEVAPI SKF_ECCExportSessionKeyEx(HANDLE hSessionKey,
    PECCPUBLICKEYBLOB pPubKey, PECCCIPHERBLOB pData);

ULONG DEVAPI SKF_ExtECCEncrypt(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbPlainText, ULONG ulPlainTextLen, PECCCIPHERBLOB pCipherText);

ULONG DEVAPI SKF_ExtECCDecrypt(DEVHANDLE hDev, PECCPRIVATEKEYBLOB pECCPriKeyBlob,
    PECCCIPHERBLOB pCipherText, BYTE *pbPlainText, ULONG *pulPlainTextLen);

ULONG DEVAPI SKF_ExtECCSign(DEVHANDLE hDev, PECCPRIVATEKEYBLOB pECCPriKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG DEVAPI SKF_ExtECCVerify(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG DEVAPI SKF_GenerateAgreementDataWithECC(HCONTAINER hContainer,
    ULONG ulAlgId, PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, HANDLE *phAgreementHandle);

ULONG DEVAPI SKF_GenerateAgreementDataAndKeyWithECC(HANDLE hContainer, ULONG ulAlgId,
    PECCPUBLICKEYBLOB pSponsorECCPubKeyBlob,
    PECCPUBLICKEYBLOB pSponsorTempECCPubKeyBlob,
    PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, BYTE *pbSponsorID, ULONG ulSponsorIDLen,
    HANDLE *phKeyHandle);

ULONG DEVAPI SKF_GenerateKeyWithECC(HANDLE hAgreementHandle,
    PECCPUBLICKEYBLOB pECCPubKeyBlob,
    PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, HANDLE *phKeyHandle);

ULONG DEVAPI SKF_ExportPublicKey(HCONTAINER hContainer, BOOL bSignFlag, BYTE *pbBlob, ULONG *pulBlobLen);

ULONG DEVAPI SKF_ImportPublicKey(HCONTAINER hContainer, BOOL bSignFlag, BYTE *pbBlob, ULONG ulBlobLen, HANDLE *phKey);

ULONG DEVAPI SKF_ImportSessionKey(HCONTAINER hContainer,
    ULONG ulAlgId, BYTE *pbWrapedData, ULONG ulWrapedLen, HANDLE *phKey);

ULONG DEVAPI SKF_SetSymmKey(DEVHANDLE hDev,
    BYTE *pbKey, ULONG ulAlgID, HANDLE *phKey);

ULONG DEVAPI SKF_EncryptInit(HANDLE hKey, BLOCKCIPHERPARAM EncryptParam);

ULONG DEVAPI SKF_Encrypt(HANDLE hKey,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen);

ULONG DEVAPI SKF_EncryptUpdate(HANDLE hKey,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen);

ULONG DEVAPI SKF_EncryptFinal(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG *pulEncryptedDataLen);

ULONG DEVAPI SKF_DecryptInit(HANDLE hKey, BLOCKCIPHERPARAM DecryptParam);

ULONG DEVAPI SKF_Decrypt(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG ulEncryptedLen, BYTE *pbData, ULONG *pulDataLen);

ULONG DEVAPI SKF_DecryptUpdate(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG ulEncryptedLen, BYTE *pbData, ULONG *pulDataLen);

ULONG DEVAPI SKF_DecryptFinal(HANDLE hKey,
    BYTE *pbDecryptedData, ULONG *pulDecryptedDataLen);

ULONG DEVAPI SKF_DigestInit(DEVHANDLE hDev, ULONG ulAlgID,
    ECCPUBLICKEYBLOB *pPubKey, BYTE *pbID, ULONG ulIDLen, HANDLE *phHash);

ULONG DEVAPI SKF_Digest(HANDLE hHash,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbHashData, ULONG *pulHashLen);

ULONG DEVAPI SKF_DigestUpdate(HANDLE hHash, BYTE *pbData, ULONG ulDataLen);

ULONG DEVAPI SKF_DigestFinal(HANDLE hHash, BYTE *pbHashData, ULONG *pulHashLen);

ULONG DEVAPI SKF_MacInit(HANDLE hKey, BLOCKCIPHERPARAM *pMacParam, HANDLE *phMac);

ULONG DEVAPI SKF_Mac(HANDLE hMac,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbMacData, ULONG *pulMacLen);

ULONG DEVAPI SKF_MacUpdate(HANDLE hMac, BYTE *pbData, ULONG ulDataLen);

ULONG DEVAPI SKF_MacFinal(HANDLE hMac, BYTE *pbMacData, ULONG *pulMacDataLen);

ULONG DEVAPI SKF_CloseHandle(HANDLE hHandle);

/*----------------------------------------------*
 * 扩展接口                                     *
 *----------------------------------------------*/
BOOL DEVAPI SKF_ExterAuth(HANDLE hHandle, UCHAR *pucKey, ULONG ulLen);

ULONG DEVAPI SKF_EncryptTunnelUpdate(DEVHANDLE hDev,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen);

ULONG DEVAPI SKF_DecryptTunnelUpdate(DEVHANDLE hDev,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbDecryptedData, ULONG *pulDecryptedLen);

ULONG DEVAPI SKF_DevTransmit(DEVHANDLE hDev, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen);

ULONG DEVAPI SKF_AppTransmit(HAPPLICATION hApplication, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen);

ULONG DEVAPI SKF_ConTransmit(HCONTAINER hContainer, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen);

ULONG DEVAPI SKF_SetDevPath(BYTE *pbDevPath);

ULONG DEVAPI SKF_ClearDevPath();


#ifdef __cplusplus
}
#endif

#endif	//_SYNO_TFKEY_SKF_H_

