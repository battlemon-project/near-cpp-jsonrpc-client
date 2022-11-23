#include "include/ModelItem.h"
#include "Helper.h"

#include "protocol/updates.grpc.pb.h"


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
	if (OutfitModellUE.copy)
	{
		TYPE_Copy(TYPE_ReConv(OutfitModelResponse.flavour().c_str()), OutfitModellUE.flavour);
		TYPE_Copy(TYPE_ReConv(OutfitModelResponse.token_id().c_str()), OutfitModellUE.token_id);
	}
	else
	{
		OutfitModellUE.flavour = (TYPE_CHAR*)OutfitModelResponse.flavour().c_str();
		OutfitModellUE.token_id = (TYPE_CHAR*)OutfitModelResponse.token_id().c_str();
	}
	OutfitModellUE.kind << OutfitModelResponse.kind();
	return OutfitModellUE;
}



ModelItems::LemonModel& operator<<(ModelItems::LemonModel& LemonModelUE, const game::battlemon::items::LemonModel& LemonModelResponse)
{
	if (LemonModelUE.copy)
	{
		TYPE_Copy(TYPE_ReConv(LemonModelResponse.exo().c_str()), LemonModelUE.exo);
		TYPE_Copy(TYPE_ReConv(LemonModelResponse.eyes().c_str()), LemonModelUE.eyes);
		TYPE_Copy(TYPE_ReConv(LemonModelResponse.head().c_str()), LemonModelUE.head);
		TYPE_Copy(TYPE_ReConv(LemonModelResponse.teeth().c_str()), LemonModelUE.teeth);
		TYPE_Copy(TYPE_ReConv(LemonModelResponse.face().c_str()), LemonModelUE.face);
	}
	else
	{
		LemonModelUE.exo = (TYPE_CHAR*)LemonModelResponse.exo().c_str();
		LemonModelUE.eyes = (TYPE_CHAR*)LemonModelResponse.eyes().c_str();
		LemonModelUE.head = (TYPE_CHAR*)LemonModelResponse.head().c_str();
		LemonModelUE.teeth = (TYPE_CHAR*)LemonModelResponse.teeth().c_str();
		LemonModelUE.face = (TYPE_CHAR*)LemonModelResponse.face().c_str();
	}

	LemonModelUE.cap << LemonModelResponse.cap();
	LemonModelUE.cloth << LemonModelResponse.cloth();
	LemonModelUE.fire_arm << LemonModelResponse.fire_arm();
	LemonModelUE.cold_arm << LemonModelResponse.cold_arm();
	LemonModelUE.back << LemonModelResponse.back();

	int size = LemonModelResponse.attached_bundles().size();

	ObjectList<ModelItems::WeaponBundle> attached_bundles(size);
	ModelItems::WeaponBundle* wp = attached_bundles.getObjectPtr();

	for (int i = 0; i < size; i++)
	{
		wp[i].bundle_num = LemonModelResponse.attached_bundles().Get(i).bundle_num();
		wp[i].copy = LemonModelUE.copy;
		if (wp[i].copy)
		{
			TYPE_Copy(TYPE_ReConv(LemonModelResponse.attached_bundles().Get(i).title().c_str()), wp[i].title);
		}
		else
		{
			wp[i].title = (TYPE_CHAR*)LemonModelResponse.attached_bundles().Get(i).title().c_str();
		}
		wp[i].level = LemonModelResponse.attached_bundles().Get(i).level();

		int sizeItems = LemonModelResponse.attached_bundles().Get(i).items().size();
		ModelItems::WeaponBundleItem* WeaponBundleItemPtr = new ModelItems::WeaponBundleItem[sizeItems];
		for (int itms_index = 0; itms_index < sizeItems; itms_index++)
		{
			WeaponBundleItemPtr[itms_index].item_type =	Helper::ConvWeaponBundleItemTypeToCPP(LemonModelResponse.attached_bundles().Get(i).items().Get(itms_index).item_type());
			WeaponBundleItemPtr[itms_index].copy = LemonModelUE.copy;
			if (WeaponBundleItemPtr[itms_index].copy)
			{
				TYPE_Copy(TYPE_ReConv(LemonModelResponse.attached_bundles().Get(i).items().Get(itms_index).skin().c_str()), WeaponBundleItemPtr[itms_index].skin);
			}
			else
			{
				WeaponBundleItemPtr[itms_index].skin = (TYPE_CHAR*)LemonModelResponse.attached_bundles().Get(i).items().Get(itms_index).skin().c_str();
			}
			WeaponBundleItemPtr[itms_index].slot_type = Helper::WeaponBundleSlotTypeToCPP(LemonModelResponse.attached_bundles().Get(i).items().Get(itms_index).slot_type());
		}
		wp[i].WeaponList.setObjectList(WeaponBundleItemPtr, sizeItems);
	}

	LemonModelUE.attached_bundles = attached_bundles;
	return LemonModelUE;
}

ModelItems::Item& operator<<(ModelItems::Item& itemsUE, const game::battlemon::items::Item& itemResponse)
{
	itemsUE.in_fight = itemResponse.in_fight();
	TYPE_Copy(TYPE_ReConv(itemResponse.token_id().c_str()), itemsUE.token_id);
	TYPE_Copy(TYPE_ReConv(itemResponse.media().c_str()), itemsUE.media);
	TYPE_Copy(TYPE_ReConv(itemResponse.owner_id().c_str()), itemsUE.owner_id);
	switch (itemResponse.model_case())
	{
	case game::battlemon::items::Item::kLemon:
		itemsUE.model = ModelItems::Model::LEMON;
		itemsUE.lemon << itemResponse.lemon();
		break;
	case game::battlemon::items::Item::kOutfit:
		itemsUE.model = ModelItems::Model::OUTFIT_MODEL;
		itemsUE.outfit << itemResponse.outfit();
		break;
	case game::battlemon::items::Item::MODEL_NOT_SET:
	default:
		itemsUE.model = ModelItems::Model::DEFAULT;
		break;
	}
	return itemsUE;
}


ModelItems::Item::Item(void* item, bool copy):copy(copy),lemon(copy), outfit(copy), token_id(nullptr), media(nullptr), owner_id(nullptr), in_fight(false), model(ModelItems::Model::DEFAULT)
{
	game::battlemon::items::Item* itemLoc = ((game::battlemon::items::Item*)item);
	if (item != nullptr)
	{
		if (copy)
		{
			this->in_fight = itemLoc->in_fight();
			TYPE_Copy(TYPE_ReConv(itemLoc->token_id().c_str()), this->token_id);
			TYPE_Copy(TYPE_ReConv(itemLoc->media().c_str()), this->media);
			TYPE_Copy(TYPE_ReConv(itemLoc->owner_id().c_str()), this->owner_id);
			switch (itemLoc->model_case())
			{
			case game::battlemon::items::Item::kLemon:
				this->model = ModelItems::Model::LEMON;
				this->lemon << itemLoc->lemon();
				break;
			case game::battlemon::items::Item::kOutfit:
				this->model = ModelItems::Model::OUTFIT_MODEL;
				this->outfit << itemLoc->outfit();
				break;
			case game::battlemon::items::Item::MODEL_NOT_SET:
			default:
				this->model = ModelItems::Model::DEFAULT;
				break;
			}
		}
		else
		{
			this->in_fight = itemLoc->in_fight();
			this->token_id = (TYPE_CHAR*)TYPE_ReConv(itemLoc->token_id()).c_str();
			this->media = (TYPE_CHAR*)TYPE_ReConv(itemLoc->media()).c_str();
			this->owner_id = (TYPE_CHAR*)TYPE_ReConv(itemLoc->owner_id()).c_str();
			
			
			switch (itemLoc->model_case())
			{
			case game::battlemon::items::Item::kLemon:
				this->model = ModelItems::Model::LEMON;
				this->lemon << itemLoc->lemon();
				break;
			case game::battlemon::items::Item::kOutfit:
				this->model = ModelItems::Model::OUTFIT_MODEL;
				this->outfit.flavour = (TYPE_CHAR*)TYPE_ReConv(itemLoc->outfit().flavour()).c_str();
				this->outfit.token_id = (TYPE_CHAR*)TYPE_ReConv(itemLoc->outfit().token_id()).c_str();
				this->outfit.kind << itemLoc->outfit().kind();
				break;
			case game::battlemon::items::Item::MODEL_NOT_SET:
			default:
				this->model = ModelItems::Model::DEFAULT;
				break;
			}
		}
	}
}

ModelItems::Item::Item(const ModelItems::Item& copyItem):copy(copyItem.copy), lemon(copyItem.lemon), outfit(copyItem.outfit), token_id(copyItem.token_id),
											media(copyItem.media), owner_id(copyItem.owner_id), in_fight(copyItem.in_fight), model(copyItem.model)
{
	if (copy)
	{
		TYPE_Copy(copyItem.token_id, token_id);
		TYPE_Copy(copyItem.media, media);
		TYPE_Copy(copyItem.owner_id, owner_id);
	}
}

ModelItems::Item::Item():lemon(false), outfit(false), copy(false), token_id(nullptr), media(nullptr), owner_id(nullptr)
{
}

ModelItems::Item::~Item()
{
	if (copy)
	{
		Helper::free(&token_id);
		Helper::free(&media);
		Helper::free(&owner_id);
		//if(model == ModelItems::Model::LEMON)
		//	lemon.~LemonModel();
		//else
		//	outfit.~OutfitModel();
	}
}

ModelItems::OutfitModel::~OutfitModel()
{
	if (copy)
	{
		Helper::free(&flavour);
		Helper::free(&token_id);
	}
}

ModelItems::LemonModel::~LemonModel()
{
	if (copy)
	{
		Helper::free(&exo);
		Helper::free(&eyes);
		Helper::free(&head);
		Helper::free(&teeth);
		Helper::free(&face);
		cap.~OutfitModel();
		cloth.~OutfitModel();
		fire_arm.~OutfitModel();
		cold_arm.~OutfitModel();
		back.~OutfitModel();
	}
	attached_bundles.~ObjectList();
}

ModelItems::LemonModel::LemonModel(const LemonModel& copy):copy(copy.copy), cap(copy.cap), cloth(copy.cloth), exo(nullptr), eyes(nullptr), head(nullptr), teeth(nullptr)
, face(nullptr), fire_arm(copy.fire_arm), cold_arm(copy.cold_arm), back(copy.back)
{
	if (this->copy)
	{
		TYPE_Copy(copy.exo, exo);
		TYPE_Copy(copy.eyes, eyes);
		TYPE_Copy(copy.head, head);
		TYPE_Copy(copy.teeth, teeth);
		TYPE_Copy(copy.face, face);
	}
	else
	{
		exo = copy.exo;
		eyes = copy.eyes;
		head = copy.head;
		teeth = copy.teeth;
		face = copy.face;
	}
	attached_bundles = copy.attached_bundles;
};

ModelItems::WeaponBundleItem::WeaponBundleItem(const WeaponBundleItem& copy):copy(copy.copy), item_type(copy.item_type), slot_type(copy.slot_type)
{
	if (copy.copy)
	{
		TYPE_Copy(copy.skin, skin);
	}
	else
	{
		skin = copy.skin;
	}
};

ModelItems::WeaponBundle::WeaponBundle(const WeaponBundle& copy)
{
	bundle_num = copy.bundle_num;
	WeaponList = copy.WeaponList;
	this->copy = copy.copy;
	if (this->copy)
		TYPE_Copy(copy.title, title);
	else 
		title = copy.title;
	level = copy.level;
}

ModelItems::OutfitModel& ModelItems::OutfitModel::operator=(const ModelItems::OutfitModel& from)
{
	copy = from.copy;
	kind = from.kind;
	if (copy)
	{
		TYPE_Copy(from.flavour, flavour);
		TYPE_Copy(from.token_id, token_id);
	}
	else
	{
		flavour = from.flavour;
		token_id = from.token_id;
	}
	return *this;
}

ModelItems::LemonModel& ModelItems::LemonModel::operator=(const ModelItems::LemonModel& from)
{
	copy = from.copy;
	cap = from.cap;
	cloth = from.cloth;
	fire_arm = from.fire_arm;
	cold_arm = from.cloth;
	back = from.back;
	attached_bundles = from.attached_bundles;
	if (this->copy)
	{
		TYPE_Copy(from.exo, exo);
		TYPE_Copy(from.eyes, eyes);
		TYPE_Copy(from.head, head);
		TYPE_Copy(from.teeth, teeth);
		TYPE_Copy(from.face, face);
	}
	else
	{
		exo = from.exo;
		eyes = from.eyes;
		head = from.head;
		teeth = from.teeth;
		face = from.face;
	}
	
	return *this;
}

ModelItems::WeaponBundleItem& ModelItems::WeaponBundleItem::operator=(const ModelItems::WeaponBundleItem& from)
{
	item_type = from.item_type;
	TYPE_Copy(from.skin, skin);
	slot_type = from.slot_type;
	return *this;
}

ModelItems::WeaponBundle& ModelItems::WeaponBundle::operator=(const ModelItems::WeaponBundle& from)
{
	bundle_num = from.bundle_num;
	WeaponList = from.WeaponList;
	this->copy = from.copy;
	if (this->copy)
		TYPE_Copy(from.title, title);
	else
		title = from.title;
	level = from.level;

	return *this;
}

ModelItems::Item& ModelItems::Item::operator=(const ModelItems::Item& from)
{
	copy = from.copy;
	in_fight = from.in_fight;
	model = from.model;
	switch (model)
	{
	case ModelItems::Model::LEMON:
		lemon = from.lemon;
		outfit.token_id = nullptr;
		outfit.flavour = nullptr;
		break;
	case ModelItems::Model::OUTFIT_MODEL:
		outfit = from.outfit;
		//lemon.attached_bundles.release();
		lemon.exo = nullptr;
		lemon.eyes = nullptr;
		lemon.head = nullptr;
		lemon.teeth = nullptr;
		lemon.face = nullptr;
		break;
	case ModelItems::Model::DEFAULT:
		break;
	default:
		break;
	}
	if (from.copy)
	{
		TYPE_Copy(from.token_id, token_id);
		TYPE_Copy(from.media, media);
		TYPE_Copy(from.owner_id, owner_id);
	}
	else
	{
		token_id = from.token_id;
		media = from.media;
		owner_id = from.owner_id;
	}

	return *this;
}


ModelItems::OutfitModel::OutfitModel(bool copy) :copy(copy), flavour(nullptr), token_id(nullptr), kind(ModelItems::OutfitKind::DEFAULT)
{
}

ModelItems::OutfitModel::OutfitModel(const OutfitModel& copy): copy(copy.copy)
{
	flavour = copy.flavour;
	kind = copy.kind;
	if (this->copy)
	{
		TYPE_Copy(copy.token_id, token_id);
	}
	else
	{
		token_id = copy.token_id;
	}
}

ModelItems::WeaponBundleItem::WeaponBundleItem() : copy(true), item_type(WeaponBundleItemType::NONE), skin(nullptr), slot_type(WeaponBundleSlotType::NONE)
{
}

ModelItems::WeaponBundleItem::WeaponBundleItem(bool copy) : copy(copy), item_type(WeaponBundleItemType::NONE), skin(nullptr), slot_type(WeaponBundleSlotType::NONE)
{
}

ModelItems::WeaponBundleItem::WeaponBundleItem(WeaponBundleItemType item_type, WeaponBundleSlotType slot_type, TYPE_CHAR* skin) : copy(true), item_type(item_type), slot_type(slot_type), skin(skin)
{
}

ModelItems::WeaponBundleItem::~WeaponBundleItem()
{
	if (copy)
	{
		Helper::free(&skin);
	}
}

ModelItems::WeaponBundle::WeaponBundle() :bundle_num(-1), title(nullptr), level(-1), WeaponList(ObjectList<WeaponBundleItem>(-1)), copy(false)
{
}

ModelItems::WeaponBundle::WeaponBundle(int size_WeaponList, bool copy) : bundle_num(-1), title(nullptr), level(-1), WeaponList(size_WeaponList), copy(copy)
{
}

ModelItems::WeaponBundle::WeaponBundle(WeaponBundleItem* WeaponList, int size_WeaponList, bool copy) : bundle_num(-1), title(nullptr), level(-1), WeaponList(WeaponList, size_WeaponList), copy(copy)
{
}

ModelItems::WeaponBundle::~WeaponBundle()
{
	if (copy)
	{
		Helper::free(&title);
	}
}

ModelItems::LemonModel::LemonModel(bool copy) :copy(copy), cap(copy), cloth(copy), fire_arm(copy), cold_arm(copy), back(copy), exo(nullptr), eyes(nullptr), head(nullptr), teeth(nullptr), face(nullptr), attached_bundles(-1)
{
}

ModelItems::LemonModel::LemonModel(int size_attached_bundles, int size_items[], bool copy) :attached_bundles(size_attached_bundles), copy(copy), cap(copy), cloth(copy), fire_arm(copy), cold_arm(copy), back(copy), 
																							exo(nullptr), eyes(nullptr), head(nullptr), teeth(nullptr), face(nullptr)
{
	if (size_attached_bundles != -1)
	{
		for (int i = 0; i < size_attached_bundles; i++)
		{
			attached_bundles.setObject(WeaponBundle(copy, size_items[i]), i);
		}
	}
}

/*ModelItems::Item::Item() :copy(true), lemon(true), outfit(true), token_id(nullptr), media(nullptr), owner_id(nullptr), in_fight(false), model(ModelItems::Model::DEFAULT)
{
}*/

ModelItems::EditBundleRequest::EditBundleRequest(const int& bundle_num, const TYPE_CHAR* const title, ObjectList<ModelItems::WeaponBundleItem>* const items) :bundle_num(&bundle_num), items(items), title(title)
{
}


ModelItems::AttachBundleRequest::AttachBundleRequest(const signed int& bundle_num, const TYPE_CHAR* lemon_id) :bundle_num(&bundle_num), lemon_id(lemon_id)
{
}

ModelItems::DetachBundleRequest::DetachBundleRequest(const signed int& bundle_num, const TYPE_CHAR* lemon_id) : AttachBundleRequest(bundle_num, lemon_id)
{
}

ModelInternalMM::RoomInfoResponse::RoomInfoResponse(void* readRoomInfoResponsePtr, bool copy) : copy(copy), room_id(nullptr)
{
	switch (((game::battlemon::mm::internal::RoomInfoResponse*)readRoomInfoResponsePtr)->mode().match_mode())
	{
	case game::battlemon::mm::MatchMode::NONE:
		this->mode.match_mode = ModelMM::MatchMode::NONE;
		break;
	case game::battlemon::mm::MatchMode::EQUIPMENT:
		this->mode.match_mode = ModelMM::MatchMode::EQUIPMENT;
		break;
	case game::battlemon::mm::MatchMode::REALISM:
		this->mode.match_mode = ModelMM::MatchMode::EQUIPMENT;
		break;
	default:
		this->mode.match_mode = ModelMM::MatchMode::NONE;
		break;
	}
	switch (((game::battlemon::mm::internal::RoomInfoResponse*)readRoomInfoResponsePtr)->mode().match_type())
	{
	case game::battlemon::mm::MatchType::DEATH_MATCH:
		this->mode.match_type = ModelMM::MatchType::DEATH_MATCH;
		break;
	case game::battlemon::mm::MatchType::CATCH_THE_FLAG:
		this->mode.match_type = ModelMM::MatchType::CATCH_THE_FLAG;
		break;
	}

	int size = ((game::battlemon::mm::internal::RoomInfoResponse*)readRoomInfoResponsePtr)->players().size();
	ObjectList<ModelInternalMM::RoomPlayerInfo> players;

	if (copy)
	{
		TYPE_Copy(TYPE_ReConv(((game::battlemon::mm::internal::RoomInfoResponse*)readRoomInfoResponsePtr)->room_id()), this->room_id);

		for (int i = 0; i < size; i++)
		{
			players[i].copy = copy;
			const game::battlemon::mm::internal::RoomPlayerInfo* pi = &((game::battlemon::mm::internal::RoomInfoResponse*)readRoomInfoResponsePtr)->players().Get(i);
			TYPE_Copy(TYPE_ReConv(pi->near_id()), players[i].near_id);
			players[i].lemon = ModelItems::Item((void*)&pi->lemon(), copy);
		}
	}
	else
	{
		this->room_id = (TYPE_CHAR*)((game::battlemon::mm::internal::RoomInfoResponse*)readRoomInfoResponsePtr)->room_id().c_str();
		for (int i = 0; i < size; i++)
		{
			players[i].copy = copy;
			const game::battlemon::mm::internal::RoomPlayerInfo* pi = &((game::battlemon::mm::internal::RoomInfoResponse*)readRoomInfoResponsePtr)->players().Get(i);
			players[i].near_id = (TYPE_CHAR*)pi->near_id().c_str();
			players[i].lemon = ModelItems::Item((void*)&pi->lemon(), copy);
		}
	}
	this->players = players;
}
ModelInternalMM::RoomInfoResponse::RoomInfoResponse(): copy(false), room_id(nullptr), players(0)
{
	mode.match_mode = ModelMM::MatchMode::NONE;
	mode.match_type = ModelMM::MatchType::DEFAULT;
}

ModelInternalMM::RoomPlayerInfo::RoomPlayerInfo(const ModelInternalMM::RoomPlayerInfo& copy): copy(copy.copy), lemon(copy.lemon)
{
	if (this->copy)
	{
		TYPE_Copy(copy.near_id, near_id);
	}
	else
	{
		near_id = copy.near_id;
	}
}

ModelInternalMM::RoomPlayerInfo::~RoomPlayerInfo()
{
	if (copy)
		Helper::free(&near_id);
}

ModelInternalMM::RoomInfoResponse::~RoomInfoResponse()
{
	if (copy)
		Helper::free(&room_id);
}

ModelInternalMM::SaveBattleResultRequest::SaveBattleResultRequest(const TYPE_CHAR* room_id, const ObjectList<ModelInternalMM::InternalPlayerResult>* results):room_id(room_id), results(results)
{
}

ModelInternalMM::CreateRoomRequest::CreateRoomRequest(const ModelMM::GameMode* mode, const ObjectList<const TYPE_CHAR*>* near_ids):mode(mode), near_ids(near_ids)
{
}

ModelInternalMM::DedicatedServerIsReadyRequest::DedicatedServerIsReadyRequest(const TYPE_CHAR* room_id): room_id(room_id)
{
}

ModelInternalMM::InternalUserLeftBattleRequest::InternalUserLeftBattleRequest(const TYPE_CHAR* near_id, const TYPE_CHAR* room_id): near_id(near_id), room_id(room_id)
{
}

ModelMM::AcceptGameRequest::AcceptGameRequest(const TYPE_CHAR* const lemon_id): lemon_id(lemon_id)
{
}

ModelMM::SearchGameRequest::SearchGameRequest(const GameMode* const game_mode): game_mode(game_mode)
{
}

ModelInternalMM::RoomInfoRequest::RoomInfoRequest(const TYPE_CHAR* const room_id) : room_id(room_id)
{
}

ModelInternalMM::RoomPlayerInfo::RoomPlayerInfo()
{
}

ModelUpdates::MessageData::MessageData(const void* Data, const unsigned long &ByteSize) :Data(Data), ByteSize(ByteSize)
{
}

using game::battlemon::updates::RoomInfo;
using game::battlemon::updates::RoomNeedAccept;
using game::battlemon::updates::Update;
using game::battlemon::updates::UpdateMessage;
using game::battlemon::updates::RoomPlayer;

ModelUpdates::Update ModelUpdates::MessageData::ReadUpdate() const
{
	game::battlemon::updates::Update update;
	return ModelUpdates::Update();
}

ModelUpdates::UpdateMessage ModelUpdates::MessageData::ReadUpdateMessage() const
{

	return ModelUpdates::UpdateMessage();
}

ModelUpdates::RoomNeedAccept ModelUpdates::MessageData::ReadRoomNeedAccept() const
{
	return ModelUpdates::RoomNeedAccept();
}

ModelUpdates::RoomInfo ModelUpdates::MessageData::ReadRoomInfo() const
{
	return ModelUpdates::RoomInfo();
}

ModelUpdates::RoomPlayer ModelUpdates::MessageData::ReadRoomPlayer() const
{
	return ModelUpdates::RoomPlayer();
}
