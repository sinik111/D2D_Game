#pragma once

#include "../D2DEngineLib/Script.h"

class BitmapRenderer;

class ScreenCurtainController :
	public Script
{
private:
	BitmapRenderer* m_bitmapRenderer = nullptr;

	float m_timer = 0.0f;
	float m_duration;
	float m_alphaFactor;
	bool m_fade;
	bool m_isFinished = false;

public:
	ScreenCurtainController(bool fade, float duration); // true - FadeIn, false - FadeOut

private:
	void Initialize() override;
	void Update() override;

public:
	bool IsFinished() const;
};