#pragma once

#include "MageActions.h"
#include "Spell.h"
#include "AiObjectContext.h"
#include "AttackAction.h"
#include "PlayerbotAI.h"

#include "WarlockActions.h"

#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerCauseInsanityAction : public AttackAction
{
public:
    HakkarTheSoulflayerCauseInsanityAction(
        PlayerbotAI* botAI,
        const std::string name = "hakkar the soulflayer cause insanity"
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

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, this->bot->GetMapId());
        const HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();

        Player* const insaneAlly = hakkarTheSoulflayerAssistant.findInsaneAlly(*this->bot);

        if (insaneAlly == nullptr)
        {
            return false;
        }

        // Value<Unit*>* const ccTargetValue = this->context->GetValue<Unit*>("cc target");

        // if (ccTargetValue == nullptr)
        // {
        //     return false;
        // }

        // ccTargetValue->Set(insaneAlly);


        const uint8_t botClassInfo = this->bot->getClass();

        // if (botClassInfo == CLASS_MAGE)
        // {
        //     this->bot->SetTarget(insaneAlly->GetGUID());

        //     this->bot->CastSpell(insaneAlly, )

        //     return this->botAI->DoSpecificAction(CreateNextAction<CastPolymorphAction>(0.0f).factory);
        // }

        if (botClassInfo == CLASS_WARLOCK)
        {
            const SpellInfo* const fearSpellInfo = SpellMgr::instance()->GetSpellInfo(5782);

            this->bot->CastSpell(insaneAlly, fearSpellInfo);
        }

        return true;
    }
};
