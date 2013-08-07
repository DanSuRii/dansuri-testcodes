#include <iostream>
#include <algorithm>

#include "Work.h"

void WorkRepository::PrintMenu()
{
	static const char* menuPrefix[] =
	{
		"==========================================",
		"SELECT MENU",
		"==========================================",
	};	
	
	std::for_each( menuPrefix, menuPrefix + _countof(menuPrefix),
		[](const char* pCurrentLine)
		{
			std::cout << pCurrentLine << std::endl;			
		}	
	);

	int boundCount = 0;

	std::for_each( pWorks, pWorks + _countof(pWorks), 
		[&boundCount](WorkBase* pWork)
		{
			++boundCount;

			std::cout << pWork->GetMyName();

			if(0 == boundCount % 3)
			{
				std::cout << std::endl;
			}
			else
			{
				std::cout.width(10);
			}
		}
	);

	std::cout << std::endl;
}

bool WorkRepository::MenuSelect()
{
	bool bReturnRepeat = true;
	char selectKey[10] = {'\0',};

	std::cin >> selectKey;



	return bReturnRepeat;
}
void WorkRepository::DoLoop()
{
	do
	{
		PrintMenu();
	}while(MenuSelect());
}



