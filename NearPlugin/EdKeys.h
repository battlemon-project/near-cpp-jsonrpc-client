#pragma once
#include <string>

class EdKeysInterfase
{

	char* StrPubKey58;

public:
	EdKeysInterfase();
	~EdKeysInterfase();
	void GeneratingKeys();
	std::string MessageSigning(const char* messageChr);
	void SaveKeys(const char* accountID, const char* network);
	void LoadKeys(const char* accountID, const char* network);
	char* GetStrPubKey()const;
};

