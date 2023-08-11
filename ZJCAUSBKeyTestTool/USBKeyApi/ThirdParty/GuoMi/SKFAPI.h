
#ifndef _SKFAPI_H_
#define _SKFAPI_H_

#include <windows.h>

#pragma pack(1)

//返回值定义
#define SAR_OK							0x00000000
#define SAR_FAIL						0x0A000001
#define SAR_UNKNOWNERR					0x0A000002
#define SAR_NOTSUPPORTYETERR			0x0A000003
#define SAR_FILEERR						0x0A000004
#define SAR_INVALIDHANDLEERR			0x0A000005
#define SAR_INVALIDPARAMERR				0x0A000006
#define SAR_READFILEERR					0x0A000007
#define SAR_WRITEFILEERR				0x0A000008
#define SAR_NAMELENERR					0x0A000009
#define SAR_KEYUSAGEERR					0x0A00000A
#define SAR_MODULUSLENERR				0x0A00000B
#define SAR_NOTINITIALIZEERR			0x0A00000C
#define SAR_OBJERR						0x0A00000D
#define SAR_MEMORYERR					0x0A00000E
#define SAR_TIMEOUTERR					0x0A00000F
#define SAR_INDATALENERR				0x0A000010
#define SAR_INDATAERR					0x0A000011
#define SAR_GENRANDERR					0x0A000012
#define SAR_HASHOBJERR					0x0A000013
#define SAR_HASHERR						0x0A000014
#define SAR_GENRSAKEYERR				0x0A000015
#define SAR_RSAMODULUSLENERR			0x0A000016
#define SAR_CSPIMPRTPUBKEYERR			0x0A000017
#define SAR_RSAENCERR					0x0A000018
#define SAR_RSADECERR					0x0A000019
#define SAR_HASHNOTEQUALERR				0x0A00001A
#define SAR_KEYNOTFOUNTERR				0x0A00001B
#define SAR_CERTNOTFOUNTERR				0x0A00001C
#define SAR_NOTEXPORTERR				0x0A00001D
#define SAR_DECRYPTPADERR				0x0A00001E
#define SAR_MACLENERR					0x0A00001F
#define SAR_BUFFER_TOO_SMALL			0x0A000020
#define SAR_KEYINFOTYPEERR				0x0A000021
#define SAR_NOT_EVENTERR				0x0A000022
#define SAR_DEVICE_REMOVED				0x0A000023
#define SAR_PIN_INCORRECT				0x0A000024
#define SAR_PIN_LOCKED					0x0A000025
#define SAR_PIN_INVALID					0x0A000026
#define SAR_PIN_LEN_RANGE				0x0A000027
#define SAR_USER_ALREADY_LOGGED_IN		0x0A000028
#define SAR_USER_PIN_NOT_INITIALIZED	0x0A000029
#define SAR_USER_TYPE_INVALID			0x0A00002A
#define SAR_APPLICATION_NAME_INVALID	0x0A00002B
#define SAR_APPLICATION_EXISTS			0x0A00002C
#define SAR_USER_NOT_LOGGED_IN			0x0A00002D
#define SAR_APPLICATION_NOT_EXISTS		0x0A00002E
#define SAR_FILE_ALREADY_EXIST			0x0A00002F
#define SAR_NO_ROOM						0x0A000030
#define SAR_FILE_NOT_EXIST				0x0A000031
//
#define SAR_GENERATESYMKEYERR			0x0A000032
#define SAR_CONTAINER_ALREADY_EXIST		0x0A000033
#define SAR_CONTAINER_NOT_EXIST			0x0A000034
#define SAR_CONTAINER_NOT_OPEN			0x0A000035

//分组加密算法标识
#define SGD_SM1_ECB						0x00000101
#define SGD_SM1_CBC						0x00000102
#define SGD_SM1_CFB						0x00000104
#define SGD_SM1_OFB						0x00000108
#define SGD_SM1_MAC						0x00000110
#define SGD_SSF33_ECB					0x00000201
#define SGD_SSF33_CBC					0x00000202
#define SGD_SSF33_CFB					0x00000204
#define SGD_SSF33_OFB					0x00000208
#define SGD_SSF33_MAC					0x00000210
#define SGD_SMS4_ECB					0x00000401
#define SGD_SMS4_CBC					0x00000402
#define SGD_SMS4_CFB					0x00000404
#define SGD_SMS4_OFB					0x00000408
#define SGD_SMS4_MAC					0x00000410
#define	SGD_AES128_CBC					0x00000501	//AES 128位(16bytes) CBC加密算法
#define	SGD_AES128_ECB					0x00000502	//AES 128位(16bytes) ECB加密算法
#define	SGD_AES192_CBC					0x00000504  //AES 192位(24bytes) CBC加密算法 
#define	SGD_AES192_ECB					0x00000508  //AES 192位(24bytes) ECB加密算法 
#define	SGD_AES256_CBC					0x00000510	//AES 256位(32bytes) CBC加密算法 
#define	SGD_AES256_ECB					0x00000511	//AES 256位(32bytes) ECB加密算法

//非对称密码算法标识
#define SGD_RSA							0x00010000
#define SGD_SM2_1						0x00020100
#define SGD_SM2_2						0x00020200
#define SGD_SM2_3						0x00020400

//密码杂凑算法标识
#define SGD_SM3							0x00000001
#define SGD_SHA1						0x00000002
#define SGD_SHA256						0x00000004
//not in gm spec
#define SGD_MD5							0x00000003

//权限类型
#define SECURE_NEVER_ACCOUNT			0x00000000	//不允许
#define SECURE_ADM_ACCOUNT				0x00000001	//管理员权限
#define SECURE_USER_ACCOUNT				0x00000010	//用户权限
#define SECURE_ANYONE_ACCOUNT			0x000000FF	//任何人

//PIN码类型
#define	ADMIN_TYPE						0x00000000	// 管理员PIN
#define USER_TYPE						0x00000001	// 用户PIN

//设备状态
#define DEV_ABSENT_STATE				0x00000000	//设备不存在	
#define DEV_PRESENT_STATE				0x00000001//设备存在
#define DEV_UNKNOW_STATE				0x00000002

#define MAX_IV_LEN 32
#define APPFILE 0x7430
#define ALG_SSF33	1
#define ALG_SM1		2

#define TRANS_TYPE_PUBKEY_SKF2HS	1
#define TRANS_TYPE_PUBKEY_HS2SKF	2
#define TRANS_TYPE_PRIKEY_SKF2HS	3
#define TRANS_TYPE_PRIKEY_HS2SKF	4

#define ALG_SM3		0x40
#define ALG_SHA1	0x80
#define ALG_SHA256	0x100
#define ALG_MD5		0x8003

// 版本
typedef struct Struct_Version{
	BYTE major;		//主版本号
	BYTE minor;		//次版本号
}VERSION;


// 设备信息 (海泰定义)
//typedef struct Struct_DEVINFO{
//	VERSION Version;
//	CHAR Manufacturer[64];
//	CHAR Issuer[64];
//	CHAR Label[32];
//	CHAR SerialNumber[32];
//	VERSION HWVersion;
//	VERSION FirmwareVersion;
//	ULONG AlgSymCap;
//	ULONG AlgAsymCap;
//	ULONG AlgHashCap;
//	ULONG DevAuthAlgId;
//	ULONG TotalSpace;
//	ULONG FreeSpace;
//	BYTE Reserved[64];
//}DEVINFO, *PDEVINFO;

/*
 *设备信息 (飞天定义)
 */
typedef struct Struct_DEVINFO{
	VERSION Version;			
	CHAR	Manufacturer[64];		//设备厂商信息
	CHAR	Issuer[64];				//应用发行者信息
	CHAR	Label[32];				//标签
	CHAR	SerialNumber[32];		//序列号
	VERSION HWVersion;				//设备硬件版本
	VERSION FirmwareVersion;		//设备本身固件版本
	ULONG		AlgSymCap;
	ULONG		AlgAsymCap;
	ULONG		AlgHashCap;
	ULONG		DevAuthAlgId;		//设备认证密钥的ID
	ULONG		TotalSpace;
	ULONG		FreeSpace;
	ULONG		MaxECCBufferSize;	//能够处理的ECC加密数据大小
	ULONG		MaxBufferSize;		//能够处理的分组运算和杂凑运算的数据大小
	BYTE  		Reserved[64];
}DEVINFO,*PDEVINFO;

typedef struct IndexSt
{
	DWORD dwID;
	char szName[32];
}INDEXFILETABLE, *PINDEXFILETABLE;


// 设备初始化信息设备类型：
typedef struct Struct_DEVINITINFO
{
	VERSION AppVersion;
	CHAR Manufacturer[64];	//设备厂商信息,最长64个字符，不足64个字符以空白字符(ASCII码为0xFF)填充，不能以null（0x00）结束。
	CHAR Label[64];	//SIC/SZD标签,最长64个字符，不足64个字符以空白字符(ASCII码为0xFF)填充，不能以null（0x00）结束。
}DEVINITINFO,*PDEVINITINFO;


#define MAX_RSA_MODULUS_LEN 256
#define MAX_RSA_EXPONENT_LEN 4

// RSA公钥交换数据块
typedef struct Struct_RSAPUBLICKEYBLOB{
	ULONG	AlgID;
	ULONG	BitLen;
	BYTE	Modulus[MAX_RSA_MODULUS_LEN];
	BYTE	PublicExponent[MAX_RSA_EXPONENT_LEN];
}RSAPUBLICKEYBLOB, *PRSAPUBLICKEYBLOB;

// RSA私钥交换数据块
typedef struct Struct_RSAPRIVATEKEYBLOB{
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
}RSAPRIVATEKEYBLOB, *PRSAPRIVATEKEYBLOB;


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


#define ECC_MAX_XCOORDINATE_BITS_LEN 512
#define ECC_MAX_YCOORDINATE_BITS_LEN 512

// ECC公钥交换数据块
typedef struct Struct_ECCPUBLICKEYBLOB{
	ULONG	BitLen;
	BYTE	XCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE	YCoordinate[ECC_MAX_YCOORDINATE_BITS_LEN/8];
}ECCPUBLICKEYBLOB, *PECCPUBLICKEYBLOB;

#define ECC_MAX_MODULUS_BITS_LEN 512

// ECC私钥交换数据块
typedef struct Struct_ECCPRIVATEKEYBLOB{
	ULONG	BitLen;
	BYTE	PrivateKey[ECC_MAX_MODULUS_BITS_LEN/8];
}ECCPRIVATEKEYBLOB, *PECCPRIVATEKEYBLOB;


// ECC密文数据结构
typedef struct Struct_ECCCIPHERBLOB{
	BYTE  XCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE  YCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE  Hash[32];
	ULONG CipherLen;
	BYTE  Cipher[1];
}ECCCIPHERBLOB, *PECCCIPHERBLOB;


// ECC签名数据结构
typedef struct Struct_ECCSIGNATUREBLOB{
	BYTE r[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE s[ECC_MAX_XCOORDINATE_BITS_LEN/8];
}ECCSIGNATUREBLOB, *PECCSIGNATUREBLOB;


typedef struct SKF_ENVELOPEDKEYBLOB{
	ULONG Version;                  // 当前版本为 1
	ULONG ulSymmAlgID;              // 规范中的算法标识，限定ECB模式
	ULONG ulBits;					// 加密密钥对的密钥位长度
	BYTE cbEncryptedPriKey[64];     // 加密保护的加密私钥
	ECCPUBLICKEYBLOB PubKey;        // 加密公钥
	ECCCIPHERBLOB ECCCipherBlob;    // SM2 公钥加密的密钥加密密钥
}ENVELOPEDKEYBLOB, *PENVELOPEDKEYBLOB;


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


// 分组密码参数
typedef struct Struct_BLOCKCIPHERPARAM{
	BYTE	IV[MAX_IV_LEN];
	ULONG	IVLen;
	ULONG	PaddingType;
	ULONG	FeedBitLen;
} BLOCKCIPHERPARAM, *PBLOCKCIPHERPARAM;

// 文件属性
typedef struct Struct_FILEATTRIBUTE{
	CHAR	FileName[32];
	ULONG	FileSize;
	ULONG	ReadRights;
	ULONG	WriteRights; 
}FILEATTRIBUTE, *PFILEATTRIBUTE;



typedef HANDLE DEVHANDLE;
typedef HANDLE HAPPLICATION;
typedef HANDLE HCONTAINER;

#endif //_SKFAPI_H_