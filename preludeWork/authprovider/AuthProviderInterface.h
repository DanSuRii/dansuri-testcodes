#ifndef __AUTHPROVIDERINTERFACE_H__
#define __AUTHPROVIDERINTERFACE_H__

namespace NS_AUTHNET
{
	class AuthSock
	{
		SOCKET hSocket;
		WSAEVENT hEvent;
	};

	class ThreadManager
	{
		HANDLE hSocketHandle;

	public:
		void WaitForTerminate();
	};
}

interface AuthProvider
{
	virtual void RequestLogin() = 0;
};

#endif // __AUTHPROVIDERINTERFACE_H__