#include "../D2DEngineLib/framework.h"
#include "ScreenCurtainController.h"

#include "../D2DEngineLib/BitmapRenderer.h"

ScreenCurtainController::ScreenCurtainController(bool fade, float duration)
	: m_fade{ fade }, m_duration{ duration <= 0.0f ? 0.001f : duration }, m_alphaFactor{ fade ? 1.0f : 0.0f }
{
	
}

void ScreenCurtainController::Initialize()
{
	m_bitmapRenderer = GetGameObject()->AddComponent<BitmapRenderer>(L"Image/ScreenCurtain.bmp");
	m_bitmapRenderer->SetColor({ 1.0f, 1.0f, 1.0f, m_alphaFactor });
	m_bitmapRenderer->SetSpaceType(SpaceType::Screen);
	m_bitmapRenderer->SetSortOrder(20);
}

void ScreenCurtainController::Update()
{
	if (m_fade && m_isFinished)
	{
		Destroy(GetGameObject());

		return;
	}

	if (MyTime::UnscaledDeltaTime() < 0.1f)
	{
		m_timer += MyTime::UnscaledDeltaTime();
		if (m_timer >= m_duration)
		{
			m_timer = m_duration;
			m_isFinished = true;
		}
	}

	if (m_fade)
	{
		m_alphaFactor = 1.0f - m_timer / m_duration;
	}
	else
	{
		m_alphaFactor = m_timer / m_duration;
	}

	m_bitmapRenderer->SetColor({ 1.0f, 1.0f, 1.0f, m_alphaFactor });

}

bool ScreenCurtainController::IsFinished() const
{
	return m_isFinished;
}
