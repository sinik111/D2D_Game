#include "../D2DEngineLib/framework.h"
#include "CameraController.h"

#include "../D2DEngineLib/PlayerInput.h"

#include "../D2DEngineLib/Camera.h"

void CameraController::Start()
{
    m_speed = 3.0f;

    PlayerInput* playerInput = GetGameObject()->AddComponent<PlayerInput>();

    auto directionAction = PlayerInput::MakeDirectionAction(&CameraController::MakeDirection, this);
    auto zoomInAction = PlayerInput::MakeAction(&CameraController::ZoomIn, this);
    auto zoomOutAction = PlayerInput::MakeAction(&CameraController::ZoomOut, this);

    playerInput->RegisterDirectionAction(PlayerInput::DirectionInputType::Arrow, directionAction);
    playerInput->RegisterActionOnKey('Q', PlayerInput::InputCheckType::Down, zoomInAction);
    playerInput->RegisterActionOnKey('W', PlayerInput::InputCheckType::Down, zoomOutAction);

    GetTransform()->SetScale(3.0f, 3.0f);
}

void CameraController::Update()
{
    GetTransform()->Translate(m_direction * m_speed);

    m_direction = Vector2(0.0f, 0.0f);
}

void CameraController::MakeDirection(Vector2 input)
{
    m_direction = input;
}

void CameraController::ZoomIn()
{
    float zoom = Camera::s_mainCamera->GetZoom();

    zoom -= 0.01f;

    Camera::s_mainCamera->SetZoom(zoom);
}

void CameraController::ZoomOut()
{
    float zoom = Camera::s_mainCamera->GetZoom();

    zoom += 0.01f;

    Camera::s_mainCamera->SetZoom(zoom);
}
