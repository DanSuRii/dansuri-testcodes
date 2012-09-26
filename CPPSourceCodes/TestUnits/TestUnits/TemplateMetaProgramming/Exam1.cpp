#include <boost/type_traits.hpp>
#include "stdafx.h"
#include "TestUnitRepository.h"

DECL_TESTUNIT(Exam1);


template<typename T, bool> struct type_check_traits;

template<class T>
struct add_const_ref
{
/*
	typedef typename T const& type;	
	enum {value = boost::is_reference<T>::value}; 
*/
	typedef typename type_check_traits<T, boost::is_reference<typename T>::value>::type type;
};

template<class T>
struct type_check_traits<T, false>
{
	typedef typename T const& type;
};

template<class T>
struct type_check_traits<T, true>
{
	typedef typename T type;
};



void Exam1::DoExecute()
{
	int aValue = 10;

	boost::add_reference<int>::type valueRef = aValue;
	add_const_ref<int>::type valueRef2 = aValue;
	add_const_ref<int&>::type valueRef3 = aValue;
	add_const_ref<int*>::type valueRef4 = (&aValue);

/*
	bool myValue = add_const_ref<int>::value;
	bool myValue2 = add_const_ref<int&>::value;

*/
	valueRef;
}