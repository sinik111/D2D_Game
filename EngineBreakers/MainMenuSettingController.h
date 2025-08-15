#pragma once

#include "../D2DEngineLib/Script.h"

class ScreenCurtainController;

class MainMenuSettingController :
	public Script
{
private:
	std::vector<GameObject*> m_settingItems;
	ScreenCurtainController* m_screenCurtainController = nullptr;

	int m_indicatingIndex = 0;

	Vector2 m_startPosition{ 0.0f, 150.0f };
	Vector2 m_endPosition{ 0.0f, -200.0f };

private:
	void Update() override;

public:
	void SetSettingItems(std::vector<GameObject*> settingItems);
};