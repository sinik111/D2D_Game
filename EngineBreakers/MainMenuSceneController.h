#pragma once

#include "../D2DEngineLib/Script.h"

class ScreenCurtainController;

class MainMenuSceneController :
	public Script
{
private:
	ScreenCurtainController* m_screenCurtainController = nullptr;

	std::vector<GameObject*> m_menuButtons;
	std::wstring m_nextSceneName;

	int m_indicatingIndex = 0;

	float m_startY = 50.0f;
	float m_endY = -400.0f;
	float m_selectedX = -960.0f;
	float m_normalX = -700.0f;

private:
	void Update() override;

public:
	void SetMenuButtons(const std::vector<GameObject*>& menuButtons);
};