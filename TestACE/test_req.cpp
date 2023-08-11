#pragma warning(disable: 4786)

#include "test_req.h"

ACE_InputCDR& operator>>(ACE_InputCDR& cdr, Task_Info& task)
{
	cdr >> task.task_id_;
	cdr >> task.ability_id_;
	cdr >> task.title_;
	cdr >> task.description_;
	cdr >> task.voice_url_;
	cdr >> task.url_;

	return cdr;
}

