//----------------------------------------------------
//	File Name: PasswordHelper.h
//	Introduce: Class CPasswordHelper definition
//----------------------------------------------------
#pragma once
//----------------------------------------------------



//----------------------------------------------------
class CPasswordHelper
{
public:
	CPasswordHelper(void);
	~CPasswordHelper(void);
	//------------------------------------------------
public:
	static LPCWSTR EncodePasswordString(LPCWSTR lpwzString);
	static LPCWSTR DecodePasswordString(LPCWSTR lpwzString);
	//------------------------------------------------
};
//----------------------------------------------------
