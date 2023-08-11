/***************************************************************************
* File name    :	SY03_SCM.h
* Function     :	Header of SY03 System Control Moudle registers
* Author       : 	Hongjz
* Date         :	2014/07/11
* Version      :    v1.0
* Description  :    the System Control Module register header for SY03
* ModifyRecord :
****************************************************************************/
#ifndef SY03_SCM_H_
#define SY03_SCM_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/


/**************************************************************************
* Global Type Definition
***************************************************************************/
// System Clock Control Register bit field
struct	SCM_SYSCLKCTRL_BITS	{
	UINT32 	FclkSrc:1;
	UINT32 	Clk32kSel:1;
	UINT32 	FclkSel:2;
	UINT32 	Rsvd1:20;
	UINT32	UsbClk48mEn:1;
	UINT32	Rsvd3:7;
};
union	SCM_SYSCLKCTRL_REG	{
	UINT32	all;
	struct SCM_SYSCLKCTRL_BITS	bit;
};

// System Reset Control Register bit field
struct	SCM_SYSRSTCTRL_BITS	{
	UINT32	Bod:1;
	UINT32	IoVd:1;
	UINT32 	Rsvd1:6;
	UINT32	Td:1;	
	UINT32	Rsvd2:7;
	UINT32	CoreVd:1;
	UINT32 	Shield:1;
	UINT32	Rsvd3:6;
	UINT32	Sdi0:1;
	UINT32	Sdi1:1;	
	UINT32	Rsvd4:6; 
};
union	SCM_SYSRSTCTRL_REG	{
	UINT32	all;
	struct SCM_SYSRSTCTRL_BITS	bit;
};

// High Frequency ROSC Control Register bit field
struct	SCM_HFROSCCTRL_BITS	{
	UINT32 	HFROscCRE:1;
	UINT32 	Rsvd1:7;  			
	UINT32	HFROscTrim:10;
	UINT32	HFROscGrade:2;
	UINT32	TrimHLimit:10;
	UINT32	Rsvd3:2;
};
union	SCM_HFROSCCTRL_REG	{
	UINT32	all;
	struct SCM_HFROSCCTRL_BITS	bit;
};


// TPR Control Register bit field
struct	SCM_TPRCTRL_BITS	{
	UINT32 	TDLTrim:3;
	UINT32 	Rsvd1:5;  			
	UINT32 	TDHTrim:3;
	UINT32 	Rsvd2:21;	
};
union	SCM_TPRCTRL_REG	{
	UINT32	all;
	struct SCM_TPRCTRL_BITS	bit;
};

// Analog Module Power Enable Control Register bit field
struct	SCM_AMODENCTRL_BITS	{
	UINT32	Hfrosc:1;
	UINT32	Rsvd0:1;
	UINT32	AcmpAn:1;
	UINT32	Rsvd1:1;
	UINT32	Rom:1;
	UINT32	Ram:1;
	UINT32	TrngRing:1;
	UINT32	UsbPhy:1;
	UINT32	Tpr:1;
	UINT32	Vbg:1;
	UINT32	Rsvd3:6;
	UINT32	BvdAn:1;
	UINT32	Rsvd4:15; 
};
union	SCM_AMODENCTRL_REG	{
	UINT32	all;
	struct SCM_AMODENCTRL_BITS	bit;
};


// Power Top Control Register bit field
struct	SCM_PWRTOPCTRL_BITS	{		
	UINT32	LdoLpEn:1;
	UINT32	BODLv:1;
	UINT32	BODLvClk:1;
	UINT32	Rsvd1:5;
	UINT32	Sel33:1;
	UINT32	Rsvd2:23;
};
union	SCM_PWRTOPCTRL_REG	{
	UINT32	all;
	struct SCM_PWRTOPCTRL_BITS	bit;
};

// Clock Output Control Register bit field
struct	SCM_CLKOUTCTRL_BITS	{
	UINT32	Div:8;		
	UINT32	En:1;
	UINT32	Rsvd:23;	
};
union	SCM_CLKOUTCTRL_REG	{
	UINT32	all;
	struct SCM_CLKOUTCTRL_BITS	bit;
};

// System Reset Status Register bit field
struct	SCM_SYSRSTSTATUS_BITS	{
	UINT32	Bod:1;
	UINT32	IoVd:1;
	UINT32	Wdt:1;
	UINT32	Soft:1;
	UINT32 	Rsvd1:4;
	UINT32	Td:1;	
	UINT32	Rsvd2:7;
	UINT32	CoreVd:1;
	UINT32	Rsvd3:15;	 
};
union	SCM_SYSRSTSTATUS_REG	{
	UINT32	all;
	struct SCM_SYSRSTSTATUS_BITS	bit;
};

// Active Shield Control Register bit field
struct	SCM_SHIELDCTRL_BITS	{
	UINT32	TbusDiv:4;	
	UINT32	Rsvd1:4;	
	UINT32	Active:1;
	UINT32	Rsvd:23;	
};
union	SCM_SHIELDCTRL_REG	{
	UINT32	all;
	struct SCM_SHIELDCTRL_BITS	bit;
};

// System Self-Destruct Control Register bit field	 
struct	SCM_SYSSDCTRL_BITS	{
	UINT32	Rsvd1:1;
	UINT32	IoVd:1;
	UINT32 	Rsvd2:6;
	UINT32	Td:1;	
	UINT32	Rsvd3:7;
	UINT32	CoreVd:1;
	UINT32 	Shield:1;
	UINT32	Rsvd4:6;
	UINT32	Sdi0:1;
	UINT32	Sdi1:1;	
	UINT32	Rsvd5:6; 
};
union	SCM_SYSSDCTRL_REG	{
	UINT32	all;
	struct SCM_SYSSDCTRL_BITS	bit;
};

// USB PHY	 
struct	SCM_USBPHYCTRL_BITS	{
	UINT32	UsbPuRes:1;
	UINT32 	Rsvd1:6;
	UINT32	PhyMod:1;	
	UINT32	PhySel:1;
	UINT32	Rsvd2:23;
};
union	SCM_USBPHYCTRL_REG	{
	UINT32	all;
	struct SCM_USBPHYCTRL_BITS	bit;
};
// System Control Moudle Registers
struct	SCM_REGS{
	UINT32 SOCID;
	UINT32 MODCLKCTRL;		// Module clock gate control, bit defined as peripheral ID in file SY03_Device.h
	UINT32 MODRSTCTRL;	 	// Module reset control, bit defined as peripheral ID in file SY03_Device.h
	union SCM_SYSCLKCTRL_REG	SYSCLKCTRL;		// system clock control register
	union SCM_SYSRSTCTRL_REG	SYSRSTCTRL;		// system reset control register
	union SCM_HFROSCCTRL_REG	HFROSCCTRL;
	union SCM_TPRCTRL_REG		TPRCTRL;
	union SCM_AMODENCTRL_REG	AMODENCTRL;
	union SCM_PWRTOPCTRL_REG	PWRTOPCTRL;
	UINT32 MEMREMAP;
	union SCM_CLKOUTCTRL_REG 	CLKOUTCTRL;
	union SCM_SYSRSTSTATUS_REG	SYSRSTSTATUS;
	UINT32 Rsvd1[1];
	union SCM_SHIELDCTRL_REG	SHIELDCTRL;
	UINT32 SHIELDINITVAL;				// 0x38
	union SCM_SYSSDCTRL_REG		SYSSDCTRL;	// 0x3C
	UINT32 Rsvd2[2];
	UINT32 REGWP0;
	union SCM_USBPHYCTRL_REG  USBPHYCTRL;
	UINT32 SRAM_ADDR_KEY;
	UINT32 SRAM_DATA_KEY;
	UINT32 SRAM_EXT_INHIBIT;
	UINT32 REGWP1;
};
/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern 	volatile struct SCM_REGS  ScmRegs;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/


#endif /*SY03_SCM_H_*/
