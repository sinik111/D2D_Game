#include "pch.h"
#include "MyTime.h"

#include "MyTimeSystem.h"

float MyTime::DeltaTime()
{
    return MyTimeSystem::Get().DeltaTime();
}

float MyTime::FixedDeltaTime()
{
    return MyTimeSystem::Get().FixedDeltaTime();
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
