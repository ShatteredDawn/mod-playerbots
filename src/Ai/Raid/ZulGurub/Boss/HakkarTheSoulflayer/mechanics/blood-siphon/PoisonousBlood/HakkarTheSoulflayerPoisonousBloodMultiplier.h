#pragma once

#include "AiObjectContext.h"
#include "HakkarTheSoulflayer/definition/enum/PoisonousCloudEnum.h"
#include "MovementActions.h"
#include "Multiplier.h"
#include "PlayerbotAI.h"
#include "Value.h"

#include "DruidActions.h"
#include "PaladinActions.h"
#include "ShamanActions.h"
#include "GenericSpellActions.h"
#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerPoisonousBloodMultiplier : public Multiplier
{
public:
    HakkarTheSoulflayerPoisonousBloodMultiplier(PlayerbotAI* botAI) : Multiplier(botAI, "hakkar the soulflayer poisonous blood") {}

    float GetValue(Action& action) override
    {
        if (this->bot == nullptr)
        {
            return 0.0f;
        }

        const uint8_t botClassInfo = this->bot->getClass();

        static constexpr std::array<uint8_t, 3> dispellingClasses = { CLASS_PALADIN, CLASS_SHAMAN, CLASS_DRUID };

        if (std::find(dispellingClasses.begin(), dispellingClasses.end(), botClassInfo) == dispellingClasses.end())
        {
            return 1.0f;
        }

        const Unit* const actionTarget = action.GetTarget();

        if (actionTarget == nullptr)
        {
            return 1.0f;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();

        if (hakkarTheSoulflayerAssistant.isInCombatWithHakkar(*this->bot) == false)
        {
            return 1.0f;
        }

        const bool hasPoisonousBlood = actionTarget->HasAura(uint32_t(PoisonousCloudEnum::AURA_POISONOUS_BLOOD));

        if (hasPoisonousBlood == false)
        {
            return 1.0f;
        }

        if (HakkarTheSoulflayerPoisonousBloodMultiplier::IsDruidDispellingAction(action))
        {
            return 0.0f;
        }

        if (HakkarTheSoulflayerPoisonousBloodMultiplier::IsPaladinDispellingAction(action))
        {
            return 0.0f;
        }

        if (HakkarTheSoulflayerPoisonousBloodMultiplier::IsShamanDispellingAction(action))
        {
            return 0.0f;
        }

        return 1.0f;
    }

private:

    [[nodiscard]] static bool IsDruidDispellingAction(const Action& action) noexcept
    {
        const CastCurePoisonAction* const castCurePoisonAction = dynamic_cast<const CastCurePoisonAction*>(&action);

        if (castCurePoisonAction != nullptr)
        {
            return true;
        }

        const CastCurePoisonOnPartyAction* const castCurePoisonOnPartyAction = dynamic_cast<const CastCurePoisonOnPartyAction*>(&action);

        if (castCurePoisonOnPartyAction != nullptr)
        {
            return true;
        }

        const CastAbolishPoisonAction* const castAbolishPoisonAction = dynamic_cast<const CastAbolishPoisonAction*>(&action);

        if (castAbolishPoisonAction != nullptr)
        {
            return true;
        }

        const CastAbolishPoisonOnPartyAction* const castAbolishPoisonOnPartyAction = dynamic_cast<const CastAbolishPoisonOnPartyAction*>(&action);

        if (castAbolishPoisonOnPartyAction != nullptr)
        {
            return true;
        }

        return false;
    }

    [[nodiscard]] static bool IsPaladinDispellingAction(const Action& action) noexcept
    {
        const CastCleansePoisonAction* const castCleansePoisonAction = dynamic_cast<const CastCleansePoisonAction*>(&action);

        if (castCleansePoisonAction != nullptr)
        {
            return true;
        }

        const CastCleansePoisonOnPartyAction* const castCleansePoisonOnPartyAction = dynamic_cast<const CastCleansePoisonOnPartyAction*>(&action);

        if (castCleansePoisonOnPartyAction != nullptr)
        {
            return true;
        }

        const CastCleanseMagicAction* const castCleanseMagicAction = dynamic_cast<const CastCleanseMagicAction*>(&action);

        if (castCleanseMagicAction != nullptr)
        {
            return true;
        }

        const CastCleanseMagicOnPartyAction* const castCleanseMagicOnPartyAction = dynamic_cast<const CastCleanseMagicOnPartyAction*>(&action);

        if (castCleanseMagicOnPartyAction != nullptr)
        {
            return true;
        }

        const CastCleanseDiseaseAction* const castCleanseDiseaseAction = dynamic_cast<const CastCleanseDiseaseAction*>(&action);

        if (castCleanseDiseaseAction != nullptr)
        {
            return true;
        }

        const CastCleanseDiseaseOnPartyAction* const castCleanseDiseaseOnPartyAction = dynamic_cast<const CastCleanseDiseaseOnPartyAction*>(&action);

        if (castCleanseDiseaseOnPartyAction != nullptr)
        {
            return true;
        }

        const CastPurifyPoisonAction* const castPurifyPoisonAction = dynamic_cast<const CastPurifyPoisonAction*>(&action);

        if (castPurifyPoisonAction != nullptr)
        {
            return true;
        }

        const CastPurifyPoisonOnPartyAction* const castPurifyPoisonOnPartyAction = dynamic_cast<const CastPurifyPoisonOnPartyAction*>(&action);

        if (castPurifyPoisonOnPartyAction != nullptr)
        {
            return true;
        }

        const CastPurifyDiseaseAction* const castPurifyDiseaseAction = dynamic_cast<const CastPurifyDiseaseAction*>(&action);

        if (castPurifyDiseaseAction != nullptr)
        {
            return true;
        }

        const CastPurifyDiseaseOnPartyAction* const castPurifyDiseaseOnPartyAction = dynamic_cast<const CastPurifyDiseaseOnPartyAction*>(&action);

        if (castPurifyDiseaseOnPartyAction != nullptr)
        {
            return true;
        }

        return false;
    }

    [[nodiscard]] static bool IsShamanDispellingAction(const Action& action) noexcept
    {
        const CastCleanseSpiritAction* const castCleanseSpiritAction = dynamic_cast<const CastCleanseSpiritAction*>(&action);

        if (castCleanseSpiritAction != nullptr)
        {
            return true;
        }

        const CastCleanseSpiritPoisonOnPartyAction* const castCleanseSpiritPoisonOnPartyAction = dynamic_cast<const CastCleanseSpiritPoisonOnPartyAction*>(&action);

        if (castCleanseSpiritPoisonOnPartyAction != nullptr)
        {
            return true;
        }

        const CastCureToxinsActionSham* const castCurePoisonActionSham = dynamic_cast<const CastCureToxinsActionSham*>(&action);

        if (castCurePoisonActionSham != nullptr)
        {
            return true;
        }

        const CastCureToxinsPoisonOnPartyActionSham* const castCurePoisonOnPartyActionSham = dynamic_cast<const CastCureToxinsPoisonOnPartyActionSham*>(&action);

        if (castCurePoisonOnPartyActionSham != nullptr)
        {
            return true;
        }

        const CastCleanseSpiritCurseOnPartyAction* const castCleanseSpiritCurseOnPartyAction = dynamic_cast<const CastCleanseSpiritCurseOnPartyAction*>(&action);

        if (castCleanseSpiritCurseOnPartyAction != nullptr)
        {
            return true;
        }

        const CastCleanseSpiritDiseaseOnPartyAction* const castCleanseSpiritDiseaseOnPartyAction = dynamic_cast<const CastCleanseSpiritDiseaseOnPartyAction*>(&action);

        if (castCleanseSpiritDiseaseOnPartyAction != nullptr)
        {
            return true;
        }

        const CastCleansingTotemAction* const castCleansingTotemAction = dynamic_cast<const CastCleansingTotemAction*>(&action);

        if (castCleansingTotemAction != nullptr)
        {
            return true;
        }

        return false;
    }
};
