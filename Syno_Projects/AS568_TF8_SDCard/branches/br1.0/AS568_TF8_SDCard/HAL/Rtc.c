/**********************************************************************************
* File name    :	RTC.c
* Function     :	The base function for Real - time clock
* Author       : 	hong
* Date         :	2012/01/13
* Version      :    v1.0
* Description  :	
* ModifyRecord :
**********************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "SY03.h"

/**************************************************************************
* Local Macro Definition
***************************************************************************/


/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static UINT8  IsLeapYear(UINT16 nyear);
static UINT8 tm_DataWeek(UINT16 nyear,UINT16 nmonth,UINT16 nday);

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
const UINT8 gnDays[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/**************************************************************************
* Local Functon Declaration
***************************************************************************/


/**************************************************************************
* Functon
***************************************************************************/

/****************************************************************************
 * Subroutine:	RTC_Init
 * Function:	RTC initialization
 * Input:		UINT32 nInitVal -- the initialization value for counter
 *				RTC_INT nIntType --  the interrupt type, SEC_INT, HSEC_INT, or MIN_INT				
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
void RTC_Init(UINT32 nInitVal, RTC_INT nIntType)
{
	SCM_ReleaseModRst(ID_RTC);	
	SCM_EnableModClk(ID_RTC);
	
	RTC_WrInitVal(nInitVal);
	RTC_SetIntType(nIntType);
	RtcRegs.CTRL.bit.En = 1;//计数器使能信号	
}
/****************************************************************************
 * Subroutine:	RTC_SetIntType
 * Function:	to set RTC interrupt type
 * Input:		RTC_INT nIntType -- the interrupt type, SEC_INT, HSEC_INT, or MIN_INT 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
void RTC_SetIntType(RTC_INT nIntType)
{
	RtcRegs.CTRL.all = RtcRegs.CTRL.all & 
						(~((UINT32)(SEC_INT | HSEC_INT | MIN_INT)<<8)) |	
						((UINT32)(nIntType)<<8);

	if(	(nIntType & HSEC_INT)
	|| (nIntType & SEC_INT)
	||	(nIntType & MIN_INT) )
	{
		RtcRegs.CTRL.bit.IntEn = 1;
		NVIC_EnableIRQ(RTC_IRQn);
	}
	else 
	{
		RtcRegs.CTRL.bit.IntEn = 0;
		NVIC_DisableIRQ(RTC_IRQn);
	}
}


/****************************************************************************
 * Subroutine:	IsLeapYear
 * Function:	Leap to judge
 * Input:		UINT16 nyear				
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
static UINT8  IsLeapYear(UINT16 nyear)  
{ 
    if(((nyear%4==0)&&(nyear%100!=0))||(nyear%400==0)) 
    {
        return TRUE; 
    }
    return FALSE; 
} 
/****************************************************************************
 * Subroutine:	tm_DataWeek
 * Function:	Known date calculation weeks
 * Input:		UINT16 nyear
 *				UINT16 nmonth
 *				UINT16 nday
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
static UINT8 tm_DataWeek(UINT16 nyear,UINT16 nmonth,UINT16 nday)
{
	if (nmonth==1||nmonth==2)
	{
		nyear -=1;		
		nmonth +=12;	
	}
	return (nday+1+2*nmonth+3*(nmonth+1)/5+nyear+(nyear/4)-nyear/100+nyear/400)%7;
}

/****************************************************************************
 * Subroutine:	RTC_DataToCV
 * Function:	to translate date into the counter value
 * Input:		DATA_FORM sDataForm
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
UINT32 RTC_DataToCV(DATA_FORM sDataForm)
{
    UINT32 nSum;
    UINT16 ni;
    
    nSum=0;
    for(ni=2000;ni<sDataForm.year;ni++)
    {
        nSum+=365;
        if(IsLeapYear(ni))
        {
            nSum+=1;
        }
    }
    
    for(ni=1;ni<sDataForm.month;ni++)
    {
        nSum+=gnDays[ni-1];
    }

    if(sDataForm.month>2)
    {
        if(IsLeapYear(sDataForm.year))
        {
            nSum+=1;
        }
    }

    nSum += sDataForm.day - 1;
    nSum = nSum*24*60;
    nSum += sDataForm.hour*60 + sDataForm.minute;
	nSum <<= 6;
	nSum += sDataForm.second;
    
    return nSum; 
} 
/****************************************************************************
 * Subroutine:	RTC_CVToData
 * Function:	Counter value into a date
 * Input:		UINT32 nCCVR -- the current counter value
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
DATA_FORM RTC_CVToData(UINT32 nCCVR)
{
    DATA_FORM sDataForm;
    UINT16  nday,nsum;
	UINT32	nMinute;
	UINT8	nSecond;


	nSecond = (nCCVR & 0x3F);
	nMinute = (nCCVR >> 6);
    
    sDataForm.year=2000;
    sDataForm.month=1;
    sDataForm.day=1;
    sDataForm.hour=0;
    sDataForm.minute=0;
    sDataForm.second=0;    
    nday = nMinute/1440;	
	nMinute -= nday*1440;
    sDataForm.hour = nMinute/60;
    nMinute -= sDataForm.hour*60;
	sDataForm.minute = nMinute;
    sDataForm.second = nSecond;
    
    while(1)
    {
        nsum=365; 
        if( IsLeapYear(sDataForm.year)) 
        {            
            nsum+=1; 
        }
        if(nday<nsum)
        {         
            break;
        }
        sDataForm.year++;
        nday -=nsum;
    }
    
    while(1)
    {
        nsum=gnDays[sDataForm.month-1];
        if(sDataForm.month==2)
        {
            if(IsLeapYear(sDataForm.year)) 
            {     
                nsum+=1; 
            }
        }
        if(nday<nsum)
        {            
            sDataForm.day+=nday;
            break;
        }
        sDataForm.month++;
        nday-=nsum;
    }
    sDataForm.week = tm_DataWeek(sDataForm.year,sDataForm.month,sDataForm.day);

    return sDataForm;
}
