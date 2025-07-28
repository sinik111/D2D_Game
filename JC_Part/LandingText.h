#pragma once

#include "../D2DEngineLib/Script.h"

class LandingText :
	public Script
{
private:
	Vector2 m_direction;

private:
	void Initialize() override;
	void Update() override;

public:
	void SetDirection(const Vector2& direction);
};