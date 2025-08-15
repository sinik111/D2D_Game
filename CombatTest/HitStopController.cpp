#include "../D2DEngineLib/framework.h"
#include "HitStopController.h"

void HitStopController::Update()
{
	if (m_isOnHitStop)
	{
		m_hitStopTimer += MyTime::UnscaledDeltaTime();
		if (m_hitStopTimer >= m_hitStopDuration)
		{
			m_isOnHitStop = false;

			MyTime::SetTimeScale(1.0f);
		}
	}
}

void HitStopController::DoHitStop(float timeScale, float duration)
{
	MyTime::SetTimeScale(timeScale);

	m_hitStopDuration = duration;

	m_hitStopTimer = 0.0f;
	m_isOnHitStop = true;
}