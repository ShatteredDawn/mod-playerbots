#pragma once

#include <ctime>
#include "Unit.h"

class SonOfHakkarMemory final
{
private:
    Unit* sonOfHakkarDesignatedPuller = nullptr;
    bool sonOfHakkarKilledForCurrentBloodSiphon = false;
    Creature* leftSonOfHakkar = nullptr;
    Creature* rightSonOfHakkar = nullptr;
    Creature* activeSonOfHakkar = nullptr;

public:
    SonOfHakkarMemory() = default;
    ~SonOfHakkarMemory() = default;

    [[nodiscard]] Unit* getSonOfHakkarDesignatedPuller() const noexcept
    {
        return this->sonOfHakkarDesignatedPuller;
    }

    void setSonOfHakkarDesignatedPuller(Unit* unit) noexcept
    {
        this->sonOfHakkarDesignatedPuller = unit;
    }

    [[nodiscard]] Creature* getActiveSonOfHakkar() const noexcept
    {
        return this->activeSonOfHakkar;
    }

    void setActiveSonOfHakkar(Creature* unit) noexcept
    {
        this->activeSonOfHakkar = unit;
    }

    [[nodiscard]] Creature* getLeftSonOfHakkar() const noexcept
    {
        return this->leftSonOfHakkar;
    }

    void setLeftSonOfHakkar(Creature* unit) noexcept
    {
        this->leftSonOfHakkar = unit;
    }

    [[nodiscard]] Creature* getRightSonOfHakkar() const noexcept
    {
        return this->rightSonOfHakkar;
    }

    void setRightSonOfHakkar(Creature* unit) noexcept
    {
        this->rightSonOfHakkar = unit;
    }

    [[nodiscard]] bool getSonOfHakkarKilledForCurrentBloodSiphon() const noexcept
    {
        return this->sonOfHakkarKilledForCurrentBloodSiphon;
    }

    void setSonOfHakkarKilledForCurrentBloodSiphon(bool value) noexcept
    {
        this->sonOfHakkarKilledForCurrentBloodSiphon = value;
    }
};