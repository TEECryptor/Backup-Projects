/***************************************************************************
 *	File name:SynoDataPadding.h
 *	Introduce:Syno common data padding functions definition
 *	Author:Syno common
 *	Date:2016/03/13
 *  Version:v1.0
 **************************************************************************/

#ifndef _SYNO_DATAPADDING_H_
#define _SYNO_DATAPADDING_H_


/*----------------------------------------------*
 * 关于数据加密时自动补齐、以及解密时自动去掉补码的函数*
 *----------------------------------------------*/
int PKCS7_AddPadding(unsigned char* lpPadded, unsigned long ulPaddedLen);
int PKCS7_RemovePadding(unsigned char* lpData, unsigned long *pulDataLen);

#endif /* _SYNO_DATAPADDING_H_ */


