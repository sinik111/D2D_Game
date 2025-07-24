#include "../D2DEngineLib/framework.h"

#include "DemoGameApp.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    // [Tip] 메모리 누수 확인 활성화.
    // Debug의 대부분의 함수들은 Release모드에서 비활성화 됨.
    Debug::EnableLeakCheck(); 

    DemoGameApp app;

    app.Initialize();

    app.Run();

    app.Shutdown();
}