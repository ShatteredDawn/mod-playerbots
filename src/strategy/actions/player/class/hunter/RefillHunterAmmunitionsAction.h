/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include <cstdint>
#include <string>

#include "Action.h"
#include "DBCStores.h"
#include "ItemTemplate.h"
#include "Player.h"
#include "PlayerbotAI.h"
#include "ReputationMgr.h"
#include "SharedDefines.h"
#include "abstract/AbstractRefillReagentAction.h"
#include "class/hunter/definition/enum/HunterAmmunitions.enum.h"
#include "hunter/definition/struct/AmmunitionReputation.struct.h"
#include "hunter/definition/struct/QuiverState.struct.h"

class RefillHunterAmmunitionsAction : public AbstractRefillReagentAction
{
public:
    RefillHunterAmmunitionsAction(PlayerbotAI* botAI) : AbstractRefillReagentAction(botAI, "refill_hunter_ammunitions") {}

	bool isPossible() override
	{
		return this->getQuiverState().type > ITEM_SUBCLASS_QUIVER1;
	}

	bool isUseful() override
	{
		return this->getQuiverState().freeSlots > 0;
	}

    bool Execute(Event event) override
	{
		const HunterAmmunitionsEnum desiredAmmunition = this->getOptimalAmmunition();
		const bool successfullClean = this->cleanQuiver(desiredAmmunition);
		const QuiverStateStruct quiverState = this->getQuiverState();

		const Item* const item = Item::CreateItem(uint32_t(desiredAmmunition), 1);

		if (item == nullptr)
		{
			return false;
		}

		const ItemTemplate* const itemTemplate = item->GetTemplate();

		return this->bot->AddItem(uint32_t(desiredAmmunition), quiverState.freeSlots * itemTemplate->GetMaxStackSize());
	}

private:

	[[nodiscard]] bool cleanQuiver(const HunterAmmunitionsEnum desiredAmmunitionEntry)
	{
		const QuiverStateStruct quiverState = this->getQuiverState();

		const Item* const possibleBag = this->bot->GetItemByPos(INVENTORY_SLOT_BAG_0, quiverState.bagSlot);

		if (possibleBag == nullptr)
		{
			return false;
		}

		const Bag* const bag = possibleBag->ToBag();

		if (bag == nullptr)
		{
			return false;
		}

		const uint32_t bagSize = bag->GetBagSize();

		for (uint32_t i = 0; i < bagSize; ++i)
		{
			const Item* const item = bag->GetItemByPos(i);

			if (item == nullptr)
			{
				continue;
			}

			const ItemTemplate* const itemTemplate = item->GetTemplate();

			if (itemTemplate == nullptr)
			{
				continue;
			}

			if (itemTemplate->ItemId != uint32_t(desiredAmmunitionEntry))
			{
				this->bot->RemoveItem(quiverState.bagSlot, i, true);
			}
		}


		return true;
	}

	[[nodiscard]] QuiverStateStruct getQuiverState() const noexcept
	{
		// Look for the first soul shard in any bag and destroy it
		for (uint32_t i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
		{
			const Item* const item = this->bot->GetItemByPos(INVENTORY_SLOT_BAG_0, i);

			if (item == nullptr)
			{
				continue;
			}

			const Bag* const possibleBag = item->ToBag();

			if (possibleBag == nullptr)
			{
				continue;
			}

			const ItemTemplate* const itemTemplate = possibleBag->GetTemplate();

			if (itemTemplate->Class == ITEM_CLASS_QUIVER)
			{
				return {
					.freeSlots = possibleBag->GetFreeSlots(),
					.type = itemTemplate->SubClass,
					.bagSlot = i,
				};
			}
		}

		return {
			.freeSlots = 0,
			.type = 0,
			.bagSlot = 0,
		};
	}

	[[nodiscard]] HunterAmmunitionsEnum getOptimalAmmunition() const noexcept
	{
		const QuiverStateStruct quiverState = this->getQuiverState();

		if (quiverState.type == ITEM_SUBCLASS_QUIVER)
		{
			return this->getOptimalArrow();
		}

		return this->getOptimalBullet();
	}

	[[nodiscard]] HunterAmmunitionsEnum getOptimalArrow() const noexcept
	{
		const uint8_t level = this->bot->GetLevel();
		const AmmunitionReputationStruct ammunitionReputation = this->getAmmunitionReputationRanks();

		if (level < 10)
		{
			return HunterAmmunitionsEnum::ROUGH_ARROW;
		}

		if (level < 25)
		{
			return HunterAmmunitionsEnum::SHARP_ARROW;
		}

		if (level < 40)
		{
			return HunterAmmunitionsEnum::RAZOR_ARROW;
		}

		if (level < 61)
		{
			if (level > 50 && ammunitionReputation.alteracValley > REP_FRIENDLY)
			{
				return HunterAmmunitionsEnum::ICE_THREADED_ARROW;
			}

			return HunterAmmunitionsEnum::JAGGED_ARROW;
		}

		if (level < 62)
		{
			if (ammunitionReputation.cenarionExpedition > REP_NEUTRAL)
			{
				return HunterAmmunitionsEnum::SCOUT_ARROW;
			}

			return HunterAmmunitionsEnum::WICKED_ARROW;
		}

		if (level < 63 && ammunitionReputation.cenarionExpedition > REP_FRIENDLY)
		{
			return HunterAmmunitionsEnum::WARDEN_ARROW;
		}

		if (level < 66)
		{
			return HunterAmmunitionsEnum::BLACKFLIGHT_ARROW;
		}

		if (level < 67)
		{
			return HunterAmmunitionsEnum::HALAANI_RAZORSHAFT;
		}

		if (level < 71 && ammunitionReputation.scaleOfTheSands > REP_FRIENDLY)
		{
			return HunterAmmunitionsEnum::TIMELESS_ARROW;
		}

		if (level < 72 && ammunitionReputation.violetEye > REP_HONORED)
		{
			return HunterAmmunitionsEnum::MYSTERIOUS_ARROW;
		}

		if (level < 75)
		{
			return HunterAmmunitionsEnum::HALAANI_RAZORSHAFT;
		}

		return HunterAmmunitionsEnum::TERRORSHAFT_ARROW;
	}

	[[nodiscard]] HunterAmmunitionsEnum getOptimalBullet() const noexcept
	{
		const uint8_t level = this->bot->GetLevel();
		const AmmunitionReputationStruct ammunitionReputation = this->getAmmunitionReputationRanks();

		if (level < 10)
		{
			return HunterAmmunitionsEnum::LIGHT_SHOT;
		}

		if (level < 25)
		{
			return HunterAmmunitionsEnum::HEAVY_SHOT;
		}

		if (level < 40)
		{
			return HunterAmmunitionsEnum::SOLID_SHOT;
		}

		if (level < 61)
		{
			if (level > 50 && ammunitionReputation.alteracValley > REP_FRIENDLY)
			{
				return HunterAmmunitionsEnum::ICE_THREADED_BULLET;
			}

			return HunterAmmunitionsEnum::ACCURATE_SLUGS;
		}

		if (level < 62)
		{
			return HunterAmmunitionsEnum::IMPACT_SHOT;
		}

		if (level < 66)
		{
			return HunterAmmunitionsEnum::IRONBITE_SHELL;
		}

		if (level < 67)
		{
			return HunterAmmunitionsEnum::HALAANI_GRIMSHOT;
		}

		if (level < 69 && ammunitionReputation.hellfirePeninsula > REP_HONORED)
		{
			if (this->bot->GetTeamId() == TEAM_ALLIANCE)
			{
				return HunterAmmunitionsEnum::FELBANE_SLUGS;
			}

			return HunterAmmunitionsEnum::HELLFIRE_SHOT;
		}

		if (level < 71 && ammunitionReputation.scaleOfTheSands > REP_FRIENDLY)
		{
			return HunterAmmunitionsEnum::TIMELESS_SHELL;
		}

		if (level < 72 && ammunitionReputation.violetEye > REP_HONORED)
		{
			return HunterAmmunitionsEnum::MYSTERIOUS_SHELL;
		}

		if (level < 75)
		{
			return HunterAmmunitionsEnum::HALAANI_GRIMSHOT;
		}

		return HunterAmmunitionsEnum::FROSTBITE_BULLETS;
	}

	[[nodiscard]] AmmunitionReputationStruct getAmmunitionReputationRanks() const noexcept
	{
		if (this->bot->GetTeamId() == TEAM_ALLIANCE)
		{
			return this->getAllianceAmmunitionReputationRanks();
		}

		return this->getHordeAmmunitionReputationRanks();
	}

	[[nodiscard]] AmmunitionReputationStruct getAllianceAmmunitionReputationRanks() const noexcept
	{
		const ReputationMgr& repMgr = this->bot->GetReputationMgr();

		const FactionEntry* const stormpikeGuard = sFactionStore.LookupEntry(730);
		const FactionEntry* const cenarionExpedition = sFactionStore.LookupEntry(942);
		const FactionEntry* const scaleOfTheSands = sFactionStore.LookupEntry(990);
		const FactionEntry* const violetEye = sFactionStore.LookupEntry(967);
		const FactionEntry* const honorHold = sFactionStore.LookupEntry(946);


		return {
			.alteracValley = repMgr.GetRank(stormpikeGuard),
			.cenarionExpedition = repMgr.GetRank(cenarionExpedition),
			.scaleOfTheSands = repMgr.GetRank(scaleOfTheSands),
			.violetEye = repMgr.GetRank(violetEye),
			.hellfirePeninsula = repMgr.GetRank(honorHold),
		};
	}

	[[nodiscard]] AmmunitionReputationStruct getHordeAmmunitionReputationRanks() const noexcept
	{
		const ReputationMgr& repMgr = this->bot->GetReputationMgr();

		const FactionEntry* const frostwolfClan = sFactionStore.LookupEntry(729);
		const FactionEntry* const cenarionExpedition = sFactionStore.LookupEntry(942);
		const FactionEntry* const scaleOfTheSands = sFactionStore.LookupEntry(990);
		const FactionEntry* const violetEye = sFactionStore.LookupEntry(967);
		const FactionEntry* const thrallmar = sFactionStore.LookupEntry(947);

		return {
			.alteracValley = repMgr.GetRank(frostwolfClan),
			.cenarionExpedition = repMgr.GetRank(cenarionExpedition),
			.scaleOfTheSands = repMgr.GetRank(scaleOfTheSands),
			.violetEye = repMgr.GetRank(violetEye),
			.hellfirePeninsula = repMgr.GetRank(thrallmar),
		};
	}
};
