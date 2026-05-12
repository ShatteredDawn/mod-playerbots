/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include <string>

#include "Action.h"
#include "PlayerbotAI.h"
#include "abstract/definition/struct/HasEnoughReagentParameterStruct.h"

class AbstractRefillReagentAction : public Action
{
public:
    AbstractRefillReagentAction(PlayerbotAI* botAI, std::string name) : Action(botAI, name) {}

	bool isPossible() override
	{
		return false;
	}

	bool isUseful() override
	{
		return false;
	}

protected:
	[[nodiscard]] bool hasEnoughReagent(HasEnoughReagentParameterStruct parameters) const noexcept
	{
		const uint32_t botLevel = this->bot->GetLevel();

		if (parameters.minimumLevel > 0 && botLevel < parameters.minimumLevel)
		{
			return true;
		}

		if (parameters.maximumLevel > 0 && botLevel > parameters.maximumLevel)
		{
			return true;
		}

		const ItemTemplate* const itemTemplate = ObjectMgr::instance()->GetItemTemplate(parameters.itemTemplateId);

		if (itemTemplate == nullptr)
		{
			return false;
		}

		return this->bot->HasItemCount(parameters.itemTemplateId, itemTemplate->GetMaxStackSize());
	}

	[[nodiscard]] bool refillReagent(uint32_t itemTemplateId)
	{
		const ItemTemplate* const itemTemplate = ObjectMgr::instance()->GetItemTemplate(itemTemplateId);

		if (itemTemplate == nullptr)
		{
			return false;
		}

		const uint32_t currentQuantity = this->bot->GetItemCount(itemTemplateId);
		const uint32_t maxStackSize = itemTemplate->GetMaxStackSize();
		const uint32_t quantityToAdd = maxStackSize - currentQuantity;
		const uint32_t totalCost = itemTemplate->BuyPrice;
		const uint32_t botMoney = this->bot->GetMoney();

		if (botMoney < totalCost)
		{
			return false;
		}

		const bool successfullyAdded = this->bot->AddItem(itemTemplateId, maxStackSize - currentQuantity);

		if (!successfullyAdded)
		{
			return false;
		}

		this->bot->ModifyMoney(-totalCost);

		return true;
	}
};
