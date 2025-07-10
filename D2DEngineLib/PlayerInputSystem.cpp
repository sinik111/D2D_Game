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
    m_pendingUnregisteredPlayerInputs.push_back(playerInput);
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

void PlayerInputSystem::ProcessInput()
{
    for (const auto& playerInput : m_playerInputs)
    {
        playerInput->SetMousePosition((float)m_mousePoint.x, (float)m_mousePoint.y);
    }
    
    ProcessArrowInput();
    ProcessWASDInput();

    for (size_t i = 0; i < m_playerInputs.size(); ++i)
    {
        m_playerInputs[i]->CallActions();
    }

    if (!m_pendingUnregisteredPlayerInputs.empty())
    {
        Util::OptimizedErase(m_playerInputs, m_pendingUnregisteredPlayerInputs);
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

    for (size_t i = 0; i < m_playerInputs.size(); ++i)
    {
        m_playerInputs[i]->CallArrowAction(horizontal, vertical);
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

    for (size_t i = 0; i < m_playerInputs.size(); ++i)
    {
        m_playerInputs[i]->CallWASDAction(horizontal, vertical);
    }
}
