#include "GRPC_Client.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>



SendCodeResponse gRPC_ClientAuth::CallRPCSendCode(const std::string& publicKey, HOOK_ERROR)
{
    SendCodeRequest write;
    SendCodeResponse read;
    write.set_public_key(publicKey);
    if(GetOneCode(write, &read))
    {
        errorH(this->error, error);
    }
    return read;
}

VerifyCodeResponse gRPC_ClientAuth::CallRPCVerifyCode(const std::string& publicKey, const std::string& sign, HOOK_ERROR)
{
    VerifyCodeRequest write;
    VerifyCodeResponse read;
    write.set_public_key(publicKey);
    write.set_sign(sign);
    if (GetOneVerify(write, &read))
    {
        errorH(this->error, error);
    }

    return read;
}
bool gRPC_ClientAuth::GetOneCode(const SendCodeRequest& write, SendCodeResponse* read)
{
    ClientContext context;
    Status status = stub->SendCode(&context, write, read);

    if (status.ok())
    {
        return status.ok();
    }
    error = status.error_message();
    return status.ok();
}

bool gRPC_ClientAuth::GetOneVerify(const VerifyCodeRequest& write, VerifyCodeResponse* read)
{
    ClientContext context;
    Status status = stub->VerifyCode(&context, write, read);


    if (status.ok())
    {
        return status.ok();
    }
    error = status.error_message();
    return status.ok();
}

bool gRPC_ClientItems::GetOnePlayersItems(const PlayersItemsRequest& write, PlayersItemsResponse* read)
{
    ClientContext context;
    Status status = stub->GetPlayersItems(&context, write, read);


    if (status.ok())
    {
        return status.ok();
    }
    error = status.error_message();
    return status.ok();
}

bool gRPC_ClientItems::GetOneSetMyItems(SetMyItemsRequest& write, Empty* read)
{
    ClientContext context;
    Status status = stub->SetMyItems(&context, write, read);


    if (status.ok())
    {
        return status.ok();
    }
    error = status.error_message();
    return status.ok();
}


PlayerItems gRPC_ClientItems::CallRPC_GetPlayersItems(const std::string& room_id, int id_Player, const std::string& near_ids, HOOK_ERROR)
{
    PlayersItemsRequest write;
    PlayersItemsResponse read;
    write.set_room_id(room_id);
    write.set_near_ids(id_Player, near_ids);
    if (GetOnePlayersItems(write, &read))
    {
        errorH(this->error, error);
    }

    return read.players_items().Get(0);
}

void gRPC_ClientItems::CallRPC_SetMyItems(const std::string& room_id, int id_Player, const std::string& nft_ids, HOOK_ERROR)
{
    SetMyItemsRequest write;
    Empty read;
    write.set_room_id(room_id);
    write.set_nft_ids(id_Player, nft_ids);
    if (GetOneSetMyItems(write, &read))
    {
        errorH(this->error, error);
    }
}
