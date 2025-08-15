#pragma once

#include "../D2DEngineLib/Script.h"

class BatchRenderer;
class ScreenCurtainController;
class Player;

class GameOverController :
	public Script
{
private:
	GameObject* m_background1 = nullptr;
	GameObject* m_background2 = nullptr;
	GameObject* m_continueNotice = nullptr;
	GameObject* m_giveupNotice = nullptr;
	BatchRenderer* m_batchRenderer = nullptr;

	ScreenCurtainController* m_screenCurtainController = nullptr;
	Player* m_player = nullptr;

	float m_duration = 21.99f;

private:
	void Start() override;
	void Update() override;

	void CleanupObjects();

public:
	void SetPlayer(Player* player);
};