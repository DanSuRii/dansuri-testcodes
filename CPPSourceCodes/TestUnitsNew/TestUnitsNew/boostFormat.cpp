#include "work.h"
#include <boost/format.hpp>

void WorkImpl<boostFormat>::DoWork()
{
	std::cout << boost::format("%03d") % 1 << std::endl;
}
