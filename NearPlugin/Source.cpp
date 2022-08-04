#include <iostream>
#include "Client.h"
int main()
{
    try
    {
        //Client client("testnet", TypeInp::AUTHORIZATION);
        Client client("", "dsbgfnghcjhgds.testnet", TypeInp::REGISTRATION);
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