#pragma once

#include "../D2DEngineLib/Script.h"

#include "RankingUtility.h"

class BatchRenderer;

class InGameUIInfoController :
	public Script
{
private:
	BatchRenderer* m_batchRenderer = nullptr;

	float m_time = 0.0f;
	int m_score = 0;
	int m_death = 0;

	float m_wordWidth = 22.0f;
	float m_timeStartPosX = -695.0f;
	float m_deathStartPosX = 490.0f;
	float m_scoreStartPosX = 862.0f;
	float m_posY = 459.0f;
	float m_timerPosY = 479.0f;

	bool m_onceBgmStarted = false;

private:
	void Initialize() override;
	void Start() override;
	void Update() override;

public:
	void SetScore(int score);
	void SetDeathCount(int deathCount);
};