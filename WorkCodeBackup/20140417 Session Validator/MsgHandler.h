#pragma once
//#include "Msg/SvrMsg.h"

class CMsgSvr : public CVMsgHeader
{
public:
	BOOL		Connected(CVSock* pFrom, char* pExtra, WORD wSizeExtra);
	BOOL		Disconnected(CVSock* pFrom, char* pExtra, WORD wSizeExtra);
};