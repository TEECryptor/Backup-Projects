/***************************************************************************************
* File name    :	MD5.h
* Function     :	The header of MD5c.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _MD5_H_
#define _MD5_H_
 
typedef struct {
  UINT32 state[4];
  UINT32 count[2]; 
  UINT8 buffer[64];
} MD5_CONTEXT;

/*******************************************************************************
函数原型：	void MD5_Init(MD5_CONTEXT *pCtx)
函数说明：	初始化MD5数据结构。
参数说明：	pctx [IO] --- MD5_CONTEXT  
返回值：	无。   
********************************************************************************/
void MD5_Init(MD5_CONTEXT *pCtx);

/*******************************************************************************
函数原型：	void MD5_Update(MD5_CONTEXT *pCtx, UINT8 *pData, UINT32 lDataLen)
函数说明：	执行Hash运算。
参数说明：	pCtx [IO] --- MD5_CONTEXT；
            pData [I] --- 运算内容；
            lDataLen [I] --- 数据长度。
返 回 值：	无。
********************************************************************************/
void MD5_Update(MD5_CONTEXT *pCtx, UINT8 *pData, UINT32 lDataLen);

/*******************************************************************************
函数原型：	void MD5_Final(MD5_CONTEXT *pCtx,UINT8 Result[16])
函数说明：	输出Hash结果。
参数说明：	pCtx [IO] --- MD5_CONTEXT；
            Output [O] --- Hash结果。16字节摘要信息。
返回值：	无。
********************************************************************************/
void MD5_Final(MD5_CONTEXT *pCtx,UINT8 Result[16]);	
 

#endif /* _MD5_H_ */
