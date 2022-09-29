#include <iostream>
#include "Client.h"
#include <thread>
#include <ctime>

#define REDIRECT "https://api.battlemon.com/rest/contracts"

int main()
{
    try
    {
        Client client("J:\\source\\GitRepos\\battlemon-project\\near-cpp-jsonrpc-client\\NearPlugin\\data\\", "testnet", TypeInp::REGISTRATION);

        std::string url = std::string("https://wallet.") + std::string("testnet") + ".near.org/login?title=rndname^&success_url=" + REDIRECT + "^&public_key=" + client.GetPublicKey();

        std::string cmdComand = "start " + url;

        system(cmdComand.c_str());
        std::this_thread::sleep_for(std::chrono::nanoseconds(15000000000));

        //Client client("J:\\source\\GitRepos\\battlemon-project\\near-cpp-jsonrpc-client\\NearPlugin\\data\\", "dsbgfnghcjhgds.testnet", TypeInp::AUTHORIZATION);

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