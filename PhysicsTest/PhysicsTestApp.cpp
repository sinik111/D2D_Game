#include "../D2DEngineLib/framework.h"
#include "PhysicsTestApp.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/SoundManager.h"

#include "TestScene1.h"

void PhysicsTestApp::Initialize()
{
	//m_windowStyle = WS_MAXIMIZE | WS_POPUP; // 전체화면
	m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_className = L"D2DGame";
	m_windowName = L"PhysicsTest";

	m_width = 1280;
	m_height = 720;

	WinApp::Initialize();

	Bounds bounds{ Vector2(0.0f, 0.0f), Vector2(m_width * 0.5f, m_height * 0.5f) };

	ComponentSystem::Get().Physics().SetupQuadtree(bounds, 5, 10);

	ResourceManager::Get().Initialize(m_d2dRenderer.GetDeviceContext(),
		m_modulePath, L"Bin", L"PhysicsTest");

	SoundManager::Get().Initialize();

	SceneManager::Get().CreateScene<TestScene1>(L"TestScene1");

	SceneManager::Get().ChangeScene(L"TestScene1");
}

void PhysicsTestApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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