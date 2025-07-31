#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

#include "DebugSystem.h"

// Release ��忡���� ��κ� �۵�����
class Debug
{
public:
    // �ܼ� ����
    static void CreateConsole();
    // �ܼ� ����
    static void ReleaseConsole();

    // �޸� ���� üũ Ȱ��ȭ
    // ���α׷� ����� �ڵ����� ������ �޸� ��µ�
    static void EnableLeakCheck();
    // �޸� ���� ���
    // ���ϴ� �������� ������ �޸� Ȯ�ο�
    static void DumpLeaks();

    // Frame Count ����, �� ������ ȣ�� �ؾ���
    static void UpdateFPS(bool display = false);
    // �ֱ� FPS
    static int GetLastFPS();
    // �ֱ� FPS �ֿܼ� ���, �⺻������ UpdateFPS���� ȣ�� ����
    static void DisplayFPS();

    static void Log(const char* log);
    static void Log(const wchar_t* log);
    static void Log(const std::string& log);
    static void Log(const std::wstring& log);
    template<typename...Args>
    static void Log(Args...args)
    {
#ifdef _DEBUG
        DebugSystem::Get().Log(args...);
#endif // _DEBUG
    }

    static UINT64 GetVRAMCurrentUsage();
    static size_t GetDRAMCurrentUsage();
    static size_t GetPageFileCurrentUsage();
};