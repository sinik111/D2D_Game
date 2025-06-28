#pragma once

#include <bitset>

class PlayerInput;

class PlayerInputSystem
{
private:
	std::vector<PlayerInput*> m_playerInputs;

	HWND m_hWnd = nullptr;
	std::bitset<256> m_currentKeyState;
	std::bitset<256> m_previousKeyState;
	POINT m_mousePoint{};

public:
	void Register(PlayerInput* playerInput);
	void Unregister(PlayerInput* playerInput);

public:
	void SetWindow(HWND hWnd);

	// �� �������� ȣ�� �ʿ�
	void Update();

	// Ű�� ������ �������� Ȯ��
	bool IsKeyHeld(int vkey) const;

	// �̹� ������ Ű�� ���������� Ȯ��
	bool IsKeyPressed(int vkey) const;

	// �̹� ������ Ű�� ���������� Ȯ��
	bool IsKeyReleased(int vkey) const;

	POINT GetCursorPoint() const;

public:
	void ProcessInput() const;
	
private:
	void ProcessArrowInput() const;
	void ProcessWASDInput() const;
};