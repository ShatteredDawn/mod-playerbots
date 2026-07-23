/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_FERALDRUIDSTRATEGY_H
#define _PLAYERBOT_FERALDRUIDSTRATEGY_H

#include "DruidActions.h"
#include "DruidShapeshiftActions.h"
#include "GenericActions.h"
#include "GenericDruidStrategy.h"

constexpr uint32 SPELL_CAT_FORM  = 768;
constexpr uint32 AURA_THICK_HIDE = 16931;

class PlayerbotAI;

class ShapeshiftDruidStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    ShapeshiftDruidStrategyActionNodeFactory()
    {
        creators["rejuvenation"] = &rejuvenation;
        creators["regrowth"] = &regrowth;
        creators["healing touch"] = &healing_touch;
        creators["rejuvenation on party"] = &rejuvenation_on_party;
        creators["regrowth on party"] = &regrowth_on_party;
        creators["healing touch on party"] = &healing_touch_on_party;
    }

private:
    static ActionNode* regrowth([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ { CreateNextAction<CastHealingTouchAction>(1.0f) },
            /*C*/ { CreateNextAction<MeleeAction>(10.0f) }
        );
    }

    static ActionNode* rejuvenation([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* healing_touch([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* regrowth_on_party([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ { CreateNextAction<CastHealingTouchOnPartyAction>(1.0f) },
            /*C*/ { CreateNextAction<MeleeAction>(10.0f) }
        );
    }

    static ActionNode* rejuvenation_on_party([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* healing_touch_on_party([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ {},
            /*C*/ {}
        );
    }
};

class FeralDruidStrategy : public GenericDruidStrategy
{
protected:
    FeralDruidStrategy(PlayerbotAI* botAI);

public:
    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    uint32 GetType() const override { return STRATEGY_TYPE_COMBAT | STRATEGY_TYPE_MELEE; }
};

// Optional strategy — enabled by default for cat and bear.
// Registers the "enemy out of melee" → Feral Charge trigger, spec-gated at
// init time so cats get Feral Charge (Cat) and bears get Feral Charge (Bear).
// Disable with: co -feral charge
// Re-enable with: co +feral charge
class FeralChargeDruidStrategy : public CombatStrategy
{
public:
    FeralChargeDruidStrategy(PlayerbotAI* botAI) : CombatStrategy(botAI) {}

    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "feral charge"; }
};

#endif
