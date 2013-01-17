#include "precomp.h"
#include "AuthProviderInterface.h"
#include "AuthProviderGameFlier.h"


namespace NS_AUTHPROVIDERGF
{
	NetCommunicator* NetCommunicator::communicators[COMMUNICATORS_CNT] = {NULL,};		


	template<typename NetCommunicator::eCommunicators eType, class myType>
	void NS_AUTHPROVIDERGF::NetCommunicator::netcomCreate()
	{
		if(eType < 0 || eType >= COMMUNICATORS_CNT)
			return ;

		if(communicators[eType] != NULL)
			return ;

		static myType myInstance;
		communicators[eType] = &myInstance;
	}


	NetCommunicator::NetCommunicator()
	{
		netcomCreate<MIDDLEWARE, MiddlewareServerCommunicator>();
		netcomCreate<PLSS, PLSSServerCommunicator>();
	}

}