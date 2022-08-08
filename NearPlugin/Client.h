#pragma once
#define DEBUG

#if defined(__APPLE__) || defined(PLATFORM_MAC)
#define TYPE_CHAR char16_t*
#else
#define TYPE_CHAR char*
#endif

enum class TypeInp
{
	AUTHORIZATION,
	REGISTRATION,
};


class Client
{
#ifdef DEBUG
	const TYPE_CHAR NameProgect;
#endif
	char* accountID;
	char* network;
	char* sing;
	void* keyPair;
	char* keyPub58;
	char* error;

	void RegistrKey();
	bool AuthServiceClient();
public:

	Client(const TYPE_CHAR NameProgect, const char* inpText, TypeInp type);

	~Client();
	Client() = delete;

	char* GetPublicKey() const { return keyPub58; };
	bool IsValidAccount() { return accountID != nullptr; };
	bool IsValidKeys();
	char* GetAccount() { return accountID; };
	char* GetError() { return error; };
	const char* GetSing(){ return sing; };
};

