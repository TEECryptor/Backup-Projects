﻿/******************************************************
*	File Name: CommonDef.h
*	Author: Youfang Yao
*	Date: 13/08/2015
*	Introduce: The common definition for USBKeyApi project
******************************************************/
#ifndef _USBKEY_COMMONDEF_H_
#define _USBKEY_COMMONDEF_H_

#define	USBKEY_ASYMMALG_RSA			0x01
#define	USBKEY_ASYMMALG_SM2			0x02

#define	USBKEY_SYMMALG_3DES			0x01
#define	USBKEY_SYMMALG_SM1_ECB		0x02
#define	USBKEY_SYMMALG_SMS4_ECB		0x03
#define	USBKEY_SYMMALG_SSF33_ECB	0x04

#define USBKEY_HASHALG_SHA1			0x01
#define USBKEY_HASHALG_SHA256		0x02
#define USBKEY_HASHALG_SM3			0x03
#define USBKEY_HASHALG_MD5			0x04

#define	USBKEY_CRYPTLEN_PERTIME		1024

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

#endif	//_USBKEY_COMMONDEF_H_

