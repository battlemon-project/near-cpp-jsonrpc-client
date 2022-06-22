#include "Client.h"
#include <iostream>
#include <fstream>
#include "EdKeys.h"
#include "GRPC_Client.h"



Client::Client(const char* accountID, const char* network): network(network), keyPair(new EdKeys())
{
	this->accountID = nullptr;
	try
	{
		((EdKeys*)keyPair)->LoadKeys(std::string(accountID) + "." + network);
		AuthServiceClient();
	}
	catch (const std::exception& e)
	{
		((EdKeys*)keyPair)->GeneratingKeys();
		RegistrKey();
	}

}

Client::Client(const char* network):network(network), keyPair(new EdKeys())
{
	accountID = nullptr;
	((EdKeys*)keyPair)->GeneratingKeys();
	RegistrKey();
}

Client::~Client()
{
	if (keyPair != nullptr)
	{
		delete[]accountID;
		accountID = nullptr;
	}
	if(keyPair != nullptr)
	{
		delete[]keyPair;
		keyPair = nullptr;
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

	if (AuthServiceClient())
	{
		((EdKeys*)keyPair)->SaveKeys(this->accountID);
		return;
	}
	else
		this->accountID = nullptr;
	((EdKeys*)keyPair)->SaveKeys("");
}

bool Client::AuthServiceClient()
{
	std::string PubKey = ((EdKeys*)keyPair)->GetPubKey58();
	GRPC_Client grpcClient;
	grpcClient.setChannel((grpc::CreateChannel("game.battlemon.com", grpc::SslCredentials(grpcClient.getSslOptions()))));
	
	std::this_thread::sleep_for(std::chrono::nanoseconds(15000000000));
	int i = 0;
	do
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000));
	
		SendCodeResponse CodeResponse = grpcClient.CallRPCSendCode(PubKey);
		if (!CodeResponse.IsInitialized())
			throw std::runtime_error("Invalid CodeResponse");
	
		std::string signatureMessage = ((EdKeys*)keyPair)->MessageSigning(grpcClient.CallRPCSendCode(((EdKeys*)keyPair)->GetPubKey58()).code().c_str());
		VerifyCodeResponse accountID = grpcClient.CallRPCVerifyCode(PubKey, signatureMessage);
	
		if (!accountID.IsInitialized())
			throw std::runtime_error("Invalid VerifyCodeResponse");
	
		if (accountID.near_account_id() != "")
		{
			this->accountID = new char[accountID.near_account_id().size() + 1];
			std::copy(accountID.near_account_id().begin(), accountID.near_account_id().end(), this->accountID);
			this->accountID[accountID.near_account_id().size()] = '\0';
			return true;
		}
		i++;
	} while (i < 5);
	return false;
}

