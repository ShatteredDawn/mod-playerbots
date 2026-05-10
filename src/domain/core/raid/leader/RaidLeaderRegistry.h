#pragma once

#include <variant>

#include "AreaDefines.h"

#include "../leader/BaseRaidLeader.h"
#include "Ai/Raid/ZulGurub/leader/ZulGurubRaidLeader.h"

class RaidLeaderRegistry
{
private:
    using RaidLeaderVariant = std::variant<
        BaseRaidLeader,
        ZulGurubRaidLeader
    >;

    using RaidLeaderRecord = std::pair<uint32_t, RaidLeaderVariant>;

    std::unordered_map<uint32_t, RaidLeaderRecord> entries{};
    mutable std::mutex entriesLock{};

public:
    [[nodiscard]] static RaidLeaderRegistry& GetInstance() noexcept
    {
        static RaidLeaderRegistry instance{};

        return instance;
    }

    void bind(const uint32_t instanceId, const uint32_t mapId)
    {
        const std::scoped_lock<std::mutex> lock{this->entriesLock};

        const std::pair<
            typename std::unordered_map<uint32_t, RaidLeaderRecord>::iterator,
            bool
        > insertedEntry =
            this->entries.try_emplace(
                instanceId,
                RaidLeaderRecord{mapId, RaidLeaderRegistry::CreateLeaderForMapId(mapId)}
            );

        if (insertedEntry.second == true)
        {
            return;
        }

        const uint32_t existingMapId = insertedEntry.first->second.first;

        if (existingMapId != mapId)
        {
            throw std::logic_error("RaidRegistry: instanceId/mapId mismatch");
        }
    }

    void erase(const uint32_t instanceId) noexcept
    {
        const std::scoped_lock<std::mutex> lock{this->entriesLock};

        (void) this->entries.erase(instanceId);
    }

    template<typename TRaidLeader>
    [[nodiscard]] TRaidLeader& get(const uint32_t instanceId)
    {
        const std::scoped_lock<std::mutex> lock{this->entriesLock};

        typename std::unordered_map<uint32_t, RaidLeaderRecord>::iterator it = this->entries.find(instanceId);

        if (it == this->entries.end())
        {
            throw std::out_of_range("RaidRegistry: instanceId not bound");
        }

        RaidLeaderVariant& v = it->second.second;

        TRaidLeader* const Leader = std::get_if<TRaidLeader>(&v);

        if (Leader == nullptr)
        {
            throw std::bad_cast{};
        }

        return *Leader;
    }

    template<typename TRaidLeader>
    [[nodiscard]] TRaidLeader& getOrBind(const uint32_t instanceId, const uint32_t mapId)
    {
        const std::scoped_lock<std::mutex> lock{this->entriesLock};

        typename std::unordered_map<uint32_t, RaidLeaderRecord>::iterator it =
            this->entries.find(instanceId);

        if (it == this->entries.end())
        {
            const std::pair<
                typename std::unordered_map<uint32_t, RaidLeaderRecord>::iterator,
                bool
            > inserted =
                this->entries.try_emplace(
                    instanceId,
                    RaidLeaderRecord{mapId, RaidLeaderRegistry::CreateLeaderForMapId(mapId)}
                );

            it = inserted.first;
        }

        const uint32_t existingMapId = it->second.first;

        if (existingMapId != mapId)
        {
            throw std::logic_error("RaidLeaderRegistry: instanceId/mapId mismatch");
        }

        RaidLeaderVariant& v = it->second.second;

        TRaidLeader* const leader = std::get_if<TRaidLeader>(&v);

        if (leader == nullptr)
        {
            throw std::bad_cast{};
        }

        return *leader;
    }
private:
    [[nodiscard]] static RaidLeaderVariant CreateLeaderForMapId(const uint32_t mapId)
    {
        if (mapId == MAP_ZUL_GURUB)
        {
            return RaidLeaderVariant{std::in_place_type<ZulGurubRaidLeader>};
        }

        return RaidLeaderVariant{std::in_place_type<BaseRaidLeader>};
    }

    RaidLeaderRegistry() = default;
    ~RaidLeaderRegistry() = default;

    RaidLeaderRegistry(const RaidLeaderRegistry&) = delete;
    RaidLeaderRegistry& operator=(const RaidLeaderRegistry&) = delete;

    RaidLeaderRegistry(RaidLeaderRegistry&&) = delete;
    RaidLeaderRegistry& operator=(RaidLeaderRegistry&&) = delete;
};
