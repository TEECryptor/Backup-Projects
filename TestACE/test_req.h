#ifndef TEST_REQ_H
#define TEST_REQ_H

#include "net_command.h"

class Get_Pending_Test_Req : public Net_Message
{
public:
	Get_Pending_Test_Req(const string& auth_id/*, int current_test_ability_id*/)
		: Net_Message(auth_id)//,
//		current_test_ability_id_(current_test_ability_id)
	{

	}

protected:
	virtual int build_payload()
	{

		payload_ << ACE_CDR::ULong(CMD_GET_PENDING_TEST);
		payload_ << auth_id_.c_str();
//		payload_ << current_test_ability_id_;

		return 0;
	}

private:
//	int current_test_ability_id_;
};

ACE_InputCDR& operator>>(ACE_InputCDR& cdr, Task_Info& task);
/*
 *	cmd: 开始单个测试任务
*/

class Get_Test_Info_Req : public Net_Message
{
public:
	Get_Test_Info_Req(const string& auth_id, const ACE_UINT32 factor_id)
		: Net_Message(auth_id),
		factor_id_(factor_id)
	{
	}

protected:
	virtual int build_payload()
	{
		payload_ << ACE_CDR::ULong(CMD_GET_TEST_INFO);
		payload_ << auth_id_.c_str();
		payload_ << factor_id_;		

		return 0;
	}

private:
	ACE_UINT32 factor_id_;
};


/*
 *	cmd: 开始单个测试任务
*/

class Start_Test_Req : public Net_Message
{
public:
	Start_Test_Req(const string& auth_id, const ACE_UINT32 ability_id)
		: Net_Message(auth_id),
		ability_id_(ability_id)
	{

	}

protected:
	virtual int build_payload()
	{
		payload_ << ACE_CDR::ULong(CMD_START_TEST);
		payload_ << auth_id_.c_str();
		payload_ << ability_id_;		

	    ACE_Time_Value now (ACE_OS::gettimeofday ());
		payload_ << now;

		return 0;
	}

private:
	ACE_UINT32 ability_id_;
};

/*
	cmd: 上传单因子测试详细信息
*/

class Upload_Test_Detail_Req : public Net_Message
{
public:
	Upload_Test_Detail_Req(const string& auth_id,		/* 认证ID */
		const ACE_INT32 factor_id,					/* 测试的factor id */
		const ACE_INT32 task_id,					/* 测试时采用的游戏任务ID */
		const ACE_INT32 level,						/* 此一级测试的等级 */
		const time_t start_time,					/* 任务开始时间 */
		const time_t finish_time,					/* 任务结束时间 */
		const int	item_amount,					/* 测试任务中所有小项的总数 */
		const int	right_ones)						/* 测试任务中结果正确的小项总数 */
		: Net_Message(auth_id),
		factor_id_(factor_id),
		task_id_(task_id),
		level_(level),
		start_time_(start_time),
		finish_time_(finish_time),
		item_amount_(item_amount),
		right_ones_(right_ones)
	{

	}

private:
	virtual int build_payload()
	{
		payload_ << ACE_CDR::ULong(CMD_UPLOAD_TEST_DETAIL);
		payload_ << auth_id_.c_str();
		payload_ << factor_id_;		
		payload_ << task_id_;		
		payload_ << level_;		
		payload_ << start_time_;
		payload_ << finish_time_;
		payload_ << item_amount_;
		payload_ << right_ones_;

		return 0;
	}

	ACE_INT32 factor_id_;
	ACE_INT32 task_id_;
	ACE_INT32 level_;
	time_t start_time_;
	time_t finish_time_;
	int	item_amount_;
	int	right_ones_;	
};

/*
	cmd: 结束单个测试任务
*/

class Finish_Test_Req : public Net_Message
{
public:
	Finish_Test_Req(const string& auth_id,		/* 认证ID */
		const ACE_INT32 ability_id,					/* 刚刚完成测试的ability id */
		const ACE_INT32 next_ability_id,			/* 下一次要进行测试的ability id, -1表示已经完成所有测试 */
		const time_t start_time,					/* 任务开始时间 */
		const time_t finish_time,					/* 任务结束时间 */
		const int	item_amount,					/* 测试任务中所有小项的总数 */
		const int	right_ones)						/* 测试任务中结果正确的小项总数 */
		: Net_Message(auth_id),
		ability_id_(ability_id),
		next_ability_id_(next_ability_id),
		start_time_(start_time),
		finish_time_(finish_time),
		item_amount_(item_amount),
		right_ones_(right_ones)
	{

	}

private:
	virtual int build_payload()
	{
		payload_ << ACE_CDR::ULong(CMD_FINISH_TEST);
		payload_ << auth_id_.c_str();
		payload_ << ability_id_;		
		payload_ << next_ability_id_;		
		payload_ << start_time_;
		payload_ << finish_time_;
		payload_ << item_amount_;
		payload_ << right_ones_;

		return 0;
	}

	ACE_INT32 ability_id_;
	ACE_INT32 next_ability_id_;
	time_t start_time_;
	time_t finish_time_;
	int	item_amount_;
	int	right_ones_;	
};


#endif //TEST_REQ_H