#include "pch.h"
#include "PlayerInputSystem.h"

#include "PlayerInput.h"
#include "ContainerUtility.h"

void PlayerInputSystem::Register(PlayerInput* playerInput)
{
    m_playerInputs.push_back(playerInput);
}

void PlayerInputSystem::Unregister(PlayerInput* playerInput)
{
    Util::OptimizedErase(m_playerInputs, playerInput);
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

bool PlayerInputSystem::IsKeyDown(int vkey) const
{
    return m_currentKeyState[vkey];
}

bool PlayerInputSystem::IsKeyPressed(int vkey) const
{
    return !m_previousKeyState[vkey] && m_currentKeyState[vkey];
}

bool PlayerInputSystem::IsKeyReleased(int vkey) const
{
    return m_previousKeyState[vkey] && !m_currentKeyState[vkey];
}

POINT PlayerInputSystem::GetCursorPoint() const
{
    return m_mousePoint;
}

void PlayerInputSystem::ProcessInput() const
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
        
        if (IsKeyPressed(i))
        {
            for (const auto& playerInput : m_playerInputs)
            {
                playerInput->CallActionOnPressed(i);
            }
        }
        
        if (IsKeyReleased(i))
        {
            for (const auto& playerInput : m_playerInputs)
            {
                playerInput->CallActionOnReleased(i);
            }
        }
    }
}

void PlayerInputSystem::ProcessArrowInput() const
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

void PlayerInputSystem::ProcessWASDInput() const
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
