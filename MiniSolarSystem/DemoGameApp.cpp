#include "../D2DEngineLib/framework.h"
#include "DemoGameApp.h"

#include <cassert>

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/SceneManager.h"
#include "SolarSystem.h"

#pragma comment(lib,"windowscodecs.lib")

using Microsoft::WRL::ComPtr;

void DemoGameApp::Initialize()
{
	m_classStyle = CS_HREDRAW | CS_VREDRAW;
	m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_hCursor = LoadCursorW(NULL, IDC_ARROW);

	m_x = CW_USEDEFAULT;
	m_y = CW_USEDEFAULT;
	m_width = 800;
	m_height = 600;

	m_className = L"D2DGame";
	m_windowName = L"MiniSolarSystem";

	__super::Initialize();

	ResourceManager::Initialize(m_d2dRenderer->GetDeviceContext(),
		m_modulePath, L"x64", L"MiniSolarSystem/Resource");

	SceneManager::CreateScene<SolarSystem>(L"SolarSystem");
	SceneManager::ChangeScene(L"SolarSystem");

	m_isRunning = true;
}

void DemoGameApp::Shutdown()
{
	__super::Shutdown();
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
			m_isRunning = false;
		}
		break;
	}
}