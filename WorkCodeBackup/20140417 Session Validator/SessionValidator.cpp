#include "StdAfx.h"
#include "SessionValidator.h"
#include "PlayerManager.h"

CSessionValidator::SESSION::SESSION( SESSION_KEY sessionKey )
	: m_sessionKey(sessionKey)
	, m_timeExpiration( ::time(nullptr) + eDF_AuthKeyExpireTimePeriod )
{
}

bool CSessionValidator::SESSION::operator==( SESSION& rhs)
{
	return m_sessionKey == rhs.m_sessionKey;
}

bool CSessionValidator::SESSION::operator==( SESSION_KEY& sessionKey )
{
	return m_sessionKey == sessionKey;
}

SESSION_KEY CSessionValidator::GenerateSessionKey()
{
	static volatile LONGLONG generator = 0;
	if(0 == InterlockedIncrement64(&generator))
		InterlockedIncrement64(&generator);
	return SESSION_KEY(generator);
}

void CSessionValidator::TickProcess( void )
{
	if(listWaitSessions.empty())
		return ;
	time_t timeCur = ::time(NULL);		
	
	do
	{
		auto current = listWaitSessions.front();

		if(current.m_timeExpiration > timeCur)
			break;

		PlayerManager::Instance().ExpireSession( current.m_sessionKey );
		listWaitSessions.pop_front();
	}while(false == listWaitSessions.empty());
}

SESSION_KEY CSessionValidator::PushSession()
{
	SESSION newSession(GenerateSessionKey());
	listWaitSessions.push_back( newSession );

	return newSession.m_sessionKey;
}

void CSessionValidator::EraseSession( SESSION_KEY sessionKey)
{
	//binary_search�� �Ϸ��� ������, GenerateSessionKey�� �׻� ���� ����Ű�� ���� ���� Ű ���� ������ ������� �ʴ´�.

/*
	auto iterSession = std::find( listWaitSessions.begin(), listWaitSessions.end(), sessionKey );
	if(iterSession == listWaitSessions.end())
	{
		return ;
	}

*/
	for( auto iterSession = listWaitSessions.begin() ; iterSession != listWaitSessions.end(); ++iterSession)
	{
		if((*iterSession) == sessionKey)
		{
			listWaitSessions.erase(iterSession);
			break;
		}
	}
}


