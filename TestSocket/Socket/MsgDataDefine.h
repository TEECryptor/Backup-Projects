//--------------------------------------------------
//	MsgDataDefine.h
//--------------------------------------------------

#define	MSG_TEST	1

typedef struct tagSocketMsg
{
	UINT	iMsgType;
	int		iParam1;
	float	fParam2;
	char	szParam3[32];
}SocketMsg, *LPSOCKETMSG;