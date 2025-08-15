#pragma once

#include "../D2DEngineLib/Script.h"

#include "RankingUtility.h"

class BatchRenderer;
class ScreenCurtainController;

class RecordNameInputController :
	public Script
{
private:
	BatchRenderer* m_batchRenderer = nullptr;
	ScreenCurtainController* m_screenCurtainController = nullptr;
	GameObject* m_indicator = nullptr;
	std::vector<RankingData> m_rankingDatas;

	const int m_letterCounts = 8;
	std::string m_inputtedName{ "AAAAAAAA" };
	std::vector<Vector2> m_letterPositions;
	Vector2 m_indicatorOffset{ 0.0f, -27.0f };
	int m_letterIndicatorIndex = 0;
	int m_rank = 0;
	int m_playerScore = 0;
	float m_wordWidth = 38.0f;
	float m_nameStartPosX = 70.0f;

	float m_scrollDelay = 0.5f;
	float m_scrollStartTimer = 0.0f;
	float m_scrollIndex = static_cast<float>(CharacterSpriteIndex::AlphaA);
	
	CharacterSpriteIndex m_prevIndex = CharacterSpriteIndex::AlphaA;

	float m_continueDelay = 2.0f;
	float m_continueDelayTimer = 0.0f;

	bool m_isHoldingUp = false;
	bool m_isHoldingDown = false;

private:
	void Initialize() override;
	void Start() override;
	void Update() override;

	void UpdateLetters();
};