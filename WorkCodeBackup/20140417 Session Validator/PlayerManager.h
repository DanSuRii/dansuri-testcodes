#pragma once

#include "IDContainer/IdContainer.h"
#include "Player.h"

namespace SvrMaster
{
	typedef IdContainer::CUniqueKeyGen<LKEY, 0x1111ffff>		TPlayerKey;
	typedef IdContainer::CIdContainer<TPlayerKey, CPlayer*>		VecPlayerID;

	class CPlayerManager : protected VecPlayerID
	{
		typedef std::map<UID, CPlayer*>							MapUser_UID;
		typedef std::map<std::string,	CPlayer*>				MapUser_ACCOUNT;
		typedef std::map<DWORD64,		CPlayer*>				MapUser_SESSION;
	public:
		CPlayerManager(void);
		~CPlayerManager(void);

		CPlayer*	GetPlayerByLKEY(LKEY key);
		CPlayer*	GetPlayerByUID(UID uid);
		CPlayer*	GetPlayerByAccount(char* pstrAccount);

		CPlayer*	LogIn(UID uid, char* szAccount, LKEY keySvr);
		void		PlayerCheckIn( CPlayer& pPlayer );
		bool		ReserveMoveSession(CPlayer&);

		struct LOGOUT_INFORMATION
		{
			bool bRemovedResources;
			LKEY keyLastAccessedServer;
			
			LOGOUT_INFORMATION():bRemovedResources(false),keyLastAccessedServer(LKEY_INVALID){}
		};
		bool		LogOut(UID uid, CVSock* pSock, LOGOUT_INFORMATION* pInfo= nullptr);

		void		LogOut( CPlayer& );
		void		LogOutForce(UID uid);
		void		ExpireSession( DWORD64 dw64Session );

		int			GetPlayerCount()						{ return m_nPlayerCount; }

	private:
		int					m_nPlayerCount;
		MapUser_UID			m_mapUser_UID;
		MapUser_ACCOUNT		m_mapUser_Account;
		MapUser_SESSION		m_mapUser_Session;

		void		AddPlayer(CPlayer* pPlayer);
		void		DelPlayer(CPlayer* pPlayer);
	};

}

typedef GLib::CGSingletonLongevity<SvrMaster::CPlayerManager, eLongevity_Singlton_Contents, CPolicySingleThreaded> PlayerManager;


