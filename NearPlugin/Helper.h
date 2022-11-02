#pragma once

#include <string>
#include "include/ModelItem.h"
#include "protocol/items.pb.h"

#if defined(__unix__) || defined(__APPLE__)
//#include <threads.h>
#include <thread>
#include <chrono>
#include <locale>
#include <codecvt>
#endif

#define ED25519 ((EdKeys*)keyPair)

#define ITEMS_RESPONSE ((game::battlemon::items::ItemsResponse*)gRPC_read)
#define GET_BUNDLES_RESPONSE ((game::battlemon::items::GetBundlesResponse*)gRPC_read)
#define WEAPON_BUNDLE ((game::battlemon::items::WeaponBundle*)gRPC_read)
#define COMMON_EMPTY ((game::battlemon::common::Empty*)gRPC_read)
#define SEARCH_GAME_RESPONSE ((game::battlemon::mm::SearchGameResponse*)gRPC_read)


#define HOOK_ERROR char*& error, void(*errorH)(const std::string& copy, char*& error)
#define THROW_HOOK error, Helper::allocateMemory
#define SET_ERROR(str) Helper::allocateMemory(str, error)



namespace Helper
{
	void allocateMemory(const std::string& copy, char*& target);
	void free(char* data);
	void free(char16_t* data);

	game::battlemon::items::WeaponBundleItemType ConvWeaponBundleItemTypeToProto(const ModelItems::WeaponBundleItemType& item_type);

	ModelItems::WeaponBundleItemType ConvWeaponBundleItemTypeToCPP(const game::battlemon::items::WeaponBundleItemType& item_type);

	game::battlemon::items::WeaponBundleSlotType WeaponBundleSlotTypeToProto(const ModelItems::WeaponBundleSlotType& slot_type);

	ModelItems::WeaponBundleSlotType WeaponBundleSlotTypeToCPP(const game::battlemon::items::WeaponBundleSlotType& slot_type);

#if defined(__unix__) || defined(__APPLE__)
	std::string convUTF8(const char16_t* utp16);
	std::u16string convUTF16(std::string utp8);
	void allocateMemoryTo16_t(const std::u16string& copy, char16_t*& target);
}
#define TYPE_Conv(str) Helper::convUTF8(str)
#define TYPE_ReConv(str) Helper::convUTF16(str)
#define TYPE_Copy(str,chr) Helper::allocateMemoryTo16_t(Helper::convUTF16(str), chr)
#else
#define TYPE_Conv(str) str
#define TYPE_ReConv(str) str
#define TYPE_Copy(str,chr) Helper::allocateMemory(str, chr)
}
#endif
