#pragma once
#include "../D2DEngineLib/Script.h"
class BitmapRenderer;
class IngameGaugeController : 
	public Script
{
	BitmapRenderer* m_bitmapRenderer = nullptr;

	int m_MaxGauge = 100;
	int m_CurrGauge;
	float m_Percentage;
private: 
	void Initialize() override;
	//void Start() override;
	void Update() override;
public :
	void SetMax(int max);
	void SetCurrent(int current);
};

