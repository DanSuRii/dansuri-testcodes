#include "precomp.h"
#include "AuthProviderInterface.h"

namespace NS_AUTHNET
{

	AuthSock::AuthSock( char* szAddress )
		:hSocket(INVALID_SOCKET),
		hEvent(WSACreateEvent()),
		iSendBufferOffset(0),
		csSendBufferLock(TRUE)
	{
		int nAddressLen = sizeof(Address);
		ASSERTV_RETURN(WSAStringToAddressA( szAddress, AF_INET, 0, (LPSOCKADDR)&Address, &nAddressLen ) == 0,
			"AuthProviderGameFiler: invalid auth provider server address(%s). ErrCode(%d)", szAddress, WSAGetLastError() );
	}

	bool AuthSock::ConnectToServer()
	{
		//ASSERT_RETFALSE( IsState(state_not_connected) && hSocket == INVALID_SOCKET);

		if(Address.sin_addr.S_un.S_addr == 0 || Address.sin_port == 0)
			return false;

		TraceVerbose(TRACE_FLAG_LOGIN_SERVER, "AuthProviderGameFiler: connecting to GF auth server");

		hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		ASSERT_RETFALSE(hSocket != INVALID_SOCKET);
		ASSERTV_DO(
			WSAEventSelect(hSocket, hEvent, FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE) != SOCKET_ERROR,
			"AuthProviderGameFiler: WSAEventSelect failed with error %d", WSAGetLastError())
		{
			//DisconnectFromServer();
			return false;
		}

		//SetState(state_connecting);
		int iResult = ::connect(hSocket, (sockaddr*)&Address, sizeof(Address));
		if(iResult != SOCKET_ERROR || WSAGetLastError() == WSAEWOULDBLOCK)
			return true;

		TraceWarn( TRACE_FLAG_LOGIN_SERVER, "AuthSock: socket connect failed with error %d\n", WSAGetLastError() );
		//DisconnectFromServer();
		return false;
	}


	void AuthSock::Disconnect()
	{
		if( INVALID_SOCKET == hSocket )
			return ;

		::shutdown(hSocket, SD_BOTH);
		closesocket( hSocket );
		hSocket = INVALID_SOCKET;
		ClearSendBufferQueue();
		
	}

	int AuthSock::SendFromBuffer( const char* szBuffer, int iBufferSize )
	{
		ASSERT_RETVAL(szBuffer != NULL, SOCKET_ERROR);
		//ASSERT_RETVAL( /* IsState(state_loggingin) || */ IsState(state_ready) , SOCKET_ERROR);

		int iBytesWritten = send(hSocket, szBuffer, iBufferSize, 0);
		if(iBytesWritten != SOCKET_ERROR)
			return iBytesWritten;

		int iLastError = WSAGetLastError();
		if(iLastError == WSAEWOULDBLOCK)
			return 0;

		TraceWarn(TRACE_FLAG_LOGIN_SERVER, "AuthProviderGameFlier: socket send failed with error %d\n", iLastError);
		//DisconnectFromServer();
		return SOCKET_ERROR;

	}

	bool AuthSock::SendFromBufferQueue()
	{
		//ASSERT_RETFALSE( /*IsState( state_LoggingIn ) ||*/ IsState(state_ready) );

		while( !SendBufferQueue.empty() )
		{
			CSAutoLock lock(&csSendBufferLock);

			const char* pSendBuffer = reinterpret_cast<const char*>(SendBufferQueue.front().first) + iSendBufferOffset;
			int iSendSize = SendBufferQueue.front().second - iSendBufferOffset;
			int iBytesSent = SendFromBuffer(pSendBuffer, iSendSize);
			if(iBytesSent == SOCKET_ERROR)
				return false;
			else if(iBytesSent < iSendSize)
			{
				iSendBufferOffset += iBytesSent;
				return true;
			}
			else
			{
				iSendBufferOffset = 0;
				MEMORYPOOL_DELETE(pMemoryPool, SendBufferQueue.front().first);
				SendBufferQueue.pop_front();
			}

		}

		return true;
	}


	bool AuthSock::SendMsg( const char* pData, size_t dataSize )
	{
		//항상 예약만 걸어둔다

		//ToDo: SendBufferLock

		//int로 바꾸면 - 값이 들어갈 위험이 있음.
		int newDataSize = int(dataSize);
		ASSERT_RETFALSE( newDataSize > 0 );

		int iBytesSent = 0;
		{
			CSAutoLock lock(&csSendBufferLock);
			if(SendBufferQueue.empty())
			{
				iBytesSent = SendFromBuffer( pData, newDataSize );
				if(iBytesSent == SOCKET_ERROR)
					return false;
				else if (iBytesSent == newDataSize)
					return true;
				ASSERT(iBytesSent < newDataSize);
			}

			int iNewBufferSize = newDataSize - iBytesSent;

			char* pNewBuffer = MEMORYPOOL_NEW( pMemoryPool ) char[iNewBufferSize];
			memcpy( pNewBuffer, pData, iNewBufferSize );

			SendBufferQueue.push_back( std::make_pair( pNewBuffer, newDataSize ) );
		}

		return true;

	}

	void AuthSock::ClearSendBufferQueue()
	{
		//ASSERT_RETURN(IsState(state_not_connected));

		{
			CSAutoLock lock(&csSendBufferLock);
			while(!SendBufferQueue.empty())
			{
				MEMORYPOOL_DELETE(pMemoryPool, SendBufferQueue.front().first);
				SendBufferQueue.pop_front();
			}	
		}

	}

	int AuthSock::RecvToBuffer( char* szBuffer, int iBufferSize, bool bPeek )
	{
		ASSERT_RETVAL(szBuffer, SOCKET_ERROR);
		//ASSERT_RETVAL( /*IsState(state_loggingIn) ||*/ IsState(state_ready), SOCKET_ERROR );

		int iBytesAvailable = recv(hSocket, szBuffer, iBufferSize, bPeek ? MSG_PEEK: 0);
		if(iBytesAvailable != SOCKET_ERROR)
			return iBytesAvailable;
		int iError = WSAGetLastError();
		if(iError == WSAEWOULDBLOCK)
			return 0;
		TraceWarn(TRACE_FLAG_LOGIN_SERVER, "AuthProviderGameFlier: socket recv failed with error %d\n", iError);
		//DisconnectFromServer();
		return SOCKET_ERROR;
	}

	AuthSock::~AuthSock()
	{
		Disconnect();
		WSACloseEvent(hEvent);
		
		MEMORYPOOL_DELETE( pMemoryPool, pReadBuffer );
	}


	void ThreadManager::WaitForTerminate()
	{
		WaitForSingleObject(hSockThreadHandle, INFINITE);
	}

	ThreadManager::~ThreadManager()
	{
		CloseHandle(hSockThreadHandle);
	}



}