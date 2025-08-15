#pragma once

#include "../D2DEngineLib/Script.h"

class ScreenCurtainController;
class BitmapRenderer;

class GuideSceneController :
	public Script
{
private:
	BitmapRenderer* m_guideImageRenderer = nullptr;

private:
	ScreenCurtainController* m_screenCurtainController = nullptr;

	int m_totalGuidePage = 2;
	int m_currentGuidePage = 0;

private:
	void Start() override;
	void Update() override;
};