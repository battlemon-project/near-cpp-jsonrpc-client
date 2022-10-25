#pragma once
#include "include/ModelItem.h"
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
#define __linux__ 1
#endif

#if __linux__
#define _GLIBCXX_USE_CXX11_ABI 0
#endif

#if _WIN32
#define TYPE_CHAR char
#else
#define TYPE_CHAR char16_t
#endif



class ItemsList
{
	ModelItems::Item* items;
public:

	const int size;
	ItemsList(ModelItems::Item*& items, int size);
	ItemsList(const ItemsList& itemsList);
	ItemsList() = delete;
	~ItemsList();

	ModelItems::Item& getItem(int id);
};

// повторяет структуру message PlayerItems
struct PlayerItemsClient
{
	char** near_id = nullptr;
	char*** items = nullptr;
	int players_items_size;
	int nft_ids_size;
	~PlayerItemsClient();
};

enum class TypeInp
{
	AUTHORIZATION,
	REGISTRATION,
};

enum class Type_gRPC
{
	NONE,
	//items.proto
	GET_ITEMS,
	GET_BUNDLES,
	CREATE_BUNDLE,
	EDIT_BUNDLE,
	REMOVE_BUNDLE,
	ATTACH_BUNDLE,
	DETACH_BUNDLE,
	SEARCH_GAME,

	//mm.proto
	ACCEPT_GAME,
	CANCEL_SEARCH,

	//auth.proto
	AUTHORIZATION,
	REGISTRATION
};

class Client
{
	char* accountID;
	char* network;
	char* sign;
	void* keyPair;
	char* keyPub58;
	char* error;
	void* gRPC_read;
	Type_gRPC type_gRPC;

public:

	Client(const TYPE_CHAR* dir, const TYPE_CHAR* inpText, TypeInp type);

	~Client();
	Client() = delete;

	Type_gRPC getType_gRPC() const { return type_gRPC; };
	char* GetPublicKey() const { return keyPub58; };
	bool IsValidAccount() { return accountID != nullptr; };
	bool IsValidKeys();
	char* GetAccount() { return accountID; };
	char* GetError() { return error; };
	const char* GetSing() { return sign; };

	void Free_gRPC_read();

	//items.proto
	ModelItems::Item gRPC_GetItem(int index);
	ItemsList gRPC_GetItems();
	ItemsList gRPC_CopyItems();


	//call server
	void CallRPC_GetItems();
	void CallRPC_GetBundles(const TYPE_CHAR* outBundle_id, const TYPE_CHAR* outAttached_lemons);
	void CallRPC_CreateBundle(const TYPE_CHAR* outBundle_id, const TYPE_CHAR* outAttached_lemons);
	void CallRPC_EditBundle(const TYPE_CHAR* inpBundle_id, const TYPE_CHAR* outBundle_id, const TYPE_CHAR* outAttached_lemons);
	bool CallRPC_RemoveBundle(const TYPE_CHAR* bundle_id);
	bool CallRPC_AttachBundle(const TYPE_CHAR* bundle_id, const TYPE_CHAR* lemon_id);
	bool CallRPC_DetachBundle(const TYPE_CHAR* bundle_id, const TYPE_CHAR* lemon_id);


	//mm.proto
	ModelMM::SearchGameResponseStatus CallRPC_SearchGame(const int& MatchType, const int& MatchMode);
	bool CallRPC_AcceptGame(const TYPE_CHAR* lemon_id);
	bool CallRPC_CancelSearch();


private:
	bool AuthServiceClient(TypeInp type);
};

