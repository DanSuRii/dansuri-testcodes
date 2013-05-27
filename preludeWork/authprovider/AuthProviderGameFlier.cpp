#include "precomp.h"
#include "AuthProviderInterface.h"
#include "AuthProviderGameFlier.h"

#include <algorithm>

//Packets begin
#pragma pack(push, 1)

#define IP_ADDRESS_SIZE 10

namespace GAMEFLIER_PACKETS
{
	//��Ŷ�� ������� ó�� Ÿ���� ���� �� �� ������,
	//ó���⸦ ���� �ϴ� ����� ���� �� ������ �Ѵ�.

	/*pseudo code
	#define PROCEDURES_BEGIN switch() {
	#define PROCEDURES_END	}
	#define REGPROCEDURES \
	case '' : worker<>->doProc( data )\

	template< typename packet<>, class callBack >
	class worker
	{		
		public:
		doProc( packet<>::type* p ){ callBack(p); }
	}
	*/
	template<class myType>
	class packetBase
	{
	public:
		typedef myType type;
		static const WORD size = sizeof(myType);
	};

	template<int code>
	class Packets;

	//middleware packets
	struct GF_Header
	{
		char head[1];
	};

	struct Request : public GF_Header
	{

		char token[100+1];
		__int64 sessionkey;
		//char ip[IP_ADDRESS_SIZE];
	};

	struct Response : public GF_Header
	{
		int result;
		char reason[50+1];
		__int64 sessionkey;
	};
	template<>
	class Packets<'s'>: public packetBase<Response>{};

	enum eMIDDLELoginResult
	{
		MIDDLELOGINRESULT_SUCCEED = 1,
		MIDDLELOGINRESULT_INVALID_IP = -19,
		MIDDLELOGINRESULT_TIMEOUT = -21,
		MIDDLELOGINRESULT_INVALID_TOKEN = -23,
		MIDDLELOGINRESULT_OTHER_ERR = -99

	};

	//PLSS packets
	struct PUREPLSS_Header
	{
		unsigned short packet_len;
		unsigned short packet_type;

	};

	struct PLSS_Header : public PUREPLSS_Header
	{
		PLSS_Header(unsigned short len, unsigned short aType)
		{
			packet_len = len;
			packet_type = aType;
		}

		bool operator ==(PLSS_Header& rhs)
		{
			return (packet_len == rhs.packet_len) && (packet_type == rhs.packet_type);
		}
	};

	struct Login : public PLSS_Header
	{
		Login():PLSS_Header(sizeof(*this), 0x0E01){}

		unsigned int	worldID;
		char			username[25];
		char			loginTime[20];
	};
	
	struct Ping : public PLSS_Header
	{
		Ping():PLSS_Header(sizeof(*this), 0x0E02){}		
	};

	struct LoginResult : public PLSS_Header
	{
		LoginResult():PLSS_Header(sizeof(*this), 0xD601){}

		char				userName[25];
		unsigned char		resultValue;
	};
	template<>
	class Packets<0xD601> : public packetBase<LoginResult>{};

	enum ePLSSLoginResult
	{
		PLSSLOGINRESULT_TIMEOUT = 0,
		PLSSLOGINRESULT_ALLOW_LOGIN = 1,
		PLSSLOGINRESULT_ALREADY_LOGGED_IN = 2,
		PLSSLOGINRESULT_ALREADY_LOGIN_WAIT = 3
	};
}
#pragma pack(pop)
//Packets end



struct NS_AUTHNET::AuthSock::SMsgBuffer
{
	char* RawMsg(void) { return &Buffer; }
	template<class T>
	void CastRawMsg(T& dest){ dest = (T)&Buffer; }


	union{
		GAMEFLIER_PACKETS::GF_Header		gfHeader;
		GAMEFLIER_PACKETS::PUREPLSS_Header	plssHeader;
		char								Buffer;
		char								ReponseMsg	[sizeof(GAMEFLIER_PACKETS::Response)];
		char								loginResult	[sizeof(GAMEFLIER_PACKETS::LoginResult)];
	};
};

template<size_t sz>
bool IsNullterminated( const char (&targetChar)[sz] )
{
	const char* pCur = &targetChar[sz-1];
	do
	{
		if(*pCur == '\0') return true;
	}while((pCur--) != targetChar );

	return false;
}

template<>
void NS_AUTHPROVIDERGF::AuthProviderGF::Handler(
	GAMEFLIER_PACKETS::Response& response )
{
	enum{
		AUTH_RESULT_SUCCEED = 1,
		AUTH_RESULT_INVALID_IP = -19,
		AUTH_RESULT_TIMEOUT = -21,
		AUTH_RESULT_INVALID_TOKEN = -23,
		AUTH_RESULT_OTHER_ERR = -99
	};

	std::string strSessionAcct("NULL");
	std::string strReason( response.reason, _countof(response.reason));
	
	//�����͸� ������ �����ϸ� �ȵȴ�. ��ȿ���� ������ ���� ����.
	Session aSession(SESSIONSTEP_MIDDLEWARE);
	ASSERT_RETURN(sessionManager.PopSession( response.sessionkey, aSession));

	/*
	// �ش� ������ ���Ǹ� ���������� �ϴ� ������ �ʿ��ϴ�.
	ASSERT_RETURN(aSession.IsStep( step_middleware ));
	*/


	ULONG billingResult = (ULONG)~BILLING_OK;
	//bool acctCompareResult = (strSessionAcct.compare( strReason ) == 0);
	//���� �Լ��� ���������� memcmp�� ȣ���ϴµ�, ���� �Ҵ� string���� compare����� �ùٸ��� �ʴ� ������ �ִ�.
	bool acctCompareResult = (PStrCmp( strSessionAcct.c_str(), strReason.c_str() ) == 0);

	if( (response.result == AUTH_RESULT_SUCCEED) && (true == acctCompareResult) )
	{
		billingResult = BILLING_OK;

		//���� ���� ���� �� ���� ���ǿ� ��� ó������ �κ� ���� �ֵ��� �Ѵ�.
		PlssRequestLogin(aSession);
	}
	else
	{
		TraceError("AuthProviderGameFlier: Login Request Failed With Result (%d), Reason(%s), AcctID(%s), Session(%I64d)",
			response.result,
			strReason.c_str(),
			strSessionAcct.c_str(),
			response.sessionkey);
/*
	LoginServer::LoginAuthResponseHandler( pMsg->sessionkey, billingResult 
#if defined(_SHUTDOWN_LAW)
		,0
#endif
#if defined(_SELECTABLE_SHUTDOWN)
		,0
#endif
		);
*/
	}


	//�� �ܰ迡���� ���������� AuthResponseHandler�� �ҷ��ְ�,
	//���������� session�� �ٽ� ����ְ� ���� �ܰ踦 �⵵�� �Ѵ�.
/*
	LoginServer::LoginAuthResponseHandler( pMsg->sessionkey, billingResult 
#if defined(_SHUTDOWN_LAW)
		,0
#endif
#if defined(_SELECTABLE_SHUTDOWN)
		,0
#endif
		);
*/

}


template<>
void NS_AUTHPROVIDERGF::AuthProviderGF::Handler(
	GAMEFLIER_PACKETS::LoginResult& response )
{
	ULONG billingResult = (ULONG)~BILLING_OK;

	ASSERT_RETURN(IsNullterminated(response.userName));

	Session aSession( SESSIONSTEP_PLSS );
	ASSERT_RETURN(sessionManager.PopPlssSession( std::string(response.userName) , aSession ));

	
	if( GAMEFLIER_PACKETS::PLSSLOGINRESULT_ALLOW_LOGIN == response.resultValue)
	{
		billingResult = BILLING_OK;
	}	

/*
	LoginServer::LoginAuthResponseHandler( aSession.sUid, billingResult 
#if defined(_SHUTDOWN_LAW)
		,0
#endif
#if defined(_SELECTABLE_SHUTDOWN)
		,0
#endif
		);
*/

}


namespace NS_AUTHPROVIDERGF
{
	NetCommunicator* NetComManager::communicators[NetCommunicator::COMMUNICATORS_CNT] = {NULL,};
	WSAEVENT NetComManager::wsaEvents[NetCommunicator::COMMUNICATORS_CNT] = {WSA_INVALID_EVENT, };


	NetcomInitializers& NetcomInitializers::setAuthProvider( AuthProviderGF* pAuthProvider )
	{
		std::for_each( info, info + _countof(info),
			[&pAuthProvider](NetcomInitializer& initializer)
		{
			initializer.pAuthProvider = pAuthProvider;
		});

		return *this;
	}


	NetCommunicator::NetCommunicator( NetcomInitializer& initStruc  )
		:sockInterface(initStruc.pszAddress), pAuthProvider(initStruc.pAuthProvider)
	{

	}

	void NetCommunicator::SetState( state aState )
	{
		eState = aState;
	}

	NetCommunicator::state NetCommunicator::GetState()
	{
		return eState;
	}

	bool NetCommunicator::IsState( state aState )
	{
		return GetState() == aState;
	}


	void NetCommunicator::ProcessSocketEvents()
	{
		//���� ������Ʈ�� ���� ���¸� connect, �������� ó���Ѵ�.

		WSANETWORKEVENTS networkEvents;
		SOCKET hSocket = this->getSocket();
		WSAEVENT hSocketEvent = this->getEvent();
		ASSERTV_DO(
			WSAEnumNetworkEvents(hSocket, hSocketEvent, &networkEvents) != SOCKET_ERROR,
			"%s: WSAEnumNetworkEvents failed with error %d", GetMyName() ,WSAGetLastError())
		{
			return DisconnectFromServer();
		}

		if(networkEvents.lNetworkEvents & FD_CONNECT)
		{
			if( false == IsState(state_connecting) )
				return ;
			ASSERTV_DO(networkEvents.iErrorCode[FD_CONNECT_BIT] == 0,
				"AuthProviderGameFiler: connection error %d", networkEvents.iErrorCode[FD_CONNECT_BIT])
			{
				return DisconnectFromServer();
			}
			//SendServerLoginMsg();
			SetState( state_ready );
			TraceVerbose(TRACE_FLAG_LOGIN_SERVER, "%s: successfully connect to GF Server", GetMyName());
		} 


		if(networkEvents.lNetworkEvents & FD_CLOSE)
		{		
			TraceWarn(TRACE_FLAG_LOGIN_SERVER,
				"%s: connection close with error %d\n",
				GetMyName(),
				networkEvents.iErrorCode[FD_CLOSE_BIT]);
			return DisconnectFromServer();
		}

		if(networkEvents.lNetworkEvents & FD_READ)
		{
			ASSERTV_DO(networkEvents.iErrorCode[FD_READ_BIT] == 0,
				"AuthProviderGameFiler: read failed with error %d", networkEvents.iErrorCode[FD_READ_BIT])
			{
				return DisconnectFromServer();
			}


			if( /* IsState(state_lggingin) ||*/
				IsState(state_ready))
			{
				RecvAndProcMsg();
			}

		}

		if(networkEvents.lNetworkEvents & FD_WRITE)
		{
			ASSERTV_DO(networkEvents.iErrorCode[FD_WRITE_BIT] == 0,
				"AuthProviderGameFiler: write failed with error %d", networkEvents.iErrorCode[FD_WRITE_BIT])
			{
				return DisconnectFromServer();
			}


			if( /* IsState(state_lggingin) ||*/
				IsState(state_ready))
			{
				sockInterface.SendFromBufferQueue();
			}
			else
			{
				TraceError("FD_WRITE isn't ready");
			}

		}


	}

	void NetCommunicator::DisconnectFromServer()
	{
		//������Ʈ ���� ó��

		TraceVerbose( TRACE_FLAG_LOGIN_SERVER, "NetCommunicator: disconnecting from server\n" );

		this->sockInterface.Disconnect();
		
		//SendBufferQueue û��
	}

	void NetCommunicator::TryConnect()
	{
		if(IsState( state_not_connected ))
		{
			if(true == sockInterface.ConnectToServer())
				SetState(state_connecting);
			else
				DisconnectFromServer();
		}
	}

	MiddlewareServerCommunicator::MiddlewareServerCommunicator( NetcomInitializer& initStruc )
		:NetCommunicator(initStruc)
	{

	}

	void MiddlewareServerCommunicator::RecvAndProcMsg()
	{
		ASSERT_RETURN(sockInterface.IsReadbufferValid());
		while (RecvNextMsg())
		{
			switch(GetReadBuffer()->gfHeader.head[0])
			{
			case 's':
				{
					GAMEFLIER_PACKETS::Packets<'s'>::type* pMsg = NULL;
					GetReadBuffer()->CastRawMsg(pMsg);

					//AuthProviderGF���� session �޼����� ����, ó���ϵ��� �Ѵ�.
					if(pAuthProvider) pAuthProvider->Handler( *pMsg );
				}
				break;
			}
		}
	}


	bool MiddlewareServerCommunicator::RecvNextMsg()
	{
		ASSERT_RETFALSE(/*IsState(state_loggingin) ||*/ IsState(state_ready));

		ASSERT_RETFALSE( sockInterface.IsReadbufferValid() );

		int iBytesAvailable = sockInterface.RecvToBuffer( GetReadBuffer()->RawMsg(), sizeof( GAMEFLIER_PACKETS::GF_Header ), true );
		if(iBytesAvailable == SOCKET_ERROR)
			return false;
		if(iBytesAvailable < sizeof(GAMEFLIER_PACKETS::GF_Header))
			return false;

		const GAMEFLIER_PACKETS::GF_Header& header = GetReadBuffer()->gfHeader;
		if(ioctlsocket(getSocket(), FIONREAD, reinterpret_cast<unsigned long*>(&iBytesAvailable)) == SOCKET_ERROR)
		{
			TraceWarn(TRACE_FLAG_LOGIN_SERVER,
				"AuthProviderGameFlier: socket ioctlsocket failed with error %d\n", WSAGetLastError());
			DisconnectFromServer();
		}

		WORD size = 0;

		switch( header.head[0] )
		{
		case 's':
			size = GAMEFLIER_PACKETS::Packets<'s'>::size;
			break;

		default:
			return false;
			break;
		}

		if(size > iBytesAvailable)
			return false; // wait for more data

		return sockInterface.RecvToBuffer(GetReadBuffer()->RawMsg(), size, false) != SOCKET_ERROR;

	}

	const char* MiddlewareServerCommunicator::GetMyName()
	{
		return "MiddlewareServerCommunicator";
	}


	PLSSServerCommunicator::PLSSServerCommunicator( NetcomInitializer& initStruc )
		:NetCommunicator(initStruc)
	{

	}

	void PLSSServerCommunicator::RecvAndProcMsg()
	{
		ASSERT_RETURN(sockInterface.IsReadbufferValid());
		while (RecvNextMsg())
		{
			switch( GetReadBuffer()->plssHeader.packet_type )
			{
			case 0xD601:
				GAMEFLIER_PACKETS::Packets<0xD601>::type* pPacket;
				GetReadBuffer()->CastRawMsg( pPacket );
				this->pAuthProvider->Handler( *pPacket );
			}
		}
	}

	bool PLSSServerCommunicator::RecvNextMsg()
	{
		ASSERT_RETFALSE(/*IsState(state_loggingin) ||*/ IsState(state_ready));

		ASSERT_RETFALSE( sockInterface.IsReadbufferValid() );

		int iBytesAvailable = sockInterface.RecvToBuffer( GetReadBuffer()->RawMsg(), sizeof( GAMEFLIER_PACKETS::PLSS_Header ), true );
		if(iBytesAvailable == SOCKET_ERROR)
			return false;
		if(iBytesAvailable < sizeof(GAMEFLIER_PACKETS::PLSS_Header))
			return false;

		GAMEFLIER_PACKETS::PUREPLSS_Header& header = GetReadBuffer()->plssHeader;
		if(ioctlsocket(getSocket(), FIONREAD, reinterpret_cast<unsigned long*>(&iBytesAvailable)) == SOCKET_ERROR)
		{
			TraceWarn(TRACE_FLAG_LOGIN_SERVER,
				"AuthProviderGameFlier: socket ioctlsocket failed with error %d\n", WSAGetLastError());
			DisconnectFromServer();
		}

		WORD size = header.packet_len;

/*
		switch( header.packet_type )
		{
		case 0xD601:
			size = GAMEFLIER_PACKETS::Packets<'s'>::size;
			break;

		default:
			return false;
			break;
		}
*/

		if(size > iBytesAvailable)
			return false; // wait for more data

		return sockInterface.RecvToBuffer(GetReadBuffer()->RawMsg(), size, false) != SOCKET_ERROR;

	}


	const char* PLSSServerCommunicator::GetMyName()
	{
		return "PLSSServerCommunicator";
	}



	template<typename NetCommunicator::eCommunicators eType, class myType>
	void NetComManager::netcomCreate(NetcomInitializers& initStruc)
	{
		if(eType < 0 || eType >= NetCommunicator::COMMUNICATORS_CNT)
			return ;

		if(communicators[eType] != NULL)
			return ;

		static myType myInstance( initStruc.info[eType] );
		communicators[eType] = &myInstance;
		wsaEvents[eType] = myInstance.getEvent();
	}


	NetComManager::NetComManager(NetcomInitializers& initStruc)
	{
		netcomCreate<NetCommunicator::MIDDLEWARE, MiddlewareServerCommunicator>(initStruc);
		netcomCreate<NetCommunicator::PLSS, PLSSServerCommunicator>(initStruc);
	}

	void NetComManager::ProcessSocketEvents()
	{

		//communicator���� keep in touch ���·� �������ش�.
		std::for_each(communicators, communicators + _countof(communicators), 
			[](NetCommunicator* communicator)
			{
				if(communicator) communicator->TryConnect();
			});
		

		static DWORD dwResult = 0;
		dwResult = WSAWaitForMultipleEvents( _countof(this->wsaEvents), wsaEvents, false, 1000, false );

		if(dwResult == WSA_WAIT_FAILED)
		{
			TraceError("NetComManager::ProcessSocketEvents: WSAWaitForMultipleEvents failed with error %d\n", WSAGetLastError());
			Sleep(1000);
		}



		//������Ʈ üũ

		int evtIdx = dwResult - WSA_WAIT_EVENT_0;
		if( evtIdx > 0 && evtIdx < _countof(communicators) )
			communicators[evtIdx]->ProcessSocketEvents();		
		else
		{
			//�ܼ� Ÿ�Ӿƿ����� disconnect�� ���ϵ��� connect ���Ѿ� �Ѵ�. (X) ������ try connect ��.
			TraceError("NetComManager::ProcessSocketEvents: WSAWaitForMultipleEvents failed with error %d\n", WSAGetLastError());
			Sleep(1000);
		}

		//�ϳ��� ������ ������ ������ keep connect�� ���ؼ� .... wait for multiple events�� �ǳ� �ٰų�- ��Ÿ�� ���� �ؾ��Ѵ�.

/*
		

		std::for_each( communicators, communicators + _countof(communicators),
			[](NetCommunicator* cur){
				cur->ProcessSocketEvents();
			}
			);
*/
			//2005������ �����ؾ� �� ���� �� ��;
	}

	template<class T>
	bool NetComManager::SendTo( NetCommunicator::eCommunicators eCommunicator, T& req )
	{
		ASSERT_RETFALSE(eCommunicator > 0 && eCommunicator < NetCommunicator::COMMUNICATORS_CNT );
		return communicators[eCommunicator]->SendMsg(req);
	}



	//�����ڿ��� this�����͸� �ѱ�� C4355 warning
	//�Ϲ������� ��� ���� �ʴ� ǥ�����̴�. this�����Ͱ� ���� �ʱ�ȭ ���� �ʾұ� ������ ��ü�� �ɹ� ����/�Լ��� ���� �� �� ����.
	//�ٸ� �ܼ� �����͸� �����ϱ� ���� �뵵���, ������ ǥ�����̴�. ( ��, ��ʰ� �������̰� ���輺�� �����ϰ� �����Ƿ� warning�̴�. )
	//��Ÿ��, ��Ӱ��踦 hard�ϰ� Ȱ���ϴ� ��쳪 pointer�� hash�ؼ� ��� �� �����̶�� ���� �̷��� ������� �ʵ��� �Ѵ�.
	AuthProviderGF::AuthProviderGF(NetcomInitializers& initStruc)
		:communicator(initStruc.setAuthProvider(this))
	{
		threadManager.doCreateThread(*this);
		myState = state_working;
	}

	void AuthProviderGF::RequestLogin(const LoginRequestStruc& req)
	{
		const LoginStruc* myReq = dynamic_cast<const LoginStruc*>(&req);
		ASSERT_RETURN(myReq);
	}

	AuthProviderGF::~AuthProviderGF()
	{
		myState = state_terminate;
		threadManager.WaitForTerminate();
	}

	//call from thread
	void AuthProviderGF::DoWork()
	{
		//communicator->wsaEvents;
		while( myState != state_terminate )
		{
			this->communicator.ProcessSocketEvents();

			//this->sessionManager

			//���� ���� ������Ʈ IsState(state_terminate) break;
		}

	}

	bool AuthProviderGF::PlssRequestLogin( Session& aSession )
	{
		GAMEFLIER_PACKETS::Login loginRequest;
		strcpy_s(loginRequest.username, aSession.account_id.c_str());
		
		time_t curTime;
		time(&curTime);
		tm curTM;
		localtime_s( &curTM, &curTime );
		
		strftime( loginRequest.loginTime, _countof(loginRequest.loginTime),
			"%Y/%m/%d %H:%M:%S", &curTM);

		loginRequest.worldID = aSession.worldID;

		ASSERT_RETFALSE(this->communicator.SendTo( NetCommunicator::PLSS, loginRequest ));
		ASSERT_RETFALSE(this->sessionManager.PushPlssSession( std::string( aSession.account_id ), aSession ));

		return true;
	}


	bool SessionManager::PopSession( SERVICEUSERID sessionkey, Session& toPop )
	{
		CSAutoLock lock(&csSessionLock);
		SessionMap::iterator iter = FindSession(sessionkey);
		ASSERT_RETFALSE(iter != sessionMap.end());
		//�Ʒ��� ��ȿ(X)
		//���� ������ ���� ���� ���ǿ� ���� ��û�� �и��� assert ����� �´�.
		//�ٸ�, step plss���� ������ session�� clear���� �ʰ� step middlware �� �ٽ� ��� ���� ������ � ���͸��� �� �������� ����
		//����� ����� �־�� �Ѵ�.
		//ASSERT_RETFALSE(toPop.sessionStep == iter->second.sessionStep);
		toPop = iter->second;

		sessionMap.erase(iter);

		return true;
	}

	SessionManager::SessionMap::iterator SessionManager::FindSession( SERVICEUSERID sessionkey )
	{
		SessionMap::iterator iter = sessionMap.find( sessionkey );
		return iter;
	}

	bool SessionManager::PopPlssSession( std::string& strToGet, Session& toPop )
	{
		CSAutoLock lock(&csPlssSessionLock);
		PlssSessionMap::iterator iter = this->plssSessionMap.find( strToGet );
		ASSERT_RETFALSE(iter != plssSessionMap.end());

		toPop = iter->second;

		plssSessionMap.erase(iter);

		return true;
	}

	bool SessionManager::PushPlssSession( std::string& strToSet, Session& toSet )
	{
		CSAutoLock lock(&csPlssSessionLock);
		
		return plssSessionMap.insert( std::make_pair( strToSet, toSet ) ).second;
	}

}