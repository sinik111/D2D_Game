#pragma once
#include "../D2DEngineLib/Scene.h"
#include "Objectimgui.h"
class KangJinTestScene : public Scene
{
	Objectimgui imgui;
public:
	void Enter() override;
};

