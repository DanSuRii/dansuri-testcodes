#ifndef __AUTHPROVIDERGAMEFLIER_H__
#define __AUTHPROVIDERGAMEFLIER_H__

#include "AuthProviderInterface.h"


namespace NS_AUTHPROVIDERGF
{	
	class SessionManager
	{

	};

	class NetCommunicator
	{
		enum eCommunicators
		{
			MIDDLEWARE,
			PLSS,
			COMMUNICATORS_CNT
		};

		static NetCommunicator* communicators[];

		template<eCommunicators, class myType>	void netcomCreate();

	public:
		NetCommunicator();

	};


	class MiddlewareServerCommunicator : public NetCommunicator{};
	class PLSSServerCommunicator : public NetCommunicator{};

	class AuthProviderGF : public AuthProvider
	{
		SessionManager sessionManager;

		NetCommunicator communicator;

		
		NS_AUTHNET::ThreadManager threadManager;
		NS_AUTHNET::AuthSock	authSocket;

	public:
		void RequestLogin(){}
	};

}

#endif // __AUTHPROVIDERGAMEFLIER_H__