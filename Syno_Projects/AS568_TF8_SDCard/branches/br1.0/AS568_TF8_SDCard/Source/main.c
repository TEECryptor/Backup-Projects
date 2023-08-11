#include "SYglobal.h"
#include "FileSystem.h"
#include "Command.h"  
#include "SdMemAcc.h"

extern VERSION  gCosVersion;
extern UINT8    gRespDataBuf[SD_CMDBUF_LEN];

extern void SD_AppInit(void);

int main(void)
{
	//����ϵͳ����ʱ��Ϊ48M 
    SCM_SetSysClk(SYS_CLK_48M);
   
	//�����̱�־���������ʱ�����������ȶ��ԣ�����ʱ����ע�������������¼���ֻ������һ��
    //ClrProgFlag(AREA_PARA_WE);                                                                   

	//SD ������ʱ����Ҫ�������沽�����
	//1.���SD�������Ժ󣬰���SD_SystemApp.c�е�Ԥ���崦�����޸ģ����������������ܵĵ�ַ
	//2.ͨ��������ø����㷨��Ӧ�ĵ�ַ���в��ԣ���SM1��ַΪ0x766750(�õ�ֵַ�����SD���������м���)
	EfcRegs.DATAPRO = 0x10030000;  

	//��Ϊ���ֻ���TF������ʱ�޷�ʶ�������ļ�,���Ծ������SD_AppInit()
	SD_AppInit();

	//ʹ��ȫ���ж�  
	__enable_irq();
	
	//��Ϊ���ֻ���OSʶ���ļ�ϵͳʱ��Ƚϳ���������ʱһ��ʱ�������������ʼ��
//	_delay_ms(50);

	__disable_irq();
 	        	  
	//�����һ��log
	printf("****************Syno TF COS****************\n");
	printf("* COS version is %d.%d\n", gCosVersion.major, gCosVersion.minor);
	printf("* Flash is intializing...\n");

	//����Flash��80KByte ��128K����Ϊ��ȫ�洢����?
    SF_Init(192, 256);
  	//FS_Format();
				  
	printf("* File system is intializing...\n");

	//��ʼ���ļ�ϵͳ
	FS_Init();
  
    //�ⲿFlash����Ϊ4MBytes
	//XF_Init(4*1024*1024);
  	
    //���������ʼ��
	Trng_Init(); 
 		
    //���FLASH�е���ʱ����
	memset(gRespDataBuf, 0x00, SD_CMDBUF_LEN); 
  
	// set pointer to buffer
    gApduResp.DATA = gRespDataBuf;
	ScmRegs.MODCLKCTRL |= 1<<ID_SEA;
	ScmRegs.MODRSTCTRL |= 1<<ID_SEA;

	gSystemInited = TRUE;
	//ʹ��ȫ���ж�  
	__enable_irq();


	printf("* COS intializeing finished!\n");
	printf("****************Syno TF COS****************\n"); 
	//SCM_SetClkout(0);
	//SCM_EnableClkout();
  
	while( 1 )
	{ 	
	} 
}
