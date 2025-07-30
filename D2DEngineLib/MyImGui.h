#pragma once
#include <string>
#include "imgui.h"
#include "D2DRenderer.h"
class MyImGui /*: public Component*/
{
protected:
	std::string name;
	float* m_value;
public:
	virtual void DrawImgui() = 0;
	void SetName(const std::string& _name) { name = _name; }
	void SetName(const char* _name) { name = std::string{ _name }; }
	void SetValuePointer(float* _value) { m_value = _value; }
	void SetImGui(const std::string& _name, float* _value)
	{
		SetName(_name);
		SetValuePointer(_value);
	}
	void Add()
	{
		D2DRenderer::Get()->AddImGui(this);
	}
};

