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
        return false;
    }
    error = status.error_message();
    return true;
}

bool gRPC_ClientAuth::GetOneVerify(const VerifyCodeRequest& write, VerifyCodeResponse* read)
{
    ClientContext context;
    Status status = stub->VerifyCode(&context, write, read);


    if (status.ok())
    {
        return false;
    }
    error = status.error_message();
    return true;
}

bool gRPC_ClientItems::GetOnePlayersItems(const PlayersItemsRequest& write, PlayersItemsResponse* read)
{
    ClientContext context;
    Status status = stub->GetPlayersItems(&context, write, read);


    if (status.ok())
    {
        return false;
    }
    error = status.error_message();
    return true;
}

bool gRPC_ClientItems::GetOneSetMyItems(SetMyItemsRequest& write, Empty* read, const std::string& nearID, const std::string& sign)
{
    ClientContext context;
    context.AddMetadata("nearid", nearID);
    context.AddMetadata("sign", sign);
    Status status = stub->SetMyItems(&context, write, read);


    if (status.ok())
    {
        return false;
    }
    error = status.error_message();
    return true;
}


PlayersItemsResponse gRPC_ClientItems::CallRPC_GetPlayersItems(const std::string& room_id, int index, const std::string* near_ids, HOOK_ERROR)
{
    PlayersItemsRequest write;
    PlayersItemsResponse read;
    write.set_room_id(room_id);
    for (int i = 0; i < index; i++)
    {
        write.add_near_ids(near_ids[i]);
    }
    if (GetOnePlayersItems(write, &read))
    {
        errorH(this->error, error);
    }
    return read;
}

void gRPC_ClientItems::CallRPC_SetMyItems(const std::string& room_id, int index, const std::string* nft_ids, const std::string& nearID, const std::string& sign, HOOK_ERROR)
{
    SetMyItemsRequest write;
    Empty read;
    write.set_room_id(room_id);
    for (int i = 0; i < index; i++)
    {
        write.add_nft_ids(nft_ids[i]);
    }
    if (GetOneSetMyItems(write, &read, nearID, sign))
    {
        errorH(this->error, error);
    }
}
bool gRPC_ClientItems::GetOneItems(const ItemsRequest& write, ItemsResponse* read, const std::string& nearID, const std::string& sign)
{
    ClientContext context;
    context.AddMetadata("nearid", nearID);
    context.AddMetadata("sign", sign);
    Status status = stub->GetItems(&context, write, read);


    if (status.ok())
    {
        return false;
    }
    error = status.error_message();
    return true;
}
ItemsResponse gRPC_ClientItems::CallRPC_GetItems(const std::string& nearID, const std::string& sign, HOOK_ERROR)
{
    ItemsRequest write;
    ItemsResponse read;
    if (GetOneItems(write, &read, nearID, sign))
    {
        errorH(this->error, error);
    }
    return read;
}