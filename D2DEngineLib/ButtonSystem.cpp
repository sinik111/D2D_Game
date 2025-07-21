#include "pch.h"
#include "ButtonSystem.h"

#include "Button.h"

void ButtonSystem::RegisterButton(Button* button)
{
	m_buttons.push_back(button);
}

void ButtonSystem::UnregisterButton(Button* button)
{
	Util::OptimizedErase(m_buttons, button);
}

void ButtonSystem::Update()
{
	if (Input::IsKeyPressed(VK_LBUTTON))
	{
		auto point = Input::GetCursorPoint();
		const Vector2 mousePoint{ point.x, point.y };

		for (const auto& button : m_buttons)
		{
			//button->IsContainPoint()
		}
	}
}