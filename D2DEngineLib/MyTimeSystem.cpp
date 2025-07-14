#include "pch.h"
#include "MyTimeSystem.h"

MyTimeSystem& MyTimeSystem::Get()
{
    static MyTimeSystem s_instance;

    return s_instance;
}

void MyTimeSystem::Update()
{
    m_currentTime = Clock::now();

    std::chrono::duration<float> duration(m_currentTime - m_previousTime);

    m_deltaTime = duration.count();

    m_previousTime = m_currentTime;
}

float MyTimeSystem::DeltaTime() const
{
    return m_deltaTime * m_timeScale;
}

float MyTimeSystem::FixedDeltaTime() const
{
    return m_fixedDeltaTime * m_timeScale;
}

void MyTimeSystem::SetTimeScale(float timeScale)
{
    m_timeScale = timeScale;
}

void MyTimeSystem::SetFixedDeltaTime(float fixedDeltaTime)
{
    m_fixedDeltaTime = fixedDeltaTime;
}