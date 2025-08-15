#include "../D2DEngineLib/framework.h"
#include "RecordScore.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/BatchRenderer.h"

#include "ScreenCurtainController.h"
#include "ResultScoreViewer.h"
#include "RecordNameInputController.h"
#include "GameDataManager.h"

void RecordScore::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	GameDataManager::Get().SetPlayerScore(Random::Int(1000, 999999)); // 임시 데이터 설정

	GameObject* recordScoreBackground = CreateGameObject(L"RecordScoreBackground");
	auto br = recordScoreBackground->AddComponent<BitmapRenderer>(L"Image/RecoredScoreBackground.bmp");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* recordScoreName = CreateGameObject(L"RecordScoreName");
	recordScoreName->GetTransform()->SetLocalPosition({ -400.0f, 0.0f });
	br = recordScoreName->AddComponent<BitmapRenderer>(L"Image/RecordScoreName.bmp");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });


	GameObject* recordScoreNumber = CreateGameObject(L"RecordScoreNumber");
	recordScoreNumber->GetTransform()->SetLocalPosition({ 400.0f, 0.0f });
	recordScoreNumber->AddComponent<ResultScoreViewer>();
	recordScoreNumber->AddComponent<BatchRenderer>();


	GameObject* recordNameInputController = CreateGameObject(L"RecordNameInputController");
	recordNameInputController->GetTransform()->SetLocalPosition({ -400.0f, -400.0f });
	recordNameInputController->AddComponent<RecordNameInputController>();
	recordNameInputController->AddComponent<BatchRenderer>();


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, GameDataManager::Get().GetFadeDuration());
}