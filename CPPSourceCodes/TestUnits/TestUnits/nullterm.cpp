#include "stdafx.h"
#include "TestUnitRepository.h"


template<size_t sz>
bool IsNullterminated( const char (&targetChar)[sz] )
{
	const char* pCur = &targetChar[sz-1];
	do
	{
		if(*pCur == '\0') return true;
	}while((pCur--) != targetChar );

		return false;
}


DECL_TESTUNIT(NullTerm);

void NullTerm::DoExecute()
{
	char myChar[10];
	memset(myChar, 0xff, sizeof(myChar));
	myChar[0] = '1';
	myChar[1] = '2';
	myChar[2] = '3';

	IsNullterminated(myChar);

	return ;
}
