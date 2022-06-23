#pragma once

class Client
{
	char* accountID;
	const char* network;
	void* keyPair;
	char* keyPub58;
	char* error;

	void RegistrKey();
	bool AuthServiceClient();
public:

	Client(const char* accountID, const char* network);
	Client(const char* network);
	~Client();
	Client() = delete;

	bool(*rewrite)(const char* path);
	char* GetPublicKey() const { return keyPub58; };
	bool IsValidAccount() { return accountID != nullptr; };
	bool IsValidKeys();
	char* GetAccount() { return accountID; };
	char* GetError() { return error; };
};

