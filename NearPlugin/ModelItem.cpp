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
	TYPE_Copy(OutfitModelResponse.flavour().c_str(), OutfitModellUE.flavour);
	TYPE_Copy(OutfitModelResponse.token_id().c_str(), OutfitModellUE.token_id);
	OutfitModellUE.kind << OutfitModelResponse.kind();
	return OutfitModellUE;
}



ModelItems::LemonModel& operator<<(ModelItems::LemonModel& LemonModelUE, const game::battlemon::items::LemonModel& LemonModelResponse)
{

	TYPE_Copy(LemonModelResponse.exo().c_str(), LemonModelUE.exo);
	TYPE_Copy(LemonModelResponse.eyes().c_str(), LemonModelUE.eyes);
	TYPE_Copy(LemonModelResponse.head().c_str(), LemonModelUE.head);
	TYPE_Copy(LemonModelResponse.teeth().c_str(), LemonModelUE.teeth);
	TYPE_Copy(LemonModelResponse.face().c_str(), LemonModelUE.face);

	LemonModelUE.cap << LemonModelResponse.cap();
	LemonModelUE.cloth << LemonModelResponse.cloth();
	LemonModelUE.fire_arm << LemonModelResponse.fire_arm();
	LemonModelUE.cold_arm << LemonModelResponse.cold_arm();
	LemonModelUE.back << LemonModelResponse.back();


	ObjectList<ModelItems::WeaponBundle> attached_bundles(LemonModelResponse.attached_bundles().size());
	ModelItems::WeaponBundle* wp = attached_bundles.getObjectPtr();

	for (int i = 0; i < LemonModelResponse.attached_bundles().size(); i++)
	{
		wp[i].bundle_num = LemonModelResponse.attached_bundles().Get(i).bundle_num();
		TYPE_Copy(LemonModelResponse.attached_bundles().Get(i).title().c_str(), wp[i].title);

		wp[i].level = LemonModelResponse.attached_bundles().Get(i).level();

		for (int itms_index = 0; itms_index < LemonModelResponse.attached_bundles().Get(i).items().size(); itms_index++)
		{
			ModelItems::WeaponBundleItem* WeaponBundleItemPtr = wp[i].WeaponList.getObjectPtr();

			WeaponBundleItemPtr[itms_index].item_type =	Helper::ConvWeaponBundleItemTypeToCPP(LemonModelResponse.attached_bundles().Get(i).items().Get(itms_index).item_type());
			TYPE_Copy(LemonModelResponse.attached_bundles().Get(i).items().Get(itms_index).skin().c_str(), WeaponBundleItemPtr[itms_index].skin);
			WeaponBundleItemPtr[itms_index].slot_type = Helper::WeaponBundleSlotTypeToCPP(LemonModelResponse.attached_bundles().Get(i).items().Get(itms_index).slot_type());
		}
	}

	LemonModelUE.attached_bundles = attached_bundles;
	return LemonModelUE;
}

ModelItems::Item& operator<<(ModelItems::Item& itemsUE, const game::battlemon::items::Item& itemResponse)
{
	TYPE_Copy(itemResponse.token_id().c_str(), itemsUE.token_id);
	TYPE_Copy(itemResponse.media().c_str(), itemsUE.media);
	TYPE_Copy(itemResponse.owner_id().c_str(), itemsUE.owner_id);
	itemsUE.lemon << itemResponse.lemon();
	itemsUE.outfit << itemResponse.outfit();
	return itemsUE;
}


ModelItems::Item::Item(void* item, int index, bool copy):copy(copy),lemon(copy), outfit(copy)
{
	if (item != nullptr)
	{
		if (copy)
		{
			*this << ((game::battlemon::items::ItemsResponse*)item)->items().Get(index);
		}
		else
		{
			this->in_fight = ((game::battlemon::items::ItemsResponse*)item)->items(index).in_fight();
			this->token_id = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).token_id()).c_str();
			this->media = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).media()).c_str();
			this->owner_id = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).owner_id()).c_str();
			this->lemon.exo = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().exo()).c_str();
			this->lemon.eyes = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().eyes()).c_str();
			this->lemon.head = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().head()).c_str();
			this->lemon.teeth = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().teeth()).c_str();
			this->lemon.face = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().face()).c_str();

			this->lemon.cap.flavour = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cap().flavour()).c_str();
			this->lemon.cap.token_id = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cap().token_id()).c_str();
			this->lemon.cap.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cap().kind();

			this->lemon.cloth.flavour = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cloth().flavour()).c_str();
			this->lemon.cloth.token_id = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cloth().token_id()).c_str();
			this->lemon.cloth.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cloth().kind();


			this->lemon.fire_arm.flavour = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().fire_arm().flavour()).c_str();
			this->lemon.fire_arm.token_id = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().fire_arm().token_id()).c_str();
			this->lemon.fire_arm.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().fire_arm().kind();

			this->lemon.cold_arm.flavour = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cold_arm().flavour()).c_str();
			this->lemon.cold_arm.token_id = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cold_arm().token_id()).c_str();
			this->lemon.cold_arm.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().cold_arm().kind();

			this->lemon.back.flavour = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().back().flavour()).c_str();
			this->lemon.back.token_id = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().back().token_id()).c_str();
			this->lemon.back.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().back().kind();

			ObjectList<ModelItems::WeaponBundle> attached_bundles(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().size());
			ModelItems::WeaponBundle* wp = attached_bundles.getObjectPtr();

			for (int i = 0; i < ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().size(); i++)
			{
				wp[i].bundle_num = ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).bundle_num();


				wp[i].title = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).title()).c_str();
				wp[i].level = ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).level();

				for (int itms_index = 0; itms_index < ((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).items().size(); itms_index++)
				{
					ModelItems::WeaponBundleItem* WeaponBundleItemPtr = wp[i].WeaponList.getObjectPtr();

					WeaponBundleItemPtr[itms_index].item_type = Helper::ConvWeaponBundleItemTypeToCPP(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).items().Get(itms_index).item_type());
					WeaponBundleItemPtr[itms_index].skin = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).items().Get(itms_index).skin()).c_str();
					WeaponBundleItemPtr[itms_index].slot_type = Helper::WeaponBundleSlotTypeToCPP(((game::battlemon::items::ItemsResponse*)item)->items(index).lemon().attached_bundles().Get(i).items().Get(itms_index).slot_type());
				}
			}
			this->lemon.attached_bundles = attached_bundles;
			this->outfit.flavour = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).outfit().flavour()).c_str();
			this->outfit.token_id = (TYPE_CHAR*)TYPE_ReConv(((game::battlemon::items::ItemsResponse*)item)->items(index).outfit().token_id()).c_str();
			this->outfit.kind << ((game::battlemon::items::ItemsResponse*)item)->items(index).outfit().kind();


		}
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

ModelItems::LemonModel::LemonModel(const LemonModel& copy):cap(copy.cap), cloth(copy.cloth), exo(copy.exo), eyes(copy.eyes), head(copy.head), teeth(copy.teeth)
, face(copy.face), fire_arm(copy.fire_arm), cold_arm(copy.cold_arm), back(copy.back)
{
};

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

ModelItems::WeaponBundleItem& ModelItems::WeaponBundleItem::operator=(const ModelItems::WeaponBundleItem& from)
{
	item_type = from.item_type;
	skin = from.skin;
	slot_type = from.slot_type;
	return *this;
}

ModelItems::WeaponBundle& ModelItems::WeaponBundle::operator=(const ModelItems::WeaponBundle& from)
{
	bundle_num = from.bundle_num;
	WeaponList = from.WeaponList;
	title = from.title;
	level = from.level;

	return *this;
}

ModelItems::Item& ModelItems::Item::operator=(const ModelItems::Item& from)
{
	token_id = from.token_id;
	media = from.media;
	owner_id = from.owner_id;
	in_fight = from.in_fight;
	lemon = from.lemon;
	outfit = from.outfit;

	return *this;
}