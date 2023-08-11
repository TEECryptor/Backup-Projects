/***************************************************************************
 *	File name:SynoConvert.c
 *	Introduce:The implement file for Syno type convert functions
 *	Author:Syno common
 *	Date:2015/11/10
 *  Version:v1.0
 **************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "SynoConvert.h"

#if defined(WIN32)
#include <string.h> 
#include <windows.h>
#elif defined(ANDROID)
#endif//


#if defined(WIN32)
/* 将ASCII字符串转化为UTF-8格式字符串 */
char* ConvertASCII2UTF8(const char* ascii)
{
	int len = 0;
	char* utf8 = NULL;
	wchar_t* unicode = NULL;

	if (!ascii || strlen(ascii) == 0)
		return "";

	len = MultiByteToWideChar(CP_ACP, 0, ascii, -1, NULL, 0);
	unicode = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	memset(unicode, 0, (len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, ascii, -1, unicode, len);

	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	utf8 = (char*)malloc(len + 1);
	memset(utf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, utf8, len, NULL, NULL);
	if (unicode) free(unicode);
	
	return utf8;
}

/* 将UNICODE字符串转化为UTF-8格式字符串 */
char* ConvertUNICODE2UTF8(const wchar_t* unicode)
{
	int len = 0;
	char* utf8 = NULL;

	if (!unicode || wcslen(unicode) == 0)
		return "";
	
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	utf8 = (char*)malloc((len + 1)*sizeof(char));
	memset(utf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, utf8, len, NULL, NULL);
		
	return utf8;
}

/* 将UTF-8格式字符串转化为ASCII字符串 */
char* ConvertUTF82ASCII(char* utf8)
{
	int len = 0;
	char* ascii = NULL;
	wchar_t* unicode = NULL;

	if (!utf8 || strlen(utf8) == 0)
		return "";
	
	len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	unicode = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	memset(unicode, 0, (len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, unicode, len);

	len = WideCharToMultiByte(CP_ACP, 0, unicode, -1, NULL, 0, NULL, NULL);
	ascii = (char*)malloc(len + 1);
	memset(ascii, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, unicode, -1, ascii, len, NULL, NULL);
	if(unicode) free(unicode);

	return ascii;
}

/* 将UTF-8格式字符串转化为UNICODE字符串 */
wchar_t* ConvertUTF82UNICODE(char* utf8)
{
	int len = 0;
	wchar_t* unicode = NULL;

	if (!utf8 || strlen(utf8) == 0)
		return L"";
	
	len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	unicode = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	memset(unicode, 0, (len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, unicode, len);

	return unicode;
}
#elif defined(ANDROID)


#endif//
