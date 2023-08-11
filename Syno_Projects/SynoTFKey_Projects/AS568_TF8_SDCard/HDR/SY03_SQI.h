/***************************************************************************
* File name    :	SY03_SQI.h
* Function     :	Header of SY03 Serial Quad Interface registers
* Author       :	Coins
* Date         :	2012/11/07
* Version      :    v1.0
* Description  :    the SQI register header for SY03.
* ModifyRecord :
****************************************************************************/
#ifndef SY03_SQI_H_
#define SY03_SQI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
//--------------- Define Bits for Registers -----------------------------
// define bits of INST or INST_READ
#define	MODE_INST			0
#define MODE_ADDR			2
#define MODE_DUMMY			4
#define	MODE_WAIT			6
#define	MODE_DATA			8
#define NUM_ADDR			12
#define	NUM_DUMMY			16
#define NUM_WAIT			20
#define	NUM_INST			23
#define INST_DATA			24

// define bits of INST_SET
#define	START				0
#define	EN_SPEC				4
#define	EN_CHANNEL			8
#define	EN_ENCRYPT			12
#define CS_LOW				13

// define bits of SPEC
#define	REG2				0
#define	REG1				8

// define bits of CLKGEN
#define	CLK_DIV				0
#define	EN_DIV				7
//-----------------------------------------------------------------------
/**************************************************************************
* Global Type Definition
***************************************************************************/
// SQI INST Bits Definition
struct	SQI_INST_BITS{     //bits	 description
	UINT32	Inst   :2;     //1:0     Inst transmit mode
    UINT32	Address:2;     //3:2     Address transmit mode
    UINT32	Dummy  :2;     //5:4     Dummy transmit mode
    UINT32	Wait   :2;     //7:6     Wait transmit mode
    UINT32	Data   :2;     //9:8     Data transmit mode
    UINT32	Rsvd1  :2;     //11:10   Reserved
    UINT32  AddNum :2;     //13:12   Address number
    UINT32  Rsvd2  :2;     //15:14   Reserved
    UINT32  DuNum  :3;     //18:16   Dummy number
    UINT32  Rsvd3  :1;     //19      Reserved
    UINT32  WaiNum :3;     //22:20
    UINT32  InNum  :1;     //23      Inst number
    UINT32  InData :8;     //31:24
};
union	SQI_INST_REG{
	UINT32	all;
	struct SQI_INST_BITS	bit;
};

// SQI INST_SET Bits Definition
struct	SQI_INST_SET_BITS{//bits	 description
	UINT32	Start  :1;     //0       start output flag
    UINT32	Rsvd1  :3;     //3:1     Reserved
    UINT32	Spec   :1;     //4       spec flag
    UINT32	Rsvd2  :3;     //7:5     Reserved
    UINT32	Channel:1;     //8       control the channel data flag
    UINT32  Rsvd3  :3;     //11:9    Reserved
    UINT32  Encrypt:1;     //12      encrypt flash data
    UINT32  CsLow  :1;     //13      Hold the chip select
    UINT32	Rsvd4  :18;    //31:14   Reserved
};
union	SQI_INST_SET_REG{
	UINT32	all;
	struct SQI_INST_SET_BITS	bit;
};

// SQI SPEC Bits Definition
struct	SQI_SPEC_BITS{     //bits	 description
	UINT32 Data0      :8;   //7:0
    UINT32 Data1      :8;   //15:8
    UINT32 Data2      :8;   //23:16
    UINT32 Data3      :8;   //31:24    Reserved
};
union	SQI_SPEC_REG{
	UINT32	all;
	struct SQI_SPEC_BITS	bit;
};

// SQI CLKGEN Bits Definition
struct	SQI_CLKGEN_BITS{   //bits	 description
	UINT32 ClkDiv  :7;     //6:0     spi CLOCK div
    UINT32 Divide  :1;     //7
    UINT32 Rsvd    :24;    //31:9    Reserved
};
union	SQI_CLKGEN_REG{
	UINT32	all;
	struct SQI_CLKGEN_BITS	bit;
};

// SQI DATA_ADDR Bits Definition
struct	SQI_DATA_ADDR_BITS{   //bits	 description
	UINT32 IdData  :8;        //7:0      read flash ID
    UINT32 Rsvd    :24;       //31:8     Reserved
};
union	SQI_DATA_ADDR_REG{
	UINT32	all;
	struct SQI_DATA_ADDR_BITS	bit;
};

// SQI INST_READ Bits Definition
struct	SQI_INST_READ_BITS{   //bits	 description
	UINT32	Inst   :2;     //1:0     Inst transmit mode
    UINT32	Address:2;     //3:2     Address transmit mode
    UINT32	Dummy  :2;     //5:4     Dummy transmit mode
    UINT32	Wait   :2;     //7:6     Wait transmit mode
    UINT32	Data   :2;     //9:8     Data transmit mode
    UINT32	Rsvd1  :2;     //11:10   Reserved
    UINT32  AddNum :2;     //13:12   Address number
    UINT32  Rsvd2  :2;     //15:14   Reserved
    UINT32  DuNum  :3;     //18:16   Dummy number
    UINT32  Rsvd3  :1;     //19      Reserved
    UINT32  WaiNum :3;     //22:20
    UINT32  InNum  :1;     //23      Inst number
    UINT32  InData :8;     //31:24
};
union	SQI_INST_READ_REG{
	UINT32	all;
	struct SQI_INST_READ_BITS  bit;
};

// SQI registers
struct	SQI_REGS{
	union SQI_INST_REG        INST;
	union SQI_INST_SET_REG    INST_SET;
	union SQI_SPEC_REG        SPEC;
	union SQI_CLKGEN_REG      CLKGEN;
	union SQI_DATA_ADDR_REG   DATA_ADDR;
	union SQI_INST_READ_REG   INST_READ;
	UINT32 ENCRYPT;
	UINT32 CHECK;
	UINT32 DATAPRO;
	UINT32 DATAOPEN;
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern volatile struct SQI_REGS SqiRegs;


/**************************************************************************
* Global Functon Declaration
***************************************************************************/




#endif /*SY03_SQI_H_*/
