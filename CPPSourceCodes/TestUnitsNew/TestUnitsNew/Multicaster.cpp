#include "Work.h"
#include <map>
#include <boost/foreach.hpp>
#include <algorithm>

typedef unsigned long DWORD;
class CPlayer;

class CMsgStgBase
{
public:
	void SendStgMsg(CPlayer&, char* msgExtra, DWORD dwSizeExtra);
};

class CPlayer
{

};

void CMsgStgBase::SendStgMsg( CPlayer& player, char* msgExtra, DWORD dwSizeExtra )
{
	
}

class CPlayerManager
{
public:
	CPlayerManager()
	{
		mapContainer[0] = new CPlayer;
	}

	~CPlayerManager()
	{
		auto iter = mapContainer.begin();
		while( iter != mapContainer.end())
		{
			if(nullptr != iter->second)
				delete iter->second;
			auto toRemove = iter++;
			mapContainer.erase(toRemove);
		}	

		auto fn = [this]()->bool{};
		
	}

	template<class TMsg, class BinaryPredicate>
	void MultiCastToUser( TMsg& msg ,BinaryPredicate pred)
	{
		for each( auto iter in mapContainer )
		{
			//static_assert(IS_STGBASE_MSG(TMsg), "Not CMsgStgBase<>");
			CPlayer* player = iter.second;

			if(nullptr == player) continue;
			
			if( true == pred( *player ) )
			{
				char* pExtra = (char*)&msg;
				((CMsgStgBase*)&msg)->SendStgMsg(*player, pExtra, 0);
			}
		}
	}


private:
	std::map<int, CPlayer*> mapContainer;
};



void WorkImpl<Multicaster>::DoWork()
{
	CPlayerManager Instance;
	
	CMsgStgBase msg;
	Instance.MultiCastToUser(msg,
		[](CPlayer& player)
		{
			return true;
		}
	);

	int myKey = 1;
	char myChar[256] = {"mycharset"};
	Instance.MultiCastToUser(msg,
		[&myKey, &myChar](CPlayer& player) -> bool
	{
		if(0 == strcmp(myChar, "myCharSet")) return true;
		if(1 == myKey) return true;

		return false;
	}
	);
}
