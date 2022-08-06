#include <iostream>
#include "Client.h"
int main()
{
    try
    {
        //Client client("Test","dsbgfnghcjhgds.testnet", TypeInp::AUTHORIZATION);
        Client client("Test", "testnet", TypeInp::REGISTRATION);
        if (client.GetError() != nullptr)
            std::cout << client.GetError();
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}