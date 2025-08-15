#pragma once

#include "../D2DEngineLib/Script.h"
#include "RankingUtility.h"

class BatchRenderer;
class ScreenCurtainController;

class ResultRankingViewer :
	public Script
{
private:
	BatchRenderer* m_batchRenderer = nullptr;
	ScreenCurtainController* m_screenCurtainController = nullptr;
	std::vector<RankingData> m_rankingDatas;
	std::vector<RankingRow> m_rankingRows;

	float m_firstRowPositionY = 250.0f;
	float m_lastRowPositionY = -330.0f;

	float m_rankColumnPositionX = -300.0f;
	float m_scoreColumnPositionX = 0.0f;
	float m_nameColumnPositionX = 100.0f;

	float m_wordWidth = 38.0f;

	int m_rowCount = 11;

	float m_continueDelay = 0.5f;
	float m_continueDelayTimer = 0.0f;

private:
	void Initialize() override;
	void Start() override;
	void Update() override;
};