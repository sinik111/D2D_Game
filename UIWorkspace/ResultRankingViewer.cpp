#include "../D2DEngineLib/framework.h"
#include "ResultRankingViewer.h"

#include <fstream>

#include "../D2DEngineLib/json.hpp"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/BatchRenderer.h"
#include "../D2DEngineLib/Screen.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/BitmapRenderer.h"

#include "ScreenCurtainController.h"
#include "GameDataManager.h"

using nlohmann::json;

static void from_json(const json& j, RankingData& rankingData)
{
	j.at("Score").get_to(rankingData.score);
	j.at("Name").get_to(rankingData.name);
}

void ResultRankingViewer::Initialize()
{
	float halfScreenWidth = Screen::Get().GetWidth() * 0.5f;
	float halfScreenHeight = Screen::Get().GetHeight() * 0.5f;

	m_batchRenderer = GetGameObject()->GetComponent<BatchRenderer>();
	m_batchRenderer->SetBitmap(L"Image/TextSpriteTest.png");
	m_batchRenderer->SetSpriteSheet(L"Data/TextSpriteTest_sprites.json");
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

void ResultRankingViewer::Start()
{
	std::ifstream inFile(ResourceManager::Get().GetResourcePath() + L"Data/RankingData.json");
	if (inFile.is_open())
	{
		json j;
		inFile >> j;
		inFile.close();

		m_rankingDatas = j["RankingDatas"].get<std::vector<RankingData>>();
	}
	else
	{
		assert(false && L"json 파일 열기 실패");
	}

	std::sort(
		m_rankingDatas.begin(),
		m_rankingDatas.end(),
		[](const RankingData& a, const RankingData& b) {
			return a.score > b.score;
		}
	);

	int playerRank = GameDataManager::Get().GetPlayerRank();

	for (int i = 0; i < m_rowCount - 1; ++i)
	{
		float rowRatio = static_cast<float>(i) / m_rowCount;
		float posY = MyMath::Lerp(m_firstRowPositionY, m_lastRowPositionY, rowRatio);

		int rank = i + 1;

		if (playerRank == rank)
		{
			auto highlighter = CreateGameObject(L"Highlighter");
			highlighter->GetTransform()->SetLocalPosition({ 0.0f, posY });
			auto br = highlighter->AddComponent<BitmapRenderer>(L"Image/RankingHighlighter.bmp");
			br->SetSpaceType(SpaceType::Screen);
			br->SetSortOrder(1);
			br->SetColor({ 1.0f, 0.9f, 0.9f, 0.3f });
		}

		BatchUnit batchUnit(
			static_cast<int>(CharacterSpriteIndex::TH),
			Vector2(m_rankColumnPositionX, posY),
			Vector2(1.0f, 1.0f),
			0.0f,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);

		if (rank == 1)
		{
			batchUnit.index = static_cast<int>(CharacterSpriteIndex::ST);
		}
		else if (rank == 2)
		{
			batchUnit.index = static_cast<int>(CharacterSpriteIndex::ND);
		}
		else if (rank == 3)
		{
			batchUnit.index = static_cast<int>(CharacterSpriteIndex::RD);
		}

		m_batchRenderer->AddBatchUnit(batchUnit);

		auto rankIndexList = ConvertNumbers(rank);

		int count = 1;
		for (auto index : rankIndexList)
		{
			m_batchRenderer->AddBatchUnit(
				{
					static_cast<size_t>(index),
					Vector2(m_rankColumnPositionX - m_wordWidth * count, posY),
					Vector2(1.0f, 1.0f),
					0.0f,
					{ 1.0f, 1.0f, 1.0f, 1.0f }
				}
			);

			++count;
		}

		count = 0;
		auto scoreIndexList = ConvertNumbers(m_rankingDatas[i].score);
		for (auto index : scoreIndexList)
		{
			m_batchRenderer->AddBatchUnit(
				{
					static_cast<size_t>(index),
					Vector2(m_scoreColumnPositionX - m_wordWidth * count, posY),
					Vector2(1.0f, 1.0f),
					0.0f,
					{ 1.0f, 1.0f, 1.0f, 1.0f }
				}
			);

			++count;
		}

		count = 0;
		auto nameIndexList = ConvertString(m_rankingDatas[i].name);
		for (auto index : nameIndexList)
		{
			m_batchRenderer->AddBatchUnit(
				{
					static_cast<size_t>(index),
					Vector2(m_nameColumnPositionX + m_wordWidth * count, posY),
					Vector2(1.0f, 1.0f),
					0.0f,
					{ 1.0f, 1.0f, 1.0f, 1.0f }
				}
			);

			++count;
		}
	}

	if (playerRank > m_rowCount - 1)
	{
		float rowRatio = static_cast<float>(m_rowCount - 1) / m_rowCount;
		float posY = MyMath::Lerp(m_firstRowPositionY, m_lastRowPositionY, rowRatio);

		int rank = playerRank;

		auto highlighter = CreateGameObject(L"Highlighter");
		highlighter->GetTransform()->SetLocalPosition({ 0.0f, posY });
		auto br = highlighter->AddComponent<BitmapRenderer>(L"Image/RankingHighlighter.bmp");
		br->SetSpaceType(SpaceType::Screen);
		br->SetSortOrder(1);
		br->SetColor({ 1.0f, 0.9f, 0.9f, 0.3f });

		BatchUnit batchUnit(
			static_cast<int>(CharacterSpriteIndex::TH),
			Vector2(m_rankColumnPositionX, posY),
			Vector2(1.0f, 1.0f),
			0.0f,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);

		m_batchRenderer->AddBatchUnit(batchUnit);

		auto rankIndexList = ConvertNumbers(rank);

		int count = 1;
		for (auto index : rankIndexList)
		{
			m_batchRenderer->AddBatchUnit(
				{
					static_cast<size_t>(index),
					Vector2(m_rankColumnPositionX - m_wordWidth * count, posY),
					Vector2(1.0f, 1.0f),
					0.0f,
					{ 1.0f, 1.0f, 1.0f, 1.0f }
				}
			);

			++count;
		}

		count = 0;
		auto scoreIndexList = ConvertNumbers(m_rankingDatas[rank - 1].score);
		for (auto index : scoreIndexList)
		{
			m_batchRenderer->AddBatchUnit(
				{
					static_cast<size_t>(index),
					Vector2(m_scoreColumnPositionX - m_wordWidth * count, posY),
					Vector2(1.0f, 1.0f),
					0.0f,
					{ 1.0f, 1.0f, 1.0f, 1.0f }
				}
			);

			++count;
		}

		count = 0;
		auto nameIndexList = ConvertString(m_rankingDatas[rank - 1].name);
		for (auto index : nameIndexList)
		{
			m_batchRenderer->AddBatchUnit(
				{
					static_cast<size_t>(index),
					Vector2(m_nameColumnPositionX + m_wordWidth * count, posY),
					Vector2(1.0f, 1.0f),
					0.0f,
					{ 1.0f, 1.0f, 1.0f, 1.0f }
				}
			);

			++count;
		}
	}
}

void ResultRankingViewer::Update()
{
	if (m_screenCurtainController == nullptr)
	{
		if (Input::IsKeyPressed('Z') || Input::IsKeyPressed('X'))
		{
			GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
			m_screenCurtainController = screenCurtainController->AddComponent<ScreenCurtainController>(false, GameDataManager::Get().GetFadeDuration());
		}
	}
	else
	{
		if (m_screenCurtainController->IsFinished())
		{
			SceneManager::Get().ChangeScene(L"MainTitle");
		}
	}
}