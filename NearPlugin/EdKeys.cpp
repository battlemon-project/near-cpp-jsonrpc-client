#include "EdKeys.h"

#include "cryptlib.h"
#include "xed25519.h"
#include "filters.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"
#include "base64.h"

struct EdKeys
{
	static CryptoPP::AutoSeededRandomPool prng;
	static CryptoPP::ed25519::Signer signer;

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
CryptoPP::AutoSeededRandomPool EdKeys::prng;
CryptoPP::ed25519::Signer EdKeys::signer;

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

	CryptoPP::ed25519::Verifier verifier = CryptoPP::ed25519::Verifier(edKeys.signer);
	const CryptoPP::ed25519PublicKey& pubKey = dynamic_cast<const CryptoPP::ed25519PublicKey&>(verifier.GetPublicKey());

	{
		std::string str = edKeys.toString(pubKey);

		this->StrPubKey58 = new char[str.size() + 1];
		std::copy(str.begin(), str.end(), this->StrPubKey58);
		this->StrPubKey58[str.size()] = '\0';
	}
}

std::string EdKeysInterfase::MessageSigning(const char* messageInp)
{
	std::string message(messageInp);
	std::string signature;


	size_t siglen = edKeys.signer.MaxSignatureLength();
	signature.resize(siglen);
	siglen = edKeys.signer.SignMessage(edKeys.prng, (const CryptoPP::byte*)&message[0], message.size(), (CryptoPP::byte*)&signature[0]);
	signature.resize(siglen);


	CryptoPP::ed25519::Verifier verifier = CryptoPP::ed25519::Verifier(edKeys.signer);
	const CryptoPP::ed25519PublicKey& pubKey = dynamic_cast<const CryptoPP::ed25519PublicKey&>(verifier.GetPublicKey());
	bool valid = verifier.VerifyMessage((const CryptoPP::byte*)&message[0], message.size(), (const CryptoPP::byte*)&signature[0], signature.size());
	
	if (valid == false)
		throw std::runtime_error("Invalid signature over message");

	std::cout << "Verified signature over message\n" << std::endl;


	message.clear();
	CryptoPP::Base64Encoder encoder;
	//const CryptoPP::byte ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
	//CryptoPP::AlgorithmParameters params = CryptoPP::MakeParameters(CryptoPP::Name::EncodingLookupArray(), (const CryptoPP::byte*)ALPHABET);
	//encoder.IsolatedInitialize(params);


	encoder.Put((const CryptoPP::byte*)&signature[0], signature.size());
	encoder.MessageEnd();

	CryptoPP::word64 size = encoder.MaxRetrievable();
	if (size)
	{
		message.resize(size);
		encoder.Get((CryptoPP::byte*)&message[0], message.size());
	}

	std::cout << "signature: " << signature << std::endl;
	std::cout << "singBase64: " << message << std::endl;
	return message;
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
	edKeys.signer.GetPrivateKey().Save(*encoder);
	encoder->MessageEnd();
	delete encoder;
}

void EdKeysInterfase::LoadKeys(const char* accountID, const char* network)
{
	std::string filename = std::string(std::string(accountID) + std::string(network) + std::string(".bin"));
	{
		std::ifstream iff(filename.c_str());
		if (iff.bad())
		{
			filename = std::string("empty") + std::string(network) + std::string(".bin");
			iff.open(filename.c_str());
			if (iff.bad())
				throw std::runtime_error("Invalid file");
		}
	}

	CryptoPP::FileSource fs1(filename.c_str(), true, new CryptoPP::HexDecoder);
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
	const CryptoPP::ed25519PrivateKey& privKey = dynamic_cast<const CryptoPP::ed25519PrivateKey&>(edKeys.signer.GetPrivateKey());

	std::cout << "Load prvKey: " << privKey.GetPrivateExponent() << std::endl;
	std::cout << "Load pubKey: " << GetStrPubKey() << std::endl;
}

char* EdKeysInterfase::GetStrPubKey() const
{
	return StrPubKey58;
}
