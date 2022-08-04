#include "Client.h"
#include "EdKeys.h"
#include "GRPC_Client.h"

#include <thread>
#include <chrono>



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

Client::Client(const char* dir, const char* inpText, TypeInp type) :  keyPair(new EdKeys()), error(nullptr), accountID(nullptr), keyPub58(nullptr), sing(nullptr)
{
	if (type == TypeInp::AUTHORIZATION)
	{
		network = nullptr;
		if (ED25519->LoadKeys(inpText, dir))
		{
			AuthServiceClient();
			allocateMemory(ED25519->GetPubKey58(), this->keyPub58);
		}
		else
			allocateMemory("error loadKeys", this->error);
	}
	else
	{
		network = (char*)inpText;
		ED25519->GeneratingKeys(error, allocateMemory);
		if (error != nullptr) return;

		RegistrKey();
		if (AuthServiceClient())
		{
			ED25519->SaveKeys(this->accountID, dir);
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
    std::string url = std::string("https://wallet.") + std::string(network) + ".near.org/login\?title=rndname\&success_url=" + "http://23.22.240.113:80/setId/" + ED25519->GetPubKey58() + "\&public_key=" + ED25519->GetPubKey58();
	std::string cmdComand = "open " + url;
#endif
    system(cmdComand.c_str());
	std::this_thread::sleep_for(std::chrono::nanoseconds(15000000000));
}

bool Client::AuthServiceClient()
{
	std::string PubKey = ED25519->GetPubKey58();
	GRPC_Client grpcClient;
	grpcClient.setChannel((grpc::CreateChannel("game.battlemon.com", grpc::SslCredentials(grpcClient.getSslOptions()))), Protocol::AUTHS);

	int i = 0;
	do
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
	} while (i < 15);

	allocateMemory("error AuthService", this->error);

	return false;
}
