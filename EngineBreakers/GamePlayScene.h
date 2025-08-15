#pragma once
#include "../D2DEngineLib/Scene.h"
class Maps;
class Player;
class GamePlayScene
	: public Scene
{
	Maps* m_map = nullptr;
	GameObject* m_Player = nullptr;
public:
	void Enter() override;
	
	void CreateMap();
	void CreateBuilding(int number, float x, float y, int id);
	void CreateInGameUI();

	void LoadSound();
};

