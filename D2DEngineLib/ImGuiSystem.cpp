#include "pch.h"
#include "MyImGui.h"
#include "ImGuiSystem.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

ImGuiSystem& ImGuiSystem::Get()
{
    // TODO: 여기에 return 문을 삽입합니다.
    static ImGuiSystem s_Instance;

    return s_Instance;
}

void ImGuiSystem::EnableImGuiSystem()
{
	isUsingImGui = true;
}

void ImGuiSystem::DisableImGuiSystem()
{
	isUsingImGui = false;
}

void ImGuiSystem::BeginDrawImGui()
{
	if(isUsingImGui == true)
	{
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX11_NewFrame();
		ImGui::NewFrame();
	}
}

void ImGuiSystem::DrawImGui()
{
	if(isUsingImGui == true)
	{
		for (auto& it : m_ImGuiVector)
		{
			it->DrawImgui();
			it->FinishDrawImgui();
		}
	}
}

void ImGuiSystem::EndDrawImGui()
{
	if (isUsingImGui == true)
	{ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());}
}

void ImGuiSystem::AddImGui(MyImGui* imgui)
{
	if (isUsingImGui == true) {m_ImGuiVector.push_back(imgui);}
}
