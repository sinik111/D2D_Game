#pragma once

#include "../D2DEngineLib/Script.h"
#include "RankingUtility.h"

class BatchRenderer;
class ScreenCurtainController;

class RankingViewer :
	public Script
{
private:
	BatchRenderer* m_batchRenderer = nullptr;
	ScreenCurtainController* m_screenCurtainController = nullptr;
	std::vector<RankingData> m_rankingDatas;
	std::vector<RankingRow> m_rankingRows;

	float m_firstRowPositionY = 300.0f;
	float m_lastRowPositionY = -500.0f;

	float m_rankColumnPositionX = -700.0f;
	float m_scoreColumnPositionX = 300.0f;
	float m_nameColumnPositionX = 400.0f;

	float m_wordWidth = 24.0f;

	int m_currentPage = 0;
	int m_totalPages = 0;
	int m_totalRows = 0;
	int m_rowsPerPage = 10;
	int m_currentRows = 0;

private:
	void Initialize() override;
	void Start() override;
	void Update() override;

private:
	void LoadRankingData();
};