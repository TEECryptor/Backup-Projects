/***************************************************************************************
* File name    :	SYDes.h
* Function     :	The header of SyDes.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SYDES_H_
#define _SYDES_H_

#define DES_ENCRYPT 1
#define DES_DECRYPT 2 

#define DES_CBC     1
#define DES_ECB     2
#define DES3_CBC    3
#define DES3_ECB    4

//---为了和老的算法库兼容，定义的宏定义---
#define Des_Init DES_Init
#define Des_Run  DES_Run
#define Des3_Run_ECB DES3_Run_ECB
#define Des3_Run_CBC DES3_Run_CBC
#define Des_Run_ECB  DES_Run_ECB
#define Des_Run_CBC  DES_Run_CBC

/*******************************************************************************
函数原型：	UINT8 DES_Init(UINT8* pKey, UINT32 nKeyLen)
函数说明：	初始化DES密钥 。
参数说明：	pKey [I] - 密钥，8字节(DES)、16字节(2Key 3DES)或24字节（3DES）
            nKeyLen[I] --密钥长度，必须为8字节（当为DES加密时）、16字节(2Key 3DES时)或24字节（当为3DES加密时）
返回值：	RT_OK或RT_FAIL 
********************************************************************************/
UINT8 DES_Init(UINT8* pKey,UINT32 nKeyLen);

/*******************************************************************************
函数原型：	UINT8 DES_Run(UINT8 nType, UINT8 nMode, UINT8* pIn, UINT8* pOut, 
							UINT16 nDataLen, UINT8* pIV)
函数说明：	DES加密或解密。
参数说明：	nType [I] - 加密或解密，定义如下：
						#define 	DES_ENCRYPT 		1
						#define 	DES_DECRYPT 		2
			nMode [I] -- ECB或CBC，定义如下：
						#define	DES_CBC		1
						#define 	DES_ECB		2
						#define	DES3_CBC		3
						#define 	DES3_ECB		4
			pIN [I] --- 要加密或解密的数据；
			pOut [O] --- 输出结果；
			nDataLen [I] --- 数据长度，必须为8的倍数，不够请先Padding；
			PIV [IO] --- 输入的IV，并返回IV值，用于CBC模式的下次运算，长度为8字节
注意事项：  因为传入的IV缓冲，同时要接收返回的IV，因此要备份原始的IV。重新运行时需要原始的IV。
返回值：	RT_OK或RT_FAIL
********************************************************************************/
UINT8 DES_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
函数原型：	UINT8 DES_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen )
函数说明：	DES ECB模式加密或解密。
参数说明：  nType [I] --- 加密或解密，定义如下：
						#define 	DES_ENCRYPT 		1
						#define 	DES_DECRYPT 		2
				pIN [I] ---  要加密或解密的数据。
				pOut [O] ---  输出结果。
nDataLen [I] --- 数据长度，必须为8的倍数，不够请先Padding。
返 回 值：	RT_OK或者RT_FAIL 
********************************************************************************/ 
UINT8 DES_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen );

/*******************************************************************************
函数原型：	UINT8 DES_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,
							UINT16 nDataLen,UINT8* pIV)
函数说明：	DES CBC模式加密或解密。
参数说明：	nType [I] - 加密或解密，定义如下：
						#define 	DES_ENCRYPT 		1
						#define 	DES_DECRYPT 		2
			pIN[I] --- 要加密或解密的数据。
			pOut [O] --- 输出结果。
			nDataLen [I] --- 数据长度，必须为8的倍数，不够请先Padding.	
			PIV[I][O] --- 输入的IV，并返回的IV值，用于CBC模式的下次运算，长度为8字节
注意事项：	因为传入的IV缓冲，同时要接收返回的IV，因此要备份原始的IV。重新运行时需要原始的IV。
返 回 值：	RT_OK--成功
            RT_FAIL--失败
********************************************************************************/
UINT8 DES_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
函数原型：	UINT8 DES3_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen )
函数说明：	3DES ECB模式加密或解密,可以初始化为2Key或3Key。
参数说明：  nType [I] --- 加密或解密，定义如下：
								#define 	DES_ENCRYPT 		1
								#define 	DES_DECRYPT 		2
			pIN [I] ---  要加密或解密的数据。
			pOut [O] ---  输出结果。
nDataLen [I] --- 数据长度，必须为8的倍数，不够请先Padding。
返 回 值：	RT_OK或者RT_FAIL
********************************************************************************/
UINT8 DES3_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen );

/*******************************************************************************
 函数原型：	UINT8 DES3_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,
UINT16 nDataLen,UINT8* pIV)
函数说明：	3DES CBC模式加密或解密,可以初始化为2Key或3Key。
参数说明：	nType [I] - 加密或解密，定义如下：
						#define 	DES_ENCRYPT 		1
						#define 	DES_DECRYPT 		2
			pIN[I] --- 要加密或解密的数据。
			pOut [O] --- 输出结果。
			nDataLen [I] --- 数据长度，必须为8的倍数，不够请先Padding.	
			PIV[I][O] --- 输入的IV，并返回的IV值，用于CBC模式的下次运算，长度为8字节
注意事项：	因为传入的IV缓冲，同时要接收返回的IV，因此要备份原始的IV。重新运行时需要原始的IV。
返 回 值：	RT_OK--成功
			RT_FAIL--失败 
********************************************************************************/
UINT8 DES3_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
函数原型：	UINT8 _Des_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen)
函数说明：	DES加密或解密。
参数说明：	nType [I] - 加密或解密，定义如下：
						#define 	DES_ENCRYPT 		1
						#define 	DES_DECRYPT 		2
			nAddr [I][O] --- 要加密或解密的数据起始地址（可用空间为0x20004000~0x20008000）；
							 处理结果的数据也存放在以该地址为起始的空间中；
			nDataLen [I] --- 数据长度，必须为8的倍数，不够请先Padding；
注意事项：  该函数直接调用算法加速器，因此需要用户将处理数据放到运算空间地址中。
			该函数只做ECB模式运算。
返回值：	RT_OK或RT_FAIL
********************************************************************************/
UINT8 _Des_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen);

#endif
