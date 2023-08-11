//-------------------------------------------------
//	Name:SocketDefine.h
//	Introduce:The socket common definition file
//-------------------------------------------------
#ifndef	_SOCKETDEFINE_H_
#define	_SOCKETDEFINE_H_
//-------------------------------------------------
#define		MAX_SOCKETDATA_LEN			512
//-------------------------------------------------
#define		SKT_MSG						WM_USER
#define		SKT_MSG_CLIENTCONNECTED		SKT_MSG + 1		//When a new client socket connected, send the message to message window
#define		SKT_MSG_RECEIVEDMSG			SKT_MSG + 2		//When received a message, send the message to message window
//-------------------------------------------------
#endif	_SOCKETDEFINE_H_
//-------------------------------------------------
