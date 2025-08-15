#include "../D2DEngineLib/framework.h"
#include "UITestApp.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/SoundManager.h"

#include "Scene1.h"

void UITestApp::Initialize()
{
	//m_windowStyle = WS_MAXIMIZE | WS_POPUP; // 전체화면
	m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_className = L"D2DGame";
	m_windowName = L"UITest";

	m_width = 1280;
	m_height = 720;

	WinApp::Initialize();

	ResourceManager::Get().Initialize(m_d2dRenderer.GetDeviceContext(), m_modulePath, L"Bin", L"UITest/Resource");

	SoundManager::Get().Initialize();

	SceneManager::Get().CreateScene<Scene1>(L"Scene1");

	SceneManager::Get().ChangeScene(L"Scene1");
}

void UITestApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
