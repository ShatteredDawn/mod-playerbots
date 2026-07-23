#include "NaxxActions.h"

#include "ObjectGuid.h"
#include "PlayerbotAIConfig.h"
#include "UnitAI.h"

namespace
{
constexpr uint32 SPELL_UNDERSTUDY_TAUNT = 29060;
constexpr uint32 SPELL_BONE_BARRIER     = 29061;
constexpr uint32 SPELL_BLOOD_STRIKE     = 61696;
}

// @TODO: This needs a complete rewrite.
bool RazuviousUseObedienceCrystalAction::Execute(Event /*event*/)
{
    if (!this->helper.UpdateBossAI())
    {
        return false;
    }

    Unit* const charmedUnit = this->bot->GetCharm();

    if (charmedUnit != nullptr)
    {
        Value<Unit*>* targetValue = this->context->GetValue<Unit*>("find target", "instructor razuvious");

        if (targetValue == nullptr)
        {
            return false;
        }

        Unit* const target = targetValue->Get();

        if (target == nullptr)
        {
            return false;
        }

        MotionMaster* const charmedUnitMotionMaster = charmedUnit->GetMotionMaster();

        if (charmedUnitMotionMaster == nullptr)
        {
            return false;
        }

        if (charmedUnitMotionMaster->GetMotionSlotType(MOTION_SLOT_ACTIVE) == NULL_MOTION_TYPE)
        {
            charmedUnitMotionMaster->Clear();
            charmedUnitMotionMaster->MoveChase(target);
            charmedUnit->GetAI()->AttackStart(target);
        }

        const Aura* forceObedience = this->botAI->GetAura("force obedience", charmedUnit);
        int32_t duration_time = 90000;

        if (forceObedience == nullptr)
        {
            forceObedience = this->botAI->GetAura("mind control", charmedUnit);
            duration_time = 60000;
        }

        if (forceObedience == nullptr)
        {
            return false;
        }

        if (charmedUnit->GetDistance(target) <= 0.51f)
        {
            // taunt
            bool tauntUseful = true;

            if (forceObedience->GetDuration() <= (duration_time - 5000))
            {
                Unit* victim = target->GetVictim();
                if (victim && victim->HasAura(SPELL_BONE_BARRIER))
                {
                    tauntUseful = false;
                }

                if (forceObedience->GetDuration() <= 3000)
                {
                    tauntUseful = false;
                }
            }

            if (forceObedience->GetDuration() >= (duration_time - 500))
            {
                tauntUseful = false;
            }

            if (tauntUseful && !charmedUnit->HasSpellCooldown(29060))
            {
                // shield
                if (!charmedUnit->HasSpellCooldown(SPELL_BONE_BARRIER))
                {
                    charmedUnit->CastSpell(charmedUnit, SPELL_BONE_BARRIER, true);
                    charmedUnit->AddSpellCooldown(SPELL_BONE_BARRIER, 0, 30 * 1000);
                }

                charmedUnit->CastSpell(target, SPELL_UNDERSTUDY_TAUNT, true);
                charmedUnit->AddSpellCooldown(SPELL_UNDERSTUDY_TAUNT, 0, 20 * 1000);
            }

            // strike
            if (!charmedUnit->HasSpellCooldown(SPELL_BLOOD_STRIKE))
            {
                charmedUnit->CastSpell(target, SPELL_BLOOD_STRIKE, true);
                charmedUnit->AddSpellCooldown(SPELL_BLOOD_STRIKE, 0, 4 * 1000);
            }
        }

        return false;
    }

    const Difficulty diff = this->bot->GetRaidDifficulty();

    if (diff == RAID_DIFFICULTY_10MAN_NORMAL)
    {
        Value<GuidVector>* const npcsValue = this->context->GetValue<GuidVector>("nearest npcs");

        if (npcsValue == nullptr)
        {
            return false;
        }

        const GuidVector npcs = npcsValue->Get();

        for (GuidVector::const_iterator i = npcs.begin(); i != npcs.end(); i++)
        {
            Creature* const unit = this->botAI->GetCreature(*i);

            if (unit == nullptr)
            {
                continue;
            }

            if (this->botAI->IsMainTank(bot) && unit->GetSpawnId() != 128352)
            {
                continue;
            }

            if (!this->botAI->IsMainTank(bot) && unit->GetSpawnId() != 128353)
            {
                continue;
            }

            if (this->MoveTo(unit, 0.0f, MovementPriority::MOVEMENT_COMBAT))
            {
                return true;
            }

            Creature* const creature = this->bot->GetNPCIfCanInteractWith(*i, UNIT_NPC_FLAG_SPELLCLICK);

            if (!creature)
            {
                continue;
            }

            creature->HandleSpellClick(bot);

            return true;
        }

        return false;
    }

    Value<GuidVector>* const attackersValue = this->context->GetValue<GuidVector>("attackers");

    if (attackersValue == nullptr)
    {
        return false;
    }

    const GuidVector attackers = attackersValue->Get();
    Unit* target = nullptr;

    for (GuidVector::const_iterator i = attackers.begin(); i != attackers.end(); ++i)
    {
        Unit* const unit = this->botAI->GetUnit(*i);

        if (unit == nullptr)
        {
            continue;
        }

        if (!this->botAI->EqualLowercaseName(unit->GetName(), "death knight understudy"))
        {
            continue;
        }

        target = unit;
    }

    if (target == nullptr)
    {
        return false;
    }

    const float spellDistance = PlayerbotAIConfig::instance().spellDistance;

    if (this->bot->GetDistance2d(target) > spellDistance)
    {
        return this->MoveNear(target, spellDistance, MovementPriority::MOVEMENT_COMBAT);
    }

    return this->botAI->CastSpell("mind control", target);
}

bool RazuviousTargetAction::Execute(Event /*event*/)
{
    if (!this->helper.UpdateBossAI())
    {
        return false;
    }

    Value<Unit*>* const razuviousValue = this->context->GetValue<Unit*>("find target", "inspector razuvious");
    Value<Unit*>* const understudyValue = this->context->GetValue<Unit*>("find target", "death knight understudy");

    if (razuviousValue == nullptr || understudyValue == nullptr)
    {
        return false;
    }

    Unit* const razuvious = razuviousValue->Get();
    Unit* const understudy = understudyValue->Get();
    Unit* target = razuvious;

    if (this->botAI->IsTank(bot))
    {
        target = understudy;
    }

    Value<Unit*>* const currentTargetValue = this->context->GetValue<Unit*>("current target");

    if (currentTargetValue == nullptr)
    {
        return false;
    }

    if (currentTargetValue->Get() == target)
    {
        return false;
    }

    return this->Attack(target);
}
