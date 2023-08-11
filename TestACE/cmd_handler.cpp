
#pragma warning(disable: 4786)

#include "stdafx.h"
#include <sstream>

#include "ace/UUID.h"

#include "mxml/mxml.h"

#include "util.h"

#include "cmd_handler.h"
#include "Test_Progress_Conf.h"

otl_connect db; // connect object

User_Info* CMD_Handler::do_auth()
{
	Authed_User::const_iterator iter = authed_users.find(auth_id_);
	
	if(iter == authed_users.end())		/* 没找到这个用户 */
		return NULL;

	User_Info* ui_p = authed_users[auth_id_];
	if(ui_p->role != ROLE_KID)
		return NULL;
	return ui_p;
}

int Login_Handler::make_auth_id(int user_id)
{
	do 	{
		ACE_Utils::UUID* uuid1 = ACE_Utils::UUID_GENERATOR::instance ()->generateUUID ();
		ACE_Utils::UUID* uuid2 = ACE_Utils::UUID_GENERATOR::instance ()->generateUUID ();
		
		//ACE_CString session_id(uuid1->to_string ()->c_str ());
		
		string session_id = uuid1->to_string ()->c_str ();
		session_id += uuid2->to_string ()->c_str ();
		delete uuid1; delete uuid2;
		
		
		Authed_User::const_iterator iter = authed_users.find(session_id);
		if(iter == authed_users.end())	{		/*	没有重复的auth_id */
			auth_id_ = session_id;
			break;
		}
	} while(1);

	User_Info* ui_p;
	ACE_NEW_RETURN(ui_p, User_Info, -1);
	ui_p->role = role_;
	ui_p->user_id = user_id;
	ui_p->login_time = ACE_OS::time();

	authed_users[auth_id_] = ui_p;

	return 0;
}

int Kid_Login_Handler::handle()
{
	unsigned int user_id;
	unsigned int pending_test = 1;
	int current_test_ability_id;

	otl_connect::otl_initialize(); // initialize ODBC environment
	try {
		db.rlogon("UID=sa;PWD=babytree;DSN=LocalServer"); // connect to ODBC

		ostringstream oss;
		oss << "SELECT kid.kid_id, pending_test.ability_id FROM kid";
		oss << " LEFT JOIN pending_test ON pending_test.kid_id = kid.kid_id";
		oss << " WHERE kid.name="<<SINGLE_QUOTE_MARK<<user_name_<<SINGLE_QUOTE_MARK;

		oss << " AND password=" <<SINGLE_QUOTE_MARK<<password_<<SINGLE_QUOTE_MARK;

		otl_stream i_user(50, // buffer size may be > 1
			oss.str().c_str(),
			db);
		
		if(i_user.eof())	{ 
			db.logoff(); // disconnect from the database
			CMD_FAIL_RETURN(Outgoing_Msg::FR_NO_USER);
		} 
		
		//fixme: 如果在pending_test表中有多个未完成测试怎么办?
		i_user >> user_id >> current_test_ability_id;
		if(i_user.is_null())	{
			pending_test = 0;
			current_test_ability_id = -1;
		}

		if(make_auth_id(user_id) < 0)	{
			db.logoff(); // disconnect from the database
			CMD_FAIL_RETURN(Outgoing_Msg::FR_NOMEM);
		}

		out_result_ << auth_id_ << pending_test << current_test_ability_id;
	}
	catch(otl_exception& p){ // intercept OTL exceptions
		cerr<<p.msg<<endl; // print out error message
		cerr<<p.stm_text<<endl; // print out SQL that caused the error
		cerr<<p.sqlstate<<endl; // print out SQLSTATE message
		cerr<<p.var_info<<endl; // print out the variable that caused the error
	}

	db.logoff(); // disconnect from the database
	out_result_.send(peer_);

	return 0;

}


//typedef vector<const Task_Info* > Task_Vec;
typedef vector<Task_Info* > Task_Vec;

#define PROGRESS_CONF_FILE	"progress1.xml"

int Logout_Handler::do_logout()
{
	User_Info* ui_p = do_auth();
	if(NULL == ui_p)
		CMD_FAIL_RETURN(Outgoing_Msg::FR_NO_USER);

	otl_connect::otl_initialize(); // initialize ODBC environment
	try {
		db.rlogon("UID=sa;PWD=babytree;DSN=LocalServer"); // connect to ODBC

		struct tm tm_login_time, tm_logout_time;
		ACE_OS::localtime_r (&ui_p->login_time, &tm_login_time);
		ACE_OS::localtime_r (&logout_time_, &tm_logout_time);

		otl_datetime dt_login(tm_login_time.tm_year + 1900, 
			tm_login_time.tm_mon + 1,
			tm_login_time.tm_mday,
			tm_login_time.tm_hour,
			tm_login_time.tm_min,
			tm_login_time.tm_sec);

		otl_datetime dt_logout(tm_logout_time.tm_year + 1900, 
			tm_logout_time.tm_mon + 1,
			tm_logout_time.tm_mday,
			tm_logout_time.tm_hour,
			tm_logout_time.tm_min,
			tm_logout_time.tm_sec);

		otl_stream o(9, // buffer size should be == 1 on INSERT
			"insert into session "
			"(kid_id, login_time, logout_time) "
			"values(:f2<int>, :f3<timestamp>, :f4<timestamp>)", 
			// INSERT statement
			db // connect object
			);

		o<<ui_p->user_id<<dt_login<<dt_logout;

	}
	catch(otl_exception& p){ // intercept OTL exceptions
		cerr<<p.msg<<endl; // print out error message
		cerr<<p.stm_text<<endl; // print out SQL that caused the error
		cerr<<p.sqlstate<<endl; // print out SQLSTATE message
		cerr<<p.var_info<<endl; // print out the variable that caused the error

		goto out;
	}

out:
	db.logoff(); // disconnect from the database

	return 0;
}
