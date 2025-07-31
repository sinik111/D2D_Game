#pragma once

#include <chrono>
#include <sstream>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

class DebugSystem
{
private:
    TimePoint m_lastTimestamp;
    int m_lastFPS = 0;
    int m_frameCount = 0;

    UINT64 m_vramCurrentUsage = 0;
    size_t m_dramCurrentUsage = 0;
    size_t m_pageFileCurrentUsage = 0;

    bool m_isConsoleOn = false;

private:
    DebugSystem();
    DebugSystem(const DebugSystem&) = delete;
    DebugSystem& operator=(const DebugSystem&) = delete;
    DebugSystem(DebugSystem&&) = delete;
    DebugSystem& operator=(DebugSystem&&) = delete;
    ~DebugSystem() = default;

public:
    static DebugSystem& Get();

public:
    // 콘솔 생성
    void CreateConsole();
    // 콘솔 해제
    void ReleaseConsole();

    // 메모리 누수 체크 활성화
    // 프로그램 종료시 자동으로 누수된 메모리 출력됨
    void EnableLeakCheck();
    // 메모리 누수 출력
    // 원하는 시점에서 누수된 메모리 확인용
    void DumpLeaks();

    // Frame Count 누적, 매 프레임 호출 해야함
    void UpdateFPS(bool display = false);
    // 최근 FPS
    int GetLastFPS() const;
    // 최근 FPS 콘솔에 출력, 기본적으로 UpdateFPS에서 호출 중임
    void DisplayFPS() const;

    void Log(const char* log) const;
    void Log(const wchar_t* log) const;
    void Log(const std::string& log) const;
    void Log(const std::wstring& log) const;

    template<typename...Args>
    void Log(Args...args) const
    {
        std::wostringstream woss;

        Log(woss, args...);

        Log(woss.str());
    }

    template<typename T>
    void Log(std::wostringstream& woss, T arg) const
    {
        woss << arg;
    }

    template<typename T, typename...Args>
    void Log(std::wostringstream& woss, T arg, Args...args) const
    {
        woss << arg;

        Log(woss, args...);
    }

    void SetVRAMCurrentUsage(UINT64 usage);
    void SetDRAMCurrentUsage(size_t usage);
    void SetPageFileCurrentUsage(size_t usage);

    UINT64 GetVRAMCurrentUsage() const;
    size_t GetDRAMCurrentUsage() const;
    size_t GetPageFileCurrentUsage() const;
};