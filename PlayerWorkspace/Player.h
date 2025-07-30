#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

class RigidBody2D;

enum class PlayerDirection
{
	LeftUp,
	Up,
	RightUp,
	Right,
	RightDown,
	Down,
	LeftDown,
	Left,
	Max
};

class Player :
	public Script
{
private:
	RigidBody2D* m_rigidBody2d = nullptr;

	FSM m_playerFSM;
	FSMContext m_fsmContext;

	float m_moveSpeed = 100.0f;
	float m_dashSpeed = 400.0f;
	float m_dodgeDistance = 300.0f;
	float m_dodgeSpeed = 800.0f;

	float m_dodgeAvailableTime = 0.2f;

private:
	void Initialize() override;
	void Start() override;
	void FixedUpdate() override;

private:
	void SetDirectionInput(Vector2 input);
	void DashKeyPressed();
	void DashKeyReleased();

public:
	static Vector2 CalculateDirectionVector(PlayerDirection direction);
	static Vector2 CalculateDirectionVector(int direction);
};