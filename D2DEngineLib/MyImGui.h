#pragma once
#include <string>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "ImGuiSystem.h"
#include "GameObject.h"

class MyImGui 
{
protected:
	std::vector<GameObject*> m_GameObjectlist;
	GameObject* m_GameObject;
	std::string Name;
public:
	virtual void DrawImgui() {}

	void SetObject(GameObject* object) 
	{
		if(ImGuiSystem::Get().IsUsingImGui())
		{
			m_GameObject = object;
			m_GameObjectlist.push_back(object);
		}
	}
	void SetName(const std::string name) 
	{ 
		if (ImGuiSystem::Get().IsUsingImGui())
		{
			Name = name;
		}
	}
	std::string GetName() { return Name; }
	void FinishDrawImgui()
	{
		ImGui::Render();
	}
};

