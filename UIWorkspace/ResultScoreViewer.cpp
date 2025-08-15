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
	m_batchRenderer->SetBitmap(L"Image/TextSpriteTest.png");
	m_batchRenderer->SetSpriteSheet(L"Data/TextSpriteTest_sprites.json");
	m_batchRenderer->SetSpaceType(SpaceType::Screen);
	m_batchRenderer->SetSortOrder(1);
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

	auto scoreIndexList = ConvertNumbers(GameDataManager::Get().GetPlayerScore());
	for (auto index : scoreIndexList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-m_wordWidth * count, 0.0f),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}
}