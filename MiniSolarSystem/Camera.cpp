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
    D2D1_VECTOR_2F input{};

    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        input.x -= 3.0f;
    }

    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        input.x += 3.0f;
    }

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        input.y += 3.0f;
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        input.y -= 3.0f;
    }

    m_transform->Translate(input);

    m_invertedMatrix = m_transform->GetMatrix();
    m_invertedMatrix.Invert();
}

D2D1::Matrix3x2F Camera::GetInvertedMatrix()
{
    return m_invertedMatrix;
}
