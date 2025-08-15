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
	auto br = guideBackground->AddComponent<BitmapRenderer>(L"Image/GuideBackground.bmp");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* guideSceneController = CreateGameObject(L"GuideSceneController");
	guideSceneController->AddComponent<GuideSceneController>();


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, GameDataManager::Get().GetFadeDuration());
}