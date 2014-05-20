#include "stdafx.h"
#include "Work.h"
#include <iostream>

#include <boost/function.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>


enum EMYFunctions
{
	MyFunctions_None,
	MyFunctions_FIRSTFUNC,
	MyFunctions_SECONDFUNC,
	MyFunctions_THIRDFUNC,
	//MyFunctions_FORTH_FUNC,

	MyFunctions_CNT
};

template<EMYFunctions>void MyFunc();//{ std::cout << "InvalidFunc" << std::endl; }

template<> void MyFunc<MyFunctions_FIRSTFUNC>(){	std::cout << "FirstFunc" << std::endl; }
template<> void MyFunc<MyFunctions_SECONDFUNC>(){	std::cout << "SecondFunc" << std::endl; }
template<> void MyFunc<MyFunctions_THIRDFUNC>(){	std::cout << "ThirdFunc" << std::endl; }

boost::function< void(void) > Functions[MyFunctions_CNT];

class FNRegister
{
public:
	template<class T>
	void operator()(T a) const
	{
		Functions[T::value] = MyFunc<T::value>;
	}
};

void WorkImpl<IterateRegister>::DoWork()
{
	typedef boost::mpl::range_c< EMYFunctions, MyFunctions_None, MyFunctions_CNT > FnRange;
	boost::mpl::for_each<FnRange>( FNRegister() );
}
