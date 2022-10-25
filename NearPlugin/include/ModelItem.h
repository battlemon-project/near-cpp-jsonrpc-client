#pragma once

namespace ModelMM
{
	enum class MatchType
	{
		DEATH_MATCH = 0,
		CATCH_THE_FLAG = 1,
		DEFAULT = 2
	};

	enum class MatchMode
	{
		NONE = 0,
		EQUIPMENT = 1,
		REALISM = 2,
		DEFAULT = 3
	};

	enum class SearchGameResponseStatus
	{
		OK = 0,
		NFT_ERROR = 1,
		ALREADY_IN_QUEUE = 2,
		ALREADY_IN_BATTLE = 3,
		GAMEMODE_ERROR = 4,
		INVALID_REQUEST = 5,
		INTERNAL_ERROR = 6,
		DEFAULT = 7
	};
}

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
		bool in_fight;
		LemonModel lemon;
		OutfitModel outfit;
		~Item();
	};
}