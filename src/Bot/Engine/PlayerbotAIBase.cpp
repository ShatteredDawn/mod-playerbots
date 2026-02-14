/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "PlayerbotAIBase.h"
#include "PerfMonitor.h"

PlayerbotAIBase::PlayerbotAIBase(bool isBotAI) : nextAICheckDelay(0), _isBotAI(isBotAI) {}

void PlayerbotAIBase::UpdateAI(uint32_t elapsed)
{
    if (this->totalPmo != nullptr)
    {
        this->totalPmo->finish();
    }

    this->totalPmo = PerfMonitor::instance().start(PERF_MON_TOTAL, "PlayerbotAIBase::FullTick");

    if (this->nextAICheckDelay > elapsed)
    {
        this->nextAICheckDelay -= elapsed;
    }
    else
    {
        this->nextAICheckDelay = 0;
    }

    if (!this->CanUpdateAI())
    {
        return;
    }

    this->UpdateAIInternal();
    this->YieldThread(nullptr);
}

void PlayerbotAIBase::SetNextCheckDelay(const uint32_t delay)
{
    this->nextAICheckDelay = delay;
}

void PlayerbotAIBase::IncreaseNextCheckDelay(const uint32_t delay)
{
    this->nextAICheckDelay += delay;
}

bool PlayerbotAIBase::CanUpdateAI()
{
    return this->nextAICheckDelay == 0;
}

// @TODO: This is extremely poorly named. This is NOT yielding the thread,
// but rather setting the next check delay to the specified value if it is greater than the current value.
void PlayerbotAIBase::YieldThread(const Player* bot, const uint32_t delay)
{
    if (this->nextAICheckDelay < delay)
    {
        // Adding a deterministic per-bot slight offset (0–200 ms) to stagger updates and prevent cpu spikes.
        uint32 offset = bot ? (bot->GetGUID().GetCounter() % 201) : 0;
    {
        this->nextAICheckDelay = delay + offset;
    }
    }
}

bool PlayerbotAIBase::IsActive()
{
    return this->nextAICheckDelay < PlayerbotAIConfig::instance().maxWaitForMove;
}

bool PlayerbotAIBase::IsBotAI() const
{
    return this->_isBotAI;
}
