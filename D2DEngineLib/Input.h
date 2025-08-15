#pragma once

class Input
{
public:
	// Ű�� ������ �������� Ȯ��
	static bool IsKeyHeld(int vKey);

	// �̹� ������ Ű�� ���������� Ȯ��
	static bool IsKeyPressed(int vKey);

	// �̹� ������ Ű�� ���������� Ȯ��
	static bool IsKeyReleased(int vKey);

	static bool IsAnyKeyHeld();
	static bool IsAnyKeyPressed();
	static bool IsAnyKeyReleased();

	static POINT GetCursorPoint();
};