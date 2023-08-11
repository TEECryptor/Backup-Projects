/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/* Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */ 
#include  "Type.h"
#include  "Sdi.h"  
#include <time.h>  
#include "SD_SystemApp.h" 
/* Definitions of physical drive number for each drive */
#define MMC		0   /* Example: Map MMC/SD card to physical drive 0 */
#define USB		1   /* Example: Map USB MSD to physical drive 1 */
#define ATA		2   /* Example: Map ATA harddisk to physical drive 2 */

#define Flash_Sector 512

SDI_ERR SD_ReadSector(UINT32 Sector, UINT8 *Buff, UINT32 Count)
{
	UINT8 SD_Err;
	UINT32 i;
	UINT32 S_Count = Count, R_Sector = Sector;

	if((R_Sector>=gSDICapacity)||(R_Sector+S_Count)>=gSDICapacity)
		return SDI_OUT_OF_RANGE;

	if(gSDISysMode == SD_TF_SYSTEM)
	{
		for (i = 0; i < S_Count; i++)
		{
			SD_Err = SDI_ReadBlock(R_Sector, Buff);
			R_Sector++;
			Buff += 512;
		}
	}
	else if(gSDISysMode == SD_SQI_FLASH_SYSTEM)
	{
		SD_Err = SQI_ReadData(Buff, (SQI_FLASH_ADDR_OFF+R_Sector*0x200), Count*0x200);
	}
	else
	{
	}
	return SD_Err;
}
		
SDI_ERR SD_WriteSector(UINT32 Sector, const UINT8 *Buff, UINT32 Count)
{
	UINT8 SD_Err;
	UINT32 i;
	UINT32 S_Count = Count, W_Sector = Sector;

	if((W_Sector>=gSDICapacity)||(W_Sector+S_Count)>=gSDICapacity)
		return SDI_OUT_OF_RANGE;

	if(gSDISysMode == SD_TF_SYSTEM)
	{
		for (i = 0; i < S_Count; i++)
		{
			SD_Err = SDI_WriteBlock(W_Sector, ( UINT8 *)Buff);
			W_Sector++;
			Buff += 512;
		}
	}
	else if(gSDISysMode == SD_SQI_FLASH_SYSTEM)
	{
		SD_Err = SQI_WriteData(( UINT8 *)Buff, (SQI_FLASH_ADDR_OFF+W_Sector*0x200), Count*0x200);
	}
	else
	{
	}
	return SD_Err;
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{   
	return 0;
#if 0
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case ATA :
		result = ATA_disk_status();

		// translate the reslut code here

		return stat;

	case MMC :
		result = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case USB :
		result = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
#endif//0
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	switch (pdrv)
	{
		case MMC :
			stat = MMC_disk_initialize();
            if (stat == SDI_HS_UNSUPPORT)
            {
               stat = SDI_OK;
            } 
			if (stat != SDI_OK)
				printf("\r\nMMC_disk_initialize_res=%d\r\n",stat);
			stat = SDI_OK;
			return stat;
			
	}
	return STA_NOINIT;

#if 0
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case ATA :
		result = ATA_disk_initialize();

		// translate the reslut code here

		return stat;

	case MMC :
		result = MMC_disk_initialize();

		// translate the reslut code here

		return stat;

	case USB :
		result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
#endif //0
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	
	switch (pdrv) {
		case MMC :
			res = SD_ReadSector(sector, buff, count);//0;
			if (res != SDI_OK)
				printf("\r\nSD_Read_err_res=%d\r\n",res);
			return res;			
	}
	return RES_PARERR;

#if 0
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		result = ATA_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case MMC :
		// translate the arguments here

		result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
#endif  //0
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	switch (pdrv) {		
		case MMC :
			res =SD_WriteSector(sector, buff, count);//0;//
			//print("disk_write res = %d\r\n",res);
			if(res!=SDI_OK)
				printf("\r\nSD_Write_err_res=%d\r\n,res");
			return res;
	}
	return RES_PARERR;

#if 0
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		result = ATA_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case MMC :
		// translate the arguments here

		result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

		result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
#endif  //0
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{        
	WORD gf = 0;
	gf = 512;
	buff = &gf;
	return RES_OK;

#if 0
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :

		// Process of the command for the ATA drive

		return res;

	case MMC :

		// Process of the command for the MMC/SD card

		return res;

	case USB :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
#endif  //0
}
#endif 

DWORD get_fattime (void)
{
	struct tm t;
	DWORD date;
	//    t.tm_sec = gRtcTime.cSec;
	//    t.tm_min = gRtcTime.cMin;
	//	t.tm_hour = gRtcTime.cHour;
	//	t.tm_mday = gRtcTime.cDate;
	//	t.tm_mon = gRtcTime.cMonth;
	//    t.tm_year=gRtcTime.wYear+20;//0602
	
	t.tm_sec = 0;
	t.tm_min = 46;
	t.tm_hour = 14;
	t.tm_mday = 21;
	t.tm_mon = 2;
	t.tm_year = 2015;
	date = 0;
	date = (t.tm_year << 25)|(t.tm_mon<<21)|(t.tm_mday<<16)|\
	(t.tm_hour<<11)|(t.tm_min<<5)|(t.tm_sec);
	
	return date;
}
