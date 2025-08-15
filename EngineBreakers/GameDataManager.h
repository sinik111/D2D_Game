#pragma once

class HealthGaugeController;
class StaminaGaugeController;
class Player;
class InGameUIInfoController;

class GameDataManager
{
private:
	HealthGaugeController* m_healthGaugeController = nullptr;
	StaminaGaugeController* m_staminaGaugeController = nullptr;
	InGameUIInfoController* m_inGameUIInfoController = nullptr;
	Player* m_player = nullptr;

	int m_selectedMenuIndex = 0;
	int m_playerScore = 0;
	int m_playerRank = 0;
	float m_fadeDuration = 0.2f;
	int m_playerContinueCount = 0;
	float m_playTime = 0.0f;
	int m_playerKillCount = 0;
	int m_playerDeathCount = 0;
	int m_inGameUISelectedIndex = 0;
	int m_playerHp = 0;

	bool m_UIScreened = false;
	bool m_isGameFinished = false;

private:
	GameDataManager() = default;
	GameDataManager(const GameDataManager&) = delete;
	GameDataManager& operator=(const GameDataManager&) = delete;
	GameDataManager(GameDataManager&&) = delete;
	GameDataManager& operator=(GameDataManager&&) = delete;
	~GameDataManager() = default;

public:
	static GameDataManager& Get();

public:
	void Reset();

	int GetSelectedMenuIndex() const;
	void SetSelectedMenuIndex(int index);

	int GetPlayerScore() const;
	void SetPlayerScore(int score);

	int GetPlayerRank() const;
	void SetPlayerRank(int rank);

	float GetFadeDuration() const;

	int GetPlayerContinueCount() const;
	void SetPlayerContinueCount(int count);

	float GetPlayTime() const;
	void SetPlayTime(float time);

	int GetPlayerKillCount() const;
	void SetPlayerKillCount(int count);
	void SetUIScreened(bool value);
	bool GetUIScreened() const ;

	void SetHealthGaugeController(HealthGaugeController* healthGaugeController);
	void SetStaminaGaugeController(StaminaGaugeController* staminaGaugeController);
	void SetInGameUIInfoController(InGameUIInfoController* inGameUIInfoController);

	void SetPlayerHp(int hp);
	int GetPlayerHp() const;
	void SetPlayerStamina(float stamina);
	void SetPlayerMaxHp(int hp);
	void SetPlayerMaxStamina(float stamina);
	void SetPlayer(Player* player);

	Player* GetPlayer() const;

	int GetPlayerDeathCount() const;
	void SetPlayerDeathCount(int count);

	int GetInGameUISelectedIndex() const;
	void SetInGameUISelectedIndex(int index);

	bool GetIsGameFinished() const;
	void SetIsGameFinished(bool finished);
};