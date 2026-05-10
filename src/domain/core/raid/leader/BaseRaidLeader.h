#pragma once

#include "./memory/RaidMemory.h"
#include "Group.h"
#include "domain/core/raid/assistant/tank-assistant/RaidTankAssistant.h"

class BaseRaidLeader
{
protected:
    Group* group{nullptr};

    RaidMemory memory{};

    RaidTankAssistant raidTankAssistant{};

public:
    BaseRaidLeader() = default;
    virtual ~BaseRaidLeader() = default;

    BaseRaidLeader(const BaseRaidLeader& other) = default;
    BaseRaidLeader(BaseRaidLeader&& other) noexcept = default;

    BaseRaidLeader& operator=(const BaseRaidLeader& other) = default;
    BaseRaidLeader& operator=(BaseRaidLeader&& other) noexcept = default;

    void setGroup(Group* group) noexcept
    {
        this->group = group;
    }

    [[nodiscard]] Group* getGroup() const noexcept
    {
        return this->group;
    }

    [[nodiscard]] const RaidMemory& getMemory() const noexcept
    {
        return this->memory;
    }

    [[nodiscard]] RaidTankAssistant& getRaidTankAssistant() noexcept
    {
        return this->raidTankAssistant;
    }
};
