#if defined(__linux__) || defined(__APPLE__)
#include <thread>
#include <chrono>
#include <locale>
#include <codecvt>
#endif

#include "Client.h"
#include "EdKeys.h"
#include "GRPC_Client.h"
//#include <ctime>


#define ED25519 ((EdKeys*)keyPair)
#define REDIRECT "https://game.battlemon.com/near"

void allocateMemory(const std::string &copy, char* &target)
{
	if (target == nullptr && copy != "")
	{
		target = new char[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), target);
		target[copy.size()] = '\0';
	}
}

#if defined(__linux__) || defined(__APPLE__)
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



Client::Client(const TYPE_CHAR* dir, const TYPE_CHAR* inpText, TypeInp type) : keyPair(new EdKeys()), error(nullptr), accountID(nullptr), keyPub58(nullptr), sign(nullptr)
{
	if (type == TypeInp::AUTHORIZATION)
	{
		ED25519->LoadKeys(TYPE_Conv(inpText), TYPE_Conv(dir));
	}
	else
	{
		ED25519->GeneratingKeys(error, allocateMemory);
	}

	allocateMemory(ED25519->GetPubKey58(), this->keyPub58);
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
	SendCodeResponse CodeResponse = grpcClient.CallRPCSendCode(PubKey, error, allocateMemory);

	std::string signatureMessage = ED25519->MessageSigning(CodeResponse.code());
	VerifyCodeResponse accountID = grpcClient.CallRPCVerifyCode(PubKey, signatureMessage, error, allocateMemory);

	if (accountID.near_account_id() != "")
	{
		allocateMemory(signatureMessage, sign);
		allocateMemory(accountID.near_account_id(), accountIDchr);
		return true;
	}
	return false;
}

bool Client::AuthServiceClient()
{
	free(error);
	if (ED25519->IsValid())
	{
		std::string PubKey = ED25519->GetPubKey58();
		gRPC_ClientAuth grpcClient;

		if (ChekClient(PubKey, grpcClient, this->error, this->keyPair, this->sign, this->accountID))
		{
			return true;
		}
	}
	return false;
}

void Client::saveKey(const TYPE_CHAR* dir)
{
	ED25519->SaveKeys(this->accountID, TYPE_Conv(dir));
}

void Client::gRPC_SetMyItems(const TYPE_CHAR* room_id, int number_of_nft_ids, const TYPE_CHAR** nft_ids)
{
	std::string room_idStr = TYPE_Conv(room_id);

	std::string* nft_idsStr = new std::string[number_of_nft_ids];
	for (int i = 0; i < number_of_nft_ids; i++)
	{
		nft_idsStr[i] = TYPE_Conv(nft_ids[i]);
	}

	gRPC_ClientItems grpcClient;
	grpcClient.CallRPC_SetMyItems(room_idStr, number_of_nft_ids, nft_idsStr, accountID, sign, error, allocateMemory);
}


void Client::gRPC_getPlayerItems(const TYPE_CHAR* room_id, int number_of_near_ids, const TYPE_CHAR** near_ids, PlayerItemsClient& playerItemsClient)
{
	std::string room_idStr = TYPE_Conv(room_id);
	std::string* near_idsStr = new std::string[number_of_near_ids];
	for (int i = 0; i < number_of_near_ids; i++)
	{
		near_idsStr[i] = TYPE_Conv(near_ids[i]);
	}

	gRPC_ClientItems grpcClient;
	PlayersItemsResponse PIR = grpcClient.CallRPC_GetPlayersItems(room_idStr, number_of_near_ids, near_idsStr, error, allocateMemory);

	PlayerItems PI;
	playerItemsClient.players_items_size = PIR.players_items().size();

	playerItemsClient.near_id = new char* [playerItemsClient.players_items_size];
	playerItemsClient.items = new char** [playerItemsClient.players_items_size];
	for (size_t i = 0; i < PIR.players_items().size(); i++)
	{
		PI = PIR.players_items().Get(i);
		playerItemsClient.near_id[i] = nullptr;
		allocateMemory(PI.near_id(), playerItemsClient.near_id[i]);
		playerItemsClient.nft_ids_size = PI.nft_ids().size();
		playerItemsClient.items[i] = new char* [playerItemsClient.nft_ids_size];
		for (size_t j = 0; j < playerItemsClient.nft_ids_size; j++)
		{
			playerItemsClient.items[i][j] = nullptr;
			allocateMemory(PI.nft_ids().Get(j), playerItemsClient.items[i][j]);

		}
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

ItemsList Client::gRPC_GetItems()
{
	gRPC_ClientItems grpcClient;
	ItemsResponse itemR = grpcClient.CallRPC_GetItems(accountID, sign, error, allocateMemory);

	ModelItems::Item* itemOUT = nullptr;
	ItemsList itemsList(itemOUT, itemR.items().size());

	for (size_t i = 0; i < itemR.items().size(); i++)
	{
		itemOUT[i] << itemR.items().Get(i);
	}

	return itemsList;
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
	delete[]items;
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
