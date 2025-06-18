#pragma once

#include <bitset>

#include "Vector2.h"

class Input
{
private:
	HWND m_hWnd;
	std::bitset<256> m_CurrentKeyState;
	std::bitset<256> m_PreviousKeyState;
	POINT m_MousePoint;

private:
	Input();
	~Input() = default;

private:
	static Input& Get();

public:
	static void SetWindow(HWND hWnd);

	// �� �������� ȣ�� �ʿ�
	static void Update();

	// Ű�� ������ �������� Ȯ��
	static bool IsKeyDown(int vkey);

	// �̹� ������ Ű�� ���������� Ȯ��
	static bool IsKeyPressed(int vkey);

	// �̹� ������ Ű�� ���������� Ȯ��
	static bool IsKeyReleased(int vkey);

	static Vector2 GetCursorPosition();

private:
	void UpdateInputStates();
};