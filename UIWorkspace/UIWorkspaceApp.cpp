#include "../D2DEngineLib/framework.h"
#include "UIWorkspaceApp.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/SoundManager.h"

#include "MainTitle.h"
#include "MainMenu.h"
#include "Ranking.h"
#include "Setting.h"
#include "Guide.h"
#include "RecordScore.h"
#include "ResultRanking.h"

void UIWorkspaceApp::Initialize()
{
	//m_windowStyle = WS_MAXIMIZE | WS_POPUP; // 전체화면
	m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_className = L"D2DGame";
	m_windowName = L"UIWorkspace";

	m_width = 1920;
	m_height = 1080;

	WinApp::Initialize();

	Bounds bounds{ Vector2(0.0f, 0.0f), Vector2(m_width * 0.5f, m_height * 0.5f) };

	ComponentSystem::Get().Physics().SetupQuadtree(bounds, 5, 10);

	ResourceManager::Get().Initialize(m_d2dRenderer.GetDeviceContext(), m_modulePath, L"Bin", L"UIWorkspace/Resource");

	SoundManager::Get().Initialize();

	SoundManager::Get().LoadGlobalSound("Sound/TheFatRat - Arcadia.mp3", L"MainSceneBGM");
	SceneManager::Get().CreateScene<MainTitle>(L"MainTitle");

	SceneManager::Get().CreateScene<MainMenu>(L"MainMenu");
	SceneManager::Get().CreateScene<Ranking>(L"Ranking");
	SceneManager::Get().CreateScene<Setting>(L"Setting");
	SceneManager::Get().CreateScene<Guide>(L"Guide");
	SceneManager::Get().CreateScene<RecordScore>(L"RecordScore");
	SceneManager::Get().CreateScene<ResultRanking>(L"ResultRanking");

	SceneManager::Get().ChangeScene(L"RecordScore");
}

void UIWorkspaceApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
