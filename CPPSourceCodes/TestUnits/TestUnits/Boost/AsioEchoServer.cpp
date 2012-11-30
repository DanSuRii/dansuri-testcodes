//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "stdafx.h"
#include "TestUnitRepository.h"

#include <process.h>

DECL_TESTUNIT(AsioEchoServer);

using boost::asio::ip::tcp;

#pragma pack(push, 1)

#define IP_ADDRESS_SIZE 10

struct GF_Header
{
	char head[1];
};

struct Request : public GF_Header
{

	char token[100+1];
	__int64 sessionkey;
	//char ip[IP_ADDRESS_SIZE];
};

struct Response : public GF_Header
{
	int result;
	char reason[50+1];
	__int64 sessionkey;

	Response(): result(1)
	{ head[0] = 's'; }
};

#pragma pack(pop)

class session
{
public:
	session(boost::asio::io_service& io_service)
		: socket_(io_service)
	{
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		socket_.async_read_some(boost::asio::buffer(data_, max_length),
			boost::bind(&session::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

private:
	void handle_read(const boost::system::error_code& error,
		size_t bytes_transferred)
	{
		Request* ptrRequest = (Request*)data_;

		Response rs;
		rs.sessionkey = ptrRequest->sessionkey;
		if (!error)
		{
			boost::asio::async_write(socket_,
				//boost::asio::buffer(data_, bytes_transferred),
				boost::asio::buffer(&rs, sizeof(rs)),
				boost::bind(&session::handle_write, this,
				boost::asio::placeholders::error));
		}
		else
		{
			delete this;
		}
	}

	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			socket_.async_read_some(boost::asio::buffer(data_, max_length),
				boost::bind(&session::handle_read, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			delete this;
		}
	}

	tcp::socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];
};

class server
{
public:
	server(boost::asio::io_service& io_service, short port)
		: io_service_(io_service),
		acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
	{
		start_accept();
	}

private:
	void start_accept()
	{
		session* new_session = new session(io_service_);
		acceptor_.async_accept(new_session->socket(),
			boost::bind(&server::handle_accept, this, new_session,
			boost::asio::placeholders::error));
	}

	void handle_accept(session* new_session,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			new_session->start();
		}
		else
		{
			delete new_session;
		}

		start_accept();
	}

	boost::asio::io_service& io_service_;
	tcp::acceptor acceptor_;
};

static void WorkerThread( void* ptrIoService )
{
	enum{
		STATE_RUNNING,
		STATE_STOP
	}curState(STATE_RUNNING);
	char cLastInput;
	boost::asio::io_service& ioService = *((boost::asio::io_service*)ptrIoService);
	
	while(curState == STATE_RUNNING)
	{
		std::cout << "Input Command(q is quiet):";
		std::cin >> cLastInput;

		switch(cLastInput)
		{
		case 'q':
			/*g_Status = STATUS_END;
			CIoServiceRepo::GetInstance().stop();*/
			ioService.stop();
			curState = STATE_STOP;
			break;
		}

		std::cout << std::endl;

		__int64 ;
	} 
}


void AsioEchoServer::DoExecute()
{
	int argc = 2;
	char* argv[2] = {"", "5063"};
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: async_tcp_echo_server <port>\n";
			return ;
		}

		boost::asio::io_service io_service;

		using namespace std; // For atoi.
		server s(io_service, atoi(argv[1]));
		
		_beginthread( WorkerThread, 0, &io_service );

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return ;

}

