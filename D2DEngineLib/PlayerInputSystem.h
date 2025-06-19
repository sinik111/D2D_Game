#pragma once

#include <bitset>

class PlayerInput;

class PlayerInputSystem
{
private:
	std::vector<PlayerInput*> m_playerInputs;

	HWND m_hWnd;
	std::bitset<256> m_currentKeyState;
	std::bitset<256> m_previousKeyState;
	POINT m_mousePoint;

public:
	PlayerInputSystem();
	~PlayerInputSystem() = default;

public:
	void Register(PlayerInput* playerInput);
	void Unregister(PlayerInput* playerInput);

public:
	void SetWindow(HWND hWnd);

	// �� �������� ȣ�� �ʿ�
	void Update();

	// Ű�� ������ �������� Ȯ��
	bool IsKeyDown(int vkey);

	// �̹� ������ Ű�� ���������� Ȯ��
	bool IsKeyPressed(int vkey);

	// �̹� ������ Ű�� ���������� Ȯ��
	bool IsKeyReleased(int vkey);

	POINT GetCursorPoint();

public:
	void ProcessInput();
	
private:
	void ProcessArrowInput();
	void ProcessWASDInput();
};