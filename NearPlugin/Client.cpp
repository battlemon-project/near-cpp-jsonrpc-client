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

Client::Client(const char* inpText, bool type) :  keyPair(new EdKeys()), error(nullptr), accountID(nullptr), keyPub58(nullptr), sing(nullptr)
{
	if (type)
	{
		network = nullptr;
		if (((EdKeys*)keyPair)->LoadKeys(std::string(inpText)))
		{
			AuthServiceClient();
			allocateMemory(((EdKeys*)keyPair)->GetPubKey58(), this->keyPub58);
		}
		else
			allocateMemory("error loadKeys", this->error);
	}
	else
	{
		network = (char*)inpText;
		((EdKeys*)keyPair)->GeneratingKeys(error, allocateMemory);
		if (error != nullptr) return;

		RegistrKey();
		allocateMemory(((EdKeys*)keyPair)->GetPubKey58(), this->keyPub58);

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
		delete keyPair;
		keyPair = nullptr;
	}
	free(accountID);
	free(keyPub58);
	free(error);
	free(sing);
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
#elif __APPLE__
	std::string url = std::string("\"https://wallet.") + std::string(network) + ".near.org/login?title=rndname&success_url=" + std::string("http://23.22.240.113:80/setId/" + ((EdKeys*)keyPair)->GetPubKey58()) + "&public_key=" + ((EdKeys*)keyPair)->GetPubKey58() + "\'";
	std::string cmdComand = "open " + url;
#endif
	system(cmdComand.c_str());

	std::this_thread::sleep_for(std::chrono::nanoseconds(15000000000));
	if (AuthServiceClient())
	{
		((EdKeys*)keyPair)->SaveKeys(this->accountID);
	}
}

bool Client::AuthServiceClient()
{
	std::string PubKey = ((EdKeys*)keyPair)->GetPubKey58();
	GRPC_Client grpcClient;
	grpcClient.setChannel((grpc::CreateChannel("game.battlemon.com", grpc::SslCredentials(grpcClient.getSslOptions()))), AUTHS);

	int i = 0;
	do
	{
		SendCodeResponse CodeResponse = grpcClient.CallRPCSendCode(PubKey, error, allocateMemory);
	
		std::string signatureMessage = ((EdKeys*)keyPair)->MessageSigning(grpcClient.CallRPCSendCode(((EdKeys*)keyPair)->GetPubKey58(),error, allocateMemory).code().c_str());
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


bool Client::VerifySing()
{
	GRPC_Client grpcClient;
	grpcClient.setChannel((grpc::CreateChannel("0n64i8m4o8.execute-api.us-east-1.amazonaws.com", grpc::SslCredentials(grpcClient.getSslOptions()))), INTERNALAUTHS);


	return false;
}