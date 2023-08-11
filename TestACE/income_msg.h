#ifndef NET_INCOME_MESSAGE_H
#define NET_INCOME_MESSAGE_H

#include "ace/SOCK_Stream.h"
#include "ace/SString.h"
#include "ace/CDR_Stream.h"

class Net_Income_Cmd
{
public:
	Net_Income_Cmd(const ACE_SOCK_Stream& peer)
	:	peer_(peer),
		payload_p(NULL),
		payload_cdr_p(NULL)
	{
	}

	virtual ~Net_Income_Cmd()	{
		
		if(payload_p) delete payload_p;
		if(payload_cdr_p) delete payload_cdr_p;
	}

	int get_header();
	
	//virtual int get_payload() = 0;
	int get_cmd_code();
	int decode_cmd_code();

	ACE_InputCDR* payload_cdr_p;

protected:
	const ACE_SOCK_Stream peer_;
	ACE_Message_Block* payload_p;
};

#if 1
class Login_Income_Msg : public Net_Income_Cmd
{
public:
	Login_Income_Msg(const ACE_SOCK_Stream& peer)
		: Net_Income_Cmd(peer)
	{

	}

public:
	virtual int get_payload();
};
#endif

#endif //RESPONSE_H