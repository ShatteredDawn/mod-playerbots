#pragma once

#include <cstdint>

#include "SharedDefines.h"
#include "ItemTemplate.h"

#include "StatsWeightCalculator.h"
#include "AbstractItemInspector.h"
#include "ItemActionStruct.h"
#include "ItemActionEnum.h"
#include "InventoryService.h"
#include "GlobalPlayerInspector.h"

class WeaponItemInspector : public AbstractItemInspector
{
public:
	WeaponItemInspector(
		uint64_t playerLowGUID,
		uint64_t itemLowGUID
	) : AbstractItemInspector(playerLowGUID, itemLowGUID)
	{}

	bool isInspectable() const override
	{
		const uint8_t itemClass = this->getCurrentItemClass();

		return AbstractItemInspector::isInspectable() && itemClass == ITEM_CLASS_WEAPON;
	}

	ItemActionStruct determineItemAction() const override
	{
		if (!this->isInspectable())
		{
			LOG_DEBUG("playerbots", "Item is not inspectable.");

			return this->getDefaultItemAction();
		}

		if (this->isForbiddenItem())
		{
			LOG_DEBUG("playerbots", "Item is forbidden");

			return this->getForbiddenItemAction();
		}

		const ObjectGuid playerGUID = ObjectGuid::Create<HighGuid::Player>(this->playerLowGUID);
		Player* player = ObjectAccessor::FindPlayer(playerGUID);

		if (player == nullptr)
		{
			LOG_DEBUG("playerbots", "Player is nullptr");

			return this->getDefaultItemAction();
		}

		Item* currentItem = this->getMutableCurrentItem();

		if (currentItem == nullptr)
		{
			LOG_DEBUG("playerbots", "new item had no template");

			return this->getDefaultItemAction();
		}

		const InventoryResult canUseItem = player->CanUseItem(currentItem);

		if (canUseItem != EQUIP_ERR_OK)
		{
			LOG_DEBUG("playerbots", "item could not be used, selling");

			return this->getSellAction();
		}

		StatsWeightCalculator statisticsWeightCalculator(player);
		const ItemTemplate* const itemTemplate = this->getCurrentItemTemplate();

		if (itemTemplate == nullptr)
		{
			LOG_DEBUG("playerbots", "Item had no template");

			return this->getDefaultItemAction();
		}

		std::vector<EquipmentSlots> slots = InventoryService::GetInstance().getItemEquipmentSlots(itemTemplate);

		const float newItemStatisticsWeight = statisticsWeightCalculator.CalculateItem(itemTemplate->ItemId);

		statisticsWeightCalculator.Reset();

		for (uint8_t i = 0; i < slots.size(); ++i)
		{
			const uint32_t equipmentSlot = slots[i];

			const Item* const currentlyEquippedItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, equipmentSlot);

			if (currentlyEquippedItem == nullptr)
			{
				LOG_DEBUG("playerbots.inspector.weapon", "No current item, equipping new");

				return {
					.action = ItemActionEnum::EQUIP,
					.bagSlot = this->getBagSlot(),
					.containerSlot = this->getItemSlot(),
					.equipmentSlot = equipmentSlot
				};
			}

			const ItemTemplate* const currentlyEquippedItemTemplate = currentlyEquippedItem->GetTemplate();

			if (currentlyEquippedItemTemplate == nullptr)
				return this->getDefaultItemAction();

			const float existingItemStatisticsWeight = statisticsWeightCalculator.CalculateItem(currentlyEquippedItemTemplate->ItemId);

			if (existingItemStatisticsWeight < newItemStatisticsWeight)
			{
				LOG_DEBUG("playerbots.inspector.weapon", "New item is better than old one.");

				return {
					.action = ItemActionEnum::EQUIP,
					.bagSlot = this->getBagSlot(),
					.containerSlot = this->getItemSlot(),
					.equipmentSlot = equipmentSlot
				};
			}
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
		return {};
	}
};
