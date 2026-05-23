/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "TrainerAction.h"

#include "AiObjectContext.h"
#include "AiFactory.h"
#include "BisListMgr.h"
#include "BudgetValues.h"
#include "RandomPlayerbotMgr.h"
#include "Event.h"
#include "PlayerbotFactory.h"
#include "PlayerbotTextMgr.h"
#include "ReputationMgr.h"
#include "SpellMgr.h"
#include "Trainer.h"

bool TrainerAction::Execute(Event event)
{
    const std::string param = event.getParam();

    Creature* const target = this->getCreatureTarget();

    if (target == nullptr)
    {
        return false;
    }

    ObjectMgr* const objectMgr = ObjectMgr::instance();

    if (objectMgr == nullptr)
    {
        return false;
    }

    const Trainer::Trainer* const trainer = objectMgr->GetTrainer(target->GetEntry());

    if (trainer == nullptr)
    {
        return false;
    }

    // NOTE: Original version uses SpellIds here, but occasionally only inserts
    // a single spell ID value from parameters. If someone wants to impl multiple
    // spells as parameters, check SkipSpellsListAction::parseIds as an example.
    const uint32_t spellId = this->chat->parseSpell(param);

    // @TODO: Move to a dedicated method instead of this boolean hell.
    const bool wasAskedToLearn = param.find("learn") != std::string::npos;
    const bool isRandomBot = RandomPlayerbotMgr::instance().IsRandomBot(bot);
    const bool isTradeSkillTrainer = trainer->GetTrainerType() == Trainer::Type::Tradeskill;
    const bool hasMaster = this->botAI->HasActivePlayerMaster();
    const bool allowLearnTrainerSpells = PlayerbotAIConfig::instance().allowLearnTrainerSpells;
    // TODO: Rewrite to only exclude start primary profession skills and make config dependent.
    const bool isAllowedToAutomaticallyLearnSpells = allowLearnTrainerSpells && (!isTradeSkillTrainer || !hasMaster);

    const bool learnSpells = wasAskedToLearn || isRandomBot || isAllowedToAutomaticallyLearnSpells;

    this->iterate(target, learnSpells, spellId);

    return true;
}

bool TrainerAction::isUseful()
{
    const Creature* const target = this->getCreatureTarget();

    if (target == nullptr)
    {
        return false;
    }

    if (!target->IsInWorld() || target->IsDuringRemoveFromWorld() || !target->IsAlive())
    {
        return false;
    }

    return target->IsTrainer();
}

bool TrainerAction::isPossible()
{
    const Creature* const target = this->getCreatureTarget();

    if (target == nullptr)
    {
        return false;
    }

    ObjectMgr* const objectMgr = ObjectMgr::instance();

    if (objectMgr == nullptr)
    {
        return false;
    }

    const Trainer::Trainer* const trainer = objectMgr->GetTrainer(target->GetEntry());

    if (trainer == nullptr)
    {
        return false;
    }

    if (!trainer->IsTrainerValidForPlayer(this->bot))
    {
        return false;
    }

    if (trainer->GetSpells().empty())
    {
        return false;
    }

    return true;
}

// There are just two scenarios: the bot has a master or it doesn't. If the
// bot has a master, the master should target a unit; otherwise, the bot
// should target the unit itself.
Unit* TrainerAction::GetTarget()
{
    const Player* const master = this->GetMaster();

    if (master == nullptr)
    {
        return this->bot->GetSelectedUnit();
    }

    return master->GetSelectedUnit();
}

Creature* TrainerAction::getCreatureTarget() noexcept
{
    Unit* const target = this->GetTarget();

    if (target == nullptr)
    {
        return nullptr;
    }

    return dynamic_cast<Creature*>(target);
}

void TrainerAction::iterate(const Creature* const creature, const bool learnSpells, const uint32_t spellId)
{
    ObjectMgr* const objectMgr = ObjectMgr::instance();

    if (objectMgr == nullptr)
    {
        return;
    }

    Trainer::Trainer* const trainer = objectMgr->GetTrainer(creature->GetEntry());

    if (trainer == nullptr)
    {
        return;
    }

    this->tellHeader(creature);

    const float reputationDiscount = this->bot->GetReputationPriceDiscount(creature);

    uint32_t totalCost = 0;

    const SpellMgr* const spellMgr = SpellMgr::instance();

    if (spellMgr == nullptr)
    {
        return;
    }

    // simplified version of Trainer::TeachSpell method
    for (const Trainer::Spell& spell : trainer->GetSpells())
    {
        if (!trainer->CanTeachSpell(this->bot, &spell))
        {
            continue;
        }

        if (spellId && spell.SpellId != spellId)
        {
            continue;
        }

        const SpellInfo* const spellInfo = spellMgr->GetSpellInfo(spell.SpellId);

        if (spellInfo == nullptr)
        {
            continue;
        }

        const uint32_t cost = uint32_t(floor(float(spell.MoneyCost) * reputationDiscount));
        totalCost += cost;

        std::ostringstream out{};
        out << chat->FormatSpell(spellInfo) << chat->formatMoney(cost);

        if (learnSpells)
        {
            out << this->learn(*spellInfo, cost);
        }

        this->botAI->TellMaster(out);
    }

    this->tellFooter(totalCost);
}

const std::string TrainerAction::learn(const SpellInfo& spellInfo, const uint32_t cost)
{
    if (!this->botAI->HasCheat(BotCheatMask::gold))
    {
        Value<uint32_t>* const freeMoneyFor = this->context->GetValue<uint32_t>("free money for", uint32_t(NeedMoneyFor::spells));

        if (freeMoneyFor == nullptr)
        {
           return " - cannot determine if I can afford it";
        }

        const uint32_t freeMoneyForSpells = freeMoneyFor->Get();

        if (freeMoneyForSpells < cost)
        {
            return " - too expensive";
        }

        this->bot->ModifyMoney(-int32_t(cost));
    }

    if (spellInfo.HasEffect(SPELL_EFFECT_LEARN_SPELL))
    {
        this->bot->CastSpell(bot, spellInfo.Id, true);

        return " - learned";
    }

    this->bot->learnSpell(spellInfo.Id, false);

    return " - learned";
}

void TrainerAction::tellHeader(const Creature* const creature) const
{
    std::ostringstream out{};

    out << "--- Can learn from " << creature->GetName() << " ---";

    this->botAI->TellMaster(out);
}

void TrainerAction::tellFooter(const uint32_t totalCost)
{
    if (totalCost == 0)
    {
        return;
    }

    std::ostringstream out{};

    out << "Total cost: " << this->chat->formatMoney(totalCost);

    this->botAI->TellMaster(out);
}

void MaintenanceAction::performAltMaintenance()
{
    const PlayerbotAIConfig& configuration = PlayerbotAIConfig::instance();
    PlayerbotFactory factory{this->bot, this->bot->GetLevel()};

    if (configuration.altMaintenanceAttunementQs)
    {
        factory.InitAttunementQuests();
    }

    if (configuration.altMaintenanceBags)
    {
        factory.InitBags(false);
    }

    if (configuration.altMaintenanceAmmo)
    {
        factory.InitAmmo();
    }

    if (configuration.altMaintenanceFood)
    {
        factory.InitFood();
    }

    if (configuration.altMaintenanceReagents)
    {
        factory.InitReagents();
    }

    if (configuration.altMaintenanceConsumables)
    {
        factory.InitConsumables();
    }

    if (configuration.altMaintenancePotions)
    {
        factory.InitPotions();
    }

    if (configuration.altMaintenanceTalentTree)
    {
        factory.InitTalentsTree(true);
    }

    if (configuration.altMaintenancePet)
    {
        factory.InitPet();
    }

    if (configuration.altMaintenancePetTalents)
    {
        factory.InitPetTalents();
    }

    if (configuration.altMaintenanceSkills)
    {
        factory.InitSkills();
    }

    if (configuration.altMaintenanceClassSpells)
    {
        factory.InitClassSpells();
    }

    if (configuration.altMaintenanceAvailableSpells)
    {
        factory.InitAvailableSpells();
    }

    if (configuration.altMaintenanceReputation)
    {
        factory.InitReputation();
    }

    if (configuration.altMaintenanceSpecialSpells)
    {
        factory.InitSpecialSpells();
    }

    if (configuration.altMaintenanceMounts)
    {
        factory.InitMounts();
    }

    if (configuration.altMaintenanceGlyphs)
    {
        factory.InitGlyphs(false);
    }

    if (configuration.altMaintenanceKeyring)
    {
        factory.InitKeyring();
    }

    if (configuration.altMaintenanceGemsEnchants && this->bot->GetLevel() >= configuration.minEnchantingBotLevel)
    {
        factory.ApplyEnchantAndGemsNew();
    }

    this->bot->DurabilityRepairAll(false, 1.0f, false);
    this->bot->SendTalentsInfoData(false);
}

bool BisGearAction::RunAutogearFallback(uint16 effectiveIlvl)
{
    if (!sPlayerbotAIConfig.autoGearCommand)
    {
        botAI->TellError(PlayerbotTextMgr::instance().GetBotTextOrDefault(
            "bis_autogear_unavailable_error",
            "autogear command is not allowed, please check the configuration.", {}));
        return false;
    }

    botAI->TellMaster(PlayerbotTextMgr::instance().GetBotTextOrDefault(
        "bis_no_rows_fallback",
        "No BiS for your tier/spec/level, check cfg, running autogear instead", {}));

    // Wipe all equipped slots so autogear gears from scratch at the requested ilvl
    // (avoids old high-tier items surviving the incremental 1.2x threshold).
    for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
    {
        if (slot == EQUIPMENT_SLOT_TABARD || slot == EQUIPMENT_SLOT_BODY)
            continue;
        if (bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            bot->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
    }

    uint32 gs = effectiveIlvl == 0
                    ? 0
                    : PlayerbotFactory::CalcMixedGearScore(effectiveIlvl, uint32_t(PlayerbotAIConfig::instance().autoGearQualityLimit));
    PlayerbotFactory factory(bot, bot->GetLevel(), uint32_t(PlayerbotAIConfig::instance().autoGearQualityLimit), gs);
    factory.InitEquipment(false, sPlayerbotAIConfig.twoRoundsGearInit);
    factory.InitAmmo();
    if (bot->GetLevel() >= sPlayerbotAIConfig.minEnchantingBotLevel)
        factory.ApplyEnchantAndGemsNew();
    bot->DurabilityRepairAll(false, 1.0f, false);
    return true;
}

bool BisGearAction::Execute(Event event)
{
    if (!sPlayerbotAIConfig.autoGearBisCommand)
    {
        botAI->TellError(PlayerbotTextMgr::instance().GetBotTextOrDefault(
            "bis_command_unavailable_error",
            "bis command is not allowed, please check the configuration.", {}));
        return false;
    }

    if (!sPlayerbotAIConfig.autoGearCommandAltBots &&
        !sPlayerbotAIConfig.IsInRandomAccountList(bot->GetSession()->GetAccountId()))
    {
        botAI->TellError(PlayerbotTextMgr::instance().GetBotTextOrDefault(
            "bis_altbot_refused_error", "You cannot use bis on alt bots.", {}));
        return false;
    }

    if (sPlayerbotAIConfig.autoGearQualityLimit < 4)
    {
        botAI->TellError(PlayerbotTextMgr::instance().GetBotTextOrDefault(
            "bis_quality_floor_error", "AutoGearQualityLimit must be 4 for BiS.", {}));
        return false;
    }

    if (sRandomPlayerbotMgr.IsSpecPvp(bot->GetGUID().GetCounter(), bot->getClass()))
    {
        botAI->TellError(PlayerbotTextMgr::instance().GetBotTextOrDefault(
            "bis_pvp_refused_error", "bis is PvE only, bot is configured as PvP.", {}));
        return false;
    }

    uint16 ilvl = static_cast<uint16>(sPlayerbotAIConfig.autoGearScoreLimit);

    // Optional explicit ilvl override: `/p autogear bis 55`.
    // Garbage or out-of-range args are hard-rejected: no autogear fallback, no gear change.
    std::string const param = event.getParam();
    if (!param.empty())
    {
        unsigned long parsed = 0;
        size_t pos = 0;
        bool valid = false;
        try
        {
            parsed = std::stoul(param, &pos);
            valid = (parsed > 0 && pos == param.size() && parsed <= 0xFFFFu);
        }
        catch (...)
        {
            valid = false;
        }

        if (!valid)
        {
            std::map<std::string, std::string> phs;
            phs["%param"] = param;
            botAI->TellError(PlayerbotTextMgr::instance().GetBotTextOrDefault(
                "bis_invalid_arg_error",
                "Invalid BiS ilvl argument '%param'. Use a positive integer.", phs));
            return false;
        }
        if (parsed > static_cast<unsigned long>(sPlayerbotAIConfig.autoGearScoreLimit))
        {
            std::map<std::string, std::string> phs;
            phs["%requested"] = std::to_string(parsed);
            phs["%limit"] = std::to_string(sPlayerbotAIConfig.autoGearScoreLimit);
            botAI->TellError(PlayerbotTextMgr::instance().GetBotTextOrDefault(
                "bis_arg_above_limit_error",
                "BiS ilvl %requested exceeds AutoGearScoreLimit %limit, refusing", phs));
            return false;
        }
        ilvl = static_cast<uint16>(parsed);
    }
    uint8 cls = bot->getClass();
    uint8 tab = AiFactory::GetPlayerSpecTab(bot);
    uint8 faction = bot->GetTeamId() == TEAM_ALLIANCE ? 1 : 2;

    // Druid Bear (Feral Tank) shares tab 1 with Cat. Use sentinel tab 10 when tank strategy active.
    constexpr uint8 BIS_TAB_DRUID_BEAR = 10;
    constexpr uint16 BIS_ILVL_FALLBACK_WINDOW = 20;
    uint16 resolvedIlvl = 0;
    std::map<uint8, uint32> bisMap;
    if (cls == CLASS_DRUID && tab == DRUID_TAB_FERAL && PlayerbotAI::IsTank(bot))
        bisMap = sBisListMgr->GetBisForNearest(ilvl, BIS_ILVL_FALLBACK_WINDOW, cls, BIS_TAB_DRUID_BEAR, faction,
                                               &resolvedIlvl);
    if (bisMap.empty())
        bisMap = sBisListMgr->GetBisForNearest(ilvl, BIS_ILVL_FALLBACK_WINDOW, cls, tab, faction, &resolvedIlvl);

    // No rows within fallback window -> full autogear fallback at the effective ilvl.
    if (bisMap.empty())
    {
        std::map<std::string, std::string> phs;
        phs["%ilvl"] = std::to_string(ilvl);
        botAI->TellMaster(PlayerbotTextMgr::instance().GetBotTextOrDefault(
            "bis_no_rows_autogear_msg",
            "No BiS at ilvl %ilvl, using Autogear %ilvl instead", phs));
        return RunAutogearFallback(ilvl);
    }

    if (resolvedIlvl != ilvl)
    {
        std::map<std::string, std::string> phs;
        phs["%requested"] = std::to_string(ilvl);
        phs["%resolved"] = std::to_string(resolvedIlvl);
        botAI->TellMaster(PlayerbotTextMgr::instance().GetBotTextOrDefault(
            "bis_closest_match_msg",
            "No BiS at ilvl %requested, using closest match at ilvl %resolved", phs));
    }

    botAI->TellMaster(PlayerbotTextMgr::instance().GetBotTextOrDefault(
        "bis_applying_msg", "Applying BiS gear", {}));

    // 1. Wipe everything currently equipped so autogear starts from a clean slate.
    //    Old items linger in inventory otherwise and autogear leaves slots empty on bag conflicts.
    for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
    {
        if (slot == EQUIPMENT_SLOT_TABARD || slot == EQUIPMENT_SLOT_BODY)
            continue;
        if (bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            bot->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
    }

    // Wipe equippable items from bags too. Autogear can shove old equipped items into bags
    // (HandleAutoStoreBagItemOpcode), and a unique-equipped duplicate stuck in a bag blocks
    // CanEquipNewItem on subsequent BiS runs. Spare consumables/reagents.
    auto destroyIfEquippable = [&](uint8 bag, uint8 slot)
    {
        Item* item = bot->GetItemByPos(bag, slot);
        if (!item)
            return;
        ItemTemplate const* tmpl = item->GetTemplate();
        if (!tmpl)
            return;
        if (tmpl->Class == ITEM_CLASS_WEAPON || tmpl->Class == ITEM_CLASS_ARMOR)
            bot->DestroyItem(bag, slot, true);
    };
    for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
        destroyIfEquippable(INVENTORY_SLOT_BAG_0, slot);
    for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
    {
        if (Bag* container = bot->GetBagByPos(bag))
            for (uint32 slot = 0; slot < container->GetBagSize(); ++slot)
                destroyIfEquippable(bag, uint8_t(slot));
    }

    // 2. Run full autogear on the empty bot so every slot gets a best-available pick.
    //    Uncovered slots will keep the autogear pick; BiS overwrites the rest below.
    if (sPlayerbotAIConfig.autoGearCommand)
    {
        uint32 fillGs = ilvl == 0
                            ? 0
                            : PlayerbotFactory::CalcMixedGearScore(ilvl, uint32_t(PlayerbotAIConfig::instance().autoGearQualityLimit));
        PlayerbotFactory fillFactory(bot, bot->GetLevel(), uint32_t(PlayerbotAIConfig::instance().autoGearQualityLimit), fillGs);
        fillFactory.InitEquipment(false, PlayerbotAIConfig::instance().twoRoundsGearInit);
    }

    // 2b. Pre-destroy autogear picks that would conflict with any BiS item by entry.
    //     Autogear may have placed the exact item BiS wants into trinket2/finger2 (or vice versa);
    //     unique-equipped enforcement would then make BiS's equip silently drop one copy.
    std::set<uint32> bisEntries;
    for (auto const& kv : bisMap)
        bisEntries.insert(kv.second);
    for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
    {
        if (Item* item = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            if (bisEntries.count(item->GetEntry()))
                bot->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
    }

    // 3. Apply BiS: only touch slots where the bot can actually equip the BiS item.
    //    If item requires reputation, grant the required rank first. If CanUseItem still
    //    fails (class/race/skill/level), keep autogear's pick for that slot.
    for (auto const& kv : bisMap)
    {
        ItemTemplate const* proto = sObjectMgr->GetItemTemplate(kv.second);
        if (!proto)
            continue;

        // Grant required reputation rank if the item gates on it.
        if (proto->RequiredReputationFaction && proto->RequiredReputationRank > 0)
        {
            if (FactionEntry const* fac = sFactionStore.LookupEntry(proto->RequiredReputationFaction))
            {
                ReputationRank requiredRank = static_cast<ReputationRank>(proto->RequiredReputationRank);
                if (bot->GetReputationRank(proto->RequiredReputationFaction) < requiredRank)
                {
                    int32 standing = ReputationMgr::ReputationRankToStanding(
                                         static_cast<ReputationRank>(requiredRank - 1)) + 1;
                    bot->GetReputationMgr().SetReputation(fac, float(standing));
                }
            }
        }

        if (bot->CanUseItem(proto) != EQUIP_ERR_OK)
            continue;

        uint8 slot = kv.first;
        if (bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            bot->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);

        uint16 dest = 0;
        InventoryResult eqResult = bot->CanEquipNewItem(slot, dest, kv.second, false);

        // Paired slots (finger 10<->11, trinket 12<->13): destroy paired slot and retry once
        // when unique-equipped or autogear residue blocks the first attempt.
        if (eqResult != EQUIP_ERR_OK)
        {
            uint8 pairedSlot = 0xFF;
            if (slot == EQUIPMENT_SLOT_FINGER1)        pairedSlot = EQUIPMENT_SLOT_FINGER2;
            else if (slot == EQUIPMENT_SLOT_FINGER2)   pairedSlot = EQUIPMENT_SLOT_FINGER1;
            else if (slot == EQUIPMENT_SLOT_TRINKET1)  pairedSlot = EQUIPMENT_SLOT_TRINKET2;
            else if (slot == EQUIPMENT_SLOT_TRINKET2)  pairedSlot = EQUIPMENT_SLOT_TRINKET1;

            if (pairedSlot != 0xFF)
            {
                if (bot->GetItemByPos(INVENTORY_SLOT_BAG_0, pairedSlot))
                    bot->DestroyItem(INVENTORY_SLOT_BAG_0, pairedSlot, true);
                eqResult = bot->CanEquipNewItem(slot, dest, kv.second, false);
            }
        }

        if (eqResult == EQUIP_ERR_OK)
        {
            bot->EquipNewItem(dest, kv.second, true);
            bot->AutoUnequipOffhandIfNeed();
        }
    }

    PlayerbotFactory factory(bot, bot->GetLevel(), ITEM_QUALITY_EPIC, 0);
    factory.InitAmmo();
    if (bot->GetLevel() >= sPlayerbotAIConfig.minEnchantingBotLevel)
        factory.ApplyEnchantAndGemsNew();

    bot->DurabilityRepairAll(false, 1.0f, false);

    botAI->TellMaster(PlayerbotTextMgr::instance().GetBotTextOrDefault(
        "bis_applied_msg", "BiS applied", {}));
    return true;
}

bool RemoveGlyphAction::Execute(Event /*event*/)
{
    for (uint32 slotIndex = 0; slotIndex < MAX_GLYPH_SLOT_INDEX; ++slotIndex)
    {
        bot->SetGlyph(uint8_t(slotIndex), 0, true);
    }
    bot->SendTalentsInfoData(false);
    return true;
}

bool AutoGearAction::Execute(Event /*event*/)
{
    if (!sPlayerbotAIConfig.autoGearCommand)
    {
        botAI->TellError("autogear command is not allowed, please check the configuration.");
        return false;
    }

    if (!sPlayerbotAIConfig.autoGearCommandAltBots &&
        !sPlayerbotAIConfig.IsInRandomAccountList(bot->GetSession()->GetAccountId()))
    {
        botAI->TellError("You cannot use autogear on alt bots.");
        return false;
    }

    botAI->TellMaster("I'm auto gearing");
    uint32 gs = sPlayerbotAIConfig.autoGearScoreLimit == 0
                    ? 0
                    : PlayerbotFactory::CalcMixedGearScore(uint32_t(PlayerbotAIConfig::instance().autoGearScoreLimit),
                                                           uint32_t(PlayerbotAIConfig::instance().autoGearQualityLimit));
    PlayerbotFactory factory(bot, bot->GetLevel(), uint32_t(PlayerbotAIConfig::instance().autoGearQualityLimit), gs);
    factory.InitEquipment(true);
    factory.InitAmmo();
    if (bot->GetLevel() >= sPlayerbotAIConfig.minEnchantingBotLevel)
    {
        factory.ApplyEnchantAndGemsNew();
    }

    this->bot->DurabilityRepairAll(false, 1.0f, false);
    this->bot->SendTalentsInfoData(false);

    return true;
}

void MaintenanceAction::performRandomBotMaintenance()
{
    const PlayerbotAIConfig& configuration = PlayerbotAIConfig::instance();
    PlayerbotFactory factory{this->bot, this->bot->GetLevel()};

    factory.InitAttunementQuests();
    factory.InitBags(false);
    factory.InitAmmo();
    factory.InitFood();
    factory.InitReagents();
    factory.InitConsumables();
    factory.InitPotions();
    factory.InitTalentsTree(true);
    factory.InitPet();
    factory.InitPetTalents();
    factory.InitSkills();
    factory.InitClassSpells();
    factory.InitAvailableSpells();
    factory.InitReputation();
    factory.InitSpecialSpells();
    factory.InitMounts();
    factory.InitGlyphs(false);
    factory.InitKeyring();

    if (bot->GetLevel() >= configuration.minEnchantingBotLevel)
    {
        factory.ApplyEnchantAndGemsNew();
    }

    this->bot->DurabilityRepairAll(false, 1.0f, false);
    this->bot->SendTalentsInfoData(false);
}

bool MaintenanceAction::Execute(Event)
{
    const PlayerbotAIConfig& configuration = PlayerbotAIConfig::instance();

    if (!configuration.maintenanceCommand)
    {
        this->botAI->TellError("maintenance command is not allowed, please check the configuration.");

        return false;
    }

    this->botAI->TellMaster("I'm maintaining");

    PlayerbotFactory factory{this->bot, this->bot->GetLevel()};

    if (this->botAI->IsAlt())
    {
        this->performAltMaintenance();

        return true;
    }

    this->performRandomBotMaintenance();

    return true;
}
