#pragma once

#include "GenericSpellActions.h"
#include "GlobalPlayerInspector.h"

class BlinkAction : public CastSpellAction
{
public:
    BlinkAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "blink") {}

    bool isPossible() override
    {
        const GlobalPlayerInspector playerInspector{this->bot->GetGUID().GetRawValue()};

    }

    bool Execute(Event event) override;
};

