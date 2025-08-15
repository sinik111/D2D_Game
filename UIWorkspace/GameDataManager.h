#pragma once

class GameDataManager
{
private:
	int m_selectedMenuIndex = 0;
	int m_playerScore = 0;
	int m_playerRank = 0;
	float m_fadeDuration = 0.2f;

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
	int GetSelectedMenuIndex() const;
	void SetSelectedMenuIndex(int index);

	int GetPlayerScore() const;
	void SetPlayerScore(int score);

	int GetPlayerRank() const;
	void SetPlayerRank(int rank);

	float GetFadeDuration() const;
};