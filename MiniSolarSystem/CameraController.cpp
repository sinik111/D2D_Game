#include "../D2DEngineLib/framework.h"
#include "CameraController.h"

#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/MyTime.h"

using DirInputType = PlayerInput::DirectionInputType;
using InputType = PlayerInput::PlayerInput::InputCheckType;

void CameraController::Start()
{
    m_speed = 200.0f;

    PlayerInput* playerInput{ GetGameObject()->AddComponent<PlayerInput>() };

    playerInput->RegisterDirectionAction(DirInputType::Arrow, this, &CameraController::MakeDirection);
    playerInput->RegisterActionOnCombinedKey({ VK_CONTROL, 'Q' }, InputType::Held, this, &CameraController::ZoomIn);
    playerInput->RegisterActionOnCombinedKey({ VK_CONTROL, 'W' }, InputType::Held, this, &CameraController::ZoomOut);
    playerInput->RegisterActionOnKey('1', InputType::Released, this, &CameraController::ChangeScene);

    GetTransform()->SetScale({ 3.0f, 3.0f });
}

void CameraController::Update()
{
    GetTransform()->Translate(m_direction * m_speed * MyTime::DeltaTime());

    m_direction = Vector2::Zero;
}

void CameraController::MakeDirection(Vector2 input)
{
    m_direction = input;
}

void CameraController::ZoomIn()
{
    float zoom{ Camera::s_mainCamera->GetZoom() };

    zoom -= 1.0f * MyTime::DeltaTime();

    Camera::s_mainCamera->SetZoom(zoom);
}

void CameraController::ZoomOut()
{
    float zoom{ Camera::s_mainCamera->GetZoom() };

    zoom += 1.0f * MyTime::DeltaTime();

    Camera::s_mainCamera->SetZoom(zoom);
}

void CameraController::ChangeScene()
{
    SceneManager::Get().ChangeScene(L"Title");
}
