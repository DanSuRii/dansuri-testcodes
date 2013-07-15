#include "stdafx.h"
#include "TestUnitRepository.h"

template <typename T, size_t N>
char (&_ArraySizeHelper( T (&array)[N] ))[N];

#define arrCntOf( arr ) (sizeof( _ArraySizeHelper( arr ) ))

template<class T>
class ArrSize;

template<class T, size_t N>
class ArrSize<T[N]>
{
public:
	static const size_t value = N;
};

//#define arrCntOfCls( arr ) ( ArrSize<typeof(arr)>::value ) // GCC
#define arrCntOfCls( arr ) ( ArrSize<decltype(arr)>::value ) // decltype define in C++11




DECL_TESTUNIT(ArrSizeOf);

void MemoryInsert(int* ptrWhere)
{
	(*ptrWhere) = 32;

	return ;
}


void ArrSizeOf::DoExecute()
{



	int MyItems[324];
	std::cout << arrCntOf(MyItems) << std::endl;
	int myItemsDuel[ 2 * arrCntOf(MyItems) ];
	std::cout << arrCntOf(myItemsDuel) << std::endl;

	//_ArraySizeHelper(MyItems);

	std::cout << arrCntOfCls( MyItems ) << std::endl;

	MemoryInsert(2 +MyItems);


	return ;
}