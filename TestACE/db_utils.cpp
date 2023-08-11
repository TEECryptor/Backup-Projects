
#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_time.h"

#include "db_utils.h"

//otl_datetime& generate_otl_datetime(time_t sec, long usec)
const otl_datetime& generate_otl_datetime(time_t sec)
{
	
	struct tm tm_time;
	ACE_OS::localtime_r (&sec, &tm_time);
	
	const otl_datetime odt(tm_time.tm_year + 1900, 
		tm_time.tm_mon + 1,
		tm_time.tm_mday,
		tm_time.tm_hour,
		tm_time.tm_min,
		tm_time.tm_sec	  
		);
	
	return odt;
}