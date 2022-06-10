#pragma once
#include <string>
#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>

class Connector
{
public:
	static std::string getResponse(std::string host, std::string port, std::string body);
};

