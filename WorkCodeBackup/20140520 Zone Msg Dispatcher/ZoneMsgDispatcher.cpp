#include "stdafx.h"
#include "boost/bind.hpp"
#include "ZoneMsgDispatcher.h"
#include "VtZoneServer.h"


bool Unhandled(GameWorld::CVtZoneServer*, CVSock*, char*, DWORD)
{
	return false;
}

CZoneMsgDispatcher::CZoneMsgDispatcher( Vector::CVtZoneBase* pOwner )
	: m_pOwner(pOwner)
	, m_fnUnhandled( boost::bind(Unhandled, nullptr, nullptr, nullptr, 0 ))
{

}

//추후에는 bool GetRecipient( Recipient*&, pZone, pSock ) 형태로 만들어서 제공하자.
GameWorld::CPlayerChar* CZoneMsgDispatcher::GetRecipient( Vector::CVtZoneBase* pZone, CVSock* pSock)
{
	GameWorld::CVtZoneServer* pZoneServer = dynamic_cast<GameWorld::CVtZoneServer*>(pZone);
	if(nullptr == pZoneServer)
	{
		LOG_SYSTEM(E, "Failed to cast zone to zone server");
		return FALSE;
	}

	if(nullptr == pSock)
	{
		LOG_SYSTEM(E, "Socket does not exist");
		return FALSE;
	}

	return pZoneServer->GetPlayerCharBySock(pSock);
}

