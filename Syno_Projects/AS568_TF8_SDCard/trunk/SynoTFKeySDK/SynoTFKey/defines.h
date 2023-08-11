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
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "SynoHeaders.h"

/*----------------------------------------------*
 * ���������㷨��ʶ                             *
 *----------------------------------------------*/
#define SGD_SM1_ECB	                  0x00000101 /* SM1�㷨ECB����ģʽ */
#define SGD_SM1_CBC	                  0x00000102 /* SM1�㷨CBC����ģʽ */
#define SGD_SM1_CFB	                  0x00000104 /* SM1�㷨CFB����ģʽ */
#define SGD_SM1_OFB	                  0x00000108 /* SM1�㷨OFB����ģʽ */
#define SGD_SM1_MAC	                  0x00000110 /* SM1�㷨MAC���� */

#define SGD_SSF33_ECB	              0x00000201 /* SSF33�㷨ECB����ģʽ */
#define SGD_SSF33_CBC	              0x00000202 /* SSF33�㷨CBC����ģʽ */
#define SGD_SSF33_CFB	              0x00000204 /* SSF33�㷨CFB����ģʽ */
#define SGD_SSF33_OFB	              0x00000208 /* SSF33�㷨OFB����ģʽ */
#define SGD_SSF33_MAC	              0x00000210 /* SSF33�㷨MAC���� */

#define SGD_SMS4_ECB	              0x00000401 /* SMS4�㷨ECB����ģʽ */
#define SGD_SMS4_CBC	              0x00000402 /* SMS4�㷨CBC����ģʽ */
#define SGD_SMS4_CFB	              0x00000404 /* SMS4�㷨CFB����ģʽ */
#define SGD_SMS4_OFB	              0x00000408 /* SMS4�㷨OFB����ģʽ */
#define SGD_SMS4_MAC	              0x00000410 /* SMS4�㷨MAC���� */

#define SGD_AES128					  0x00000500 /* AES 128λ(16bytes) �����㷨��������Կʱʹ�� */
#define	SGD_AES128_CBC	              0x00000501 /* AES 128λ(16bytes) CBC�����㷨 */
#define	SGD_AES128_ECB	              0x00000502 /* AES 128λ(16bytes) ECB�����㷨 */
#define SGD_AES192					  0x00000505 /* AES 192λ(24bytes) �����㷨��������Կʱʹ�� */
#define	SGD_AES192_CBC	              0x00000506 /* AES 192λ(24bytes) CBC�����㷨 */
#define	SGD_AES192_ECB	              0x00000507 /* AES 192λ(24bytes) ECB�����㷨 */
#define SGD_AES256					  0x00000510 /* AES 256λ(32bytes) �����㷨��������Կʱʹ�� */
#define	SGD_AES256_CBC	              0x00000511 /* AES 256λ(32bytes) CBC�����㷨 */
#define	SGD_AES256_ECB	              0x00000512 /* AES 256λ(32bytes) ECB�����㷨 */

/*----------------------------------------------*
 * �ǶԳ������㷨��ʶ                           *
 *----------------------------------------------*/
#define SGD_RSA                       0x00010000 /* RSA�㷨 */
#define SGD_SM2_1                     0x00020100 /* ��Բ����ǩ���㷨 */
#define SGD_SM2_2                     0x00020200 /* ��Բ������Կ����Э�� */
#define SGD_SM2_3                     0x00020400 /* ��Բ���߼����㷨 */

/*----------------------------------------------*
 * �Ӵ������㷨��ʶ                             *
 *----------------------------------------------*/
#define SGD_SM3                       0x00000001 /* SM3�����Ӵ��㷨 */
#define SGD_SHA1                      0x00000002 /* SHA1�����Ӵ��㷨 */
#define SGD_SHA256                    0x00000003 /* SHA256�����Ӵ��㷨 */

#ifndef HANDLE
typedef void *                           HANDLE; /* ���,ָ���������ݶ������ʼ��ַ */
#endif

#ifndef DEVHANDLE
typedef HANDLE                        DEVHANDLE; /* �豸��� */
#endif

#ifndef HAPPLICATION
typedef HANDLE                     HAPPLICATION; /* Ӧ�þ�� */
#endif

#ifndef HCONTAINER
typedef HANDLE                       HCONTAINER; /* ������� */
#endif

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
#ifndef TRUE
#define TRUE		                  0x00000001 /* ����ֵΪ�� */
#endif

#ifndef FALSE
#define FALSE		                  0x00000000 /* ����ֵΪ�� */
#endif

#define ADMIN_TYPE	                        0x00 /* ����ԱPIN���� */
#define USER_TYPE	                        0x01 /* �û�PIN���� */

#pragma pack(push)
#pragma pack(1)		                             /* �趨Ϊ1�ֽڶ��� */

/*----------------------------------------------*
 * �汾                                         *
 *----------------------------------------------*/
typedef struct Struct_Version
{
    BYTE major;
    BYTE minor;
} VER_NUM;

/*----------------------------------------------*
 * �豸��Ϣ                                     *
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
 * RSA ��Կ���ݽṹ                             *
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
 * RSA ˽Կ���ݽṹ                             *
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
 * ECC��Կ���ݽṹ                              *
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
 * ECC˽Կ���ݽṹ                              *
 *----------------------------------------------*/
#define ECC_MAX_MODULUS_BITS_LEN             512

typedef struct Struct_ECCPRIVATEKEYBLOB
{
    ULONG	BitLen;
    BYTE	PrivateKey[ECC_MAX_MODULUS_BITS_LEN/8];
} ECCPRIVATEKEYBLOB, *PECCPRIVATEKEYBLOB;

/*----------------------------------------------*
 * ECC�������ݽṹ                              *
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
 * ECCǩ�����ݽṹ                              *
 *----------------------------------------------*/
typedef struct Struct_ECCSIGNATUREBLOB
{
    BYTE r[ECC_MAX_XCOORDINATE_BITS_LEN/8];
    BYTE s[ECC_MAX_XCOORDINATE_BITS_LEN/8];
} ECCSIGNATUREBLOB, *PECCSIGNATUREBLOB;

/*----------------------------------------------*
 * �����������                                 *
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
 * ECC������Կ�Ա����ṹ                        *
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
 * �ļ�����                                     *
 *----------------------------------------------*/
typedef struct Struct_FILEATTRIBUTE
{
	CHAR	FileName[48];                        /* ���ܹ淶Ϊ32 */
	ULONG	FileSize;
	ULONG	ReadRights;
	ULONG	WriteRights;
} FILEATTRIBUTE, *PFILEATTRIBUTE;

/*----------------------------------------------*
 * Ȩ������                                     *
 *----------------------------------------------*/
#define SECURE_NEVER_ACCOUNT          0x00000000 /* ������ */
#define SECURE_ADM_ACCOUNT            0x00000001 /* ����ԱȨ�� */
#define SECURE_USER_ACCOUNT           0x00000010 /* �û�Ȩ�� */
#define SECURE_ANYONE_ACCOUNT         0x000000FF /* �κ��� */

/*----------------------------------------------*
 * �豸״̬                                     *
 *----------------------------------------------*/
#define DEV_ABSENT_STATE              0x00000000 /* �豸������ */
#define DEV_PRESENT_STATE             0x00000001 /* �豸���� */
#define DEV_UNKNOW_STATE              0x00000002 /* �豸״̬δ֪ */

/*----------------------------------------------*
 * PIN��Ϣ�ṹ����                              *
 *----------------------------------------------*/
#define KT_SOPIN	                        0x00 /* ����Ա */
#define KT_USERPIN	                        0x01 /* �û� */
#define KT_SMARTPIN	                        0x02 /* ���� */
#define KT_SAFE_USERPIN	                    0x03 /* �߰��û� */
#define KT_SAFE_SMARTPIN                    0x04 /* �߰����� */
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
