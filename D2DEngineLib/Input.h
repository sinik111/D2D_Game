#pragma once

class Input
{
public:
	// Ű�� ������ �������� Ȯ��
	static bool IsKeyDown(int vKey);

	// �̹� ������ Ű�� ���������� Ȯ��
	static bool IsKeyPressed(int vKey);

	// �̹� ������ Ű�� ���������� Ȯ��
	static bool IsKeyReleased(int vKey);

	static POINT GetCursorPoint();
};