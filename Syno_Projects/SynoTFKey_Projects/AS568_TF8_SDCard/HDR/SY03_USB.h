/***************************************************************************************
* File name    :	SY03_USB.h
* Function     :	Header of SY03 USB registers
* Author       : 	coins
* Date         :	2012/01/03
* Version      :    v1.0
* Description  :    the USB register header for SY03
* ModifyRecord :
*****************************************************************************************/
#ifndef SY03_USB_H_
#define SY03_USB_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/**************************************************************************
* Global Type Definition
***************************************************************************/
//#pragma pack(push) //±£´æ¶ÔÆë×´Ì¬ 
#pragma pack(1)
//----- USB FADDR Register Bit Definition-----
struct USB_FADDR_BITS		{//	bits	description
	  UINT8 Addr:7;		//	Address of usb
	  UINT8 Update:1;	//	Set when FAddr is written. Cleared when the new address takes effect (at the end of the current transfer).	  
};

union USB_FADDR_REG	{//reset is 0x00
	UINT8 all;
	struct USB_FADDR_BITS bit;
};
//---- USB POWER Register Bit Definition -------
struct USB_POWER_BITS{//	bits	description
	  UINT8 EnSuspendM:1;	//Set by the CPU to enable the SUSPENDM signal.
	  UINT8 SuspendMode:1;	//	This read-only bit is set when Suspend mode is entered. It is cleared when the CPU reads the
                             //interrupt register, or sets the Resume bit of this register
      UINT8 Resume:1; //Set by the CPU to generate Resume signaling when the function is in Suspend mode. The CPU
                      //should clear this bit after 10 ms (a maximum of 15 ms) to end Resume signaling.  
      UINT8 Reset:1;//This read-only bit is set when Reset signaling has been detected on the bus
      
      UINT8 Rev:3;
      UINT8 ISOUpdate:1; 
};
union USB_POWER_REG	{//reset is 0x20
	UINT8 all;
	struct USB_POWER_BITS bit;
};
//----- USB Interrupt IN Register Bit Definition-----
struct USB_INTRIN_BITS	{		//	bits	description
	   UINT16 EP0:1;  	        //	0 		EP0 interrupt
	   UINT16 EP1In:1;   	    //	1 		EP1 In interrupt
	   UINT16 EP2In:1;			//	1		EP2 In interrupt
	   UINT16 EP3In:1;   	    //	1 		EP3 In interrupt
	   UINT16 EP4In:1;	 		//	1		EP4 In interrupt
	   UINT16 Rev:11;
};
union USB_INTRIN_REG{//reset is 0x0000
	 UINT16 all;
	 struct USB_INTRIN_BITS bit;
};
//------ USB Interrupt OUT Register Bit Definition------
struct USB_INTROUT_BITS	{		//	bits	description
	   UINT16 RESERVED:1;  	     //	0 	 
	   UINT16 EP1Out:1;   	    //	1 		EP1 Out interrupt
	   UINT16 EP2Out:1;	 		//	1		EP2 Out interrupt
	   UINT16 EP3Out:1;   	    //	1 		EP3 Out interrupt
	   UINT16 EP4Out:1;			//	1		EP4 Out interrupt
	   UINT16 REV:11;
};
union USB_INTROUT_REG{//reset is 0x0000
	 UINT16 all;
	 struct USB_INTROUT_BITS bit;
};

//---- USB Interrupt enable IN Register Bit Definition------
struct USB_INTRINE_BITS	{		//	bits	description
	   UINT16 EP0:1;  	        //	0 		EP0 interrupt
	   UINT16 EP1In:1;   	    //	1 		EP1 In interrupt
	   UINT16 EP2In:1;			//	1		EP2 In interrupt
	   UINT16 EP3In:1;   	    //	1 		EP3 In interrupt
	   UINT16 EP4In:1;	 		//	1		EP4 In interrupt
	   UINT16 Rev:11;
};
union USB_INTRINE_REG{//reset is 0xffff
	 UINT16 all;
	 struct USB_INTRINE_BITS bit;
};
//----- USB Interrupt enable OUT Register Bit Definition-----
struct USB_INTROUTE_BITS	{		//	bits	description
	   UINT16 RESERVED:1;  	     //	0 	 
	   UINT16 EP1Out:1;   	    //	1 		EP1 Out interrupt
	   UINT16 EP2Out:1;			//	1		EP2 Out interrupt
	   UINT16 EP3Out:1;   	    //	1 		EP3 Out interrupt
	   UINT16 EP4Out:1; 		//	1		EP4 Out interrupt
	   UINT16 Rev:11;
};
union USB_INTROUTE_REG{//reset is 0xffff
	 UINT16 all;
	 struct USB_INTROUTE_BITS bit;
};

//---- USB Interrupt Register Bit Definition------- 
struct USB_INTRUSB_BITS{//	bits	description
	  UINT8 Suspend:1;	//Set when suspend signaling is detected on the bus.
	  UINT8 Resume:1; //Set when resume signaling is detected on the bus while the MUSBHSFC is in Suspend mode..  
      UINT8 Reset:1;//Set when reset signaling is detected on the bus.
      UINT8 SOF:1;//Set at the start of each frame      
      UINT8 Reserved:4;                       
};
union USB_INTRUSB_REG	{//reset is 0x00
	UINT8 all;
	struct USB_INTRUSB_BITS bit;
};
//----- USB Interrupt Enable Register Bit Definition -----
struct USB_INTRUSBE_BITS		{//	bits	description
	  UINT8 Suspend:1;	//Set when suspend signaling is detected on the bus.
	  UINT8 Resume:1; //Set when resume signaling is detected on the bus while the MUSBHSFC is in Suspend mode..  
      UINT8 Reset:1;//Set when reset signaling is detected on the bus.
      UINT8 SOF:1;//Set at the start of each frame      
      UINT8 Reserved:4;                       
};
union USB_INTRUSBE_REG	{//reset is 0x06
	UINT8 all;
	struct USB_INTRUSBE_BITS bit;
};

//----- USB Frame Register Bit Definition------- 
struct USB_FRAME_BITS		{//	bits	description
	  UINT16 FrameNumber:11;	     
      UINT16 Reserved:5;                      
};
union USB_FRAME_REG	{//reset is 0x0000
	UINT16 all;
	struct USB_FRAME_BITS bit;
};

//----- USB Index Register Bit Definition --------
struct USB_INDEX_BITS		{//	bits	description
	  UINT8 EPIndex:4;	     
      UINT8 Reserved:4;                     
                                              
};
union USB_INDEX_REG	{//reset is 0x0000
	UINT8 all;
	struct USB_INDEX_BITS bit;
};
// ----USB control and status bits for Endpoint 0 Register Bit Definition------
struct USB_CSR0_BITS	{//	bits	description
	    UINT8 OutPktRdy:1;		//0 This bit is set when a data packet has been received.
	    UINT8 InPktRdy:1;		//1 The CPU sets this bit after loading a data packet into the FIFO.
	    UINT8 SentStall:1;		//2 This bit is set when a STALL handshake is transmitted. The CPU should clear this bit.
	    UINT8 DataEnd:1;		//3 The CPU sets this bit:
                                    //1. When setting InPktRdy for the last data packet.
                                    //2. When clearing OutPktRdy after unloading the last data packet.
                                    //3. When setting InPktRdy for a zero length data packet.
                                    //It is cleared automatically.
	    UINT8 SetupEnd:1;		//4 This bit will be set when a control transaction ends before the DataEnd bit has been set.
                                   //An interrupt will be generated and the FIFO flushed at this time.
	    UINT8 SendStall:1;		//5 The CPU writes a 1 to this bit to terminate the current transaction. The STALL handshake
                                //will be transmitted and then this bit will be cleared automatically.
	    UINT8 ServicedOutPktRdy:1;//6 The CPU writes a 1 to this bit to clear the OutPktRdy bit. It is cleared automatically.
	    UINT8 ServicedSetupEnd:1;//7 The CPU writes a 1 to this bit to clear the SetupEnd bit. It is cleared automatically.
	     
};
union USB_CSR0_REG{//reset is 0x00
	    UINT8 all;
	    struct USB_CSR0_BITS bit;
};
//---- USB Count of EP0 FIFO Register Bit Definition------ 
struct USB_COUNT0_BITS		{//	bits	description
	  UINT8 Count:7;	     
      UINT8 Reserved:1;                     
                                              
};
union USB_COUNT0_REG	{//reset is 0x0000
	UINT8 all;
	struct USB_COUNT0_BITS bit;
};

//------- USB provides control and status bits for IN transactions Register Bit Definition
struct USB_INCSRL_BITS	{//	bits	description
	    UINT8 InPktRdy:1;		//0 The CPU sets this bit after loading a data packet into the FIFO.
	    UINT8 FIFONotEmpty:1;	//1 This bit is set when there is at least 1 packet in the IN FIFO.
	    UINT8 UnderRun:1;		//2 In ISO mode,
	    UINT8 FlushFIFO:1;		//3 TThe CPU writes a 1 to this bit to flush the next packet to be transmitted from the endpoint IN FIFO.
	    UINT8 SendStall:1;		//4 The CPU writes a 1 to this bit to issue a STALL handshake to an IN token.
	    UINT8 SentStall:1;		//5 This bit is set when a STALL handshake is transmitted.
	    UINT8 ClrDataTog:1;//6 The CPU writes a 1 to this bit to reset the endpoint IN data toggle to 0.
	    UINT8 IncompTx:1;//7 used in ISO mode
};
union USB_INCSRL_REG{//reset is 0x00
	    UINT8 all;
	    struct USB_INCSRL_BITS bit;
};

//------- USB provides additional control for IN transactions Register Bit Definition
struct USB_INCSRU_BITS	{//	bits	description
	    UINT8 Reserved1:1;		//0 The CPU sets this bit after loading a data packet into the FIFO.
	    UINT8 DPktBufDis:1;	    //1 This bit is used to control the use of double packet buffering. It is ignored when Dynamic Fifo is enabled
	    UINT8 Reserved2:1;		//2 The CPU sets this bit to select DMA Request Mode 1 and clears it to select DMA Request Mode 0.
	    UINT8 FrcDataTog:1;		//3 sets this bit to force the endpoint¡¯s IN data toggle to switch after each data packet is sent
	    UINT8 Reserved3:1;		//4 sets this bit to enable the DMA request for the IN endpoint.
	    UINT8 Mode:1;		//5 sets this bit to enable the endpoint direction as IN, and clears it to enable the endpoint direction as OUT.
	    UINT8 ISO:1;//6  used in ISO mode
	    UINT8 AutoSet:1;//7 If the CPU sets this bit, InPktRdy will be automatically set when data of the maximum packet size
                          //(as set in InMaxP) is loaded into the IN FIFO. If a packet of less than the maximum packet size is
                           //loaded, InPktRdy will have to be set manually. Note:	     
};
union USB_INCSRU_REG{//reset is 0x00
	    UINT8 all;
	    struct USB_INCSRU_BITS bit;
};
// -----USB Count of EPx OUT Max amount of data that can be transferred through the selected IN endpoint in a single
//operation. Register Bit Definition 
struct USB_OUTMAXP_BITS		{//	bits	description
	  UINT8 MaxAmount:3;	     
      UINT8 Reserved:5;                                              
};
union USB_OUTMAXP_REG	{//reset is 0x0000
	UINT8 all;
	struct USB_OUTMAXP_BITS bit;
};

//------- USB provides control and status bits for Out transactions Register Bit Definition
struct USB_OUTCSRL_BITS	{//	bits	description
	    UINT8 OutPktRdy:1;		//0 This bit is set when a data packet has been received.
	    UINT8 FIFOFull:1;	//1 This bit is set when no more packets can be loaded into the OUT FIFO.
	    UINT8 OverRun:1;		//2 This bit is set if an OUT packet arrives while FIFOFull is set
	    UINT8 DataError:1;		//3 This bit is set at the same time that OutPktRdy is set if the data packet has a CRC error.
	    UINT8 FlushFIFO:1;		//4 writes a 1 to this bit to flush the next packet to be read from the endpoint OUT FIFO.
	    UINT8 SendStall:1;		//5 writes a 1 to this bit to issue a STALL handshake to a DATA packet.
	    UINT8 SentStall:1;//6 This bit is set when a STALL handshake is transmitted. The CPU should clear this bit.
	    UINT8 ClrDataTog:1;//7 writes a 1 to this bit to reset the endpoint data toggle to 0.	     
};
union USB_OUTCSRL_REG{//reset is 0x00
	    UINT8 all;
	    struct USB_OUTCSRL_BITS bit;
};

//----- USB provides additional control for Out transactions Register Bit Definition
struct USB_OUTCSRU_BITS	{//	bits	description
	    UINT8 Reserved1:1;		//0 This bit is set in a high-bandwidth Isochronous transfer if the packet in the OUT FIFO is incomplete
	    UINT8 DPktBufDis:1;	    //1 This bit is used to control the use of double packet buffering. It is ignored when Dynamic Fifo is enabled.
	    UINT8 Reserved2:1;		//2
	    UINT8 Reserved3:1;		//3 Two modes of DMA Request operation are supported:
	    UINT8 DisNYETPIDError:1;		//4 sets this bit to disable the sending of NYET handshakes.
	    UINT8 Reserved4:1;		//5 sets this bit to enable the DMA request for the OUT endpoint.
	    UINT8 ISO:1;//6  used in ISO mode
	    UINT8 AutoClear:1;//7 sets this bit then the OutPktRdy bit will be automatically cleared when a packet of
                            //OutMaxP bytes has been unloaded from the OUT FIFO.
	     
};
union USB_OUTCSRU_REG{//reset is 0x00
	    UINT8 all;
	    struct USB_OUTCSRU_BITS bit;
};
 
union USB_STAS_REG{    
    union USB_CSR0_REG CSR0;
    union USB_INCSRL_REG INCSRL;};
//=========== USB Registers=====================

struct	USB_REGS{
	union  USB_FADDR_REG	FADDR; 	//8
	union  USB_POWER_REG 	POWER;	//8
	union  USB_INTRIN_REG   INTRIN;	//16
	union  USB_INTROUT_REG  INTROUT;   //16
	union  USB_INTRUSB_REG  INTRUSB;//8	  
	union  USB_INTRINE_REG  INTRINE;	//16	
	union  USB_INTROUTE_REG INTROUTE;//16

	union  USB_INTRUSBE_REG INTRUSBE;//8
	UINT16 FRAME;	//16//union  USB_FRAME_REG    FRAME;
    UINT8 INDEX;	//8//union  USB_INDEX_REG     INDEX;
    UINT8 RESERVED1; //8
    UINT8 INMAXP;  //8//union USB_INMAXP_REG    INMAXP;	
    union USB_STAS_REG       STAS;//8
	union USB_INCSRU_REG     INCSRU;//8
    UINT8 OUTMAXP;	//8 
	union USB_OUTCSRL_REG     OUTCSRL;//8	
	union USB_OUTCSRU_REG     OUTCSRU;//8
	UINT16 OUTCOUNT;//16
	UINT16 RESERVED2[4];
	UINT32 FIFO_EP0;
	UINT32 FIFO_EP1;
	UINT32 FIFO_EP2;
	UINT32 FIFO_EP3;
	UINT32 FIFO_EP4;
};
#pragma pack()

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern volatile struct USB_REGS UsbRegs;


/**************************************************************************
* Global Functon Declaration
***************************************************************************/


#endif /*SY03_USB_H_*/

