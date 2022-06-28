#pragma execution_character_set("utf-8")
#include "Client.h"
#include <exception>
#include <iostream>

int main() 
{

    try
    {
        Client client("testnet", TypeInp::REGISTRATION);
        int res = client.VerifySing();
        std::cout << "res: " << res << std::endl;
        if(client.GetError() != nullptr)
            std::cout << "GetError: " << client.GetError() << std::endl;

    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

