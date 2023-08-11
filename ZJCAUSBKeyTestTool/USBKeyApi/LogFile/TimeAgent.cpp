#include "stdafx.h"
#include "TimeAgent.h"
//#include "MemoryBlock.h"

namespace Utils
{
	Time::Time(void)
	{
	}

	Time::~Time(void)
	{
	}

	FILETIME Time::GetLocalTimeAsFileTime()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		FILETIME ft;
		SystemTimeToFileTime(&st, &ft);
		return ft;
	}

	SYSTEMTIME Time::GetTime()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		return st;
	}

	LONG Time::GetSecondsBetweenDate(const SYSTEMTIME& stFrom, const SYSTEMTIME& stTo)
	{
		if (stFrom.wYear == stTo.wYear && stFrom.wMonth == stTo.wMonth) // simple situation
		{
			return (stTo.wSecond - stFrom.wSecond) + (stTo.wMinute - stFrom.wMinute) * 60 + (stTo.wHour - stFrom.wHour) * 3600 + (stTo.wDay - stFrom.wDay) * 86400;
		}
		else
		{
			DWORD dwSecondsFrom = 0, dwSecondsTo = 0;
			dwSecondsFrom = GetSecondsFrom2K(stFrom);
			dwSecondsTo = GetSecondsFrom2K(stTo);
			return dwSecondsTo - dwSecondsFrom;
		}
	}

#define IsLeapYear(year) ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
	LONG Time::GetDaysBetweenDate(const SYSTEMTIME& stFrom, const SYSTEMTIME& stTo)
	{
		int years = stTo.wYear - stFrom.wYear;
		int days = (years) * 365 + (years / 4) + ((IsLeapYear(stFrom.wYear) && years > 0) ? 1 : 0);

		int fromDays = 0;
		int toDays = 0;
		for (int i = 1; i < stFrom.wMonth; i++)
			fromDays += GetDaysOfMonth(i, stFrom.wYear);
		for (int i = 1; i < stTo.wMonth; i++)
			toDays += GetDaysOfMonth(i, stTo.wYear);
		fromDays += stFrom.wDay;
		toDays += stTo.wDay;

		days += (toDays - fromDays);
		return days;
	}

	LONG Time::GetSecondsFrom2K(const SYSTEMTIME &st)
	{
		SYSTEMTIME st2K = {0};
		st2K.wYear = 2000;
		LONG dwDays = GetDaysBetweenDate(st2K, st);
		LONG dwHours = dwDays * 24 + st.wHour;
		LONG dwMinutes = dwHours * 60 + st.wMinute;
		LONG dwSeconds = dwMinutes * 60 + st.wSecond;
		return dwSeconds;
	}

	USHORT Time::GetDaysOfMonth(WORD wMonth, WORD wYear)
	{
		if (wMonth > 12)
			return -1;

		const USHORT iDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		return iDays[wMonth - 1] + ((wMonth == 2 && IsLeapYear(wYear))? 1 : 0);
	}
}
