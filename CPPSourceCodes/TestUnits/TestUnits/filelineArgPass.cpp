#include "stdafx.h"
#include "TestUnitRepository.h"

DECL_TESTUNIT(fileLineArgPass);

class FLPass
{

public:

	inline void doWrite( const char* file = __FILE__, int line = __LINE__ )
	{
		std::cout << file << "line(" << line << ")" << std::endl;
	}
};

void fileLineArgPass::DoExecute()
{
	FLPass instance;

	//instance.doWrite<__FILE__, __LINE__>();
	instance.doWrite();
	
}
