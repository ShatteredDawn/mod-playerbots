#pragma once

#include "AreaDefines.h"
#include "InstanceMapScript.h"

#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class ZulGurubInstanceScript : public InstanceMapScript
{
public:
    ZulGurubInstanceScript() : InstanceMapScript("ZulGurubInstanceScript", MAP_ZUL_GURUB) {}

    void OnCreate(InstanceMap* map) override
    {
        if (map == nullptr)
        {
            return;
        }

        RaidLeaderRegistry& raidLeaderRegistry = RaidLeaderRegistry::GetInstance();

        raidLeaderRegistry.bind(map->GetInstanceId(), map->GetId());
    }

    void OnDestroy(InstanceMap* map) override
    {
        if (map == nullptr)
        {
            return;
        }

        RaidLeaderRegistry& raidLeaderRegistry = RaidLeaderRegistry::GetInstance();

        raidLeaderRegistry.erase(map->GetInstanceId());
    }
};
