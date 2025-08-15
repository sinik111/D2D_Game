#include "../D2DEngineLib/framework.h"
#include "GameOverController.h"

#include "../D2DEngineLib/BatchRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/SoundManager.h"

#include "RankingUtility.h"
#include "ScreenCurtainController.h"
#include "GameDataManager.h"
#include "Player.h"

void GameOverController::Start()
{
	m_batchRenderer = GetGameObject()->AddComponent<BatchRenderer>();
	m_batchRenderer->SetBitmap(L"Image/UI/COUNTDOWN_NUMBER.png");
	m_batchRenderer->SetSpriteSheet(L"Image/UI/COUNTDOWN_NUMBER_sprites.json");
	m_batchRenderer->SetSpaceType(SpaceType::Screen);
	m_batchRenderer->SetSortOrder(8);

	m_background1 = CreateGameObject(L"GameOverBackground1");
	auto br = m_background1->AddComponent<BitmapRenderer>(L"Image/UI/OVER_OPACITY_01.png");
	br->SetSpaceType(SpaceType::Screen);
	br->SetSortOrder(6);

	m_background2 = CreateGameObject(L"GameOverBackground2");
	br = m_background2->AddComponent<BitmapRenderer>(L"Image/UI/OVER_OPACITY_02.png");
	br->SetSpaceType(SpaceType::Screen);
	br->SetSortOrder(7);

	m_continueNotice = CreateGameObject(L"GameOverContinueNotice");
	br = m_continueNotice->AddComponent<BitmapRenderer>(L"Image/UI/OVER_PRESS_Z.png");
	br->SetSpaceType(SpaceType::Screen);
	br->SetSortOrder(8);
	m_continueNotice->GetTransform()->SetLocalPosition({ 0.0f, 200.0f });

	m_giveupNotice = CreateGameObject(L"GameOverGiveupNotice");
	br = m_giveupNotice->AddComponent<BitmapRenderer>(L"Image/UI/OVER_PRESS_X.png");
	br->SetSpaceType(SpaceType::Screen);
	br->SetSortOrder(8);
	m_giveupNotice->GetTransform()->SetLocalPosition({ 0.0f, -200.0f });

	SoundManager::Get().PauseSound(L"GamePlayBGM");
	SoundManager::Get().PlaySound(L"CountDown", SoundType::BGM, nullptr, L"CountDown", true);
}

void GameOverController::Update()
{
	if (m_screenCurtainController == nullptr)
	{
		m_duration -= MyTime::DeltaTime();
		if (m_duration <= 0.0f)
		{
			m_duration = 0.0f;
		
			GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
			m_screenCurtainController = screenCurtainController->AddComponent<ScreenCurtainController>(false, 1.0f);
		}

		m_batchRenderer->ClearBatchUnitList();

		int index = static_cast<int>(m_duration) / 2;
		if (index == 10)
		{
			m_batchRenderer->AddBatchUnit(
				{
					static_cast<size_t>(ScoreSpriteIndex::Num1),
					Vector2(-55.0f, 0.0f),
					Vector2(1.0f, 1.0f),
					0.0f,
					{ 1.0f, 1.0f, 1.0f, 1.0f }
				}
			);

			m_batchRenderer->AddBatchUnit(
				{
					static_cast<size_t>(ScoreSpriteIndex::Num0),
					Vector2(55.0f, 0.0f),
					Vector2(1.0f, 1.0f),
					0.0f,
					{ 1.0f, 1.0f, 1.0f, 1.0f }
				}
			);
		}
		else
		{
			m_batchRenderer->AddBatchUnit(
				{
					static_cast<size_t>(ScoreSpriteIndex::Num0),
					Vector2(-55.0f, 0.0f),
					Vector2(1.0f, 1.0f),
					0.0f,
					{ 1.0f, 1.0f, 1.0f, 1.0f }
				}
			);

			m_batchRenderer->AddBatchUnit(
				{
					static_cast<size_t>(index),
					Vector2(55.0f, 0.0f),
					Vector2(1.0f, 1.0f),
					0.0f,
					{ 1.0f, 1.0f, 1.0f, 1.0f }
				}
			);
		}
		
		if (Input::IsKeyPressed('X'))
		{
			SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);
			SoundManager::Get().StopSound(L"CountDown");
			GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
			m_screenCurtainController = screenCurtainController->AddComponent<ScreenCurtainController>(false, 1.0f);
		}
		else if (Input::IsKeyPressed('Z'))
		{
			SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);
			SoundManager::Get().StopSound(L"CountDown");
			SoundManager::Get().ResumeSound(L"GamePlayBGM");
			
			m_player->GetPlayerStatus().currentHp = m_player->GetPlayerStat().maxHp;

			m_player->SetPlayerNextState(L"Idle");

			m_player->SetIsGameOver(false);

			int deathCount = GameDataManager::Get().GetPlayerDeathCount();
			int continueCount = GameDataManager::Get().GetPlayerContinueCount();

			GameDataManager::Get().SetPlayerDeathCount(deathCount + 1);
			GameDataManager::Get().SetPlayerContinueCount(continueCount + 1);

			int playerScore = GameDataManager::Get().GetPlayerScore();

			playerScore -= 1000;
			if (playerScore < 0)
			{
				playerScore = 0;
			}

			GameDataManager::Get().SetPlayerScore(playerScore);

			CleanupObjects();
		}
	}
	else
	{
		if (m_screenCurtainController->IsFinished())
		{
			SoundManager::Get().StopSound(L"GamePlayBGM");

			SceneManager::Get().ChangeScene(L"MainTitle");
		}
	}
}

void GameOverController::CleanupObjects()
{
	if (m_background1)
	{
		Destroy(m_background1);
	}

	if (m_background2)
	{
		Destroy(m_background2);
	}

	if (m_continueNotice)
	{
		Destroy(m_continueNotice);
	}

	if (m_giveupNotice)
	{
		Destroy(m_giveupNotice);
	}

	Destroy(GetGameObject());
}

void GameOverController::SetPlayer(Player* player)
{
	m_player = player;
}
