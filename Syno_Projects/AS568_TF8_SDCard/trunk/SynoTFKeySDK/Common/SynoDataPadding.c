/***************************************************************************
 *	File name:SynoDataPadding.c
 *	Introduce:The implement file for Syno data padding functions
 *	Author:Syno common
 *	Date:2016/03/13
 *  Version:v1.0
 **************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "SynoDataPadding.h"


int PKCS7_AddPadding(unsigned char* lpPadded, unsigned long ulPaddedLen)
{
	unsigned char i = 0;
	unsigned char btPad = 0;

	if (!lpPadded || ulPaddedLen > 0x10)
	{
		return -1;
	}

	btPad = (unsigned char)ulPaddedLen;
	for (i = 0; i < btPad; i++)
	{
		lpPadded[i] = btPad;
	}

	return 0;
}

int PKCS7_RemovePadding(unsigned char* lpData, unsigned long *pulDataLen)
{
	unsigned char n = 0;
	unsigned char btPad = 0;
	int bPadded = 1;

	if (!lpData || *pulDataLen % 16 != 0)
	{
		return -1;
	}

	btPad = lpData[*pulDataLen - 1];
	if (btPad > 0x10)
	{
		return 0;
	}

	for (n = 0; n < btPad; n++)
	{
		if (lpData[*pulDataLen - n - 1] != btPad)
		{
			bPadded = 0;
			break;
		}
	}

	if (1 == bPadded)
	{
		for (n = 0; n < btPad; n++)
		{
			lpData[*pulDataLen - n - 1] = 0;
		}
		*pulDataLen -= btPad;
	}

	return 0;
}
