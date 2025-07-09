#include "pch.h"
#include "MyTime.h"

#include "MyTimeSystem.h"

float MyTime::GetDeltaTime()
{
    return MyTimeSystem::Get().GetDeltaTime();
}

TimePoint MyTime::GetTimestamp()
{
    return Clock::now();
}

TimePoint MyTime::GetAccumulatedTime(const TimePoint& timePoint, int seconds)
{
    return timePoint + std::chrono::seconds(seconds);
}

float MyTime::GetElapsedSeconds(const TimePoint& timePoint)
{
    return Duration(Clock::now() - timePoint).count();
}

void MyTime::SetTimeScale(float timeScale)
{
    return MyTimeSystem::Get().SetTimeScale(timeScale);
}
