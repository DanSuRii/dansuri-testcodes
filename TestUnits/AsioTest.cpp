#include <boost/asio.hpp>
#include "stdafx.h"
#include "TestUnitRepository.h"

DECL_TESTUNIT(AsioTest);

void AsioTest::DoExecute()
{
	boost::asio::io_service io;

	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));

	t.wait();

	std::cout << "Waiting Over" << std::endl;

	return ;
}

DECL_TESTUNIT(AsioTest2);

void print(const boost::system::error_code& /*e*/)
{
	std::cout << "Async Waiting Over" << std::endl;
}

void AsioTest2::DoExecute()
{
	boost::asio::io_service io;

	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));

	t.async_wait(&print);

	io.run();

	return ;
}