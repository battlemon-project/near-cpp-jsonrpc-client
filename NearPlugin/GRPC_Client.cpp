#include "GRPC_Client.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

//#define CALL_gRPC
//#define CALL_getStatus Status status = CALL_gRPC;if (status.ok()){return false;}error = status.error_message();return true;

/*  
*   bool getStatus(const ClientContext &context, const Request& write, Response* read)
*        {
*           Status status = stub->gRPC(&context, write, read);  gRPC ===> CALL_gRPC
*
*           if (status.ok())
*           {
*               return false;
*           }
*           error = status.error_message();
*           return true;
*       }
*/


/*
#define CALL_gRPC stub->SendCode(&context, write, read)
bool gRPC_ClientItems::CallRPC(ClientContext& context, const SendCodeRequest& write, SendCodeResponse* read)
{
    CALL_getStatus
}

#define CALL_gRPC stub->VerifyCode(&context, write, read)
bool gRPC_ClientItems::CallRPC(ClientContext& context, const VerifyCodeRequest& write, VerifyCodeResponse* read)
{
    CALL_getStatus
}

*/


template <typename ServiceStub, typename Request, typename Response>
bool CallRPC(ServiceStub* stub, ClientContext& context, const Request & write, Response* read, std::string& error, Status(ServiceStub::*gRPC_ptr)(ClientContext* context, const Request& , Response* ))
{
    Status status = (stub->*gRPC_ptr)(&context, write, read);
    if (status.ok())
    {
        return false;
    }
    error = status.error_message();
    return true;
}


SendCodeResponse gRPC_ClientAuth::CallRPCSendCode(const std::string& publicKey, HOOK_ERROR)
{
    SendCodeRequest write;
    SendCodeResponse read;
    write.set_public_key(publicKey);

    ClientContext context;
    if (CallRPC<AuthService::Stub, SendCodeRequest, SendCodeResponse>(stub.get(), context, write, &read, this->error, &AuthService::Stub::SendCode))
    {
        errorH(this->error, error);
    }

    /*if (CallRPC(context, write, &read))
    {
        errorH(this->error, error);
    }*/

    return read;
}

VerifyCodeResponse gRPC_ClientAuth::CallRPCVerifyCode(const std::string& publicKey, const std::string& sign, HOOK_ERROR)
{
    VerifyCodeRequest write;
    VerifyCodeResponse read;
    write.set_public_key(publicKey);
    write.set_sign(sign);

    ClientContext context;
    if (CallRPC<AuthService::Stub, VerifyCodeRequest, VerifyCodeResponse>(stub.get(), context, write, &read, this->error, &AuthService::Stub::VerifyCode))
    {
        errorH(this->error, error);
    }

    return read;
}




//        gRPC_ClientItems



ItemsResponse gRPC_ClientItems::CallRPC_GetItems(const std::string& nearID, const std::string& sign, HOOK_ERROR)
{
    ItemsRequest write;
    ItemsResponse read;

    std::string meta[] = { "nearid" , "sign" };
    std::string value[] = { nearID , sign };
    ClientContext context;
    CreateContext(context, meta, value, 2);

    if (CallRPC<ItemsService::Stub, ItemsRequest, ItemsResponse>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::GetItems))
    {
        errorH(this->error, error);
    }
    return read;
}


GetBundlesResponse gRPC_ClientItems::CallRPC_GetBundles(const std::string& nearID, const std::string& sign, HOOK_ERROR)
{
    GetBundlesRequest write;
    GetBundlesResponse read;

    std::string meta[] = { "nearid" , "sign" };
    std::string value[] = { nearID , sign };
    ClientContext context;
    CreateContext(context, meta, value, 2);

    if (CallRPC<ItemsService::Stub, GetBundlesRequest, GetBundlesResponse>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::GetBundles))
    {
        errorH(this->error, error);
    }
    return read;
}

WeaponBundle gRPC_ClientItems::CallRPC_CreateBundle(const std::string& nearID, const std::string& sign, HOOK_ERROR)
{
    CreateBundleRequest write;
    WeaponBundle read;

    std::string meta[] = { "nearid" , "sign" };
    std::string value[] = { nearID , sign };
    ClientContext context;
    CreateContext(context, meta, value, 2);
    
    if (CallRPC<ItemsService::Stub, CreateBundleRequest, WeaponBundle>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::CreateBundle))
    {
        errorH(this->error, error);
    }
    return read;
}

WeaponBundle gRPC_ClientItems::CallRPC_EditBundle(const std::string& bundle_id, HOOK_ERROR)
{
    EditBundleRequest write;
    WeaponBundle read;
    write.set_bundle_id(bundle_id);

    ClientContext context;

    if (CallRPC<ItemsService::Stub, EditBundleRequest, WeaponBundle>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::EditBundle))
    {
        errorH(this->error, error);
    }

    return read;
}

bool gRPC_ClientItems::CallRPC_RemoveBundle(const std::string& bundle_id, HOOK_ERROR)
{
    RemoveBundleRequest write; 
    Empty read;
    write.set_bundle_id(bundle_id);

    ClientContext context;
    if (CallRPC<ItemsService::Stub, RemoveBundleRequest, Empty>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::RemoveBundle))
    {
        errorH(this->error, error);
        return false;
    }

    return true;
}

bool gRPC_ClientItems::CallRPC_AttachBundle(const std::string& bundle_id, const std::string& lemon_id, HOOK_ERROR)
{
    AttachBundleRequest write;
    Empty read;
    write.set_bundle_id(bundle_id);
    write.set_lemon_id(lemon_id);

    ClientContext context;
    if (CallRPC<ItemsService::Stub, AttachBundleRequest, Empty>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::AttachBundle))
    {
        errorH(this->error, error);
        return false;
    }

    return true;
}

bool gRPC_ClientItems::CallRPC_DetachBundle(const std::string& bundle_id, const std::string& lemon_id, HOOK_ERROR)
{
    DetachBundleRequest write;
    Empty read;
    write.set_bundle_id(bundle_id);
    write.set_lemon_id(lemon_id);

    ClientContext context;
    if (CallRPC<ItemsService::Stub, DetachBundleRequest, Empty>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::DetachBundle))
    {
        errorH(this->error, error);
        return false;
    }

    return true;
}



//        gRPC_ClientMM



SearchGameResponse gRPC_ClientMM::CallRPC_SearchGame(const int& MatchType, const int& MatchMode, HOOK_ERROR)
{
    SearchGameRequest write;
    SearchGameResponse read;

    GameMode gameMode;
    game::battlemon::mm::MatchMode value;
    
    switch (MatchType)
    {
    case 0:
        gameMode.set_match_type(game::battlemon::mm::MatchType::DEATH_MATCH);
        break;
    case 1:
        gameMode.set_match_type(game::battlemon::mm::MatchType::CATCH_THE_FLAG);
        break;
    }

    switch (MatchMode)
    {
    case 0:
        gameMode.set_match_mode(game::battlemon::mm::MatchMode::NONE);
        break;
    case 1:
        gameMode.set_match_mode(game::battlemon::mm::MatchMode::EQUIPMENT);
        break;
    case 2:
        gameMode.set_match_mode(game::battlemon::mm::MatchMode::REALISM);
        break;
    }
    write.set_allocated_game_mode(&gameMode);

    ClientContext context;

    if (CallRPC<MMService::Stub, SearchGameRequest, SearchGameResponse>(stub.get(), context, write, &read, this->error, &MMService::Stub::SearchGame))
    {
        errorH(this->error, error);
    }

    return read;
}

bool gRPC_ClientMM::CallRPC_AcceptGame(const std::string& lemon_id, HOOK_ERROR)
{
    AcceptGameRequest write;
    Empty read;

    write.set_lemon_id(lemon_id);
    ClientContext context;

    if (CallRPC<MMService::Stub, AcceptGameRequest, Empty>(stub.get(), context, write, &read, this->error, &MMService::Stub::AcceptGame))
    {
        errorH(this->error, error);
        return false;
    }

    return true;
}

bool gRPC_ClientMM::CallRPC_CancelSearch(const std::string& nearID, const std::string& sign, HOOK_ERROR)
{
    CancelSearchRequest write;
    Empty read;

    std::string meta[] = { "nearid" , "sign" };
    std::string value[] = { nearID , sign };
    ClientContext context;
    CreateContext(context, meta, value, 2);

    if (CallRPC<MMService::Stub, CancelSearchRequest, Empty>(stub.get(), context, write, &read, this->error, &MMService::Stub::CancelSearch))
    {
        errorH(this->error, error);
        return false;
    }

    return true;
}
