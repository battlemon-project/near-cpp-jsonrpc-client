#include "Client.h"
#include "EdKeys.h"
#include "GRPC_Client.h"

void allocateMemory(const std::string &copy, char* &target)
{
	if (target == nullptr)
	{
		target = new char[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), target);
		target[copy.size()] = '\0';
	}
}

Client::Client(const char* accountID, const char* network) : network(network), keyPair(new EdKeys()), error(nullptr), accountID(nullptr), keyPub58(nullptr)
{

	if (((EdKeys*)keyPair)->LoadKeys(std::string(accountID) + "." + network))
	{
		AuthServiceClient();
		allocateMemory(((EdKeys*)keyPair)->GetPubKey58(), this->keyPub58);
	}
	else
		allocateMemory("error loadKeys", this->error);
}

Client::Client(const char* network):network(network), keyPair(new EdKeys()), error(nullptr), accountID(nullptr), keyPub58(nullptr)
{
	((EdKeys*)keyPair)->GeneratingKeys(error, allocateMemory);
	if (error != nullptr) return;

	RegistrKey();
	allocateMemory(((EdKeys*)keyPair)->GetPubKey58(), this->keyPub58);
}

Client::~Client()
{
	if (accountID != nullptr)
	{
		delete[]accountID;
		accountID = nullptr;
	}
	if(keyPair != nullptr)
	{
		delete keyPair;
		keyPair = nullptr;
	}
	if (keyPub58 != nullptr)
	{
		delete[]keyPub58;
		keyPub58 = nullptr;
	}
	if (error != nullptr)
	{
		delete[]error;
		error = nullptr;
	}
}

bool Client::IsValidKeys()
{
	{ return ((EdKeys*)keyPair)->GetPubKey58() != ""; };
}

void Client::RegistrKey()
{
#ifdef __linux__ 
	//linux code goes here
	std::string url = std::string("https://wallet.") + std::string(network) + ".near.org/login?title=rndname&success_url=" + std::string("http://23.22.240.113:80/setId/" + ((EdKeys*)keyPair)->GetPubKey58()) + "&public_key=" + ((EdKeys*)keyPair)->GetPubKey58();
	std::string cmdComand = "gio open " + url;

#elif _WIN32
	std::string url = std::string("https://wallet.") + std::string(network) + ".near.org/login?title=rndname^&success_url=" + "http://23.22.240.113:80/setId/" + ((EdKeys*)keyPair)->GetPubKey58() + "^&public_key=" + ((EdKeys*)keyPair)->GetPubKey58();
	std::string cmdComand = "start " + url;
	system(cmdComand.c_str());
#else

#endif

	std::this_thread::sleep_for(std::chrono::nanoseconds(15000000000));
	if (AuthServiceClient())
	{
		return;
	}
	delete[] this->error;
	allocateMemory("error RegistrKey", this->error);
}

bool Client::AuthServiceClient()
{
	std::string PubKey = ((EdKeys*)keyPair)->GetPubKey58();
	GRPC_Client grpcClient;
	grpcClient.setChannel((grpc::CreateChannel("game.battlemon.com", grpc::SslCredentials(grpcClient.getSslOptions()))));

	int i = 0;
	do
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000));
	
		SendCodeResponse CodeResponse = grpcClient.CallRPCSendCode(PubKey, error, allocateMemory);
	
		std::string signatureMessage = ((EdKeys*)keyPair)->MessageSigning(grpcClient.CallRPCSendCode(((EdKeys*)keyPair)->GetPubKey58(),error, allocateMemory).code().c_str());
		VerifyCodeResponse accountID = grpcClient.CallRPCVerifyCode(PubKey, signatureMessage, error, allocateMemory);
	
		if (accountID.near_account_id() != "")
		{
			((EdKeys*)keyPair)->SaveKeys(this->accountID);
			allocateMemory(accountID.near_account_id(), this->accountID);
			return true;
		}
		i++;
	} while (i < 15);

	allocateMemory("error AuthService", this->error);

	return false;
}

