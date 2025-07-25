#include "../D2DEngineLib/framework.h"
#include "CameraController.h"

#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/SceneManager.h"

void CameraController::Start()
{
    m_speed = 200.0f;

    PlayerInput* playerInput = GetGameObject()->AddComponent<PlayerInput>();

    playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &CameraController::MakeDirection);
    playerInput->RegisterActionOnCombinedKey({ VK_CONTROL, 'Q' }, KeyState::Held, this, &CameraController::ZoomIn);
    playerInput->RegisterActionOnCombinedKey({ VK_CONTROL, 'W' }, KeyState::Held, this, &CameraController::ZoomOut);
    playerInput->RegisterActionOnKey('3', KeyState::Pressed, this, &CameraController::ChangeScene);

    GetTransform()->SetLocalScale({ 3.0f, 3.0f });
}

void CameraController::Update()
{
    GetTransform()->Translate(m_direction * m_speed * MyTime::DeltaTime());
}

void CameraController::MakeDirection(Vector2 input)
{
    m_direction = input;
}

void CameraController::ZoomIn()
{
    float zoom = Camera::s_mainCamera->GetZoom();

    zoom -= 1.0f * MyTime::DeltaTime();

    Camera::s_mainCamera->SetZoom(zoom);
}

void CameraController::ZoomOut()
{
    float zoom = Camera::s_mainCamera->GetZoom();

    zoom += 1.0f * MyTime::DeltaTime();

    Camera::s_mainCamera->SetZoom(zoom);
}

void CameraController::ChangeScene()
{
    SceneManager::Get().ChangeScene(L"AnimationTest");
}
