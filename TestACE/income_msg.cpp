
#include "stdafx.h"
#include "ace/Log_Msg.h"

#include "income_msg.h"

int Net_Income_Cmd::get_header()
{
	ACE_Message_Block *header_p = 0;
	ACE_NEW_RETURN (header_p,
		ACE_Message_Block (ACE_DEFAULT_CDR_BUFSIZE),
		-1);
	
	auto_ptr <ACE_Message_Block> header (header_p);
	
	// Align the Message Block for a CDR stream
	ACE_CDR::mb_align (header.get());
	
	ACE_CDR::Boolean byte_order;
	ACE_CDR::ULong length;
	
	ssize_t count = peer_.recv_n (header->wr_ptr(),
		8);
	
	switch (count)
	{
		// Handle shutdown and error cases.
	default:
	case -1:
	case 0:
		ACE_DEBUG ((LM_DEBUG,
			ACE_TEXT ("client closing down\n")));
		
		return -1;		
	case 8:
		// Just fall through in this case..
		break;
	}
	// Reflect addition of 8 bytes for the header.
	header->wr_ptr(8); 
	
	// Create a CDR stream to parse the 8-byte header.
	ACE_InputCDR header_cdr (header.get ());
	
	// Extract the byte-order and use helper methods to disambiguate
	// octet, booleans, and chars.
	header_cdr >> ACE_InputCDR::to_boolean (byte_order);
	
	// Set the byte-order on the stream...
	header_cdr.reset_byte_order (byte_order);
	
	// Extract the length
	header_cdr >> length;
	
	ACE_NEW_RETURN (payload_p,
		ACE_Message_Block (length),
		-1);
//	auto_ptr <ACE_Message_Block> payload (payload_p);
	
	// Ensure there's sufficient room for log record payload.
//	ACE_CDR::grow (payload.get (), 8 + ACE_CDR::MAX_ALIGNMENT + length);
	ACE_CDR::grow (payload_p, 8 + ACE_CDR::MAX_ALIGNMENT + length);

	count = peer_.recv_n (payload_p->wr_ptr(),
							length);
	if(count != length)	{
		ACE_DEBUG ((LM_DEBUG,
			ACE_TEXT ("client closing down\n")));
		
		return -1;		
	}

	// Reflect additional bytes for the message.
	payload_p->wr_ptr (length);   
	
//	ACE_InputCDR payload_cdr (payload_p);
	payload_cdr_p = new ACE_InputCDR(payload_p);
//	payload_cdr.reset_byte_order (byte_order);
	payload_cdr_p->reset_byte_order (byte_order);

	return 0;
}

#if 0
//int Net_Income_Cmd::recieve()
int Net_Income_Cmd::get_cmd_code()
{
	get_header();
	return(decode_cmd_code());
}
#endif

//int Net_Income_Cmd::decode_cmd_code()
int Net_Income_Cmd::get_cmd_code()
{
	ACE_CDR::ULong cmd_code;
	*payload_cdr_p >> cmd_code;
//	payload_cdr_ >> cmd_code;

	return cmd_code;
}

