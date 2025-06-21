#include "pch.h"
#include "DebugSystem.h"

#include <crtdbg.h>
#include <iostream>

#include "MyTime.h"

DebugSystem::DebugSystem()
    : m_lastTimestamp{ Clock::now() }
{
    
}

DebugSystem& DebugSystem::Get()
{
    static DebugSystem s_instance;

    return s_instance;
}

void DebugSystem::CreateConsole()
{
    if (!m_isConsoleOn)
    {
        AllocConsole();
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        SetConsoleTitleW(L"Debug Console");
        m_isConsoleOn = true;

        Log("Console created");
    }
    else
    {
        Log("Console is already created");
    }

}

void DebugSystem::ReleaseConsole()
{
    if (m_isConsoleOn)
    {
        // 표준 출력 스트림 닫기
        fclose(stdout);
        // 콘솔 해제
        FreeConsole();

        m_isConsoleOn = false;

        Log("Console released");
    }
    else
    {
        Log("Console is not created");
    }
}

void DebugSystem::EnableLeakCheck()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

void DebugSystem::DumpLeaks()
{
    _CrtDumpMemoryLeaks();
}

void DebugSystem::UpdateFPS(bool display)
{
    ++m_frameCount;

    if (MyTime::GetElapsedSeconds(m_lastTimestamp) > 1.0f)
    {
        m_lastTimestamp = MyTime::GetAccumulatedTime(m_lastTimestamp, 1);

        m_lastFPS = m_frameCount;

        m_frameCount = 0;

#ifdef _DEBUG
        if (display)
        {
            DisplayFPS();
        }
#endif // _DEBUG
    }
}

int DebugSystem::GetLastFPS()
{
    return m_lastFPS;
}

void DebugSystem::DisplayFPS()
{
    Log(std::string("FPS: " + std::to_string(m_lastFPS)));
}

void DebugSystem::Log(const std::string& log)
{
    std::string str = log + "\n";

    OutputDebugStringA(str.c_str());

    if (m_isConsoleOn)
    {
        std::cout << str;
    }
}

void DebugSystem::Log(const std::wstring& log)
{
    std::wstring str = log + L"\n";

    OutputDebugStringW(str.c_str());

    if (m_isConsoleOn)
    {
        std::wcout << str;
    }
}
