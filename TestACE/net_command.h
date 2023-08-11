#ifndef NET_COMMAND_H
#define NET_COMMAND_H

#include <string>
//#include <hash_map>
#include <map>

#include "ace/OS_NS_sys_time.h"
#include "ace/Date_Time.h"

#include "ace/SOCK_Stream.h"
#include "ace/SString.h"
#include "ace/CDR_Stream.h"

//#include "user_login.h"

using namespace std;

enum 
{
	CMD_MAX_PARAM_LEN = 4096,
	MAX_PAYLOAD_SIZE = 	4 // command code
						+ 4 //role
						+ CMD_MAX_PARAM_LEN // data
						+ ACE_CDR::MAX_ALIGNMENT // padding

};

enum Command_Code
{
	CMD_LOGIN = 1000,
	CMD_GET_PENDING_TEST,
	CMD_GET_PENDING_TRAINING,
	CMD_GET_TEST_INFO,
	CMD_START_TEST,
	CMD_UPLOAD_TEST_DETAIL,
	CMD_FINISH_TEST,
	CMD_GET_TRAINING_CYCLE,
	CMD_START_TRAINING_TASK,
	CMD_FINISH_TRAINING_TASK,
	CMD_LOGOUT
};

enum User_Role
{
	ROLE_PARENT = 2000,
		ROLE_KID = 2001
};

struct User_Info
{
//	ACE_UINT32 user_id;
	int user_id;
	User_Role role;
	time_t	login_time;
};

#if 0
struct Task_Info
{
	string	title_;
	string	description_;
	string	voice_url_;
	unsigned int ability_id_;
	string	url_;		
};
#endif

struct Task_Info
{	
	ACE_INT32 task_id_;
	ACE_INT32 ability_id_;
	ACE_CString	title_;
	ACE_CString	description_;
	ACE_CString	voice_url_;
	ACE_CString	url_;		
};


#if 0
struct Eq_ACE_CString
{
	bool operator()(const ACE_CString& str1, const ACE_CString& str2) const
	{
		return (str1 == str2);
	}

};

typedef hash_map<ACE_CString, User_Info*, hash<>, Eq_ACE_CString> Authed_User; 
#endif

//fixme: map::find可能会很慢, 今后需要使用hash_map作为映射容器
//typedef hash_map<string, User_Info*> Authed_User; 
typedef map<string, User_Info*> Authed_User; 

extern Authed_User authed_users;

//class Net_Command
class Net_Message
{
public:
	Net_Message()
	: payload_(MAX_PAYLOAD_SIZE),
      header_(ACE_CDR::MAX_ALIGNMENT + 8),
	  payload_len_(0)
	{

	}

#if 1
//	Net_Message(const ACE_CString& auth_id)
	Net_Message(const string& auth_id)
	: auth_id_(auth_id),
	  payload_(MAX_PAYLOAD_SIZE),
      header_(ACE_CDR::MAX_ALIGNMENT + 8),
	  payload_len_(0)
	{

	}
#endif

	virtual int send(const ACE_SOCK_Stream& peer, const ACE_Time_Value *timeout = 0);
	virtual int build_payload() = 0;

protected:
	int build()
	{
		build_payload();
		build_header();

		return 0;
	}

	virtual int build_header()
	{
		// Get the number of bytes used by the CDR stream.
		ACE_CDR::ULong length = payload_.total_length ();
		
		header_ << ACE_OutputCDR::from_boolean (ACE_CDR_BYTE_ORDER);
		
		// Store the size of the payload that follows
		header_ << ACE_CDR::ULong (length);

		return 0;
	}
	
protected:
	ACE_OutputCDR header_;

	ACE_CDR::Long cmd_code_;
//	ACE_CString session_id_;
//	ACE_CString auth_id_;
	string auth_id_;

	ACE_OutputCDR payload_;
//    ACE_CDR::Boolean byte_order_;
	ACE_CDR::ULong payload_len_;
};

ACE_OutputCDR& operator<<(ACE_OutputCDR& cdr, const ACE_Time_Value& time)
;
#if 0
{
	cdr << time.sec();
	cdr << time.usec();

	return cdr;
}
#endif


#endif //NET_COMMAND_H