#include "../D2DEngineLib/framework.h"
#include "DemoGameApp.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/ImGuiSystem.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "KangJinTestScene.h"

void DemoGameApp::Initialize()
{
	m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_className = L"D2DGame";
	m_windowName = L"MiniSolarSystem";

	m_width = 1920;
	m_height = 1080;

	ImGuiSystem::Get().EnableImGuiSystem();

	WinApp::Initialize();

	Bounds bounds{ Vector2(0.0f, 0.0f), Vector2(m_width * 0.5f, m_height * 0.5f) };

	ComponentSystem::Get().Physics().SetupQuadtree(bounds, 5, 10);

	ResourceManager::Get().Initialize(m_d2dRenderer.GetDeviceContext(),
		m_modulePath, L"x64", L"KangJin_Map_Test/Resource");

	SceneManager::Get().CreateScene<KangJinTestScene>(L"KangJinTestScene");
	SceneManager::Get().ChangeScene(L"KangJinTestScene");
}

void DemoGameApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WinApp::MessageProc(hWnd, uMsg, wParam, lParam);
	
	switch (uMsg)
	{
	case WM_KEYUP:
		if(wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		break;
	}
}
