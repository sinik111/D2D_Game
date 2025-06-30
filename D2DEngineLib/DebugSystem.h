#pragma once

#include <chrono>

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
    // �ܼ� ����
    void CreateConsole();
    // �ܼ� ����
    void ReleaseConsole();

    // �޸� ���� üũ Ȱ��ȭ
    // ���α׷� ����� �ڵ����� ������ �޸� ��µ�
    void EnableLeakCheck();
    // �޸� ���� ���
    // ���ϴ� �������� ������ �޸� Ȯ�ο�
    void DumpLeaks();

    // Frame Count ����, �� ������ ȣ�� �ؾ���
    void UpdateFPS(bool display = false);
    // �ֱ� FPS
    int GetLastFPS();
    // �ֱ� FPS �ֿܼ� ���, �⺻������ UpdateFPS���� ȣ�� ����
    void DisplayFPS();

    void Log(const std::string& log);
    void Log(const std::wstring& log);

    void SetVRAMCurrentUsage(UINT64 usage);
    void SetDRAMCurrentUsage(size_t usage);
    void SetPageFileCurrentUsage(size_t usage);

    UINT64 GetVRAMCurrentUsage() const;
    size_t GetDRAMCurrentUsage() const;
    size_t GetPageFileCurrentUsage() const;
};