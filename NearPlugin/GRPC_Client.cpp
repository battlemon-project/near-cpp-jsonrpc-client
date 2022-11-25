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

gRPC_ClientAuth::gRPC_ClientAuth(const bool& ssl, std::string url):gRPC_Client(ssl, url)
{

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

gRPC_ClientItems::gRPC_ClientItems(const bool& ssl, std::string url) :gRPC_Client(ssl, url)
{
}

WeaponBundle gRPC_ClientItems::CallRPC_EditBundle(const std::string& nearID, const std::string& sign, ModelItems::EditBundleRequest& request, HOOK_ERROR)
{
    EditBundleRequest write;
    WeaponBundle read;
    write.set_bundle_num(*request.bundle_num);
    write.set_title(TYPE_Conv(request.title));

    for (int i = 0; i < request.items->getSize(); i++)
    {
        game::battlemon::items::WeaponBundleItem* wr = write.add_items();
        wr->set_skin(TYPE_Conv(request.items->getObject(i).skin));
        wr->set_item_type(Helper::ConvWeaponBundleItemTypeToProto(request.items->getObject(i).item_type));
        wr->set_slot_type(Helper::WeaponBundleSlotTypeToProto(request.items->getObject(i).slot_type));
    }

    std::string meta[] = { "nearid" , "sign" };
    std::string value[] = { nearID , sign };
    ClientContext context;
    CreateContext(context, meta, value, 2);
    if (CallRPC<ItemsService::Stub, EditBundleRequest, WeaponBundle>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::EditBundle))
    {
        errorH(this->error, error);
    }
    return read;
}

bool gRPC_ClientItems::CallRPC_AttachBundle(const std::string& nearID, const std::string& sign, ModelItems::AttachBundleRequest& request, HOOK_ERROR)
{
    AttachBundleRequest write;
    Empty read;

    write.set_bundle_num(*request.bundle_num);
    write.set_lemon_id(TYPE_Conv(request.lemon_id));

    std::string meta[] = { "nearid" , "sign" };
    std::string value[] = { nearID , sign };
    ClientContext context;
    CreateContext(context, meta, value, 2);

    if (CallRPC<ItemsService::Stub, AttachBundleRequest, Empty>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::AttachBundle))
    {
        errorH(this->error, error);
        return false;
    }

    return true;
}

bool gRPC_ClientItems::CallRPC_DetachBundle(const std::string& nearID, const std::string& sign, ModelItems::DetachBundleRequest& request, HOOK_ERROR)
{
    DetachBundleRequest write;
    Empty read;

    write.set_bundle_num(*request.bundle_num);
    write.set_lemon_id(TYPE_Conv(request.lemon_id));

    std::string meta[] = { "nearid" , "sign" };
    std::string value[] = { nearID , sign };
    ClientContext context;
    CreateContext(context, meta, value, 2);

    if (CallRPC<ItemsService::Stub, DetachBundleRequest, Empty>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::DetachBundle))
    {
        errorH(this->error, error);
        return false;
    }

    return true;
}



//        gRPC_ClientMM

gRPC_ClientMM::gRPC_ClientMM(const bool& ssl, std::string url):gRPC_Client(ssl, url)
{

}

SearchGameResponse gRPC_ClientMM::CallRPC_SearchGame(const std::string& nearID, const std::string& sign, ModelMM::SearchGameRequest& Request, HOOK_ERROR)
{
    SearchGameRequest write;
    SearchGameResponse read;

    GameMode* gameMode = new GameMode();
    switch (Request.game_mode->match_type)
    {
    case ModelMM::MatchType::DEATH_MATCH:
        gameMode->set_match_type(game::battlemon::mm::MatchType::DEATH_MATCH);
        break;
    case ModelMM::MatchType::CATCH_THE_FLAG:
        gameMode->set_match_type(game::battlemon::mm::MatchType::CATCH_THE_FLAG);
        break;
    default:
        break;
    }

    switch (Request.game_mode->match_mode)
    {
    case ModelMM::MatchMode::NONE:
        gameMode->set_match_mode(game::battlemon::mm::MatchMode::NONE);
        break;
    case ModelMM::MatchMode::EQUIPMENT:
        gameMode->set_match_mode(game::battlemon::mm::MatchMode::EQUIPMENT);
        break;
    case ModelMM::MatchMode::REALISM:
        gameMode->set_match_mode(game::battlemon::mm::MatchMode::REALISM);
        break;
    case ModelMM::MatchMode::DEFAULT:
        gameMode->set_match_mode(game::battlemon::mm::MatchMode::NONE);
        break;
    }
    write.set_allocated_game_mode(gameMode);
    
    std::string meta[] = { "nearid" , "sign" };
    std::string value[] = { nearID , sign };
    ClientContext context;
    CreateContext(context, meta, value, 2);

    if (CallRPC<MMService::Stub, SearchGameRequest, SearchGameResponse>(stub.get(), context, write, &read, this->error, &MMService::Stub::SearchGame))
    {
        errorH(this->error, error);
    }

    return read;
}

bool gRPC_ClientMM::CallRPC_AcceptGame(const std::string& nearID, const std::string& sign, ModelMM::AcceptGameRequest& Request, HOOK_ERROR)
{
    AcceptGameRequest write;
    Empty read;

    write.set_lemon_id(TYPE_Conv(Request.lemon_id));
    
    std::string meta[] = { "nearid" , "sign" };
    std::string value[] = { nearID , sign };
    ClientContext context;
    CreateContext(context, meta, value, 2);

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



// internalMM


gRPC_ClientInternalMM::gRPC_ClientInternalMM(const bool& ssl, std::string url) :gRPC_Client(ssl, url)
{
}

bool gRPC_ClientInternalMM::CallRPC_UserLeftBattle(ModelInternalMM::InternalUserLeftBattleRequest& Request, HOOK_ERROR)
{
    InternalUserLeftBattleRequest write;
    Empty read;

    write.set_near_id(TYPE_Conv(Request.near_id));
    write.set_room_id(TYPE_Conv(Request.room_id));

    ClientContext context;

    if (CallRPC<InternalMMService::Stub, InternalUserLeftBattleRequest, Empty>(stub.get(), context, write, &read, this->error, &InternalMMService::Stub::UserLeftBattle))
    {
        errorH(this->error, error);
        return false;
    }

    return true;
}

bool gRPC_ClientInternalMM::CallRPC_SaveBattleResult(ModelInternalMM::SaveBattleResultRequest& Request, HOOK_ERROR)
{
    SaveBattleResultRequest write;
    Empty read;

    write.set_room_id(TYPE_Conv(Request.room_id));

    ModelInternalMM::InternalPlayerResult* ptr = Request.results->getObjectPtr();
    for (int i = 0; i < Request.results->getSize(); i++)
    {
        game::battlemon::mm::internal::InternalPlayerResult* ipr = write.add_results();
        ipr->set_near_id(TYPE_Conv(ptr[i].near_id));
        ipr->set_place(*ptr[i].place);
    }

    ClientContext context;

    if (CallRPC<InternalMMService::Stub, SaveBattleResultRequest, Empty>(stub.get(), context, write, &read, this->error, &InternalMMService::Stub::SaveBattleResult))
    {
        errorH(this->error, error);
        return false;
    }
    return true;
}

RoomInfoResponse gRPC_ClientInternalMM::CallRPC_GetRoomInfo(ModelInternalMM::RoomInfoRequest& Request, HOOK_ERROR)
{
    RoomInfoRequest write;
    RoomInfoResponse read;

    write.set_room_id(TYPE_Conv(Request.room_id));


    ClientContext context;

    if (CallRPC<InternalMMService::Stub, RoomInfoRequest, RoomInfoResponse>(stub.get(), context, write, &read, this->error, &InternalMMService::Stub::GetRoomInfo))
    {
        errorH(this->error, error);
        return read;
    }
    return read;
}

RoomInfoResponse gRPC_ClientInternalMM::CallRPC_CreateRoomWithPlayers(ModelInternalMM::CreateRoomRequest& Request, HOOK_ERROR)
{
    CreateRoomRequest write;
    RoomInfoResponse read;

    GameMode* gameMode = (GameMode*)&write.mode();

    switch (Request.mode->match_type)
    {
    case ModelMM::MatchType::DEATH_MATCH:
        (*gameMode).set_match_type(game::battlemon::mm::MatchType::DEATH_MATCH);
        break;
    case ModelMM::MatchType::CATCH_THE_FLAG:
        (*gameMode).set_match_type(game::battlemon::mm::MatchType::CATCH_THE_FLAG);
        break;
    default:
        break;
    }

    switch (Request.mode->match_mode)
    {
    case ModelMM::MatchMode::NONE:
        (*gameMode).set_match_mode(game::battlemon::mm::MatchMode::NONE);
        break;
    case ModelMM::MatchMode::EQUIPMENT:
        (*gameMode).set_match_mode(game::battlemon::mm::MatchMode::EQUIPMENT);
        break;
    case ModelMM::MatchMode::REALISM:
        (*gameMode).set_match_mode(game::battlemon::mm::MatchMode::REALISM);
        break;
    case ModelMM::MatchMode::DEFAULT:
        (*gameMode).set_match_mode(game::battlemon::mm::MatchMode::NONE);
        break;
    }

    for (int i = 0; i < Request.near_ids->getSize(); i++)
    {
        write.set_near_ids(i, TYPE_Conv(Request.near_ids->getObject(i)));
    }

    ClientContext context;

    if (CallRPC<InternalMMService::Stub, CreateRoomRequest, RoomInfoResponse>(stub.get(), context, write, &read, this->error, &InternalMMService::Stub::CreateRoomWithPlayers))
    {
        errorH(this->error, error);
        return read;
    }
    return read;
}

bool gRPC_ClientInternalMM::CallRPC_DedicatedServerIsReady(ModelInternalMM::DedicatedServerIsReadyRequest& Request, HOOK_ERROR)
{
    DedicatedServerIsReadyRequest write;
    Empty read;
    write.set_room_id(TYPE_Conv(Request.room_id));

    ClientContext context;

    if (CallRPC<InternalMMService::Stub, DedicatedServerIsReadyRequest, Empty>(stub.get(), context, write, &read, this->error, &InternalMMService::Stub::DedicatedServerIsReady))
    {
        errorH(this->error, error);
        return false;
    }
    return true;
}


///Updates.Proto


ClientUpdates::ClientUpdates(const void* out, const ModelUpdates::MessageData& message)
{
    ((game::battlemon::updates::UpdateMessage*)out)->ParsePartialFromArray(message.Data, message.ByteSize);
}