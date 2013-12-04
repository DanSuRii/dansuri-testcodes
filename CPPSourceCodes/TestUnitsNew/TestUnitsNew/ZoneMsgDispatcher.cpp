#include "Work.h"

#include "boost/bind.hpp"

#include <windows.h>
#include <vector>
#include "ZoneMsgDispatcher.h"



bool Unhandled(GameWorld::CVtZoneServer*, CVSock*, char*, DWORD)
{
	return false;
}

CZoneMsgDispatcher::CZoneMsgDispatcher( Vector::CVtZoneBase* pOwner )
	: m_pOwner(pOwner)
	, m_fnUnhandled( boost::bind(Unhandled, nullptr, nullptr, nullptr, 0 ))
{

}

