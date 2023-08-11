/******************************************************************************

                  版权所有 (C), 2013-2023, 安徽云盾信息技术有限公司

 ******************************************************************************
  文 件 名   : template.h
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

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "internal.h"
//#include "ica_types.h"
#include "Common.h"

/* RAS公钥对象模板 */
CK_ATTRIBUTE g_astRSAPublicKeyTemplate[10];

/* RAS公钥对象属性个数 */
#define CK_I_RSA_PUBLIC_KEY_ATTR_COUNT      10

/* RAS私钥对象模板 */
CK_ATTRIBUTE g_astRSAPrivateKeyTemplate[8];

/* RAS私钥对象属性个数 */
#define CK_I_RSA_PRIVATE_KEY_ATTR_COUNT     8

/* DES保密秘钥模板 */
CK_ATTRIBUTE g_astDESKeyTemplate[10];

/* DES保密秘钥对象属性个数 */
#define CK_I_DES_SECRET_KEY_ATTR_COUNT      10


CK_ATTRIBUTE g_astFindRSATemplate[2];

CK_ATTRIBUTE g_astGetRSAKeyIDTemplate[1];

CK_ATTRIBUTE g_astGetRSAPublicKeyTemplate[1];

/* 查找RSA秘钥对象属性个数 */
#define CK_I_RSA_FIND_ATTR_COUNT            2

/* 定义RAS机制 */
CK_MECHANISM g_stMechGenRSA;
CK_MECHANISM g_stMechRSA;

/* DES机制 */
CK_MECHANISM g_stMechGenDES;
CK_MECHANISM g_stDESmech;

#endif /* TEMPLATE_H */