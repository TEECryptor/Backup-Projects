#ifndef TRAINING_REQ_H
#define TRAINING_REQ_H

#include "net_command.h"

class Get_Pending_Training_Req : public Net_Message
{
public:
	Get_Pending_Training_Req(const string& auth_id)
		: Net_Message(auth_id)
	{

	}

protected:
	virtual int build_payload()
	{
		payload_ << ACE_CDR::ULong(CMD_GET_PENDING_TRAINING);
		payload_ << auth_id_.c_str();

		return 0;
	}

};

class Get_Training_Cycle_Req : public Net_Message
{
public:
	Get_Training_Cycle_Req(const string& auth_id)
		: Net_Message(auth_id)
	{

	}

protected:
	virtual int build_payload()
	{
		payload_ << ACE_CDR::ULong(CMD_GET_TRAINING_CYCLE);
		payload_ << auth_id_.c_str();

		return 0;
	}

};

class Start_Training_Req : public Net_Message
{
public:
	Start_Training_Req(const string& auth_id, const ACE_INT32 task_id, const ACE_INT32 ability_id)
		: Net_Message(auth_id),
		task_id_(task_id),
		ability_id_(ability_id)
	{

	}

protected:
	virtual int build_payload()
	{
		payload_ << ACE_CDR::ULong(CMD_START_TRAINING_TASK);
		payload_ << auth_id_.c_str();
		payload_ << task_id_;		
		payload_ << ability_id_;		

	    ACE_Time_Value now (ACE_OS::gettimeofday ());
		payload_ << now;

		return 0;
	}

private:
	ACE_UINT32 task_id_;
	ACE_UINT32 ability_id_;
};

class Finish_Training_Req : public Net_Message
{
public:
	Finish_Training_Req(const string& auth_id,		/* 认证ID */
		const ACE_UINT32 task_id,					/* task id */
		const time_t start_time,					/* 任务开始时间 */
		const time_t finish_time,					/* 任务结束时间 */
		const int	item_amount,					/* 测试任务中所有小项的总数 */
		const int	right_ones,						/* 测试任务中结果正确的小项总数 */
		const int	tries							/* 这是其第多少次重试 */
		)
		: Net_Message(auth_id),
		task_id_(task_id),
		start_time_(start_time),
		finish_time_(finish_time),
		item_amount_(item_amount),
		right_ones_(right_ones),
		tries_(tries)
	{

	}

private:
	virtual int build_payload()
	{
		payload_ << ACE_CDR::ULong(CMD_FINISH_TRAINING_TASK);
		payload_ << auth_id_.c_str();
		payload_ << task_id_;		
		payload_ << start_time_;
		payload_ << finish_time_;
		payload_ << item_amount_;
		payload_ << right_ones_;
		payload_ << tries_;

		return 0;
	}

	ACE_UINT32 task_id_;
	time_t start_time_;
	time_t finish_time_;
	int	item_amount_;
	int	right_ones_;	
	int	tries_;
};

#endif //TRAINING_REQ_H