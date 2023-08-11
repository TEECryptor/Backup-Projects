/***************************************************************************
 *	File name:Defs.h
 *	Introduce:Syno SDK test common and macro definition
 *	Author:Syno common
 *	Date:2015/11/07
 *  Version:v1.0
 **************************************************************************/
#ifndef _DEFS_H_
#define _DEFS_H_

#define	USBKEY_ASYMMALG_RSA			0x01
#define	USBKEY_ASYMMALG_SM2			0x02

#define	USBKEY_SYMMALG_DES			0x04
#define	USBKEY_SYMMALG_3DES			0x08
#define	USBKEY_SYMMALG_SM1_ECB		0x10
#define	USBKEY_SYMMALG_SMS4_ECB		0x20
#define	USBKEY_SYMMALG_SSF33_ECB	0x40

#define USBKEY_HASHALG_SHA1			0x01
#define USBKEY_HASHALG_SHA256		0x02
#define USBKEY_HASHALG_SM3			0x03
#define USBKEY_HASHALG_MD5			0x04

#define USBKEY_SIGNALG_RSA			0x01
#define USBKEY_SIGNALG_RSA_MD5		0x02
#define USBKEY_SIGNALG_RSA_SHA1		0x03
#define USBKEY_SIGNALG_RSA_SHA256	0x04
#define USBKEY_SIGNALG_SM2			0x05
#define USBKEY_SIGNALG_SM2_SM3		0x06

#define	USBKEY_CRYPTLEN_PERTIME		1024

/* 错误代码定义 */
#define	USBKEY_ERR_OK				0x00000000
#define	USBKEY_ERR_FAILED			0x00000001
#define	USBKEY_ERR_INVALIDPARAM		0x00000002
#define	USBKEY_ERR_INVALIDCALL		0x00000003
#define	USBKEY_ERR_BUFFER_TOO_SMALL	0x00000004
#define	USBKEY_ERR_NOT_SUPPORT		0x00000005
#define	USBKEY_ERR_NO_KEYSET		0x00000006
#define USBKEY_ERR_NO_CERT			0x00000007
#define USBKEY_ERR_NO_DEVICE		0x00000008
#define USBKEY_ERR_NO_APP			0x00000009
#define	USBKEY_ERR_INDATAERROR		0x0000000A
#define	USBKEY_ERR_NO_INTERFACE		0x0000000B
#define	USBKEY_ERR_VERIFYFAILED		0x0000000C

/* SDK类型枚举 */
enum SDK_TYPE
{
	SDK_TYPE_SKF		= 1,
	SDK_TYPE_PKCS11		= 2,
	SDK_TYPE_CNT
};

/* 用户类型枚举 */
enum USR_TYPE
{
	USER_TYPE_UNKNOW	= 0,
	USER_TYPE_SO		= 1,
	USER_TYPE_USER		= 2,
	USER_TYPE_CNT
};

/* 密钥/证书用途类型枚举 */
enum USAGE_TYPE
{
	USAGE_TYPE_UNKNOW	= 0,
	USAGE_TYPE_SIGN		= 1,
	USAGE_TYPE_EXCH		= 2,
	USAGE_TYPE_CNT
};

#endif	//_DEFS_H_