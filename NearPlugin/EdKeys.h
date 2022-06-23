#pragma once
#include <string>


class EdKeys
{
	uint8_t public_key[32];
	uint8_t private_key[64];
	void SaveK(std::string filename, void* key, size_t size);
	void LoadK(std::string path, std::string accountID, std::string filetype, void* key, size_t size);

public:
	EdKeys();
	~EdKeys();
	void GeneratingKeys(char*& error, void(*errorH)(const std::string& copy, char*& error));
	std::string MessageSigning(const std::string &messageChr);
	void SaveKeys(std::string accountID);
	void LoadKeys(std::string accountID);
	std::string GetPubKey58()const;
};

