#include "../D2DEngineLib/framework.h"
#include "RecordScorePopup.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/BatchRenderer.h"
#include "../D2DEngineLib/SoundManager.h"

#include "RecordNameInputController.h"
#include "GameDataManager.h"
#include "ResultScoreViewer.h"

void RecordScorePopup::Initialize()
{
}

void RecordScorePopup::Start()
{
	//GameDataManager::Get().SetPlayerScore(Random::Int(1000, 999999)); // 임시 데이터 설정
	//GameDataManager::Get().SetPlayTime(Random::Float(300.0f, 600.0f)); // 임시 데이터 설정
	//GameDataManager::Get().SetPlayerKillCount(Random::Int(3, 10)); // 임시 데이터 설정

	GameObject* recordScoreBackground1 = CreateGameObject(L"RecordScoreBackground1");
	auto br = recordScoreBackground1->AddComponent<BitmapRenderer>(L"Image/UI/MAP_OPACITY_01.png");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* recordScoreBackground2 = CreateGameObject(L"RecordScoreBackground2");
	br = recordScoreBackground2->AddComponent<BitmapRenderer>(L"Image/UI/MAP_OPACITY_02.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* recordScoreNumber = CreateGameObject(L"RecordScoreNumber");
	recordScoreNumber->AddComponent<ResultScoreViewer>();
	recordScoreNumber->AddComponent<BatchRenderer>();


	GameObject* continueNotice = CreateGameObject(L"ContinueNotice");
	continueNotice->GetTransform()->SetLocalPosition({ 0.0f, -250.0f });
	br = continueNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_ZX.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* recordNameInputController = CreateGameObject(L"RecordNameInputController");
	recordNameInputController->GetTransform()->SetLocalPosition({ -150.0f, -170.0f });
	recordNameInputController->AddComponent<RecordNameInputController>();
	recordNameInputController->AddComponent<BatchRenderer>();


	SoundManager::Get().PlayGlobalSound(L"GameClearBGM", SoundType::BGM, nullptr, L"GameClearBGM", false);
}