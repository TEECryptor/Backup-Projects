/***************************************************************************
* File name    :	UsbProtocol.c
* Function     :	Do with USB interrupt
* Author       : 	howard
* Date         :	2012/08/12
* Version      :    v1.11
* Description  :     
* ModifyRecord :
****************************************************************************/
#include "SY03.h"
#define	USB_SUSPENDED_6M  0	
#define	USB_SUSPENDED_32K 1

#define	USB_SUSPENDED_MODE USB_SUSPENDED_6M
 
#define	USB_SUSPENDED	(0x80)
UINT8 gSysClkPar; 
UINT8 gUSBFlag = 0;
UINT16 gIntrOut=0;
UINT16 gIntrIn=0; 
/***********************************************************************************
 * Subrountine:	USBDoWithPackage
 * Function:	Deal receive package from PC
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * 				支持Setup为中断方式，Bulk为轮询方式 
 * Date:		2013.03.28
 * ModifyRecord:
 **********************************************************************************/


void USB_DoWithPackage(void)
{ 
 
    UINT8 IntrUSB;
	UINT16 IntrIn;
	UINT16 IntrOut;
	
	UINT8 tmp = UsbRegs.INDEX; 
	// 临时保存，防止中断程序打断bulk传输导致index改变 	
	
    IntrUSB = UsbRegs.INTRUSB.all; 
	IntrIn  = UsbRegs.INTRIN.all;
	IntrOut = UsbRegs.INTROUT.all; 

	if(gUsbMode==USB_SETUP_INT_MODE)
	{
		if(IntrIn>0)
		   gIntrIn|=IntrIn;      
		 if(IntrOut>0)
		    gIntrOut|=IntrOut;
	}
    
	if(IntrUSB&M_INTR_SUSPEND)
    {
		gSysClkPar = USB_SUSPENDED | (UINT8)ScmRegs.SYSCLKCTRL.bit.FclkSel;
#if(USB_SUSPENDED_MODE == USB_SUSPENDED_6M)
		SCM_SetSysClk(SYS_CLK_6M);	// lower system clock
#endif

#if(USB_SUSPENDED_MODE == USB_SUSPENDED_32K)
		ScmRegs.SYSCLKCTRL.bit.Clk32kSel = 0;
		ScmRegs.SYSCLKCTRL.bit.FclkSrc = 0x0;
		__nop();__nop();__nop();__nop();__nop();
#endif
		gUSBFlag = 1; 
    }  
	if(IntrUSB&M_INTR_RESUME)
    {
		if(gSysClkPar&USB_SUSPENDED)
		{
			gSysClkPar &= ~USB_SUSPENDED;
			SCM_SetSysClk((SYS_CLK)gSysClkPar);	// resume system clock 
		}
		gUSBFlag = 1; 
    }    
    
	if(IntrUSB&M_INTR_RESET) //reset
    {  
		if(gSysClkPar&USB_SUSPENDED)
		{
			gSysClkPar &= ~USB_SUSPENDED;
			SCM_SetSysClk((SYS_CLK)gSysClkPar);	// resume system clock 
		}
		USB_Reset(); 
		gUSBFlag = 1;   
    }
	if(IntrIn&M_INTR_EP0)// setup packet received
    {	     
        USB_SetupHandle(); 		      
    }
 
    if(gUsbMode!=USB_SETUP_INT_MODE)
    {  
        if((IntrOut&M_INTR_EP1)||(IntrOut&M_INTR_EP2)||(IntrOut&M_INTR_EP3)||(gIntrOut&M_INTR_EP4))// OUT request received
        {            
        	USB_CBWHandle(); 
        }
    } 
    else
    {     
			
	    UsbRegs.INDEX = tmp;
    }     
   
 
     
}


/***********************************************************************************
 * Subrountine:	USBBulkPoll
 * Function:	Deal bulk by polling mode
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * 			 
 * Date:		2013.03.28
 * ModifyRecord:
 **********************************************************************************/
 void USB_BulkPoll(void)
 {
  	if(gUSBFlag == 1)
		gUSBFlag = 0; 
	if(gIntrOut&M_INTR_EP1)// OUT request received
 	{ 
	    gIntrOut&=(~M_INTR_EP1);	 	 
 		USB_CBWHandle(); 	 		 				 
 	}
	else if(gIntrOut&M_INTR_EP2)// OUT request received
 	{ 
	    gIntrOut&=(~M_INTR_EP2);	 	 	 		 				 
 	}
	else if(gIntrOut&M_INTR_EP3)// OUT request received
 	{ 
	    gIntrOut&=(~M_INTR_EP3);	 	 	 		 				 
 	}
	else if(gIntrOut&M_INTR_EP4)// OUT request received
 	{ 
	    gIntrOut&=(~M_INTR_EP4);	 	 	 		 				 
 	}
 }

 /***********************************************************************************
* Subrountine:	USB_Handler
* Function:	    USB interrupt 
* Input:		None
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/
void USB_Handler(void)
{
	 USB_DoWithPackage();
}
	

