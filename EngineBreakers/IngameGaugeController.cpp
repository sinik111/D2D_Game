#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"

#include "IngameGaugeController.h"

void IngameGaugeController::Initialize()
{
	m_bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
}

void IngameGaugeController::Update()
{
	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, m_currentGauge / m_maxGauge);
}

void IngameGaugeController::SetMax(float max)
{
	m_maxGauge = max;
	m_currentGauge = m_maxGauge;
}

void IngameGaugeController::SetCurrent(float current)
{
	m_currentGauge = current;
}

