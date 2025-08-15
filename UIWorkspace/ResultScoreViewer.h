#pragma once

#include "../D2DEngineLib/Script.h"

class BatchRenderer;

class ResultScoreViewer :
	public Script
{
private:
	BatchRenderer* m_batchRenderer = nullptr;

	float m_wordWidth = 24.0f;

private:
	void Initialize() override;
	void Start() override;
};