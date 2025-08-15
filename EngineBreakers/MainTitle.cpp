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

	if (!SoundManager::Get().IsPlaying(L"MainTitleBGM"))
	{
		SoundManager::Get().PlayGlobalSound(L"MainTitleBGM", SoundType::BGM, nullptr, L"MainTitleBGM", true);
	}


	GameObject* mainTitleBackground = CreateGameObject(L"MainTitleBackground");
	auto br = mainTitleBackground->AddComponent<BitmapRenderer>(L"Image/UI/TITLE.png");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* mainTitleTitle = CreateGameObject(L"MainTitleTitle");
	mainTitleTitle->GetTransform()->SetLocalPosition({ 0.0f, 300.0f });
	br = mainTitleTitle->AddComponent<BitmapRenderer>(L"Image/UI/LOGO.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* mainTitlePressKeyNotice = CreateGameObject(L"MainTitlePressKeyNotice");
	mainTitlePressKeyNotice->GetTransform()->SetLocalPosition({ 0.0f, -400.0f });
	br = mainTitlePressKeyNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_ZX.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, 1.0f);


	GameObject* mainTitleSceneChanger = CreateGameObject(L"MainTitleSceneChanger");
	mainTitleSceneChanger->AddComponent<MainTitleSceneChanger>();
}