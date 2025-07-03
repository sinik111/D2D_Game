#pragma once

#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

class MyTimeSystem
{
private:
    TimePoint m_previousTime;
    TimePoint m_currentTime;

    float m_deltaTime = 0.0f;
    float m_timeScale = 1.0f;

private:
    MyTimeSystem();
    ~MyTimeSystem() = default;

public:
    static MyTimeSystem& Get();

public:
    void Update();
    float DeltaTime();
    void SetTimeScale(float timeScale);
};