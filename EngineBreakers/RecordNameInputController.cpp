#include "../D2DEngineLib/framework.h"
#include "RecordNameInputController.h"

#include <fstream>

#include "../D2DEngineLib/json.hpp"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/BatchRenderer.h"
#include "../D2DEngineLib/Screen.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/SoundManager.h"

#include "ScreenCurtainController.h"
#include "GameDataManager.h"

using nlohmann::json;

static void to_json(json& j, const RankingData& rankingData)
{
	j = json{
		{ "Score", rankingData.score },
		{ "Name", rankingData.name }
	};
}

static void from_json(const json& j, RankingData& rankingData)
{
	j.at("Score").get_to(rankingData.score);
	j.at("Name").get_to(rankingData.name);
}

void RecordNameInputController::Initialize()
{
	float halfScreenWidth = Screen::Get().GetWidth() * 0.5f;
	float halfScreenHeight = Screen::Get().GetHeight() * 0.5f;

	m_batchRenderer = GetGameObject()->GetComponent<BatchRenderer>();
	m_batchRenderer->SetBitmap(L"Image/UI/RANKING_LOCAL.png");
	m_batchRenderer->SetSpriteSheet(L"Image/UI/RANKING_LOCAL_sprites.json");
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

void RecordNameInputController::Start()
{
	ComponentSystem::Get().PlayerInput().SetIsStopped(true);
	GameDataManager::Get().SetIsGameFinished(true);

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

	int rank = 1;
	m_playerScore = GameDataManager::Get().GetPlayerScore();

	for (const auto& data : m_rankingDatas)
	{
		if (m_playerScore > data.score)
		{
			break;
		}

		++rank;
	}

	m_rank = rank;
	GameDataManager::Get().SetPlayerRank(m_rank);

	for (int i = 0; i < m_letterCounts; ++i)
	{
		m_letterPositions.push_back(Vector2(m_nameStartPosX + m_wordWidth * i, 0.0f));
	}

	UpdateLetters();

	m_indicator = CreateGameObject(L"LetterIndicator");
	auto br = m_indicator->AddComponent<BitmapRenderer>(L"Image/UI/SELECT_BAR.png");
	br->SetSpaceType(SpaceType::Screen);
	br->SetSortOrder(1);

	m_indicator->GetTransform()->SetLocalPosition(
		GetTransform()->GetLocalPosition() + m_letterPositions[0] + m_indicatorOffset);
}

void RecordNameInputController::Update()
{
	m_continueDelayTimer += MyTime::UnscaledDeltaTime();

	if (m_screenCurtainController == nullptr)
	{
		if (Input::IsKeyPressed(VK_LEFT))
		{
			if (m_letterIndicatorIndex == 0)
			{
				m_letterIndicatorIndex = m_letterCounts - 1;
			}
			else
			{
				--m_letterIndicatorIndex;
			}
			SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);

			CharacterSpriteIndex index = ConvertLetterToIndex(m_inputtedName[m_letterIndicatorIndex]);

			m_scrollIndex = static_cast<float>(index);

			m_indicator->GetTransform()->SetLocalPosition(
				GetTransform()->GetLocalPosition() + m_letterPositions[m_letterIndicatorIndex] + m_indicatorOffset);

		}
		else if (Input::IsKeyPressed(VK_RIGHT))
		{
			if (m_letterIndicatorIndex == m_letterCounts - 1)
			{
				m_letterIndicatorIndex = 0;
			}
			else
			{
				++m_letterIndicatorIndex;
			}
			SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);

			CharacterSpriteIndex index = ConvertLetterToIndex(m_inputtedName[m_letterIndicatorIndex]);

			m_scrollIndex = static_cast<float>(index);

			m_indicator->GetTransform()->SetLocalPosition(
				GetTransform()->GetLocalPosition() + m_letterPositions[m_letterIndicatorIndex] + m_indicatorOffset);
		}

		if (Input::IsKeyPressed(VK_UP))
		{
			m_isHoldingUp = true;
			m_isHoldingDown = false;
			m_scrollStartTimer = 0.0f;

			CharacterSpriteIndex index = ConvertLetterToIndex(m_inputtedName[m_letterIndicatorIndex]);

			if (index == CharacterSpriteIndex::AlphaA)
			{
				index = CharacterSpriteIndex::Blank;
			}
			else if (index == CharacterSpriteIndex::Num0)
			{
				index = CharacterSpriteIndex::AlphaZ;
			}
			else if (index == CharacterSpriteIndex::Blank)
			{
				index = CharacterSpriteIndex::Num9;
			}
			else
			{
				index = static_cast<CharacterSpriteIndex>(static_cast<int>(index) - 1);
			}

			SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);

			m_inputtedName[m_letterIndicatorIndex] = ConvertIndexToLetter(index);

			if (index == CharacterSpriteIndex::Blank)
			{
				m_scrollIndex = 37.0f;
			}
			else
			{
				m_scrollIndex = static_cast<float>(index);
			}

			m_batchRenderer->ClearBatchUnitList();

			UpdateLetters();
		}

		if (Input::IsKeyPressed(VK_DOWN))
		{
			m_isHoldingDown = true;
			m_isHoldingUp = false;
			m_scrollStartTimer = 0.0f;

			CharacterSpriteIndex index = ConvertLetterToIndex(m_inputtedName[m_letterIndicatorIndex]);

			if (index == CharacterSpriteIndex::Blank)
			{
				index = CharacterSpriteIndex::AlphaA;
			}
			else if (index == CharacterSpriteIndex::AlphaZ)
			{
				index = CharacterSpriteIndex::Num0;
			}
			else if (index == CharacterSpriteIndex::Num9)
			{
				index = CharacterSpriteIndex::Blank;
			}
			else
			{
				index = static_cast<CharacterSpriteIndex>(static_cast<int>(index) + 1);
			}

			SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);

			m_inputtedName[m_letterIndicatorIndex] = ConvertIndexToLetter(index);

			if (index == CharacterSpriteIndex::Blank)
			{
				m_scrollIndex = 37.0f;
			}
			else
			{
				m_scrollIndex = static_cast<float>(index);
			}

			m_batchRenderer->ClearBatchUnitList();

			UpdateLetters();
		}

		if (Input::IsKeyHeld(VK_UP))
		{
			if (m_isHoldingUp)
			{
				m_scrollStartTimer += MyTime::UnscaledDeltaTime();

				if (m_scrollStartTimer >= m_scrollDelay)
				{
					m_scrollIndex -= MyTime::UnscaledDeltaTime() * 10;
					if (m_scrollIndex < 0.0f)
					{
						m_scrollIndex = 36.99f;
					}

					CharacterSpriteIndex index;

					if (m_scrollIndex >= 36.0f)
					{
						index = CharacterSpriteIndex::Blank;
					}
					else
					{
						index = static_cast<CharacterSpriteIndex>(m_scrollIndex);
					}

					if (m_prevIndex != index)
					{
						m_prevIndex = index;

						SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);
					}

					m_inputtedName[m_letterIndicatorIndex] = ConvertIndexToLetter(index);

					m_batchRenderer->ClearBatchUnitList();

					UpdateLetters();
				}
			}
		}

		if (Input::IsKeyHeld(VK_DOWN))
		{
			if (m_isHoldingDown)
			{
				m_scrollStartTimer += MyTime::UnscaledDeltaTime();

				if (m_scrollStartTimer >= m_scrollDelay)
				{
					m_scrollIndex += MyTime::UnscaledDeltaTime() * 10;
					if (m_scrollIndex > 37)
					{
						m_scrollIndex = 0.0f;
					}

					CharacterSpriteIndex index;

					if (m_scrollIndex >= 36.0f)
					{
						index = CharacterSpriteIndex::Blank;
					}
					else
					{
						index = static_cast<CharacterSpriteIndex>(m_scrollIndex);
					}

					if (m_prevIndex != index)
					{
						m_prevIndex = index;

						SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);
					}

					m_inputtedName[m_letterIndicatorIndex] = ConvertIndexToLetter(index);

					m_batchRenderer->ClearBatchUnitList();

					UpdateLetters();
				}
			}
		}

		if (m_continueDelayTimer > m_continueDelay && (Input::IsKeyPressed('X') || Input::IsKeyPressed('Z')))
		{
			SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);

			m_rankingDatas.push_back({ m_playerScore,  m_inputtedName });

			json j;

			j["RankingDatas"] = m_rankingDatas;

			std::ofstream outputFile(ResourceManager::Get().GetResourcePath() + L"Data/RankingData.json");
			if (outputFile.is_open())
			{
				outputFile << j.dump(4);
				outputFile.close();
			}

			GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
			m_screenCurtainController = screenCurtainController->AddComponent<ScreenCurtainController>(false, GameDataManager::Get().GetFadeDuration());
		}
	}
	else
	{
		if (m_screenCurtainController->IsFinished())
		{
			SceneManager::Get().ChangeScene(L"ResultRanking");
		}
	}
}

void RecordNameInputController::UpdateLetters()
{
	BatchUnit batchUnit(
		static_cast<int>(CharacterSpriteIndex::TH),
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 1.0f),
		0.0f,
		{ 1.0f, 1.0f, 1.0f, 1.0f }
	);

	if (m_rank == 1)
	{
		batchUnit.index = static_cast<int>(CharacterSpriteIndex::ST);
	}
	else if (m_rank == 2)
	{
		batchUnit.index = static_cast<int>(CharacterSpriteIndex::ND);
	}
	else if (m_rank == 3)
	{
		batchUnit.index = static_cast<int>(CharacterSpriteIndex::RD);
	}

	m_batchRenderer->AddBatchUnit(batchUnit);

	auto rankIndexList = ConvertNumbers(m_rank);

	int count = 1;
	for (auto index : rankIndexList)
	{
		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				Vector2(-(count == 1 ? 44.0f : m_wordWidth * (count - 1) + 44.0f), 0.0f),
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}

	count = 0;
	auto nameIndexList = ConvertString(m_inputtedName);
	for (auto index : nameIndexList)
	{
		if (index == CharacterSpriteIndex::Blank)
		{
			++count;
			continue;
		}

		m_batchRenderer->AddBatchUnit(
			{
				static_cast<size_t>(index),
				m_letterPositions[count],
				Vector2(1.0f, 1.0f),
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);

		++count;
	}
}
