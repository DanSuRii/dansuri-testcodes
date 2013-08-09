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
			std::cout << boundCount << "." << pWork->GetMyName();
			
			++boundCount;

			if(0 == boundCount % 3)
			{
				std::cout << std::endl;
			}
			else
			{
				std::cout.width(20);
			}
		}
	);

	std::cout << std::endl << "Select Menu(q to Exit): ";
}

WorkBase* WorkRepository::GetWork(int idx)
{
	WorkBase* pRet = nullptr;
	
	if( (0 <= idx )
		&& (idx < _countof(pWorks)) )
		pRet = pWorks[idx];

	return pRet;
}

bool WorkRepository::MenuSelect()
{
	bool bReturnRepeat = true;
	char selectKey[10] = {'\0',};

	std::cin >> selectKey;

	selectKey[ _countof(selectKey)-1 ] = '\0';

	if('q' == selectKey[0])
		bReturnRepeat = false;
	else
	{
		int aSelected = atoi( selectKey );
		WorkBase* pWork = GetWork(aSelected);
		if(pWork)
			pWork->DoWork();
	}

	std::cout << std::endl;
	return bReturnRepeat;
}
void WorkRepository::DoLoop()
{
	do
	{
		PrintMenu();
	}while(MenuSelect());
}



