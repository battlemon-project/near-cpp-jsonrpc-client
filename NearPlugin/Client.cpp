#include "Helper.h"



#include "include/Client.h"
#include "EdKeys.h"
#include "GRPC_Client.h"

#include <ctime>


Client::Client(const TYPE_CHAR* dir, const TYPE_CHAR* inpText, Type_Call_gRPC::Type_gRPC_Auth type) : keyPair(new EdKeys()), error(nullptr), accountID(nullptr), keyPub58(nullptr), sign(nullptr)
{

	if (type == Type_Call_gRPC::Type_gRPC_Auth::AUTHORIZATION)
	{
		if (ED25519->LoadKeys(TYPE_Conv(inpText), TYPE_Conv(dir)))
		{
			AuthServiceClient(Type_Call_gRPC::Type_gRPC_Auth::AUTHORIZATION);
			Helper::allocateMemory(ED25519->GetPubKey58(), this->keyPub58);
		}
		else
			Helper::allocateMemory("error loadKeys", this->error);
	}
	else
	{

		ED25519->GeneratingKeys(THROW_HOOK);
		if (error != nullptr) return;

		if (AuthServiceClient(Type_Call_gRPC::Type_gRPC_Auth::REGISTRATION))
		{
			ED25519->SaveKeys(this->accountID, TYPE_Conv(dir));
		}

		Helper::allocateMemory(ED25519->GetPubKey58(), this->keyPub58);
	}
}

void Client::saveKey(const TYPE_CHAR* dir)
{
	ED25519->SaveKeys(this->accountID, TYPE_Conv(dir));
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
		Helper::allocateMemory(signatureMessage, sign);
		Helper::allocateMemory(accountID.near_account_id(), accountIDchr);
		return true;
	}
	return false;
}

bool Client::AuthServiceClient(Type_Call_gRPC::Type_gRPC_Auth type)
{
	std::string PubKey = ED25519->GetPubKey58();
	gRPC_ClientAuth grpcClient;

	if (Type_Call_gRPC::Type_gRPC_Auth::REGISTRATION == type)
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

		Helper::allocateMemory("error AuthService", this->error);

		return false;
	}
	else
	{
		return ChekClient(PubKey, grpcClient, this->error, this->keyPair, this->sign, this->accountID);
	}
}




