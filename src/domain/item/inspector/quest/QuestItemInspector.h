#pragma once

#include <cstdint>

#include "ItemTemplate.h"

#include "AbstractItemInspector.h"
#include "ItemActionStruct.h"
#include "definition/enum/QuestItemEnum.h"

class QuestItemInspector : public AbstractItemInspector
{
public:
	QuestItemInspector(
		uint64_t playerLowGUID,
		uint64_t itemLowGUID
	) : AbstractItemInspector(playerLowGUID, itemLowGUID)
	{}

	bool isInspectable() const override
	{
		const uint8_t itemClass = this->getCurrentItemClass();

		return AbstractItemInspector::isInspectable() && itemClass == ITEM_CLASS_QUEST;
	}

	ItemActionStruct determineItemAction() const override
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

		if (player->HasQuestForItem(itemTemplate->ItemId))
			return this->getKeepItemAction();

		if (
			this->isZulGurubCurrency(itemTemplate->ItemId)
			|| this->isZulGurubToken(itemTemplate->ItemId)
			|| this->isZulGurubDoll(itemTemplate->ItemId)
			|| itemTemplate->ItemId == uint32_t(QuestItemEnum::PRIMAL_HAKKARI_IDOL)
		)
		{
			return this->getKeepItemAction();
		}

		return this->getDestroyItemAction();
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

	// @TODO: Move this to a dedicated class.
	static constexpr std::array<QuestItemEnum, 18> ZulGurubCurrencies = {
		QuestItemEnum::ZULIAN_COIN,
		QuestItemEnum::RAZZASHI_COIN,
		QuestItemEnum::HAKKARI_COIN,
		QuestItemEnum::GURUBASHI_COIN,
		QuestItemEnum::VILEBREW_COIN,
		QuestItemEnum::WITHERBARK_COIN,
		QuestItemEnum::SANDFURY_COIN,
		QuestItemEnum::SKULLSPLITTER_COIN,
		QuestItemEnum::BLOODSCALP_COIN,

		QuestItemEnum::RED_HAKKARI_BIJOU,
		QuestItemEnum::BLUE_HAKKARI_BIJOU,
		QuestItemEnum::YELLOW_HAKKARI_BIJOU,
		QuestItemEnum::ORANGE_HAKKARI_BIJOU,
		QuestItemEnum::GREEN_HAKKARI_BIJOU,
		QuestItemEnum::PURPLE_HAKKARI_BIJOU,
		QuestItemEnum::BRONZE_HAKKARI_BIJOU,
		QuestItemEnum::SILVER_HAKKARI_BIJOU,
		QuestItemEnum::GOLD_HAKKARI_BIJOU
	};

	static constexpr std::array<QuestItemEnum, 9> ZulGurubTokens = {
		QuestItemEnum::PRIMAL_HAKKARI_BINDINGS,
		QuestItemEnum::PRIMAL_HAKKARI_ARMSPLINT,
		QuestItemEnum::PRIMAL_HAKKARI_STANCHION,
		QuestItemEnum::PRIMAL_HAKKARI_GIRDLE,
		QuestItemEnum::PRIMAL_HAKKARI_SASH,
		QuestItemEnum::PRIMAL_HAKKARI_SHAWL,
		QuestItemEnum::PRIMAL_HAKKARI_TABARD,
		QuestItemEnum::PRIMAL_HAKKARI_KOSSACK,
		QuestItemEnum::PRIMAL_HAKKARI_AEGIS
	};

	static constexpr std::array<QuestItemEnum, 9> ZulGurubDolls = {
		QuestItemEnum::PUNCTURED_VOODOO_DOLL_WARRIOR,
		QuestItemEnum::PUNCTURED_VOODOO_DOLL_ROGUE,
		QuestItemEnum::PUNCTURED_VOODOO_DOLL_PALADIN,
		QuestItemEnum::PUNCTURED_VOODOO_DOLL_HUNTER,
		QuestItemEnum::PUNCTURED_VOODOO_DOLL_SHAMAN,
		QuestItemEnum::PUNCTURED_VOODOO_DOLL_MAGE,
		QuestItemEnum::PUNCTURED_VOODOO_DOLL_WARLOCK,
		QuestItemEnum::PUNCTURED_VOODOO_DOLL_PRIEST,
		QuestItemEnum::PUNCTURED_VOODOO_DOLL_DRUID
	};

	[[nodiscard]] bool isZulGurubCurrency(const uint32_t itemTemplateId) const noexcept
	{
		for (const QuestItemEnum zulGurubCurrency : ZulGurubCurrencies)
		{
			if (itemTemplateId == uint32_t(zulGurubCurrency))
			{
				return true;
			}
		}

		return false;
	}

	[[nodiscard]] bool isZulGurubToken(const uint32_t itemTemplateId) const noexcept
	{
		for (const QuestItemEnum zulGurubToken : ZulGurubTokens)
		{
			if (itemTemplateId == uint32_t(zulGurubToken))
			{
				return true;
			}
		}

		return false;
	}

	[[nodiscard]] bool isZulGurubDoll(const uint32_t itemTemplateId) const noexcept
	{
		for (const QuestItemEnum zulGurubDoll : ZulGurubDolls)
		{
			if (itemTemplateId == uint32_t(zulGurubDoll))
			{
				return true;
			}
		}

		return false;
	}

	const std::unordered_set<uint32_t> getForbiddenItemsGUIDs() const
	{
		return {};
	}
};
