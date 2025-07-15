#include "../D2DEngineLib/framework.h"
#include "DemoGameApp.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/SceneManager.h"
#include "SolarSystem.h"
#include "MainTitle.h"
#include "AnimationTestScene.h"

void DemoGameApp::Initialize()
{
	m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_className = L"D2DGame";
	m_windowName = L"MiniSolarSystem";

	m_width = 1920;
	m_height = 1080;

	__super::Initialize();

	ResourceManager::Get().Initialize(m_d2dRenderer->GetDeviceContext(),
		m_modulePath, L"x64", L"MiniSolarSystem/Resource");

	SceneManager::Get().CreateScene<MainTitle>(L"Title");
	SceneManager::Get().CreateScene<SolarSystem>(L"SolarSystem");
	SceneManager::Get().CreateScene<AnimationTestScene>(L"AnimationTest");

	SceneManager::Get().ChangeScene(L"Title");
}

void DemoGameApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	__super::MessageProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		break;
	}
}