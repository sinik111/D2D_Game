#pragma once
#include "../D2DEngineLib/MyImGui.h"
class Objectimgui : public MyImGui
{
	GameObject* m_GameObject;
	
public:
	void DrawImgui() override;
	void SetValue(GameObject* object) {m_GameObject = object;}
};

