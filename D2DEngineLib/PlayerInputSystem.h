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
	bool m_currentAnyKeyState = false;
	bool m_previousAnyKeyState = false;

	bool m_IsStopped = false;
public:
	void Register(PlayerInput* playerInput);
	void Unregister(PlayerInput* playerInput);

public:
	void SetWindow(HWND hWnd);

	// 매 루프마다 호출 필요
	void Update();

	bool IsKeyHeld(int vkey) const;
	bool IsKeyPressed(int vkey) const;
	bool IsKeyReleased(int vkey) const;

	bool IsAnyKeyHeld() const;
	bool IsAnyKeyPressed() const;
	bool IsAnyKeyReleased() const;

	POINT GetCursorPoint() const;

public:
	void ProcessInput() const;
	
private:
	void ProcessArrowInput() const;
	void ProcessWASDInput() const;

public:
	void SetIsStopped(bool value);
	bool GetIsStopped() const;
};