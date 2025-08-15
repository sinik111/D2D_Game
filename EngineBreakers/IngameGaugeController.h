#pragma once

#include "../D2DEngineLib/Script.h"

class BitmapRenderer;

class IngameGaugeController : 
	public Script
{
private:
	BitmapRenderer* m_bitmapRenderer = nullptr;

	float m_maxGauge = 0.0f;
	float m_currentGauge = 0.0f;

private: 
	void Initialize() override;
	void Update() override;

public :
	void SetMax(float max);
	void SetCurrent(float current);
};

