#pragma once
#include <string>

class Connector
{
public:
	static std::string getResponse(std::string host, std::string port, std::string body);
};

