#pragma once

#include "../D2DEngineLib/Script.h"

class BatchRenderer;

class ResultScoreViewer :
	public Script
{
private:
	BatchRenderer* m_batchRenderer = nullptr;

	float m_wordWidth = 30.0f;
	float m_positionX = 230.0f;
	float m_continuePosY = 100.0f;
	float m_killPosY = 45.0f;
	float m_timePosY = -8.0f;
	float m_scorePosY = -80.0f;

private:
	void Initialize() override;
	void Start() override;
};