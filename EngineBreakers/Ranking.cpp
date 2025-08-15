#include "../D2DEngineLib/framework.h"
#include "Ranking.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/BatchRenderer.h"

#include "ScreenCurtainController.h"
#include "RankingViewer.h"
#include "GameDataManager.h"

void Ranking::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();


	GameObject* rankingBackground = CreateGameObject(L"RankingBackground");
	auto br = rankingBackground->AddComponent<BitmapRenderer>(L"Image/UI/RANKING_BACKGROUND.png");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* rankingViewer = CreateGameObject(L"RankingViewer");
	rankingViewer->AddComponent<RankingViewer>();
	rankingViewer->AddComponent<BatchRenderer>();


	GameObject* returnNotice = CreateGameObject(L"XReturn");
	returnNotice->GetTransform()->SetLocalPosition({ 800.0f, -470.0f });
	br = returnNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_X.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });


	GameObject* moveNotice = CreateGameObject(L"MoveNotice");
	moveNotice->GetTransform()->SetLocalPosition({ 800.0f, -440.0f });
	moveNotice->GetTransform()->SetLocalScale(0.65f, 0.65f);
	br = moveNotice->AddComponent<BitmapRenderer>(L"Image/UI/USE_A.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, GameDataManager::Get().GetFadeDuration());
}