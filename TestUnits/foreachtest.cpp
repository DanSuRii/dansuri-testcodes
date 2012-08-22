#include <boost/foreach.hpp>
#include "stdafx.h"
#include "TestUnitRepository.h"

#define FOREACH(arr, functionPtr) std::for_each(arr, arr+_countof(arr), functionPtr)

DECL_TESTUNIT(foreachTest);

void foreachTestFunc(int& a)
{
	std::cout << a << std::endl;
}

class TestFunc
{
public:
	void operator()(int& a){std::cout << a << std::endl;}
};

void foreachTest::DoExecute()
{
	int a[]={1, 2, 3};

/*
	std::for_each(a, a+_countof(a), foreachTestFunc);

	std::for_each(a, a+_countof(a), TestFunc());	
*/

	FOREACH(a, foreachTestFunc);

	FOREACH(a, TestFunc());

	BOOST_FOREACH(int iA, a)
	{
		if(iA > 1)
			break;

		std::cout << iA << std::endl;
	}

	return ;
}