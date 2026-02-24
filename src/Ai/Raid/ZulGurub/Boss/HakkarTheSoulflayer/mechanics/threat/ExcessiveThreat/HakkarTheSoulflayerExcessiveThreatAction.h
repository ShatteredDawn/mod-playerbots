#pragma once

#include "HunterActions.h"
#include "MageActions.h"
#include "PaladinActions.h"
#include "PriestActions.h"
#include "RogueActions.h"
#include "Spell.h"
#include "AiObjectContext.h"
#include "AttackAction.h"
#include "PlayerbotAI.h"

#include "WarlockActions.h"

class HakkarTheSoulflayerExcessiveThreatAction : public AttackAction
{
public:
    HakkarTheSoulflayerExcessiveThreatAction(
        PlayerbotAI* botAI,
        const std::string name = "hakkar the soulflayer excessive threat"
    ) : AttackAction(botAI, name) {}

    bool Execute(Event) override
    {
        if (this->bot == nullptr)
        {
            return false;
        }

        if (this->botAI == nullptr)
        {
            return false;
        }

        // const uint8_t botClassInfo = this->bot->getClass();

        // if (botClassInfo == CLASS_PALADIN)
        // {
        //     const SpellInfo* const handOfSalvationSpellInfo = SpellMgr::instance()->GetSpellInfo(1038);

        //     this->bot->CastSpell(this->bot, handOfSalvationSpellInfo);
        // }

        // if (botClassInfo == CLASS_HUNTER)
        // {
        //     const SpellInfo* const feignDeathSpellInfo = SpellMgr::instance()->GetSpellInfo(5384);

        //     this->bot->CastSpell(this->bot, feignDeathSpellInfo);
        // }

        // if (botClassInfo == CLASS_ROGUE)
        // {


        //     return this->botAI->DoSpecificAction(CreateNextAction<CastVanishAction>(0.0f).factory)
        //         || this->botAI->DoSpecificAction(CreateNextAction<CastFeintAction>(0.0f).factory);
        // }

        // if (botClassInfo == CLASS_PRIEST)
        // {
        //     return this->botAI->DoSpecificAction(CreateNextAction<CastFadeAction>(0.0f).factory);
        // }

        this->botAI->SetNextCheckDelay(500);

        return true;
    }
};
