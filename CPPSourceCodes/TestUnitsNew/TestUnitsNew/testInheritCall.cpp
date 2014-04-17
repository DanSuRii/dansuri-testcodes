#include "stdafx.h"
#include <iostream>

#include "Work.h"

class baseA
{
public:

	virtual void Foo(){
		std::cout << __FUNCTION__ << std::endl;
	}
};

class B : public baseA
{
public:
	virtual void Foo(){
		std::cout << __FUNCTION__ << std::endl;
	}
};

class C : public B
{
public:
};

void WorkImpl<testInheritCall>::DoWork()
{

	C myWork;
	myWork.Foo();

	baseA myWorkSecond;
	myWorkSecond.Foo();

}
