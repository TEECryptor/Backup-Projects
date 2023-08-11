/***************************************************************************
 *	File name:template.h
 *	Introduce:PKCS11 template object functions definiton
 *	Author:Syno common
 *	Date:2015/11/04
 *  Version:v1.0
 **************************************************************************/

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "internal.h"
#include "SynoHeaders.h"

/* RAS��Կ����ģ�� */
CK_ATTRIBUTE g_astRSAPublicKeyTemplate[10];

/* RAS��Կ�������Ը��� */
#define CK_I_RSA_PUBLIC_KEY_ATTR_COUNT      10

/* RAS˽Կ����ģ�� */
CK_ATTRIBUTE g_astRSAPrivateKeyTemplate[8];

/* RAS˽Կ�������Ը��� */
#define CK_I_RSA_PRIVATE_KEY_ATTR_COUNT     8

/* DES������Կģ�� */
CK_ATTRIBUTE g_astDESKeyTemplate[10];

/* DES������Կ�������Ը��� */
#define CK_I_DES_SECRET_KEY_ATTR_COUNT      10


CK_ATTRIBUTE g_astFindRSATemplate[2];

CK_ATTRIBUTE g_astGetRSAKeyIDTemplate[1];

CK_ATTRIBUTE g_astGetRSAPublicKeyTemplate[1];

/* ����RSA��Կ�������Ը��� */
#define CK_I_RSA_FIND_ATTR_COUNT            2

/* ����RAS���� */
CK_MECHANISM g_stMechGenRSA;
CK_MECHANISM g_stMechRSA;

/* DES���� */
CK_MECHANISM g_stMechGenDES;
CK_MECHANISM g_stDESmech;

#endif /* TEMPLATE_H */