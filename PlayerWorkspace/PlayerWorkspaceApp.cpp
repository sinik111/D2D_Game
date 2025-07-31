#include "../D2DEngineLib/framework.h"
#include "PlayerWorkspaceApp.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/ComponentSystem.h"

#include "Scene1.h"

void PlayerWorkspaceApp::Initialize()
{
	//m_windowStyle = WS_MAXIMIZE | WS_POPUP; // 전체화면
	m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_className = L"D2DGame";
	m_windowName = L"PlayerWorkspace";

	m_width = 1920;
	m_height = 1080;

	WinApp::Initialize();

	Bounds bounds{ Vector2(0.0f, 0.0f), Vector2(m_width * 0.5f, m_height * 0.5f) };

	ComponentSystem::Get().Physics().SetupQuadtree(bounds, 5, 10);

	ResourceManager::Get().Initialize(m_d2dRenderer.GetDeviceContext(),
		m_modulePath, L"x64", L"PlayerWorkspace");

	SceneManager::Get().CreateScene<Scene1>(L"Scene1");

	SceneManager::Get().ChangeScene(L"Scene1");
}

void PlayerWorkspaceApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WinApp::MessageProc(hWnd, uMsg, wParam, lParam);

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
