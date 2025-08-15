#include "../D2DEngineLib/framework.h"
#include "EngineBreakers.h"

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
#include "GamePlayScene.h"
#include "Credit.h"

void EngineBreakers::Initialize()
{
	m_windowStyle = WS_MAXIMIZE | WS_POPUP; // 전체화면
	//m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_className = L"D2DGame";
	m_windowName = L"EngineBreakers";

	ShowCursor(FALSE);

	m_width = 1920;
	m_height = 1080;

	WinApp::Initialize();

	Bounds bounds{ Vector2(3000.0f, 2000.0f), Vector2(6000.0f, 5000.0f) };

	ComponentSystem::Get().Physics().SetupQuadtree(bounds, 5, 10);

	ResourceManager::Get().Initialize(m_d2dRenderer.GetDeviceContext(), m_modulePath, L"Bin", L"Resource");

	SoundManager::Get().Initialize();

	SoundManager::Get().LoadGlobalSound("Sound/BGM/TitleAndMain.mp3", L"MainTitleBGM", true);
	SoundManager::Get().LoadGlobalSound("Sound/BGM/GameClear.mp3", L"GameClearBGM", true);
	SoundManager::Get().LoadGlobalSound("Sound/UI/Change_Button.mp3", L"ChangeButton", true);
	SoundManager::Get().LoadGlobalSound("Sound/UI/Select_Buttion.mp3", L"SelectButton", true);

	SceneManager::Get().CreateScene<MainTitle>(L"MainTitle");
	SceneManager::Get().CreateScene<MainMenu>(L"MainMenu");
	SceneManager::Get().CreateScene<Ranking>(L"Ranking");
	SceneManager::Get().CreateScene<Setting>(L"Setting");
	SceneManager::Get().CreateScene<Guide>(L"Guide");
	SceneManager::Get().CreateScene<Credit>(L"Credit");
	SceneManager::Get().CreateScene<RecordScore>(L"RecordScore");
	SceneManager::Get().CreateScene<ResultRanking>(L"ResultRanking");
	SceneManager::Get().CreateScene<GamePlayScene>(L"Game");

	SceneManager::Get().ChangeScene(L"MainTitle");
}

void EngineBreakers::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WinApp::MessageProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_CREATE:
		break;
	}
}
