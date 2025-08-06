#pragma once
#include "../D2DEngineLib/MyImGui.h"
class Objectimgui : public MyImGui
{
	GameObject* m_GameObject;
	std::string Name;
public:
	void DrawImgui() override;
	void SetValue(GameObject* object) {m_GameObject = object;}
	void SetName(const std::string& name) { Name = name; }
	std::string GetName() { return Name; }
};

