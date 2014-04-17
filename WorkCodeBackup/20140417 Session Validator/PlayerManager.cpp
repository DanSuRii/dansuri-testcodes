#include "StdAfx.h"
#include "Misc/Prime.h"
#include "PlayerManager.h"

namespace SvrMaster
{

	CPlayerManager::CPlayerManager(void)
		: VecPlayerID(GLib::GetPrimeBelow<size_t>(eMx_GameConnection))
	{
		m_nPlayerCount = 0;
	}

	CPlayerManager::~CPlayerManager(void)
	{
	}


	CPlayer*	CPlayerManager::GetPlayerByLKEY(LKEY key)
	{
		return Get(key);
	}

	CPlayer*	CPlayerManager::GetPlayerByUID(UID uid)
	{
		MapUser_UID::iterator it = m_mapUser_UID.find(uid);

		if (it == m_mapUser_UID.end())
			return NULL;

		return it->second;
	}

	CPlayer*	CPlayerManager::GetPlayerByAccount(char* pstrAccount)
	{
		MapUser_ACCOUNT::iterator it = m_mapUser_Account.find(pstrAccount);

		if (it == m_mapUser_Account.end())
			return NULL;

		return it->second;
	}

	CPlayer*	CPlayerManager::LogIn(UID uid, char* szAccount, LKEY keySvr)
	{
		PROFILER_FUNCTION_SCOPE;

		CPlayer* pPlayer = GetPlayerByUID(uid);
		if (NULL != pPlayer)
		{
			LOG_SYSTEM(E)("[CPlayerManager::LogIn] pPlayer(%d, %s) Duplicated Error.", uid, szAccount);
			return NULL;
		}

		pPlayer = GetPlayerByAccount(szAccount);
		if(NULL != pPlayer)
		{
			LOG_SYSTEM(E)("[CPlayerManager::LogIn] pPlayer(%d, %s) Duplicated Error.", uid, szAccount);
			return NULL;
		}

		pPlayer = new CPlayer;
		pPlayer->SetUID(uid);
		pPlayer->SetAccountName(szAccount);
		pPlayer->SetSvrKey(keySvr);

		// Key 발급.
		AddPlayer(pPlayer);

		++m_nPlayerCount;

		// 자료구조 정보 추가.
		m_mapUser_UID.insert(std::make_pair(uid, pPlayer));
		m_mapUser_Account.insert(std::make_pair(szAccount, pPlayer));

		SetSessionExpired(pPlayer);
		return pPlayer;
	}

	/// 접속이 DB에서 응답이 오기전에 접속이 끊길수 있다.
	bool	CPlayerManager::LogOut(UID uid, CVSock* pSock, LOGOUT_INFORMATION* pInfo)
	{
		CPlayer* pPlayer = GetPlayerByUID(uid);
		if (NULL == pPlayer)
		{
			LOG_SYSTEM(E)("[CPlayerManager::LogOut] Player(%d) Logout Error.", uid);
			return false;
		}
		
		if(pInfo)
			pInfo->keyLastAccessedServer = pPlayer->GetSvrKey();

		/// 현재의 접속이 아닌곳에서 발생한 끊김처리
		if (SvcConfig::Instance().FindServerKey(pSock) != pPlayer->GetSvrKey())
			return true;
		if (pPlayer->GetPlayerAuthKey() != 0)
			return true;

		LogOut(*pPlayer);

		if(pInfo)
			pInfo->bRemovedResources = true;
			
		return true;
	}


// 	void CPlayerManager::LogOutForce( UID uid )
// 	{
// 		CPlayer* pPlayer = GetPlayerByUID(uid);
// 		if (NULL == pPlayer)
// 		{
// 			LOG_SYSTEM(E)("[" __FUNCTION__ "] Player(%d) Logout Error.", uid);
// 			return ;
// 		}
// 
// 		LogOut(*pPlayer);
// 	}


	void CPlayerManager::LogOut( CPlayer& refPlayer )
	{
		if (refPlayer.GetUID() != UID_INVALID)
		{
			MapUser_UID::iterator it = m_mapUser_UID.find(refPlayer.GetUID());
			if (it != m_mapUser_UID.end())
			{
				m_mapUser_UID.erase(it);
			}
		}

		{
			MapUser_ACCOUNT::iterator it = m_mapUser_Account.find(refPlayer.GetAccountName());
			if (it != m_mapUser_Account.end())
			{
				m_mapUser_Account.erase(it);
			}
		}
		SetSessionChecked(&refPlayer);

		// Key 해지.
		DelPlayer(&refPlayer);

		refPlayer.Clear();

		// 카운트 감소.
		--m_nPlayerCount;

		//플레이어는 항상 Heap에 할당되어있다.
		CPlayer* pPlayer = &refPlayer;
		SAFE_DELETE(pPlayer);
	}

	// Private Function.
	void	CPlayerManager::AddPlayer(CPlayer* pPlayer)
	{
		if(NULL == pPlayer)
		{
			LOG_SYSTEM_O(E, "[CPlayerManager::AddPlayer] pPlayer is NULL.");
			return ;
		}

		LKEY keyValue = Set(pPlayer);
		if(c_InvalidKey == keyValue)
		{
			LOG_SYSTEM_O(E, "[CPlayerManager::AddPlayer] keyValue is INVALID.");
			return ;
		}
		pPlayer->SetKey(keyValue);

	}

	void	CPlayerManager::DelPlayer(CPlayer* pPlayer)
	{
		if(LKEY_INVALID == pPlayer->GetKey())
		{
			return ;
		}

		Del(pPlayer->GetKey());
	}

	void CPlayerManager::ProcessExpired(void)
	{
		if(m_lstWaitExpired.empty())
			return ;

		time_t timeCur = ::time(NULL);

		do
		{
			auto pPlayer = m_lstWaitExpired.front();

			if(pPlayer->GetTimeExpiration() > timeCur)
				break;

			m_lstWaitExpired.pop_front();
			LOG_SYSTEM(N)(__FUNCTION__ "() Session remove by expire, AccountID(%s), UID(%d)", pPlayer->GetAccountName(), pPlayer->GetUID());
			LogOut(*pPlayer);

		} while(!m_lstWaitExpired.empty());
	}

	void CPlayerManager::SetSessionChecked( CPlayer* pPlayer )
	{
		auto it = m_lstWaitExpired.begin();

		for ( ; it!=m_lstWaitExpired.end() ; ++it)
		{
			if (pPlayer == *it)
			{
				m_lstWaitExpired.erase(it);
				break;
			}
		}
	}

	bool CPlayerManager::SetSessionExpired( CPlayer* pPlayer )
	{
		pPlayer->SetTimeExpiration();
		m_lstWaitExpired.push_back(pPlayer);
		return true;
	}

}

// 
// CPlayerManager::CPlayerManager(void)
// {
// 	m_iMaxSize = 0;
// }
// 
// CPlayerManager::~CPlayerManager(void)
// {
// 	for_each(m_pPlayerList.begin(), m_pPlayerList.end(), SAFE_DELETE_F<CPlayer*>());
// }
// 
// void CPlayerManager::Init(int iMaxSize)
// {
// 	if ( m_iMaxSize > 0 )
// 		return;
// 
// 	m_iMaxSize = iMaxSize;
// 
// 	for ( int i = 0; i < iMaxSize; i++ )
// 	{
// 		CPlayer* pPlayer = new CPlayer;
// 		pPlayer->m_iListId = i;
// 		m_pPlayerList.push_back(pPlayer);
// 	}
// }
// 
// CPlayer* CPlayerManager::GetPlayer(int iListNum)
// {
// 	if ( iListNum < 0 )
// 		return NULL;
// 
// 	return m_pPlayerList[iListNum];
// }
// 
// CPlayer* CPlayerManager::GetPlayerUID(DWORD dwUID)
// {
// 	if ( dwUID <= 1 )
// 		return NULL;
// 
// 	return m_mapPlayerList[dwUID];
// }
// 
// void CPlayerManager::SetMapPlayer(CPlayer* pPlayer)
// {
// 	if ( pPlayer->m_dwUID <= 1 )
// 		return;
// 
// 	m_mapPlayerList.insert(std::make_pair( pPlayer->m_dwUID, pPlayer ));
// }
// 
// 
// CPlayer* CPlayerManager::FindPlayer(DWORD dwUID)
// {
// 	for ( int i = 0; i < m_pPlayerList.size(); i++ )
// 	{
// 		if ( dwUID > 0 && m_pPlayerList[i]->m_dwUID == dwUID )
// 			return m_pPlayerList[i];
// 	}
// 
// 	return NULL;
// }
// 
// CPlayer* CPlayerManager::SetPlayerUse()
// {
// 	for ( int i = 0; i < m_pPlayerList.size(); i++ )
// 	{
// 		if ( m_pPlayerList[i]->m_dwUID == 0 )
// 		{
// 			m_pPlayerList[i]->m_dwUID = 1;
// 			return m_pPlayerList[i];
// 		}
// 	}
// 
// 	return NULL;
// }
// 
// void CPlayerManager::SetPlayerNoUse(CVSock* pFrom)
// {
// 	for ( int i = 0; i < m_pPlayerList.size(); i++ )
// 	{
// 		if ( m_pPlayerList[i]->m_pSock == pFrom )
// 		{
// 			if ( m_pPlayerList[i]->m_dwUID > 1 )
// 			{
// 				std::map<DWORD, CPlayer*>::iterator	it = m_mapPlayerList.find(m_pPlayerList[i]->m_dwUID);
// 				m_mapPlayerList.erase(it);
// 			}
// 
// 			m_pPlayerList[i]->Clear();
// 			break;
// 		}
// 	}
// }
// 
// void CPlayerManager::SetPlayerNoUse(DWORD dwUID)
// {
// 	for ( int i = 0; i < m_pPlayerList.size(); i++ )
// 	{
// 		if ( m_pPlayerList[i]->m_dwUID == dwUID )
// 		{
// 			if ( m_pPlayerList[i]->m_dwUID > 1 )
// 			{
// 				std::map<DWORD, CPlayer*>::iterator	it = m_mapPlayerList.find(m_pPlayerList[i]->m_dwUID);
// 				m_mapPlayerList.erase(it);
// 			}
// 
// 			m_pPlayerList[i]->Clear();
// 			break;
// 		}
// 	}
// }
// 
// void CPlayerManager::PrintCommandPlayerCount()
// {
// 	DWORD dwNum = 0;
// 	for ( int i = 0; i < m_pPlayerList.size(); i++ )
// 	{
// 		if ( m_pPlayerList[i]->m_dwUID >= 1 )
// 		{
// 			char szAddress[32];
// 			DWORD dwAddressSize = 0;
// 			m_pPlayerList[i]->GetSock()->GetPeerAddress(szAddress, &dwAddressSize);
// 			LOG("IP : " + szAddress);
// 
// 			dwNum++;
// 			break;
// 		}
// 	}
// 
// 	char szCount[32];
// 	itoa(dwNum, szCount, 10);
// 	LOG("COUNT : " + szCount);
// }