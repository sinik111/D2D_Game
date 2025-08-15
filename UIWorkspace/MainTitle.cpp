#include "../D2DEngineLib/framework.h"
#include "MainTitle.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/SoundManager.h"
#include "../D2DEngineLib/BitmapRenderer.h"

#include "ScreenCurtainController.h"
#include "MainTitleSceneChanger.h"

void MainTitle::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	if (!SoundManager::Get().IsPlaying(L"MainSceneBGM"))
	{
		SoundManager::Get().PlayGlobalSound(L"MainSceneBGM", SoundType::BGM, L"MainSceneBGM", true);
	}


	GameObject* mainTitleBackground = CreateGameObject(L"MainTitleBackground");
	auto br = mainTitleBackground->AddComponent<BitmapRenderer>(L"Image/MainTitleBackground.png");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* mainTitlePressKeyNotice = CreateGameObject(L"MainTitlePressKeyNotice");
	mainTitlePressKeyNotice->GetTransform()->SetLocalPosition({ 0.0f, -200.0f });
	br = mainTitlePressKeyNotice->AddComponent<BitmapRenderer>(L"Image/MainTitlePressKeyNotice.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetColor({ 1.0f, 0.8f, 0.8f, 1.0f });


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, 1.0f);


	GameObject* mainTitleSceneChanger = CreateGameObject(L"MainTitleSceneChanger");
	mainTitleSceneChanger->AddComponent<MainTitleSceneChanger>();
}