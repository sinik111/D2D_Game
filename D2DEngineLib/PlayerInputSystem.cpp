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
    m_previousAnyKeyState = m_currentAnyKeyState;

    for (int i = 0; i < 256; ++i)
    {
        if (i == VK_HANGEUL)
        {
            continue;
        }

        m_currentKeyState[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }

    m_currentAnyKeyState = m_currentKeyState.any();

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

bool PlayerInputSystem::IsAnyKeyHeld() const
{
    return m_currentAnyKeyState;
}

bool PlayerInputSystem::IsAnyKeyPressed() const
{
    return !m_previousAnyKeyState && m_currentAnyKeyState;
}

bool PlayerInputSystem::IsAnyKeyReleased() const
{
    return m_previousAnyKeyState && !m_currentAnyKeyState;
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
    
    // UI Ã¢ ¶ç¿üÀ» ¶§ Input ¾È¹Þµµ·Ï
    if (GetIsStopped()) return;

    ProcessArrowInput();
    ProcessWASDInput();

    for (const auto& playerInput : m_playerInputs)
    {
        playerInput->CallActions();
    }
}

void PlayerInputSystem::ProcessArrowInput() const
{
    float horizontal = 0.0f;
    float vertical = 0.0f;

    if (IsKeyHeld(VK_UP))
    {
        vertical += 1.0f;
    }

    if (IsKeyHeld(VK_DOWN))
    {
        vertical -= 1.0f;
    }

    if (IsKeyHeld(VK_RIGHT))
    {
        horizontal += 1.0f;
    }

    if (IsKeyHeld(VK_LEFT))
    {
        horizontal -= 1.0f;
    }

    for (const auto& playerInput : m_playerInputs)
    {
        playerInput->CallArrowAction(horizontal, vertical);
    }
}

void PlayerInputSystem::ProcessWASDInput() const
{
    float horizontal = 0.0f;
    float vertical = 0.0f;

    if (IsKeyHeld('W'))
    {
        vertical += 1.0f;
    }

    if (IsKeyHeld('S'))
    {
        vertical -= 1.0f;
    }

    if (IsKeyHeld('D'))
    {
        horizontal += 1.0f;
    }

    if (IsKeyHeld('A'))
    {
        horizontal -= 1.0f;
    }

    for (const auto& playerInput : m_playerInputs)
    {
        playerInput->CallWASDAction(horizontal, vertical);
    }
}

void PlayerInputSystem::SetIsStopped(bool value)
{
    m_IsStopped = value;
}

bool PlayerInputSystem::GetIsStopped() const
{
    return m_IsStopped;
}
