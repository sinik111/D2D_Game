#include "pch.h"
#include "Debug.h"

void Debug::CreateConsole()
{
#ifdef _DEBUG
    DebugSystem::Get().CreateConsole();
#endif // _DEBUG
}

void Debug::ReleaseConsole()
{
#ifdef _DEBUG
    DebugSystem::Get().ReleaseConsole();
#endif // _DEBUG
}

void Debug::EnableLeakCheck()
{
#ifdef _DEBUG
    DebugSystem::Get().EnableLeakCheck();
#endif // _DEBUG
}

void Debug::DumpLeaks()
{
#ifdef _DEBUG
    DebugSystem::Get().DumpLeaks();
#endif // _DEBUG
}

void Debug::UpdateFPS(bool display)
{
    DebugSystem::Get().UpdateFPS(display);
}

int Debug::GetLastFPS()
{
    return DebugSystem::Get().GetLastFPS();
}

void Debug::DisplayFPS()
{
#ifdef _DEBUG
    DebugSystem::Get().DisplayFPS();
#endif // _DEBUG
}

void Debug::Log(const char* log)
{
#ifdef _DEBUG
    DebugSystem::Get().Log(log);
#endif // _DEBUG
}

void Debug::Log(const wchar_t* log)
{
#ifdef _DEBUG
    DebugSystem::Get().Log(log);
#endif // _DEBUG
}

void Debug::Log(const std::string& log)
{
#ifdef _DEBUG
    DebugSystem::Get().Log(log);
#endif // _DEBUG
}

void Debug::Log(const std::wstring& log)
{
#ifdef _DEBUG
    DebugSystem::Get().Log(log);
#endif // _DEBUG
}

UINT64 Debug::GetVRAMCurrentUsage()
{
    return DebugSystem::Get().GetVRAMCurrentUsage();
}

size_t Debug::GetDRAMCurrentUsage()
{
    return DebugSystem::Get().GetDRAMCurrentUsage();
}

size_t Debug::GetPageFileCurrentUsage()
{
    return DebugSystem::Get().GetPageFileCurrentUsage();
}
