#pragma once

#include <ctime>
#include "utility/StopWatch.h"

class HakkarTheSoulflayerMemory final
{
private:
    Stopwatch bloodSiphonWatch{};

public:
    HakkarTheSoulflayerMemory() = default;
    ~HakkarTheSoulflayerMemory() = default;

    [[nodiscard]] Stopwatch& getBloodSiphonWatch() noexcept
    {
        return this->bloodSiphonWatch;
    }

    [[nodiscard]] const Stopwatch& getBloodSiphonWatch() const noexcept
    {
        return this->bloodSiphonWatch;
    }
};