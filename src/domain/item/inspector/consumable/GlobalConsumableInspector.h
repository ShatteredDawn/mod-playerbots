#pragma once

#include <cstdint>

#include "ItemTemplate.h"

#include "AbstractConsumableInspector.h"
#include "ItemActionStruct.h"

class GlobalConsumableInspector : public AbstractConsumableInspector
{
public:
	GlobalConsumableInspector(
		uint64_t playerLowGUID,
		uint64_t itemLowGUID
	) : AbstractConsumableInspector(playerLowGUID, itemLowGUID)
	{}

	bool isInspectable() const
	{
		const uint8_t itemSubclass = this->getCurrentItemSubclass();

		return AbstractConsumableInspector::isInspectable() && itemSubclass == ITEM_SUBCLASS_CONSUMABLE;
	}

	bool isForbiddenItem() const
	{
		const std::unordered_set<uint32_t> forbiddenItems = this->getForbiddenItemsGUIDs();
		const ItemTemplate* const itemTemplate = this->getCurrentItemTemplate();

		if (itemTemplate == nullptr)
			return false;

		return forbiddenItems.contains(itemTemplate->ItemId);
	}

	ItemActionStruct determineItemAction() const
	{
		if (!this->isInspectable())
			return this->getDefaultItemAction();

		if (this->isForbiddenItem())
			return this->getForbiddenItemAction();

		return this->getSellAction();
	}

protected:

	std::unordered_set<uint32_t> getForbiddenItemsGUIDs() const
	{
		return {};
	}

};
