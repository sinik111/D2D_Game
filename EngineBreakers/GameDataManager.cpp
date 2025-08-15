#include "../D2DEngineLib/framework.h"
#include "GameDataManager.h"

#include "HealthGaugeController.h"
#include "StaminaGaugeController.h"
#include "InGameUIInfoController.h"

GameDataManager& GameDataManager::Get()
{
	static GameDataManager s_instance;

	return s_instance;
}

void GameDataManager::Reset()
{
	m_healthGaugeController = nullptr;
	m_staminaGaugeController = nullptr;
	m_inGameUIInfoController = nullptr;
	m_player = nullptr;

	m_selectedMenuIndex = 0;
	m_playerScore = 0;
	m_playerRank = 0;
	m_fadeDuration = 0.2f;
	m_playerContinueCount = 0;
	m_playTime = 0.0f;
	m_playerKillCount = 0;
	m_playerDeathCount = 0;
	m_inGameUISelectedIndex = 0;
	m_playerHp = 0;

	m_UIScreened = false;
	m_isGameFinished = false;
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
	m_inGameUIInfoController->SetScore(score);

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
int GameDataManager::GetPlayerContinueCount() const
{
	return m_playerContinueCount;
}

void GameDataManager::SetPlayerContinueCount(int count)
{
	m_playerContinueCount = count;
}

float GameDataManager::GetPlayTime() const
{
	return m_playTime;
}

void GameDataManager::SetPlayTime(float time)
{
	m_playTime = time;
}

int GameDataManager::GetPlayerKillCount() const
{
	return m_playerKillCount;
}

void GameDataManager::SetPlayerKillCount(int count)
{
	m_playerKillCount = count;
}

void GameDataManager::SetUIScreened(bool value)
{
	m_UIScreened = value;
}

bool GameDataManager::GetUIScreened() const
{
	return m_UIScreened;
}

void GameDataManager::SetHealthGaugeController(HealthGaugeController* healthGaugeController)
{
	m_healthGaugeController = healthGaugeController;
}

void GameDataManager::SetStaminaGaugeController(StaminaGaugeController* staminaGaugeController)
{
	m_staminaGaugeController = staminaGaugeController;
}

void GameDataManager::SetInGameUIInfoController(InGameUIInfoController* inGameUIInfoController)
{
	m_inGameUIInfoController = inGameUIInfoController;
}

void GameDataManager::SetPlayerHp(int hp)
{
	m_playerHp = hp;
	m_healthGaugeController->SetCurrent(static_cast<float>(hp));
}

int GameDataManager::GetPlayerHp() const
{
	return m_playerHp;
}

void GameDataManager::SetPlayerStamina(float stamina)
{
	m_staminaGaugeController->SetCurrent(stamina);
}

void GameDataManager::SetPlayerMaxHp(int hp)
{
	m_playerHp = hp;
	m_healthGaugeController->SetMax(static_cast<float>(hp));
}

void GameDataManager::SetPlayerMaxStamina(float stamina)
{
	m_staminaGaugeController->SetMax(stamina);
}

void GameDataManager::SetPlayer(Player* player)
{
	m_player = player;
}

Player* GameDataManager::GetPlayer() const
{
	return m_player;
}

int GameDataManager::GetPlayerDeathCount() const
{
	return m_playerDeathCount;
}

void GameDataManager::SetPlayerDeathCount(int count)
{
	m_inGameUIInfoController->SetDeathCount(count);

	m_playerDeathCount = count;
}

int GameDataManager::GetInGameUISelectedIndex() const
{
	return m_inGameUISelectedIndex;
}

void GameDataManager::SetInGameUISelectedIndex(int index)
{
	m_inGameUISelectedIndex = index;
}

bool GameDataManager::GetIsGameFinished() const
{
	return m_isGameFinished;
}

void GameDataManager::SetIsGameFinished(bool finished)
{
	m_isGameFinished = finished;
}
