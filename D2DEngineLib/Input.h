#pragma once

class Input
{
public:
	// 키가 눌려진 상태인지 확인
	static bool IsKeyHeld(int vKey);

	// 이번 루프에 키가 눌려졌는지 확인
	static bool IsKeyPressed(int vKey);

	// 이번 루프에 키가 놓아졌는지 확인
	static bool IsKeyReleased(int vKey);

	static POINT GetCursorPoint();
};