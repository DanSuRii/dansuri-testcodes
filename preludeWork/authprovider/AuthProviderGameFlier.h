#ifndef __AUTHPROVIDERGAMEFLIER_H__
#define __AUTHPROVIDERGAMEFLIER_H__

#include "AuthProviderInterface.h"


namespace NS_AUTHPROVIDERGF
{	
	class SessionManager
	{

	};


	struct NetcomInitializer;

	//지금은 상속구조로 되어있는데,
	//RecvAndProc을 제외하면 나머지는 다 같은 처리가 될 가능성이 있으므로
	//RecvAndProc을 하나의 interface로 놓고 핸들러 처리를 하는 것으로도 방향을 잡아본다.

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
#endif //추후에 제대로 만들면 쓸 수 있을 것 같다.
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

	};

	class AuthProviderGF : public AuthProvider
	{
		SessionManager sessionManager;

		NetComManager communicator;

		
		NS_AUTHNET::ThreadManager threadManager;
		//NS_AUTHNET::AuthSock	authSocket;		

	public:
		AuthProviderGF(NetcomInitializers&);
		~AuthProviderGF();
		void RequestLogin();
		void DoWork();
		template<class T> void Handler(T&);

	};

	

}

#endif // __AUTHPROVIDERGAMEFLIER_H__