#include <vector>

#include "Work.h"

void WorkImpl<workOne>::DoWork()
{

}

void WorkImpl<workTwo>::DoWork()
{

}

void WorkImpl<vectorPushBack>::DoWork()
{
	int temp[] = {13, 26, 32, 56, 77, 63, 18};
	std::vector<int> myInts(temp, temp + _countof(temp));

	size_t repeatCnt = 700;
	while(repeatCnt-- != 0)
	{
		myInts.push_back( rand() );
	}
}
