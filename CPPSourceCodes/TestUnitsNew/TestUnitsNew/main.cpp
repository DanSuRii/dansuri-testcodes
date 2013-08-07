#include <cstdlib>
#include "Work.h"

int main(void)
{
	WorkRepository::GetInstance().DoLoop();

	system("pause");
	
	return 0;
}