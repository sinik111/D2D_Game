#include "../D2DEngineLib/framework.h"
#include "CameraController.h"

#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/SceneManager.h"
#include <iostream>

void CameraController::Start()
{
	m_speed = 200.0f;

	PlayerInput* playerInput = GetGameObject()->AddComponent<PlayerInput>();

	playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &CameraController::MakeDirection);
	playerInput->RegisterActionOnCombinedKey({ VK_CONTROL, 'Q' }, KeyState::Held, this, &CameraController::ZoomIn);
	playerInput->RegisterActionOnCombinedKey({ VK_CONTROL, 'W' }, KeyState::Held, this, &CameraController::ZoomOut);

	GetTransform()->SetLocalScale({ 1.0f, 1.0f });
}

void CameraController::Update()
{
	//마우스 우클릭에 따른 카메라 이동입니다.
	if (Input::IsKeyPressed(VK_MBUTTON))
	{
		prevPos = { -(float)Input::GetCursorPoint().x, (float)Input::GetCursorPoint().y };
	}
	if (Input::IsKeyHeld(VK_MBUTTON))
	{
		Vector2 currPos = { -(float)Input::GetCursorPoint().x , (float)Input::GetCursorPoint().y };
		Vector2 movement = currPos - prevPos;
		GetTransform()->Translate(zoom*movement * m_speed * MyTime::DeltaTime());
		prevPos = currPos;
	}
	GetTransform()->Translate(m_direction * m_speed * MyTime::DeltaTime());
}

void CameraController::MakeDirection(Vector2 input)
{
	m_direction = input;
}

void CameraController::ZoomIn()
{
	zoom = Camera::s_mainCamera->GetZoom();

	zoom -= 1.0f * MyTime::DeltaTime();

	Camera::s_mainCamera->SetZoom(zoom);
}

void CameraController::ZoomOut()
{
	zoom = Camera::s_mainCamera->GetZoom();

	zoom += 1.0f * MyTime::DeltaTime();

	Camera::s_mainCamera->SetZoom(zoom);
}

void CameraController::ChangeScene()
{
	SceneManager::Get().ChangeScene(L"AnimationTest");
}
