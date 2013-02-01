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

interface AuthProvider
{
	virtual void RequestLogin() = 0;
};

#endif // __AUTHPROVIDERINTERFACE_H__