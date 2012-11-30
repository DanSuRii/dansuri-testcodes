#include "stdafx.h"
#include "TestUnitRepository.h"
#include <algorithm>

DECL_TESTUNIT(LambdaTest);


void LambdaTest::DoExecute()
{
	int values[32] = {0, };
	int increaseCnt = 0;
	//int tmp;
	std::for_each( values, values + _countof(values), [&increaseCnt]( int& i ){ i = ++increaseCnt; } );
}