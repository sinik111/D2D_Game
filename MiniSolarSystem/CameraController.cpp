#include "../D2DEngineLib/framework.h"
#include "CameraController.h"

#include "../D2DEngineLib/Input.h"

void CameraController::Start()
{
    m_speed = 3.0f;
}

void CameraController::Update()
{
    ProcessInput();
}

void CameraController::ProcessInput()
{
    float horizontal = 0.0f;
    float vertical = 0.0f;

    if (Input::IsKeyDown(VK_LEFT))
    {
        horizontal -= m_speed;
    }

    if (Input::IsKeyDown(VK_RIGHT))
    {
        horizontal += m_speed;
    }

    if (Input::IsKeyDown(VK_UP))
    {
        vertical += m_speed;
    }

    if (Input::IsKeyDown(VK_DOWN))
    {
        vertical -= m_speed;
    }

    GetTransform()->Translate(Vector2(horizontal, vertical));
}