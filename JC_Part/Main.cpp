#include "../D2DEngineLib/framework.h"

#include "JC_DemoGameApp.h"

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	// [Tip] �޸� ���� Ȯ�� Ȱ��ȭ.
	// Debug�� ��κ��� �Լ����� Release��忡�� ��Ȱ��ȭ ��.
	Debug::EnableLeakCheck();

	JC_DemoGameApp app;

	app.Initialize();

	app.Run();

	app.Shutdown();
}