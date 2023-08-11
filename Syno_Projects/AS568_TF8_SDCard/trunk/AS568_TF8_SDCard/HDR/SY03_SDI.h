/***************************************************************************************
* File name    :	SY03_SDI.h
* Function     :	Header of SY03 SD Card Controller
* Author       : 	coins
* Date         :	2011/07/08
* Version      :    v1.0
* Description  :    the SD Card Controller register header for SY03
* ModifyRecord :
*****************************************************************************************/
#ifndef SY03_SDI_H_
#define SY03_SDI_H_

/**************************************************************************
* Global Macro Definition
***************************************************************************/

#define	SDI_DAT_BUF		(UINT8 *)0x20004000
#define	SDI_DAT_BUF_SIZE	(16*1024)

/**************************************************************************
* Global Type Definition
***************************************************************************/

// SDI Status Register Bits Definition
struct	SDI_STATUS_BITS	{	// bits		description
	UINT32	DataTranOver:1;	// 0		Data transfer over 
	UINT32	RespRecOver:1;	// 1		Response receive over
	UINT32	Timeout:1;		// 2		Timeout
	UINT32	BusReady:1;		// 3		Bus Ready
	UINT32	DatCrcErr:1;	// 4		Data read CRC error
	UINT32	Rsvd3:1;		// 5		Reserved
	UINT32	RespCrcErr:1;	// 6		Write high to Busy low
	UINT32	Rsvd4:1;		// 7		Reserved
	UINT32	Rsvd5:24;		// 31:8		Reserved
};
union 	SDI_STATUS_REG	{
	UINT32	all;
	struct SDI_STATUS_BITS	bit;
};

//SDI Control Register Bits Definition
struct	SDI_CTRL_BITS	{	// bits		description
	UINT32	ClkEn:1;		// 0		Reset 
	UINT32	NxtDatStart:1;	// 1		next data block read/write operation start
	UINT32	Rsvd1:1;		// 2		Reserved
	UINT32	CmdSendStart:1;	// 3		command send start
	UINT32	BufBankSel:5;	// 8:4		data buffer RAM bank select
	UINT32	Rsvd2:7;		// 15:9		Reserved
	UINT32	Rsvd3:16;		// 31:16	Reserved
};
union 	SDI_CTRL_REG	{
	UINT32	all;
	struct SDI_CTRL_BITS	bit;
};

//SDI CRC Status Token Register Bits Definition (write block CRC Response)
struct	SDI_CRC_TOKEN_BITS	{	// bits		description
	UINT32	EndBit:1;			// 0		End bit 
	UINT32	Status:3;			// 3:1		CRC status
	UINT32	Rsvd2:4;			// 7:4		Reserved
	UINT32	Rsvd3:24;			// 31:8		Reserved
};
union 	SDI_CRC_TOKEN_REG	{
	UINT32	all;
	struct SDI_CRC_TOKEN_BITS	bit;
};

//SDI Clock Divider Register Bits Definition
struct	SDI_CLK_DIV_BITS	{	// bits		description
	UINT32	Div:8;			// 7:0			Reserved 
	UINT32	Rsvd1:24;		// 31:8			Reserved
};
union 	SDI_CLK_DIV_REG	{
	UINT32	all;
	struct SDI_CLK_DIV_BITS	bit;
};

//SDI Interrupt Register Bits Definition
struct	SDI_INT_BITS	{	// bits		description
	UINT32	DataTranOverInt:1;	// 0		Data transfer over 
	UINT32	Rsvd1:7;
	UINT32	RespRecOverInt:1;	// 8		command response over
	UINT32	Rsvd2:7;
	UINT32	TimeoutInt:1;		// 16		time out
	UINT32	Rsvd3:7;
	UINT32	Rsvd4:8;
};
union 	SDI_INT_REG	{
	UINT32	all;
	struct SDI_INT_BITS	bit;
};


// SDI Registers
struct	SDI_REGS	{
	union 	SDI_STATUS_REG	STATUS;			// SDI Status Register
	union	SDI_CTRL_REG	CTRL;			// SDI Control Register
	UINT32	TIMEOUT;						// SDI Command timerout Register
	union 	SDI_CRC_TOKEN_REG	CRC_TOKEN;	// SDI CRC Status Register
	union 	SDI_CLK_DIV_REG	CLK_DIVIDER;	// SDI Clock Divider Register
	union 	SDI_INT_REG		INT_FLAG;		// SDI Interrupt Flag Register
	union 	SDI_INT_REG		INT_EN;			// SDI Interrupt Enable Register
	UINT32	BLK_LEN;						// Block Length
	UINT32	CMD_BUF0;						// Command Buffer
	UINT32	CMD_BUF1;
	UINT32	RSP_BUF0;						// Response Buffer
	UINT32	RSP_BUF1;
	UINT32	RSP_BUF2;
	UINT32	RSP_BUF3;
};


/**************************************************************************
* Global Variable Declaration
***************************************************************************/

//Declaration of SD Card Interface Controller register variable
extern volatile struct SDI_REGS SdiRegs;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/


#endif /*SY03_SDI_H_*/
