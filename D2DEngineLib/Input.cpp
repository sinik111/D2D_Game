#include "pch.h"
#include "Input.h"


Input::Input()
    :m_hWnd{ nullptr }, m_MousePoint{}
{

}

Input& Input::Get()
{
    static Input s_input;

    return s_input;
}

void Input::SetWindow(HWND hWnd)
{
    Get().m_hWnd = hWnd;
}

void Input::Update()
{
    Get().UpdateInputStates();
}

bool Input::IsKeyDown(int vkey)
{
    return Get().m_CurrentKeyState[vkey];
}

bool Input::IsKeyPressed(int vkey)
{
    return !Get().m_PreviousKeyState[vkey] && Get().m_CurrentKeyState[vkey];
}

bool Input::IsKeyReleased(int vkey)
{
    return Get().m_PreviousKeyState[vkey] && !Get().m_CurrentKeyState[vkey];
}

Vector2 Input::GetCursorPosition()
{
    return Vector2((float)Get().m_MousePoint.x, (float)Get().m_MousePoint.y);
}

void Input::UpdateInputStates()
{
    m_PreviousKeyState = m_CurrentKeyState;

    for (int i = 0; i < 256; ++i)
    {
        m_CurrentKeyState[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }

    GetCursorPos(&m_MousePoint);

    ScreenToClient(m_hWnd, &m_MousePoint);
}
