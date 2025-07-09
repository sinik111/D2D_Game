#pragma once

#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

class MyTimeSystem
{
private:
    TimePoint m_previousTime = Clock::now();
    TimePoint m_currentTime = Clock::now();

    float m_deltaTime = 0.0f;
    float m_fixedDeltaTime = 0.02f;
    float m_timeScale = 1.0f;

private:
    MyTimeSystem() = default;
    MyTimeSystem(const MyTimeSystem&) = delete;
    MyTimeSystem operator=(const MyTimeSystem&) = delete;
    MyTimeSystem(MyTimeSystem&&) = delete;
    MyTimeSystem operator=(MyTimeSystem&&) = delete;
    ~MyTimeSystem() = default;

public:
    static MyTimeSystem& Get();

public:
    void Update();

    float DeltaTime() const;
    float FixedDeltaTime() const;

    void SetTimeScale(float timeScale);
    void SetFixedDeltaTime(float fixedDeltaTime);
};