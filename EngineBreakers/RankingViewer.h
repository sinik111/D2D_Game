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

	float m_firstRowPositionY = 250.0f;
	float m_lastRowPositionY = -330.0f;
	float m_pageViewPositionY = -350.0f;

	float m_rankColumnPositionX = -300.0f;
	float m_scoreColumnPositionX = 0.0f;
	float m_nameColumnPositionX = 100.0f;

	float m_wordWidth = 38.0f;

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