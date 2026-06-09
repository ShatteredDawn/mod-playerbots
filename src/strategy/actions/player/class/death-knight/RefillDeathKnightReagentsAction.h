/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include <string>

#include "Action.h"
#include "PlayerbotAI.h"
#include "abstract/AbstractRefillReagentAction.h"
#include "class/death-knight/definition/enum/DeathKnightReagents.enum.h"

class RefillDeathKnightReagentsAction : public AbstractRefillReagentAction
{
public:
    RefillDeathKnightReagentsAction(PlayerbotAI* botAI) : AbstractRefillReagentAction(botAI, "refill_death_knight_reagents") {}

	bool isPossible() override
	{
		if (!this->isHighLevelEnough())
		{
			return false;
		}

		return this->bot->GetFreeInventorySpace() > 0;
	}

	bool isUseful() override
	{
		if (!this->hasEnoughCorpseDust())
		{
			return true;
		}

		return false;
	}

    bool Execute(Event event) override
	{
		bool result = true;

		if (!this->hasEnoughCorpseDust())
		{
			result = result && this->refillReagent(uint32_t(DeathKnightReagentsEnum::CORPSE_DUST));
		}

		return result;
	}

private:
	// @TODO: It might be a good idea to check if the bot has any spell requiring the reagent.
	// This would allow people to customise their spells. Niche use.
	[[nodiscard]] bool isHighLevelEnough() const noexcept
	{
		return this->bot->GetLevel() > 54;
	}

	[[nodiscard]] bool hasEnoughCorpseDust() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 55,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(DeathKnightReagentsEnum::CORPSE_DUST)
			}
		);
	}
};
