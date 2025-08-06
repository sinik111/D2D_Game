#pragma once

#include "../D2DEngineLib/Script.h"

class HitStopController :
	public Script
{
private:
	float m_hitStopTimer = 0.0f;
	float m_hitStopDuration = 0.0f;
	bool m_isOnHitStop = false;

private:
	void Update() override;

public:
	void DoHitStop(float timeScale, float duration);
};