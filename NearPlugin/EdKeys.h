#pragma once
#include <string>


class EdKeys
{
	uint8_t public_key[32];
	uint8_t private_key[64];
	void SaveK(const std::string& filename, void* key, size_t size);
	bool LoadK(const std::string& path, const std::string& accountID, const std::string& filetype, void* key, size_t size);
    std::string getAppData();

public:
	EdKeys();
	~EdKeys();
	void GeneratingKeys(char*& error, void(*errorH)(const std::string& copy, char*& error));
	std::string MessageSigning(const std::string &messageChr);
	void SaveKeys(const std::string &accountID, std::string NameProgect);
	bool LoadKeys(const std::string &accountID, std::string NameProgect);
	std::string GetPubKey58()const;
	std::string GetPrKey58()const;
};

