#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"

#include "IngameGaugeController.h"

void IngameGaugeController::Initialize()
{
	m_bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
	m_CurrGauge = m_MaxGauge;
}

void IngameGaugeController::Update()
{
	m_Percentage = (float)m_CurrGauge / m_MaxGauge;
	//Debug
	m_CurrGauge -= 0.01f * m_MaxGauge;

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, m_Percentage);
}

void IngameGaugeController::SetMax(int max)
{
	m_MaxGauge = max;
	m_CurrGauge = m_MaxGauge;
}

void IngameGaugeController::SetCurrent(int current)
{
	m_CurrGauge = current;
}

