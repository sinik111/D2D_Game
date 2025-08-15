#include "../D2DEngineLib/framework.h"
#include "ResultRanking.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/BatchRenderer.h"

#include "ScreenCurtainController.h"
#include "ResultRankingViewer.h"
#include "GameDataManager.h"

void ResultRanking::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();


	GameObject* resultRankingBackground = CreateGameObject(L"ResultRankingBackground");
	auto br = resultRankingBackground->AddComponent<BitmapRenderer>(L"Image/ResultRankingBackground.bmp");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* resultRankingViewer = CreateGameObject(L"ResultRankingViewer");
	resultRankingViewer->AddComponent<ResultRankingViewer>();
	resultRankingViewer->AddComponent<BatchRenderer>();


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, GameDataManager::Get().GetFadeDuration());
}