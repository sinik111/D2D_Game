#include "../D2DEngineLib/framework.h"
#include "Camera.h"

#include "../D2DEngineLib/Transform.h"

void Camera::Update()
{
    ProcessInput();
}

void Camera::Render()
{
}

void Camera::ProcessInput()
{
    float horizontal = 0.0f;
    float vertical = 0.0f;

    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        horizontal -= 3.0f;
    }

    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        horizontal += 3.0f;
    }

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        vertical += 3.0f;
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        vertical -= 3.0f;
    }

    GetTransform()->Translate(Vector2(horizontal, vertical));

    m_invertedMatrix = GetTransform()->GetWorldMatrix().Inverse();
}

Matrix3x2 Camera::GetInvertedMatrix()
{
    return m_invertedMatrix;
}
