#ifdef __APPLE__
#include <thread>
#include <chrono>
#include <locale>
#include <codecvt>
#endif

#ifdef __unix__
//#include <threads.h>

#include <thread>
#include <chrono>
#include <locale>
#include <codecvt>

#endif

#include "Client.h"
#include "EdKeys.h"
#include "GRPC_Client.h"


#define ED25519 ((EdKeys*)keyPair)
#define THROW_HOOK error, allocateMemory

void allocateMemory(const std::string &copy, char* &target)
{
	if (target == nullptr && copy != "")
	{
		target = new char[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), target);
		target[copy.size()] = '\0';
	}
}

#if defined(__unix__) || defined(__APPLE__)
std::string convUTF(const char16_t* utp16)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	std::u16string utf16_string(utp16);
	std::string UTF8 = convert.to_bytes(utf16_string);
	return UTF8;
}
#define TYPE_Conv(str) convUTF(str)
#else
#define TYPE_Conv(str) str
#endif


#include <ctime>

void Client::Free_gRPC_read()
{
	switch (type_gRPC)
	{
	case Type_gRPC::NONE:
		break;
	case Type_gRPC::GET_ITEMS:
		delete (ItemsResponse*)gRPC_read;
		break;
	case Type_gRPC::GET_BUNDLES:
		break;
	case Type_gRPC::CREATE_BUNDLE:
		break;
	case Type_gRPC::EDIT_BUNDLE:
		break;
	case Type_gRPC::REMOVE_BUNDLE:
		break;
	case Type_gRPC::ATTACH_BUNDLE:
		break;
	case Type_gRPC::DETACH_BUNDLE:
		break;
	case Type_gRPC::SEARCH_GAME:
		break;
	case Type_gRPC::ACCEPT_GAME:
		break;
	case Type_gRPC::CANCEL_SEARCH:
		break;
	case Type_gRPC::AUTHORIZATION:
		break;
	case Type_gRPC::REGISTRATION:
		break;
	default:
		break;
	}
	gRPC_read = nullptr;
	type_gRPC = Type_gRPC::NONE;
}

Client::Client(const TYPE_CHAR* dir, const TYPE_CHAR* inpText, TypeInp type) : keyPair(new EdKeys()), error(nullptr), accountID(nullptr), keyPub58(nullptr), sign(nullptr), gRPC_read(nullptr), type_gRPC(Type_gRPC::NONE)
{
	network = nullptr;

	if (type == TypeInp::AUTHORIZATION)
	{
		if (ED25519->LoadKeys(TYPE_Conv(inpText), TYPE_Conv(dir)))
		{
			AuthServiceClient(TypeInp::AUTHORIZATION);
			type_gRPC = Type_gRPC::AUTHORIZATION;
			allocateMemory(ED25519->GetPubKey58(), this->keyPub58);
		}
		else
			allocateMemory("error loadKeys", this->error);
	}
	else
	{
		allocateMemory(TYPE_Conv(inpText), network);

		ED25519->GeneratingKeys(THROW_HOOK);
		if (error != nullptr) return;

		if (AuthServiceClient(TypeInp::REGISTRATION))
		{
			type_gRPC = Type_gRPC::REGISTRATION;
			ED25519->SaveKeys(this->accountID, TYPE_Conv(dir));
		}

		allocateMemory(ED25519->GetPubKey58(), this->keyPub58);
	}
}

void free(char* data)
{
	if (data != nullptr)
	{
		delete[]data;
		data = nullptr;
	}
}

Client::~Client()
{
	if (keyPair != nullptr)
	{
		delete ED25519;
		keyPair = nullptr;
	}
	free(accountID);
	free(keyPub58);
	free(error);
	free(sign);
}

bool Client::IsValidKeys()
{
	{ return ED25519->GetPubKey58() != ""; };
}

bool ChekClient(const std::string &PubKey, gRPC_ClientAuth &grpcClient, char* &error, void* &keyPair, char* &sign, char* &accountIDchr)
{
	SendCodeResponse CodeResponse = grpcClient.CallRPCSendCode(PubKey, THROW_HOOK);

	std::string signatureMessage = ED25519->MessageSigning(CodeResponse.code());
	VerifyCodeResponse accountID = grpcClient.CallRPCVerifyCode(PubKey, signatureMessage, THROW_HOOK);

	if (accountID.near_account_id() != "")
	{
		allocateMemory(signatureMessage, sign);
		allocateMemory(accountID.near_account_id(), accountIDchr);
		return true;
	}
	return false;
}

bool Client::AuthServiceClient(TypeInp type)
{
	std::string PubKey = ED25519->GetPubKey58();
	gRPC_ClientAuth grpcClient;

	if (TypeInp::REGISTRATION == type)
	{

		int i = 0;
		while (i < 30)
		{
			if (ChekClient(PubKey, grpcClient,this->error, this->keyPair, this->sign, this->accountID))
			{
				return true;
			}
			i++;
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000));
		}

		allocateMemory("error AuthService", this->error);

		return false;
	}
	else
	{
		return ChekClient(PubKey, grpcClient, this->error, this->keyPair, this->sign, this->accountID);
	}
}

ModelItems::OutfitKind& operator<<(ModelItems::OutfitKind& OutfitKindUE, const game::battlemon::items::OutfitKind& OutfitKindResponse)
{
	switch (OutfitKindResponse)
	{
	case 0:
		OutfitKindUE = ModelItems::OutfitKind::CAP;
		break;
	case 1:
		OutfitKindUE = ModelItems::OutfitKind::CLOTH;
		break;
	case 2:
		OutfitKindUE = ModelItems::OutfitKind::FIRE_ARM;
		break;
	case 3:
		OutfitKindUE = ModelItems::OutfitKind::COLD_ARM;
		break;
	case 4:
		OutfitKindUE = ModelItems::OutfitKind::BACK;
		break;
	default:
		OutfitKindUE = ModelItems::OutfitKind::DEFAULT;
		break;
	}
	return OutfitKindUE;
}

ModelItems::OutfitModel& operator<<(ModelItems::OutfitModel& OutfitModellUE, const game::battlemon::items::OutfitModel& OutfitModelResponse)
{
	allocateMemory(OutfitModelResponse.flavour(), OutfitModellUE.flavour);
	allocateMemory(OutfitModelResponse.token_id(), OutfitModellUE.token_id);
	OutfitModellUE.kind << OutfitModelResponse.kind();
	return OutfitModellUE;
}

ModelItems::LemonModel& operator<<(ModelItems::LemonModel& LemonModelUE, const game::battlemon::items::LemonModel& LemonModelResponse)
{
	allocateMemory(LemonModelResponse.exo(), LemonModelUE.exo);
	allocateMemory(LemonModelResponse.eyes(), LemonModelUE.eyes);
	allocateMemory(LemonModelResponse.head(), LemonModelUE.head);
	allocateMemory(LemonModelResponse.teeth(), LemonModelUE.teeth);
	allocateMemory(LemonModelResponse.face(), LemonModelUE.face);
	LemonModelUE.cap << LemonModelResponse.cap();
	LemonModelUE.cloth << LemonModelResponse.cloth();
	LemonModelUE.fire_arm << LemonModelResponse.fire_arm();
	LemonModelUE.cold_arm << LemonModelResponse.cold_arm();
	LemonModelUE.back << LemonModelResponse.back();
	return LemonModelUE;
}

ModelItems::Item& operator<<(ModelItems::Item& itemsUE, const game::battlemon::items::Item& itemResponse)
{
	allocateMemory(itemResponse.token_id(), itemsUE.token_id);
	allocateMemory(itemResponse.media(), itemsUE.media);
	allocateMemory(itemResponse.owner_id(), itemsUE.owner_id);
	itemsUE.lemon << itemResponse.lemon();
	itemsUE.outfit << itemResponse.outfit();
	return itemsUE;
}


ModelItems::Item& ItemsList::getItem(int id)
{
	return items[id];
}

ModelItems::Item Client::gRPC_GetItem(int index)
{
	if (type_gRPC == Type_gRPC::GET_ITEMS)
	{
		//const game::battlemon::items::Item item_read = ((ItemsResponse*)gRPC_read)->items(index);

		ModelItems::Item item;
		item.in_fight = ((ItemsResponse*)gRPC_read)->items(index).in_fight();
		item.token_id = (char*)((ItemsResponse*)gRPC_read)->items(index).token_id().c_str();
		item.media = (char*)((ItemsResponse*)gRPC_read)->items(index).media().c_str();
		item.owner_id = (char*)((ItemsResponse*)gRPC_read)->items(index).owner_id().c_str();
		item.lemon.exo = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().exo().c_str();
		item.lemon.eyes = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().eyes().c_str();
		item.lemon.head = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().head().c_str();
		item.lemon.teeth = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().teeth().c_str();
		item.lemon.face = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().face().c_str();

		item.lemon.cap.flavour = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().cap().flavour().c_str();
		item.lemon.cap.token_id = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().cap().token_id().c_str();
		item.lemon.cap.kind << ((ItemsResponse*)gRPC_read)->items(index).lemon().cap().kind();

		item.lemon.cloth.flavour = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().cloth().flavour().c_str();
		item.lemon.cloth.token_id = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().cloth().token_id().c_str();
		item.lemon.cloth.kind << ((ItemsResponse*)gRPC_read)->items(index).lemon().cloth().kind();


		item.lemon.fire_arm.flavour = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().fire_arm().flavour().c_str();
		item.lemon.fire_arm.token_id = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().fire_arm().token_id().c_str();
		item.lemon.fire_arm.kind << ((ItemsResponse*)gRPC_read)->items(index).lemon().fire_arm().kind();

		item.lemon.cold_arm.flavour = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().cold_arm().flavour().c_str();
		item.lemon.cold_arm.token_id = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().cold_arm().token_id().c_str();
		item.lemon.cold_arm.kind << ((ItemsResponse*)gRPC_read)->items(index).lemon().cold_arm().kind();

		item.lemon.back.flavour = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().back().flavour().c_str();
		item.lemon.back.token_id = (char*)((ItemsResponse*)gRPC_read)->items(index).lemon().back().token_id().c_str();
		item.lemon.back.kind << ((ItemsResponse*)gRPC_read)->items(index).lemon().back().kind();

		item.outfit.flavour = (char*)((ItemsResponse*)gRPC_read)->items(index).outfit().flavour().c_str();
		item.outfit.token_id = (char*)((ItemsResponse*)gRPC_read)->items(index).outfit().token_id().c_str();
		item.outfit.kind << ((ItemsResponse*)gRPC_read)->items(index).outfit().kind();

		return item;
	}

	return ModelItems::Item();
}

ItemsList Client::gRPC_GetItems()
{
	if (type_gRPC == Type_gRPC::GET_ITEMS)
	{
		type_gRPC = Type_gRPC::GET_ITEMS;
		ModelItems::Item* itemOUT = nullptr;
		ItemsList itemsList(itemOUT, ((ItemsResponse*)gRPC_read)->items().size());

		for (size_t i = 0; i < ((ItemsResponse*)gRPC_read)->items().size(); i++)
		{
			itemOUT[i] = gRPC_GetItem(i);
		}

		return itemsList;
	}

	ModelItems::Item* itemOUT = nullptr;
	ItemsList itemsList(itemOUT, -1);
	return itemsList;
}

ItemsList Client::gRPC_CopyItems()
{
	if (type_gRPC == Type_gRPC::GET_ITEMS)
	{
		type_gRPC = Type_gRPC::GET_ITEMS;
		ModelItems::Item* itemOUT = nullptr;
		ItemsList itemsList(itemOUT, ((ItemsResponse*)gRPC_read)->items().size());

		for (size_t i = 0; i < ((ItemsResponse*)gRPC_read)->items().size(); i++)
		{
			itemOUT[i] << ((ItemsResponse*)gRPC_read)->items().Get(i);
		}

		return itemsList;
	}

	ModelItems::Item* itemOUT = nullptr;
	ItemsList itemsList(itemOUT, -1);
	return itemsList;
}

void Client::CallRPC_GetItems()
{
	Free_gRPC_read();
	gRPC_ClientItems grpcClient;
	gRPC_read = new ItemsResponse(grpcClient.CallRPC_GetItems(accountID, sign, THROW_HOOK));
	type_gRPC = Type_gRPC::GET_ITEMS;
}

void Client::CallRPC_GetBundles(const TYPE_CHAR* outBundle_id, const TYPE_CHAR* outAttached_lemons)
{
	gRPC_ClientItems grpcClient;
	GetBundlesResponse read = grpcClient.CallRPC_GetBundles(accountID, sign, THROW_HOOK);

	game::battlemon::items::WeaponBundle* wb = new game::battlemon::items::WeaponBundle[read.bundles_size()];

	for (size_t i = 0; i < read.bundles_size(); i++)
	{
		wb[i] = read.bundles(i);
	}

}

ModelMM::SearchGameResponseStatus Client::CallRPC_SearchGame(const int& MatchType, const int& MatchMode)
{
	type_gRPC = Type_gRPC::SEARCH_GAME;
	gRPC_ClientMM  gRPC_Client;
	SearchGameResponse read = gRPC_Client.CallRPC_SearchGame(MatchType, MatchMode, THROW_HOOK);

	switch (read.status())
	{
	case SearchGameResponseStatus::OK:
		return ModelMM::SearchGameResponseStatus::OK;
		break;
	case SearchGameResponseStatus::NFT_ERROR:
		return ModelMM::SearchGameResponseStatus::NFT_ERROR;
		break;
	case SearchGameResponseStatus::ALREADY_IN_QUEUE:
		return ModelMM::SearchGameResponseStatus::ALREADY_IN_QUEUE;
		break;
	case SearchGameResponseStatus::GAMEMODE_ERROR:
		return ModelMM::SearchGameResponseStatus::GAMEMODE_ERROR;
		break;
	case SearchGameResponseStatus::INVALID_REQUEST:
		return ModelMM::SearchGameResponseStatus::INVALID_REQUEST;
		break;
	case SearchGameResponseStatus::INTERNAL_ERROR:
		return ModelMM::SearchGameResponseStatus::INTERNAL_ERROR;
		break;
	default:
		return ModelMM::SearchGameResponseStatus::DEFAULT;
		break;
	} 
}

bool Client::CallRPC_AcceptGame(const TYPE_CHAR* lemon_id)
{
	gRPC_ClientMM  gRPC_Client;
	return gRPC_Client.CallRPC_AcceptGame(TYPE_Conv(lemon_id), THROW_HOOK);
}

bool Client::CallRPC_CancelSearch()
{
	gRPC_ClientMM  gRPC_Client;
	return gRPC_Client.CallRPC_CancelSearch(accountID, sign, THROW_HOOK);
}

ItemsList::ItemsList(ModelItems::Item* &items, int size) : size(size), items(nullptr)
{
	if (size != 0)
		this->items = items = new ModelItems::Item[size];
}

ItemsList::ItemsList(const ItemsList& itemsList) : size(itemsList.size), items(nullptr)
{
	if (size != -1)
	{
		items = new ModelItems::Item[size];
		for (int i = 0; i < size; i++)
		{
			items[i] = itemsList.items[i];
		}
	}
}

ItemsList::~ItemsList()
{
	if(items != nullptr)
		delete[]items;
	items = nullptr;
}

PlayerItemsClient::~PlayerItemsClient()
{
	if (items != nullptr)
	{
		for (int i = 0; i < players_items_size; i++)
		{
			free(near_id[i]);
			for (int j = 0; j < nft_ids_size; j++)
			{
				free(items[i][j]);
			}
			delete[] items[i];
		}
		delete[] near_id;
		delete[] items;
		players_items_size = nft_ids_size = -1;
	}
}

ModelItems::Item::~Item()
{
	free(token_id);
	free(media);
	free(owner_id);
	lemon.~LemonModel();
	outfit.~OutfitModel();
}

ModelItems::OutfitModel::~OutfitModel()
{
	free(flavour);
	free(token_id);
}

ModelItems::LemonModel::~LemonModel()
{
	free(exo);
	free(eyes);
	free(head);
	free(teeth);
	free(face);
	cap.~OutfitModel();
	cloth.~OutfitModel();
	fire_arm.~OutfitModel();
	cold_arm.~OutfitModel();
	back.~OutfitModel();
}
