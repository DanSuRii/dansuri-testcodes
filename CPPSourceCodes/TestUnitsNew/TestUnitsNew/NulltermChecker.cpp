#include "stdafx.h"
#include "NulltermChecker.h"
#include <algorithm>


template<class _InIt, class _Ty>
_InIt NULLTERM_CHECKER::FindWrapper( _InIt _First, _InIt _Last, const _Ty& _Val )
{
	return std::find( _First, _Last, _Val );
}



void NULLTERM_CHECKER::Create()
{
	NULLTERM_CHECKER _INstance;
	char myArr[10];
	FindWrapper<char*, char>( myArr, myArr + _countof(myArr), 0 );
	wchar_t SecondArr[10];
	FindWrapper<wchar_t*, wchar_t>( SecondArr, SecondArr + _countof(SecondArr), 0x0000 );


}


/*
void fakeFunction()
{
	NULLTERM_CHECKER _Instance;
	_Instance.Create();
}*/