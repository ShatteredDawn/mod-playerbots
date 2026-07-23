#pragma once

#include <cstdint>

#include "DeathKnightReagents.enum.h"
#include "DruidReagents.enum.h"
#include "ItemTemplate.h"

#include "AbstractItemInspector.h"
#include "ItemActionStruct.h"
#include "MageReagents.enum.h"
#include "ObjectAccessor.h"
#include "PaladinReagents.enum.h"
#include "PriestReagents.enum.h"
#include "ShamanReagents.enum.h"
#include "SharedDefines.h"
#include "WarlockReagents.enum.h"

class ReagentItemInspector : public AbstractItemInspector
{
public:
	ReagentItemInspector(
		uint64_t playerLowGUID,
		uint64_t itemLowGUID
	) : AbstractItemInspector(playerLowGUID, itemLowGUID)
	{}

	bool isInspectable() const override
	{
		const uint8_t itemClass = this->getCurrentItemClass();

		return AbstractItemInspector::isInspectable() && itemClass == ITEM_CLASS_REAGENT;
	}

	ItemActionStruct determineItemAction() const override
	{
		if (!this->isInspectable())
			return this->getDefaultItemAction();

		if (this->isForbiddenItem())
			return this->getForbiddenItemAction();

		if (this->shouldBeKept())
		{
			return this->getKeepItemAction();
		}

		return this->getSellAction();
	}

	bool isForbiddenItem() const
	{
		const std::unordered_set<uint32_t> forbiddenItems = this->getForbiddenItemsGUIDs();
		const ItemTemplate* const itemTemplate = this->getCurrentItemTemplate();

		if (itemTemplate == nullptr)
			return false;

		return forbiddenItems.contains(itemTemplate->ItemId);
	}

protected:

	[[nodiscard]] bool shouldBeKept() const noexcept
	{
		const Player* const player = ObjectAccessor::FindPlayerByLowGUID(this->playerLowGUID);

		if (player == nullptr)
		{
			return false;
		}

		const uint8_t botClass = player->getClass();

		switch (botClass)
		{
			case CLASS_DEATH_KNIGHT:
				return this->isActiveDeathKnightReagent();

			case CLASS_DRUID:
				return this->isActiveDruidReagent();

			case CLASS_MAGE:
				return this->isActiveMageReagent();

			case CLASS_PALADIN:
				return this->isActivePaladinReagent();

			case CLASS_PRIEST:
				return this->isActivePriestReagent();

			case CLASS_SHAMAN:
				return this->isActiveShamanReagent();

			case CLASS_WARLOCK:
				return this->isActiveWarlockReagent();

			default:
				return false;
		}
	}

	[[nodiscard]] bool isActiveDeathKnightReagent() const noexcept
	{
		return this->itemLowGUID == uint32_t(DeathKnightReagentsEnum::CORPSE_DUST);
	}

	[[nodiscard]] bool isActiveDruidReagent() const noexcept
	{
		const Player* const player = ObjectAccessor::FindPlayerByLowGUID(this->playerLowGUID);

		if (player == nullptr)
		{
			return false;
		}

		const uint8_t botLevel = player->GetLevel();

		if (botLevel < 20)
		{
			return false;
		}

		if (botLevel < 30)
		{
			return this->itemLowGUID == uint32_t(DruidReagentsEnum::MAPLE_SEED);
		}

		if (botLevel < 40)
		{
			return this->itemLowGUID == uint32_t(DruidReagentsEnum::STRANGLETHORN_SEED);
		}

		if (botLevel < 50)
		{
			return this->itemLowGUID == uint32_t(DruidReagentsEnum::ASHWOOD_SEED);
		}

		if (botLevel < 60)
		{
			return this->itemLowGUID == uint32_t(DruidReagentsEnum::HORNBEAM_SEED)
				|| this->itemLowGUID == uint32_t(DruidReagentsEnum::WILD_BERRIES);
		}

		if (botLevel < 70 && this->itemLowGUID == uint32_t(DruidReagentsEnum::WILD_THORNROOT))
		{
			return true;
		}

		if (botLevel < 69)
		{
			return this->itemLowGUID == uint32_t(DruidReagentsEnum::IRONWOOD_SEED);
		}

		if (botLevel < 80 && this->itemLowGUID == uint32_t(DruidReagentsEnum::WILD_QUILLVINE))
		{
			return true;
		}

		if (botLevel < 79)
		{
			return this->itemLowGUID == uint32_t(DruidReagentsEnum::FLINTWEED_SEED);
		}

		return this->itemLowGUID == uint32_t(DruidReagentsEnum::STARLEAF_SEED)
			|| this->itemLowGUID == uint32_t(DruidReagentsEnum::WILD_SPINELEAF);
	}

	[[nodiscard]] bool isActiveMageReagent() const noexcept
	{
		const Player* const player = ObjectAccessor::FindPlayerByLowGUID(this->playerLowGUID);

		if (player == nullptr)
		{
			return false;
		}

		const uint8_t botLevel = player->GetLevel();

		if (botLevel < 20)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(MageReagentsEnum::RUNE_OF_PORTALS))
		{
			return true;
		}

		if (botLevel < 40)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(MageReagentsEnum::RUNE_OF_TELEPORTATION))
		{
			return true;
		}

		if (botLevel < 56)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(MageReagentsEnum::ARCANE_POWDER))
		{
			return true;
		}

		return false;
	}

	[[nodiscard]] bool isActivePaladinReagent() const noexcept
	{
		const Player* const player = ObjectAccessor::FindPlayerByLowGUID(this->playerLowGUID);

		if (player == nullptr)
		{
			return false;
		}

		const uint8_t botLevel = player->GetLevel();

		if (botLevel < 30)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(PaladinReagentsEnum::SYMBOL_OF_DIVINITY))
		{
			return true;
		}

		if (botLevel < 52)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(PaladinReagentsEnum::SYMBOL_OF_KINGS))
		{
			return true;
		}

		return false;
	}

	[[nodiscard]] bool isActivePriestReagent() const noexcept
	{
		const Player* const player = ObjectAccessor::FindPlayerByLowGUID(this->playerLowGUID);

		if (player == nullptr)
		{
			return false;
		}

		const uint8_t botLevel = player->GetLevel();

		if (botLevel < 48)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(PriestReagentsEnum::HOLY_CANDLE))
		{
			return true;
		}

		if (botLevel < 56)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(PriestReagentsEnum::SACRED_CANDLE))
		{
			return true;
		}

		if (botLevel < 77)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(PriestReagentsEnum::DEVOUT_CANDLE))
		{
			return true;
		}

		return false;
	}

	[[nodiscard]] bool isActiveShamanReagent() const noexcept
	{
		const Player* const player = ObjectAccessor::FindPlayerByLowGUID(this->playerLowGUID);

		if (player == nullptr)
		{
			return false;
		}

		const uint8_t botLevel = player->GetLevel();

		if (botLevel < 4)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(ShamanReagentsEnum::EARTH_TOTEM))
		{
			return true;
		}

		if (botLevel < 10)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(ShamanReagentsEnum::FIRE_TOTEM))
		{
			return true;
		}

		if (botLevel < 20)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(ShamanReagentsEnum::WATER_TOTEM))
		{
			return true;
		}

		if (botLevel < 30)
		{
			return false;
		}

		return this->itemLowGUID == uint32_t(ShamanReagentsEnum::AIR_TOTEM)
			|| this->itemLowGUID == uint32_t(ShamanReagentsEnum::ANKH);
	}

	[[nodiscard]] bool isActiveWarlockReagent() const noexcept
	{
		const Player* const player = ObjectAccessor::FindPlayerByLowGUID(this->playerLowGUID);

		if (player == nullptr)
		{
			return false;
		}

		const uint8_t botLevel = player->GetLevel();

		if (botLevel < 50)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(WarlockReagentsEnum::INFERNAL_STONE))
		{
			return true;
		}

		if (botLevel < 60)
		{
			return false;
		}

		if (this->itemLowGUID == uint32_t(WarlockReagentsEnum::DEMONIC_FIGURE))
		{
			return true;
		}

		return false;
	}

	const std::unordered_set<uint32_t> getForbiddenItemsGUIDs() const
	{
		return {};
	}
};
