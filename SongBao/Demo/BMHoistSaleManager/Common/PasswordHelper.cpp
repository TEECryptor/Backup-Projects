//----------------------------------------------------
//	File Name: PasswordHelper.cpp
//	Introduce: Class CPasswordHelper implement
//----------------------------------------------------
#include "StdAfx.h"
#include "PasswordHelper.h"
//----------------------------------------------------




//----------------------------------------------------
CPasswordHelper::CPasswordHelper(void)
{
}
//----------------------------------------------------
CPasswordHelper::~CPasswordHelper(void)
{
}
//----------------------------------------------------
LPCWSTR CPasswordHelper::EncodePasswordString(LPCWSTR lpwzString)
{
	if (NULL == lpwzString)
		return L"";

	int iCharCnt = wcslen(lpwzString);
	if (iCharCnt <= 0)
		return L"";

	static WCHAR wcResult[256] = {0};
	memset(wcResult, 0, sizeof(WCHAR) * 256);
	for (int i = 0; i < iCharCnt && i < 256; i++)
	{
		if (i % 3 == 0)
			wcResult[i] = lpwzString[i] + 2;
		else
			wcResult[i] = lpwzString[i] + 3;
	}

	return wcResult;
}
//----------------------------------------------------
LPCWSTR CPasswordHelper::DecodePasswordString(LPCWSTR lpwzString)
{
	if (NULL == lpwzString)
		return L"";

	int iCharCnt = wcslen(lpwzString);
	if (iCharCnt <= 0)
		return L"";

	static WCHAR wcResult[256] = {0};
	memset(wcResult, 0, sizeof(WCHAR) * 256);
	for (int i = 0; i < iCharCnt && i < 256; i++)
	{
		if (i % 3 == 0)
			wcResult[i] = lpwzString[i] - 2;
		else
			wcResult[i] = lpwzString[i] - 3;
	}

	return wcResult;
}
//----------------------------------------------------
