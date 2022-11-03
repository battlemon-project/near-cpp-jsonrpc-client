#include "Helper.h"
#include "include/gRPCResponse.h"
#include "GRPC_Client.h"


//////							gRPC_ResponseItem


gRPC_ResponseItem::gRPC_ResponseItem(Client** client, void* inRequest, Type_Call_gRPC::Type_gRPCItem type_gRPC):gRPC_Response(client, type_gRPC)
{
	free_gRPC();
	switch (type_gRPC)
	{
	case Type_Call_gRPC::Type_gRPCItem::NONE:
		break;
	case Type_Call_gRPC::Type_gRPCItem::GET_ITEMS:
		CallRPC_GetItems();
		break;
	case Type_Call_gRPC::Type_gRPCItem::GET_BUNDLES:
		CallRPC_GetBundles();
		break;
	case Type_Call_gRPC::Type_gRPCItem::EDIT_BUNDLE:
		CallRPC_EditBundle(*(ModelItems::EditBundleRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPCItem::ATTACH_BUNDLE:
		CallRPC_AttachBundle(*(ModelItems::AttachBundleRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPCItem::DETACH_BUNDLE:
		CallRPC_DetachBundle(*(ModelItems::DetachBundleRequest*)inRequest);
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

	SET_ERROR("check free_gRPC");
}

gRPC_ResponseItem::~gRPC_ResponseItem()
{
	free_gRPC();
	Helper::free(error);
}

ModelItems::Item gRPC_ResponseItem::gRPC_GetItem(int index)
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::GET_ITEMS)
	{
		//const game::battlemon::items::Item item_read = ((ItemsResponse*)gRPC_read)->items(index);

		ModelItems::Item item(gRPC_read, index, false);
		return item;
	}
	SET_ERROR("check gRPC_GetItem");
	return ModelItems::Item(nullptr, -1, true);
}

ObjectList<ModelItems::Item> gRPC_ResponseItem::gRPC_GetItemsArray()
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::GET_ITEMS)
	{
		type_gRPC = Type_Call_gRPC::Type_gRPCItem::GET_ITEMS;
		ModelItems::Item* itemOUT = nullptr;
		ObjectList<ModelItems::Item> itemsList(ITEMS_RESPONSE->items().size());

		for (int i = 0; i < ITEMS_RESPONSE->items().size(); i++)
		{
			itemOUT[i] = gRPC_GetItem(i);
		}

		return itemsList;
	}

	ObjectList<ModelItems::Item> itemsList(-1);
	return itemsList;
}

ObjectList<ModelItems::Item> gRPC_ResponseItem::gRPC_CopyDataItems()
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::GET_ITEMS)
	{
		type_gRPC = Type_Call_gRPC::Type_gRPCItem::GET_ITEMS;
		ObjectList<ModelItems::Item> itemsList(ITEMS_RESPONSE->items().size());

		for (int i = 0; i < ITEMS_RESPONSE->items().size(); i++)
		{
			itemsList.setObject(ModelItems::Item(gRPC_read, i, true), i);
		}

		return itemsList;
	}

	ObjectList<ModelItems::Item> itemsList(-1);
	return itemsList;
}

ModelItems::WeaponBundle gRPC_ResponseItem::gRPC_GetBundle(int index)
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::GET_BUNDLES)
	{
		ModelItems::WeaponBundle wb(GET_BUNDLES_RESPONSE->bundles(index).items_size());

		wb.bundle_num = GET_BUNDLES_RESPONSE->bundles(index).bundle_num();

		wb.title = (TYPE_CHAR*)TYPE_ReConv(GET_BUNDLES_RESPONSE->bundles(index).title()).c_str();
		wb.level = GET_BUNDLES_RESPONSE->bundles(index).level();
		ModelItems::WeaponBundleItem* item = wb.WeaponList.getObjectPtr();

		for (int i = 0; i < GET_BUNDLES_RESPONSE->bundles(index).items_size(); i++)
		{
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

			switch (GET_BUNDLES_RESPONSE->bundles(index).items().Get(i).item_type())
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
		}
		return wb;
	}

	return ModelItems::WeaponBundle(-1);
}

ObjectList<ModelItems::WeaponBundle> gRPC_ResponseItem::gRPC_GetBundlesArray()
{
	if (type_gRPC == Type_Call_gRPC::Type_gRPCItem::GET_BUNDLES)
	{
		type_gRPC = Type_Call_gRPC::Type_gRPCItem::GET_BUNDLES;
		ModelItems::WeaponBundle* itemOUT = nullptr;
		ObjectList<ModelItems::WeaponBundle> itemsList(ITEMS_RESPONSE->items().size());

		for (int i = 0; i < ITEMS_RESPONSE->items().size(); i++)
		{
			itemOUT[i] = gRPC_GetBundle(i);
		}

		return itemsList;
	}

	ObjectList<ModelItems::WeaponBundle> itemsList(-1);
	return itemsList;
	return ObjectList<ModelItems::WeaponBundle>(-1);
}

ObjectList<ModelItems::WeaponBundle> gRPC_ResponseItem::gRPC_CopyDataBundles()
{
	return ObjectList<ModelItems::WeaponBundle>(-1);
}

void gRPC_ResponseItem::CallRPC_GetItems()
{
	free_gRPC();
	gRPC_ClientItems grpcClient;
	if ((*client) != nullptr)
		gRPC_read = new ItemsResponse(grpcClient.CallRPC_GetItems((*client)->GetAccount(), (*client)->GetSing(), THROW_HOOK));
	else
		SET_ERROR("check CallRPC_GetItems() client == nullptr");
}

void gRPC_ResponseItem::CallRPC_GetBundles()
{
	free_gRPC();
	gRPC_ClientItems grpcClient;
	gRPC_read = new game::battlemon::items::GetBundlesResponse(grpcClient.CallRPC_GetBundles((*client)->GetAccount(), (*client)->GetSing(), THROW_HOOK));
}


void gRPC_ResponseItem::CallRPC_EditBundle(ModelItems::EditBundleRequest& request)
{
	free_gRPC();
	gRPC_ClientItems grpcClient;
	gRPC_read = new game::battlemon::items::WeaponBundle(grpcClient.CallRPC_EditBundle(request, THROW_HOOK));
}

bool gRPC_ResponseItem::CallRPC_AttachBundle(ModelItems::AttachBundleRequest& request)
{
	free_gRPC();
	gRPC_ClientItems grpcClient;
	gRPC_read = new bool(grpcClient.CallRPC_AttachBundle(request, THROW_HOOK));
	return *((bool*)gRPC_read);
}

bool gRPC_ResponseItem::CallRPC_DetachBundle(ModelItems::DetachBundleRequest& request)
{
	free_gRPC();
	gRPC_ClientItems grpcClient;
	gRPC_read = new bool(grpcClient.CallRPC_DetachBundle(request, THROW_HOOK));
	return *((bool*)gRPC_read);
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

void gRPC_ResponseMM::CallRPC_SearchGame(ModelMM::SearchGameRequest& inRequest)
{
	gRPC_ClientMM  gRPC_Client;
	gRPC_read = new game::battlemon::mm::SearchGameResponse(gRPC_Client.CallRPC_SearchGame(inRequest, THROW_HOOK));
}

bool gRPC_ResponseMM::CallRPC_AcceptGame(ModelMM::AcceptGameRequest& inRequest)
{
	gRPC_ClientMM  gRPC_Client;
	gRPC_read = new bool(gRPC_Client.CallRPC_AcceptGame(inRequest, THROW_HOOK));
	return *(bool*)gRPC_read;
}

bool gRPC_ResponseMM::CallRPC_CancelSearch()
{
	gRPC_ClientMM  gRPC_Client;
	gRPC_read = new bool(gRPC_Client.CallRPC_CancelSearch((*client)->GetAccount(), (*client)->GetSing(), THROW_HOOK));
	return *(bool*)gRPC_read;
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
		break;
	};

	return read;
}

bool gRPC_ResponseMM::getResponse_AcceptGame()
{
	return *((bool*)gRPC_read);
}

bool gRPC_ResponseMM::getResponse_CancelSearch()
{
	return *((bool*)gRPC_read);
}

gRPC_ResponseMM::gRPC_ResponseMM(Client** client, void* inRequest, Type_Call_gRPC::Type_gRPC_MM type_gRPC) :gRPC_Response(client, type_gRPC)
{
	free_gRPC();
	switch (type_gRPC)
	{
	case Type_Call_gRPC::Type_gRPC_MM::NONE:

		break;
	case Type_Call_gRPC::Type_gRPC_MM::SEARCH_GAME:
		CallRPC_SearchGame(*(ModelMM::SearchGameRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPC_MM::ACCEPT_GAME:
		CallRPC_AcceptGame(*(ModelMM::AcceptGameRequest*)inRequest);
		break;
	case Type_Call_gRPC::Type_gRPC_MM::CANCEL_SEARCH:
		CallRPC_CancelSearch();
		break;
	default:
		break;
	}
}

gRPC_ResponseMM::~gRPC_ResponseMM()
{
	free_gRPC();
	client = nullptr;
	Helper::free(error);
}
