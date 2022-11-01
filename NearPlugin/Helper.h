#pragma once

#include <string>
#include "include/ModelItem.h"
#include "protocol/items.pb.h"

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
	void allocateMemory(const std::string& copy, char*& target)
	{
		if (target == nullptr && copy != "")
		{
			target = new char[copy.size() + 1];
			std::copy(copy.begin(), copy.end(), target);
			target[copy.size()] = '\0';
		}
	}

	void free(char* data)
	{
		if (data != nullptr)
		{
			delete[]data;
			data = nullptr;
		}
	}


	game::battlemon::items::WeaponBundleItemType ConvWeaponBundleItemTypeToProto(const ModelItems::WeaponBundleItemType& item_type)
	{
		switch (item_type)
		{
		case ModelItems::WeaponBundleItemType::BUNDLE_ITEM_PRIMARY:
			return game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_PRIMARY;
			break;
		case ModelItems::WeaponBundleItemType::BUNDLE_ITEM_SECONDARY:
			return game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_SECONDARY;
			break;
		case ModelItems::WeaponBundleItemType::BUNDLE_ITEM_MELEE:
			return game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_MELEE;
			break;
		case ModelItems::WeaponBundleItemType::BUNDLE_ITEM_TACTICAL:
			return game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_TACTICAL;
			break;
		case ModelItems::WeaponBundleItemType::BUNDLE_ITEM_MILITARY:
			return game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_MILITARY;
			break;
		case ModelItems::WeaponBundleItemType::BUNDLE_ITEM_PERK:
			return game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_PERK;
			break;
		default:
			break;
		}
	}

	ModelItems::WeaponBundleItemType ConvWeaponBundleItemTypeToCPP(const game::battlemon::items::WeaponBundleItemType& item_type)
	{
		switch (item_type)
		{
		case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_PRIMARY:
			return ModelItems::WeaponBundleItemType::BUNDLE_ITEM_PRIMARY;
			break;
		case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_SECONDARY:
			return ModelItems::WeaponBundleItemType::BUNDLE_ITEM_SECONDARY;
			break;
		case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_MELEE:
			return ModelItems::WeaponBundleItemType::BUNDLE_ITEM_MELEE;
			break;
		case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_TACTICAL:
			return ModelItems::WeaponBundleItemType::BUNDLE_ITEM_TACTICAL;
			break;
		case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_MILITARY:
			return ModelItems::WeaponBundleItemType::BUNDLE_ITEM_MILITARY;
			break;
		case game::battlemon::items::WeaponBundleItemType::BUNDLE_ITEM_PERK:
			return ModelItems::WeaponBundleItemType::BUNDLE_ITEM_PERK;
			break;
		default:
			return ModelItems::WeaponBundleItemType::NONE;
			break;
		}
	}

	game::battlemon::items::WeaponBundleSlotType WeaponBundleSlotTypeToProto(const ModelItems::WeaponBundleSlotType& slot_type)
	{
		switch (slot_type)
		{
		case ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_PRIMARY:
			return game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_PRIMARY;
			break;
		case ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_SECONDARY:
			return game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_SECONDARY;
			break;
		case ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_MELEE:
			return game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_MELEE;
			break;
		case ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_ONE:
			return game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_ONE;
			break;
		case ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_TWO:
			return game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_TWO;
			break;
		case ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_ONE:
			return game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_ONE;
			break;
		case ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_TWO:
			return game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_TWO;
			break;
		case ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_PERK_ONE:
			return game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_PERK_ONE;
			break;
		case ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_PERK_TWO:
			return game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_PERK_TWO;
			break;
		default:
			break;
		}
	}

	ModelItems::WeaponBundleSlotType WeaponBundleSlotTypeToCPP(const game::battlemon::items::WeaponBundleSlotType& slot_type)
	{
		switch (slot_type)
		{
		case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_PRIMARY:
			return ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_PRIMARY;
			break;
		case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_SECONDARY:
			return ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_SECONDARY;
			break;
		case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_MELEE:
			return ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_MELEE;
			break;
		case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_ONE:
			return ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_ONE;
			break;
		case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_TWO:
			return ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_TACTICAL_TWO;
			break;
		case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_ONE:
			return ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_ONE;
			break;
		case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_TWO:
			return ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_MILITARY_TWO;
			break;
		case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_PERK_ONE:
			return ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_PERK_ONE;
			break;
		case game::battlemon::items::WeaponBundleSlotType::BUNDLE_SLOT_PERK_TWO:
			return ModelItems::WeaponBundleSlotType::BUNDLE_SLOT_PERK_TWO;
			break;
		default:
			return ModelItems::WeaponBundleSlotType::NONE;
			break;
		}
	}

#if defined(__unix__) || defined(__APPLE__)
	std::string convUTF(const char16_t* utp16)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
		std::u16string utf16_string(utp16);
		std::string UTF8 = convert.to_bytes(utf16_string);
		return UTF8;
	}
}
#define TYPE_Conv(str) Helper::convUTF(str)
#else
#define TYPE_Conv(str) str
}
#endif
