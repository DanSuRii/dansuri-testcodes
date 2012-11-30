#include "stdafx.h"
#include "TestUnitRepository.h"

DECL_TESTUNIT(stringTest);

void stringTest::DoExecute()
{
	char testChar[256] = {'a', 'b'};

	std::string strTest(testChar, _countof(testChar));

	std::cout << strTest.c_str() << std::endl;

	return ;
}