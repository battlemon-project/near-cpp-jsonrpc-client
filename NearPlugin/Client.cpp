#include "Helper.h"



#include "include/Client.h"
#include "EdKeys.h"
#include "GRPC_Client.h"

#include <ctime>


Client::Client(const TYPE_CHAR* dir, const TYPE_CHAR* inpText, Type_Call_gRPC::Type_gRPC_Auth type) : keyPair(new EdKeys()), error(nullptr), accountID(nullptr), keyPub58(nullptr)
{

	if (type == Type_Call_gRPC::Type_gRPC_Auth::AUTHORIZATION)
	{
		if (ED25519->LoadKeys(TYPE_Conv(inpText), TYPE_Conv(dir)))
		{
			//AuthServiceClient(Type_Call_gRPC::Type_gRPC_Auth::AUTHORIZATION);
			Helper::allocateMemory(ED25519->GetPubKey58(), this->keyPub58);
		}
		else
			Helper::allocateMemory("error loadKeys", this->error);
	}
	else
	{

		ED25519->GeneratingKeys(THROW_HOOK);
		if (error != nullptr) return;

		//if (AuthServiceClient(Type_Call_gRPC::Type_gRPC_Auth::REGISTRATION))
		//{
		//	ED25519->SaveKeys(this->accountID, TYPE_Conv(dir));
		//}

		Helper::allocateMemory(ED25519->GetPubKey58(), this->keyPub58);
	}
}

const char* Client::GetSing() const
{
	return ED25519->GetSign().c_str();
}

void Client::saveKey(const TYPE_CHAR* dir)
{
	ED25519->SaveKeys(this->accountID, TYPE_Conv(dir));
}

void Client::saveSign(const TYPE_CHAR* dir)
{
	ED25519->SaveSign(this->accountID, TYPE_Conv(dir));
}

Client::~Client()
{
	if (keyPair != nullptr)
	{
		delete ED25519;
		keyPair = nullptr;
	}
	Helper::free(&accountID);
	Helper::free(&keyPub58);
	Helper::free(&error);
}

bool Client::IsValidKeys()
{
	{ return ED25519->GetPubKey58() != ""; };
}

bool ChekClient(const std::string &PubKey, gRPC_ClientAuth &grpcClient, char* &error, void* &keyPair, char* &accountIDchr)
{
	SendCodeResponse CodeResponse;
	std::string signatureMessage;
	if (ED25519->GetSign() == "")
	{
		CodeResponse = grpcClient.CallRPCSendCode(PubKey, THROW_HOOK);
		signatureMessage = ED25519->MessageSigning(CodeResponse.code());
	}
	else
		signatureMessage = ED25519->GetSign();

	VerifyCodeResponse accountID = grpcClient.CallRPCVerifyCode(PubKey, signatureMessage, THROW_HOOK);

	if (error != nullptr)
	{
		Helper::free(&error);
		CodeResponse = grpcClient.CallRPCSendCode(PubKey, THROW_HOOK);
		signatureMessage = ED25519->MessageSigning(CodeResponse.code());
		accountID = grpcClient.CallRPCVerifyCode(PubKey, signatureMessage, THROW_HOOK);
	}
	else
		if (accountID.near_account_id() != "")
		{
			ED25519->SetSign(signatureMessage);
			Helper::allocateMemory(accountID.near_account_id(), accountIDchr);
			return true;
		}
	return false;
}

bool Client::AuthServiceClient(const TYPE_CHAR* url)
{
	std::string PubKey = ED25519->GetPubKey58();
	gRPC_ClientAuth grpcClient(true, TYPE_Conv(url));
	return ChekClient(PubKey, grpcClient, this->error, this->keyPair, this->accountID);
}




