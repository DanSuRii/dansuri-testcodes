#include "stdafx.h"
#include "TestUnitRepository.h"

DECL_TESTUNIT(arrayType);

typedef int myType[7];

myType n = {3,6,22,5,3,4,1};

void arrayType::DoExecute()
{
	std::for_each(n, n + _countof(n), [](int& nCur){
		std::cout << nCur << std::endl;
	});	  

	return ;
}