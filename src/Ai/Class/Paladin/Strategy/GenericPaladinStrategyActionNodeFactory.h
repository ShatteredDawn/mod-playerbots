/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_GENERICPALADINSTRATEGYACTIONNODEFACTORY_H
#define _PLAYERBOT_GENERICPALADINSTRATEGYACTIONNODEFACTORY_H

#include "Action.h"
#include "ActionNode.h"
#include "CreateNextAction.h"
#include "NamedObjectContext.h"
#include "PaladinActions.h"

class PlayerbotAI;

class GenericPaladinStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    GenericPaladinStrategyActionNodeFactory()
    {
        creators["cleanse poison"] = &cleanse_poison;
        creators["cleanse disease"] = &cleanse_disease;
        creators["cleanse poison on party"] = &cleanse_poison_on_party;
        creators["cleanse disease on party"] = &cleanse_disease_on_party;
        creators["seal of wisdom"] = &seal_of_wisdom;
        creators["seal of justice"] = &seal_of_justice;
        creators["hand of reckoning"] = &hand_of_reckoning;
        creators["judgement of wisdom"] = &judgement_of_wisdom;
        creators["divine shield"] = &divine_shield;
        creators["flash of light"] = &flash_of_light;
        creators["flash of light on party"] = &flash_of_light_on_party;
        creators["retribution aura"] = &retribution_aura;
        creators["taunt spell"] = &hand_of_reckoning;
        creators["righteous defense"] = &righteous_defense;
        creators["avenger's shield"] = &avengers_shield;
        creators["divine sacrifice"] = &divine_sacrifice;
    }

private:
    static ActionNode* retribution_aura(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastDevotionAuraAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* cleanse_poison(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastPurifyPoisonAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* cleanse_disease(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastPurifyDiseaseAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* cleanse_poison_on_party(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastPurifyPoisonOnPartyAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* cleanse_disease_on_party(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastPurifyDiseaseOnPartyAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* seal_of_wisdom(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastSealOfCorruptionAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* seal_of_justice(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastSealOfRighteousnessAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* hand_of_reckoning(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastRighteousDefenseAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* righteous_defense(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastAvengersShieldAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* avengers_shield(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastJudgementOfWisdomAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* divine_sacrifice(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ { CreateNextAction<CastCancelDivineSacrificeAction>(1.0f) }
        );
    }
    static ActionNode* judgement_of_wisdom(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastJudgementOfLightAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* divine_shield(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastDivineProtectionAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* flash_of_light(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastHolyLightAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* flash_of_light_on_party(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastHolyLightOnPartyAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* seal_of_command(PlayerbotAI* /* ai */)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastSealOfRighteousnessAction>(1.0f) },
            /*C*/ {}
        );
    }
};

#endif
