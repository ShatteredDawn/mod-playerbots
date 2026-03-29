#pragma once

#include "Formations.h"
#include "MovementActions.h"
#include "Transport.h"

class PlayerbotAI;

struct BoardPointTransportResultStruct
{
    float x;
    float y;
    float z;
    bool found;
};

enum class HandleMovingTransportsResultEnum : uint8_t
{
    FALSE = 0,
    TRUE = 1,
    NONE = 2
};

class FollowAction : public MovementAction
{
public:
    FollowAction(PlayerbotAI* botAI, std::string const name = "follow") : MovementAction(botAI, name) {}

    bool Execute(Event event) override;
    bool isUseful() override;

protected:

    [[nodiscard]] bool CanDeadFollow(const Unit* const target) const noexcept;
    [[nodiscard]] Transport* getTransportForPosTolerant(Map& map, WorldObject& ref, uint32_t phaseMask, float x, float y, float z);
    [[nodiscard]] BoardPointTransportResultStruct findBoardingPointOnTransport(
        Map& map, Transport& expectedTransport, WorldObject& ref,
        float masterX, float masterY, float masterZ,
        float botX, float botY, float botZ
    );
    [[nodiscard]] HandleMovingTransportsResultEnum handleMovingTransports(Player& master);
    [[nodiscard]] float getDistanceToTarget(Formation& formation, const std::string& target);
};

class FleeToGroupLeaderAction : public FollowAction
{
public:
    FleeToGroupLeaderAction(PlayerbotAI* botAI) : FollowAction(botAI, "flee to group leader") {}

    bool Execute(Event event) override;
    bool isUseful() override;
};
