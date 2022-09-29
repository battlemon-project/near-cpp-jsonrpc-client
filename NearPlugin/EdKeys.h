#pragma once
#include <string>


class EdKeys
{
	uint8_t public_key[32];
	uint8_t private_key[64];
	bool state;
	void SaveK(const std::string& filename, void* key, size_t size);
	bool LoadK(const std::string& path, const std::string& accountID, const std::string& filetype, void* key, size_t size);

public:
	EdKeys();
	~EdKeys();
	void GeneratingKeys(char*& error, void(*errorH)(const std::string& copy, char*& error));
	std::string MessageSigning(const std::string &messageChr);
	void SaveKeys(const std::string &accountID, std::string dir);
	bool LoadKeys(const std::string &accountID, std::string dir);
	std::string GetPubKey58()const;
	std::string GetPrKey58()const;
	bool IsValid() const;
};

