/***************************************************************************
 *	File name:SynoConvert.h
 *	Introduce:Syno common type convert operation functions definition
 *	Author:Syno common
 *	Date:2015/11/10
 *  Version:v1.0
 **************************************************************************/

#ifndef _SYNO_CONVERT_H_
#define _SYNO_CONVERT_H_

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------*
 * ×Ö·û´®×ª»»º¯Êý                                *
 *----------------------------------------------*/
char* ConvertASCII2UTF8(const char* ascii);
char* ConvertUNICODE2UTF8(const wchar_t* unicode);
char* ConvertUTF82ASCII(char* utf8);
wchar_t* ConvertUTF82UNICODE(char* utf8);

#ifdef __cplusplus
}
#endif

#endif /* _SYNO_CONVERT_H_ */


