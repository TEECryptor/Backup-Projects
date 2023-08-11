#ifndef SESSION_H
#define SESSION_H

#include "net_command.h"

class Login_Req : public Net_Message
{
public:
	Login_Req(User_Role role, const string& user_name, const string& password)
	: role_(role),
	  user_name_(user_name.c_str()),
	  password_(password.c_str())
	{

	}

protected:
	virtual int build_payload()
	{
		payload_ << ACE_CDR::Long(CMD_LOGIN);
		payload_ << ACE_CDR::Long(ROLE_PARENT);

		payload_ << user_name_;
		payload_ << password_;
		
		return 0;
	}

private:
	User_Role	role_;
	ACE_CString user_name_;
	ACE_CString password_;
};


class Logout_Req : public Net_Message
{
public:
	Logout_Req(const string& auth_id, const time_t logout_time)
	: Net_Message(auth_id),
	logout_time_(logout_time)
	{

	}

private:
	virtual int build_payload()
	{
		payload_ << ACE_CDR::ULong(CMD_LOGOUT);
		payload_ << auth_id_.c_str();
		payload_ << logout_time_;

		return 0;
	}

	time_t	logout_time_;
};

#endif //SESSION_H