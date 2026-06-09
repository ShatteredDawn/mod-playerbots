/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include <string>

#include "Action.h"
#include "PlayerbotAI.h"
#include "abstract/AbstractRefillReagentAction.h"
#include "class/priest/definition/enum/PriestReagents.enum.h"

class RefillPriestReagentsAction : public AbstractRefillReagentAction
{
public:
    RefillPriestReagentsAction(PlayerbotAI* botAI) : AbstractRefillReagentAction(botAI, "refill_priest_reagents") {}

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
		if (!this->hasEnoughHolyCandle())
		{
			return true;
		}

		if (!this->hasEnoughSacredCandle())
		{
			return true;
		}

		if (!this->hasEnoughDevoutCandle())
		{
			return true;
		}

		return false;
	}

    bool Execute(Event event) override
	{
		bool result = true;

		if (!this->hasEnoughHolyCandle())
		{
			result = result && this->refillReagent(uint32_t(PriestReagentsEnum::HOLY_CANDLE));
		}

		if (!this->hasEnoughSacredCandle())
		{
			result = result && this->refillReagent(uint32_t(PriestReagentsEnum::SACRED_CANDLE));
		}

		if (!this->hasEnoughDevoutCandle())
		{
			result = result && this->refillReagent(uint32_t(PriestReagentsEnum::DEVOUT_CANDLE));
		}

		return result;
	}

private:
	// @TODO: It might be a good idea to check if the bot has any spell requiring the reagent.
	// This would allow people to customise their spells. Niche use.
	[[nodiscard]] bool isHighLevelEnough() const noexcept
	{
		return this->bot->GetLevel() > 47;
	}

	[[nodiscard]] bool hasEnoughHolyCandle() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 48,
				.maximumLevel = 60,
				.itemTemplateId = uint32_t(PriestReagentsEnum::HOLY_CANDLE)
			}
		);
	}

	[[nodiscard]] bool hasEnoughSacredCandle() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 56,
				.maximumLevel = 80,
				.itemTemplateId = uint32_t(PriestReagentsEnum::SACRED_CANDLE)
			}
		);
	}

	[[nodiscard]] bool hasEnoughDevoutCandle() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 77,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(PriestReagentsEnum::DEVOUT_CANDLE)
			}
		);
	}
};
