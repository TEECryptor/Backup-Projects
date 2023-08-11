/***************************************************************************
* File name    :	SY03_ISO7816.h
* Function     :	Header of SY03 ISO7816 Controller registers
* Author       : 	coins
* Date         :	2013/10/21
* Version      :    v1.0
* Description  :    the ISO7816 register header for SY03.
* ModifyRecord :
****************************************************************************/

#ifndef SY03_ISO7816_H_
#define SY03_ISO7816_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/**************************************************************************
* Global Type Definition
***************************************************************************/ 
struct	ISO7816_CON0_BITS   {	// bits		description
	UINT32  CLKOE:1;            //  0       clock output enable (master)
	UINT32  INV:1;              //  1       inverse convention
	UINT32  Rsvd_1:2;           //  3:2     Reserved
	UINT32  RBRST:1;            //  4       write 1 to reset rx buffer     
	UINT32  TR:1;               //  5       0:Rx mode; 1:Tx mode
	UINT32  Rsvd_2:1;           //  6       Reserved
	UINT32  MOD:1;              //  7       0:hardware;1:direct I/O
	UINT32  Rsvd_3:24;          //  31:8 Reserved
};

union  ISO7816_CON0_REG   {
     UINT32	all;
     struct ISO7816_CON0_BITS  bit;
};

struct      ISO7816_CON1_BITS   {   // bits                  description
	UINT32  TRY:3;                  //       2:0             auto tx times
	UINT32  RRY:3;                  //       5:3             auto rx times			
	UINT32  DIV:5;                  //       10:6            divider for clock output
	UINT32  Rsvd_1:5;               //       15:11           Reserved
	UINT32  EGT:9;                  //       24:16           extra guard time for tx	
	UINT32  Rsvd_2:7;               //       31:25           Reserved
};

union	ISO7816_CON1_REG   {
	UINT32	all;
	struct	ISO7816_CON1_BITS  bit;
};

struct	ISO7816_CON2_BITS   { 	// bits 	description
	UINT32 	WES0:1; 			//	0      	error signal width
	UINT32 	Rsvd_1:1;			//	1       Reserved
  	UINT32  TPS:1;				// 	2   	T=0 or T=1 protocal
 	UINT32	Rsvd_2:4; 			//	6:3  	Reserved
	UINT32	SBIT:1; 			// 	7  		I/O negedge flag
	UINT32	Rsvd_3:24; 			// 	31:8 	Reserved
};

union	ISO7816_CON2_REG   {
	UINT32     all;
    struct ISO7816_CON2_BITS  bit;
};

struct	ISO7816_STS_BITS 	{	// bits    	description
	UINT32	TBE:1;				//	0       tx buffer empty flag
	UINT32	RBF:1;              //  1     	rx buffer full flag
	UINT32	RBNE:1;             //	2      	rx buffer is not empty
	UINT32	PE:1;               //	3       parity error flag
	UINT32	OE:1;               //	4       rx buffer overrun error flag
	UINT32	WAIT:1;             // 	5   	1:register access is not finished
	UINT32	Rsvd_1:26;          // 	31:6 	Reserved
};

union	ISO7816_STS_REG	{
	UINT32	all;
	struct 	ISO7816_STS_BITS	bit;
};

struct	ISO7816_BRC_BITS	{	// bits    	description
 	UINT32 	DI:4;             	//	3:0     baud rate parameter: D
    UINT32	FI:4;               //	7:4  	baud rate parameter: F
    UINT32	Rsvd_1:24;          //	31:8 	Reserved
};

union	ISO7816_BRC_REG      {
	UINT32	all;
	struct 	ISO7816_BRC_BITS	bit;
};

struct	ISO7816_BUF_BITS 	{ 	// bits    	description
	UINT32 	DATA:8;           	//	7:0     tx or rx buffer
	UINT32  Rsvd_1:24;        	//	31:8 	Reserved
};

union	ISO7816_BUF_REG  	{
	UINT32	all;
	struct 	ISO7816_BUF_BITS	bits;
};

struct	ISO7816_DIO_BITS	{	// bits  	description
	UINT32	DIO:1;            	//	0      	data input/output
	UINT32	ENO:1;             	//	1       direct output enable
	UINT32 	Rsvd_1:30;         	//	31:2 	Reserved
};

union	ISO7816_DIO_REG 	{
	UINT32	all;
	struct 	ISO7816_DIO_BITS 	bit;
};

struct	ISO7816_MSK_BITS 	{	// bits  	description
	UINT32	MTBE:1; 			//	0      	mask for TBE
	UINT32	MRBF:1;            	// 	1      	mask for RBF
	UINT32	MRBNE:1;            //	2      	mask for RBNE
	UINT32	MPE:1;           	// 	3       mask for PE
	UINT32	MOE:1;             	// 	4      	mask for OE
	UINT32	MSBIT:1;          	// 	5      	mask for SBIT
	UINT32	Rsvd_2:26;         	// 	31:6 	Reserved
};

union	ISO7816_MSK_REG      {
	UINT32	all;
	struct 	ISO7816_MSK_BITS 	bit;
};

struct	ISO7816_TCON_BITS   { 	// bits    	description
	UINT32	TS:1;             	// 	0      	ETU counter start
	UINT32	TMOD:2;           	//	2:1    	ETU counter mode selection
	UINT32	TF:1;              	// 	3      	ETU counter overflow flag
	UINT32	Rsvd_1:28;        	// 	31:4 	Reserved 
};

union	ISO7816_TCON_REG   {
	UINT32	all;
	struct ISO7816_TCON_BITS  bit;
};

struct	ISO7816_TDAT_BITS 	{ 	// bits  	description
	UINT32	DAT:16;           	//	15:0 	ETU counter initial value
	UINT32 	Rsvd_1:16;        	//	31:16  	Reserved

};

union	ISO7816_TDAT_REG 	{
	UINT32	all;
	struct 	ISO7816_TDAT_BITS   bit;
};

struct	ISO7816_TRLD_BITS    { 	// bits    	description
	UINT32	RLDAT:16;          	//	15:0 	ETU counter reload value	
	UINT32 	Rsvd_1:16;         	// 	31:16   Reserved
};

union	ISO7816_TRLD_REG    {
	UINT32	all;   
	struct 	ISO7816_TRLD_BITS  bit;
};

struct	ISO7816_TMSK_BITS    {	// bits    	description
	UINT32	MTF:1;            	//	0    	mask for TF
    UINT32	Rsvd_1:31;      	//	31:1 	Reserved
};

union	ISO7816_TMSK_REG    {
	UINT32 	all;	
	struct 	ISO7816_TMSK_BITS  bit; 
};

struct	ISO7816_PROC_BITS    {	// bits    	description	  
	UINT32	DATA:8;       		//	7:0   	auto tx value	
    UINT32	Rsvd_1:24;         	//	31:8 	Reserved	
};

union        ISO7816_PROC_REG    {
         UINT32     all;	
         struct ISO7816_PROC_BITS  bit;	 
};

struct	ISO7816_REGS{
	union	ISO7816_CON0_REG	CON0;
    union	ISO7816_CON1_REG   	CON1;
    union	ISO7816_CON2_REG   	CON2;
    union	ISO7816_STS_REG    	STS;
    union	ISO7816_BRC_REG    	BRC;
    union	ISO7816_BUF_REG    	BUF;
    union	ISO7816_DIO_REG    	DIO;
    union	ISO7816_MSK_REG    	MSK;
    union	ISO7816_TCON_REG   	TCON0;
    union 	ISO7816_TDAT_REG   	TDAT0;
    union 	ISO7816_TRLD_REG   	TRLD0;
    union 	ISO7816_TMSK_REG   	TMSK0;
    union 	ISO7816_PROC_REG   	PROC0;
	union	ISO7816_TCON_REG   	TCON1;
    union 	ISO7816_TDAT_REG   	TDAT1;
    union 	ISO7816_TRLD_REG   	TRLD1;
    union 	ISO7816_TMSK_REG   	TMSK1;
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern 	volatile struct ISO7816_REGS  Iso7816Regs;


/**************************************************************************
* Global Functon Declaration
***************************************************************************/


#endif /*SY03_ISO7816_H_*/


