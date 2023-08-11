/***************************************************************************
 *	File name:SynoError.c
 *	Introduce:The implement file for Syno error functions
 *	Author:Syno common
 *	Date:2015/11/04
 *  Version:v1.0
 **************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "SynoError.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#ifdef ANDROID
#include <android/log.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

/*****************************************************************************
 函 数 名  : GetErrorString
 功能描述  : 获取错误码代表的具体含义
 输入参数  : ulError 错误码
 输出参数  : 无
 返 回 值  : 错误码的具体含义

 修改历史      :
  1.日    期   : 2014年01月16日
    作    者   : 张晓垒
    修改内容   : 新生成函数

*****************************************************************************/
char *GetErrorString(unsigned int ulError)
{
    static char acStrError[512] = {0};

	switch(ulError)
	{
    	case SAR_OK:
    		strcpy(acStrError,"command success!"); break;
    	case SAR_FAIL:
    		strcpy(acStrError,"command failed!");break;
    	case SAR_NOTSUPPORT_SYS:
    		strcpy(acStrError,"unsupport system!");break;
    	case SAR_FILEERR:
    		strcpy(acStrError,"file operation error!");break;
    	case SAR_INVALIDPARAMERR:
    		strcpy(acStrError,"invalide param!");break;
    	case SAR_NAMELENERR:
    		strcpy(acStrError,"name length error!");break;
    	case SAR_OBJERR:
    		strcpy(acStrError,"object error!");break;
    	case SAR_MEMORYERR:
    		strcpy(acStrError,"memroy error");break;
    	case SAR_INDATALENERR:
    		strcpy(acStrError,"input data len error");break;
    	case SAR_INDATAERR:
    		strcpy(acStrError,"input data error");break;
    	case SAR_BUFFER_TOO_SMALL:
    		strcpy(acStrError,"buffer is not enough");break;
    	case SAR_PIN_LEN_RANGE:
    		strcpy(acStrError,"PIN length error!");break;
        case SAR_DEVICEAUTHERR:
            strcpy(acStrError,"device auther error!");break;
        case SAR_INVALIDDEVNAME:
            strcpy(acStrError,"invalide device name error!");break;
        case SAR_INVALIDDEVPATH:
            strcpy(acStrError,"invalide device path error!");break;
    	default:
            sprintf(acStrError, "Unknown ErrNo(0x%x):", ulError);
	}

    return (char *)acStrError;
}