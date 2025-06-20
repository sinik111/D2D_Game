#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

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

    static void Log(const std::string& log);
    static void Log(const std::wstring& log);
};