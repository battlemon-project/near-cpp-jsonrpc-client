#include <iostream>
#include "Client.h"
#pragma execution_character_set("utf-8")
/*#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
*/
int main()
{
    try
    {
        //Client client("testnet", TypeInp::AUTHORIZATION);
        Client client("dsbgfnghcjhgds.testnet", TypeInp::REGISTRATION);
        if (client.GetError() != nullptr)
            std::cout << client.GetError();
       //auto const host = "0n64i8m4o8.execute-api.us-east-1.amazonaws.com";
       //auto const port = "443";
       //auto const text = "/test/";
       //
       //// The io_context is required for all I/O
       //net::io_context ioc;
       //
       //// These objects perform our I/O
       //tcp::resolver resolver{ ioc };
       //websocket::stream<tcp::socket> ws{ ioc };
       //boost::asio::ip::tcp::resolver::query query(host, port);
       //
       //// Look up the domain name
       //auto const results = resolver.resolve(query);
       //std::string str = query.host_name();
       //net::connect(ws.next_layer(), results.begin(), results.end());
       //
       //// Set a decorator to change the User-Agent of the handshake
       //ws.set_option(websocket::stream_base::decorator(
       //    [](websocket::request_type& req)
       //    {
       //        req.set(http::field::user_agent,
       //            std::string(BOOST_BEAST_VERSION_STRING) +
       //            " websocket-client-coro");
       //    }));
       //
       //ws.handshake(str, "/test");
       //
       //// Send the message
       ////ws.write(net::buffer(std::string(text)));
       //
       //// This buffer will hold the incoming message
       //beast::flat_buffer buffer;
       //
       //// Read a message into our buffer
       //ws.read(buffer);
       //
       //// Close the WebSocket connection
       //ws.close(websocket::close_code::normal);
       //
       //// If we get here then the connection is closed gracefully
       //
       //// The make_printable() function helps print a ConstBufferSequence
       //std::cout << beast::make_printable(buffer.data()) << std::endl;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}