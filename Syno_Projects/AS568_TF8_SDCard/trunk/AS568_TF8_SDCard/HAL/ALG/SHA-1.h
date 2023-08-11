/***************************************************************************************
* File name    :	SHA-1.h
* Function     :	The header of SHA-1.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SHA1_H_
#define _SHA1_H_ 

typedef struct {
    UINT32  h0,h1,h2,h3,h4;
    UINT32  nblocks;
    UINT8 buf[64];
    UINT32  count;
} SHA1_CONTEXT;

/*******************************************************************************
函数原型：	void SHA1_Init(SHA1_CONTEXT *hd )
函数说明：	初始化SHA1数据结构。
参数说明：	hd [O] --- SHA1数据结构初始值； 
返回值：	无。
********************************************************************************/
void SHA1_Init( SHA1_CONTEXT *hd );

/*******************************************************************************
函数原型：	void SHA1_Update(SHA1_CONTEXT *hd, UINT8 *inbuf, UINT32 inlen)
函数说明：	执行Hash运算。
参数说明：	hd [IO] --- SHA1_CONTEXT；
			inbuf [I] --- 运算内容；
			inlen [I] --- 数据长度
返回值：	无。
********************************************************************************/
void SHA1_Update( SHA1_CONTEXT *hd, UINT8 *inbuf, UINT32 inlen);

/*******************************************************************************
函数原型：	void SHA1_Final(SHA1_CONTEXT *hd, UINT8 output[20])
函数说明：	输出Hash结果。
参数说明：	hd [IO] --- SHA1_CONTEXT，结果存放在hd的h0，h1，h2，h3，h4中；
			Output [O] --- Hash结果，也就是hd的h0，h1，h2，h3，h4。20字节摘要信息。
返回值：	无。
********************************************************************************/
void SHA1_Final(SHA1_CONTEXT *hd,UINT8 output[20]);		 

#endif 
