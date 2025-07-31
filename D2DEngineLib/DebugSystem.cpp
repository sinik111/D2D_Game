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

int DebugSystem::GetLastFPS() const
{
    return m_lastFPS;
}

void DebugSystem::DisplayFPS() const
{
    Log("FPS: ", m_lastFPS);
}

void DebugSystem::Log(const char* log) const
{
    std::string str{ log };

    str += '\n';

    OutputDebugStringA(str.c_str());

    if (m_isConsoleOn)
    {
        std::cout << str;
    }
}

void DebugSystem::Log(const wchar_t* log) const
{
    std::wstring str{ log };

    str += L'\n';

    OutputDebugStringW(str.c_str());

    if (m_isConsoleOn)
    {
        std::wcout << str;
    }
}

void DebugSystem::Log(const std::string& log) const
{
    std::string str = log + '\n';

    OutputDebugStringA(str.c_str());

    if (m_isConsoleOn)
    {
        std::cout << str;
    }
}

void DebugSystem::Log(const std::wstring& log) const
{
    std::wstring str = log + L'\n';

    OutputDebugStringW(str.c_str());

    if (m_isConsoleOn)
    {
        std::wcout << str;
    }
}

void DebugSystem::SetVRAMCurrentUsage(UINT64 usage)
{
    m_vramCurrentUsage = usage;
}

void DebugSystem::SetDRAMCurrentUsage(size_t usage)
{
    m_dramCurrentUsage = usage;
}

void DebugSystem::SetPageFileCurrentUsage(size_t usage)
{
    m_pageFileCurrentUsage = usage;
}

UINT64 DebugSystem::GetVRAMCurrentUsage() const
{
    return m_vramCurrentUsage;
}

size_t DebugSystem::GetDRAMCurrentUsage() const
{
    return m_dramCurrentUsage;
}

size_t DebugSystem::GetPageFileCurrentUsage() const
{
    return m_pageFileCurrentUsage;
}
