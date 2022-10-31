#pragma once
#include "include/ModelItem.h"
#if PLATFORM_WINDOWS
#define __APPLE__ 0
#define _WIN32 1
#define __linux__ 0
#elif PLATFORM_MAC
#define __APPLE__ 1
#define _WIN32 0
#define __linux__ 0
#elif PLATFORM_LINUX
#define __APPLE__ 0
#define _WIN32 0
#define __linux__ 1
#endif

#if __linux__
#define _GLIBCXX_USE_CXX11_ABI 0
#endif

#if _WIN32
#define TYPE_CHAR char
#else
#define TYPE_CHAR char16_t
#endif


class Client
{
	char* accountID;
	char* sign;
	void* keyPair;
	char* keyPub58;
	char* error;
	
public:

	Client(const TYPE_CHAR* dir, const TYPE_CHAR* inpText, Type_Call_gRPC::Type_gRPC_Auth type);

	~Client();
	Client() = delete;

	char* GetPublicKey() const { return keyPub58; };
	bool IsValidAccount() { return accountID != nullptr; };
	bool IsValidKeys();
	char* GetAccount() const { return accountID; };
	char* GetError() const { return error; };
	const char* GetSing() const { return sign; };


	//mm.proto

	void saveKey(const TYPE_CHAR* dir);
private:
	bool AuthServiceClient(Type_Call_gRPC::Type_gRPC_Auth type);
};