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

//---Ϊ�˺��ϵ��㷨����ݣ�����ĺ궨��---
#define Des_Init DES_Init
#define Des_Run  DES_Run
#define Des3_Run_ECB DES3_Run_ECB
#define Des3_Run_CBC DES3_Run_CBC
#define Des_Run_ECB  DES_Run_ECB
#define Des_Run_CBC  DES_Run_CBC

/*******************************************************************************
����ԭ�ͣ�	UINT8 DES_Init(UINT8* pKey, UINT32 nKeyLen)
����˵����	��ʼ��DES��Կ ��
����˵����	pKey [I] - ��Կ��8�ֽ�(DES)��16�ֽ�(2Key 3DES)��24�ֽڣ�3DES��
            nKeyLen[I] --��Կ���ȣ�����Ϊ8�ֽڣ���ΪDES����ʱ����16�ֽ�(2Key 3DESʱ)��24�ֽڣ���Ϊ3DES����ʱ��
����ֵ��	RT_OK��RT_FAIL 
********************************************************************************/
UINT8 DES_Init(UINT8* pKey,UINT32 nKeyLen);

/*******************************************************************************
����ԭ�ͣ�	UINT8 DES_Run(UINT8 nType, UINT8 nMode, UINT8* pIn, UINT8* pOut, 
							UINT16 nDataLen, UINT8* pIV)
����˵����	DES���ܻ���ܡ�
����˵����	nType [I] - ���ܻ���ܣ��������£�
						#define 	DES_ENCRYPT 		1
						#define 	DES_DECRYPT 		2
			nMode [I] -- ECB��CBC���������£�
						#define	DES_CBC		1
						#define 	DES_ECB		2
						#define	DES3_CBC		3
						#define 	DES3_ECB		4
			pIN [I] --- Ҫ���ܻ���ܵ����ݣ�
			pOut [O] --- ��������
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ8�ı�������������Padding��
			PIV [IO] --- �����IV��������IVֵ������CBCģʽ���´����㣬����Ϊ8�ֽ�
ע�����  ��Ϊ�����IV���壬ͬʱҪ���շ��ص�IV�����Ҫ����ԭʼ��IV����������ʱ��Ҫԭʼ��IV��
����ֵ��	RT_OK��RT_FAIL
********************************************************************************/
UINT8 DES_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
����ԭ�ͣ�	UINT8 DES_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen )
����˵����	DES ECBģʽ���ܻ���ܡ�
����˵����  nType [I] --- ���ܻ���ܣ��������£�
						#define 	DES_ENCRYPT 		1
						#define 	DES_DECRYPT 		2
				pIN [I] ---  Ҫ���ܻ���ܵ����ݡ�
				pOut [O] ---  ��������
nDataLen [I] --- ���ݳ��ȣ�����Ϊ8�ı�������������Padding��
�� �� ֵ��	RT_OK����RT_FAIL 
********************************************************************************/ 
UINT8 DES_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen );

/*******************************************************************************
����ԭ�ͣ�	UINT8 DES_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,
							UINT16 nDataLen,UINT8* pIV)
����˵����	DES CBCģʽ���ܻ���ܡ�
����˵����	nType [I] - ���ܻ���ܣ��������£�
						#define 	DES_ENCRYPT 		1
						#define 	DES_DECRYPT 		2
			pIN[I] --- Ҫ���ܻ���ܵ����ݡ�
			pOut [O] --- ��������
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ8�ı�������������Padding.	
			PIV[I][O] --- �����IV�������ص�IVֵ������CBCģʽ���´����㣬����Ϊ8�ֽ�
ע�����	��Ϊ�����IV���壬ͬʱҪ���շ��ص�IV�����Ҫ����ԭʼ��IV����������ʱ��Ҫԭʼ��IV��
�� �� ֵ��	RT_OK--�ɹ�
            RT_FAIL--ʧ��
********************************************************************************/
UINT8 DES_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
����ԭ�ͣ�	UINT8 DES3_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen )
����˵����	3DES ECBģʽ���ܻ����,���Գ�ʼ��Ϊ2Key��3Key��
����˵����  nType [I] --- ���ܻ���ܣ��������£�
								#define 	DES_ENCRYPT 		1
								#define 	DES_DECRYPT 		2
			pIN [I] ---  Ҫ���ܻ���ܵ����ݡ�
			pOut [O] ---  ��������
nDataLen [I] --- ���ݳ��ȣ�����Ϊ8�ı�������������Padding��
�� �� ֵ��	RT_OK����RT_FAIL
********************************************************************************/
UINT8 DES3_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen );

/*******************************************************************************
 ����ԭ�ͣ�	UINT8 DES3_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,
UINT16 nDataLen,UINT8* pIV)
����˵����	3DES CBCģʽ���ܻ����,���Գ�ʼ��Ϊ2Key��3Key��
����˵����	nType [I] - ���ܻ���ܣ��������£�
						#define 	DES_ENCRYPT 		1
						#define 	DES_DECRYPT 		2
			pIN[I] --- Ҫ���ܻ���ܵ����ݡ�
			pOut [O] --- ��������
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ8�ı�������������Padding.	
			PIV[I][O] --- �����IV�������ص�IVֵ������CBCģʽ���´����㣬����Ϊ8�ֽ�
ע�����	��Ϊ�����IV���壬ͬʱҪ���շ��ص�IV�����Ҫ����ԭʼ��IV����������ʱ��Ҫԭʼ��IV��
�� �� ֵ��	RT_OK--�ɹ�
			RT_FAIL--ʧ�� 
********************************************************************************/
UINT8 DES3_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
����ԭ�ͣ�	UINT8 _Des_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen)
����˵����	DES���ܻ���ܡ�
����˵����	nType [I] - ���ܻ���ܣ��������£�
						#define 	DES_ENCRYPT 		1
						#define 	DES_DECRYPT 		2
			nAddr [I][O] --- Ҫ���ܻ���ܵ�������ʼ��ַ�����ÿռ�Ϊ0x20004000~0x20008000����
							 ������������Ҳ������Ըõ�ַΪ��ʼ�Ŀռ��У�
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ8�ı�������������Padding��
ע�����  �ú���ֱ�ӵ����㷨�������������Ҫ�û����������ݷŵ�����ռ��ַ�С�
			�ú���ֻ��ECBģʽ���㡣
����ֵ��	RT_OK��RT_FAIL
********************************************************************************/
UINT8 _Des_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen);

#endif
