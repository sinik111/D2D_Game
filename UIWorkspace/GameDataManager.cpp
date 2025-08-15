#include "../D2DEngineLib/framework.h"
#include "GameDataManager.h"

GameDataManager& GameDataManager::Get()
{
	static GameDataManager s_instance;

	return s_instance;
}

int GameDataManager::GetSelectedMenuIndex() const
{
	return m_selectedMenuIndex;
}

void GameDataManager::SetSelectedMenuIndex(int index)
{
	m_selectedMenuIndex = index;
}

int GameDataManager::GetPlayerScore() const
{
	return m_playerScore;
}

void GameDataManager::SetPlayerScore(int score)
{
	m_playerScore = score;
}

int GameDataManager::GetPlayerRank() const
{
	return m_playerRank;
}

void GameDataManager::SetPlayerRank(int rank)
{
	m_playerRank = rank;
}

float GameDataManager::GetFadeDuration() const
{
	return m_fadeDuration;
}
