/***************************************************************************
 *	File name:defines.h
 *	Introduce:Head file for Syno TF KEY common definition
 *	Author:Syno common
 *	Date:2015/11/18
 *  Version:v1.0
 **************************************************************************/

#ifndef _DEFINES_H_
#define _DEFINES_H_

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "SynoHeaders.h"

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

#define SGD_AES128					  0x00000500 /* AES 128位(16bytes) 加密算法，生成密钥时使用 */
#define	SGD_AES128_CBC	              0x00000501 /* AES 128位(16bytes) CBC加密算法 */
#define	SGD_AES128_ECB	              0x00000502 /* AES 128位(16bytes) ECB加密算法 */
#define SGD_AES192					  0x00000505 /* AES 192位(24bytes) 加密算法，生成密钥时使用 */
#define	SGD_AES192_CBC	              0x00000506 /* AES 192位(24bytes) CBC加密算法 */
#define	SGD_AES192_ECB	              0x00000507 /* AES 192位(24bytes) ECB加密算法 */
#define SGD_AES256					  0x00000510 /* AES 256位(32bytes) 加密算法，生成密钥时使用 */
#define	SGD_AES256_CBC	              0x00000511 /* AES 256位(32bytes) CBC加密算法 */
#define	SGD_AES256_ECB	              0x00000512 /* AES 256位(32bytes) ECB加密算法 */

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
#define SGD_SHA256                    0x00000003 /* SHA256密码杂凑算法 */

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

#define ADMIN_TYPE	                        0x00 /* 管理员PIN类型 */
#define USER_TYPE	                        0x01 /* 用户PIN类型 */

#pragma pack(push)
#pragma pack(1)		                             /* 设定为1字节对齐 */

/*----------------------------------------------*
 * 版本                                         *
 *----------------------------------------------*/
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


#endif /* _DEFINES_H_ */
