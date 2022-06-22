#pragma execution_character_set("utf-8")
#include "Client.h"
#include <exception>
#include <iostream>

int main() 
{
    try
    {
        Client client("dsbgfnghcjhgds", "testnet");
        std::cout << "GetAccount: " << client.GetAccount() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

