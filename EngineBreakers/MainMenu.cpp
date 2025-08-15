#include "../D2DEngineLib/framework.h"
#include "MainMenu.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ComponentSystem.h"

#include "ScreenCurtainController.h"
#include "MainMenuSceneController.h"
#include "GameDataManager.h"

void MainMenu::Enter()
{
	ComponentSystem::Get().PlayerInput().SetIsStopped(false);

	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();


	GameObject* mainMenuBackground = CreateGameObject(L"MainMenuBackground");
	auto br = mainMenuBackground->AddComponent<BitmapRenderer>(L"Image/UI/TITLE_BACKGROUND.png");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* mainMenuTitle = CreateGameObject(L"MainMenuTitle");
	br = mainMenuTitle->AddComponent<BitmapRenderer>(L"Image/UI/LOGO.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.0f });
	mainMenuTitle->GetTransform()->SetLocalPosition({ -875.0f, 442.0f });


	GameObject* startButton = CreateGameObject(L"Game");
	br = startButton->AddComponent<BitmapRenderer>(L"Image/UI/ICON_START_01.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });

	GameObject* rankingButton = CreateGameObject(L"Ranking");
	br = rankingButton->AddComponent<BitmapRenderer>(L"Image/UI/ICON_RANKING_01.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });

	GameObject* guideButton = CreateGameObject(L"Guide");
	br = guideButton->AddComponent<BitmapRenderer>(L"Image/UI/ICON_GUIDE_01.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });

	GameObject* settingButton = CreateGameObject(L"Setting");
	br = settingButton->AddComponent<BitmapRenderer>(L"Image/UI/ICON_Option_01.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });

	GameObject* creditButton = CreateGameObject(L"Credit");
	br = creditButton->AddComponent<BitmapRenderer>(L"Image/UI/ICON_CREDIT_01.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });

	GameObject* exitButton = CreateGameObject(L"Exit");
	br = exitButton->AddComponent<BitmapRenderer>(L"Image/UI/ICON_QUITGAME_01.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });


	GameObject* mainMenuSceneController = CreateGameObject(L"MainMenuSceneController");
	auto script = mainMenuSceneController->AddComponent<MainMenuSceneController>();
	script->SetMenuButtons(
		{
			startButton,
			rankingButton,
			guideButton,
			settingButton,
			creditButton,
			exitButton
		}
	);


	float fadeInTime;
	if (SceneManager::Get().GetPreviousSceneName() == L"MainTitle")
	{
		fadeInTime = 1.0f;
	}
	else
	{
		fadeInTime = 0.1f;
	}


	GameObject* selectNotice = CreateGameObject(L"ZSelect");
	selectNotice->GetTransform()->SetLocalPosition({ 800.0f, -500.0f });
	br = selectNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_Z.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });


	GameObject* moveNotice = CreateGameObject(L"MoveNotice");
	moveNotice->GetTransform()->SetLocalPosition({ 800.0f, -470.0f });
	moveNotice->GetTransform()->SetLocalScale(0.65f, 0.65f);
	br = moveNotice->AddComponent<BitmapRenderer>(L"Image/UI/USE_A.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, fadeInTime);
}