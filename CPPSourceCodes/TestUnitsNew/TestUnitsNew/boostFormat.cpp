#include "stdafx.h"

#include "work.h"
#include <iostream>
#include <boost/format.hpp>

void WorkImpl<boostFormat>::DoWork()
{
	std::cout << boost::format("%03d") % 1 << std::endl;
}
