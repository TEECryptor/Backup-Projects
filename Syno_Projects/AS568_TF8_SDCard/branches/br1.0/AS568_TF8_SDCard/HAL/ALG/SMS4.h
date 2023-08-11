#ifndef _SMS4_H_
#define _SMS4_H_
 
#define SMS4_ENCRYPT 1
#define SMS4_DECRYPT 2 

#define SMS4_CBC 1
#define SMS4_ECB 2
 
/*******************************************************************************
����ԭ�ͣ�	UINT8 SMS4_Init(UINT8* pEK);
����˵����	��ʼ��SMS4��Կ ��
����˵����	pEK [I] --- ����Կ������Ϊ16�ֽڡ�
�� �� ֵ��	RT_OK--�ɹ�      
            RT_FAIL--ʧ��
********************************************************************************/
UINT8 SMS4_Init(UINT8* pEK);

/*******************************************************************************
����ԭ�ͣ�	UINT8 SMS4_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV)
����˵����	SMS4���ܻ���ܡ�
����˵����	nType [I] --- ���ܻ���ܣ��������£�
							#define 	SMS4_ENCRYPT 		1
							#define 	SMS4_DECRYPT 		2
			nMode [I] --- ECB��CBC��OFB��CFB���������£�
							#define SMS4_CBC 1
							#define SMS4_ECB 2				 
			pIN[I] --- Ҫ���ܻ���ܵ����ݡ�
			pOut [O] --- ��������
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding��
			PIV[I][O] --- �����IV�������ص�IVֵ������CBCģʽ���´����㣬����Ϊ16�ֽڡ�
			           ��ΪECBģʽʱ����ֵΪNULL.
ע�����	��Ϊ�����IV���壬ͬʱҪ���շ��ص�IV�����Ҫ����ԭʼ��IV����������ʱ��Ҫԭʼ��IV��
�� �� ֵ��	RT_OK--�ɹ� 
			RT_FAIL--ʧ��
********************************************************************************/
UINT8 SMS4_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
����ԭ�ͣ�	UINT8 SMS4_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen )
����˵����	SMS4 ECBģʽ���ܻ���ܡ�
����˵����	nType [I] --- ���ܻ���ܣ��������£�
			#define 	SMS4_ENCRYPT 		1
			#define 	SMS4_DECRYPT 		2
			pIN[I] --- Ҫ���ܻ���ܵ����ݡ�
			pOut [O] --- ��������
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding.
�� �� ֵ��	RT_OK--�ɹ� 
			RT_FAIL--ʧ��

********************************************************************************/
UINT8 SMS4_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen);

/*******************************************************************************
����ԭ�ͣ�	UINT8 SMS4_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,
                               UINT8* pIV)
����˵����	SMS4 CBCģʽ���ܻ���ܡ�
����˵����	nType [I] --- ���ܻ���ܣ��������£�
							#define 	SMS4_ENCRYPT 		1
							#define 	SMS4_DECRYPT 		2
			pIN[I] --- Ҫ���ܻ���ܵ����ݡ�
			pOut [O] --- ��������
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding��
			PIV[I][O] --- �����IV�������ص�IVֵ������CBCģʽ���´����㣬����Ϊ16�ֽ�
ע�����	��Ϊ�����IV���壬ͬʱҪ���շ��ص�IV�����Ҫ����ԭʼ��IV����������ʱ��Ҫԭʼ��IV��
�� �� ֵ��	RT_OK--�ɹ� 
            RT_FAIL--ʧ�� 
********************************************************************************/
UINT8 SMS4_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);
 
#endif
