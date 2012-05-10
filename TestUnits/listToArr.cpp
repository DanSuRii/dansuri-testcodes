#include "stdafx.h"
#include "TestUnitRepository.h"


/*
class ListToArray : public TestUnit
{
public:

	virtual void DoExecute();
};	
*/

template<typename T>
inline T MIN(T param1, T param2)
{
	return (param1 < param2) ? param1 : param2;
}


DECL_TESTUNIT(ListToArray);

void ListToArray::DoExecute()
{
	int intExport[7] = {0,};
	std::list<int> myList(4, 100);

	int nLen = MIN( _countof(intExport), myList.size() );
	std::copy( myList.begin() , myList.end() , intExport);	

	return ;
}

