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

	// 매 루프마다 호출 필요
	static void Update();

	// 키가 눌려진 상태인지 확인
	static bool IsKeyDown(int vkey);

	// 이번 루프에 키가 눌려졌는지 확인
	static bool IsKeyPressed(int vkey);

	// 이번 루프에 키가 놓아졌는지 확인
	static bool IsKeyReleased(int vkey);

	static Vector2 GetCursorPosition();

private:
	void UpdateInputStates();
};