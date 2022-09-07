#ifdef __APPLE__
#include <thread>
#include <chrono>
#include <locale>
#include <codecvt>
#endif

#include "Client.h"
#include "EdKeys.h"
#include "GRPC_Client.h"


#define ED25519 ((EdKeys*)keyPair)

void allocateMemory(const std::string &copy, char* &target)
{
	if (target == nullptr && copy != "")
	{
		target = new char[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), target);
		target[copy.size()] = '\0';
	}
}

#ifdef __APPLE__
std::string convUTF(const char16_t* utp16)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	std::u16string utf16_string(NameProgect);
	std::string UTF8 = convert.to_bytes(utf16_string);
}
#define TYPE_Conv(str) convUTF(str)
#else
#define TYPE_Conv(str) str
#endif


Client::Client(const TYPE_CHAR dir, const TYPE_CHAR inpText, TypeInp type) : keyPair(new EdKeys()), error(nullptr), accountID(nullptr), keyPub58(nullptr), sign(nullptr)
{
	network = nullptr;

	if (type == TypeInp::AUTHORIZATION)
	{
		if (ED25519->LoadKeys(TYPE_Conv(inpText), TYPE_Conv(dir)))
		{
			AuthServiceClient();
			allocateMemory(ED25519->GetPubKey58(), this->keyPub58);
		}
		else
			allocateMemory("error loadKeys", this->error);
	}
	else
	{
		allocateMemory(TYPE_Conv(inpText), network);

		ED25519->GeneratingKeys(error, allocateMemory);
		if (error != nullptr) return;

		RegistrKey();
		if (AuthServiceClient())
		{
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


void Client::RegistrKey()
{
#ifdef __linux__ 
	//linux code goes here
	std::string url = std::string("https://wallet.") + std::string(network) + ".near.org/login?title=rndname&success_url=" + std::string("http://23.22.240.113:80/setId/" + ED25519->GetPubKey58()) + "&public_key=" + ED25519->GetPubKey58();
	std::string cmdComand = "gio open " + url;
#elif _WIN32
	std::string url = std::string("https://wallet.") + std::string(network) + ".near.org/login?title=rndname^&success_url=" + "http://23.22.240.113:80/setId/" + ED25519->GetPubKey58() + "^&public_key=" + ED25519->GetPubKey58();
	std::string cmdComand = "start " + url;
#elif __APPLE__
    std::string url = std::string("https://wallet.") + std::string(network) + ".near.org/login\\?title=rndname\\&success_url=" + "http://23.22.240.113:80/setId/" + ED25519->GetPubKey58() + "\\&public_key=" + ED25519->GetPubKey58();
	std::string cmdComand = "open " + url;
#endif
    system(cmdComand.c_str());
	std::this_thread::sleep_for(std::chrono::nanoseconds(15000000000));
}

bool Client::AuthServiceClient()
{
	std::string PubKey = ED25519->GetPubKey58();
	gRPC_ClientAuth grpcClient;

	int i = 0;
	while (i < 15)
	{
		SendCodeResponse CodeResponse = grpcClient.CallRPCSendCode(PubKey, error, allocateMemory);
	
		std::string signatureMessage = ED25519->MessageSigning(CodeResponse.code());
		VerifyCodeResponse accountID = grpcClient.CallRPCVerifyCode(PubKey, signatureMessage, error, allocateMemory);
	
		if (accountID.near_account_id() != "")
		{
			allocateMemory(signatureMessage, sign);
			allocateMemory(accountID.near_account_id(), this->accountID);
			return true;
		}
		i++;
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000));
	} 

	allocateMemory("error AuthService", this->error);

	return false;
}

void Client::gRPC_SetMyItems(const TYPE_CHAR room_id, int number_of_nft_ids, const TYPE_CHAR* nft_ids)
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


PlayerItemsClient Client::gRPC_getPlayerItems(const TYPE_CHAR room_id, int number_of_near_ids, const TYPE_CHAR* near_ids)
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
	PlayerItemsClient playerItemsClient;
	playerItemsClient.sizeItems = PIR.players_items().size();

	playerItemsClient.near_id = nullptr;
	allocateMemory(PI.near_id(), playerItemsClient.near_id);

	playerItemsClient.items = nullptr;
	playerItemsClient.items = new char** [playerItemsClient.sizeItems];
	for (size_t i = 0; i < PIR.players_items().size(); i++)
	{
		PI = PIR.players_items().Get(i);
		playerItemsClient.nft_ids_size = PI.nft_ids().size();
		playerItemsClient.items[i] = new char* [playerItemsClient.nft_ids_size];
		for (size_t j = 0; j < playerItemsClient.nft_ids_size; j++)
		{
			playerItemsClient.items[i][j] = nullptr;
			allocateMemory(PI.nft_ids().Get(j), playerItemsClient.items[i][j]);

		}
	}

	return playerItemsClient;
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


PlayerItemsClient& operator<<(PlayerItemsClient& PIC, PlayerItems& PI)
{
	allocateMemory(PI.near_id(), PIC.near_id);
	return PIC;
}

ModelItems::Item Client::gRPC_GetItems()
{
	gRPC_ClientItems grpcClient;
	ItemsResponse itemR = grpcClient.CallRPC_GetItems(accountID, sign, error, allocateMemory);
	ModelItems::Item itemOUT;

	itemOUT << itemR.items().Get(0);

	return itemOUT;
}

PlayerItemsClient::~PlayerItemsClient()
{
	free(near_id);
	for (int i = 0; i < sizeItems; i++)
	{
		for (int j = 0; j < nft_ids_size; j++)
			delete[] items[i][j];
		delete[] items[i];
	}
	sizeItems = -1;
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