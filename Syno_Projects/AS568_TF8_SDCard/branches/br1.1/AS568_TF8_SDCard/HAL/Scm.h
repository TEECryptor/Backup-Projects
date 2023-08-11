/***************************************************************************
* File name    :	Scm.h
* Function     :	Header of System Control Module
* Author       : 	coins
* Date         :	2012/07/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
****************************************************************************/
#ifndef SCM_H_
#define SCM_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define	ROM_SPACE_SIZE 		0x30
#define	ROM_SYSFREQ_ADD 	0x20000000
#define	ROM_GCNT_US1_ADD 	0x20000020
#define	ROM_GCNT_US2_ADD 	0x20000024
#define	ROM_GCNT_MS_ADD 	0x20000028

// define the default system clock frequency
#define		F_DEF_SYSCLK	(F_HFROSC/4)

// define the system tick stclk clock frequency
#define		F_SYST_STCLK	(F_LFROSC/4)

// define the clock source for SysTick
#define		SYST_STCLK	0	// the STCLK for SysTick
#define		SYST_SCLK	1	// the SCLK for SysTick  
// define the work mode	for SysTick
#define		SYST_CNT	0	// SysTick count but no interrupt
#define		SYST_INT	1  	// SysTick count and interrupt when down to 0
	

// Define the analog module
#define	AN_HFROSC		(1<<0)
#define	AN_ACMP			(1<<2)
#define	AN_ROM			(1<<4)
#define	AN_RAM			(1<<5)
#define	AN_TRNG_RING	(1<<6)
#define	AN_USB_PHY		(1<<7)
#define	AN_TPR			(1<<8)
#define	AN_TPR_VBG		(1<<9)
#define	AN_BVD			(1<<16)


/**************************************************************************
* Global Type Definition
***************************************************************************/
// define the system clock for setting
typedef	enum	{
	SYS_CLK_6M		= 0,
   	SYS_CLK_12M		= 1,
	SYS_CLK_24M		= 2,
	SYS_CLK_48M		= 3,
}SYS_CLK;

// define the vector table memory map type
typedef enum	{
	VT_MEM_ROM 		= 0,
	VT_MEM_SRAM 	= 1,
	VT_MEM_FLASH 	= 2,
}VT_MEM;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
// Common
__INLINE UINT32 SCM_GetSOCID(void);

// System Clock
UINT8 SCM_SetSysClk(SYS_CLK Clk);
UINT32 SCM_GetSysClk(void);
void SCM_SetSysClkLowFreq(void);
UINT8 SCM_RecoverSysClk(void);

// HFROSC 
void SCM_HFRoscPd(void);
UINT8 SCM_LoadHFRoscTrim(void);
__INLINE void SCM_HFRoscCRE(void);
__INLINE void SCM_SetHFRoscTrimMax(UINT16 nMax);

// Clock Output
__INLINE void SCM_SetClkout(UINT8 nDiv);
__INLINE void SCM_EnableClkout(void);
__INLINE void SCM_DisableClkout(void);

// Analog Module Power control
void SCM_AnModPowerEnable(UINT32 nAnMods);
void SCM_AnModPowerDisable(UINT32 nAnMods);

// Module Control
__INLINE void SCM_DisableModClk(UINT8 ModId);
__INLINE void SCM_EnableModClk(UINT8 ModId);
__INLINE void SCM_KeepModRst(UINT8 ModId);
__INLINE void SCM_ReleaseModRst(UINT8 ModId);

// Memory Control
__INLINE void SCM_VectReMapTo(VT_MEM mem);
//__INLINE void SCM_SetSramKey(UINT8 nAddrKey, UINT8 nDataKey);

// System Reset
__INLINE UINT32 SCM_GetRstStatus(void);
// BOD
void SCM_SetBodLv(UINT8 nBodLv);
__INLINE UINT8 SCM_GetBodLv(void);
__INLINE void SCM_EnableBodRst(void);
__INLINE void SCM_DisableBodRst(void);

// TPR
void SCM_EnableTpr(void);
void SCM_DisableTpr(void);
__INLINE void SCM_EnableCoreVdRst(void);
__INLINE void SCM_DisableCoreVdRst(void);
__INLINE void SCM_EnableIoVdRst(void);
__INLINE void SCM_DisableIoVdRst(void);
__INLINE void SCM_EnableTdRst(void);
__INLINE void SCM_DisableTdRst(void);
__INLINE void SCM_SetTdLowTrim(UINT8 nTrim);
__INLINE void SCM_SetTdHighTrim(UINT8 nTrim);
__INLINE UINT8 SCM_GetTdLowTrim(void);
__INLINE UINT8 SCM_GetTdHighTrim(void);

// Active Shield
__INLINE void SCM_EnableShieldRst(void);
__INLINE void SCM_DisableShieldRst(void);
__INLINE void SCM_EnableShield(UINT8 nDiv);
__INLINE void SCM_DisableShield(void);

__INLINE void SCM_DisableSramExe(void);

// USB
__INLINE void SCM_DisableUsbClk(void);
__INLINE void SCM_EnableUsbClk(void);
void SCM_EnableUsbPuRes(void);
void SCM_DisableUsbPuRes(void);
void SCM_UsbPhySet(void);

// internal LDO
void SCM_SetLdoPowerMode(UINT8 nLdoMode);
__INLINE SCM_GetLdoInputRange(void);

// System Tick
UINT8 SYST_Config(UINT8 nClkSource, UINT32 nTicks, UINT8 nMode);
__INLINE UINT32 SYST_GetCnt(void);
__INLINE void SYST_Stop(void);
__INLINE void SYST_Continue(void);

// Delay
void _delay_ms(UINT32 ms);
void _delay_us(UINT32 us);

/**************************************************************************
* Functon
***************************************************************************/
/****************************************************************************
 * Subroutine:	SCM_GetRstStatus
 * Function:	to get reset status 
 * Input:		None 
 * Output:		reset status	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT32 SCM_GetRstStatus(void)
{
	return ScmRegs.SYSRSTSTATUS.all;
}

/****************************************************************************
 * Subroutine:	SCM_RoscCRE
 * Function:	to enable HF ROSC clock recover 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2012.10.25
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_HFRoscCRE(void)
{
	ScmRegs.HFROSCCTRL.bit.HFROscCRE = 1;
}

/****************************************************************************
 * Subroutine:	SCM_SetHFRoscTrimMax
 * Function:	to set HF ROSC trimming maximum value 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_SetHFRoscTrimMax(UINT16 nMax)
{
	ScmRegs.REGWP1 = 0x32107654;
	ScmRegs.HFROSCCTRL.bit.TrimHLimit = nMax;
	ScmRegs.REGWP1 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_DisableSramExe
 * Function:	to disable SRAM execute function
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableSramExe(void)
{
	ScmRegs.REGWP1 = 0x32107654;
	ScmRegs.SRAM_EXT_INHIBIT = 0x45239876;
	ScmRegs.REGWP1 = 0;
}


/****************************************************************************
 * Subroutine:	SCM_DisableModClk
 * Function:	to disable Module clock
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableModClk(UINT8 ModId)
{
	ScmRegs.MODCLKCTRL &= ~(1<<ModId);
}

/****************************************************************************
 * Subroutine:	SCM_EnableModClk
 * Function:	to enable Module clock
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableModClk(UINT8 ModId)
{
	ScmRegs.MODCLKCTRL |= (1<<ModId);
}

/****************************************************************************
 * Subroutine:	SCM_KeepModRst
 * Function:	to keep Module in reset state
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_KeepModRst(UINT8 ModId)
{
	ScmRegs.MODRSTCTRL &= ~(1<<ModId);
}

/****************************************************************************
 * Subroutine:	SCM_ReleaseModRst
 * Function:	to release Module from reset state
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_ReleaseModRst(UINT8 ModId)
{
	ScmRegs.MODRSTCTRL |= (1<<ModId);
}

/****************************************************************************
 * Subroutine:	SCM_GetSOCID
 * Function:	to get SOC ID Code
 * Input:		None 
 * Output:		return SOC ID	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT32 SCM_GetSOCID(void)
{
	return ScmRegs.SOCID;
}

/****************************************************************************
 * Subroutine:	SCM_VectReMapTo
 * Function:	to re-map vector to specified memory
 * Input:		VT_MEM mem -- to specify the memory type  
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_VectReMapTo(VT_MEM mem)
{
	ScmRegs.MEMREMAP = mem;
}

///****************************************************************************
// * Subroutine:	SCM_SetSramKey
// * Function:	to set SRAM bus encryption key
// * Input:		UINT8 nAddrKey -- to specify the address bus encryption key
// *				UINT8 nDataKey -- to specify the data bus encryption key  
// * Output:		None	
// * Description:	 
// * Date:		2012.10.29
// * ModifyRecord:
// * *************************************************************************/
//__INLINE void SCM_SetSramKey(UINT8 nAddrKey, UINT8 nDataKey)
//{
//	ScmRegs.REGWP1 = 0x32107654;
//	ScmRegs.SRAM_ADDR_KEY = nAddrKey;
//	ScmRegs.SRAM_DATA_KEY = nDataKey;
//	ScmRegs.REGWP1 = 0;
//}


/****************************************************************************
 * Subroutine:	SCM_SetClkout
 * Function:	to set clock out
 * Input:		UINT8 nDiv -- to specify the divider of clock out (0~255) 
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_SetClkout(UINT8 nDiv)
{
	GpioRegs.GPAUSE0.bit.GPA0 = MUX_CLKOUT;
	ScmRegs.CLKOUTCTRL.all = nDiv | 0x100;
}

/****************************************************************************
 * Subroutine:	SCM_EnableClkout
 * Function:	to enable clock out
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableClkout(void)
{
	ScmRegs.CLKOUTCTRL.all |= 0x100;
}

/****************************************************************************
 * Subroutine:	SCM_DisableClkout
 * Function:	to disable clock out
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableClkout(void)
{
	ScmRegs.CLKOUTCTRL.all &= ~0x100;
}

/****************************************************************************
 * Subroutine:	SCM_GetBodLv
 * Function:	to get BOD level
 * Input:		None
 * Output:		current BOD Level (0: low; 1: high)	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT8 SCM_GetBodLv(void)
{
	return 	ScmRegs.PWRTOPCTRL.bit.BODLv;
}

/****************************************************************************
 * Subroutine:	SCM_EnableBodRst
 * Function:	to enable BOD reset
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableBodRst(void)
{
	ScmRegs.SYSRSTCTRL.bit.Bod = 1;
}

/****************************************************************************
 * Subroutine:	SCM_DisableBodRst
 * Function:	to disable BOD reset
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableBodRst(void)
{
	ScmRegs.SYSRSTCTRL.bit.Bod = 0;
}

/****************************************************************************
 * Subroutine:	SCM_EnableCoreVdRst
 * Function:	to enable CoreVd reset
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableCoreVdRst(void)
{
	ScmRegs.SYSRSTCTRL.bit.CoreVd = 1;
}
/****************************************************************************
 * Subroutine:	SCM_DisableCoreVdRst
 * Function:	to disable CoreVd reset
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableCoreVdRst(void)
{
	ScmRegs.SYSRSTCTRL.bit.CoreVd = 0;
}
/****************************************************************************
 * Subroutine:	SCM_EnableIoVdRst
 * Function:	to enable IoVd reset
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableIoVdRst(void)
{
	ScmRegs.SYSRSTCTRL.bit.IoVd = 1;
}
/****************************************************************************
 * Subroutine:	SCM_DisableIoVdRst
 * Function:	to disable IoVd reset
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableIoVdRst(void)
{
	ScmRegs.SYSRSTCTRL.bit.IoVd = 0;
}
/****************************************************************************
 * Subroutine:	SCM_EnableTdRst
 * Function:	to enable Td reset
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableTdRst(void)
{
	ScmRegs.SYSRSTCTRL.bit.Td = 1;
}
/****************************************************************************
 * Subroutine:	SCM_DisableTdRst
 * Function:	to disable Td reset
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableTdRst(void)
{
	ScmRegs.SYSRSTCTRL.bit.Td = 0;
}
/****************************************************************************
 * Subroutine:	SCM_SetTdLowTrim
 * Function:	to set Td low trimming
 * Input:		UINT8 nTrim -- to specify the trimming value (0~7)
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_SetTdLowTrim(UINT8 nTrim)
{
	ScmRegs.TPRCTRL.bit.TDLTrim = nTrim;
}
/****************************************************************************
 * Subroutine:	SCM_SetTdHighTrim
 * Function:	to set Td high trimming
 * Input:		UINT8 nTrim -- to specify the trimming value (0~7)
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_SetTdHighTrim(UINT8 nTrim)
{
	ScmRegs.TPRCTRL.bit.TDHTrim = nTrim;
}
/****************************************************************************
 * Subroutine:	SCM_GetTdLowTrim
 * Function:	to get Td low trimming
 * Input:		None
 * Output:		the low trimming value	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT8 SCM_GetTdLowTrim(void)
{
	return 	ScmRegs.TPRCTRL.bit.TDLTrim;
}

/****************************************************************************
 * Subroutine:	SCM_GetTdHighTrim
 * Function:	to get Td high trimming
 * Input:		None
 * Output:		the high trimming value	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT8 SCM_GetTdHighTrim(void)
{
	return 	ScmRegs.TPRCTRL.bit.TDHTrim;
}

/****************************************************************************
 * Subroutine:	SCM_EnableShieldRst
 * Function:	to enable active shield reset
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableShieldRst(void)
{
	ScmRegs.SYSRSTCTRL.bit.Shield = 1;	
}
/****************************************************************************
 * Subroutine:	SCM_DisableLdRst
 * Function:	to disable active shield  reset
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableShieldRst(void)
{
	ScmRegs.SYSRSTCTRL.bit.Shield = 0;
}

/****************************************************************************
 * Subroutine:	SCM_EnableShield
 * Function:	to enable active shield
 * Input:		UINT8 nDiv -- to specify the divider for active shield clock (0~15)
 * Output:		None	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableShield(UINT8 nDiv)
{
	ScmRegs.SHIELDINITVAL = 0xFFFF;
	ScmRegs.SHIELDCTRL.all = nDiv | (1<<8);
}

/****************************************************************************
 * Subroutine:	SCM_DisableShield
 * Function:	to disable active shield
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableShield(void)
{
	ScmRegs.SHIELDCTRL.all &= ~(1<<8);
	ScmRegs.SHIELDINITVAL = 0;
}

/****************************************************************************
 * Subroutine:	SCM_GetLdoInputRange
 * Function:	to get LDO input range flag
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.10.14
 * ModifyRecord:
 * *************************************************************************/
__INLINE SCM_GetLdoInputRange(void)
{
	return ScmRegs.PWRTOPCTRL.bit.Sel33;
}

/****************************************************************************
 * Subroutine:	SCM_EnableUsbClk
 * Function:	to enable USB 48MHz clock (enabled by default)
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.10.14
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_EnableUsbClk(void)
{
	ScmRegs.SYSCLKCTRL.bit.UsbClk48mEn = 1;	
}

/****************************************************************************
 * Subroutine:	SCM_DisableUsbClk
 * Function:	to disable USB 48MHz clock
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.10.14
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SCM_DisableUsbClk(void)
{
	ScmRegs.SYSCLKCTRL.bit.UsbClk48mEn = 0;	
}


/****************************************************************************
 * Subroutine:	SYST_GetCnt
 * Function:	to get SysTick current count value
 * Input:		None
 * Output:		UINT32 -- the value of current count	
 * Description:	 
 * Date:		2012.10.22
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT32 SYST_GetCnt(void)
{
	return SysTick->VAL;
}

/****************************************************************************
 * Subroutine:	SYST_Stop
 * Function:	to stop SysTick
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.10.22
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SYST_Stop(void)
{
	SysTick->CTRL &= ~(1<<SYSTICK_ENABLE);
}

/****************************************************************************
 * Subroutine:	SYST_Continue
 * Function:	to Continue SysTick count
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.10.22
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SYST_Continue(void)
{
	SysTick->CTRL |= (1<<SYSTICK_ENABLE);
}

#endif /*SCM_H_*/
