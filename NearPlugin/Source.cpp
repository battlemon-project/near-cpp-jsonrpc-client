#include <iostream>
#include "Client.h"
int main()
{
    try
    {
        //Client client("Test","dsbgfnghcjhgds.testnet", TypeInp::AUTHORIZATION);
        Client client("J:\\source\\GitRepos\\battlemon-project\\near-cpp-jsonrpc-client\\NearPlugin\\data", "dsbgfnghcjhgds.testnet", TypeInp::AUTHORIZATION);
        if (client.GetError() != nullptr)
            std::cout << client.GetError();

        const char* nft_ids[] = {"nft_ids_1", "nft_ids_2", "nft_ids_3"};
        client.gRPC_SetMyItems("123", 3, nft_ids);
        const char* near_ids[1] = { "dsbgfnghcjhgds.testnet" };
        PlayerItemsClient pic = client.gRPC_getPlayerItems("123", 1, near_ids);
        std::cout << pic.near_id;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}