#include "../D2DEngineLib/framework.h"

#include "PlayerWorkspaceApp.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    Debug::EnableLeakCheck();

    PlayerWorkspaceApp app;

    app.Initialize();

    app.Run();

    app.Shutdown();
}