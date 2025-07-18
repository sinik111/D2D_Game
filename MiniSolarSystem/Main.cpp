#include "../D2DEngineLib/framework.h"

#include "DemoGameApp.h"
#include "../D2DEngineLib/Debug.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    Debug::EnableLeakCheck(); 

    DemoGameApp app;

    app.Initialize();

    app.Run();

    app.Shutdown();
}