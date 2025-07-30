#include "pch.h"
#include "Float2Imgui.h"
void Float2Imgui::DrawImgui()
{
	ImGui::SliderFloat2(name.c_str(), m_value, 0, 10);
	ImGui::NewLine();
}
