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

    for (int i{ 0 }; i < 256; ++i)
    {
        m_currentKeyState[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }

    GetCursorPos(&m_mousePoint);

    ScreenToClient(m_hWnd, &m_mousePoint);
}

bool PlayerInputSystem::IsKeyHeld(int vkey) const
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

    for (const auto& playerInput : m_playerInputs)
    {
        playerInput->CallActions();
    }
}

void PlayerInputSystem::ProcessArrowInput() const
{
    bool isHeld{ false };
    float horizontal{ 0.0f };
    float vertical{ 0.0f };

    if (IsKeyHeld(VK_UP))
    {
        vertical += 1.0f;

        isHeld = true;
    }

    if (IsKeyHeld(VK_DOWN))
    {
        vertical -= 1.0f;

        isHeld = true;
    }

    if (IsKeyHeld(VK_RIGHT))
    {
        horizontal += 1.0f;

        isHeld = true;
    }

    if (IsKeyHeld(VK_LEFT))
    {
        horizontal -= 1.0f;

        isHeld = true;
    }

    if (isHeld && (vertical != 0.0f || horizontal != 0.0f))
    {
        for (const auto& playerInput : m_playerInputs)
        {
            playerInput->CallArrowAction(horizontal, vertical);
        }
    }
}

void PlayerInputSystem::ProcessWASDInput() const
{
    bool isHeld{ false };
    float horizontal{ 0.0f };
    float vertical{ 0.0f };

    if (IsKeyHeld('W'))
    {
        vertical += 1.0f;

        isHeld = true;
    }

    if (IsKeyHeld('S'))
    {
        vertical -= 1.0f;

        isHeld = true;
    }

    if (IsKeyHeld('D'))
    {
        horizontal += 1.0f;

        isHeld = true;
    }

    if (IsKeyHeld('A'))
    {
        horizontal -= 1.0f;

        isHeld = true;
    }

    if (isHeld && (vertical != 0.0f || horizontal != 0.0f))
    {
        for (const auto& playerInput : m_playerInputs)
        {
            playerInput->CallWASDAction(horizontal, vertical);
        }
    }
}
