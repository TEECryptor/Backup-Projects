#include "SYglobal.h"
#include "FileSystem.h"
#include "Command.h"  
#include "SdMemAcc.h"

extern VERSION  gCosVersion;
extern UINT8    gRespDataBuf[SD_CMDBUF_LEN];

extern void SD_AppInit(void);

int main(void)
{
	//设置系统工作时钟为48M 
    SCM_SetSysClk(SYS_CLK_48M);
   
	//清除编程标志，仿真调试时可提升连接稳定性，量产时必须注销，否则代码烧录完后只能运行一次
    //ClrProgFlag(AREA_PARA_WE);                                                                   

	//SD 流加密时，需要按照下面步骤进行
	//1.获得SD卡容量以后，按照SD_SystemApp.c中的预定义处进行修改，获得软件进行流加密的地址
	//2.通过软件设置各种算法对应的地址进行测试，如SM1地址为0x766750(该地址值需根据SD卡容量进行计算)
	EfcRegs.DATAPRO = 0x10030000;  

	//因为在手机的TF卡槽有时无法识别虚拟文件,所以尽早调用SD_AppInit()
	SD_AppInit();

	//使能全局中断  
	__enable_irq();
	
	//因为在手机上OS识别文件系统时间比较长，所以延时一段时间后再做其他初始化
//	_delay_ms(50);

	__disable_irq();
 	        	  
	//输出第一条log
	printf("****************Syno TF COS****************\n");
	printf("* COS version is %d.%d\n", gCosVersion.major, gCosVersion.minor);
	printf("* Flash is intializing...\n");

	//设置Flash从80KByte 到128K区域为安全存储区域?
    SF_Init(192, 256);
  	//FS_Format();
				  
	printf("* File system is intializing...\n");

	//初始化文件系统
	FS_Init();
  
    //外部Flash设置为4MBytes
	//XF_Init(4*1024*1024);
  	
    //真随机数初始化
	Trng_Init(); 
 		
    //清除FLASH中的临时变量
	memset(gRespDataBuf, 0x00, SD_CMDBUF_LEN); 
  
	// set pointer to buffer
    gApduResp.DATA = gRespDataBuf;
	ScmRegs.MODCLKCTRL |= 1<<ID_SEA;
	ScmRegs.MODRSTCTRL |= 1<<ID_SEA;

	gSystemInited = TRUE;
	//使能全局中断  
	__enable_irq();


	printf("* COS intializeing finished!\n");
	printf("****************Syno TF COS****************\n"); 
	//SCM_SetClkout(0);
	//SCM_EnableClkout();
  
	while( 1 )
	{ 	
	} 
}
