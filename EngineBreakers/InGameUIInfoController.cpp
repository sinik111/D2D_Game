#include "../D2DEngineLib/framework.h"
#include "InGameUIInfoController.h"

#include "../D2DEngineLib/BatchRenderer.h"
#include "../D2DEngineLib/Screen.h"
#include "../D2DEngineLib/SoundManager.h"

#include "RankingUtility.h"
#include "GameDataManager.h"

void InGameUIInfoController::Initialize()
{
	float halfScreenWidth = Screen::Get().GetWidth() * 0.5f;
	float halfScreenHeight = Screen::Get().GetHeight() * 0.5f;

	m_batchRenderer = GetGameObject()->GetComponent<BatchRenderer>();
	m_batchRenderer->SetBitmap(L"Image/UI/SCORE&DEATH_NUMBER.png");
	m_batchRenderer->SetSpriteSheet(L"Image/UI/SCORE&DEATH_NUMBER_sprites.json");
	m_batchRenderer->SetSpaceType(SpaceType::Screen);
	m_batchRenderer->SetSortOrder(2);
	m_batchRenderer->SetLocalRect(
		{
			-halfScreenWidth,
			halfScreenHeight,
			halfScreenWidth,
			-halfScreenHeight
		}
	);
}

void InGameUIInfoController::Start()
{
	int time = 0;

	int count = 0;
	auto secondIndexList = ConvertScoreNumbers(time % 60);
	for (auto index : secondIndexList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_timeStartPosX, m_timerPosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}

	if (count == 1)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(ScoreSpriteIndex::Num0),
				Vector2(-m_wordWidth * count + m_timeStartPosX, m_timerPosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}

	m_batchRenderer->AddBatchUnit(
		{
			static_cast<size_t>(ScoreSpriteIndex::Colon),
			Vector2(-m_wordWidth * count + m_timeStartPosX, m_timerPosY),
			Vector2(1.0f, 1.0f),
			0.0f,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		}
	);

	++count;

	auto minuteIndexList = ConvertScoreNumbers(time / 60);
	for (auto index : minuteIndexList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_timeStartPosX, m_timerPosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}

	if (count == 4)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(ScoreSpriteIndex::Num0),
				Vector2(-m_wordWidth * count + m_timeStartPosX, m_timerPosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);
	}

	int death = 0;
	count = 0;
	auto deathCountList = ConvertScoreNumbers(death);

	for (auto index : deathCountList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_deathStartPosX, m_posY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);
	}

	int score = 0;
	count = 0;
	auto scoreNumList = ConvertScoreNumbers(score);

	for (auto index : scoreNumList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_scoreStartPosX, m_posY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);
	}
}

void InGameUIInfoController::Update()
{
	if (MyTime::DeltaTime() < 0.1f)
	{
		m_time += MyTime::DeltaTime();

		if (!m_onceBgmStarted)
		{
			m_onceBgmStarted = true;

			SoundManager::Get().PlaySound(L"GamePlayBGM", SoundType::BGM, nullptr, L"GamePlayBGM", true);
		}
	}

	GameDataManager::Get().SetPlayTime(m_time);

	m_batchRenderer->ClearBatchUnitList();

	int count = 0;
	auto secondIndexList = ConvertScoreNumbers(static_cast<int>(m_time) % 60);
	for (auto index : secondIndexList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_timeStartPosX, m_timerPosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}

	if (count == 1)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(ScoreSpriteIndex::Num0),
				Vector2(-m_wordWidth * count + m_timeStartPosX, m_timerPosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}

	m_batchRenderer->AddBatchUnit(
		{
			static_cast<size_t>(ScoreSpriteIndex::Colon),
			Vector2(-m_wordWidth * count + m_timeStartPosX, m_timerPosY),
			Vector2(1.0f, 1.0f),
			0.0f,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		}
	);

	++count;

	auto minuteIndexList = ConvertScoreNumbers(static_cast<int>(m_time / 60));
	for (auto index : minuteIndexList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_timeStartPosX, m_timerPosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}

	if (count == 4)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(ScoreSpriteIndex::Num0),
				Vector2(-m_wordWidth * count + m_timeStartPosX, m_timerPosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);
	}

	int death = GameDataManager::Get().GetPlayerDeathCount();
	count = 0;
	auto deathCountList = ConvertScoreNumbers(death);

	for (auto index : deathCountList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_deathStartPosX, m_posY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);
		++count;
	}

	int score = GameDataManager::Get().GetPlayerScore();
	count = 0;
	auto scoreNumList = ConvertScoreNumbers(score);

	for (auto index : scoreNumList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_scoreStartPosX, m_posY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);
		++count;
	}
}

void InGameUIInfoController::SetScore(int score)
{
	m_score = score;
}

void InGameUIInfoController::SetDeathCount(int deathCount)
{
	m_death = deathCount;
}
