#ifndef __AUTHPROVIDERINTERFACE_H__
#define __AUTHPROVIDERINTERFACE_H__

namespace NS_AUTHNET
{
	class AuthSock
	{
	public:
		struct SMsgBuffer;

	private:
		SOCKET hSocket;
		WSAEVENT hEvent;
		sockaddr_in Address;

		typedef deque_mp< std::pair<const char*, int> >::type TSendBufferQueue; 
		TSendBufferQueue SendBufferQueue;
		int iSendBufferOffset;

		SMsgBuffer* pReadBuffer;
		CCriticalSection csSendBufferLock;
		int SendFromBuffer( const char* szBuffer, int iBufferSize );


	public:
		AuthSock(char* szAddress);
		~AuthSock();

		bool		IsReadbufferValid() const { return NULL != pReadBuffer; }
		SMsgBuffer* GetReadBuffer() { return pReadBuffer; }

		bool		SendFromBufferQueue();
		bool		ConnectToServer();

		int			RecvToBuffer( char* szBuffer, int iBufferSize, bool bPeek );

		bool		SendMsg( const char* pData, size_t dataSize);
		
		
		void		ClearSendBufferQueue();
		
		WSAEVENT	getEvent(){ return hEvent; }
		SOCKET		getSocket(){ return hSocket; }
		void		Disconnect();

		template< class REQ >
		bool		SendMsg( REQ* req )
		{
			return SendMsg( (const char *)req ,sizeof(REQ) );
		};
	};



	class ThreadManager
	{
		HANDLE hSockThreadHandle;

	public:
		void WaitForTerminate();

		~ThreadManager();

		template<class T>
		static unsigned int WINAPI WorkthreadCaller(void* pThis)
		{
			(static_cast<T*>(pThis))->DoWork();

			return 0;
		}

		template<class T>
		void doCreateThread(T& tTarget)
		{
			hSockThreadHandle = reinterpret_cast<HANDLE>( _beginthreadex( 0, 0, WorkthreadCaller<T>, &tTarget, 0, 0 ) );
		}

	};	



}

enum AuthProviderTypes
{
	APT_GAMEFLIER
};

interface AuthProvider
{
	class LoginRequestStruc{
	public:
		virtual ~LoginRequestStruc(){;}
	};

	enum eErrCode
	{
		err_none,
		err_invalid_parameter
	};

	//const여야 하는 이유는 MEC++ p.158 에 설명되어있다. 임시객체를 사용하기 위해서 필수불가결한 요소.
	virtual void RequestLogin(const LoginRequestStruc& ) = 0;

	
	/*
	template<class member1, class member2>
	struct ConcreateLoginStruct : public LoginRequestStruc

	맴버가 더 많이 필요한 login request가 있으면 더 정의해서 쓰면 된다. 2개 / 3개 버전은 미리 만들어 둠.
	*/

	template<class member1, class member2>
	struct ConcreateLoginStruct2 : public LoginRequestStruc
	{
		member1*	ptrMember1;
		member2*	ptrMember2;

		ConcreateLoginStruct2(member1* pmbr1, member2* pmbr2)
			: ptrMember1(pmbr1), ptrMember2(pmbr2)
		{
		}
	};

	template<class member1, class member2, class member3>
	struct ConcreateLoginStruct3 : public LoginRequestStruc
	{
		member1*	ptrMember1;
		member2*	ptrMember2;
		member3*	ptrMember3;

		ConcreateLoginStruct3(member1* pmbr1, member2* pmbr2, member3* pmbr3)
			: ptrMember1(pmbr1), ptrMember2(pmbr2), ptrMember3(pmbr3)
		{
		}
	};


};

template<AuthProviderTypes> class MyProvider;

template<class BaseClass, class member1, class member2>
typename BaseClass::LoginStruc makeLoginRequest(member1* ptrMember1, member2* ptrMember2)
{	
	return BaseClass::LoginStruc(ptrMember1, ptrMember2);
}

template<AuthProviderTypes providerType, class member1, class member2>
typename MyProvider<providerType>::type::LoginStruc makeLoginRequest(member1* ptrMember1, member2* ptrMember2)
{
	return makeLoginRequest< MyProvider<providerType>::type >(ptrMember1, ptrMember2);
}


#endif // __AUTHPROVIDERINTERFACE_H__