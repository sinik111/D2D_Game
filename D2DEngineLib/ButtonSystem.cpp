#include "pch.h"
#include "ButtonSystem.h"

#include "Button.h"
#include "Camera.h"
#include "Screen.h"

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
		const POINT point = Input::GetCursorPoint();
		const Vector2 mousePoint{ static_cast<float>(point.x), static_cast<float>(point.y) };

		const Matrix3x2 screenMatrix = Matrix3x2::Translation(-Screen::Get().GetWidth() / 2.0f, -Screen::Get().GetHeight() / 2.0f);
		const Matrix3x2 worldMatrix = Camera::s_mainCamera->GetViewMatrix() * screenMatrix;

		const Vector2 screenMousePoint = screenMatrix.TransformPoint(mousePoint);
		const Vector2 worldMousePoint = worldMatrix.TransformPoint(mousePoint);
		
		for (const auto& button : m_buttons)
		{
			const SpaceType type = button->GetSpaceType();

			if (type == SpaceType::Screen)
			{
				if (button->IsContainPoint(screenMousePoint))
				{
					m_pressedButton = button;

					m_pressedButton->SetClickedColor({ 0.9f, 0.9f, 0.9f, 1.0f });
				}
			}
			else
			{
				if (button->IsContainPoint(worldMousePoint))
				{
					m_pressedButton = button;

					m_pressedButton->SetClickedColor({ 0.9f, 0.9f, 0.9f, 1.0f });
				}
			}
		}
	}

	if (Input::IsKeyReleased(VK_LBUTTON))
	{
		if (m_pressedButton != nullptr)
		{
			const POINT point = Input::GetCursorPoint();
			const Vector2 mousePoint{ static_cast<float>(point.x), static_cast<float>(point.y) };
		
			const Matrix3x2 screenMatrix = Matrix3x2::Translation(-Screen::Get().GetWidth() / 2.0f, -Screen::Get().GetHeight() / 2.0f);
			const Matrix3x2 worldMatrix = Camera::s_mainCamera->GetViewMatrix() * screenMatrix;
		
			const Vector2 screenMousePoint = screenMatrix.TransformPoint(mousePoint);
			const Vector2 worldMousePoint = worldMatrix.TransformPoint(mousePoint);

			const SpaceType type = m_pressedButton->GetSpaceType();

			if (type == SpaceType::Screen)
			{
				if (m_pressedButton->IsContainPoint(screenMousePoint))
				{
					m_pressedButton->InvokeAction();
				}
			}
			else
			{
				if (m_pressedButton->IsContainPoint(worldMousePoint))
				{
					m_pressedButton->InvokeAction();
				}
			}

			m_pressedButton->ResetColor();

			m_pressedButton = nullptr;
		}
	}
}