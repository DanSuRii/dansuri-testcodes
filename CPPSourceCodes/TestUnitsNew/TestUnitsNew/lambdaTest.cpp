#include "stdafx.h"
#include "Work.h"
#include <algorithm>

void WorkImpl<lambdaTest>::DoWork()
{
	int values[] = {3,4,5,1,2,3,7,8,9};

	int aParam = 7;
	
	auto LambdaOne = [](int& lhs)->void{ lhs += 1; };
	auto LambdaTwo = [](int& lhs)->void{ lhs += 2; };
	auto LambdaParam = [&aParam](int& lhs)->void{ lhs += aParam; };

	std::for_each( values, values + _countof(values), LambdaOne );
	std::for_each( values, values + _countof(values), LambdaTwo );
	std::for_each( values, values + _countof(values), LambdaParam );
}
