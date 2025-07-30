#include "pch.h"
#include "Float3Imgui.h"

void Float3Imgui::DrawImgui()
{
	ImGui::DragFloat3(name.c_str(), m_value, 0.0f, 10.0f);
	ImGui::NewLine();
}
