#pragma once
#include "VGSHeader.h"
#include <string>

class CVGSString
{
public:
	CVGSString(){m_str = "";}
	CVGSString(std::string str){m_str = str;}
	std::string string()
	{
		return m_str;
	}
	inline bool operator < (const CVGSString &str)
	{
		return m_str.compare(((CVGSString)str).string()) < 0;
	}
	inline bool operator == (const CVGSString &str)
	{
		return m_str.compare(((CVGSString)str).string()) == 0;
	}
	friend bool operator < (const CVGSString &str1, const CVGSString &str2)
	{
		return ((CVGSString)str1).string().compare(((CVGSString)str2).string()) < 0;
	}
private:
	std::string m_str;
};
