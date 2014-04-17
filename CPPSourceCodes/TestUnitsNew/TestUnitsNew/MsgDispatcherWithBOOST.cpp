#include "stdafx.h"
#include "Work.h"
#include <windows.h>
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include <vector>

#include "ZoneMsgDispatcher.h"


typedef ULONGLONG GID;

namespace GameWorld
{
	class CVtZoneServer;
}

enum eMSG
{
	eDMH_Invalid,
	eSMH_Base,
	eUMH_Char_Party_Invite_Send_Req
};
class CVMsgHeader
{
	WORD				m_wMH;
};
template<WORD wID>
class CMsgBase : public CVMsgHeader
{

};

template<WORD wID>
class CMsgStgBase : public CMsgBase<wID>
{

};

class CMsgStg : public CMsgStgBase<eSMH_Base>
{
public:
	bool StgHandler( GameWorld::CVtZoneServer* pZoneServer, CVSock* pFrom, char* pExtra, WORD wSize )
	{
		return true;
	}
};

template<WORD wID>
class CMsg;

template<>
class CMsg<eUMH_Char_Party_Invite_Send_Req> : public CMsgStgBase<eUMH_Char_Party_Invite_Send_Req>
{
	int nMyValue;
};

namespace Vector
{
	class ZoneBase
	{
		GID			m_GID;
	};
}

namespace GameWorld
{
	class CVtZoneServer : public Vector::ZoneBase
	{
		void Register()
		{
			//zoneMsgDispatcher->Register< CMsgStg, &CMsgStg::StgHandler >( eUMH_Char_Party_Invite_Send_Req );
		}

	public:
		CVtZoneServer()			
		{
			//zoneMsgDispatcher = new CZoneMsgDispatcher(this);
			
			
			Register();
		}

		virtual ~CVtZoneServer()
		{
			delete zoneMsgDispatcher;
		}

		CZoneMsgDispatcher* zoneMsgDispatcher;
	private:
	};
}



void WorkImpl<MsgDispatcherWithBOOST>::DoWork()
{
	GameWorld::CVtZoneServer _Instance;
	CMsg<eUMH_Char_Party_Invite_Send_Req> msg;
	_Instance.zoneMsgDispatcher->Dispatch( (CVSock*)0x01, eUMH_Char_Party_Invite_Send_Req, (char*)&msg, sizeof(msg) );
	_Instance.zoneMsgDispatcher->Dispatch( (CVSock*)0x02, eSMH_Base, (char*)&msg, sizeof(msg) );
}
