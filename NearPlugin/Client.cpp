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
	if (target == nullptr)
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


Client::Client(const TYPE_CHAR dir, const TYPE_CHAR inpText, TypeInp type) : keyPair(new EdKeys()), error(nullptr), accountID(nullptr), keyPub58(nullptr), sing(nullptr)
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
	free(sing);
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
	while (i < 15);
	{
		SendCodeResponse CodeResponse = grpcClient.CallRPCSendCode(PubKey, error, allocateMemory);
	
		std::string signatureMessage = ED25519->MessageSigning(CodeResponse.code());
		VerifyCodeResponse accountID = grpcClient.CallRPCVerifyCode(PubKey, signatureMessage, error, allocateMemory);
	
		if (accountID.near_account_id() != "")
		{
			allocateMemory(signatureMessage, sing);
			allocateMemory(accountID.near_account_id(), this->accountID);
			return true;
		}
		i++;
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000));
	} 

	allocateMemory("error AuthService", this->error);

	return false;
}

ModelItems::OutfitKind& operator<<(ModelItems::OutfitKind& OModel, const game::battlemon::items::OutfitKind& OModel2)
{
	switch (OModel2)
	{
	case 0:
		OModel= ModelItems::OutfitKind::CAP;
		break;
	case 1:
		OModel = ModelItems::OutfitKind::CLOTH;
		break;
	case 2:
		OModel = ModelItems::OutfitKind::FIRE_ARM;
		break;
	case 3:
		OModel = ModelItems::OutfitKind::COLD_ARM;
		break;
	case 4:
		OModel = ModelItems::OutfitKind::BACK;
		break;
	default:
		OModel = ModelItems::OutfitKind::DEFAULT;
		break;
	}
	return OModel;
}

ModelItems::OutfitModel& operator<<(ModelItems::OutfitModel& OModel, const game::battlemon::items::OutfitModel& OModel2)
{
	allocateMemory(OModel2.flavour(), OModel.flavour);
	allocateMemory(OModel2.token_id(), OModel.token_id);
	OModel.kind << OModel2.kind();
	return OModel;
}

ModelItems::LemonModel& operator<<(ModelItems::LemonModel& LModel, const game::battlemon::items::LemonModel& LModel2)
{
	LModel.cap << LModel2.cap();
	LModel.cloth << LModel2.cloth();
	allocateMemory(LModel2.exo(), LModel.exo);
	allocateMemory(LModel2.eyes(), LModel.eyes);
	allocateMemory(LModel2.head(), LModel.head);
	allocateMemory(LModel2.teeth(), LModel.teeth);
	allocateMemory(LModel2.face(), LModel.face);
	LModel.fire_arm << LModel2.fire_arm();
	LModel.cold_arm << LModel2.cold_arm();
	LModel.back << LModel2.back();
	return LModel;
}

ModelItems::ItemM& operator<<(ModelItems::ItemM& IM, game::battlemon::items::Item& mesI)
{
	allocateMemory(mesI.token_id(), IM.token_id);
	allocateMemory(mesI.media(), IM.media);
	allocateMemory(mesI.owner_id(), IM.owner_id);
	IM.lemon << mesI.lemon();
	IM.outfit << mesI.outfit();
	return IM;
}

PlayerItemsClient& operator<<(PlayerItemsClient& PIC, PlayerItems& PI)
{
	allocateMemory(PI.near_id(), PIC.near_id);
	return PIC;
}




void Client::gRPC_SetMyItems(const TYPE_CHAR room_id, int id_Player, const TYPE_CHAR nft_ids)
{
	std::string room_idStr = TYPE_Conv(room_id);
	std::string near_idsStr = TYPE_Conv(nft_ids);

	gRPC_ClientItems grpcClient;
	grpcClient.CallRPC_SetMyItems(room_idStr, id_Player, near_idsStr, error, allocateMemory);
}


PlayerItemsClient Client::gRPC_getPlayerItems(const TYPE_CHAR room_id, int id_Player, const TYPE_CHAR near_ids)
{
	std::string room_idStr = TYPE_Conv(room_id);
	std::string near_idsStr = TYPE_Conv(near_ids);

	gRPC_ClientItems grpcClient;

	PlayerItems PI = grpcClient.CallRPC_GetPlayersItems(room_idStr, id_Player, near_idsStr, error, allocateMemory);

	PlayerItemsClient playerItemsClient;
	playerItemsClient << PI;

	return playerItemsClient;
}

PlayerItemsClient::~PlayerItemsClient()
{
	free(near_id);
	items.~ItemM();
}

ModelItems::ItemM::~ItemM()
{
	free(token_id );
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