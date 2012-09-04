#define USE_STLPORT
#include "stdafx.h"
#include <stlport/map>
#include <stlport/list>
#include "TestUnitRepository.h"


DECL_TESTUNIT(stlportmapTest);

void stlportmapTest::DoExecute()
{
	std::map<int, int> testMap;

	testMap.insert( std::make_pair(1,1) );

	return ;
}