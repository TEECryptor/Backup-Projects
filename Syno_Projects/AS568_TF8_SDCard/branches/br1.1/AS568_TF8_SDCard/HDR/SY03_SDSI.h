/***************************************************************************
* File name    :	SY03_SDREG.h
* Function     :	Header of SY03 SD Slave Interface Controller registers
* Author       : 	
* Date         :	2013/12/15
* Version      :    
* Description  :    the SDREG register header for SY03.
* ModifyRecord :
****************************************************************************/
#ifndef SY03_SDSI_H_
#define SY03_SDSI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define	SDSI_BUFF_BASE_ADDR		0x20004000	// the SDSI read/write buffer base address
#define	SDSI_BUFF_BANK_SIZE		512		
#define	SDSI_BUFF_BANK_ADDR(BANK_NUM)		(SDSI_BUFF_BASE_ADDR + SDSI_BUFF_BANK_SIZE*BANK_NUM)
#define	SDSI_BUFF_BANK(BANK_NUM)	(void *)SDSI_BUFF_BANK_ADDR(BANK_NUM)


#define	SDSI_ADMA2_DESC_TBL_ADDR	0x20008C00	// the SDSI ADMA2 descriptor table


// Define the ADMA2 Descriptor Table Attibute field
#define	SDSI_ADMA2_DT_VALID_BIT		0
#define	SDSI_ADMA2_DT_VALID			(1<<SDSI_ADMA2_DT_VALID_BIT)

#define	SDSI_ADMA2_DT_END_BIT		1
#define	SDSI_ADMA2_DT_END			(1<<SDSI_ADMA2_DT_END_BIT)

#define	SDSI_ADMA2_DT_INT_BIT		2
#define	SDSI_ADMA2_DT_INT			(1<<SDSI_ADMA2_DT_INT_BIT)

#define	SDSI_ADMA2_DT_ACT_BITS		4
#define	SDSI_ADMA2_DT_ACT_NOP		(0<<SDSI_ADMA2_DT_ACT_BITS)
#define	SDSI_ADMA2_DT_ACT_RSV		(1<<SDSI_ADMA2_DT_ACT_BITS)
#define	SDSI_ADMA2_DT_ACT_TRAN		(2<<SDSI_ADMA2_DT_ACT_BITS)
#define	SDSI_ADMA2_DT_ACT_LINK		(3<<SDSI_ADMA2_DT_ACT_BITS)



// Define the SD SDIO interrupt bit
#define	SDSI_SD_SDIO_INT_SOFT_RST				(1<<0)
#define	SDSI_SD_SDIO_INT_VOLT_SWITCH_CMD		(1<<1)
#define	SDSI_SD_SDIO_INT_CMD19_RD_STRT			(1<<2)
#define	SDSI_SD_SDIO_INT_CMD19_RD_TRANS_OVER	(1<<3)
#define	SDSI_SD_SDIO_INT_FN0_WR_START			(1<<4)
#define	SDSI_SD_SDIO_INT_FN0_WR_TRN_OVER		(1<<5)
#define	SDSI_SD_SDIO_INT_FN0_RD_START			(1<<6)
#define	SDSI_SD_SDIO_INT_FN0_RD_TRN_OVER		(1<<7)
#define	SDSI_SD_SDIO_INT_FN0_RD_TRN_ERR			(1<<8)
#define	SDSI_SD_SDIO_INT_FN0_ADMA_END			(1<<9)
#define	SDSI_SD_SDIO_INT_FN0_ADMA				(1<<10)
#define	SDSI_SD_SDIO_INT_FN0_ADMA_ERR			(1<<11)
#define	SDSI_SD_SDIO_INT_ALL					(0x0FFF)

// Define the Mem interrupt bit
#define	SDSI_MEM_INT_PASSWORD_UPDATE			(1<<0)
#define	SDSI_MEM_INT_ERASE						(1<<1)
#define	SDSI_MEM_INT_PRE_ERASE					(1<<2)
#define	SDSI_MEM_INT_SD_RD_START				(1<<3)
#define	SDSI_MEM_INT_SD_WR_START				(1<<4)
#define	SDSI_MEM_INT_WR_OVER					(1<<5)
#define	SDSI_MEM_INT_RD_OVER					(1<<6)
#define	SDSI_MEM_INT_SEC_ERASE_PULSE			(1<<7)
#define	SDSI_MEM_INT_CHGE_SEC_WR_PULSE			(1<<8)
#define	SDSI_MEM_INT_ADMA_END					(1<<9)
#define	SDSI_MEM_INT_UPDATE_CSD					(1<<10)
#define	SDSI_MEM_INT_CMD20						(1<<11)
#define	SDSI_MEM_INT_ADMA						(1<<12)
#define	SDSI_MEM_INT_ADMA_ERR					(1<<13)
#define	SDSI_MEM_INT_CMD6_SWITCH				(1<<14)
#define	SDSI_MEM_INT_XPC						(1<<15)
#define	SDSI_MEM_INT_ALL						(0xFFFF)


// Define the Fn1 interrupt bit
#define	SDSI_FN1_INT_WR_OVER					(1<<0)
#define	SDSI_FN1_INT_RD_OVER					(1<<1)
#define	SDSI_FN1_INT_RD_ERR						(1<<2)
#define	SDSI_FN1_INT_RST						(1<<3)
#define	SDSI_FN1_INT_SD_HOST_FN1_MSG_RDY		(1<<4)
#define	SDSI_FN1_INT_ACK_TO_ARM					(1<<5)
#define	SDSI_FN1_INT_SDIO_RD_START				(1<<6)
#define	SDSI_FN1_INT_SDIO_WR_START				(1<<7)
#define	SDSI_FN1_INT_ADMA_END					(1<<8)
#define	SDSI_FN1_INT_SUSPEND					(1<<9)
#define	SDSI_FN1_INT_RESUME						(1<<10)
#define	SDSI_FN1_INT_ADMA						(1<<11)
#define	SDSI_FN1_INT_ADMA_ERR					(1<<12)
#define	SDSI_FN1_INT_FN1_EN						(1<<13)
#define	SDSI_FN1_INT_ALL						(0x3FFF)

// Define the Card Type
#define	MEM_CARD	1
#define	IO_CARD		2
#define	COMBO_CARD	3

							

/**************************************************************************
* Global Type Definition
***************************************************************************/

//Clock Wake-up Definition
struct	SDSI_CLK_WAKEUP_BITS {
	UINT32	AutoClkEn:1;  		//	0		
	UINT32	ManualClkEn:1;
	UINT32	Rsvd1:6;
	UINT32	Rsvd2:24;
};
union	SDSI_CLK_WAKEUP_REG {
	UINT32	all;
	struct SDSI_CLK_WAKEUP_BITS bit;
};

//ESW CCCR Reg Definition
struct SDSI_ESW_CCCR_BITS {
	UINT32 CCCRRev:4;
	UINT32 SDIORev:4;
	UINT32 SDRev:4;
	UINT32 SCSI:1;
	UINT32 SDC:1;
	UINT32 SMB:1;
	UINT32 SRW:1;
	UINT32 SBS:1;
	UINT32 S4MI:1;
	UINT32 LSC:1;
	UINT32 BLS:1;
	UINT32 SMPC:1;
	UINT32 SHS:1;
	UINT32 MemPre:2;
	UINT32 CmdIndex:6;
	UINT32 Rsvd:2;
};
union 	SDSI_ESW_CCCR_REG {
	UINT32 all;
	struct SDSI_ESW_CCCR_BITS bit;
};

// ESW Card Ready
struct 	SDSI_ESW_CARD_READY_BITS {
	UINT32 	CardRdy:1;
	UINT32  Rsvd:31;
};
union SDSI_ESW_CARD_READY_REG {
	UINT32 all;
	struct SDSI_ESW_CARD_READY_BITS bit;
};

// ESW Fun Ready
struct SDSI_ESW_FUN_READY_BITS {
	UINT32 FunRdy:1;
	UINT32 Rsvd:31;	
};
union SDSI_ESW_FUN_READY_REG {
	UINT32 all;
	struct SDSI_ESW_FUN_READY_BITS  bit;
};

// SD SDIO Interrupt Enable/Status Register
struct SDSI_SD_SDIO_INT_BITS {
	UINT32 SoftRst:1;      
	UINT32 VoltSwitchCmd:1;
	UINT32 Cmd19RdStrt:1;
	UINT32 Cmd19RdTransOver:1;
	UINT32 Fn0WrStart:1;
	UINT32 Fn0WrTrnOver:1;
	UINT32 Fn0RdStart:1;
	UINT32 Fn0RdTrnOver:1;
	UINT32 Fn0RdTrnErr:1;
	UINT32 Fn0AdmaEnd:1;
	UINT32 Fn0Adma:1;
	UINT32 Fn0AdmaErr:1;	
	UINT32 Rsvd:20;
};
union SDSI_SD_SDIO_INT_REG {
	UINT32 all;
	struct SDSI_SD_SDIO_INT_BITS bit;
};


// Soft reset valid
struct SDSI_SOFT_RESET_VALID_BITS {
	UINT32 	ahbsoft_valid:1;
	UINT32  Rsvd:31;	
};
union SDSI_SOFT_RESET_VALID_REG {
	UINT32 all;
	struct SDSI_SOFT_RESET_VALID_BITS bit;
};

// AHB MEM Interrupt Enable/Status Register
struct SDSI_MEM_INT_BITS {
	UINT32 PasswordUpdate:1;	  
	UINT32 Erase:1;
	UINT32 PreErase:1;
	UINT32 SdRdStart:1;
	UINT32 SdWrStart:1;      
	UINT32 MemWriteOver:1;
	UINT32 MemReadOver:1;
	UINT32 SecErasePulse:1;
	UINT32 ChgeSecWrPulse:1;
	UINT32 AdmaEnd:1;
	UINT32 UpdateCSD:1;
	UINT32 Cmd20:1;
	UINT32 Adma:1;
	UINT32 AdmaErr:1;
	UINT32 Cmd6Switch:1;
	UINT32 Xpc:1;		   
	UINT32 Rsvd:16;
};
union SDSI_MEM_INT_REG {
	UINT32 all;
	struct SDSI_MEM_INT_BITS bit;
};

//Global Interrupt Status/Enable Register
struct SDSI_GLOBAL_INT_BITS {
	UINT32 IntFrmFn0:1;	  
	UINT32 IntFrmMem:1;
	UINT32 IntFrmFn1:1;
	UINT32 Rsvd:29;
};
union SDSI_GLOBAL_INT_REG {
	UINT32 all;
	struct SDSI_GLOBAL_INT_BITS bit;
};

// IO Access mode	
struct SDSI_IO_ACCESS_MODE_BITS {
	UINT32 SSDR50:1;	  
	UINT32 SSDR104:1;
	UINT32 SDDR50:1;
	UINT32 SDTA:1;
	UINT32 SDTC:1;
	UINT32 SDTD:1;
	UINT32 SAI:1;		  
	UINT32 Rsvd:25;
};
union SDSI_IO_ACCESS_MODE_REG {
	UINT32 all;
	struct SDSI_IO_ACCESS_MODE_BITS bit;
};

// UHS Support
struct SDSI_UHS_SUPPORT_BITS {
	UINT32 UHSSupport:1;	  
	UINT32 DDRDlySelect:1;
	UINT32 CardVoltAccepted:1;
	UINT32 SdClkLineSwitched:1;
	UINT32 SdCmdLineSwitched:1;
	UINT32 Rsvd:27;
};
union SDSI_UHS_SUPPORT_REG {
	UINT32 all;
	struct SDSI_UHS_SUPPORT_BITS bit;
};


// Power control	   0x40
struct SDSI_PWR_CTRL_BITS {
	UINT32 EMPC:1;	  //BIT0
	UINT32 EPS:1;
	UINT32 Rsvd:30;
};
union SDSI_PWR_CTRL_REG {
	UINT32 all;
	struct SDSI_PWR_CTRL_BITS bit;
};

// Power state	 0x44
struct SDSI_PWR_STATE_BITS {
	UINT32 PwrStateFun1:1;	  //BIT0
	UINT32 Rsvd:31;
};
union SDSI_PWR_STATE_REG {
	UINT32 all;
	struct SDSI_PWR_STATE_BITS bit;
};
//Reserved	0x048 - 0x0FF

///***************************************************************************
//* Function1 Reg Global Type Definition
//* USE ARM Access Offset ESW Base Address + Offset
//****************************************************************************/

//AHB Interrupt Enable/Status Register (0x 118)
struct SDSI_AHB_INT_BITS {
	UINT32 Fn1WrOver:1;
	UINT32 Fn1RdOver:1;
	UINT32 Fn1RdErr:1;
	UINT32 Fun1Rst:1;
	UINT32 SdHostFn1MsgRdy:1;		  
	UINT32 Fn1AckToARM:1;
	UINT32 Fn1SdioRdStart:1;		  
	UINT32 Fn1SdioWrStart:1;
	UINT32 AdmaEnd:1;		  
	UINT32 Fn1Suspend:1;
	UINT32 Resume:1;		  
	UINT32 Adma:1;
	UINT32 AdmaErr:1;
	UINT32 Fun1En:1;		  
	UINT32 Rsvd:18;
};
union SDSI_AHB_INT_REG {
	UINT32 all;
	struct SDSI_AHB_INT_BITS bit;
};

//ESW FBR1 Register (0x 11c)
struct SDSI_ESW_FBR1_BITS {
	UINT32 IODeviceCode1:4;
	UINT32 FunCsaSup:1;
	UINT32 ExtendedIODeviceCode1:8;
	UINT32 SPS:1;		  
	UINT32 Rsvd:18;
};
union SDSI_ESW_FBR1_REG {
	UINT32 all;
	struct SDSI_ESW_FBR1_BITS bit;
};

//Reserved 0x130-0x13F

///***************************************************************************
//* SD Memory Register Address Mapping Global Type Definition
//****************************************************************************/
//
// MEM OCR Register (0x 200)
struct SDSI_MEM_OCR_BITS {
	UINT32 MemOCR:24;
	UINT32 Rsvd:6;	
	UINT32 CCS:1;
	UINT32 CardEccDisabled:1;
};
union SDSI_MEM_OCR_REG {
	UINT32 all;
	struct SDSI_MEM_OCR_BITS bit;
};

//Block size register (0x 244)
struct SDSI_BLOCK_SIZE_BITS {
	UINT32 BlockSize:12;
	UINT32 SinMulBlk:1;
	UINT32 Rsvd:19;	
};
union SDSI_BLOCK_SIZE_REG {
	UINT32 all;
	struct SDSI_BLOCK_SIZE_BITS bit;
};


//SD memory program Register (0x 25c)
struct SDSI_SD_MEM_PROGRAM_BITS {
	UINT32 WrProgDone:1;
	UINT32 Rsvd1:4;
	UINT32 AkeWpFail:1;
	UINT32 AkeError:1;
	UINT32 OutOfRange:1;
	UINT32 EccFail:1;
	UINT32 EraseParam:1;
	UINT32 Rsvd2:22;	
};
union SDSI_SD_MEM_PROGRAM_REG {
	UINT32 all;
	struct SDSI_SD_MEM_PROGRAM_BITS bit;
};

// Access Mode Register (0x 270)
struct SDSI_ACCESS_MODE_BITS {
	UINT32 SDR12:1;
	UINT32 SDR25:1;
	UINT32 SDR50:1;
	UINT32 SDR104:1;
	UINT32 DDR50:1;
	UINT32 DefaultCmdSys:1;
	UINT32 eC:1;
	UINT32 OTP:1;
	UINT32 ASSD:1;
	UINT32 eSD:1;
	UINT32 VendorSpec:1;
	UINT32 TypeB:1;
	UINT32 TypeA:1;
	UINT32 TypeC:1;
	UINT32 TypeD:1;
	UINT32 CurrLimit200:1;
	UINT32 CurrLimit400:1;
	UINT32 CurrLimit600:1;
	UINT32 CurrLimit800:1;
	UINT32 Rsvd:13;	
};
union SDSI_ACCESS_MODE_REG {
	UINT32 all;
	struct SDSI_ACCESS_MODE_BITS bit;
};

//SD Card Type (0x 280)
struct SDSI_SD_CARD_TYPE_BITS {
	UINT32 CardType:16; 
	UINT32 SpeedClass:8;
	UINT32 PerformanceMove:8;	
};
union SDSI_SD_CARD_TYPE_REG {
	UINT32 all;
	struct SDSI_SD_CARD_TYPE_BITS bit;
};

//SD Erase (0x 284)
struct SDSI_SD_ERASE_BITS {
	UINT32 EraseSize:16; 
	UINT32 EraseTimeout:6;
	UINT32 EraseOffset:2;
	UINT32 Rsvd:8;		
};
union SDSI_SD_ERASE_REG {
	UINT32 all;
	struct SDSI_SD_ERASE_BITS bit;
};

//CMD6_MODE (0x 288)
struct SDSI_CMD6_MODE_BITS {
	UINT32 AccessMode:4; 
	UINT32 CommandSystem:4;
	UINT32 DriverStrength:4;
	UINT32 CurrentLimit:4;
	UINT32 Rsvd:16;		
};
union SDSI_CMD6_MODE_REG {
	UINT32 all;
	struct SDSI_CMD6_MODE_BITS bit;
};


// SDSI registers
struct	SDSI_REGS 	{
	// FUN0 Registers
	union SDSI_CLK_WAKEUP_REG			CLK_WAKEUP;			// 0x00							   
	union SDSI_ESW_CCCR_REG				ESW_CCCR;			// 0x04
	UINT32 								ADMA_SYS_ADDR;		// 0x08
	union SDSI_ESW_CARD_READY_REG    	ESW_CARD_READY;		// 0x0c		READ ONLY
	union SDSI_ESW_FUN_READY_REG		ESW_FUN_READY;		// 0x10           
	union SDSI_SD_SDIO_INT_REG			SD_SDIO_INT_EN;		// 0x14
	union SDSI_SD_SDIO_INT_REG			SD_SDIO_INT_STATUS;	// 0x18
	union SDSI_SOFT_RESET_VALID_REG		SOFT_RESET_VALID;	// 0x1c
	union SDSI_MEM_INT_REG				MEM_INT_EN;			// 0x20
	union SDSI_MEM_INT_REG				MEM_INT_STUTAS;		// 0x24
	union SDSI_GLOBAL_INT_REG			GLOBAL_INT_EN;  	// 0x28
	union SDSI_GLOBAL_INT_REG			GLOBAL_INT_STATUS;	// 0x2c
	UINT32								CSA_PTR;			// 0x30		READ ONLY	24 bit
	union SDSI_IO_ACCESS_MODE_REG		IO_ACCESS_MODE;		// 0x34
	union SDSI_UHS_SUPPORT_REG			UHS_SUPPORT;		// 0x38
	UINT32 								CLK_DELAY_TIMER;	// 0x3c		32 bit
	union SDSI_PWR_CTRL_REG 		  	POWER_CONTROL;		// 0x40		READ ONLY
	union SDSI_PWR_STATE_REG			POWER_STATE;		// 0x44		READ ONLY
	UINT32 								SDSI_ADMA_GRANT;
	UINT32 RSVD0[45];				  						// 0x48 - 0xFF

	// FUN1 registers
	UINT32 								DATA_PORT;			// 0x100   	32 bit
	UINT32								IO_OCR;		 		// 0x104	24 bit	
	UINT32 RSVD1;						 					// 0x108
	UINT32								AHB_XFER_CNT;	  	// 0x10c 	21 bit
	UINT32						 		SDIO_XFER_CNT;	  	// 0x110	READ ONLY 	21 bit
	union SDSI_AHB_INT_REG				AHB_INT_STATUS;		// 0x114
	union SDSI_AHB_INT_REG				AHB_INT_EN;			// 0x118
	union SDSI_ESW_FBR1_REG				ESW_FBR1;	  		// 0x11c
	UINT32								ESW_IOR1;	  		// 0x120  	1 bit
	UINT32 								SD_HOST_GPR;		// 0x124	READ ONLY	32 bit
	UINT32 								ARM_GPR;			// 0x128	WRITE ONLY	32 bit
	UINT32								FUN1_RD_DATA_RDY;	// 0x12c   	1 bit
	UINT32 RSVD2[52];	   									// 0x130 - 0x1FF

	// SD Memory register
	union  SDSI_MEM_OCR_REG				MEM_OCR;			// 0x200
	UINT32 								LAST_FETCH_ADDR;	// 0x204	READ ONLY	32 bit
	UINT32 								CID0;				// 0x208   	32 bit
	UINT32 								CID1;				// 0x20c  	32 bit
	UINT32 								CID2;				// 0x210
	UINT32 								CID3;				// 0x214
	UINT32 								CSD0;				// 0x218
	UINT32 								CSD1;				// 0x21c
	UINT32 								CSD2;				// 0x220
	UINT32 								CSD3;				// 0x224
	UINT32 								SCR0;				// 0x228
	UINT32 								SCR1;				// 0x22c
	UINT32								PWD_LEN;			// 0x230 	8 bit
	UINT32 								PWD0;				// 0x234
	UINT32 								PWD1;				// 0x238
	UINT32 								PWD2;				// 0x23c
	UINT32 								PWD3;				// 0x240
	union  SDSI_BLOCK_SIZE_REG			BL0CK_SIZE; 		// 0x244 	READ ONLY
	UINT32 								ARGUMENT;			// 0x248	READ ONLY
	UINT32								NUM_WR_BLKS; 		// 0x24c   	10 bit
	UINT32 								ERASE_START_ADDR;	// 0x250 	READ ONLY	
	UINT32 								ERASE_END_ADDR;		// 0x254	READ ONLY
	UINT32								PRE_ERASE_BLK_CNT; 	// 0x258	READ ONLY	23 bit
	union  SDSI_SD_MEM_PROGRAM_REG		SD_MEM_PROGRAM;		// 0x25c
	UINT32								UNIT_CNT;	 		// 0x260 	10 bit
	UINT32								WRBLKCNT; 			// 0x264   	16 bit
	UINT32  							RDBLKCNT; 			// 0x268 	16 bit  
	UINT32								SECURE_REG;	 		// 0x26c  	9 bit
	union  SDSI_ACCESS_MODE_REG			ACCESS_MODE;	 	// 0x270 	
	UINT32								AU_SIZE;	 		// 0x274 	4 bit
	UINT32								MAX_CURR;			// 0x278   	16 bit
	UINT32 								PROTECTED_AREA_SIZE;// 0x27c   	32 bit
	union  SDSI_SD_CARD_TYPE_REG		SD_CARD_TYPE;		// 0x280
	union  SDSI_SD_ERASE_REG			SD_ERASE;		 	// 0x284
	union  SDSI_CMD6_MODE_REG			CMD6_MODE;			// 0x288
};

// SDSI ADMA2 Descriptor Table
struct	ADMA2_DESC_TBL{
	UINT16 ATTRIBUTE;
	UINT16 LENGTH;
	UINT32 ADDRESS;
};


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
// Definition of SDSI register variable
extern volatile struct SDSI_REGS SdsiRegs;
extern volatile struct ADMA2_DESC_TBL SdsiADMA2DescTbl[32];


/**************************************************************************
* Global Functon Declaration
***************************************************************************/


#endif	  /*SY03_SDSI_H_*/
