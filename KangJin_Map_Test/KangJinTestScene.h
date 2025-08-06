#pragma once
#include "../D2DEngineLib/Scene.h"
#include "Objectimgui.h"
class KangJinTestScene : public Scene
{
	Objectimgui im1;
	//Objectimgui im2;
public:
	void Enter() override;

	void CreateMap1();
	void CreatePlayer();
	void CreateBuilding1(float x, float y);
	void CreateBuilding2(int index, float x, float y);
};
