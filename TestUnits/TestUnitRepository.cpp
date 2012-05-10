#include "stdafx.h"
#include "TestUnitRepository.h"
#include <sstream>


TestUnitRepository::TestUnitRepository()
:curMenuIndex(0)
{
	
}

void TestUnitRepository::DoRegist(TestUnit* pTarget)
{
	InterlockedIncrement(&curMenuIndex);

	menuMap.insert( std::make_pair( curMenuIndex, pTarget ) );
}

void TestUnitRepository::DoExecute()
{
	MENU_MAP::iterator iter = menuMap.begin();
	while(iter != menuMap.end())
	{
		this->PrintMenu();
		iter = menuMap.end();
		iter = this->GetSelection();
		if(iter != menuMap.end())		
			(*iter).second->DoExecute();

		fflush(stdin);
	}
/*
	if(menuMap.begin() != menuMap.end())
		(*menuMap.begin()).second->DoExecute();
*/
}

void TestUnitRepository::PrintMenu()
{
	static char* menuPan[] =
	{
		"********************************************************",
		"*						MENU PAN						*"
		"********************************************************",
	};

	DoPrintCharArr(menuPan, _countof(menuPan));
	
	class PrintMenus
	{
	public:
		void operator()(MENUMAP_PAIR pair)
		{
			std::cout << pair.first << "." << (pair.second->GetMyName()) << std::endl;
		}
	} PrintMenuInstance;

	std::for_each( menuMap.begin(), menuMap.end(), PrintMenuInstance );
}

TestUnitRepository::MENU_MAP::iterator TestUnitRepository::GetSelection()
{
	static char* selectPrint[] = 
	{
		"Press Menu: "
	};
	 

	DoPrintCharArr(selectPrint, _countof(selectPrint));

	//std::stringstream strInput(std::stringstream::out | std::stringstream::in);
	int nSelectedValue = 0;

	std::cin >> nSelectedValue;
	

	return menuMap.find(nSelectedValue);
}

void TestUnitRepository::DoPrintCharArr( char** pTarget, size_t sizeArr )
{
	for(size_t i = 0; i < sizeArr; ++i)
		std::cout << pTarget[i] << std::endl;

}
