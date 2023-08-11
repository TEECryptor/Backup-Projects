#ifndef _SM1_H_
#define _SM1_H_


#define SM1_ENCRYPT 1
#define SM1_DECRYPT 2 

#define SM1_CBC 1
#define SM1_ECB 2
 
 
/*******************************************************************************
����ԭ�ͣ�	UINT8 SM1_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen )
����˵����	SM1 ECBģʽ���ܻ���ܡ�
����˵����  nType [I] --- ���ܻ���ܣ��������£�
#define 	SM1_ENCRYPT 		1
#define 	SM1_DECRYPT 		2
				pIN [I] ---  Ҫ���ܻ���ܵ����ݡ�
				pOut [O] ---  ��������
nDataLen [I] --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding��
�� �� ֵ��	RT_OK����RT_FAIL   
********************************************************************************/
UINT8 SM1_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen );

/*******************************************************************************
����ԭ�ͣ�	UINT8 SM1_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,
									UINT16 nDataLen,UINT8* pIV)
����˵����	SM1 CBCģʽ���ܻ���ܡ�
����˵����	nType [I] - ���ܻ���ܣ��������£�
						#define 	SM1_ENCRYPT 		1
						#define 	SM1_DECRYPT 		2
			pIN[I] --- Ҫ���ܻ���ܵ����ݡ�
						pOut [O] --- ��������
						nDataLen [I] --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding.	
			PIV[I][O] --- �����IV�������ص�IVֵ������CBCģʽ���´����㣬����Ϊ16�ֽ�
ע�����	��Ϊ�����IV���壬ͬʱҪ���շ��ص�IV�����Ҫ����ԭʼ��IV����������ʱ��Ҫԭʼ��IV��
�� �� ֵ��	RT_OK--�ɹ�
            RT_FAIL--ʧ��

********************************************************************************/
UINT8 SM1_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
����ԭ�ͣ�	UINT8 SM1_Init(UINT8* pEK, UINT8* pAK)
����˵����	��ʼ��SM1��Կ��
����˵����	pEK, pAK [I] --- ��������Կ��ÿ������Ϊ16�ֽڣ�
����ֵ��	RT_OK��RT_FAIL 
********************************************************************************/
UINT8 SM1_Init(UINT8* pEK,UINT8* pAK);

/*******************************************************************************
����ԭ�ͣ�	UINT8 SM1_Run(UINT8 nType, UINT8 nMode, UINT8* pIn, UINT8* pOut, 
								UINT16 nDataLen,UINT8* pIV)
����˵����	SM1���ܻ���ܡ�
����˵����	nType [I] --- ���ܻ���ܣ��������£�
							#define 	SM1_ENCRYPT 		1
							#define 	SM1_DECRYPT 		2
			nMode [I] --- ECB��CBC���������£�
							#define SM1_CBC 1
							#define SM1_ECB 2	
			pIN [I] --- Ҫ���ܻ���ܵ����ݣ�
			pOut [O] --- ��������
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding��
			PIV[IO] --- �����IV��������IVֵ������CBCģʽ���´����㣬����Ϊ16�ֽڡ���ΪECBģʽʱ����ֵΪNULL
ע�������Ϊ�����IV���壬ͬʱҪ���շ��ص�IV�����Ҫ����ԭʼ��IV����������ʱ��Ҫԭʼ��IV��
����ֵ��	RT_OK��RT_FAIL	
********************************************************************************/
UINT8 SM1_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
����ԭ�ͣ�	UINT8 _SM_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen)
����˵����	SM���ܻ���ܡ�
����˵����	nType [I] - ���ܻ���ܣ��������£�
						#define 	SM1_ENCRYPT 		1
						#define 	SM1_DECRYPT 		2
			nAddr [I][O] --- Ҫ���ܻ���ܵ�������ʼ��ַ�����ÿռ�Ϊ0x20004000~0x20008000����
							 ������������Ҳ������Ըõ�ַΪ��ʼ�Ŀռ��У�
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding��
ע�����  �ú���ֱ�ӵ����㷨�������������Ҫ�û����������ݷŵ�����ռ��ַ�С�
			�ú���ֻ��ECBģʽ���㡣
����ֵ��	RT_OK��RT_FAIL
********************************************************************************/
UINT8 _SM_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen);
 
#endif
