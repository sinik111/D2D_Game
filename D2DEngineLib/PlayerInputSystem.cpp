#include "pch.h"
#include "PlayerInputSystem.h"

#include "PlayerInput.h"

PlayerInputSystem::PlayerInputSystem()
    :m_hWnd{ nullptr }, m_mousePoint{}
{

}

void PlayerInputSystem::Register(PlayerInput* playerInput)
{
    m_playerInputs.push_back(playerInput);
}

void PlayerInputSystem::Unregister(PlayerInput* playerInput)
{

}

void PlayerInputSystem::SetWindow(HWND hWnd)
{
    m_hWnd = hWnd;
}

void PlayerInputSystem::Update()
{
    m_previousKeyState = m_currentKeyState;

    for (int i = 0; i < 256; ++i)
    {
        m_currentKeyState[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }

    GetCursorPos(&m_mousePoint);

    ScreenToClient(m_hWnd, &m_mousePoint);
}

bool PlayerInputSystem::IsKeyDown(int vkey)
{
    return m_currentKeyState[vkey];
}

bool PlayerInputSystem::IsKeyPressed(int vkey)
{
    return !m_previousKeyState[vkey] && m_currentKeyState[vkey];
}

bool PlayerInputSystem::IsKeyReleased(int vkey)
{
    return m_previousKeyState[vkey] && !m_currentKeyState[vkey];
}

POINT PlayerInputSystem::GetCursorPoint()
{
    return m_mousePoint;
}

void PlayerInputSystem::ProcessInput()
{
    for (const auto& playerInput : m_playerInputs)
    {
        playerInput->SetMousePosition((float)m_mousePoint.x, (float)m_mousePoint.y);
    }
    
    ProcessArrowInput();
    ProcessWASDInput();

    for (int i = 0; i < 256; ++i)
    {
        if (IsKeyDown(i))
        {
            for (const auto& playerInput : m_playerInputs)
            {
                playerInput->CallActionOnDown(i);
            }
        }
        else if (IsKeyPressed(i))
        {
            for (const auto& playerInput : m_playerInputs)
            {
                playerInput->CallActionOnPressed(i);
            }
        }
        else if (IsKeyReleased(i))
        {
            for (const auto& playerInput : m_playerInputs)
            {
                playerInput->CallActionOnReleased(i);
            }
        }
    }
}

void PlayerInputSystem::ProcessArrowInput()
{
    bool isDown = false;
    float horizontal = 0.0f;
    float vertical = 0.0f;

    if (IsKeyDown(VK_UP) || IsKeyPressed(VK_UP))
    {
        vertical += 1.0f;

        isDown = true;
    }

    if (IsKeyDown(VK_DOWN) || IsKeyPressed(VK_DOWN))
    {
        vertical -= 1.0f;

        isDown = true;
    }

    if (IsKeyDown(VK_RIGHT) || IsKeyPressed(VK_LEFT))
    {
        horizontal += 1.0f;

        isDown = true;
    }

    if (IsKeyDown(VK_LEFT) || IsKeyPressed(VK_LEFT))
    {
        horizontal -= 1.0f;

        isDown = true;
    }

    if (isDown)
    {
        for (const auto& playerInput : m_playerInputs)
        {
            playerInput->CallArrowAction(horizontal, vertical);
        }
    }
}

void PlayerInputSystem::ProcessWASDInput()
{
    bool isDown = false;
    float horizontal = 0.0f;
    float vertical = 0.0f;

    if (IsKeyDown('W') || IsKeyPressed('W'))
    {
        vertical += 1.0f;

        isDown = true;
    }

    if (IsKeyDown('S') || IsKeyPressed('S'))
    {
        vertical -= 1.0f;

        isDown = true;
    }

    if (IsKeyDown('D') || IsKeyPressed('D'))
    {
        horizontal += 1.0f;

        isDown = true;
    }

    if (IsKeyDown('A') || IsKeyPressed('A'))
    {
        horizontal -= 1.0f;

        isDown = true;
    }

    if (isDown)
    {
        for (const auto& playerInput : m_playerInputs)
        {
            playerInput->CallWASDAction(horizontal, vertical);
        }
    }
}
