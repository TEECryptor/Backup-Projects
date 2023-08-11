/***************************************************************************************
* File name    :	SHA-256.h
* Function     :	The header of SHA-256.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
 
#ifndef _SHA2_H
#define _SHA2_H
 
typedef struct
{
    UINT32 total[2];     
    UINT32 state[8];    
    UINT8 buffer[64];    
}
SHA256_CONTEXT;
/*******************************************************************************
函数原型：	void SHA256_Init(SHA256_CONTEXT *ctx)
函数说明：	初始化SHA256数据结构。
参数说明：	ctx [IO] --- SHA256_CONTEXT 
返回值：	无。
********************************************************************************/ 
void SHA256_Init( SHA256_CONTEXT *ctx);
/*******************************************************************************
函数原型：	void SHA256_Update(SHA256_CONTEXT *ctx, UINT8 *input, INT32 ilen)
函数说明：	执行Hash运算。
参数说明：	ctx [IO] --- SHA256_CONTEXT；
			input [I] --- 运算内容；
			ilen [I] --- 数据长度
返回值：	无
********************************************************************************/ 
void SHA256_Update( SHA256_CONTEXT *ctx, UINT8 *input, INT32 ilen );
/*******************************************************************************
函数原型：	void SHA256_Final(SHA256_CONTEXT *ctx, UINT8 output[32])
函数说明：	输出Hash结果。
参数说明：	ctx [I O] --- SHA256_CONTEXT；
			Output [O] --- Hash结果，32字节。
返回值：	无
********************************************************************************/ 
void SHA256_Final( SHA256_CONTEXT *ctx, UINT8 output[32] );
  

#endif /* sha2.h */
