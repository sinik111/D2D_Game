#include "../D2DEngineLib/framework.h"
#include "CameraController.h"

#include "../D2DEngineLib/Transform.h"

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

    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        horizontal -= m_speed;
    }

    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        horizontal += m_speed;
    }

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        vertical += m_speed;
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        vertical -= m_speed;
    }

    GetTransform()->Translate(Vector2(horizontal, vertical));
}