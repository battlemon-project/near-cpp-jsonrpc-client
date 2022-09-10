#include <iostream>
#include "Client.h"
int main()
{
    try
    {
        //Client client("J:\\source\\GitRepos\\battlemon-project\\near-cpp-jsonrpc-client\\NearPlugin\\data\\","testnet", TypeInp::REGISTRATION);
        Client client("J:\\source\\GitRepos\\battlemon-project\\near-cpp-jsonrpc-client\\NearPlugin\\data\\", "dsbgfnghcjhgds.testnet", TypeInp::AUTHORIZATION);
        if (client.GetError() != nullptr)
            std::cout << client.GetError();

        const char* nft_ids[] = {"nft_ids_1", "nft_ids_2", "nft_ids_3", "nft_ids_4" };
        client.gRPC_SetMyItems("321", 4, nft_ids);
        const char* near_ids[] = { "dsbgfnghcjhgds.testnet"};
        PlayerItemsClient pic;
        client.gRPC_getPlayerItems("321", 1, near_ids, pic);
        for (size_t i = 0; i < pic.players_items_size; i++)
            for (size_t j = 0; j < pic.nft_ids_size; j++)
                std::cout << pic.items[i][j] << std::endl;

        ItemsList il = client.gRPC_GetItems();
        for (size_t i = 0; i < il.size; i++)
        {
            std::cout << il.getItem(i).outfit.flavour;
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}