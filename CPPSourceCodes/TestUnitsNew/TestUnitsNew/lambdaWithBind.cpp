#include "stdafx.h"
#include "Work.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

enum eHostSwitch
{
	eHostSwitch_ClientAttach = 0,
	eHostSwitch_Dedicated,


	eHostSwitch_Cnt,
	eHostSwitch_Default = eHostSwitch_ClientAttach,
};

enum eHostDedicateSwitch
{
	eHostDedicateSwitch_AddPool,
	eHostDedicateSwitch_DirectGS
};

class HostRuntimeConfiguration
{
public:
	union{
		unsigned int m_Config;

		struct
		{
			eHostSwitch hostSwitch:16;
			eHostDedicateSwitch dedicatedSwitch:16;
		};

	};
};


void WorkImpl<lambdaWithBind>::DoWork()
{
	bool bFlag = false;
	struct{
		const char* szArgChar;
		//boost::function< void (void) > fn;
		std::function< void(void) > fn;
	}
	argList[] ={
		//{},
		//{"-retail", boost::bind( [](bool& bFlag)->void{ bFlag = true; }, bFlag )},
		{"-retail", []()->void{ } },
	};

/*
	boost::function<void( HostRuntimeConfiguration& )> f = (_1.m_Config = 3);
*/

	HostRuntimeConfiguration runtimeConfiguration;
	runtimeConfiguration.dedicatedSwitch = eHostDedicateSwitch_DirectGS;
	runtimeConfiguration.hostSwitch = eHostSwitch_Dedicated;

/*
	f(runtimeConfiguration);
*/
}
