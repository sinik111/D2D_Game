#pragma once
#include <string>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "ImGuiSystem.h"
class MyImGui 
{
public:
	virtual void DrawImgui() = 0;
	void FinishDrawImgui()
	{
		ImGui::End();
		ImGui::Render();
	}

	void Add()
	{
		ImGuiSystem::Get().AddImGui(this);
	}
};

