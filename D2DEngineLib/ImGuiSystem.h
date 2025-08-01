#pragma once
class MyImGui;
class ImGuiSystem
{
	std::vector<MyImGui*> m_ImGuiVector;
	bool isUsingImGui = false;
public :
	static ImGuiSystem& Get();
	void EnableImGuiSystem();

	bool IsUsingImGui() { return isUsingImGui; }

	void BeginDrawImGui();
	void DrawImGui();
	void EndDrawImGui();
	
	void AddImGui(MyImGui* imgui);
};

