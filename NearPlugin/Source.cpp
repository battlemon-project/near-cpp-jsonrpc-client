#include <iostream>
#include "include/Client.h"
#include "include/gRPCResponse.h"
#include <thread>
#include <ctime>
#include "protocol/updates.grpc.pb.h"


#define REDIRECT "https://api.battlemon.com/rest/contracts"
static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}


int main()
{
    try
    {
        Client* client = new Client(u"J:\\source\\GitRepos\\battlemon-project\\near-cpp-jsonrpc-client\\NearPlugin\\data\\", u"dsbgfnghcjhgds.testnet", Type_Call_gRPC::Type_gRPC_Auth::AUTHORIZATION);
        if (client->AuthServiceClient(u"game.battlemon.com"))
        {


            gRPC_ResponseItem* gRPC_Item = new gRPC_ResponseItem(&client, nullptr, true, u"game.battlemon.com", Type_Call_gRPC::Type_gRPCItem::GET_ITEMS);

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