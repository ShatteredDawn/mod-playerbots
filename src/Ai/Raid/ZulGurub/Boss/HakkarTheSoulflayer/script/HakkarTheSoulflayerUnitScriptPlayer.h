#pragma once

#include "Group.h"
#include "Map.h"
#include "HakkarTheSoulflayer/definition/enum/HakkarTheSoulflayerEnum.h"
#include "PlayerbotMgr.h"
#include "Unit.h"
#include "Player.h"
#include "UnitScript.h"
#include "PlayerbotAI.h"


class HakkarTheSoulflayerUnitScriptPlayer : public UnitScript
{
public:
    HakkarTheSoulflayerUnitScriptPlayer() : UnitScript("HakkarTheSoulflayerUnitScriptPlayer") {}

    void OnAuraApply(Unit* unit, Aura* aura) noexcept override
    {
        if (unit == nullptr || aura == nullptr)
        {
            return;
        }

        if (aura->GetSpellInfo()->Id != uint32_t(HakkarTheSoulflayerEnum::SPELL_CORRUPTED_BLOOD))
        {
            return;
        }

        Player* const player = dynamic_cast<Player*>(unit);

        if (player == nullptr)
        {
            return;
        }

        PlayerbotAI* const botAI = PlayerbotsMgr::instance().GetPlayerbotAI(player);

        botAI->SayToRaid("I have been infected with the soulflayer's corrupted blood!");
    }
};
