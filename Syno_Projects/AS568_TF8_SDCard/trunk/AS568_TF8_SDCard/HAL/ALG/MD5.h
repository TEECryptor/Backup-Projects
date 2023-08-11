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
����ԭ�ͣ�	void MD5_Init(MD5_CONTEXT *pCtx)
����˵����	��ʼ��MD5���ݽṹ��
����˵����	pctx [IO] --- MD5_CONTEXT  
����ֵ��	�ޡ�   
********************************************************************************/
void MD5_Init(MD5_CONTEXT *pCtx);

/*******************************************************************************
����ԭ�ͣ�	void MD5_Update(MD5_CONTEXT *pCtx, UINT8 *pData, UINT32 lDataLen)
����˵����	ִ��Hash���㡣
����˵����	pCtx [IO] --- MD5_CONTEXT��
            pData [I] --- �������ݣ�
            lDataLen [I] --- ���ݳ��ȡ�
�� �� ֵ��	�ޡ�
********************************************************************************/
void MD5_Update(MD5_CONTEXT *pCtx, UINT8 *pData, UINT32 lDataLen);

/*******************************************************************************
����ԭ�ͣ�	void MD5_Final(MD5_CONTEXT *pCtx,UINT8 Result[16])
����˵����	���Hash�����
����˵����	pCtx [IO] --- MD5_CONTEXT��
            Output [O] --- Hash�����16�ֽ�ժҪ��Ϣ��
����ֵ��	�ޡ�
********************************************************************************/
void MD5_Final(MD5_CONTEXT *pCtx,UINT8 Result[16]);	
 

#endif /* _MD5_H_ */
