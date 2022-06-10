#include "Client.h"
#include <iostream>
#include <fstream>
#include "Connector.h"

Client::Client(const char* accountID, const char* network): network(network)
{
	try
	{
		keyPair.LoadKeys(accountID, network); 
		{
			std::string str(accountID);

			this->accountID = new char[str.size() + 1];
			std::copy(str.begin(), str.end(), this->accountID);
			this->accountID[str.size()] = '\0';
		}
	}
	catch (const std::exception& e)
	{
		keyPair.GeneratingKeys();
		RegistrKey();
		keyPair.SaveKeys(this->accountID, this->network);
	}

}

Client::Client(const char* network):network(network)
{
	accountID = nullptr;
	keyPair.GeneratingKeys();
	RegistrKey();
	
	keyPair.SaveKeys(this->accountID, this->network);
}

Client::~Client()
{
	delete[]accountID;
	accountID = nullptr;
}

char* Client::GetPublicKey() const
{
	return keyPair.GetStrPubKey();
}

void Client::RegistrKey()
{
#ifdef __linux__ 
	//linux code goes here
	std::string url = std::string("https://wallet.") + std::string("testnet") + ".near.org/login?title=rndname&success_url=" + std::string("http://207.154.208.184:3000/setId/" + keyPair.GetStrPubKey()) + "&public_key=" + keyPair.GetStrPubKey();
	std::string cmdComand = "gio open " + url;

#elif _WIN32
	std::string url = std::string("https://wallet.") + std::string("testnet") + ".near.org/login?title=rndname^&success_url=" + std::string("http://207.154.208.184:3000/setId/" + std::string(keyPair.GetStrPubKey())) + "^&public_key=" + std::string(keyPair.GetStrPubKey());
	std::string cmdComand = "start " + url;
	system(cmdComand.c_str());
#else

#endif

	std::string accountID = Connector::getResponse("207.154.208.184", "3000", "/getId/" + std::string(keyPair.GetStrPubKey()));

	if (accountID != "{}")
	{
		std::size_t start = accountID.find(":\"") + 2;
		std::size_t end = accountID.find("\"}");
		std::size_t size = end - start;

		this->accountID = new char[size];
		std::size_t length = accountID.copy(this->accountID, size, start);
		this->accountID[length] = '\0';
	}
}
