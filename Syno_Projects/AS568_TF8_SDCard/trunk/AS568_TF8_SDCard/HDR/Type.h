/***************************************************************************
* File name    :	Type.h
* Function     :	Data type definition
* Author       : 	synochip
* Date         :	2009/03/31
* Version      :    v1.0
* Description  :
* ModifyRecord :
***************************************************************************/
#ifndef TYPE_H_
#define TYPE_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
//-------------- General Return Code -------------
#define RT_OK            0x00 //success
#define RT_FAIL          0x01  //fail
#define RT_COMMAND_ERR   0x02  //command error
#define RT_PARAM_ERR     0x03  //param error
#define RT_OVERTIME      0x04  //over time
#define RT_ECC_ERR       0x05  //ecc error
#define RT_WRITE_ERR     0x06  //write flash err
#define RT_READ_ERR      0x07  //read flash err

// define BOOL value
#define FALSE	0
#define TRUE	1
#define false 	0
#define true  	1

#define NULL 	0

/**************************************************************************
* Global Type Definition
***************************************************************************/
//------- BOOL type definition --------------
#ifndef	_BOOL_TYPE_
#define	_BOOL_TYPE_
typedef	unsigned char 	BOOL;
#endif

//------- UINT8 type definition --------------
#ifndef	_UINT8_TYPE_
#define _UINT8_TYPE_
typedef unsigned char  UINT8;
#endif

//------- UINT16 type definition --------------
#ifndef	_UINT16_TYPE_
#define _UINT16_TYPE_
typedef unsigned short 	UINT16;
#endif

//------- UINT32 type definition --------------
#ifndef	_UINT32_TYPE_
#define _UINT32_TYPE_
typedef unsigned long  UINT32;
#endif

//------- UINT64 type definition --------------
#ifndef	_UINT64_TYPE_
#define _UINT64_TYPE_
typedef unsigned long long UINT64;
#endif

//-------- INT8 type definition --------------
#ifndef	_INT8_TYPE_
#define _INT8_TYPE_
typedef signed char  INT8;
#endif

//-------- INT16 type definition --------------
#ifndef	_INT16_TYPE_
#define _INT16_TYPE_
typedef short 	INT16;
#endif

//-------- INT32 type definition --------------
#ifndef	_INT32_TYPE_
#define _INT32_TYPE_
typedef long  INT32;
#endif

//------- INT64 type definition --------------
#ifndef	_INT64_TYPE_
#define _INT64_TYPE_
typedef long long INT64;
#endif

//------- VUINT8 type definition --------------
#ifndef	_VUINT8_TYPE_
#define _VUINT8_TYPE_
typedef volatile unsigned char  VUINT8;
#endif

//------- VUINT16 type definition --------------
#ifndef	_VUINT16_TYPE_
#define _VUINT16_TYPE_
typedef volatile unsigned short 	VUINT16;
#endif

//------- VUINT32 type definition --------------
#ifndef	_VUINT32_TYPE_
#define _VUINT32_TYPE_
typedef volatile unsigned long  VUINT32;
#endif

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/

#endif /*TYPE_H_*/
