#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

// Release 모드에서는 대부분 작동안함
class Debug
{
public:
    // 콘솔 생성
    static void CreateConsole();
    // 콘솔 해제
    static void ReleaseConsole();

    // 메모리 누수 체크 활성화
    // 프로그램 종료시 자동으로 누수된 메모리 출력됨
    static void EnableLeakCheck();
    // 메모리 누수 출력
    // 원하는 시점에서 누수된 메모리 확인용
    static void DumpLeaks();

    // Frame Count 누적, 매 프레임 호출 해야함
    static void UpdateFPS(bool display = false);
    // 최근 FPS
    static int GetLastFPS();
    // 최근 FPS 콘솔에 출력, 기본적으로 UpdateFPS에서 호출 중임
    static void DisplayFPS();

    static void Log(const std::string& log);
    static void Log(const std::wstring& log);

    static UINT64 GetVRAMCurrentUsage();
    static size_t GetDRAMCurrentUsage();
    static size_t GetPageFileCurrentUsage();
};