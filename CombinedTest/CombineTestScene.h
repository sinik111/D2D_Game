#pragma once
#include "../D2DEngineLib/Scene.h"
class CombineTestScene : public Scene
{
public:
	void Enter() override;
public:
	void CreateMap();
	void CreateEnemy();
	void CreateBuilding(int index, float x, float y, int id);
};

