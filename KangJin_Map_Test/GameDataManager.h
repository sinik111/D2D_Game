#pragma once
class GameDataManager
{
	int m_Score = 0;
	int m_Death = 0;
public:
	static GameDataManager& Get();

	void AddScore(int value) { m_Score += value; }
	int GetScore() { return m_Score; }

	void AddDeath() { m_Death++; }
	int GetDeath() { return m_Death; }
};

