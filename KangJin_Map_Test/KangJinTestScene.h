#pragma once
#include "../D2DEngineLib/Scene.h"
#include "Objectimgui.h"
class Maps;
class KangJinTestScene : public Scene
{
	Maps* m_map;
public:
	void Enter() override;

	void CreateMap();
	void CreateEnemy();
	void CreateBuilding(int number, float x, float y, int id);
};
