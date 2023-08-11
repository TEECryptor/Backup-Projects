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

//---Ϊ�˺��ϵ��㷨����ݣ�����ĺ궨��---
#define  Aes_Init AES_Init 
#define  Aes_Run  AES_Run

/*******************************************************************************
����ԭ�ͣ�	UINT8 AES_Init(UINT8* pKey, UINT32 nKeyLen)
����˵����	��ʼ��Aes��Կ ��
����˵����	pKey [I] --- ��Կ��
            nKeyLen [I] --- ��Կ���ȣ�����Ϊ16�ֽڣ���Ϊ128bit����ʱ����24�ֽڣ���Ϊ192bit����ʱ����32�ֽ�(��Ϊ256bit����ʱ)��
����ֵ��	RT_OK��RT_FAIL 
********************************************************************************/ 
UINT8 AES_Init(UINT8* pKey,UINT32 nKeyLen);

/*******************************************************************************
����ԭ�ͣ�	UINT8 AES_Run(UINT8 nType, UINT8 nMode, UINT8* pIn, UINT8* pOut, 
UINT16 nDataLen, UINT8* pIV)
����˵����	AES���ܻ���ܡ�
����˵����	nType [I] --- ���ܻ���ܣ��������£�
							#define 	AES_ENCRYPT 		1
							#define 	AES_DECRYPT 		2
			nMode [I] --- ECB��CBC���������£�
							#define	AES128_CBC 		1
							#define	AES128_ECB 		2 
							#define	AES192_CBC 		3
							#define	AES192_ECB 		4
							#define	AES256_CBC 		5
							#define	AES256_ECB 		6 
			pIN [I] --- Ҫ���ܻ���ܵ����ݣ�
			pOut [O] --- ��������
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding��
			PIV[IO] --- �����IV��������IVֵ������CBCģʽ���´����㣬����Ϊ16�ֽڡ�
ע�������Ϊ�����IV���壬ͬʱҪ���շ��ص�IV�����Ҫ����ԭʼ��IV����������ʱ��Ҫԭʼ��IV��
����ֵ��	RT_OK��RT_FAIL
********************************************************************************/
UINT8 AES_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
����ԭ�ͣ�	UINT8 _Aes_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen)
����˵����	AES���ܻ���ܡ�
����˵����	nType [I] - ���ܻ���ܣ��������£�
						#define 	AES_ENCRYPT 		1
						#define 	AES_DECRYPT 		2
			nMode [I] --- ECB���������£�
							#define	AES128_ECB 		2 
							#define	AES192_ECB 		4
							#define	AES256_ECB 		6 
			nAddr [I][O] --- Ҫ���ܻ���ܵ�������ʼ��ַ�����ÿռ�Ϊ0x20004000~0x20008000����
							 ������������Ҳ������Ըõ�ַΪ��ʼ�Ŀռ��У�
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding��
ע�����  �ú���ֱ�ӵ����㷨�������������Ҫ�û����������ݷŵ�����ռ��ַ�С�
			�ú���ֻ��ECBģʽ���㡣
����ֵ��	RT_OK��RT_FAIL
********************************************************************************/
UINT8 _Aes_Run(UINT8 nType, UINT8 nMode, UINT32 nAddr,UINT16 nDataLen);

#endif /*AES_H_*/
