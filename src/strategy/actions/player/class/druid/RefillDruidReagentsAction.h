/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include <string>

#include "Action.h"
#include "PlayerbotAI.h"
#include "abstract/AbstractRefillReagentAction.h"
#include "class/druid/definition/enum/DruidReagents.enum.h"

class RefillDruidReagentsAction : public AbstractRefillReagentAction
{
public:
    RefillDruidReagentsAction(PlayerbotAI* botAI) : AbstractRefillReagentAction(botAI, "refill_druid_reagents") {}

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
		if (!this->hasEnoughMapleSeed())
		{
			return true;
		}

		if (!this->hasEnoughStranglethornSeed())
		{
			return true;
		}

		if (!this->hasEnoughAshwoodSeed())
		{
			return true;
		}

		if (!this->hasEnoughHornbeamSeed())
		{
			return true;
		}

		if (!this->hasEnoughIronwoodSeed())
		{
			return true;
		}

		if (!this->hasEnoughFlintweedSeed())
		{
			return true;
		}

		if (!this->hasEnoughStarleafSeed())
		{
			return true;
		}

		if (!this->hasEnoughWildBerries())
		{
			return true;
		}

		if (!this->hasEnoughWildThornroot())
		{
			return true;
		}

		if (!this->hasEnoughWildQuillvine())
		{
			return true;
		}

		if (!this->hasEnoughWildSpineleaf())
		{
			return true;
		}

		return false;
	}

    bool Execute(Event event) override
	{
		bool result = true;

		if (!this->hasEnoughMapleSeed())
		{
			result = result && this->refillReagent(uint32_t(DruidReagentsEnum::MAPLE_SEED));
		}

		if (!this->hasEnoughStranglethornSeed())
		{
			result = result && this->refillReagent(uint32_t(DruidReagentsEnum::STRANGLETHORN_SEED));
		}

		if (!this->hasEnoughAshwoodSeed())
		{
			result = result && this->refillReagent(uint32_t(DruidReagentsEnum::ASHWOOD_SEED));
		}

		if (!this->hasEnoughHornbeamSeed())
		{
			result = result && this->refillReagent(uint32_t(DruidReagentsEnum::HORNBEAM_SEED));
		}

		if (!this->hasEnoughIronwoodSeed())
		{
			result = result && this->refillReagent(uint32_t(DruidReagentsEnum::IRONWOOD_SEED));
		}

		if (!this->hasEnoughFlintweedSeed())
		{
			result = result && this->refillReagent(uint32_t(DruidReagentsEnum::FLINTWEED_SEED));
		}

		if (!this->hasEnoughStarleafSeed())
		{
			result = result && this->refillReagent(uint32_t(DruidReagentsEnum::STARLEAF_SEED));
		}

		if (!this->hasEnoughWildBerries())
		{
			result = result && this->refillReagent(uint32_t(DruidReagentsEnum::WILD_BERRIES));
		}

		if (!this->hasEnoughWildThornroot())
		{
			result = result && this->refillReagent(uint32_t(DruidReagentsEnum::WILD_THORNROOT));
		}

		if (!this->hasEnoughWildQuillvine())
		{
			result = result && this->refillReagent(uint32_t(DruidReagentsEnum::WILD_QUILLVINE));
		}

		if (!this->hasEnoughWildSpineleaf())
		{
			result = result && this->refillReagent(uint32_t(DruidReagentsEnum::WILD_SPINELEAF));
		}

		return result;
	}

private:
	// @TODO: It might be a good idea to check if the bot has any spell requiring the reagent.
	// This would allow people to customise their spells. Niche use.
	[[nodiscard]] bool isHighLevelEnough() const noexcept
	{
		return this->bot->GetLevel() > 19;
	}

	[[nodiscard]] bool hasEnoughMapleSeed() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 20,
				.maximumLevel = 29,
				.itemTemplateId = uint32_t(DruidReagentsEnum::MAPLE_SEED)
			}
		);
	}

	[[nodiscard]] bool hasEnoughStranglethornSeed() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 30,
				.maximumLevel = 39,
				.itemTemplateId = uint32_t(DruidReagentsEnum::STRANGLETHORN_SEED)
			}
		);
	}

	[[nodiscard]] bool hasEnoughAshwoodSeed() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 40,
				.maximumLevel = 49,
				.itemTemplateId = uint32_t(DruidReagentsEnum::ASHWOOD_SEED)
			}
		);
	}

	[[nodiscard]] bool hasEnoughHornbeamSeed() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 50,
				.maximumLevel = 59,
				.itemTemplateId = uint32_t(DruidReagentsEnum::HORNBEAM_SEED)
			}
		);
	}

	[[nodiscard]] bool hasEnoughIronwoodSeed() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 60,
				.maximumLevel = 68,
				.itemTemplateId = uint32_t(DruidReagentsEnum::IRONWOOD_SEED)
			}
		);
	}

	[[nodiscard]] bool hasEnoughFlintweedSeed() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 69,
				.maximumLevel = 78,
				.itemTemplateId = uint32_t(DruidReagentsEnum::FLINTWEED_SEED)
			}
		);
	}

	[[nodiscard]] bool hasEnoughStarleafSeed() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 79,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(DruidReagentsEnum::STARLEAF_SEED)
			}
		);
	}

	[[nodiscard]] bool hasEnoughWildBerries() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 50,
				.maximumLevel = 59,
				.itemTemplateId = uint32_t(DruidReagentsEnum::WILD_BERRIES)
			}
		);
	}

	[[nodiscard]] bool hasEnoughWildThornroot() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 60,
				.maximumLevel = 69,
				.itemTemplateId = uint32_t(DruidReagentsEnum::WILD_THORNROOT)
			}
		);
	}

	[[nodiscard]] bool hasEnoughWildQuillvine() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 70,
				.maximumLevel = 79,
				.itemTemplateId = uint32_t(DruidReagentsEnum::WILD_QUILLVINE)
			}
		);
	}

	[[nodiscard]] bool hasEnoughWildSpineleaf() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 80,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(DruidReagentsEnum::WILD_SPINELEAF)
			}
		);
	}
};
