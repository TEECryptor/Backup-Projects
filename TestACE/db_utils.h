#ifndef DB_UTILS_H
#define DB_UTILS_H

#include "otlv4.h"

#define OTL_ODBC // Compile OTL 4.0/ODBC

// Default "numeric" NULLs to (-1)
#define OTL_DEFAULT_NUMERIC_NULL_TO_VAL (-1)
// Default "datetime" NULLs to otl_datetime(2000,1,1,0,0,0)
#define OTL_DEFAULT_DATETIME_NULL_TO_VAL otl_datetime(2000,1,1,0,0,0)
// Default "string" NULLs to "***NULL***"
#define OTL_DEFAULT_STRING_NULL_TO_VAL "***NULL***"
// Default "char" NULLs to '*'
#define OTL_DEFAULT_CHAR_NULL_TO_VAL '*'

//const otl_datetime& generate_otl_datetime(time_t sec, long usec);
const otl_datetime& generate_otl_datetime(time_t sec);

extern otl_connect db; // connect object

#define SINGLE_QUOTE_MARK	"'"

#endif //DB_UTILS_H