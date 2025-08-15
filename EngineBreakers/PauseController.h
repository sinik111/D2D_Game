#pragma once

#include "../D2DEngineLib/Script.h"

class ScreenCurtainController;

enum class MenuState
{
	Off,
	Main,
	Guide,
	Setting
};

class PauseController :
	public Script
{
private:
	MenuState m_currentMenuState = MenuState::Off;

	GameObject* m_pauseBackground = nullptr;
	GameObject* m_pausePanel = nullptr;
	std::vector<GameObject*> m_menuButtons;
	GameObject* m_guide = nullptr;
	std::vector<GameObject*> m_settingButtons;
	ScreenCurtainController* m_screenCurtainController = nullptr;

	GameObject* m_selectNotice = nullptr;
	GameObject* m_returnNotice = nullptr;
	GameObject* m_arrowNotice = nullptr;

	Vector2 m_settingStartPosition{ 0.0f, 100.0f };
	Vector2 m_settingEndPosition{ 0.0f, -200.0f };
	int m_menuButtonIndex = 0;
	int m_settingIndicatingIndex = 0;

private:
	void Start() override;
	void Update() override;

private:
	void CreateMenuButtons();
	void DestroyMenuButtons();
	void ClearGameObjects();
	void ClearNotices();
};