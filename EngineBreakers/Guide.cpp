#include "../D2DEngineLib/framework.h"
#include "Guide.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"

#include "ScreenCurtainController.h"
#include "GuideSceneController.h"
#include "GameDataManager.h"

void Guide::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();


	GameObject* guideBackground = CreateGameObject(L"GuideBackground");
	auto br = guideBackground->AddComponent<BitmapRenderer>(L"Image/UI/GUIDE.png");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* guideSceneController = CreateGameObject(L"GuideSceneController");
	guideSceneController->AddComponent<GuideSceneController>();


	GameObject* returnNotice = CreateGameObject(L"XReturn");
	returnNotice->GetTransform()->SetLocalPosition({ 800.0f, -350.0f });
	br = returnNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_X.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, GameDataManager::Get().GetFadeDuration());
}