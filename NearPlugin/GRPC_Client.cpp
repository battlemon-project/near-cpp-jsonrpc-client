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

WeaponBundle gRPC_ClientItems::CallRPC_EditBundle(ModelItems::EditBundleRequest& request, HOOK_ERROR)
{
    EditBundleRequest write;
    WeaponBundle read;
    write.set_bundle_num(request.getBundle_num());
    write.set_title(TYPE_Conv(request.getTitle()));

    ObjectList<ModelItems::WeaponBundleItem>* itemsU = &request.getItems();


    for (int i = 0; i < itemsU->getSize(); i++)
    {
        game::battlemon::items::WeaponBundleItem* wr = write.add_items();
        wr->set_skin(TYPE_Conv(itemsU->getObject(i).skin));
        wr->set_item_type(Helper::ConvWeaponBundleItemTypeToProto(itemsU->getObject(i).item_type));
        wr->set_slot_type(Helper::WeaponBundleSlotTypeToProto(itemsU->getObject(i).slot_type));
    }

    ClientContext context;

    if (CallRPC<ItemsService::Stub, EditBundleRequest, WeaponBundle>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::EditBundle))
    {
        errorH(this->error, error);
    }
    return read;
}

bool gRPC_ClientItems::CallRPC_AttachBundle(ModelItems::AttachBundleRequest& request, HOOK_ERROR)
{
    AttachBundleRequest write;
    Empty read;

    write.set_bundle_num(request.get_bundle_num());
    write.set_lemon_id(TYPE_Conv(request.get_lemon_id()));

    ClientContext context;
    if (CallRPC<ItemsService::Stub, AttachBundleRequest, Empty>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::AttachBundle))
    {
        errorH(this->error, error);
        return false;
    }

    return true;
}

bool gRPC_ClientItems::CallRPC_DetachBundle(ModelItems::DetachBundleRequest& request, HOOK_ERROR)
{
    DetachBundleRequest write;
    Empty read;

    write.set_bundle_num(request.get_bundle_num());
    write.set_lemon_id(TYPE_Conv(request.get_lemon_id()));

    ClientContext context;
    if (CallRPC<ItemsService::Stub, DetachBundleRequest, Empty>(stub.get(), context, write, &read, this->error, &ItemsService::Stub::DetachBundle))
    {
        errorH(this->error, error);
        return false;
    }

    return true;
}



//        gRPC_ClientMM



SearchGameResponse gRPC_ClientMM::CallRPC_SearchGame(ModelMM::SearchGameRequest& Request, HOOK_ERROR)
{
    SearchGameRequest write;
    SearchGameResponse read;

    GameMode* gameMode = (GameMode*)&write.game_mode();
    
    switch (Request.game_mode.match_type)
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

    switch (Request.game_mode.match_mode)
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
    
    ClientContext context;

    if (CallRPC<MMService::Stub, SearchGameRequest, SearchGameResponse>(stub.get(), context, write, &read, this->error, &MMService::Stub::SearchGame))
    {
        errorH(this->error, error);
    }

    return read;
}

bool gRPC_ClientMM::CallRPC_AcceptGame(ModelMM::AcceptGameRequest& Request, HOOK_ERROR)
{
    AcceptGameRequest write;
    Empty read;

    write.set_lemon_id(TYPE_Conv(Request.lemon_id));
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



// internalMM

bool gRPC_ClientInternalMM::CallRPC_UserLeftBattle(ModelInternalMM::InternalUserLeftBattleRequest& Request, HOOK_ERROR)
{
    InternalUserLeftBattleRequest write;
    Empty read;

    write.set_near_id(Request.near_id);
    write.set_room_id(Request.room_id);

    ClientContext context;

    if (CallRPC<InternalMMService::Stub, InternalUserLeftBattleRequest, Empty>(stub.get(), context, write, &read, this->error, &InternalMMService::Stub::UserLeftBattle))
    {
        errorH(this->error, error);
        return false;
    }

    return true;
}

bool gRPC_ClientInternalMM::SaveBattleResult(ModelInternalMM::SaveBattleResultRequest& Request, HOOK_ERROR)
{
    SaveBattleResultRequest write;
    Empty read;

    write.set_room_id(Request.room_id);

    ModelInternalMM::InternalPlayerResult* ptr = Request.results.getObjectPtr();
    for (int i = 0; i < Request.results.getSize(); i++)
    {
        game::battlemon::mm::internal::InternalPlayerResult* ipr = write.add_results();
        ipr->set_near_id(ptr[i].near_id);
        ipr->set_place(ptr[i].place);
    }

    ClientContext context;

    if (CallRPC<InternalMMService::Stub, SaveBattleResultRequest, Empty>(stub.get(), context, write, &read, this->error, &InternalMMService::Stub::SaveBattleResult))
    {
        errorH(this->error, error);
        return false;
    }
    return true;
}

RoomInfoResponse gRPC_ClientInternalMM::GetRoomInfo(ModelInternalMM::RoomInfoRequest& Request, HOOK_ERROR)
{
    RoomInfoRequest write;
    RoomInfoResponse read;

    write.set_room_id(Request.room_id);


    ClientContext context;

    if (CallRPC<InternalMMService::Stub, RoomInfoRequest, RoomInfoResponse>(stub.get(), context, write, &read, this->error, &InternalMMService::Stub::GetRoomInfo))
    {
        errorH(this->error, error);
        return read;
    }
    return read;
}

RoomInfoResponse gRPC_ClientInternalMM::CreateRoomWithPlayers(ModelInternalMM::CreateRoomRequest& Request, HOOK_ERROR)
{
    CreateRoomRequest write;
    RoomInfoResponse read;

    GameMode* gameMode = (GameMode*)&write.mode();

    switch (Request.mode.match_type)
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

    switch (Request.mode.match_mode)
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

    ClientContext context;

    if (CallRPC<InternalMMService::Stub, CreateRoomRequest, RoomInfoResponse>(stub.get(), context, write, &read, this->error, &InternalMMService::Stub::CreateRoomWithPlayers))
    {
        errorH(this->error, error);
        return read;
    }
    return read;
}