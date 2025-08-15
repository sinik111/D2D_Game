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

	float m_firstRowPositionY = 350.0f;
	float m_lastRowPositionY = -450.0f;

	float m_rankColumnPositionX = -700.0f;
	float m_scoreColumnPositionX = 300.0f;
	float m_nameColumnPositionX = 400.0f;

	float m_wordWidth = 24.0f;

	int m_rowCount = 11;

private:
	void Initialize() override;
	void Start() override;
	void Update() override;
};