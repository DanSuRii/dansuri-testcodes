#include "Work.h"
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/detail/interlocked.hpp>
#include <boost/lexical_cast.hpp>

class SharedObj
{
	int _ID;
public:
	SharedObj()
	{
		static volatile long inc;
		_ID = BOOST_INTERLOCKED_INCREMENT( &inc );
	}
	virtual ~SharedObj()
	{			
	}
};

class SharedObjUsr
{
public:
	boost::shared_ptr<SharedObj> pObj;
};

void WorkImpl<sharedPointer>::DoWork()
{
	std::list<boost::shared_ptr<SharedObj>> objContainer;

	//boost::shared_ptr<SharedObj> aObj(new SharedObj);

	int i = 0;
	while( ++i < 255 )
		objContainer.push_back( boost::shared_ptr<SharedObj>(new SharedObj) );

	
}