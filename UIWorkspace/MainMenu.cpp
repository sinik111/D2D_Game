#include "../D2DEngineLib/framework.h"
#include "MainMenu.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"

#include "ScreenCurtainController.h"
#include "MainMenuSceneController.h"

void MainMenu::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();


	GameObject* mainMenuBackground = CreateGameObject(L"MainMenuBackground");
	auto br = mainMenuBackground->AddComponent<BitmapRenderer>(L"Image/MainMenuBackground.bmp");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* mainMenuTitle = CreateGameObject(L"MainMenuTitle");
	br = mainMenuTitle->AddComponent<BitmapRenderer>(L"Image/MainMenuTitle.bmp");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetColor({ 1.0f, 1.0f, 0.9f, 1.0f });
	br->SetPivot({ 0.0f, 0.0f });
	mainMenuTitle->GetTransform()->SetLocalPosition({ -830.0f, 420.0f });


	GameObject* startButton = CreateGameObject(L"Game");
	br = startButton->AddComponent<BitmapRenderer>(L"Image/MainMenuGameStart.bmp");
	br->SetColor({ 0.9f, 1.0f, 0.9f, 1.0f });
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });

	GameObject* rankingButton = CreateGameObject(L"Ranking");
	br = rankingButton->AddComponent<BitmapRenderer>(L"Image/MainMenuRanking.bmp");
	br->SetColor({ 0.9f, 1.0f, 0.9f, 1.0f });
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });

	GameObject* guideButton = CreateGameObject(L"Guide");
	br = guideButton->AddComponent<BitmapRenderer>(L"Image/MainMenuGuide.bmp");
	br->SetColor({ 0.9f, 1.0f, 0.9f, 1.0f });
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });

	GameObject* settingButton = CreateGameObject(L"Setting");
	br = settingButton->AddComponent<BitmapRenderer>(L"Image/MainMenuSetting.bmp");
	br->SetColor({ 0.9f, 1.0f, 0.9f, 1.0f });
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });

	GameObject* exitButton = CreateGameObject(L"Exit");
	br = exitButton->AddComponent<BitmapRenderer>(L"Image/MainMenuExit.bmp");
	br->SetColor({ 0.9f, 1.0f, 0.9f, 1.0f });
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });

	GameObject* indicator = CreateGameObject(L"Indicator");
	br = indicator->AddComponent<BitmapRenderer>(L"Image/MainMenuArrow.bmp");
	br->SetColor({ 0.9f, 0.9f, 1.0f, 1.0f });
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });


	GameObject* mainMenuSceneController = CreateGameObject(L"MainMenuSceneController");
	auto script = mainMenuSceneController->AddComponent<MainMenuSceneController>();
	script->SetMenuButtons(
		{
			startButton,
			rankingButton,
			guideButton,
			settingButton,
			exitButton
		}
	);
	script->SetIndicator(indicator);



	float fadeInTime;
	if (SceneManager::Get().GetPreviousSceneName() == L"MainTitle")
	{
		fadeInTime = 1.0f;
	}
	else
	{
		fadeInTime = 0.1f;
	}

	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, fadeInTime);
}