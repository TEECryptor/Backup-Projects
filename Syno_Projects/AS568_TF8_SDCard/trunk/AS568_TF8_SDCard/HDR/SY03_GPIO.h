/****************************************************************************
* File name    :	SY03_GPIO.h
* Function     :	Header of SY03 General Purpose Input/Output Port
* 					Registers
* Author       : 	coins
* Date         :	2012/06/18
* Version      :    v1.0
* Description  :    the GPIO register header for SY03
* ModifyRecord :
****************************************************************************/
#ifndef SY03_GPIO_H_
#define SY03_GPIO_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
//the function of GPIO to be configured
//the value from 0 to 15, but not all mode are used
#ifndef _GPIO_MODE_
#define _GPIO_MODE_
#define INPUT     			0
#define FUNC1				1
#define FUNC2				2
#define FUNC3				3
#define OUTPUT    			8
#define	INT_RISE_EDGE		12
#define INT_FALL_EDGE		13

// Define the pheriphals' MUX 
#define MUX_SPI         FUNC1
#define MUX_I2C         FUNC1
#define MUX_UART        FUNC1
#define MUX_SQI         FUNC1
#define	MUX_TMR			FUNC1
#define MUX_SWD        	FUNC1
#define	MUX_ACMP		FUNC1
#define MUX_SSI         FUNC1
#define	MUX_USB			FUNC1
#define	MUX_SCI			FUNC1
#define	MUX_ISO7816		FUNC1
#define	MUX_SDI			FUNC2
#define	MUX_SDSI		FUNC2
#define	MUX_SWPS		FUNC1
#define	MUX_SWPM		FUNC1
#define	MUX_VPWM		FUNC1
#define MUX_CLKOUT      FUNC1
#define	MUX_SDIN		FUNC1
#endif		

/**************************************************************************
* Global Type Definition
***************************************************************************/
// GPIOA DATA/DATA SET/DATA CLR/INT STA Bits Definition
struct	GPIO_GPADATA_BITS	{
	UINT32	GPA0:1;
	UINT32	GPA1:1;
	UINT32	GPA2:1;
	UINT32	GPA3:1;
	UINT32	GPA4:1;
	UINT32	GPA5:1;
	UINT32	GPA6:1;
	UINT32	GPA7:1;
	UINT32	GPA8:1;
	UINT32	GPA9:1;
	UINT32	GPA10:1;
	UINT32	GPA11:1;
	UINT32	GPA12:1;
	UINT32	GPA13:1;
	UINT32	GPA14:1;
	UINT32	GPA15:1;
	UINT32	GPA16:1;
	UINT32	GPA17:1;
	UINT32	GPA18:1;
	UINT32	GPA19:1;
	UINT32	GPA20:1;
	UINT32	GPA21:1;
	UINT32	GPA22:1;
	UINT32	GPA23:1;
	UINT32	GPA24:1;
	UINT32	GPA25:1;
	UINT32	GPA26:1;
	UINT32	GPA27:1;
	UINT32	GPA28:1;
	UINT32	GPA29:1;
	UINT32	GPA30:1;
	UINT32	GPA31:1;
};
union	GPIO_GPADATA_REG	{
	UINT32	all;
	struct GPIO_GPADATA_BITS	bit;
};

// GPIOA USE0 Bits Definition
struct	GPIO_GPAUSE0_BITS	{// bits	description
	UINT32	GPA0:4;			//	3:0		Use of GPIOA0
	UINT32	GPA1:4;			//	7:4		Use of GPIOA1
	UINT32	GPA2:4;			//	11:8	Use of GPIOA2
	UINT32	GPA3:4;			//	15:12	Use of GPIOA3
	UINT32	GPA4:4;			//	19:16	Use of GPIOA4
	UINT32	GPA5:4;			//	23:20	Use of GPIOA5
	UINT32	GPA6:4;			//	27:24	Use of GPIOA6
	UINT32	GPA7:4;			//	31:28	Use of GPIOA7
};
union 	GPIO_GPAUSE0_REG	{
	UINT32	all;
	struct GPIO_GPAUSE0_BITS	bit;
};

//GPIOA USE1 Bits Definition
struct	GPIO_GPAUSE1_BITS	{// bits	description
	UINT32	GPA8:4;			//	3:0		Use of GPIOA8
	UINT32	GPA9:4;			//	7:4		Use of GPIOA9
	UINT32	GPA10:4;		//	11:8	Use of GPIOA10
	UINT32	GPA11:4;		//	15:12	Use of GPIOA11
	UINT32	GPA12:4;		//	19:16	Use of GPIOA12
	UINT32	GPA13:4;		//	23:20	Use of GPIOA13
	UINT32	GPA14:4;		//	27:24	Use of GPIOA14
	UINT32	GPA15:4;		//	31:28	Use of GPIOA15
};
union 	GPIO_GPAUSE1_REG	{
	UINT32	all;
	struct GPIO_GPAUSE1_BITS	bit;
};

//GPIOA USE2 Bits Definition
struct	GPIO_GPAUSE2_BITS	{// bits	description
	UINT32	GPA16:4;		//	3:0		Use of GPIOA16
	UINT32	GPA17:4;		//	7:4		Use of GPIOA17
	UINT32	GPA18:4;		//	11:8	Use of GPIOA18
	UINT32	GPA19:4;		//	15:12	Use of GPIOA19
	UINT32	GPA20:4;		//	19:16	Use of GPIOA20
	UINT32	GPA21:4;		//	23:20	Use of GPIOA21
	UINT32	GPA22:4;		//	27:24	Use of GPIOA22
	UINT32	GPA23:4;		//	31:28	Use of GPIOA23
};
union 	GPIO_GPAUSE2_REG	{
	UINT32	all;
	struct GPIO_GPAUSE2_BITS	bit;
};

//GPIOA USE3 Bits Definition
struct	GPIO_GPAUSE3_BITS	{// bits	description
	UINT32	GPA24:4;		//	3:0		Use of GPIOA24
	UINT32	GPA25:4;		//	7:4		Use of GPIOA25
	UINT32	GPA26:4;		//	11:8	Use of GPIOA26
	UINT32	GPA27:4;		//	15:12	Use of GPIOA27
	UINT32	GPA28:4;		//	19:16	Use of GPIOA28
	UINT32	GPA29:4;		//	23:20	Use of GPIOA29
	UINT32	GPA30:4;		//	27:24	Use of GPIOA30
	UINT32	GPA31:4;		//	31:28	Use of GPIOA31
};
union 	GPIO_GPAUSE3_REG	{
	UINT32	all;
	struct GPIO_GPAUSE3_BITS	bit;
};

//-------------- GPIOB --------------------------
//GPIOB DATA/DATA SET/DATA CLR/INT STA Bits Definition
struct	GPIO_GPBDATA_BITS	{
	UINT32	GPB0:1;
	UINT32	GPB1:1;
	UINT32	GPB2:1;
	UINT32	GPB3:1;
	UINT32	GPB4:1;
	UINT32	GPB5:1;
	UINT32	GPB6:1;
	UINT32	GPB7:1;
	UINT32	GPB8:1;
	UINT32	GPB9:1;
	UINT32	GPB10:1;
	UINT32	GPB11:1;
	UINT32	GPB12:1;
	UINT32	GPB13:1;
	UINT32	GPB14:1;
	UINT32	GPB15:1;
	UINT32	GPB16:1;
	UINT32	GPB17:1;
	UINT32	GPB18:1;
	UINT32	GPB19:1;
	UINT32	GPB20:1;
	UINT32	GPB21:1;
	UINT32	GPB22:1;
	UINT32	GPB23:1;
	UINT32	GPB24:1;
	UINT32	GPB25:1;
	UINT32	GPB26:1;
	UINT32	GPB27:1;
	UINT32	GPB28:1;
	UINT32	GPB29:1;
	UINT32	GPB30:1;
	UINT32	GPB31:1;
};
union	GPIO_GPBDATA_REG	{
	UINT32	all;
	struct GPIO_GPBDATA_BITS	bit;
};

//GPIOB USE0 Bits Definition
struct	GPIO_GPBUSE0_BITS	{// bits	description
	UINT32	GPB0:4;			//	3:0		Use of GPIOB0
	UINT32	GPB1:4;			//	7:4		Use of GPIOB1
	UINT32	GPB2:4;			//	11:8	Use of GPIOB2
	UINT32	GPB3:4;			//	15:12	Use of GPIOB3
	UINT32	GPB4:4;			//	19:16	Use of GPIOB4
	UINT32	GPB5:4;			//	23:20	Use of GPIOB5
	UINT32	GPB6:4;			//	27:24	Use of GPIOB6
	UINT32	GPB7:4;			//	31:28	Use of GPIOB7
};
union 	GPIO_GPBUSE0_REG	{
	UINT32	all;
	struct GPIO_GPBUSE0_BITS	bit;
};

//GPIOB USE1 Bits Definition
struct	GPIO_GPBUSE1_BITS	{// bits	description
	UINT32	GPB8:4;			//	3:0		Use of GPIOB8
	UINT32	GPB9:4;			//	7:4		Use of GPIOB9
	UINT32	GPB10:4;		//	11:8	Use of GPIOB10
	UINT32	GPB11:4;		//	15:12	Use of GPIOB11
	UINT32	GPB12:4;		//	19:16	Use of GPIOB12
	UINT32	GPB13:4;		//	23:20	Use of GPIOB13
	UINT32	GPB14:4;		//	27:24	Use of GPIOB14
	UINT32	GPB15:4;		//	31:28	Use of GPIOB15
};
union 	GPIO_GPBUSE1_REG	{
	UINT32	all;
	struct GPIO_GPBUSE1_BITS	bit;
};

//GPIOB USE2 Bits Definition
struct	GPIO_GPBUSE2_BITS	{// bits	description
	UINT32	GPB16:4;		//	3:0		Use of GPIOB16
	UINT32	GPB17:4;		//	7:4		Use of GPIOB17
	UINT32	GPB18:4;		//	11:8	Use of GPIOB18
	UINT32	GPB19:4;		//	15:12	Use of GPIOB19
	UINT32	GPB20:4;		//	19:16	Use of GPIOB20
	UINT32	GPB21:4;		//	23:20	Use of GPIOB21
	UINT32	GPB22:4;		//	27:24	Use of GPIOB22
	UINT32	GPB23:4;		//	31:28	Use of GPIOB23
};
union 	GPIO_GPBUSE2_REG	{
	UINT32	all;
	struct GPIO_GPBUSE2_BITS	bit;
};

//GPIOB USE3 Bits Definition
struct	GPIO_GPBUSE3_BITS	{// bits	description
	UINT32	GPB24:4;		//	3:0		Use of GPIOB24
	UINT32	GPB25:4;		//	7:4		Use of GPIOB25
	UINT32	GPB26:4;		//	11:8	Use of GPIOB26
	UINT32	GPB27:4;		//	15:12	Use of GPIOB27
	UINT32	GPB28:4;		//	19:16	Use of GPIOB28
	UINT32	GPB29:4;		//	23:20	Use of GPIOB29
	UINT32	GPB30:4;		//	27:24	Use of GPIOB30
	UINT32	GPB31:4;		//	31:28	Use of GPIOB31
};
union 	GPIO_GPBUSE3_REG	{
	UINT32	all;
	struct GPIO_GPBUSE3_BITS	bit;
};

// GPIOA DRIVE0 Bits Definition
struct	GPIO_GPADRIVE0_BITS	{// bits	description
	UINT32	GPA0:2;			//	1:0		Use of GPIOA0
	UINT32	GPA1:2;			//	3:2		Use of GPIOA1
	UINT32	GPA2:2;			//	5:4		Use of GPIOA2
	UINT32	GPA3:2;			//	7:6		Use of GPIOA3
	UINT32	GPA4:2;			//	9:8		Use of GPIOA4
	UINT32	GPA5:2;			//	11:10	Use of GPIOA5
	UINT32	GPA6:2;			//	13:12	Use of GPIOA6
	UINT32	GPA7:2;			//	15:14	Use of GPIOA7
	UINT32	GPA8:2;			//	17:16	Use of GPIOA8
	UINT32	GPA9:2;			//	19:18	Use of GPIOA9
	UINT32	GPA10:2;		//	21:20	Use of GPIOA10
	UINT32	GPA11:2;		//	23:22	Use of GPIOA11
	UINT32	GPA12:2;		//	25:24	Use of GPIOA12
	UINT32	GPA13:2;		//	27:26	Use of GPIOA13
	UINT32	GPA14:2;		//	29:28	Use of GPIOA14
	UINT32	GPA15:2;		//	31:30	Use of GPIOA15
};
union 	GPIO_GPADRIVE0_REG	{
	UINT32	all;
	struct GPIO_GPADRIVE0_BITS	bit;
};

// GPIOA DRIVE1 Bits Definition
struct	GPIO_GPADRIVE1_BITS	{// bits	description
	UINT32	GPA16:2;		//	1:0		Use of GPIOA16
	UINT32	GPA17:2;		//	3:2		Use of GPIOA17
	UINT32	GPA18:2;		//	5:4		Use of GPIOA18
	UINT32	GPA19:2;		//	7:6		Use of GPIOA19
	UINT32	GPA20:2;		//	9:8		Use of GPIOA20
	UINT32	GPA21:2;		//	11:10	Use of GPIOA21
	UINT32	GPA22:2;		//	13:12	Use of GPIOA22
	UINT32	GPA23:2;		//	15:14	Use of GPIOA23
	UINT32	GPA24:2;		//	17:16	Use of GPIOA24
	UINT32	GPA25:2;		//	19:18	Use of GPIOA25
	UINT32	GPA26:2;		//	21:20	Use of GPIOA26
	UINT32	GPA27:2;		//	23:22	Use of GPIOA27
	UINT32	GPA28:2;		//	25:24	Use of GPIOA28
	UINT32	GPA29:2;		//	27:26	Use of GPIOA29
	UINT32	GPA30:2;		//	29:28	Use of GPIOA30
	UINT32	GPA31:2;		//	31:30	Use of GPIOA31
};
union 	GPIO_GPADRIVE1_REG	{
	UINT32	all;
	struct GPIO_GPADRIVE1_BITS	bit;
};

// GPIOB DRIVE0 Bits Definition
struct	GPIO_GPBDRIVE0_BITS	{// bits	description
	UINT32	GPB0:2;			//	1:0		Use of GPIOB0
	UINT32	GPB1:2;			//	3:2		Use of GPIOB1
	UINT32	GPB2:2;			//	5:4		Use of GPIOB2
	UINT32	GPB3:2;			//	7:6		Use of GPIOB3
	UINT32	GPB4:2;			//	9:8		Use of GPIOB4
	UINT32	GPB5:2;			//	11:10	Use of GPIOB5
	UINT32	GPB6:2;			//	13:12	Use of GPIOB6
	UINT32	GPB7:2;			//	15:14	Use of GPIOB7
	UINT32	GPB8:2;			//	17:16	Use of GPIOB8
	UINT32	GPB9:2;			//	19:18	Use of GPIOB9
	UINT32	GPB10:2;		//	21:20	Use of GPIOB10
	UINT32	GPB11:2;		//	23:22	Use of GPIOB11
	UINT32	GPB12:2;		//	25:24	Use of GPIOB12
	UINT32	GPB13:2;		//	27:26	Use of GPIOB13
	UINT32	GPB14:2;		//	29:28	Use of GPIOB14
	UINT32	GPB15:2;		//	31:30	Use of GPIOB15
};
union 	GPIO_GPBDRIVE0_REG	{
	UINT32	all;
	struct GPIO_GPBDRIVE0_BITS	bit;
};

// GPIOB DRIVE1 Bits Definition
struct	GPIO_GPBDRIVE1_BITS	{// bits	description
	UINT32	GPB16:2;		//	1:0		Use of GPIOB16
	UINT32	GPB17:2;		//	3:2		Use of GPIOB17
	UINT32	GPB18:2;		//	5:4		Use of GPIOB18
	UINT32	GPB19:2;		//	7:6		Use of GPIOB19
	UINT32	GPB20:2;		//	9:8		Use of GPIOB20
	UINT32	GPB21:2;		//	11:10	Use of GPIOB21
	UINT32	GPB22:2;		//	13:12	Use of GPIOB22
	UINT32	GPB23:2;		//	15:14	Use of GPIOB23
	UINT32	GPB24:2;		//	17:16	Use of GPIOB24
	UINT32	GPB25:2;		//	19:18	Use of GPIOB25
	UINT32	GPB26:2;		//	21:20	Use of GPIOB26
	UINT32	GPB27:2;		//	23:22	Use of GPIOB27
	UINT32	GPB28:2;		//	25:24	Use of GPIOB28
	UINT32	GPB29:2;		//	27:26	Use of GPIOB29
	UINT32	GPB30:2;		//	29:28	Use of GPIOB30
	UINT32	GPB31:2;		//	31:30	Use of GPIOB31
};
union 	GPIO_GPBDRIVE1_REG	{
	UINT32	all;
	struct GPIO_GPBDRIVE1_BITS	bit;
};

// GPIO Registers
struct	GPIO_REGS	{
	//-------- GPIOA --------------
	union GPIO_GPAUSE0_REG	GPAUSE0;	// GPIOA use0 register
	union GPIO_GPAUSE1_REG	GPAUSE1;	// GPIOA use1 register
	union GPIO_GPAUSE2_REG	GPAUSE2;	// GPIOA use2 register
	union GPIO_GPAUSE3_REG	GPAUSE3;	// GPIOA use3 register
	union GPIO_GPADATA_REG	GPADATA;	// GPIOA data register
	union GPIO_GPADATA_REG	GPASET;		// GPIOA data set register
	union GPIO_GPADATA_REG 	GPACLR;		// GPIOA data clear register
	union GPIO_GPADATA_REG	GPATOG;		// GPIOA data toggle register
	union GPIO_GPADATA_REG	GPAINT;		// GPIOA interrupt flag and clear register
	union GPIO_GPADATA_REG	GPAPE;		// GPIOA Pull Enable
	union GPIO_GPADRIVE0_REG GPADRIVE0;	  //GPIOA Dirve control	register
	union GPIO_GPADRIVE1_REG GPADRIVE1;	  //GPIOA Dirve control	register 
//	UINT32 	RsvdA1[2];
	//union GPIO_GPADATA_REG	GPAPAD;		// GPIOA PAD pull up/down control register
	//--------- GPIOB -------------
	union GPIO_GPBUSE0_REG	GPBUSE0;	// GPIOB use0 register
	union GPIO_GPBUSE1_REG	GPBUSE1;	// GPIOB use1 register
	union GPIO_GPBUSE2_REG	GPBUSE2;	// GPIOB use2 register
	union GPIO_GPBUSE3_REG	GPBUSE3;	// GPIOB use3 register
	union GPIO_GPBDATA_REG	GPBDATA;	// GPIOB data register
	union GPIO_GPBDATA_REG	GPBSET;		// GPIOB data set register
	union GPIO_GPBDATA_REG 	GPBCLR;		// GPIOB data clear register
	union GPIO_GPBDATA_REG	GPBTOG;		// GPIOB data toggle register
	union GPIO_GPBDATA_REG	GPBINT;		// GPIOB interrupt flag and clear register
	union GPIO_GPBDATA_REG	GPBPE;		// GPIOB Pull Enable
	union GPIO_GPBDRIVE0_REG GPBDRIVE0;	   //GPIOB Dirve control register
	union GPIO_GPBDRIVE1_REG GPBDRIVE1;	   //GPIOB Dirve control register
//	UINT32 	RsvdB1[2];
	//union GPIO_GPBDATA_REG	GPBPAD;		// GPIOB PAD pull up/down control register
	//--------- GPIOC -------------
	UINT32  RsvdC[12];
	//---------- GPIOD -----------
	UINT32  RsvdD[12];
	//---------- GPIOE -----------
	UINT32  RsvdE[12];
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern 	volatile struct GPIO_REGS GpioRegs;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/


#endif /*SY03_GPIO_H_*/
