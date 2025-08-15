#include "../D2DEngineLib/framework.h"
#include "ResultScoreViewer.h"

#include "../D2DEngineLib/BatchRenderer.h"
#include "../D2DEngineLib/Screen.h"

#include "RankingUtility.h"
#include "GameDataManager.h"

void ResultScoreViewer::Initialize()
{
	float halfScreenWidth = Screen::Get().GetWidth() * 0.5f;
	float halfScreenHeight = Screen::Get().GetHeight() * 0.5f;

	m_batchRenderer = GetGameObject()->GetComponent<BatchRenderer>();
	m_batchRenderer->SetBitmap(L"Image/UI/RANKING_LETTER.png");
	m_batchRenderer->SetSpriteSheet(L"Image/UI/RANKING_LETTER_sprites.json");
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

void ResultScoreViewer::Start()
{
	int count = 0;

	auto continueIndexList = ConvertNumbersSmall(GameDataManager::Get().GetPlayerContinueCount());
	for (auto index : continueIndexList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_positionX, m_continuePosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}

	count = 0;
	auto killIndexList = ConvertNumbersSmall(GameDataManager::Get().GetPlayerKillCount());
	for (auto index : killIndexList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_positionX, m_killPosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}


	int time = static_cast<int>(GameDataManager::Get().GetPlayTime());

	int score = GameDataManager::Get().GetPlayerScore();
	score += 12000 - time * 10;
	if (score < 0)
	{
		score = 0;
	}

	GameDataManager::Get().SetPlayerScore(score);

	count = 0;
	auto secondIndexList = ConvertNumbersSmall(time % 60);
	for (auto index : secondIndexList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_positionX, m_timePosY),
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
				static_cast<size_t>(CharacterSpriteIndexOtherNumbers::SmallNum0),
				Vector2(-m_wordWidth * count + m_positionX, m_timePosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}

	m_batchRenderer->AddBatchUnit(
		{
			static_cast<size_t>(CharacterSpriteIndexOtherNumbers::Colon),
			Vector2(-m_wordWidth * count + m_positionX, m_timePosY),
			Vector2(1.0f, 1.0f),
			0.0f,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		}
	);

	++count;

	auto minuteIndexList = ConvertNumbersSmall(time / 60);
	for (auto index : minuteIndexList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_positionX, m_timePosY),
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
				static_cast<size_t>(CharacterSpriteIndexOtherNumbers::SmallNum0),
				Vector2(-m_wordWidth * count + m_positionX, m_timePosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);
	}


	count = 0;
	auto scoreIndexList = ConvertNumbersSmallBold(GameDataManager::Get().GetPlayerScore());
	for (auto index : scoreIndexList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count + m_positionX, m_scorePosY),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}
}