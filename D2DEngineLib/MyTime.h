#pragma once

#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

class MyTime
{
private:
    MyTime();
    ~MyTime() = default;

public:
    static float DeltaTime();
    static TimePoint GetTimestamp();
    static TimePoint GetAccumulatedTime(const TimePoint& timePoint, int seconds);
    static float GetElapsedSeconds(const TimePoint& timePoint);
};