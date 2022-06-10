#include "EdKeys.h"

#include "cryptlib.h"
#include "xed25519.h"
#include "filters.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"
#include "pem.h"
#include <iostream>

struct EdKeys
{
	CryptoPP::AutoSeededRandomPool prng;
	CryptoPP::ed25519::Signer signer;

	std::string base58encode(CryptoPP::Integer plain, int zeros) 
	{
		std::string b58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
		std::string adr;
		CryptoPP::Integer remainder, temp;
		while (plain != 0)
		{
			plain.Divide(remainder, temp, plain, 58);
			plain = temp;
			adr = b58[remainder.ConvertToLong()] + adr;
		}
		while (zeros)
		{
			adr = "1" + adr;
			zeros--;
		}
		return adr;
	}
	std::string toString(const CryptoPP::ed25519PublicKey& pubKey)
	{
		return std::string("ed25519:") + base58encode(pubKey.GetPublicElement(), 0);
	}
};

static EdKeys edKeys;

EdKeysInterfase::EdKeysInterfase()
{
	StrPubKey58 = nullptr;
}

EdKeysInterfase::~EdKeysInterfase()
{
	delete[]StrPubKey58;
}

void EdKeysInterfase::GeneratingKeys()
{
	edKeys.signer.AccessPrivateKey().GenerateRandom(edKeys.prng);
	const CryptoPP::ed25519PrivateKey& privKey = dynamic_cast<const CryptoPP::ed25519PrivateKey&>(edKeys.signer.GetPrivateKey());

	CryptoPP::ed25519::Verifier verifier = CryptoPP::ed25519::Verifier(edKeys.signer);
	const CryptoPP::ed25519PublicKey& pubKey = dynamic_cast<const CryptoPP::ed25519PublicKey&>(verifier.GetPublicKey());


	const CryptoPP::Integer& x = privKey.GetPrivateExponent();
	std::cout << x << std::endl;
	const CryptoPP::Integer& y = pubKey.GetPublicElement();
	std::cout << std::hex << y << std::endl;

	{
		std::string str = edKeys.toString(pubKey);

		this->StrPubKey58 = new char[str.size() + 1];
		std::copy(str.begin(), str.end(), this->StrPubKey58);
		this->StrPubKey58[str.size()] = '\0';
	}
}

void EdKeysInterfase::SaveKeys(const char* accountID, const char* network)
{
	CryptoPP::HexEncoder* encoder;
	if (accountID != nullptr)
	{
		encoder = new CryptoPP::HexEncoder(new CryptoPP::FileSink(std::string(std::string(accountID) + std::string(network)+ std::string(".bin")).c_str()));
	}
	else
	{
		encoder = new CryptoPP::HexEncoder(new CryptoPP::FileSink(std::string(std::string("empty") + std::string(network) + std::string(".bin")).c_str()));
	}

	std::cout << "Private key: ";
	edKeys.signer.GetPrivateKey().Save(*encoder);
	std::cout << "\n" << std::endl;
}

void EdKeysInterfase::LoadKeys(const char* accountID, const char* network)
{
	std::string filename = std::string(std::string(accountID) + std::string(network) + std::string(".bin"));
	{
		std::ifstream iff(filename.c_str());
		if (iff.bad() == true)
		{
			filename = std::string("empty") + std::string(network) + std::string(".bin");
			iff.open(filename.c_str());
			if (iff.bad() == true)
				throw std::runtime_error("Invalid file");
		}
	}

	CryptoPP::FileSource fs1(filename.c_str(), true);
	edKeys.signer.AccessPrivateKey().Load(fs1);

	bool valid = edKeys.signer.GetPrivateKey().Validate(edKeys.prng, 3);
	if (valid == false)
	{
		throw std::runtime_error("Invalid private key");
	}

	CryptoPP::ed25519::Verifier verifier = CryptoPP::ed25519::Verifier(edKeys.signer);
	const CryptoPP::ed25519PublicKey& pubKey = dynamic_cast<const CryptoPP::ed25519PublicKey&>(verifier.GetPublicKey());

	{
		std::string str = edKeys.toString(pubKey);

		this->StrPubKey58 = new char[str.size() + 1];
		std::copy(str.begin(), str.end(), this->StrPubKey58);
		this->StrPubKey58[str.size()] = '\0';
	}
}

char* EdKeysInterfase::GetStrPubKey() const
{
	return StrPubKey58;
}
