#pragma once

#include <cstdint>

#include "ItemTemplate.h"

#include "AbstractConsumableInspector.h"
#include "ItemActionStruct.h"
#include "PlayerbotAI.h"
#include "PlayerbotAIConfig.h"
#include "PlayerbotMgr.h"

class ConsumableFoodInspector : public AbstractConsumableInspector
{
public:
	ConsumableFoodInspector(
		uint64_t playerLowGUID,
		uint64_t itemLowGUID
	) : AbstractConsumableInspector(playerLowGUID, itemLowGUID)
	{}

	bool isInspectable() const
	{
		const uint8_t itemSubclass = this->getCurrentItemSubclass();

		return AbstractConsumableInspector::isInspectable() && itemSubclass == ITEM_SUBCLASS_FOOD;
	}

	ItemActionStruct determineItemAction() const
	{
		if (!this->isInspectable())
			return this->getDefaultItemAction();

		if (this->isForbiddenItem())
			return this->getForbiddenItemAction();

		const ItemTemplate* const itemTemplate = this->getCurrentItemTemplate();

		if (itemTemplate == nullptr)
			return this->getDefaultItemAction();

		const ObjectGuid playerGUID = ObjectGuid::Create<HighGuid::Player>(this->playerLowGUID);
		Player* player = ObjectAccessor::FindPlayer(playerGUID);

		if (player == nullptr)
			return this->getDefaultItemAction();

		PlayerbotAI* const playerbotAI = PlayerbotsMgr::instance().GetPlayerbotAI(player);

		if (playerbotAI == nullptr)
			return this->getDefaultItemAction();

		if (playerbotAI->HasCheat(BotCheatMask::food))
		{
			return this->getSellAction();
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

	const std::unordered_set<uint32_t> getForbiddenItemsGUIDs() const
	{
		return {

		};
	}

};
