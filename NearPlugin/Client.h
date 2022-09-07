#pragma once
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
#define __linux__ 1=
#endif

#if __linux__
#define _GLIBCXX_USE_CXX11_ABI 0
#endif

#if __APPLE__
#define TYPE_CHAR char16_t*
#else
#define TYPE_CHAR char*
#endif


namespace ModelItems
{
	enum OutfitKind
	{
		CAP = 0,
		CLOTH = 1,
		FIRE_ARM = 2,
		COLD_ARM = 3,
		BACK = 4,
		DEFAULT = 5
	};

	struct OutfitModel
	{
		char* flavour = nullptr;
		char* token_id = nullptr;
		OutfitKind kind;
		~OutfitModel();
	};

	struct LemonModel
	{
		OutfitModel cap;
		OutfitModel cloth;
		char* exo = nullptr;
		char* eyes = nullptr;
		char* head = nullptr;
		char* teeth = nullptr;
		char* face = nullptr;
		OutfitModel fire_arm;
		OutfitModel cold_arm;
		OutfitModel back;
		~LemonModel();
	};

	struct Item
	{
		char* token_id = nullptr;
		char* media = nullptr;
		char* owner_id = nullptr;
		LemonModel lemon;
		OutfitModel outfit;
		~Item();
	};

}

// повторяет структуру message PlayerItems
struct PlayerItemsClient
{
	char* near_id;
	char*** items = nullptr;
	int sizeItems;
	int sizenft_ids;
	~PlayerItemsClient();
};

enum class TypeInp
{
	AUTHORIZATION,
	REGISTRATION,
};


class Client
{
	char* accountID;
	char* network;
	char* sign;
	void* keyPair;
	char* keyPub58;
	char* error;

	void RegistrKey();
	bool AuthServiceClient();
public:

	Client(const TYPE_CHAR dir, const TYPE_CHAR inpText, TypeInp type);

	~Client();
	Client() = delete;

	char* GetPublicKey() const { return keyPub58; };
	bool IsValidAccount() { return accountID != nullptr; };
	bool IsValidKeys();
	char* GetAccount() { return accountID; };
	char* GetError() { return error; };
	const char* GetSing(){ return sign; };

	PlayerItemsClient gRPC_getPlayerItems(const TYPE_CHAR room_id, int index, const TYPE_CHAR* near_ids);
	void gRPC_SetMyItems(const TYPE_CHAR room_id, int index, const TYPE_CHAR* nft_ids);
	ModelItems::Item gRPC_GetItems();
};

