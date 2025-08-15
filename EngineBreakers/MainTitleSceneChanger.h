#pragma once

#include "../D2DEngineLib/Script.h"

class ScreenCurtainController;

class MainTitleSceneChanger :
	public Script
{
private:
	ScreenCurtainController* m_screenCurtainController = nullptr;

private:
	void Update() override;
};