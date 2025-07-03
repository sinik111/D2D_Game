#include "pch.h"
#include "MyTimeSystem.h"

MyTimeSystem::MyTimeSystem()
    : m_previousTime{ Clock::now() }, m_currentTime{ Clock::now() }
{

}

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

float MyTimeSystem::DeltaTime()
{
    return m_deltaTime * m_timeScale;
}

void MyTimeSystem::SetTimeScale(float timeScale)
{
    m_timeScale = timeScale;
}
