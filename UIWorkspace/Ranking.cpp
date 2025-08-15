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
	auto br = rankingBackground->AddComponent<BitmapRenderer>(L"Image/RankingBackground.bmp");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* rankingViewer = CreateGameObject(L"RankingViewer");
	rankingViewer->AddComponent<RankingViewer>();
	rankingViewer->AddComponent<BatchRenderer>();


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, GameDataManager::Get().GetFadeDuration());
}