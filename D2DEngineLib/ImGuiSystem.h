#pragma once
class MyImGui;
class ImGuiSystem
{
	std::vector<std::unique_ptr<MyImGui>> m_ImGuiVector;
	bool isUsingImGui = false;
public :
	static ImGuiSystem& Get();
	void EnableImGuiSystem();
	void DisableImGuiSystem();
	bool IsUsingImGui() { return isUsingImGui; }

	void BeginDrawImGui();
	void DrawImGui();
	void EndDrawImGui();
	
	template<typename T>
	T* CreateImGui()
	{
		if (isUsingImGui)
		{
			m_ImGuiVector.push_back(std::make_unique<T>());
			return (T*)m_ImGuiVector.back().get();
		}
		return nullptr;
	}
};

