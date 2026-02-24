#pragma once

#include <chrono>

class Stopwatch final
{
private:
    bool hasStarted;
    std::chrono::steady_clock::time_point startPoint;
public:
    Stopwatch() noexcept : hasStarted(false), startPoint() {}
    ~Stopwatch() = default;

    void start() noexcept
    {
        this->startPoint = std::chrono::steady_clock::now();
        this->hasStarted = true;
    }

    void reset() noexcept
    {
        this->startPoint = std::chrono::steady_clock::now();
        this->hasStarted = false;
    }

    [[nodiscard]] double getElapsedSeconds() const noexcept
    {
        if (this->hasStarted == false)
        {
            return 0.0f;
        }

        const std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - this->startPoint;

        return elapsed.count();
    }
};
