#include "Helper.h"
#include "include/gRPCResponse.h"
#include "GRPC_Client.h"


//////							gRPC_ResponseItem


gRPC_ResponseItem::gRPC_ResponseItem(Client** client, void* inRequest, const bool& ssl, const TYPE_CHAR* url, Type_Call_gRPC::Type_gRPCItem type_gRPC):gRPC_Response(client, type_gRPC)
{
	free_gRPC();
	switch (type_gRPC)
	{
	case Type_Call_gRPC::Type_gRPCItem::NONE:
		break;
	case Type_Call_gRPC::Type_gRPCItem::GET_ITEMS:
		CallRPC_GetItems(ssl, url);
		break;
	case Type_Call_gRPC::Type_gRPCItem::GET_BUNDLES:
		CallRPC_GetBundles(ssl, url);
		break;
	case Type_Call_gRPC::Type_gRPCItem::EDIT_BUNDLE:
		CallRPC_EditBundle(ssl, url, *(ModelItems::EditBundleRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPCItem::ATTACH_BUNDLE:
		CallRPC_AttachBundle(ssl, url, *(ModelItems::AttachBundleRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPCItem::DETACH_BUNDLE:
		CallRPC_DetachBundle(ssl, url, *(ModelItems::DetachBundleRequest*)inRequest);
		break;
	default:
		break;
	}
}

void gRPC_ResponseItem::free_gRPC()
{
	if (gRPC_read != nullptr)
	{
		switch (type_gRPC)
		{
		case Type_Call_gRPC::Type_gRPCItem::NONE:
			break;
		case Type_Call_gRPC::Type_gRPCItem::GET_ITEMS:
			delete ITEMS_RESPONSE;
			break;
		case Type_Call_gRPC::Type_gRPCItem::GET_BUNDLES:
			delete GET_BUNDLES_RESPONSE;
			break;
		case Type_Call_gRPC::Type_gRPCItem::EDIT_BUNDLE:
			delete WEAPON_BUNDLE;
			break;
		case Type_Call_gRPC::Type_gRPCItem::ATTACH_BUNDLE:
			delete COMMON_EMPTY;
			break;
		case Type_Call_gRPC::Type_gRPCItem::DETACH_BUNDLE:
			delete COMMON_EMPTY;
			break;
		}

		type_gRPC = Type_Call_gRPC::Type_gRPCItem::NONE;
		gRPC_read = nullptr;
		return;
	}
}

gRPC_ResponseItem::~gRPC_ResponseItem()
{
	free_gRPC();
	Helper::free(&error);
}

ModelItems::WeaponBundle gRPC_ResponseItem::gRPC_EditBundle()
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::EDIT_BUNDLE && gRPC_read != nullptr)
	{
		int size = WEAPON_BUNDLE->items().size();
		ModelItems::WeaponBundleItem* WBI = new ModelItems::WeaponBundleItem[size];
		for (int i = 0; i < size; i++)
		{
			WBI[i].copy = false;
			WBI[i].item_type = Helper::ConvWeaponBundleItemTypeToCPP(WEAPON_BUNDLE->items(i).item_type());
			WBI[i].slot_type = Helper::WeaponBundleSlotTypeToCPP(WEAPON_BUNDLE->items(i).slot_type());
			WBI[i].skin = (TYPE_CHAR*)WEAPON_BUNDLE->items(i).skin().c_str();
		}

		ModelItems::WeaponBundle WB(WBI, size, false);

		WB.bundle_num = WEAPON_BUNDLE->bundle_num();
		WB.level = WEAPON_BUNDLE->level();
		WB.title = (TYPE_CHAR*)WEAPON_BUNDLE->title().c_str();

		return WB;
	}

	
	return ModelItems::WeaponBundle(-1, false);
}

bool gRPC_ResponseItem::gRPC_AttachBundle()
{
	if(type_gRPC == Type_Call_gRPC::Type_gRPCItem::ATTACH_BUNDLE && gRPC_read != nullptr)
		return *((bool*)gRPC_read);
	return false;
}

bool gRPC_ResponseItem::gRPC_DetachBundle()
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::DETACH_BUNDLE && gRPC_read != nullptr)
		return *((bool*)gRPC_read);
	return false;
}

ModelItems::Item gRPC_ResponseItem::gRPC_GetItem(int index)
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::GET_ITEMS)
	{
		//const game::battlemon::items::Item item_read = ((ItemsResponse*)gRPC_read)->items(index);

		ModelItems::Item item((void*)&ITEMS_RESPONSE->items(index), false);
		return item;
	}
	SET_ERROR("check gRPC_GetItem");
	return ModelItems::Item(nullptr, false);
}

ObjectList<ModelItems::Item> gRPC_ResponseItem::gRPC_GetItemsArray()
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::GET_ITEMS)
	{
		type_gRPC = Type_Call_gRPC::Type_gRPCItem::GET_ITEMS;

		int size = ITEMS_RESPONSE->items().size();
		ModelItems::Item* itemOUT = new ModelItems::Item[size];
		for (int i = 0; i < size; i++)
		{
			itemOUT[i] = gRPC_GetItem(i);
		}

		return ObjectList<ModelItems::Item>(itemOUT, size);
	}

	ObjectList<ModelItems::Item> itemsListErr(-1);
	return itemsListErr;
}

ObjectList<ModelItems::Item> gRPC_ResponseItem::gRPC_CopyDataItems()
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::GET_ITEMS)
	{
		int size = ITEMS_RESPONSE->items().size();
		ModelItems::Item* itemOUT = new ModelItems::Item[size];
		for (int i = 0; i < size; i++)
		{
			itemOUT[i] = ModelItems::Item((void*)&ITEMS_RESPONSE->items(i), true);
		}
		return ObjectList<ModelItems::Item>(itemOUT, size);
	}

	return ObjectList<ModelItems::Item>(-1);
}

ModelItems::WeaponBundle gRPC_ResponseItem::gRPC_GetBundle(int index)
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::GET_BUNDLES)
	{
		int size = GET_BUNDLES_RESPONSE->bundles(index).items_size();
		ModelItems::WeaponBundleItem* item = new ModelItems::WeaponBundleItem[size];

		for (int i = 0; i < size; i++)
		{
			item[i].copy = false;
			switch (GET_BUNDLES_RESPONSE->bundles(index).items().Get(i).item_type())
			{
			case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_PRIMARY:
				item[i].item_type = ModelItems::WeaponBundleItemType::BUNDLE_ITEM_PRIMARY;
				break;
			case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_SECONDARY:
				item[i].item_type = ModelItems::WeaponBundleItemType::BUNDLE_ITEM_SECONDARY;
				break;
			case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_MELEE:
				item[i].item_type = ModelItems::WeaponBundleItemType::BUNDLE_ITEM_MELEE;
				break;
			case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_TACTICAL:
				item[i].item_type = ModelItems::WeaponBundleItemType::BUNDLE_ITEM_TACTICAL;
				break;
			case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_MILITARY:
				item[i].item_type = ModelItems::WeaponBundleItemType::BUNDLE_ITEM_MILITARY;
				break;
			case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_PERK:
				item[i].item_type = ModelItems::WeaponBundleItemType::BUNDLE_ITEM_PERK;
				break;
			default:
				break;
			}

			switch (GET_BUNDLES_RESPONSE->bundles(index).items().Get(i).slot_type())
			{
			case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_PRIMARY:
				item[i].slot_type = ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_PRIMARY;
				break;
			case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_MELEE:
				item[i].slot_type = ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_MELEE;
				break;
			case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_ONE:
				item[i].slot_type = ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_ONE;
				break;
			case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_TWO:
				item[i].slot_type = ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_TWO;
				break;
			case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_ONE:
				item[i].slot_type = ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_ONE;
				break; 
			case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_TWO:
				item[i].slot_type = ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_TWO;
				break;
			case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_PERK_ONE:
				item[i].slot_type = ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_PERK_ONE;
				break;
			case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_PERK_TWO:
				item[i].slot_type = ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_PERK_TWO;
				break;
			default:
				break;
			}
			item[i].skin = (TYPE_CHAR* )GET_BUNDLES_RESPONSE->bundles(index).items().Get(i).skin().c_str();
		}

		ModelItems::WeaponBundle wb(item, size, false);
		wb.bundle_num = GET_BUNDLES_RESPONSE->bundles(index).bundle_num();
		wb.title = (TYPE_CHAR*)TYPE_ReConv(GET_BUNDLES_RESPONSE->bundles(index).title()).c_str();
		wb.level = GET_BUNDLES_RESPONSE->bundles(index).level();

		return wb;
	}

	return ModelItems::WeaponBundle(-1, false);
}

ObjectList<ModelItems::WeaponBundle> gRPC_ResponseItem::gRPC_GetBundlesArray()
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::GET_BUNDLES)
	{
		int size = GET_BUNDLES_RESPONSE->bundles().size();
		ModelItems::WeaponBundle* wbOut = new ModelItems::WeaponBundle[size];

		for (int i = 0; i < size; i++)
		{
			wbOut[i] = gRPC_GetBundle(i);
		}

		return ObjectList<ModelItems::WeaponBundle>(wbOut, size);
	}

	return ObjectList<ModelItems::WeaponBundle>(-1);
}

ObjectList<ModelItems::WeaponBundle> gRPC_ResponseItem::gRPC_CopyDataBundles()
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::GET_BUNDLES)
	{
		ModelItems::WeaponBundle item;
		ObjectList<ModelItems::WeaponBundle> itemsList(GET_BUNDLES_RESPONSE->bundles().size());

		int size = GET_BUNDLES_RESPONSE->bundles().size();
		ModelItems::WeaponBundle* wbOut = new ModelItems::WeaponBundle[size];

		for (int i = 0; i < GET_BUNDLES_RESPONSE->bundles().size(); i++)
		{
			TYPE_Copy(GET_BUNDLES_RESPONSE->bundles(i).title(),item.title);
			item.level = GET_BUNDLES_RESPONSE->bundles(i).level();
			item.bundle_num = GET_BUNDLES_RESPONSE->bundles(i).bundle_num();

			wbOut[i] = item;
		}

		return ObjectList<ModelItems::WeaponBundle>(wbOut, size);
	}

	return ObjectList<ModelItems::WeaponBundle>(-1);
}


void gRPC_ResponseItem::CallRPC_GetItems(const bool& ssl, const TYPE_CHAR* url)
{
	free_gRPC();
	gRPC_ClientItems grpcClient(ssl, url);
	if ((*client) != nullptr)
		gRPC_read = new ItemsResponse(grpcClient.CallRPC_GetItems((*client)->GetAccount(), (*client)->GetSing(), THROW_HOOK));
	else
		SET_ERROR("check CallRPC_GetItems() client == nullptr");
}

void gRPC_ResponseItem::CallRPC_GetBundles(const bool& ssl, const TYPE_CHAR* url)
{
	free_gRPC();
	gRPC_ClientItems grpcClient(ssl, url);
	gRPC_read = new game::battlemon::items::GetBundlesResponse(grpcClient.CallRPC_GetBundles((*client)->GetAccount(), (*client)->GetSing(), THROW_HOOK));
}


void gRPC_ResponseItem::CallRPC_EditBundle(const bool& ssl, const TYPE_CHAR* url, ModelItems::EditBundleRequest& request)
{
	free_gRPC();
	gRPC_ClientItems grpcClient(ssl, url);
	gRPC_read = new game::battlemon::items::WeaponBundle(grpcClient.CallRPC_EditBundle((*client)->GetAccount(), (*client)->GetSing(), request, THROW_HOOK));
}

void gRPC_ResponseItem::CallRPC_AttachBundle(const bool& ssl, const TYPE_CHAR* url, ModelItems::AttachBundleRequest& request)
{
	free_gRPC();
	gRPC_ClientItems grpcClient(ssl, url);
	gRPC_read = new bool(grpcClient.CallRPC_AttachBundle((*client)->GetAccount(), (*client)->GetSing(), request, THROW_HOOK));
}

void gRPC_ResponseItem::CallRPC_DetachBundle(const bool& ssl, const TYPE_CHAR* url, ModelItems::DetachBundleRequest& request)
{
	free_gRPC();
	gRPC_ClientItems grpcClient(ssl, url);
	gRPC_read = new bool(grpcClient.CallRPC_DetachBundle((*client)->GetAccount(), (*client)->GetSing(), request, THROW_HOOK));
}


//////							gRPC_ResponseMM

void gRPC_ResponseMM::free_gRPC()
{
	if(gRPC_read!=nullptr)
		switch (type_gRPC)
		{
		case Type_Call_gRPC::Type_gRPC_MM::NONE:
			break;
		case Type_Call_gRPC::Type_gRPC_MM::SEARCH_GAME:
			delete SEARCH_GAME_RESPONSE;
			break;
		case Type_Call_gRPC::Type_gRPC_MM::ACCEPT_GAME:
		case Type_Call_gRPC::Type_gRPC_MM::CANCEL_SEARCH:
			delete (bool*)gRPC_read;
			break;
		default:
			break;
		}
	type_gRPC = Type_Call_gRPC::Type_gRPC_MM::NONE;
	gRPC_read = nullptr;
}

void gRPC_ResponseMM::CallRPC_SearchGame(const bool& ssl, const TYPE_CHAR* url, ModelMM::SearchGameRequest& inRequest)
{
	gRPC_ClientMM  gRPC_Client(ssl, url);
	gRPC_read = new game::battlemon::mm::SearchGameResponse(gRPC_Client.CallRPC_SearchGame((*client)->GetAccount(), (*client)->GetSing(), inRequest, THROW_HOOK));
}

void gRPC_ResponseMM::CallRPC_AcceptGame(const bool& ssl, const TYPE_CHAR* url, ModelMM::AcceptGameRequest& inRequest)
{
	gRPC_ClientMM  gRPC_Client(ssl, url);
	gRPC_read = new bool(gRPC_Client.CallRPC_AcceptGame((*client)->GetAccount(), (*client)->GetSing(), inRequest, THROW_HOOK));
}

void gRPC_ResponseMM::CallRPC_CancelSearch(const bool& ssl, const TYPE_CHAR* url)
{
	gRPC_ClientMM  gRPC_Client(ssl, url);
	gRPC_read = new bool(gRPC_Client.CallRPC_CancelSearch((*client)->GetAccount(), (*client)->GetSing(), THROW_HOOK));
}


ModelMM::SearchGameResponse gRPC_ResponseMM::getResponse_SearchGame()
{
	ModelMM::SearchGameResponse read;

	switch (SEARCH_GAME_RESPONSE->status())
	{
	case game::battlemon::mm::SearchGameResponseStatus::OK:
		read.status = ModelMM::SearchGameResponseStatus::OK;
		return read;
	case game::battlemon::mm::SearchGameResponseStatus::NFT_ERROR:
		read.status = ModelMM::SearchGameResponseStatus::NFT_ERROR;
		return read;
	case game::battlemon::mm::SearchGameResponseStatus::ALREADY_IN_QUEUE:
		read.status = ModelMM::SearchGameResponseStatus::ALREADY_IN_QUEUE;
		return read;
	case game::battlemon::mm::SearchGameResponseStatus::ALREADY_IN_BATTLE:
		read.status = ModelMM::SearchGameResponseStatus::ALREADY_IN_BATTLE;
		return read;
	case game::battlemon::mm::SearchGameResponseStatus::GAMEMODE_ERROR:
		read.status = ModelMM::SearchGameResponseStatus::GAMEMODE_ERROR;
		return read;
	case game::battlemon::mm::SearchGameResponseStatus::INVALID_REQUEST:
		read.status = ModelMM::SearchGameResponseStatus::INVALID_REQUEST;
		return read;
	case game::battlemon::mm::SearchGameResponseStatus::INTERNAL_ERROR:
		read.status = ModelMM::SearchGameResponseStatus::INTERNAL_ERROR;
		return read;
	default:
		read.status = ModelMM::SearchGameResponseStatus::DEFAULT;
		break;
	};

	return read;
}

bool gRPC_ResponseMM::getResponse_AcceptGame()
{
	if (gRPC_read != nullptr)
		return *((bool*)gRPC_read);
	return false;
}

bool gRPC_ResponseMM::getResponse_CancelSearch()
{
	if (gRPC_read != nullptr)
		return *((bool*)gRPC_read);
	return false;
}

gRPC_ResponseMM::gRPC_ResponseMM(Client** client, void* inRequest, const bool& ssl, const TYPE_CHAR* url, Type_Call_gRPC::Type_gRPC_MM type_gRPC) :gRPC_Response(client, type_gRPC)
{
	free_gRPC();
	switch (type_gRPC)
	{
	case Type_Call_gRPC::Type_gRPC_MM::NONE:

		break;
	case Type_Call_gRPC::Type_gRPC_MM::SEARCH_GAME:
		CallRPC_SearchGame(ssl, url, *(ModelMM::SearchGameRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPC_MM::ACCEPT_GAME:
		CallRPC_AcceptGame(ssl, url, *(ModelMM::AcceptGameRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPC_MM::CANCEL_SEARCH:
		CallRPC_CancelSearch(ssl, url);
		break;
	default:
		break;
	}
}

gRPC_ResponseMM::~gRPC_ResponseMM()
{
	free_gRPC();
	client = nullptr;
	Helper::free(&error);
}

gRPC_ResponseInternalMM::gRPC_ResponseInternalMM(Client** client, void* inRequest, const bool& ssl, const TYPE_CHAR* url, Type_Call_gRPC::Type_gRPC_InternalMM type_gRPC) :gRPC_Response(client, type_gRPC)
{
	free_gRPC();
	switch (type_gRPC)
	{
	case Type_Call_gRPC::Type_gRPC_InternalMM::NONE:
		break;
	case Type_Call_gRPC::Type_gRPC_InternalMM::USER_LEFT_BATTLE:
		CallRPC_UserLeftBattle(ssl, url, *(ModelInternalMM::InternalUserLeftBattleRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPC_InternalMM::SAVE_BATTLE_RESULT:
		CallRPC_SaveBattleResult(ssl,url, *(ModelInternalMM::SaveBattleResultRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPC_InternalMM::GET_ROOM_INFO:
		CallRPC_GetRoomInfo(ssl, url, *(ModelInternalMM::RoomInfoRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPC_InternalMM::CREATE_ROOM_WITH_PLAYERS:
		CallRPC_CreateRoomWithPlayers(ssl, url, *(ModelInternalMM::CreateRoomRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPC_InternalMM::DEDICATED_SERVER_IS_READY:
		CallRPC_DedicatedServerIsReady(ssl, url, *(ModelInternalMM::DedicatedServerIsReadyRequest*)inRequest);
		break;
	default:
		break;
	}
}

gRPC_ResponseInternalMM::~gRPC_ResponseInternalMM()
{
	free_gRPC();
	Helper::free(&error);
}

void gRPC_ResponseInternalMM::free_gRPC()
{
	if (gRPC_read != nullptr)
	switch (type_gRPC)
	{
	case Type_Call_gRPC::Type_gRPC_InternalMM::NONE:
		break;
	case Type_Call_gRPC::Type_gRPC_InternalMM::GET_ROOM_INFO:
	case Type_Call_gRPC::Type_gRPC_InternalMM::CREATE_ROOM_WITH_PLAYERS:
		delete ROOM_INFO_RESPONSE;
		break;
	case Type_Call_gRPC::Type_gRPC_InternalMM::DEDICATED_SERVER_IS_READY:
	case Type_Call_gRPC::Type_gRPC_InternalMM::USER_LEFT_BATTLE:
	case Type_Call_gRPC::Type_gRPC_InternalMM::SAVE_BATTLE_RESULT:
		delete (bool*)gRPC_read;
		break;
	default:
		break;
	}
	type_gRPC = Type_Call_gRPC::Type_gRPC_InternalMM::NONE;
	gRPC_read = nullptr;
}

void gRPC_ResponseInternalMM::CallRPC_UserLeftBattle(const bool& ssl, const TYPE_CHAR* url, ModelInternalMM::InternalUserLeftBattleRequest& inRequest)
{
	gRPC_ClientInternalMM  gRPC_Client(ssl, url);
	gRPC_read = new bool(gRPC_Client.CallRPC_UserLeftBattle(inRequest, THROW_HOOK));
}

void gRPC_ResponseInternalMM::CallRPC_SaveBattleResult(const bool& ssl, const TYPE_CHAR* url, ModelInternalMM::SaveBattleResultRequest& inRequest)
{
	gRPC_ClientInternalMM  gRPC_Client(ssl, url);
	gRPC_read = new bool(gRPC_Client.CallRPC_SaveBattleResult(inRequest, THROW_HOOK));
}

void gRPC_ResponseInternalMM::CallRPC_GetRoomInfo(const bool& ssl, const TYPE_CHAR* url, ModelInternalMM::RoomInfoRequest& inRequest)
{
	gRPC_ClientInternalMM  gRPC_Client(ssl, url);
	gRPC_read = new game::battlemon::mm::internal::RoomInfoResponse(gRPC_Client.CallRPC_GetRoomInfo(inRequest, THROW_HOOK));
}

void gRPC_ResponseInternalMM::CallRPC_CreateRoomWithPlayers(const bool& ssl, const TYPE_CHAR* url, ModelInternalMM::CreateRoomRequest& inRequest)
{
	gRPC_ClientInternalMM  gRPC_Client(ssl, url);
	gRPC_read = new game::battlemon::mm::internal::RoomInfoResponse(gRPC_Client.CallRPC_CreateRoomWithPlayers(inRequest, THROW_HOOK));
}

void gRPC_ResponseInternalMM::CallRPC_DedicatedServerIsReady(const bool& ssl, const TYPE_CHAR* url, ModelInternalMM::DedicatedServerIsReadyRequest& inRequest)
{
	gRPC_ClientInternalMM  gRPC_Client(ssl, url);
	gRPC_read = new bool(gRPC_Client.CallRPC_DedicatedServerIsReady(inRequest, THROW_HOOK));
}

bool gRPC_ResponseInternalMM::getResponse_UserLeftBattle()
{
	if(gRPC_read != nullptr)
		return *((bool*)gRPC_read);
	return false;
}

bool gRPC_ResponseInternalMM::getResponse_SaveBattleResult()
{
	if (gRPC_read != nullptr)
		return *((bool*)gRPC_read);
	return false;
}

ModelInternalMM::RoomInfoResponse gRPC_ResponseInternalMM::getResponse_GetRoomInfo()
{
	if (gRPC_read != nullptr && type_gRPC == Type_Call_gRPC::Type_gRPC_InternalMM::GET_ROOM_INFO)
	{
		return ModelInternalMM::RoomInfoResponse(gRPC_read, false);
	}

	return ModelInternalMM::RoomInfoResponse();
}

ModelInternalMM::RoomInfoResponse gRPC_ResponseInternalMM::getResponse_CreateRoomWithPlayers()
{
	if (gRPC_read != nullptr && type_gRPC == Type_Call_gRPC::Type_gRPC_InternalMM::CREATE_ROOM_WITH_PLAYERS)
	{
		return ModelInternalMM::RoomInfoResponse(gRPC_read, false);
	}
	return ModelInternalMM::RoomInfoResponse();
}

bool gRPC_ResponseInternalMM::getResponse_DedicatedServerIsReady()
{
	if (gRPC_read != nullptr)
		return *((bool*)gRPC_read);
	return false;
}

gRPC_ResponseUptate::gRPC_ResponseUptate():messageData(new ClientUpdates())
{
}

gRPC_ResponseUptate::~gRPC_ResponseUptate()
{
	delete (ClientUpdates*)messageData;
	messageData = nullptr;
}

const ModelUpdates::MessageData& gRPC_ResponseUptate::writeUpdate(const ModelUpdates::Update& Request)
{
	((ClientUpdates*)messageData)->writeUpdate(Request);
	return ((ClientUpdates*)messageData)->getMessageData();
}

const ModelUpdates::MessageData& gRPC_ResponseUptate::writeUpdateMessage(const ModelUpdates::UpdateMessage& Request)
{
	((ClientUpdates*)messageData)->writeUpdateMessage(Request);
	return ((ClientUpdates*)messageData)->getMessageData();
}

const ModelUpdates::MessageData& gRPC_ResponseUptate::writeRoomNeedAccept(const ModelUpdates::RoomNeedAccept& Request)
{
	((ClientUpdates*)messageData)->writeRoomNeedAccept(Request);
	return ((ClientUpdates*)messageData)->getMessageData();
}

const ModelUpdates::MessageData& gRPC_ResponseUptate::writeRoomInfo(const ModelUpdates::RoomInfo& Request)
{
	((ClientUpdates*)messageData)->writeRoomInfo(Request);
	return ((ClientUpdates*)messageData)->getMessageData();
}

const ModelUpdates::MessageData& gRPC_ResponseUptate::writeRoomPlayer(const ModelUpdates::RoomPlayer& Request)
{
	((ClientUpdates*)messageData)->writeRoomPlayer(Request);
	return ((ClientUpdates*)messageData)->getMessageData();
}

