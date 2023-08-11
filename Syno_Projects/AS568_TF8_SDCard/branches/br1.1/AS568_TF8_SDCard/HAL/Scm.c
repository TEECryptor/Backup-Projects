/****************************************************************************
* File name    :	Scm.c
* Function     :	the driver of System Control Module
* Author       : 	coins
* Date         :	2012/07/23
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SY03.h"

extern volatile uint32_t		SystemFrequency; 	// defined in system_SY03.c
extern volatile unsigned char 	SerInit;	 		// defined in Serial.c 

/**************************************************************************
* Local Macro Definition
***************************************************************************/
#define	SCM_ROM_FUNC_USED
#define		CALC_MS(freq)	((freq)/4000)

/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static UINT16 gCntMs = 424;
static UINT8 gCntUs = 1;
static SYS_CLK gSysClk = 1;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/
static UINT8 SCM_RegWrEn(void);

/**************************************************************************
* Functon
***************************************************************************/

/****************************************************************************
 * Subroutine:	CM_RegWrEn
 * Function:	to enable register MODPDCTRL and PWRTOPCTRL writeable 
 * Input:		None
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
static UINT8 SCM_RegWrEn(void)
{
	ScmRegs.REGWP0 = 0x55;
	ScmRegs.REGWP0 = 0xAA;
	ScmRegs.REGWP0 = 0x67;
	if(ScmRegs.REGWP0&0x100) return RT_OK;
	else return RT_FAIL;
}

/****************************************************************************
 * Subroutine:	SCM_AnModPowerEnable
 * Function:	to enable analog mode power 
 * Input:		UINT32 nAnMods --	to specify the analog modules. 
 * Output:		None	
 * Description:	 
 * Date:		2013.12.18
 * ModifyRecord:
 * *************************************************************************/
void SCM_AnModPowerEnable(UINT32 nAnMods)
{
	SCM_RegWrEn();
	ScmRegs.AMODENCTRL.all |= (nAnMods);
	ScmRegs.REGWP0 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_AnModPowerDisable
 * Function:	to disable analog mode power 
 * Input:		UINT32 nAnMods --	to specify the analog modules. 
 * Output:		None	
 * Description:	 
 * Date:		2013.12.18
 * ModifyRecord:
 * *************************************************************************/
void SCM_AnModPowerDisable(UINT32 nAnMods)
{
	SCM_RegWrEn();
	ScmRegs.AMODENCTRL.all &= ~(nAnMods);
	ScmRegs.REGWP0 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_SetSysClk
 * Function:	to set system clock 
 * Input:		UINT8 Clk --	to specify the system clock. 
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_SetSysClk(SYS_CLK Clk)
{	
	if(ScmRegs.AMODENCTRL.bit.Hfrosc == 0)
	{
		SCM_RegWrEn();
		ScmRegs.AMODENCTRL.bit.Hfrosc = 1;
		ScmRegs.REGWP0 = 0x00;
	}
	#ifndef	SCM_ROM_FUNC_USED
	ScmRegs.SYSCLKCTRL.all = (ScmRegs.SYSCLKCTRL.all & 0xFFFFFFF0) | 0x1 | (Clk<<2);
	__nop();__nop();__nop();__nop();__nop();
	#else
	if(ScmRegs.SYSCLKCTRL.bit.FclkSrc == 0)
		ScmRegs.SYSCLKCTRL.bit.FclkSrc = 1;
	_SCM_SetSysClk(Clk);
	#endif
	gSysClk = Clk; 
	SystemFrequency = F_HFROSC/8*((UINT8)1<<Clk);
	SerInit = 0;
	switch(Clk)
	{
		case SYS_CLK_6M: 
			EFC_ConfigTiming(0, TC0_6M, TC1_6M);	
			gCntMs = 366;
			gCntUs = 1;
			*((UINT32 *)ROM_GCNT_US1_ADD) = 0;
			*((UINT32 *)ROM_GCNT_US2_ADD) = 0;
			*((UINT32 *)ROM_GCNT_MS_ADD) = 1500;
			break;
		case SYS_CLK_12M: 
			EFC_ConfigTiming(0, TC0_12M, TC1_12M);
			gCntMs = 976;
			gCntUs = 1;
			*((UINT32 *)ROM_GCNT_US1_ADD) = 0;
			*((UINT32 *)ROM_GCNT_US2_ADD) = 0;
			*((UINT32 *)ROM_GCNT_MS_ADD) = 3000;		
			break;
		case SYS_CLK_24M: 
			EFC_ConfigTiming(0, TC0_24M, TC1_24M);
			gCntMs = 1954;
			gCntUs = 1;
			*((UINT32 *)ROM_GCNT_US1_ADD) = 2;
			*((UINT32 *)ROM_GCNT_US2_ADD) = 2;
			*((UINT32 *)ROM_GCNT_MS_ADD) = 6000;	
			break;
		case SYS_CLK_48M: 
			EFC_ConfigTiming(0, TC0_48M, TC1_48M);
			gCntMs = 3351;
			gCntUs = 3;
			*((UINT32 *)ROM_GCNT_US1_ADD) = 7;
			*((UINT32 *)ROM_GCNT_US2_ADD) = 7;
			*((UINT32 *)ROM_GCNT_MS_ADD) = 12000;	
			break;
		default:
			return RT_FAIL;
	}
	return RT_OK;
}

/****************************************************************************
 * Subroutine:	SCM_GetSysClk
 * Function:	to get system clock 
 * Input:		None 
 * Output:		return the system clock frequency in Hz.	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
UINT32 SCM_GetSysClk(void)
{
	return SystemFrequency;
}


/****************************************************************************
 * Subroutine:	SCM_SetSysClkLowFreq
 * Function:	to set system clock to LFROSC 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.01.05
 * ModifyRecord:
 * *************************************************************************/
void SCM_SetSysClkLowFreq(void)
{
	ScmRegs.SYSCLKCTRL.bit.FclkSrc = 0x0;
	__nop();__nop();__nop();__nop();__nop();
}

/****************************************************************************
 * Subroutine:	SCM_RecoverSysClk
 * Function:	to exit system low frequency (LFROSC) and to recover system clock  
 * Input:		None 
 * Output:		RT_OK or RT_FAIL	
 * Description:	 This function is only to recover system clock(HFROSC) as soon as possible, 
 *		but not set EFC_ConfigTiming. You can first To deal with urgent matter,then
 *               to set SCM_SetSysClk function.
 * Date:		2013.01.05
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_RecoverSysClk(void)
{
	ScmRegs.REGWP0 = 0x55;
	ScmRegs.REGWP0 = 0xAA;
	ScmRegs.REGWP0 = 0x67;
	if((ScmRegs.REGWP0&0x100) == 0) return RT_FAIL;
	ScmRegs.AMODENCTRL.bit.Hfrosc = 1;
	ScmRegs.REGWP0 = 0x00;
	ScmRegs.SYSCLKCTRL.all = 0x1 | (gSysClk<<2);
	__nop();__nop();__nop();__nop();__nop();
	SerInit = 0;
	return RT_OK;
}

/****************************************************************************
 * Subroutine:	SCM_HFRoscPd
 * Function:	to power down HF ROSC clock 
 * Input:		None 
 * Output:		None	
 * Description:	This function should not be called when the system clock is 
 *				fed by HFROSC 
 * Date:		2013.01.05
 * ModifyRecord:
 * *************************************************************************/
void SCM_HFRoscPd(void)
{
	SCM_RegWrEn();
	ScmRegs.AMODENCTRL.bit.Hfrosc = 0;
	ScmRegs.REGWP0 = 0x00;
}

/****************************************************************************
 * Subroutine:	SCM_LoadHFRoscTrim
 * Function:	to load default HF ROSC Trimming value from OTP and update to register 
 * Input:		None 
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.10.25
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_LoadHFRoscTrim(void)
{
	UINT8 nOffset = ScmRegs.HFROSCCTRL.bit.HFROscGrade;
	UINT16 *pDefaultTrim = (UINT16 *)0x10040420;  	// trimming field address in OTP
	UINT16 DefaultTrim = pDefaultTrim[nOffset]&0xFFF; 
	union SCM_HFROSCCTRL_REG  uHfOscCtrl;
	if(DefaultTrim != 0xFFF)
	{
		uHfOscCtrl.all = ScmRegs.HFROSCCTRL.all;
		uHfOscCtrl.bit.HFROscGrade = DefaultTrim>>10;
		uHfOscCtrl.bit.HFROscTrim = DefaultTrim&0x3FF;
		ScmRegs.HFROSCCTRL.all = uHfOscCtrl.all;
		return RT_OK;
	}
	else
	{
		return RT_FAIL;
	}	
}

/****************************************************************************
 * Subroutine:	SCM_BodLvSel
 * Function:	to select BOD Vth
 * Input:		UINT8 nBodLv -- to specify the Vth, 0: low, 1: high
 * Output:		None	
 * Description:	 
 * Date:		2013.01.29
 * ModifyRecord:
 * *************************************************************************/
void SCM_SetBodLv(UINT8 nBodLv)
{
	SCM_RegWrEn();
	ScmRegs.PWRTOPCTRL.bit.BODLvClk = 0;
	ScmRegs.PWRTOPCTRL.bit.BODLv = nBodLv;
	ScmRegs.PWRTOPCTRL.bit.BODLvClk = 1;
	ScmRegs.PWRTOPCTRL.bit.BODLvClk = 0;
	ScmRegs.REGWP0 = 0x00;
}

/****************************************************************************
 * Subroutine:	SCM_EnableTpr
 * Function:	to enable TPR
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.01.29
 * ModifyRecord:
 * *************************************************************************/
void SCM_EnableTpr(void)
{
	SCM_RegWrEn();
	ScmRegs.AMODENCTRL.bit.Tpr = 1;
	ScmRegs.REGWP0 = 0x00;	
}

/****************************************************************************
 * Subroutine:	SCM_DisableTpr
 * Function:	to disable TPR
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.01.29
 * ModifyRecord:
 * *************************************************************************/
void SCM_DisableTpr(void)
{
	SCM_RegWrEn();
	ScmRegs.AMODENCTRL.bit.Tpr = 0;
	ScmRegs.REGWP0 = 0x00;	
}

/****************************************************************************
 * Subroutine:	SCM_UsbPhySet
 * Function:	to set usb phy 
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.01.29
 * ModifyRecord:
 * *************************************************************************/
void SCM_UsbPhySet(void)
{
	ScmRegs.USBPHYCTRL.bit.PhySel = 1;
	ScmRegs.USBPHYCTRL.bit.PhyMod = 1;
}

/****************************************************************************
 * Subroutine:	SCM_EnableUsbPuRes
 * Function:	to enable USB 1.5K Pull up Resistor
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.01.29
 * ModifyRecord:
 * *************************************************************************/
void SCM_EnableUsbPuRes(void)
{
	ScmRegs.USBPHYCTRL.bit.UsbPuRes = 1;
}

/****************************************************************************
 * Subroutine:	SCM_DisableUsbPuRes
 * Function:	to disable USB 1.5K Pull up Resistor
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.01.29
 * ModifyRecord:
 * *************************************************************************/
void SCM_DisableUsbPuRes(void)
{
	ScmRegs.USBPHYCTRL.bit.UsbPuRes = 0;
}

/****************************************************************************
 * Subroutine:	SCM_SetLdoPowerMode
 * Function:	to set LDO power mode
 * Input:		UINT8 nLdoMode -- to specify the LDO power mode, 0: nomal, 1: low power
 * Output:		None	
 * Description:	 
 * Date:		2013.10.14
 * ModifyRecord:
 * *************************************************************************/
void SCM_SetLdoPowerMode(UINT8 nLdoMode)
{
	SCM_RegWrEn();
	ScmRegs.PWRTOPCTRL.bit.LdoLpEn = nLdoMode;
	ScmRegs.REGWP0 = 0x00;
}

/****************************************************************************
 * Subroutine:	SYST_Config
 * Function:	to set system tick  
 * Input:		UINT8 nClkSource --	to specify the clock source for SysTick;
 *									0 -- STCLK; 1 -- SCLK
 *				UINT32 nTicks -- to specify the initialization counter value;
 *				UINT8 nMode -- to specify the work mode for SysTick
 *								0 -- counter mode; 1 -- interrupt mode		
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
UINT8 SYST_Config(UINT8 nClkSource, UINT32 nTicks, UINT8 nMode)
{
	if ((nTicks > SYSTICK_MAXCOUNT) || (nClkSource > SYST_SCLK) || (nMode > SYST_INT)) 
		return RT_FAIL;                                               

  	SysTick->LOAD  =  (nTicks & SYSTICK_MAXCOUNT) - 1;   	/* set reload register */
	/* set Priority for Cortex-M0 System Interrupts */
  	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); 
  	SysTick->VAL   =  (0x00);  /* Load the SysTick Counter Value */
	/* Enable SysTick Timer */
  	SysTick->CTRL = (nClkSource << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE) | (nMode<<SYSTICK_TICKINT);    
  	return RT_OK;                                                    
}



/****************************************************************************
 * Subroutine:	_delay_ms
 * Function:	to delay ms
 * Input:		UINT32 ms --	to specify the time of ms;
 * Output:		None	
 * Description:	this delay loops about [(4 * gCntMs + 7) * ms + 18] cycles 
 * Date:		2012.07.24
 * ModifyRecord:
 * *************************************************************************/
#pragma push
#pragma O3
#pragma Ospace
void _delay_ms(register UINT32 ms)
{
	UINT32 i;	
	while(ms--)
	{
		i =  gCntMs;
		while(i--)
		{
		}
	}
}
#pragma pop

/****************************************************************************
 * Subroutine:	_delay_us
 * Function:	to delay us
 * Input:		UINT32 us --	to specify the time of us;
 * Output:		None	
 * Description:	the delay loops about [(5 * gCntUs + 12) * us + 2]
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
#pragma push
#pragma O3
#pragma Ospace
void _delay_us(register UINT32 us)
{
	register UINT32 i, j;
	j = gCntUs;	
	while(j--)
	{
		i =  us;
		while(i--)
		{
		}
	}
}
#pragma pop



