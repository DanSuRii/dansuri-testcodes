#include "precomp.h"
#include "AuthProviderGameFlier.h"

void main(void)
{
	char* pszAddress[] = 
	{
		"127.0.0.1:45890",
		"127.0.0.2:55430"
	};
	NS_AUTHPROVIDERGF::NetcomInitializers initStruc(pszAddress);
	NS_AUTHPROVIDERGF::AuthProviderGF instance(initStruc);

	int members[] = {1, 2};
	instance.RequestLogin( makeLoginRequest<APT_GAMEFLIER>( &members[0], &members[1] ) );
	//WSAEventSelect();
}