#include <iostream>
#include "include/Client.h"
#include "include/gRPCResponse.h"
#include <thread>
#include <ctime>

#define REDIRECT "https://api.battlemon.com/rest/contracts"

int main()
{
    try
    {
        Client* client = new Client("J:\\source\\GitRepos\\battlemon-project\\near-cpp-jsonrpc-client\\NearPlugin\\data\\", "rasta42.testnet", Type_Call_gRPC::Type_gRPC_Auth::AUTHORIZATION);
        if (client->AuthServiceClient("game.battlemon.com"))
        {



            gRPC_ResponseItem* gRPC_Item = new gRPC_ResponseItem(&client, nullptr, "game.battlemon.com", Type_Call_gRPC::Type_gRPCItem::GET_ITEMS);

            ObjectList<ModelItems::WeaponBundle> IA;
            IA = gRPC_Item->gRPC_GetBundlesArray();


            gRPC_Item->gRPC_EditBundle();
            ModelItems::WeaponBundle* wb = IA.getObjectPtr();

            for (size_t i = 0; i < IA.getSize(); i++)
            {
                //std::cout << "bundle_num " << wb[i].lemon.attached_bundles.getObject(0).bundle_num;
                std::cout << "bundle_num " << wb[i].bundle_num << std::endl;
                std::cout << "level " << wb[i].level << std::endl;
                std::cout << "title " << wb[i].title << std::endl;
                std::cout << "WeaponList.getSize " << wb[i].WeaponList.getSize() << std::endl;
            }
        }
        

        if (client->GetError() != nullptr)
            std::cout << client->GetError();
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}