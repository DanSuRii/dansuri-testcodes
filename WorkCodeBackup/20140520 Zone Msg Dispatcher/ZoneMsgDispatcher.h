#pragma once
#include <boost/function.hpp>

namespace GameWorld
{
	class CVtZoneServer;
	class CPlayerChar;
}
class CVSock;


class CZoneMsgDispatcher
{
	typedef boost::function<BOOL( CVSock*, char*, DWORD )> FnMsgHandler;

	CZoneMsgDispatcher(void);
public:
	
	CZoneMsgDispatcher(Vector::CVtZoneBase*);
	virtual ~CZoneMsgDispatcher(void){}

	BOOL	Dispatch(CVSock* pFrom, WORD wID, char* pMsg, DWORD dwMsgSize)
	{		
		if( wID >= m_vecHandlers.size() )
			return false;

		FnMsgHandler f = m_vecHandlers[wID];
		if(f.empty())
			return false;

		return f(pFrom, pMsg, dwMsgSize);
	}

	__inline BOOL Dispatch(WORD wID, char* pMsg, DWORD dwMsgSize)
	{
		return Dispatch(nullptr, wID, pMsg, dwMsgSize);
	}

	
	template<class TMsg >
	class RegistMsg
	{
	public:
		typedef BOOL (TMsg::*FP_CALL_ORIGINAL)(  CVSock*, char*, DWORD );
		typedef BOOL (TMsg::*FP_CALL_3)( Vector::CVtZoneBase*, char*, DWORD );
		typedef BOOL (TMsg::*FP_CALL_4)( Vector::CVtZoneBase*, CVSock*, char*, DWORD );
	};

	template < class TMsg >
	BOOL Register( typename RegistMsg<TMsg>::FP_CALL_ORIGINAL fpCall, WORD wID)
	{
		struct Local
		{
			static	BOOL	Trampoline(Vector::CVtZoneBase* , typename RegistMsg<TMsg>::FP_CALL_ORIGINAL fpCall, CVSock* pFrom, char* pMsg, DWORD dwMsgSize)
			{
				if (dwMsgSize < sizeof(TMsg))
					return FALSE;

				char*	pExtra = NULL;

				if (dwMsgSize > sizeof(TMsg))
					pExtra = pMsg+sizeof(TMsg);

				return (((TMsg*)pMsg)->*fpCall)(pFrom, pExtra, (dwMsgSize-sizeof(TMsg)));
			}
		};

		FnMsgHandler f = boost::bind(&Local::Trampoline, m_pOwner, fpCall,_1, _2, _3);
		Register(wID, f );
		return true;

	}

	template < class TMsg >
	BOOL Register( typename RegistMsg<TMsg>::FP_CALL_3 fpCall, WORD wID)
	{
		struct Local
		{
			static	BOOL	Trampoline(Vector::CVtZoneBase* pSvr, typename RegistMsg<TMsg>::FP_CALL_3 fpCall, CVSock* , char* pMsg, DWORD dwMsgSize)
			{
				if (dwMsgSize < sizeof(TMsg))
					return FALSE;

				char*	pExtra = NULL;

				if (dwMsgSize > sizeof(TMsg))
					pExtra = pMsg+sizeof(TMsg);

				return (((TMsg*)pMsg)->*fpCall)(pSvr, pExtra, (dwMsgSize-sizeof(TMsg)));
			}
		};

		FnMsgHandler f = boost::bind(&Local::Trampoline, m_pOwner, fpCall,_1, _2, _3);
		Register(wID, f );
		return true;

	}
	
	template < class TMsg >
	BOOL Register(typename RegistMsg<TMsg>::FP_CALL_4 fpCall, WORD wID )
	{
		struct Local
		{
			static	BOOL	Trampoline(Vector::CVtZoneBase* pSvr, typename RegistMsg<TMsg>::FP_CALL_4 fpCall , CVSock* pFrom, char* pMsg, DWORD dwMsgSize)
			{
				if (dwMsgSize < sizeof(TMsg))
					return FALSE;

				char*	pExtra = NULL;

				if (dwMsgSize > sizeof(TMsg))
					pExtra = pMsg+sizeof(TMsg);

				return (((TMsg*)pMsg)->*fpCall)(pSvr,pFrom, pExtra, (dwMsgSize-sizeof(TMsg)));
			}
		};

		FnMsgHandler f = boost::bind(&Local::Trampoline, m_pOwner, fpCall, _1, _2, _3);
		Register(wID, f );
		return true;

	}


	//일단은... Socket으로 부터 구하는걸로.... 좀 별로다...
	template<class Recipient, class TMsg>
	class RecipientHandler
	{
	public:
		typedef BOOL (Recipient::*FP_CALL_ORIGINAL)( TMsg&, char*, DWORD );
	};

	template< class Recipient, class TMsg >
	BOOL RegisterCustomHandler( typename RecipientHandler<Recipient, TMsg>::FP_CALL_ORIGINAL fpCall , WORD wID)
	{
		struct Local
		{
			static	BOOL	Trampoline(Vector::CVtZoneBase* pSvr, typename RecipientHandler<Recipient, TMsg>::FP_CALL_ORIGINAL fpCall , CVSock* pFrom, char* pMsg, DWORD dwMsgSize)
			{
				if (dwMsgSize < sizeof(TMsg))
					return FALSE;

				char*	pExtra = NULL;

				if (dwMsgSize > sizeof(TMsg))
					pExtra = pMsg+sizeof(TMsg);

				Recipient* pRecipient = CZoneMsgDispatcher::GetRecipient( pSvr, pFrom );
				if(nullptr == pRecipient)
					return FALSE;				 
				
				return (pRecipient->*fpCall)( *((TMsg*)pMsg), pExtra, (dwMsgSize-sizeof(TMsg)));
			}
		};
		FnMsgHandler f = boost::bind(&Local::Trampoline, m_pOwner, fpCall, _1, _2, _3);
		Register(wID, f );
		return true;
	}

	static GameWorld::CPlayerChar* GetRecipient( Vector::CVtZoneBase*, CVSock* );

private:
	void	Register(WORD wID, FnMsgHandler f)
	{

		if (m_vecHandlers.size() <= wID)
			m_vecHandlers.resize(wID+1);
		assert((m_vecHandlers[wID].empty()) && "Msg handler is allocated already ~~");
		m_vecHandlers[wID] = f;
	}

private:

	std::vector<FnMsgHandler>		m_vecHandlers;
	FnMsgHandler					m_fnUnhandled;
	Vector::CVtZoneBase*			m_pOwner;
};