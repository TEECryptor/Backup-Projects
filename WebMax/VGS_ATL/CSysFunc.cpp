
#include "stdafx.h"
#include "CSysFunc.h"


std::string _GetPureFileNameStr(const std::string &str)
{
	CHAR pureFileName[1024];
	ZeroMemory(pureFileName, 1024);
	GetPureFileName(pureFileName, (char*)str.c_str());
	return std::string(pureFileName);
}