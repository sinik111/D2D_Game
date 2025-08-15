#pragma once

#include "../D2DEngineLib/Script.h"

class ScreenCurtainController;

class CreditSceneController :
	public Script
{
private:
	ScreenCurtainController* m_screenCurtainController = nullptr;

private:
	void Start() override;
	void Update() override;
};