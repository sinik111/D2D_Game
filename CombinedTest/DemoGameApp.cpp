#include "../D2DEngineLib/framework.h"
#include "DemoGameApp.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/ImGuiSystem.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/SoundManager.h"

#include "CombineTestScene.h"

void DemoGameApp::Initialize()
{
	m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_className = L"D2DGame";
	m_windowName = L"CombinedTest";

	m_width = 1920;
	m_height = 1080;

	ImGuiSystem::Get().EnableImGuiSystem();
	//ImGuiSystem::Get().DisableImGuiSystem();

	WinApp::Initialize();

	Bounds bounds{ Vector2(0.0f, 0.0f), Vector2(19200, 10800) };

	ComponentSystem::Get().Physics().SetupQuadtree(bounds, 5, 10);

	ResourceManager::Get().Initialize(m_d2dRenderer.GetDeviceContext(),
		m_modulePath, L"Bin", L"CombinedTest/Resource");

	SoundManager::Get().Initialize(ResourceManager::Get().GetResourcePathA());

	SceneManager::Get().CreateScene<CombineTestScene>(L"CombineTest");
	SceneManager::Get().ChangeScene(L"CombineTest");
}

void DemoGameApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WinApp::MessageProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		break;
	}
}
