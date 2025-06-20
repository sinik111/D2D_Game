#include "pch.h"
#include "Debug.h"

#include "DebugSystem.h"

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
