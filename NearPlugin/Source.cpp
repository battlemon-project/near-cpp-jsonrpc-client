#pragma execution_character_set("utf-8")
#include "Client.h"
#include <exception>
#include <iostream>

int main() 
{

    try
    {
        //Client client("testnet");
        //if (client.error != nullptr)
        //{
        //    std::cout << client.error << std::endl;
        //}
        //std::cout << "GetAccount: " << client.GetPublicKey() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

