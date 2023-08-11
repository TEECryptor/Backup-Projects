#pragma once

namespace Utils
{
	class Time
	{
	public:
		Time(void);
		~Time(void);

	public:
		static FILETIME GetLocalTimeAsFileTime();
		static SYSTEMTIME GetTime();
		LONG GetSecondsBetweenDate(const SYSTEMTIME& stFrom, const SYSTEMTIME& stTo);
		LONG GetDaysBetweenDate(const SYSTEMTIME& stFrom, const SYSTEMTIME& stTo);
		LONG GetSecondsFrom2K(const SYSTEMTIME& st);
		static USHORT GetDaysOfMonth(WORD wMonth, WORD wYear);
	};
}
