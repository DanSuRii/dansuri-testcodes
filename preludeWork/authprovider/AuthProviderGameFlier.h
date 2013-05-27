#ifndef __AUTHPROVIDERGAMEFLIER_H__
#define __AUTHPROVIDERGAMEFLIER_H__

#include "AuthProviderInterface.h"


namespace NS_AUTHPROVIDERGF
{	
	//�ϳ��� �����̶�� ���� ID�� ������ �ְ� ���� ������ ���¸� ������ �ִ� ���� ���Ѵ�.

	enum eSessionStep
	{
		SESSIONSTEP_MIDDLEWARE,
		SESSIONSTEP_PLSS
	};
	
	struct Session
	{
		unsigned int	worldID;		//���߿� ���� ������ �𸣴ϱ� �ϴ� �߰�.
		std::string		account_id;
		time_t			lastActionTime;
		SERVICEUSERID	sUid;
		eSessionStep	sessionStep;
		
		Session(eSessionStep step):sessionStep(step){}
	};

	class SessionManager
	{
		typedef map_mp< SERVICEUSERID, Session >::type SessionMap;
		SessionMap sessionMap;
		CCriticalSection csSessionLock;

		typedef map_mp< std::string, Session >::type	PlssSessionMap;
		PlssSessionMap	plssSessionMap;
		CCriticalSection csPlssSessionLock;
		
		SessionMap::iterator FindSession(SERVICEUSERID);

	public:
		SessionManager():csSessionLock(TRUE), csPlssSessionLock(TRUE){}
		//toPop�� SessionStep���ٰ� ���ϴ� session������ �ְ� pop�ؾߵ�. 
		bool		PopSession( SERVICEUSERID sessionkey, Session& toPop );
		bool		PopPlssSession( std::string& strToGet, Session& toPop );
		bool		PushPlssSession( std::string& strToSet, Session& toSet );
	};


	struct NetcomInitializer;

	//������ ��ӱ����� �Ǿ��ִµ�,
	//RecvAndProc�� �����ϸ� �������� �� ���� ó���� �� ���ɼ��� �����Ƿ�
	//RecvAndProc�� �ϳ��� interface�� ���� �ڵ鷯 ó���� �ϴ� �����ε� ������ ��ƺ���.

	class AuthProviderGF;

	class NetCommunicator
	{

protected:
	NS_AUTHNET::AuthSock	sockInterface;
	AuthProviderGF*			pAuthProvider;

		enum state
		{
			state_terminate,
			state_not_connected,
			state_connecting,
			state_ready
		};
	
		state eState;
/*
		typedef void (NetCommunicator::*MSGPROC)();
		MSGPROC pRecvAndProcHandler;
*/


#if 0
		template<class T>
		const char* GetMyName(T* pT)
		{
#define GETSTRNAME(typeName) #typeName
			return GETSTRNAME(T);
#undef GETSTRNAME
		}
#endif //���Ŀ� ����� ����� �� �� ���� �� ����.
		virtual const char* GetMyName() = 0;

	public:
		WSAEVENT		getEvent(){ return sockInterface.getEvent(); }
		SOCKET			getSocket(){ return sockInterface.getSocket(); }

		enum eCommunicators
		{
			MIDDLEWARE,
			PLSS,
			COMMUNICATORS_CNT
		};
		
		NetCommunicator(NetcomInitializer&);
		void ProcessSocketEvents();
		void DisconnectFromServer();
		void TryConnect();

		NS_AUTHNET::AuthSock::SMsgBuffer* GetReadBuffer(){ return sockInterface.GetReadBuffer(); }

		virtual void RecvAndProcMsg() = 0;
		
		void		SetState( state aState );
		state		GetState();
		bool		IsState( state aState );

		template< class REQ >
		bool		SendMsg( REQ& req )
		{
			return sockInterface.SendMsg(&req);
		}
	};

	struct NetcomInitializer
	{
		NetCommunicator::eCommunicators eType;
		char*							pszAddress;
		AuthProviderGF*					pAuthProvider;
	};
	
	class MiddlewareServerCommunicator : public NetCommunicator
	{
	protected:
		//void ProcessSocketEvents(){}

	public:
		MiddlewareServerCommunicator(NetcomInitializer&);

		virtual void		RecvAndProcMsg();

		virtual const char* GetMyName();
		bool				RecvNextMsg();
	};
	class PLSSServerCommunicator : public NetCommunicator
	{
	protected: 
		//void ProcessSocketEvents(){}
	public:
		PLSSServerCommunicator(NetcomInitializer&);

		virtual void RecvAndProcMsg();

		virtual const char* GetMyName();
		bool PLSSServerCommunicator::RecvNextMsg();
	};

	struct NetcomInitializers
	{
		NetcomInitializer info[NetCommunicator::COMMUNICATORS_CNT];
	private:
		NetcomInitializers();

	public:
		template<size_t arrSize>
		NetcomInitializers(char* (&szAddressList)[arrSize])
		{
			 int iterCnt = (arrSize >= _countof(info)) ? _countof(info) : arrSize;
			 int curIdx = 0;
			 char* firstPtr = szAddressList[0];
			 std::for_each( info, info + iterCnt,
				 [&curIdx, &szAddressList](NetcomInitializer& curInfo){
				 curInfo.pszAddress = szAddressList[ curIdx++ ];
					 //curInfo.pszAddress = "";
			 } );
		}

		NetcomInitializers& setAuthProvider(AuthProviderGF* pAuthProvider);
	};	


	class NetComManager
	{
		static NetCommunicator* communicators[];
		static WSAEVENT			wsaEvents[];
	public:
		template<typename NetCommunicator::eCommunicators, class myType>
		static void netcomCreate(NetcomInitializers&);

		NetComManager(NetcomInitializers&);

		void ProcessSocketEvents();
		template<class T>
		bool SendTo(NetCommunicator::eCommunicators, T& );

	};

	class AuthProviderGF : public AuthProvider
	{
		SessionManager sessionManager;

		NetComManager communicator;

		
		NS_AUTHNET::ThreadManager threadManager;
		//NS_AUTHNET::AuthSock	authSocket;		
		bool PlssRequestLogin( Session& aSession );

		enum state {
			state_idle,
			state_working,
			state_terminate
		}			myState;

	public:
		AuthProviderGF(NetcomInitializers&);
		~AuthProviderGF();
		void DoWork();
		template<class T> void Handler(T&);

		//typedef ConcreateLoginStruct2<ClientContext,MSG_LOGIN_CCMD_LOGIN_AUTH> LoginStruc;
		typedef ConcreateLoginStruct2<int, int> LoginStruc;
		virtual void RequestLogin(const LoginRequestStruc&);

	};	

}

template<>
class MyProvider<APT_GAMEFLIER>
{
public:
	typedef NS_AUTHPROVIDERGF::AuthProviderGF type;
};

#endif // __AUTHPROVIDERGAMEFLIER_H__