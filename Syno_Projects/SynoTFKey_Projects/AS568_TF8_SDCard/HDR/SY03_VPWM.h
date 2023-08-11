/***************************************************************************
* File name    :	SY03_VPWM.h
* Function     :	Header of SY03 VPWM registers
* Author       : 	Yang
* Date         :	2013/10/16
* Version      :    v0.1
* Description  :    the VPWM register header for SY03
* ModifyRecord :
****************************************************************************/
#ifndef SY03_VPWM_H_
#define SY03_VPWM_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/**************************************************************************
* Global Type Definition
***************************************************************************/
// VPWM Timer Register Bit Definition
struct	VPWM_TIMER_BITS{   //	bits	     description
   UINT32 FsPeriod:16;    		//	15:0      	frequency
   UINT32 Rsvd1:16;		//	31:16
};
union	VPWM_TIMER_REG	{
	UINT32	all;
	struct VPWM_TIMER_BITS	bit;
};

// VPWM config Register Bit Definition
struct	VPWM_CFG_BITS{   //	bits	     description
   UINT32 En:1;    		//	0
   UINT32 DutySelect:1;    		//	1
   UINT32 Sample_b:1;    		//	2
   UINT32 RLshift:1;    		//	3
   UINT32 ShiftBits:4;    		//	7:4
   UINT32 FuncSelect:1;         //8
   UINT32 Empty:1;              //9
   UINT32 Full:1;               //10
   UINT32 Rsvd1:21;		        //	31:11
};
union	VPWM_CFG_REG	{
	UINT32 all;
	struct VPWM_CFG_BITS bit;
};

// VPWM period Register Bit Definition
struct	VPWM_PR_BITS{   //	bits	     description
   UINT32 Period:16;    //	15:0
   UINT32 Rsvd1:16;		//	31:16
};
union	VPWM_PR_REG	{
	UINT32	all;
	struct VPWM_PR_BITS	bit;
};

// VPWM duty Register Bit Definition
struct	VPWM_DR_BITS{   //	bits	     description
   UINT32 Duty:16;      //	15:0
   UINT32 Rsvd1:16;		//	31:16
};
union	VPWM_DR_REG	{
	UINT32	all;
	struct VPWM_DR_BITS	bit;
};

// VPWM int Register Bit Definition
struct	VPWM_INTCTRL_BITS{   //	bits	     description
   UINT32 NoFullEn:1;      	//	0
   UINT32 EmptyEn:1;      	//	1
   UINT32 NoFullInt:1;      	//	2
   UINT32 EmptyInt:1;      	//	3
   UINT32 Rsvd1:28;			//	31:4
};
union	VPWM_INTCTRL{
	UINT32	all;
	struct VPWM_INTCTRL_BITS	bit;
};

// VPWM FIFO Register Bit Definition
struct	VPWM_FIFO_BITS{   //	bits	     description
   UINT32 FIFO:16;      //	15:0
   UINT32 Rsvd1:16;		//	31:16
};
union	VPWM_FIFO_REG	{
	UINT32	all;
	struct VPWM_FIFO_BITS	bit;
};

// VPWM Registers
struct VPWM_REGS	{
	union       VPWM_TIMER_REG         TIMER;
	union       VPWM_CFG_REG           CFG;
	union       VPWM_PR_REG            PR;
	union       VPWM_DR_REG            DR;
	union       VPWM_INTCTRL			INTCTRL;
	union       VPWM_FIFO_REG          FIFO;
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
// Declaration of VPWM register variable
extern volatile struct VPWM_REGS VpwmRegs;


/**************************************************************************
* Global Functon Declaration
***************************************************************************/


#endif 

