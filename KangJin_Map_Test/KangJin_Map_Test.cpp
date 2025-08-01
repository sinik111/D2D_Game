// KangJin_Map_Test.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "../D2DEngineLib/framework.h"

#include "KangJin_Map_Test.h"
#include "DemoGameApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    Debug::EnableLeakCheck();

    DemoGameApp app;

    app.Initialize();

    app.Run();

    app.Shutdown();
}
