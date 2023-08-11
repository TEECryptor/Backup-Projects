/***************************************************************************
* File name    :	SY03_BVD.h
* Function     :	Header of SY03 BVD registers
* Author       : 	Leixj
* Date         :	2012/07/06
* Version      :    v0.1
* Description  :    the BVD register header for SY03
* ModifyRecord :
****************************************************************************/
#ifndef SY03_BVD_H_
#define SY03_BVD_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
// Define bits of BVD_CTRL
#define CTRL_CLKDIV		8
#define CTRL_SOC		4
#define	CTRL_RSTAN		2
#define CTRL_VINSEL		1

/**************************************************************************
* Global Type Definition
***************************************************************************/
// BVD Control Register Bit Definition
struct	BVD_CTRL_BITS{   //	bits	     description
	UINT32 Rsvd0:1;		 // 0			 reserved
	UINT32 VinSel:1;     //	1   		 voltage Range Select
	UINT32 RstAn:1;   	 //	2   	     Reset
	UINT32 Rvsd1:1;		 // 3			 reserved
	UINT32 Soc:1;        // 4            start control
	UINT32 Rsvd2:3;		 //	5:7			 reserved
	UINT32 ClkDiv:16;	 //	23:8		 clock divider
	UINT32 Rsvd3:8;    	 //	31:24   	 reserved
};
union	BVD_CTRL_REG	{
	UINT32	all;
	struct BVD_CTRL_BITS	bit;
};

// BVD STAT Register Bit Definition
struct	BVD_STAT_BITS{    //	bits	     description
   UINT32 Result:3;        //  2:0           Result
   UINT32 Rsvd1:1;   	   //	3	   	     reserved
   UINT32 Eoc:1;		   //	4			 end 
   UINT32 Eoclf:1;		   //	5			 Convert finish flag
   UINT32 Rsvd2:26;    	   //  31:6			 reserved
};
union	BVD_STAT_REG	{
	UINT32	all;
	struct BVD_STAT_BITS	bit;
};

// BVD Registers
struct BVD_REGS	{
	union BVD_CTRL_REG		CTRL;
	union BVD_STAT_REG      STAT;
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern volatile struct BVD_REGS BvdRegs;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

#endif /* SY03_BVD_H_ */
