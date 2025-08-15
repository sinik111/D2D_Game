#include "../D2DEngineLib/framework.h"
#include "Credit.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"

#include "ScreenCurtainController.h"
#include "CreditSceneController.h"
#include "GameDataManager.h"

void Credit::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	GameObject* creditBackground = CreateGameObject(L"CreditBackground");
	auto br = creditBackground->AddComponent<BitmapRenderer>(L"Image/UI/credit.png");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* creditSceneController = CreateGameObject(L"CreditSceneController");
	creditSceneController->AddComponent<CreditSceneController>();


	GameObject* returnNotice = CreateGameObject(L"XReturn");
	returnNotice->GetTransform()->SetLocalPosition({ 800.0f, -350.0f });
	br = returnNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_X.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, GameDataManager::Get().GetFadeDuration());
}