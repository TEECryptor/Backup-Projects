#ifndef CMD_HANDLER_H
#define CMD_HANDLER_H

#include "net_command.h"
#include "db_utils.h"

#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_time.h"

struct Test_Info
{
	Test_Info(int ai, int ti)
	: ability_id(ai), task_id(ti)
	{

	}

	int ability_id;
	int task_id;
};


class Outgoing_Msg : public Net_Message
{
public:
	enum CMD_Status
	{
		CMD_Status_SUCCESS = 0,
		CMD_Status_Failed = -1
	};

	enum FAILED_REASON	{
		FR_NO_ERROR = 0,		/* �޴��� */
		FR_NOMEM = 1,			/* �ڴ治�� */
		FR_NOENT,				/* �ļ�,Ŀ¼������ */
		FR_DB_ERROR,				/* ���ݿ���� */
		FR_FILE_ERROR,				/* �ļ����� */
		FR_NO_USER,				/* �û������� */
		
		FR_NO_PENDING_TEST,		/* û��δ��ɵĲ��� */
		FR_NO_PENDING_TRAINING,		/* û��δ��ɵ�ѵ������ */
	};

	virtual int build_payload()	{

		return 0;
	}

	Outgoing_Msg& operator<<(ACE_CDR::ULong ul)	{
		payload_ << ul;
		return *this;
	}
	Outgoing_Msg& operator<<(ACE_CDR::Long l)	{
		payload_ << l;
		return *this;
	};
	Outgoing_Msg& operator<<(const string& str)	{
		payload_ << str.c_str();
		return *this;
	};
#if 0
	Outgoing_Msg& operator<<(Task_Info* task)	{
		payload_ << task->title_.c_str();
		payload_ << task->description_.c_str();
		payload_ << task->voice_url_.c_str();
		payload_ << task->ability_id_;
		payload_ << task->url_.c_str();
		return *this;
	}
#endif
	Outgoing_Msg& operator<<(const Task_Info& task)	{
		payload_ << task.task_id_;
		payload_ << task.ability_id_;
		payload_ << task.title_.c_str();
		payload_ << task.description_.c_str();
		payload_ << task.voice_url_.c_str();
		payload_ << task.url_.c_str();
		return *this;
	}

};

class CMD_Handler
{
public:
	CMD_Handler(const ACE_SOCK_Stream& peer, const char* auth_id)
		: peer_(peer),
	auth_id_(auth_id)
	{}

	virtual int handle()	{
		return 0;
	}

protected:
	User_Info* do_auth();

protected:
	string auth_id_;
	const ACE_SOCK_Stream peer_;
	
	Outgoing_Msg out_result_;				/* ���ظ��ͻ��˵Ľ�� */
	Outgoing_Msg::FAILED_REASON fr_;
};

class Login_Handler : public CMD_Handler
{
public:
	Login_Handler(const ACE_SOCK_Stream& peer, User_Role role, const char* user_name, const char* password)
		: CMD_Handler(peer,	""),
		role_(role),
		user_name_(user_name),
		password_(password)
	{

	};
	
protected:
	int make_auth_id(int user_id); 

protected:
	User_Role role_;	

	const char* user_name_;
	const char* password_;
};

class Kid_Login_Handler : public Login_Handler
{
public:

	//fixme: ���Ӧ�ò���CRAM-MD5������֤
	Kid_Login_Handler(const ACE_SOCK_Stream& peer, const char* user_name, const char* password)
	: Login_Handler(peer, ROLE_KID, user_name, password)
	{

	}

	virtual int handle();
};

//��ʼ�����ƽ������Ĳ�
#define DIFF_AVG_INITIALLEVEL 3

class Logout_Handler : public CMD_Handler
{
public:
	Logout_Handler(const ACE_SOCK_Stream& peer, const char* auth_id, time_t logout_time)
		: CMD_Handler(peer,	auth_id),
		logout_time_(logout_time)
	{

	}

	int do_logout();
private:
	time_t logout_time_;

};

# define CMD_FAIL_RETURN(reason) \
						   do {														\
								out_result_ << (Outgoing_Msg::CMD_Status_Failed);	\
								out_result_ << (reason);							\
								 return -1;											\
						   } while (0)


#endif //TASK_H