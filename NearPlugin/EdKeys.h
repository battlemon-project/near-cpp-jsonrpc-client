#pragma once
#include <string>


#if _WIN32
#define _T(c) L##c
#define TEXT(c) L##c
#define TYPE_STRING std::wstring
#define TYPE_CHAR wchar_t
#else
#define _T(c) u##c
#define TEXT(c) u##c
#define TYPE_STRING std::u16string
#define TYPE_CHAR char16_t
#endif

class EdKeys
{
	uint8_t public_key[32];
	uint8_t private_key[64];
	TYPE_STRING sign;

	bool state;
	void SaveK(const TYPE_STRING& filename, void* key, size_t size);
	bool LoadK(const TYPE_STRING& path, const TYPE_STRING& accountID, const TYPE_STRING& filetype, void* key, size_t size);

public:
	EdKeys();
	~EdKeys();
	void GeneratingKeys(TYPE_CHAR*& error, void(*errorH)(const TYPE_STRING& copy, TYPE_CHAR*& error));
	const TYPE_STRING& MessageSigning(const char* messageChr);
	void SaveKeys(const TYPE_STRING &accountID, TYPE_STRING dir);
	bool LoadKeys(const TYPE_STRING &accountID, TYPE_STRING dir);
	void SaveSign(const TYPE_STRING& accountID, TYPE_STRING dir, const TYPE_CHAR* sign);
	bool LoadSign(const TYPE_STRING& accountID, TYPE_STRING dir);
	TYPE_STRING GetPubKey58()const;
	TYPE_STRING GetPrKey58()const;
	const TYPE_STRING& GetSign()const;
	void SetSign(const TYPE_STRING& sign);
	bool IsValid() const;
};

