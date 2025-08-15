#pragma once

#include "../D2DEngineLib/Script.h"

class ScreenCurtainController;

class MainMenuSceneController :
	public Script
{
private:
	ScreenCurtainController* m_screenCurtainController = nullptr;

	std::vector<GameObject*> m_menuButtons;
	GameObject* m_indicator = nullptr;
	std::wstring m_nextSceneName;

	int m_indicatingIndex = 0;

	Vector2 m_startPosition{ -830.0f, 100.0f };
	Vector2 m_endPosition{ -830.0f, -400.0f };

private:
	void Start() override;
	void Update() override;

public:
	void SetMenuButtons(const std::vector<GameObject*>& menuButtons);
	void SetIndicator(GameObject* indicator);
};