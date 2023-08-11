
#include "stdafx.h"
#include "ace/config-lite.h"

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_time.h"

#include "ace/Log_Msg.h"
#include "ace/UUID.h"

#include "net_command.h"

int Net_Message::send(const ACE_SOCK_Stream& peer, const ACE_Time_Value *timeout)
{

	if(build())
		return -1;

	iovec iov[2];
	iov[0].iov_base = header_.begin ()->rd_ptr ();
	iov[0].iov_len  = 8;
	iov[1].iov_base = payload_.begin ()->rd_ptr ();
	iov[1].iov_len  = payload_.total_length();

	if (peer.sendv_n (iov, 2, timeout) == -1)	{
		ACE_ERROR ((LM_ERROR,
			ACE_TEXT ("(%t) %p\n"),
			ACE_TEXT ("sendv_n")));
		return -1;
	}

	return 0;
}

#if 0
int Login_Response::build_payload()
{
	payload_ << ACE_OutputCDR::from_boolean(login_success_);

	//登陆尚未成功, 同志还需努力
	if(!login_success_)
		return -1;

	do 	{
		ACE_Utils::UUID* uuid1 = ACE_Utils::UUID_GENERATOR::instance ()->generateUUID ();
		ACE_Utils::UUID* uuid2 = ACE_Utils::UUID_GENERATOR::instance ()->generateUUID ();
		
		//ACE_CString session_id(uuid1->to_string ()->c_str ());
		
		auth_id_ = uuid1->to_string ()->c_str ();
		auth_id_ += uuid2->to_string ()->c_str ();
		delete uuid1; delete uuid2;
		
		
		//	payload_ << auth_id_;
		payload_ << auth_id_.c_str();
		
		Authed_User::const_iterator iter = authed_users.find(auth_id_);
		
		if(iter == authed_users.end())	{		/*	没有重复的auth_id */
			break;
		}
	} while(1);

	User_Info* ui_p;
	ACE_NEW_RETURN(ui_p, User_Info, -1);
	ui_p->role = role();
	ui_p->user_id = user_id();
	ui_p->login_time = ACE_OS::time();

	authed_users[auth_id_] = ui_p;

	return 0;
}

int Parent_Login_Response::build_payload()
{
	Login_Response::build_payload();

	activies_num_ = acts_.size();
	payload_ << ACE_CDR::ULong(activies_num_);

	Act_List::const_iterator act_iter;
	for(act_iter = acts_.begin(); act_iter != acts_.end(); ++act_iter)	{
		const Activity* p = *act_iter;
		*this << *p;
	}

	return true;
}

int Parent_Login_Response::operator<<(const Activity& act)
{
//	activies_num_++;
	ACE_CString act_title(act.title_.c_str());
	ACE_CString act_desc(act.desc_.c_str());
	ACE_CString act_url(act.url_.c_str());

	payload_ << ACE_CDR::ULong(act.type_);

	payload_ << ACE_CDR::ULong(act.time_.year);
	payload_ << ACE_CDR::ULong(act.time_.month);
	payload_ << ACE_CDR::ULong(act.time_.day);
	payload_ << ACE_CDR::ULong(act.time_.hour);
	payload_ << ACE_CDR::ULong(act.time_.minute);
	payload_ << ACE_CDR::ULong(act.time_.second);
	payload_ << ACE_CDR::ULong(act.time_.fraction);
	payload_ << ACE_CDR::ULong(act.time_.frac_precision);

	payload_ << act_title;
	payload_ << act_desc;
	payload_ << act_url;

	return 0;
}
#endif

#if 0
int Login_Response::operator<<(const Act_List& act_list)
{
	activies_num_ = act_list.size();

	Act_List::const_iterator act_iter;
	for(act_iter = act_list.begin(); act_iter != act_list.end(); ++act_iter)	{
		const Activity* p = *act_iter;
		*this << *p;
	}

	return 0;
}
#endif

#if 0
int Kid_Login_Response::build_payload()
{
	Login_Response::build_payload();

	
#if 0
	activies_num_ = acts_.size();
	payload_ << ACE_CDR::ULong(activies_num_);

	Task_List::const_iterator task_iter = other_recommended_tasks_.begin();
	for(; task_iter !=  = other_recommended_tasks_.end(); ++task_iter)	{
		const Task_Info* p = *task_iter;
		*this << *p;
	}
#endif

	int	incomplete_task_num = 0;
	if(incomplete_task_)
		incomplete_task_num = 1;
	payload_ << ACE_CDR::ULong(incomplete_task_num);
	if(incomplete_task_num)	{
		*this << incomplete_task_;
	}

	int	recommended_task_num = 0;
	if(recommended_task_)
		recommended_task_num = 1;
	payload_ << ACE_CDR::ULong(recommended_task_num);
	if(recommended_task_num)	{
		*this << recommended_task_;
	}

	return 0;
}

int Kid_Login_Response::operator<<(const Task_Info* task)
{
	payload_ << task->title_.c_str();
	payload_ << task->description_.c_str();
	payload_ << task->voice_url_.c_str();
	payload_ << task->ability_id_;
	payload_ << task->url_.c_str();

	return 0;
}
#endif

ACE_OutputCDR& operator<<(ACE_OutputCDR& cdr, const ACE_Time_Value& time)
{
	cdr << time.sec();
	cdr << time.usec();

	return cdr;
}
