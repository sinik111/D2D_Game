#include "../D2DEngineLib/framework.h"
#include "CameraController.h"

#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/MyTime.h"

void CameraController::Start()
{
    m_speed = 200.0f;

    PlayerInput* playerInput = GetGameObject()->AddComponent<PlayerInput>();

    auto directionAction = PlayerInput::MakeDirectionAction(&CameraController::MakeDirection, this);
    auto zoomInAction = PlayerInput::MakeAction(&CameraController::ZoomIn, this);
    auto zoomOutAction = PlayerInput::MakeAction(&CameraController::ZoomOut, this);
    auto changeScene = PlayerInput::MakeAction(&CameraController::ChangeScene, this);

    playerInput->RegisterDirectionAction(PlayerInput::DirectionInputType::Arrow, directionAction);
    playerInput->RegisterActionOnKey('Q', PlayerInput::InputCheckType::Down, zoomInAction);
    playerInput->RegisterActionOnKey('W', PlayerInput::InputCheckType::Down, zoomOutAction);
    playerInput->RegisterActionOnKey('1', PlayerInput::InputCheckType::Released, changeScene);

    GetTransform()->SetScale(3.0f, 3.0f);
}

void CameraController::Update()
{
    GetTransform()->Translate(m_direction * m_speed * MyTime::DeltaTime());

    m_direction = Vector2(0.0f, 0.0f);
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
    SceneManager::Get().ChangeScene(L"Title");
}
