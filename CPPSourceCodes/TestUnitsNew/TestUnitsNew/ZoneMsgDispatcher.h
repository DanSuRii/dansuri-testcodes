#pragma once
#include "boost/function.hpp"

namespace Vector
{
	class CVtZoneBase;
}

namespace GameWorld
{
	class CVtZoneServer;
}
class CVSock;

/*
bool CMsg<eHeader>::Handler(GameWorld::CVtZoneServer* pZoneServer, CVSock*, char*, WORD)
{
	pZoneServer->
	DeclearPlayer(CVSock*)
}
*/

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
		typedef BOOL (TMsg::*FP_CALL_3)( Vector::CVtZoneBase*, char*, WORD );
		typedef BOOL (TMsg::*FP_CALL_4)( Vector::CVtZoneBase*, CVSock*, char*, WORD );
	};

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

				return (((TMsg*)pMsg)->*fpCall)(pSvr, pExtra, (WORD)(dwMsgSize-sizeof(TMsg)));
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
			static	BOOL	Trampoline(Vector::CVtZoneBase* pSvr, typename RegistMsg<typename TMsg>::FP_CALL_4 fpCall , CVSock* pFrom, char* pMsg, DWORD dwMsgSize)
			{
				if (dwMsgSize < sizeof(TMsg))
					return FALSE;

				char*	pExtra = NULL;

				if (dwMsgSize > sizeof(TMsg))
					pExtra = pMsg+sizeof(TMsg);

				return (((TMsg*)pMsg)->*fpCall)(pSvr,pFrom, pExtra, (WORD)(dwMsgSize-sizeof(TMsg)));
			}
		};

		FnMsgHandler f = boost::bind(&Local::Trampoline, m_pOwner, fpCall, _1, _2, _3);
		Register(wID, f );
		return true;

	}


/*
	template<  class TMsg, typename BOOL (TMsg::*FN_CALL)( Vector::CVtZoneBase*, char*, WORD ) >
	BOOL Register(WORD wID, FN_CALL fpCall)
	{
		struct Local
		{
			static	BOOL	Trampoline( FN_CALL fpCall, Vector::CVtZoneBase* pSvr, CVSock* , char* pMsg, DWORD dwMsgSize)
			{
				if (dwMsgSize < sizeof(TMsg))
					return FALSE;

				char*	pExtra = NULL;

				if (dwMsgSize > sizeof(TMsg))
					pExtra = pMsg+sizeof(TMsg);

				return (((TMsg*)pMsg)->*fpCall)(pSvr, pExtra, (WORD)(dwMsgSize-sizeof(TMsg)));
			}
		};
		FnMsgHandler f = boost::bind(&Local::Trampoline, m_pOwner, fpCall ,_1, _2, _3);
		Register(wID, f );
		return true;
	}

	template< class TMsg, BOOL (TMsg::*fpCall)( Vector::CVtZoneBase*, char*, WORD ) >
	BOOL Register(WORD wID)
	{
		struct Local
		{
			static	BOOL	Trampoline(Vector::CVtZoneBase* pSvr, CVSock* , char* pMsg, DWORD dwMsgSize)
			{
				if (dwMsgSize < sizeof(TMsg))
					return FALSE;

				char*	pExtra = NULL;

				if (dwMsgSize > sizeof(TMsg))
					pExtra = pMsg+sizeof(TMsg);

				return (((TMsg*)pMsg)->*fpCall)(pSvr, pExtra, (WORD)(dwMsgSize-sizeof(TMsg)));
			}
		};

		FnMsgHandler f = boost::bind(&Local::Trampoline, m_pOwner,_1, _2, _3);
		Register(wID, f );
		return true;
	}

	template< class TMsg, BOOL (TMsg::*fpCall)( Vector::CVtZoneBase*, CVSock*, char*, WORD ) >
	BOOL Register(WORD wID)
	{
		struct Local
		{
			static	BOOL	Trampoline(Vector::CVtZoneBase* pSvr, CVSock* pFrom, char* pMsg, DWORD dwMsgSize)
			{
				if (dwMsgSize < sizeof(TMsg))
					return FALSE;

				char*	pExtra = NULL;

				if (dwMsgSize > sizeof(TMsg))
					pExtra = pMsg+sizeof(TMsg);

				return (((TMsg*)pMsg)->*fpCall)(pSvr,pFrom, pExtra, (WORD)(dwMsgSize-sizeof(TMsg)));
			}
		};

		FnMsgHandler f = boost::bind(&Local::Trampoline, m_pOwner,_1, _2, _3);
		Register(wID, f );
		return true;
	}
*/



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
	Vector::CVtZoneBase*		m_pOwner;
};