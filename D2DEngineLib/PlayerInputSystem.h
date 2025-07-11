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

	// 매 루프마다 호출 필요
	void Update();

	// 키가 눌려진 상태인지 확인
	bool IsKeyHeld(int vkey) const;

	// 이번 루프에 키가 눌려졌는지 확인
	bool IsKeyPressed(int vkey) const;

	// 이번 루프에 키가 놓아졌는지 확인
	bool IsKeyReleased(int vkey) const;

	POINT GetCursorPoint() const;

public:
	void ProcessInput() const;
	
private:
	void ProcessArrowInput() const;
	void ProcessWASDInput() const;
};