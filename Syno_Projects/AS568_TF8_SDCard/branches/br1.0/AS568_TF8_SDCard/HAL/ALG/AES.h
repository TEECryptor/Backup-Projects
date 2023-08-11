/***************************************************************************************
* File name    :	AES.h
* Function     :	The header of AES.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef AES_H_
#define AES_H_

#define AES_ENCRYPT 1
#define AES_DECRYPT 2 

#define AES128_CBC 1
#define AES128_ECB 2 
#define AES192_CBC 3
#define AES192_ECB 4
#define AES256_CBC 5
#define AES256_ECB 6 

//---为了和老的算法库兼容，定义的宏定义---
#define  Aes_Init AES_Init 
#define  Aes_Run  AES_Run

/*******************************************************************************
函数原型：	UINT8 AES_Init(UINT8* pKey, UINT32 nKeyLen)
函数说明：	初始化Aes密钥 。
参数说明：	pKey [I] --- 密钥；
            nKeyLen [I] --- 密钥长度，必须为16字节（当为128bit加密时）或24字节（当为192bit加密时）或32字节(当为256bit加密时)。
返回值：	RT_OK或RT_FAIL 
********************************************************************************/ 
UINT8 AES_Init(UINT8* pKey,UINT32 nKeyLen);

/*******************************************************************************
函数原型：	UINT8 AES_Run(UINT8 nType, UINT8 nMode, UINT8* pIn, UINT8* pOut, 
UINT16 nDataLen, UINT8* pIV)
函数说明：	AES加密或解密。
参数说明：	nType [I] --- 加密或解密，定义如下：
							#define 	AES_ENCRYPT 		1
							#define 	AES_DECRYPT 		2
			nMode [I] --- ECB或CBC，定义如下：
							#define	AES128_CBC 		1
							#define	AES128_ECB 		2 
							#define	AES192_CBC 		3
							#define	AES192_ECB 		4
							#define	AES256_CBC 		5
							#define	AES256_ECB 		6 
			pIN [I] --- 要加密或解密的数据；
			pOut [O] --- 输出结果；
			nDataLen [I] --- 数据长度，必须为16的倍数，不够请先Padding；
			PIV[IO] --- 输入的IV，并返回IV值，用于CBC模式的下次运算，长度为16字节。
注意事项：因为传入的IV缓冲，同时要接收返回的IV，因此要备份原始的IV。重新运行时需要原始的IV。
返回值：	RT_OK或RT_FAIL
********************************************************************************/
UINT8 AES_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
函数原型：	UINT8 _Aes_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen)
函数说明：	AES加密或解密。
参数说明：	nType [I] - 加密或解密，定义如下：
						#define 	AES_ENCRYPT 		1
						#define 	AES_DECRYPT 		2
			nMode [I] --- ECB，定义如下：
							#define	AES128_ECB 		2 
							#define	AES192_ECB 		4
							#define	AES256_ECB 		6 
			nAddr [I][O] --- 要加密或解密的数据起始地址（可用空间为0x20004000~0x20008000）；
							 处理结果的数据也存放在以该地址为起始的空间中；
			nDataLen [I] --- 数据长度，必须为16的倍数，不够请先Padding；
注意事项：  该函数直接调用算法加速器，因此需要用户将处理数据放到运算空间地址中。
			该函数只做ECB模式运算。
返回值：	RT_OK或RT_FAIL
********************************************************************************/
UINT8 _Aes_Run(UINT8 nType, UINT8 nMode, UINT32 nAddr,UINT16 nDataLen);

#endif /*AES_H_*/
