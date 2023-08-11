/***************************************************************************
* File name    :	SY03_EFC.h
* Function     :	Header of SY03 Embedded Flash Controller registers
* Author       : 	coins
* Date         :	2012/09/11
* Version      :    v1.0
* Description  :    the Embedded Flash Controller register header for SY03
* ModifyRecord :
****************************************************************************/
#ifndef SY03_EFC_H_
#define SY03_EFC_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define	EFC_CTRL_PECMD		(1<<0)
#define	EFC_CTRL_BURSTEN	(1<<4)
#define	EFC_CTRL_ENCRYPT	(1<<8)


/**************************************************************************
* Global Type Definition
***************************************************************************/
// EFC Timing Configuration Register 1 Bit Definition
struct 	EFC_TC1_BITS	{// bits		description
	UINT32	Thv:8;		 // 7:0			
	UINT32	Terase:8;
	UINT32	Tprog:8;
	UINT32	Tpgs:8;
};
union	EFC_TC1_REG	{
	UINT32	all;
	struct EFC_TC1_BITS	bit;
};

//EFC Timing Configuration Register 0 Bit Definition
struct 	EFC_TC0_BITS	{// bits		description
	UINT32	Trcv:4;		// 	3:0			
	UINT32	Tpgh:4;		// 	7:4
	UINT32	Tadh:4;		// 	11:8
	UINT32	Tads:4;		//	15:12
	UINT32	Tsel:4;		// 	19:16
	UINT32	Txa:4;		//	23:20
	UINT32	Tnvh:4;		// 	27:24
	UINT32	Tnvs:4;		//	31:28
};
union	EFC_TC0_REG	{
	UINT32	all;
	struct EFC_TC0_BITS	bit;
};

// EFC Control Register Bit Definition
struct	EFC_CTRL_BITS	{// bits		description
   UINT32 PeCmd:1;    	//	0			Page Erase Command Assert
   UINT32 Rsvd1:3;    	//	3:1			reserved
   UINT32 BurstEn:1;	//	4			burst enable
   UINT32 Rsvd2:3;    	//	7:5			reserved
   UINT32 Encrypt:1;    //	8			Encrypt enable
   UINT32 Rsvd3:3;    	//	11:9		reserved
   UINT32 ReadMode:2;	//  13:12		Read mode
   UINT32 Rsvd4:18;		//	31:14		reserved
};
union	EFC_CTRL_REG	{
	UINT32	all;
	struct EFC_CTRL_BITS	bit;
};

//EFC Write Enable Register Bit Definition
struct	EFC_WREN_BITS	{// bits		description
   UINT32  Addr:19;    	 //	18:0		address for writeable
   UINT32  Rsvd1:12;     //	30:19		reserved
   UINT32  Wen:1;    	 //	31			write enable
};
union	EFC_WREN_REG	{
	UINT32	all;
	struct EFC_WREN_BITS	bit;
};

// EFC Registers
struct EFC_REGS	{
	union EFC_TC1_REG	TC1;			// Timing Configuration Register 1
	UINT32		STA;			// State Register
	UINT32		PEBA;			// Page Erase Byte Address
	union EFC_TC0_REG	TC0;			// Timing Configuration Register 0
	union EFC_CTRL_REG	CTRL;		// Control Register
	union EFC_WREN_REG	WREN;		// Write Enable Register
	UINT32		ENCRYPT;
	UINT32		CHECK;
	UINT32		DATAPRO;		// Data area start address
	UINT32      RSVD[3];
	UINT32      ADDR_ENC;      //0x30
};


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern volatile struct EFC_REGS EfcRegs;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/


#endif


