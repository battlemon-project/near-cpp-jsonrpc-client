#include "Connector.h"
#include <thread>
#include <chrono>
#include <string>
#include <boost/bind.hpp>
//#include <boost/thread.hpp>

//void handle_clients_thread()
//{
//
//	while (Client::response != "{}")
//	{
//		boost::beast::flat_buffer buffer;
//		http::response<http::dynamic_body> res;
//		http::read(socket, buffer, res);
//		Client::response = boost::beast::buffers_to_string(res.body().data());
//	}
//}


namespace http = boost::beast::http;

std::string Connector::getResponse(std::string host, std::string port, std::string body)
{
	boost::asio::io_context io;
	boost::asio::ip::tcp::resolver resolver(io);
	boost::asio::ip::tcp::socket socket(io);
	
	boost::asio::connect(socket, resolver.resolve(host, port));
	http::request<http::string_body> req(http::verb::get, body/* "/getId/" + std::string(pubKey)*/, 11);

	req.set(http::field::host, host);
	req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

	std::string response;
	int i = 0;
	//boost::thread(handle_clients_thread);
	do{
		i++;
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000));
		http::write(socket, req);
	
		boost::beast::flat_buffer buffer;
		http::response<http::dynamic_body> res;
		http::read(socket, buffer, res);
	
		response = boost::beast::buffers_to_string(res.body().data());
	} while (response == "{}" && i != 61);
	
	socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	
	if (response == "{}")
		throw std::runtime_error("Invalid response");

	return response;
}