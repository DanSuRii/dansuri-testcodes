#include "Work.h"

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

void WorkImpl<boostSmartPointers>::DoWork()
{
	boost::scoped_ptr<char*>;
	boost::scoped_array<char*>;
	boost::shared_ptr<char*>;
	boost::shared_array<char*>;
	boost::weak_ptr<char*>;
	boost::intrusive_ptr<char*>;
}
