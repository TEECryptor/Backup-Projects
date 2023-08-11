#ifndef _LOG_H
#define _LOG_H 
#include "SYglobal.h"

void dbg_print_buf(const char *msg, ...); 
void dbg_print_log(const char *msg,
    	const char *pccFunc,    
    	const int   ciLine,
    	const char *pccFmt,...); 
#ifdef DBG_LOG_ENABLE
#define DBG_INFO(...) dbg_print_log("INFO", __FUNCTION__, __LINE__, __VA_ARGS__)
#define DBG_BUF(...) dbg_print_buf("BUFF",__VA_ARGS__) 
#define DBG_FUNC_BEGIN DBG_INFO("Begin") 
#define DBG_FUNC_END DBG_INFO("End")
#else                                   
#define DBG_FUNC_BEGIN 
#define DBG_FUNC_END
//#define DBG_INFO(...)  do{}while(0)
//#define DBG_BUF(...)   do{}while(0)
#define DBG_INFO(...)  
#define DBG_BUF(...)   
#endif
#define DBG_ERR(...) dbg_print_log(">ERR", __FUNCTION__, __LINE__, __VA_ARGS__)

#endif
