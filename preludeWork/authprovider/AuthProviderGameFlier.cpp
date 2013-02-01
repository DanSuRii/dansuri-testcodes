#include "precomp.h"
#include "AuthProviderInterface.h"
#include "AuthProviderGameFlier.h"

#include <algorithm>

//Packets begin
#pragma pack(push, 1)

#define IP_ADDRESS_SIZE 10

namespace GAMEFLIER_PACKETS
{
	//패킷의 사이즈와 처리 타입은 지정 할 수 있지만,
	//처리기를 연결 하는 방법을 구상 해 보도록 한다.

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

	//middle ware packets
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

template<>
void NS_AUTHPROVIDERGF::AuthProviderGF::Handler<GAMEFLIER_PACKETS::Response>(
	GAMEFLIER_PACKETS::Response& response )
{
	switch(response.result)
	{

	}
}



namespace NS_AUTHPROVIDERGF
{
	NetCommunicator* NetComManager::communicators[NetCommunicator::COMMUNICATORS_CNT] = {NULL,};
	WSAEVENT NetComManager::wsaEvents[NetCommunicator::COMMUNICATORS_CNT] = {WSA_INVALID_EVENT, };



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
		//현재 스테이트가 끊김 상태면 connect, 상위에서 처리한다.

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
		//스테이트 관련 처리

		TraceVerbose( TRACE_FLAG_LOGIN_SERVER, "NetCommunicator: disconnecting from server\n" );

		this->sockInterface.Disconnect();
		
		//SendBufferQueue 청소
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

					//AuthProviderGF에게 session 메세지를 전달, 처리하도록 한다.
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
		throw std::exception("The method or operation is not implemented.");
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

		//communicator들을 keep in touch 상태로 유지해준다.
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



		//스테이트 체크

		int evtIdx = dwResult - WSA_WAIT_EVENT_0;
		if( evtIdx > 0 && evtIdx < _countof(communicators) )
			communicators[evtIdx]->ProcessSocketEvents();		
		else
		{
			//단순 타임아웃때는 disconnect된 소켓들을 connect 시켜야 한다. (X) 위에서 try connect 함.
			TraceError("NetComManager::ProcessSocketEvents: WSAWaitForMultipleEvents failed with error %d\n", WSAGetLastError());
			Sleep(1000);
		}

		//하나가 접속이 끊겼을 때에는 keep connect를 위해서 .... wait for multiple events를 건너 뛰거나- 여타의 일을 해야한다.

/*
		

		std::for_each( communicators, communicators + _countof(communicators),
			[](NetCommunicator* cur){
				cur->ProcessSocketEvents();
			}
			);
*/
			//2005에서는 수정해야 할 꺼임 ㅡ ㅜ;
	}



	AuthProviderGF::AuthProviderGF(NetcomInitializers& initStruc)
		:communicator(initStruc.setAuthProvider(this))
	{
		threadManager.doCreateThread(*this);

	}

	void AuthProviderGF::RequestLogin()
	{

	}

	AuthProviderGF::~AuthProviderGF()
	{
		threadManager.WaitForTerminate();
	}

	//call from thread
	void AuthProviderGF::DoWork()
	{
		//communicator->wsaEvents;

		this->communicator.ProcessSocketEvents();

	}


	NetcomInitializers& NetcomInitializers::setAuthProvider( AuthProviderGF* pAuthProvider )
	{
		std::for_each( info, info + _countof(info),
			[&pAuthProvider](NetcomInitializer& initializer)
		{
			initializer.pAuthProvider = pAuthProvider;
		});

		return *this;
	}

}