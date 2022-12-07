#include "Helper.h"

#include "include/Client.h"
#include "EdKeys.h"


Client::Client(const TYPE_CHAR* dir, const TYPE_CHAR* inpText, TypeClient& type) : keyPair(new EdKeys()), error(nullptr), accountID(nullptr), keyPub58(nullptr)
{

	if (type == TypeClient::OLD)
	{
		if (ED25519->LoadKeys(inpText, dir))
		{
			Helper::allocateMemory(ED25519->GetPubKey58(), this->keyPub58);
		}
		else
			Helper::allocateMemory(_T("error loadKeys"), this->error);
	}
	else
	{
		ED25519->GeneratingKeys(THROW_HOOK);
		if (error != nullptr) return;
		Helper::allocateMemory(ED25519->GetPubKey58(), this->keyPub58);
	}
}

const char* Client::GetSing() const
{
	return (const char*)ED25519->GetSign().c_str();
}

void Client::SaveKey(const TYPE_CHAR* dir)
{
	ED25519->SaveKeys(this->accountID, dir);
}

void Client::SaveSign(const TYPE_CHAR* dir, const TYPE_CHAR* sign)
{
	ED25519->SaveSign(this->accountID, dir, sign);
}

Client::~Client()
{
	if (keyPair != nullptr)
	{
		delete ED25519;
		keyPair = nullptr;
	}
}

bool Client::IsValidKeys()
{
	return ED25519->GetPubKey58() != _T("");
}

const char* Client::CreateMessageNewSign(const char* Message)
{
	return (const char*)ED25519->MessageSigning(Message).c_str();
}




