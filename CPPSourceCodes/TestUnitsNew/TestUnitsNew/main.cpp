#include "stdafx.h"
#include <cstdlib>
#include "Work.h"

#include "test.h"

int main(void)
{
	//PerfAutoInitialize();

	WorkRepository::GetInstance().DoLoop();

	//PerfAutoCleanup();
	
	system("pause");

	return 0;
}