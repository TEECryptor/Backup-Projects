#ifndef _SSF33Alg_H_
#define _SSF33Alg_H_

#define SSF33_ENCRYPT 1
#define SSF33_DECRYPT 2 

#define SSF33_CBC 1
#define SSF33_ECB 2
 
/*******************************************************************************
����ԭ��:  UINT8 SSF33_Init(UINT8* pEK);
����˵��:  ��ʼ��SSF33��Կ ��
����˵��:  pEK  [I] -����Կ������Ϊ16�ֽ� 					 
�� �� ֵ:  RT_OK:�ɹ�      
           RT_FAIL:ʧ��
********************************************************************************/
UINT8 SSF33_Init(UINT8* pEK);

/*******************************************************************************
����ԭ��:  UINT8 SSF33_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV)
����˵��:  SMS4���ܻ���ܡ�
����˵��:  nType [I] - ���ܻ���ܣ��������£�
			#define 	SSF33_ENCRYPT 		1
			#define 	SSF33_DECRYPT 		2
			nMode [I]  -- ECB�� CBC ��OFB��CFB,��������:
			#define SSF33_CBC 1
			#define SSF33_ECB 2				 
						pIN[I]     --  Ҫ���ܻ���ܵ����ݡ�
					    pOut [O]  ---   ��������
			nDataLen [I]  --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding.	
			PIV[I][O] - �����IV�������ص�IVֵ������CBCģʽ���´����㣬����Ϊ
			16�ֽڡ���ΪECBģʽʱ����ֵΪNULL
ע������:  ��Ϊ�����IV����,ͬʱҪ���շ��ص�IV�����Ҫ����ԭʼ��IV����������ʱ��Ҫԭʼ��IV��
�� �� ֵ:  RT_OK:�ɹ�      
           RT_FAIL:ʧ��
 
********************************************************************************/
UINT8 SSF33_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
����ԭ��:  UINT8 SSF33Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen )
����˵��:  SSF33 ECBģʽ���ܻ���ܡ�
����˵��:  nType [I] - ���ܻ���ܣ��������£�
						#define 	SSF33_ENCRYPT 		1
						#define 	SSF33_DECRYPT 		2
			pIN[I]     --  Ҫ���ܻ���ܵ����ݡ�
		    pOut [O] ---   ��������
            nDataLen [I] --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding.	
�� �� ֵ:  RT_OK:�ɹ�      
           RT_FAIL:ʧ��

********************************************************************************/
UINT8 SSF33_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen );

/*******************************************************************************
����ԭ��:  UINT8 SSF33_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,
UINT8* pIV)
����˵��:  SSF33 CBCģʽ���ܻ���ܡ�
����˵��:  nType [I] - ���ܻ���ܣ��������£�
				#define 	SSF33_ENCRYPT 		1
				#define 	SSF33_DECRYPT 		2
							pIN[I]     --  Ҫ���ܻ���ܵ����ݡ�
						    pOut [O] ---  ��������
			nDataLen [I] --- ���ݳ��ȣ�����Ϊ16�ı�������������Padding.	
			PIV[I][O]-�����IV�������ص�IVֵ������CBCģʽ���´�����,����Ϊ16��ڣ
ע������:  ��Ϊ�����IV����,ͬʱҪ���շ��ص�IV�����Ҫ����ԭʼ��IV����������ʱ��Ҫԭʼ��IV��
�� �� ֵ:  RT_OK:�ɹ�      
			RT_FAIL:ʧ�� 
********************************************************************************/
UINT8 SSF33_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);
 
#endif
