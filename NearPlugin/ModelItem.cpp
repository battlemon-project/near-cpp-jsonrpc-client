#include "include/ModelItem.h"
#include "Helper.h"
#include "protocol/items.grpc.pb.h"



ModelItems::OutfitKind& operator<<(ModelItems::OutfitKind& OutfitKindUE, const game::battlemon::items::OutfitKind& OutfitKindResponse)
{
	switch (OutfitKindResponse)
	{
	case 0:
		OutfitKindUE = ModelItems::OutfitKind::CAP;
		break;
	case 1:
		OutfitKindUE = ModelItems::OutfitKind::CLOTH;
		break;
	case 2:
		OutfitKindUE = ModelItems::OutfitKind::FIRE_ARM;
		break;
	case 3:
		OutfitKindUE = ModelItems::OutfitKind::COLD_ARM;
		break;
	case 4:
		OutfitKindUE = ModelItems::OutfitKind::BACK;
		break;
	default:
		OutfitKindUE = ModelItems::OutfitKind::DEFAULT;
		break;
	}
	return OutfitKindUE;
}

ModelItems::OutfitModel& operator<<(ModelItems::OutfitModel& OutfitModellUE, const game::battlemon::items::OutfitModel& OutfitModelResponse)
{
	Helper::allocateMemory(OutfitModelResponse.flavour(), OutfitModellUE.flavour);
	Helper::allocateMemory(OutfitModelResponse.token_id(), OutfitModellUE.token_id);
	OutfitModellUE.kind << OutfitModelResponse.kind();
	return OutfitModellUE;
}

ModelItems::LemonModel& operator<<(ModelItems::LemonModel& LemonModelUE, const game::battlemon::items::LemonModel& LemonModelResponse)
{
	Helper::allocateMemory(LemonModelResponse.exo(), LemonModelUE.exo);
	Helper::allocateMemory(LemonModelResponse.eyes(), LemonModelUE.eyes);
	Helper::allocateMemory(LemonModelResponse.head(), LemonModelUE.head);
	Helper::allocateMemory(LemonModelResponse.teeth(), LemonModelUE.teeth);
	Helper::allocateMemory(LemonModelResponse.face(), LemonModelUE.face);
	LemonModelUE.cap << LemonModelResponse.cap();
	LemonModelUE.cloth << LemonModelResponse.cloth();
	LemonModelUE.fire_arm << LemonModelResponse.fire_arm();
	LemonModelUE.cold_arm << LemonModelResponse.cold_arm();
	LemonModelUE.back << LemonModelResponse.back();


	LemonModelUE.attached_bundles;
	ObjectList<ModelItems::WeaponBundle> attached_bundles(LemonModelResponse.attached_bundles().size());
	ModelItems::WeaponBundle* wp = attached_bundles.getObjectPtr();

	for (int i = 0; i < LemonModelResponse.attached_bundles().size(); i++)
	{
		wp[i].bundle_num = LemonModelResponse.attached_bundles().Get(i).bundle_num();
		Helper::allocateMemory(LemonModelResponse.attached_bundles().Get(i).title(), wp[i].title);
		wp[i].level = LemonModelResponse.attached_bundles().Get(i).level();

		for (int itms_index = 0; itms_index < LemonModelResponse.attached_bundles().Get(i).items().size(); itms_index++)
		{
			ModelItems::WeaponBundleItem* WeaponBundleItemPtr = wp[i].WeaponList.getObjectPtr();

			WeaponBundleItemPtr[itms_index].item_type =	Helper::ConvWeaponBundleItemTypeToCPP(LemonModelResponse.attached_bundles().Get(i).items().Get(itms_index).item_type());
			Helper::allocateMemory(LemonModelResponse.attached_bundles().Get(i).items().Get(itms_index).skin(), WeaponBundleItemPtr[itms_index].skin);
			WeaponBundleItemPtr[itms_index].slot_type = Helper::WeaponBundleSlotTypeToCPP(LemonModelResponse.attached_bundles().Get(i).items().Get(itms_index).slot_type());
		}
	}

	LemonModelResponse.attached_bundles();
	return LemonModelUE;
}

ModelItems::Item& operator<<(ModelItems::Item& itemsUE, const game::battlemon::items::Item& itemResponse)
{
	Helper::allocateMemory(itemResponse.token_id(), itemsUE.token_id);
	Helper::allocateMemory(itemResponse.media(), itemsUE.media);
	Helper::allocateMemory(itemResponse.owner_id(), itemsUE.owner_id);
	itemsUE.lemon << itemResponse.lemon();
	itemsUE.outfit << itemResponse.outfit();
	return itemsUE;
}


ModelItems::Item::Item(void* item, int index, bool copy):copy(copy),lemon(copy), outfit(copy)
{
	if(item != nullptr)
		if (copy)
		{
			*this << ((game::battlemon::items::ItemsResponse*)item)->items().Get(index);
		}
		else
		{
			this->in_fight = ((game::battlemon::items::ItemsResponse*)item)->items(index).in_fight();
			this->token_id = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).token_id().c_str();
			this->media = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).media().c_str();
			this->owner_id = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).owner_id().c_str();
			this->lemon.exo = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().exo().c_str();
			this->lemon.eyes = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().eyes().c_str();
			this->lemon.head = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().head().c_str();
			this->lemon.teeth = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().teeth().c_str();
			this->lemon.face = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().face().c_str();

			this->lemon.cap.flavour = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cap().flavour().c_str();
			this->lemon.cap.token_id = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cap().token_id().c_str();
			this->lemon.cap.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cap().kind();

			this->lemon.cloth.flavour = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cloth().flavour().c_str();
			this->lemon.cloth.token_id = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cloth().token_id().c_str();
			this->lemon.cloth.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cloth().kind();


			this->lemon.fire_arm.flavour = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().fire_arm().flavour().c_str();
			this->lemon.fire_arm.token_id = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().fire_arm().token_id().c_str();
			this->lemon.fire_arm.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().fire_arm().kind();

			this->lemon.cold_arm.flavour = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cold_arm().flavour().c_str();
			this->lemon.cold_arm.token_id = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cold_arm().token_id().c_str();
			this->lemon.cold_arm.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cold_arm().kind();

			this->lemon.back.flavour = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().back().flavour().c_str();
			this->lemon.back.token_id = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().back().token_id().c_str();
			this->lemon.back.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().back().kind();

			this->lemon.attached_bundles;
			ObjectList<ModelItems::WeaponBundle> attached_bundles(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().size());
			ModelItems::WeaponBundle* wp = attached_bundles.getObjectPtr();

			for (int i = 0; i < ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().size(); i++)
			{
				wp[i].bundle_num = ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).bundle_num();


				wp[i].title = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).title().c_str();
				wp[i].level = ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).level();

				for (int itms_index = 0; itms_index < ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).items().size(); itms_index++)
				{
					ModelItems::WeaponBundleItem* WeaponBundleItemPtr = wp[i].WeaponList.getObjectPtr();

					WeaponBundleItemPtr[itms_index].item_type = Helper::ConvWeaponBundleItemTypeToCPP(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).items().Get(itms_index).item_type());
					WeaponBundleItemPtr[itms_index].skin = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).items().Get(itms_index).skin().c_str();
					WeaponBundleItemPtr[itms_index].slot_type = Helper::WeaponBundleSlotTypeToCPP(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).items().Get(itms_index).slot_type());
				}
			}

			this->outfit.flavour = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).outfit().flavour().c_str();
			this->outfit.token_id = (char*)((game::battlemon::items::ItemsResponse*)item)->items(index).outfit().token_id().c_str();
			this->outfit.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).outfit().kind();


		}
}

ModelItems::Item::Item(const Item& copyItem):lemon(copyItem.lemon), outfit(copyItem.outfit), copy(copyItem.copy), token_id(copyItem.token_id),
											media(copyItem.media), owner_id(copyItem.owner_id), in_fight(copyItem.in_fight)
{
}


ModelItems::Item::~Item()
{
	if (copy)
	{
		Helper::free(token_id);
		Helper::free(media);
		Helper::free(owner_id);
		lemon.~LemonModel();
		outfit.~OutfitModel();
	}
}

ModelItems::OutfitModel::~OutfitModel()
{
	if (copy)
	{
		Helper::free(flavour);
		Helper::free(token_id);
	}
}

ModelItems::LemonModel::~LemonModel()
{
	if (copy)
	{
		Helper::free(exo);
		Helper::free(eyes);
		Helper::free(head);
		Helper::free(teeth);
		Helper::free(face);
		cap.~OutfitModel();
		cloth.~OutfitModel();
		fire_arm.~OutfitModel();
		cold_arm.~OutfitModel();
		back.~OutfitModel();
	}
	attached_bundles.~ObjectList();
}

ModelItems::WeaponBundleItem::WeaponBundleItem(const WeaponBundleItem& copy)
{
	item_type = copy.item_type;
	skin = copy.skin;
	slot_type = copy.slot_type;
};

ModelItems::WeaponBundle::WeaponBundle(const WeaponBundle& copy)
{
	bundle_num = copy.bundle_num;
	WeaponList = copy.WeaponList;

	title = copy.title;
	level = copy.level;
}

ModelItems::OutfitModel& ModelItems::OutfitModel::operator=(const ModelItems::OutfitModel& from)
{
	flavour = from.flavour;
	token_id = from.token_id;
	kind = from.kind;
	copy = from.copy;
	return *this;
}

ModelItems::LemonModel& ModelItems::LemonModel::operator=(const ModelItems::LemonModel& from)
{
	copy = from.copy;
	cap = from.cap;
	cloth = from.cloth;
	exo = from.exo;
	eyes = from.eyes;
	head = from.head;
	teeth = from.teeth;
	face = from.face;
	fire_arm = from.fire_arm;
	cold_arm = from.cloth;
	back = from.back;
	attached_bundles = from.attached_bundles;
	return *this;
}